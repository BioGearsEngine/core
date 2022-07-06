//**********************************************************************************
// Copyright 2015 Applied Research Associates, Inc.
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use
// this file except in compliance with the License.You may obtain a copy of the License
// at :
// http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software distributed under
// the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied.See the License for the
// specific language governing permissions and limitations under the License.
//**************************************************************************************

#include "Driver.h"

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <system_error>

#if defined(BIOGEARS_SUBPROCESS_SUPPORT)
#define WIN32_LEAN_AND_MEAN
#include <boost/asio.hpp>
#include <boost/date_time.hpp>
#include <boost/process.hpp>
#include <boost/process/exception.hpp>
#endif
#undef ERROR

#include "../utils/Executor.h"

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenario.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenarioExec.h>
#include <biogears/io/io-manager.h>

#ifdef CMD_BIO_SUPPORT_CIRCUIT_TEST
#include <biogears/cdm/test/CommonDataModelTest.h>
#include <biogears/engine/test/BioGearsEngineTest.h>
#endif

#ifdef BIOGEARS_IO_PRESENT
#include <biogears/io/directories/patients.h>
#ifdef BIOGEARS_IO_EMBED_STATES
#include <biogears/io/directories/states.h>
#endif
#endif

#include "../utils/Config.h"
#include "../utils/Executor.h"
#include <biogears/filesystem/path.h>
#include <biogears/schema/cdm/Scenario.hxx>
#include <biogears/string/manipulation.h>
#include <xsd/cxx/tree/exceptions.hxx>

#if defined(BIOGEARS_SUBPROCESS_SUPPORT)
inline std::string fmt_localtime()
{
  using namespace boost::posix_time;
  static std::locale loc(std::cout.getloc(),
                         new time_facet("%H:%M"));

  std::stringstream ss;
  ss.imbue(loc);
  ss << boost::posix_time::second_clock::local_time();
  ;
  return ss.str();
}

auto cout_loop(boost::process::async_pipe& p, boost::asio::mutable_buffer buf) -> void
{
  p.async_read_some(buf, [&p, buf](std::error_code ec, size_t n) {
    std::cout.write(boost::asio::buffer_cast<char const*>(buf), n);
    if (!ec)
      cout_loop(p, buf);
  });
}

void cerr_loop(boost::process::async_pipe& p, boost::asio::mutable_buffer buf)
{
  p.async_read_some(buf, [&p, buf](std::error_code ec, size_t n) {
    std::cerr.write(boost::asio::buffer_cast<char const*>(buf), n);
    if (!ec)
      cerr_loop(p, buf);
  });
}
#endif

namespace biogears {
Driver::Driver(char* exe_name, size_t thread_count)
  : _pool { thread_count }
  , _jobs(thread_count)
  , _thread_count(0)
  , _process_count(0)
  , _total_work(0)
{
  biogears::filesystem::path p { exe_name };
  _relative_path = p.parent_path();
}
//-----------------------------------------------------------------------------
Driver::~Driver()
{
  _pool.stop();
  _pool.join();
  if (_join_thread.joinable()) {
    _join_thread.join();
  }
}
//-----------------------------------------------------------------------------
void Driver::configure(const Config& runs)
{
  _globals = runs;
  _globals.clear();
}
//-----------------------------------------------------------------------------
void Driver::queue(const Config& runs, bool as_subprocess)
{
  for (auto& exec : runs) {
    switch (exec.Driver()) {
    case EDriver::Undefined:
      std::cerr << "Unable to queue Undefined Executor for " << exec.Name() << "\n";
      continue;
    case EDriver::BGEUnitTestDriver:
      queue_BGEUnitTest(exec, as_subprocess);
      break;
    case EDriver::CDMUnitTestDriver:
      queue_CDMUnitTest(exec, as_subprocess);
      break;
    case EDriver::ScenarioTestDriver:
      queue_Scenario(exec, as_subprocess);
      break;
    default:
      std::cerr << "Unsupported Driver type " << exec.Driver() << " please update your biogears libraries \n";
      continue;
    }
  }
}
//-----------------------------------------------------------------------------
void Driver::run()
{
  _start_time = std::chrono::steady_clock().now();
  _pool.start();
}
//-----------------------------------------------------------------------------
void Driver::stop()
{
  _pool.stop();
}
//-----------------------------------------------------------------------------
bool Driver::stop_if_empty()
{
  return _pool.stop_if_empty();
}
//-----------------------------------------------------------------------------
void Driver::stop_when_empty()
{
  size_t count;
  std::chrono::seconds duration;
  std::chrono::minutes minutes;
  std::chrono::hours hours;
  std::string process_s;

  while (!_pool.stop_if_empty()) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    count = _total_work - _pool.get_sink()->unsafe_size();
    duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock().now() - _start_time);
    hours = std::chrono::duration_cast<std::chrono::hours>(duration);
    minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);

    process_s = process_str();
    if (duration < std::chrono::hours(1)) {
      std::cout << asprintf("\r%sProgress %d/%d; Elapsed Time  %dm%ds\u001b[0K\r", process_s.c_str(), count, _total_work, minutes.count(), duration.count() % 60);
      std::cout << std::flush;
    } else {
      std::cout << asprintf("\r%sProgress %d/%d; Elapsed Time  %dh%dm%ds\u001b[0K\r", process_s.c_str(), count, _total_work, hours.count(), minutes.count() % 60, duration.count() % 60) << std::flush;
    }
  }
}
//-----------------------------------------------------------------------------
void Driver::join()
{
  _join_thread = std::thread([&]() { _pool.join(); });

  size_t count;
  std::chrono::seconds duration;
  std::chrono::minutes minutes;
  std::chrono::hours hours;
  std::string process_s;

  while (!_pool.joined()) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    count = _total_work; // Work queue is empty so we must of completed the total work
    duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock().now() - _start_time);
    hours = std::chrono::duration_cast<std::chrono::hours>(duration);
    minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);

    process_s = process_str();
    if (duration < std::chrono::hours(1)) {
      std::cout << asprintf("\r%sProgress %d/%d; Elapsed Time  %dm%ds\u001b[0K\r", process_s.c_str(), count, _total_work, minutes.count(), duration.count() % 60) << std::flush;
    } else {
      std::cout << asprintf("\r%sProgress %d/%d; Elapsed Time  %dh%dm%ds\u001b[0K\r", process_s.c_str(), count, _total_work, hours.count(), minutes.count() % 60, duration.count() % 60) << std::flush;
    }
  }
  if (_join_thread.joinable()) {
    _join_thread.join();
  }
  count = _total_work; // Work queue is empty so we must of completed the total work
  duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock().now() - _start_time);
  hours = std::chrono::duration_cast<std::chrono::hours>(duration);
  minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);

  if (duration < std::chrono::hours(1)) {
    std::cout << asprintf("\rProgress %d/%d; Elapsed Time  %dm%ds\u001b[0K", count, _total_work, minutes.count(), duration.count() % 60) << std::flush;
  } else {
    std::cout << asprintf("\rProgress %d/%d; Elapsed Time  %dh%dm%ds\u001b[0K", count, _total_work, hours.count(), minutes.count() % 60, duration.count() % 60) << std::flush;
  }
}
//-----------------------------------------------------------------------------
void Driver::queue_BGEUnitTest(Executor exec, bool as_subprocess)
{
  // TODO: Add Subprocess suport for BGEUnitTest by porting test_driver to bg-unittest
#ifdef CMD_BIO_SUPPORT_CIRCUIT_TEST
  _pool.queue_work(
    [=]() {
      BioGearsEngineTest* executor;
      try {
        executor = new BioGearsEngineTest;
      } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return 1;
      }
      executor->RunTest(exec.Name(), exec.Computed());
      delete executor;
      return 0;
    });
  ++_total_work;
#endif
}
//-----------------------------------------------------------------------------
void Driver::queue_CDMUnitTest(Executor exec, bool as_subprocess)
{
  // TODO: Add Subprocess suport for BGEUnitTest by porting test_driver to bg-unittest
#ifdef CMD_BIO_SUPPORT_CIRCUIT_TEST
  _pool.queue_work(
    [&]() {
      CommonDataModelTest* executor;
      try {
        executor = new CommonDataModelTest;
      } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return 1;
      }
      executor->RunTest(exec.Name(), exec.Computed());
      delete executor;
      return 0;
    });
  ++_total_work;
#endif
}
//-----------------------------------------------------------------------------
void Driver::queue_Scenario(Executor exec, bool as_subprocess)
{
  using namespace std::placeholders;
  enum class PatientType {
    FILE,
    STATE,
    INLINE
  };

  std::function<void(Executor, bool)> scenario_launch;

#if defined(BIOGEARS_SUBPROCESS_SUPPORT)
  if (as_subprocess) {
    scenario_launch = [&](Executor ex, bool b) { this->subprocess_execute(ex, b); };
  } else {
    scenario_launch = [&](Executor ex, bool b) { this->async_execute(ex, b); };
  }
#else
  scenario_launch = [&](Executor ex, bool b) { this->async_execute(ex, b); };
#endif

  biogears::IOManager io {};
  biogears::Logger logger { "", io };

  filesystem::path resolved_filepath = io.FindScenarioFile(exec.Scenario().c_str());
  using biogears::filesystem::path;
  using mil::tatrc::physiology::datamodel::ScenarioData;
  std::unique_ptr<ScenarioData> scenario;
  std::unique_ptr<CDM::ObjectData> obj;
  try {
    if (resolved_filepath.empty()) {
#ifdef BIOGEARS_IO_PRESENT
      size_t content_size;
      auto content = io.get_embedded_resource_file(exec.Scenario().c_str(), content_size);
      if (content_size == 0) {
        std::cerr << "Failed to open Scenarios/" << exec.Scenario() << " skipping\n";
        return;
      }
      obj = Serializer::ReadBuffer((XMLByte*)content, content_size, &logger);
#endif
    } else {
      std::cout << "Reading " << exec.Scenario() << std::endl;
      obj = Serializer::ReadFile(resolved_filepath,
                                 &logger);
    }
    scenario.reset(reinterpret_cast<ScenarioData*>(obj.release()));
    if (scenario == nullptr) {
      throw std::runtime_error(exec.Scenario() + " is not a valid Scenario file.");
    }
  } catch (std::runtime_error e) {
    std::cout << "Error while processing " << exec.Scenario() << "\n";
    std::cout << e.what() << "\n\n";
    return;
  } catch (xsd::cxx::tree::parsing<char> e) {
    std::cout << "Error while processing " << exec.Scenario() << "\n";
    std::cout << e << "\n\n";
    return;
  }

  // The Biogears Schema states that ScenarioData supports the following boot strap tags
  // 1. EngineStateFile -- Overrides PatientFile and Patient tag and skips initialization using the state file
  // 2. PatientFile -- External file which will be read in with a patient definition
  // 3. Patient -- Inline Patient definition similar to PatientFile
  // Currently only Patient File is supported by this function
  // TODO: Test for EngineState file and call an appropriate launcher
  // TODO: Test for Patient if PatientFile is not present and call the appropriate launcher
  if (scenario->EngineStateFile().present()) {
    const auto state_file = scenario->EngineStateFile().get();
    std::string nc_state_file = state_file;
    std::transform(nc_state_file.begin(), nc_state_file.end(), nc_state_file.begin(), ::tolower);
    if ("all" == nc_state_file) {
      auto state_files = biogears::ListFiles("states", R"(.*\.xml)", false);
#if BIOGEARS_IO_PRESENT && BIOGEARS_IO_EMBED_STATES
      if (state_files.size() == 0) {
        auto states = biogears::io::list_patients_files();
        state_files = std::vector<biogears::filesystem::path> { states, states + biogears::io::patients_file_count() };
      }
#endif
      auto infection_state_files = biogears::ListFiles("states/InfectionStates", R"(.*\.xml)", false);
      state_files.insert(state_files.end(), infection_state_files.begin(), infection_state_files.end());
      queue_from_sate_files(exec, state_files, scenario_launch);
      return;
    } else if (filesystem::exists(scenario->EngineStateFile().get())) {
      if (filesystem::is_directory(scenario->EngineStateFile().get())) {
        auto state_files = biogears::ListFiles(scenario->EngineStateFile().get(), R"(.*\.xml)", false);
        queue_from_sate_files(exec, state_files, scenario_launch);
        return;
      } else {
        exec.State(scenario->EngineStateFile().get());
        _pool.queue_work(std::bind(scenario_launch, std::move(exec), false));
        ++_total_work;
        return;
      }
    } else if (filesystem::exists("states/" + scenario->EngineStateFile().get())) {
      if (filesystem::is_directory("states/" + scenario->EngineStateFile().get())) {
        auto state_files = biogears::ListFiles("states/" + scenario->EngineStateFile().get(), R"(.*\.xml)", false);
        queue_from_sate_files(exec, state_files, scenario_launch);
        return;
      } else {
        exec.State(scenario->EngineStateFile().get());
        _pool.queue_work(std::bind(scenario_launch, std::move(exec), false));
        ++_total_work;
        return;
      }
    } else {
      auto matching_states = find_matching_files(scenario->EngineStateFile().get());
      if (matching_states.empty()) {
        matching_states = find_matching_files("states/" + scenario->EngineStateFile().get());
      }
      queue_from_sate_files(exec, matching_states, scenario_launch);
    }

  } else if (scenario->InitialParameters().present() && scenario->InitialParameters()->PatientFile().present()) {
    const auto patient_file = scenario->InitialParameters()->PatientFile().get();
    std::string nc_patient_file = patient_file;
    std::transform(nc_patient_file.begin(), nc_patient_file.end(), nc_patient_file.begin(), ::tolower);
    if ("all" == nc_patient_file) {
      auto patient_files = biogears::ListFiles("patients", R"(.*\.xml)", false);
#if BIOGEARS_IO_PRESENT
      if (patient_files.size() == 0) {
        auto patients = biogears::io::list_patients_files();
        patient_files = std::vector<biogears::filesystem::path> { patients, patients + biogears::io::patients_file_count() };
      }
#endif
      queue_from_patient_files(exec, patient_files, scenario_launch);
    } else if (filesystem::exists(scenario->InitialParameters()->PatientFile().get())) {
      if (filesystem::is_directory(scenario->InitialParameters()->PatientFile().get())) {
        auto patient_files = biogears::ListFiles(scenario->InitialParameters()->PatientFile().get(), R"(.*\.xml)", false);
        queue_from_patient_files(exec, patient_files, scenario_launch);
        return;
      } else {
        exec.Patient(patient_file);
        _pool.queue_work(std::bind(scenario_launch, std::move(exec), false));
        ++_total_work;
        return;
      }
    } else if (filesystem::exists("patients/" + scenario->InitialParameters()->PatientFile().get())) {
      if (filesystem::is_directory("patients/" + scenario->InitialParameters()->PatientFile().get())) {
        auto patient_files = biogears::ListFiles("patients/" + scenario->InitialParameters()->PatientFile().get(), R"(.*\.xml)", false);
        queue_from_patient_files(exec, patient_files, scenario_launch);
        return;
      } else {
        exec.Patient("patients/" + patient_file);
        _pool.queue_work(std::bind(scenario_launch, std::move(exec), false));
        ++_total_work;
        return;
      }
    } else {
      auto matching_patients = find_matching_files(scenario->InitialParameters()->PatientFile().get());
      if (matching_patients.empty()) {
        matching_patients = find_matching_files("patients/" + scenario->InitialParameters()->PatientFile().get());
      }
      queue_from_patient_files(exec, matching_patients, scenario_launch);
    }
  } else if (scenario->InitialParameters().present() && scenario->InitialParameters()->Patient().present()) {
    exec.Patient("");
    exec.State("");
    _pool.queue_work(std::bind(scenario_launch, std::move(exec), false));
    ++_total_work;
  } else {
    std::cout << "Skipping " << exec.Name() << " no patient specified.\n";
    return;
  }
}
//-----------------------------------------------------------------------------
void Driver::queue_from_sate_files(const Executor& exec, const std::vector<filesystem::path>& state_files, std::function<void(Executor, bool)> scenario_launch_func)
{
  for (auto& state_file : state_files) {
    Executor stateEx { exec };
    stateEx.State(state_file);
    std::string trimmed_state_path(trim(stateEx.State()));
    auto split_state_path = filesystem::path(trimmed_state_path);
    auto state_no_extension = split(split_state_path.back(), '.').front();
    std::string trimmed_scenario_path(trim(stateEx.Scenario()));
    auto split_scenario_path = filesystem::path(trimmed_scenario_path);
    auto scenario_no_extension = split(split_scenario_path.back(), '.').front();

    if (stateEx.Name().empty()) {
      stateEx.Results({ scenario_no_extension + "-" + state_no_extension });
      stateEx.Name(scenario_no_extension + "-" + state_no_extension);
    } else {
      auto patient_name = split(stateEx.Name(), '/').back();
      std::size_t found = patient_name.find_last_of(".");
      stateEx.Results({ patient_name.substr(0, found) + "-" + state_no_extension });
      stateEx.Name(patient_name.substr(0, found) + "-" + state_no_extension);
    }
    ///----
    _pool.queue_work(std::bind(scenario_launch_func, std::move(stateEx), true));
    ++_total_work;
  }
}
//-----------------------------------------------------------------------------
void Driver::queue_from_patient_files(const Executor& exec, const std::vector<filesystem::path>& patient_files, std::function<void(Executor, bool)> scenario_launch_func)
{
  for (auto& patient_file : patient_files) {
    std::cout << "\t" << patient_file << std::endl;
    Executor patientEx { exec };
    patientEx.Patient(patient_file);

    ///----
    std::string trimmed_patient_path(trim(patientEx.Patient()));
    auto split_patient_path = split(trimmed_patient_path, '/');
    auto patient_no_extension = split(split_patient_path.back(), '.').front();

    std::string trimmed_scenario_path(trim(patientEx.Scenario()));
    auto split_scenario_path = split(trimmed_scenario_path, '/');
    auto scenario_no_extension = split(split_scenario_path.back(), '.').front();

    if (patientEx.Name().empty()) {
      patientEx.Results({ scenario_no_extension + "-" + patient_no_extension });
      patientEx.Name(scenario_no_extension + "-" + patient_no_extension);
    } else {
      auto patient_name = split(patientEx.Name(), '/').back();
      std::size_t found = patient_name.find_last_of(".");
      patientEx.Results({ patient_name.substr(0, found) + "-" + patient_no_extension });
      patientEx.Name(patient_name.substr(0, found) + "-" + patient_no_extension);
    }
    ///----
    _pool.queue_work(std::bind(scenario_launch_func, std::move(patientEx), true));
    ++_total_work;
  }
}
//-----------------------------------------------------------------------------
//!
//!  \input std::string : Regex for finding files
//!  \return std::vector<std::string> list of all paths that matched the regex
//!
//!

std::vector<filesystem::path> Driver::find_matching_files(const std::string& pattern)
{

  //
  // Attempt to handle Regex based matching
  // Example   Pattern  = "states/*/Billy*.xml"
  //
  // Steps:
  // Insert first path segment in possible_paths
  // Foreach segment
  // Foreach possible_paths
  //   Does possible_path/segment exists
  //   If so possible_path = possible_path/segment
  //   Else  Assume segment is a Regex and List all matches of possible_path/segment_regex
  //         Remove current possible_path
  //          Append result of Regex Step
  // For each dir in possible_paths queue all *.xml files
  // For each file in possible_paths queue file
  // Queue Executors

  auto path_parts = split(pattern, '/');

  std::vector<filesystem::path> possible_paths { "" };
  std::vector<filesystem::path> new_work;

  std::vector<int> remove_queue;
  remove_queue.reserve(possible_paths.size());

  int index;
  for (auto& part : path_parts) {
    index = 0;
    for (auto& path : possible_paths) {
      auto temp = filesystem::path(path) / part;
      if (temp.exists()) {
        path = temp;
      } else {
        auto work = ListFiles(path, part, false);
        new_work.insert(new_work.begin(), work.begin(), work.end());
        remove_queue.push_back(index);
      }
      ++index;
    }

    auto end = possible_paths.end();
    for (auto& index : remove_queue) {
      std::swap(possible_paths[index], possible_paths.back());
      end = end - 1;
    }
    if (end != possible_paths.end()) {
      end = possible_paths.erase(end, possible_paths.end());
    }
    if (new_work.size()) {
      possible_paths.insert(possible_paths.end(), new_work.begin(), new_work.end());
    }
    new_work.clear();
    remove_queue.clear();
  }
  return possible_paths;
}

//-----------------------------------------------------------------------------
#if defined(BIOGEARS_SUBPROCESS_SUPPORT)
void Driver::subprocess_execute(biogears::Executor& ex, bool multi_patient_run)
{
  using namespace biogears;
  _process_count += 1;

  try {
    using namespace boost::process;

    std::string options;

    options = "--quiet";

    if (ex.Name().size()) {
      auto name = biogears::trim(ex.Name());
      if (name.front() == '\'' && name.back() == '\'') {
        name = biogears::trim(name, "'");
      }
      if (name.front() == '\"' && name.back() == '\"') {
        name = biogears::trim(name, "\"");
      }
      options += biogears::asprintf(" --name \"%s\"", name.c_str());
    }

    if (ex.Driver() != EDriver::ScenarioTestDriver) {
      options += " --driver ";
      switch (ex.Driver()) {
      case EDriver::ScenarioTestDriver:
        options += "'ScenarioTestDriver'";
        break;
      case EDriver::BGEUnitTestDriver:
        options += "'BGEUnitTestDriver'";
        break;
      case EDriver::CDMUnitTestDriver:
        options += "'CDMUnitTestDriver'";
        break;
      }
    }

    if (ex.Patient().size()) {
      auto patient = biogears::trim(ex.Patient());
      if (patient.front() == '\'' && patient.back() == '\'') {
        patient = biogears::trim(patient, "'");
      }
      if (patient.front() == '\"' && patient.back() == '\"') {
        patient = biogears::trim(patient, "\"");
      }
      options += biogears::asprintf(" --patient \"%s\"", patient.c_str());
    }

    if (ex.State().size()) {
      auto state = biogears::trim(ex.State());
      if (state.front() == '\'' && state.back() == '\'') {
        state = biogears::trim(state, "'");
      }
      if (state.front() == '\"' && state.back() == '\"') {
        state = biogears::trim(state, "\"");
      }
      options += biogears::asprintf(" --state \"%s\"", state.c_str());
    }

    if (ex.Scenario().size()) {
      auto scenario = biogears::trim(ex.Scenario());
      if (scenario.front() == '\'' && scenario.back() == '\'') {
        scenario = biogears::trim(scenario, "'");
      }
      if (scenario.front() == '\"' && scenario.back() == '\"') {
        scenario = biogears::trim(scenario, "\"");
      }
      options += biogears::asprintf(" --scenario \"%s\"", scenario.c_str());
    }

    if (ex.Results().size()) {
      auto results = biogears::trim(ex.Results()[0]);
      if (results.front() == '\'' && results.back() == '\'') {
        results = biogears::trim(results, "'");
      }
      if (results.front() == '\"' && results.back() == '\"') {
        results = biogears::trim(results, "\"");
      }
      options += biogears::asprintf(" --results \"%s\"", results.c_str());
    }

    if (ex.Group().size()) {
      auto group = biogears::trim(ex.Group());
      if (group.front() == '\'' && group.back() == '\'') {
        group = biogears::trim(group, "'");
      }
      if (group.front() == '\"' && group.back() == '\"') {
        group = biogears::trim(group, "\"");
      }
      options += biogears::asprintf(" --group \"%s\"", group.c_str());
    }

    if (ex.TrackStabilization()) {
      options += biogears::asprintf(" --track-stabilization");
    }

    boost::asio::io_service svc;
    async_pipe out_stream(svc);
    async_pipe err_stream(svc);
    char out_buffer[128];
    char err_buffer[128];

    std::cout << asprintf("[%s] Starting %s\n", fmt_localtime().c_str(), ex.Name().c_str()) << std::flush;
    child c;
    if (boost::filesystem::exists(_relative_path)) {
      c = child((boost::filesystem::path(_relative_path) / ("/bg-scenario" DEBUG_POSTFIX " ")).string() + options, std_out > out_stream, std_err > err_stream);
    } else {
      c = child("bg-scenario " + options, std_out > out_stream, std_err > err_stream);
    }

    cout_loop(out_stream, buffer(out_buffer));
    cerr_loop(err_stream, buffer(err_buffer));

    svc.run();
    c.wait();
    auto code = c.exit_code();
    switch (code) {
    case ExecutionErrors::NONE:
      std::cout << asprintf("[%s] Completed %s\n", fmt_localtime().c_str(), ex.Name().c_str()) << std::flush;
      break;
    case ExecutionErrors::ARGUMENT_ERROR:
      std::cerr << asprintf("[%s] Error-%d: %s failed with arguments %s\n", fmt_localtime().c_str(), code, ex.Name().c_str(), options.c_str()) << std::flush;
      break;
    case ExecutionErrors::SCENARIO_IO_ERROR:
      std::cerr << asprintf("[%s] Error-%d: %s failed to find the specified scenario file %s\n", fmt_localtime().c_str(), code, ex.Name().c_str(), ex.Scenario().c_str()) << std::flush;
      break;
    case ExecutionErrors::SCENARIO_PARSE_ERROR:
      std::cerr << asprintf("[%s] Error-%d: %s failed parse the specified scenario file %s\n", fmt_localtime().c_str(), code, ex.Name().c_str(), ex.Scenario().c_str()) << std::flush;
      break;
    case ExecutionErrors::PATIENT_IO_ERROR:
      std::cerr << asprintf("[%s] Error-%d: %s failed to find the specified patient file %s\n", fmt_localtime().c_str(), code, ex.Name().c_str(), ex.Patient().c_str()) << std::flush;
      break;
    case ExecutionErrors::PATIENT_PARSE_ERROR:
      std::cerr << asprintf("[%s] Error-%d: %s failed to parse the specified patient file %s\n", fmt_localtime().c_str(), code, ex.Name().c_str(), ex.Patient().c_str()) << std::flush;
      break;
    case ExecutionErrors::BIOGEARS_RUNTIME_ERROR:
      std::cout << asprintf("[%s] Error-%d: %s failed mid simulation see log for more details.\n", fmt_localtime().c_str(), code, ex.Name().c_str()) << std::flush;
      break;
    default:
      std::cerr << asprintf("[%s] %s failed see log for details.\n", fmt_localtime().c_str(), ex.Name().c_str()) << std::flush;
      break;
    }
    _process_count -= 1;
    return;
  } catch (boost::process::process_error e) {
    std::cerr << asprintf("[%s] Failed %s"
                          "\n",
                          fmt_localtime().c_str(), ex.Name().c_str());
    std::cerr << e.what() << std::flush;
    _process_count -= 1;
    return;
  } catch (...) {
    std::cerr << asprintf("[%s] Failed %s\n", fmt_localtime().c_str(), ex.Name().c_str()) << std::flush;
    _process_count -= 1;
    return;
  }
  _process_count -= 1;
  return;
}

#endif

void Driver::async_execute(biogears::Executor& ex, bool multi_patient_run)
{
  using namespace biogears;
  _thread_count += 1;
  std::string trimed_scenario_path(trim(ex.Scenario()));
  std::ifstream scenario_stream { trimed_scenario_path };
  if (!scenario_stream.is_open()) {
    trimed_scenario_path = "Scenarios/" + trimed_scenario_path;
  }
  scenario_stream.close();

  auto split_scenario_path = filesystem::path(trimed_scenario_path);
  auto scenario_no_extension = split(split_scenario_path.back(), '.').front();

  std::string trimed_patient_path(trim(ex.Patient()));
  auto split_patient_path = filesystem::path(trimed_patient_path);
  auto patient_no_extension = split(split_patient_path.back(), '.').front();

  // NOTE: This loses non relative prefixes as the split will eat the leading path_separator
  filesystem::path parent_dir = split_scenario_path.parent_path();

  if (multi_patient_run) {
    ex.Name(ex.Name() + "-" + patient_no_extension);
  }

  std::string base_file_name = (multi_patient_run) ? scenario_no_extension + "-" + patient_no_extension : scenario_no_extension;
  std::string console_file = base_file_name + ".log";
  std::string log_file = base_file_name + "Results.log";
  std::string results_file = base_file_name + "Results.csv";

  std::unique_ptr<BioGearsEngine> eng;
  Logger console_logger;
  if (filesystem::path(ex.Computed()) <= parent_dir) {
    ex.Computed("");
  }
  filesystem::path logfilepath = filesystem::path(ex.Computed()) / parent_dir / console_file;
  Logger file_logger(logfilepath);
  try {
    file_logger.SetConsoleLogLevel(Logger::eWarning);
    file_logger.SetConsoleConversionPattern("[{%H:%M}] " + ex.Name() + " <:priority:> :message::endline:");
    console_logger.SetConsoleConversionPattern("[{%H:%M}] :message::endline:");
    console_logger.FormatMessages(true);

    eng = std::make_unique<BioGearsEngine>(&file_logger);
  } catch (std::exception e) {
    std::cout << e.what();
    _thread_count -= 1;
    return;
  }

  if (ex.TrackStabilization()) {
    std::cout << "Checking the function\n";
    eng->SetTrackStabilizationFlag(true);
  }

  BioGearsScenario sce(eng->GetSubstanceManager());
  if (!sce.Load(trim(trimed_scenario_path))) {
    console_logger.Info(biogears::asprintf("Error[%d]: %s failed to find the specified scenario file %s", ExecutionErrors::SCENARIO_IO_ERROR, ex.Name().c_str(), ex.Scenario().c_str()));
  }

  if (!ex.Patient().empty()) {
    sce.GetInitialParameters().SetPatientFile(ex.Patient());
  } else if (!ex.Scenario().empty()) {
    sce.GetInitialParameters().SetPatientFile(ex.Scenario());
  } else {
    auto logger = eng->GetLogger();
    auto io = logger->GetIoManager().lock();

    filesystem::path resolved_filepath = io->FindScenarioFile(ex.Scenario().c_str());
    if (resolved_filepath.empty()) {
      console_logger.Info(biogears::asprintf("Error[%d]: %s failed to find the specified scenario file %s", ExecutionErrors::SCENARIO_IO_ERROR, ex.Name().c_str(), ex.Scenario().c_str()));
      _thread_count -= 1;
      return;
    }
    using biogears::filesystem::path;
    using mil::tatrc::physiology::datamodel::ScenarioData;
    std::unique_ptr<ScenarioData> scenario;
    try {
      std::cout << "Reading " << ex.Scenario() << std::endl;
      auto obj = Serializer::ReadFile(resolved_filepath,
                                      eng->GetLogger());
      scenario.reset(reinterpret_cast<ScenarioData*>(obj.release()));
      if (scenario == nullptr) {
        throw std::runtime_error(ex.Scenario() + " is not a valid Scenario file.");
      }
    } catch (std::runtime_error e) {
      std::cout << "Error while processing " << ex.Scenario() << "\n";
      std::cout << e.what() << "\n"
                << std::endl;
      console_logger.Info(biogears::asprintf("Error[%d]: %s failed parse the specified scenario file %s", ExecutionErrors::SCENARIO_PARSE_ERROR, ex.Name().c_str(), ex.Scenario().c_str()));
      _thread_count -= 1;
      return;
    } catch (xsd::cxx::tree::parsing<char> e) {
      std::cout << "Error while processing " << ex.Scenario() << "\n";
      std::cout << e << "\n"
                << std::endl;
      console_logger.Info(biogears::asprintf("Error[%d]: %s failed parse the specified scenario file %s", ExecutionErrors::SCENARIO_PARSE_ERROR, ex.Name().c_str(), ex.Scenario().c_str()));
      _thread_count -= 1;
      return;
    }

    biogears::SEPatient patient { sce.GetLogger() };
    ex.Patient(scenario->InitialParameters()->Patient().get().Name());
    patient.Load(scenario->InitialParameters()->Patient().get());
    sce.GetInitialParameters().SetPatient(patient);
  }

  console_logger.Info("Starting " + ex.Name());
  try {
    BioGearsScenarioExec bse { *eng };
    filesystem::path resultsFilePath = ex.Computed();
    resultsFilePath /= parent_dir / results_file;
    bse.Execute(sce, resultsFilePath, nullptr);
    console_logger.Info("Completed " + ex.Name());
  } catch (...) {
    console_logger.Error("Failed " + ex.Name());
    _thread_count -= 1;
    return;
  }
  _thread_count -= 1;
  return;
}
//-------------------------------------------------------------------------------
std::string Driver::process_str()
{
  std::stringstream ss;
  if (_thread_count != 0) {
    ss << _thread_count << " thread" << ((1 == _process_count) ? "" : "s") << ((_process_count != 0) ? "; " : " ");
  }
  if (_process_count != 0) {

    ss << _process_count << " process" << ((1 == _process_count) ? "" : "es") << "; ";
  }
  return ss.str();
}
} // NAMESPACE
