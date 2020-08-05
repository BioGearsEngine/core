//**********************************************************************************
//Copyright 2015 Applied Research Associates, Inc.
//Licensed under the Apache License, Version 2.0 (the "License"); you may not use
//this file except in compliance with the License.You may obtain a copy of the License
//at :
//http://www.apache.org/licenses/LICENSE-2.0
//Unless required by applicable law or agreed to in writing, software distributed under
//the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//CONDITIONS OF ANY KIND, either express or implied.See the License for the
//specific language governing permissions and limitations under the License.
//**************************************************************************************

#include "Driver.h"

#include <functional>
#include <iostream>
#include <system_error>

#if defined(BIOGEARS_SUBPROCESS_SUPPORT)
#include <boost/asio.hpp>
#include <boost/process.hpp>
#endif

#include "../utils/Executor.h"

#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenario.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenarioExec.h>

#ifdef CMD_BIO_SUPPORT_CIRCUIT_TEST
#include <biogears/cdm/test/CommonDataModelTest.h>
#include <biogears/engine/test/BioGearsEngineTest.h>
#endif

#include <biogears/schema/cdm/Scenario.hxx>
#include <biogears/string/manipulation.h>
#include <xsd/cxx/tree/exceptions.hxx>

#include "../utils/Config.h"
#include "../utils/Executor.h"

#if defined(BIOGEARS_SUBPROCESS_SUPPORT)

auto cout_loop(boost::process::async_pipe& p, boost::asio::mutable_buffer buf) -> void
{
  p.async_read_some(buf, [&p, buf](std::error_code ec, size_t n) {
    std::cout.write(boost::asio::buffer_cast<char const*>(buf), n);
    if (!ec)
      cout_loop(p, buf);
  });
}

#endif

#if defined(BIOGEARS_SUBPROCESS_SUPPORT)
void cerr_loop(boost::process::async_pipe& p, boost::asio::mutable_buffer buf)
{
  p.async_read_some(buf, [&p, buf](std::error_code ec, size_t n) {
    std::cerr.write(boost::asio::buffer_cast<char const*>(buf), n);
    if (!ec)
      cerr_loop(p, buf);
  });
}
#endif

#if defined(BIOGEARS_SUBPROCESS_SUPPORT)
void subprocess_execute(biogears::Executor& ex, bool multi_patient_run)
{
  using namespace biogears;
  Logger console_logger;
  try {
    console_logger.SetConsolesetConversionPattern("%d{%H:%M} [%p] %m%n");
    console_logger.FormatMessages(false);
  } catch (std::exception e) {
    std::cout << e.what();
  }
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

    boost::asio::io_service svc;
    async_pipe out_stream(svc);
    async_pipe err_stream(svc);
    char out_buffer[128];
    char err_buffer[128];

    //child c("bg-scenario " + options, (std_out & std_err) > out_stream, svc);
    //std::cout << "..\\outputs\\RelWithDebInfo\\bin\\bg-scenario " + options << std::endl;

    console_logger.Info("Starting " + ex.Name());

    child c("bg-scenario " + options, std_out > out_stream, std_err > err_stream);

    std::function<void(void)> test = [&]() { test(); };
    cout_loop(out_stream, buffer(out_buffer));
    cerr_loop(err_stream, buffer(err_buffer));

    svc.run();
    c.wait();
    auto code = c.exit_code();
    switch (code) {
    case ExecutionErrors::NONE:
      console_logger.Info("Completed " + ex.Name());
      return;
    case ExecutionErrors::ARGUMENT_ERROR:
      console_logger.Info(biogears::asprintf("Error[%d]: %s failed with arguments %s", code, ex.Name().c_str(), options.c_str()));
      return;
    case ExecutionErrors::SCENARIO_IO_ERROR:
      console_logger.Info(biogears::asprintf("Error[%d]: %s failed to find the specified scenario file %s", code, ex.Name().c_str(), ex.Scenario().c_str()));
      return;
    case ExecutionErrors::SCENARIO_PARSE_ERROR:
      console_logger.Info(biogears::asprintf("Error[%d]: %s failed parse the specified scenario file %s", code, ex.Name().c_str(), ex.Scenario().c_str()));
      return;
    case ExecutionErrors::PATIENT_IO_ERROR:
      console_logger.Info(biogears::asprintf("Error[%d]: %s failed to find the specified patient file %s", code, ex.Name().c_str(), ex.Patient().c_str()));
      return;
    case ExecutionErrors::PATIENT_PARSE_ERROR:
      console_logger.Info(biogears::asprintf("Error[%d]: %s failed to parse the specified patient file %s", code, ex.Name().c_str(), ex.Patient().c_str()));
      return;
    case ExecutionErrors::BIOGEARS_RUNTIME_ERROR:
      console_logger.Info(biogears::asprintf("Error[%d]: %s failed mid simulation see log for more details.", code, ex.Name().c_str()));
      return;
    default:
      console_logger.Info(ex.Name() + " failed see log for details.\n");
      return;
    }

  } catch (...) {
    console_logger.Error("Failed " + ex.Name() + "\n");
  }
}

#endif

void async_execute(biogears::Executor& ex, bool multi_patient_run)
{
  //auto scenario_launch = [](Executor& ex, bool multi_patient_run, PatientType patient_type, CDM::PatientData patient_data) {

  using namespace biogears;

  std::string trimed_scenario_path(trim(ex.Scenario()));
  auto split_scenario_path = split(trimed_scenario_path, '/');
  auto scenario_no_extension = split(split_scenario_path.back(), '.').front();

  std::string trimed_patient_path(trim(ex.Patient()));
  auto split_patient_path = split(trimed_patient_path, '/');
  auto patient_no_extension = split(split_patient_path.back(), '.').front();

  //NOTE: This loses non relative prefixes as the split will eat the leading path_separator
  std::string parent_dir;
  for (auto dir = split_scenario_path.begin(); dir != split_scenario_path.end(); ++dir) {
    if (dir + 1 != split_scenario_path.end()) {
      parent_dir.append(*dir);
      parent_dir += "/";
    }
  }

  if (multi_patient_run) {
    ex.Name(ex.Name() + "-" + patient_no_extension);
  }
  std::string base_file_name = (multi_patient_run) ? scenario_no_extension + "-" + patient_no_extension : scenario_no_extension;
  std::string console_file = base_file_name + ".log";
  std::string log_file = base_file_name + "Results.log";
  std::string results_file = base_file_name + "Results.csv";

  std::unique_ptr<PhysiologyEngine> eng;
  Logger console_logger;
  Logger file_logger(ex.Computed() + parent_dir + console_file);
  try {
    file_logger.SetConsoleLogLevel(log4cpp::Priority::WARN);
    file_logger.SetConsolesetConversionPattern("%d{%H:%M} [%p] " + ex.Name() + " %m%n");
    console_logger.SetConsolesetConversionPattern("%d{%H:%M} [%p] %m%n");
    console_logger.FormatMessages(false);

    eng = CreateBioGearsEngine(&file_logger);
  } catch (std::exception e) {
    std::cout << e.what();
  }

  BioGearsScenario sce(eng->GetSubstanceManager());
  if (!sce.Load(trim(trimed_scenario_path))) {
    if (!sce.Load("Scenario/" + trim(trimed_scenario_path))) {
      console_logger.Info(biogears::asprintf("Error[%d]: %s failed to find the specified scenario file %s", ExecutionErrors::SCENARIO_IO_ERROR, ex.Name().c_str(), ex.Scenario().c_str()));
    }
  }

  if (!ex.Patient().empty()) {
    sce.GetInitialParameters().SetPatientFile(ex.Patient());
  } else if (!ex.Scenario().empty()) {
    sce.GetInitialParameters().SetPatientFile(ex.Scenario());
  } else {
    std::ifstream ifs { ex.Scenario() };
    if (!ifs.is_open()) {
      ifs.open("Scenarios/" + ex.Scenario());
      if (!ifs.is_open()) {
        console_logger.Info(biogears::asprintf("Error[%d]: %s failed to find the specified scenario file %s", ExecutionErrors::SCENARIO_IO_ERROR, ex.Name().c_str(), ex.Scenario().c_str()));
        return;
      }
    }
    std::unique_ptr<mil::tatrc::physiology::datamodel::ScenarioData> scenario;
    try {
      xml_schema::flags xml_flags;
      xml_schema::properties xml_properties;
      std::cout << "Reading " << ex.Scenario() << std::endl;
      xml_properties.schema_location("uri:/mil/tatrc/physiology/datamodel", "xsd/BioGearsDataModel.xsd");
      scenario = mil::tatrc::physiology::datamodel::Scenario(ifs, xml_flags, xml_properties);
    } catch (std::runtime_error e) {
      std::cout << e.what() << std::endl;
      console_logger.Info(biogears::asprintf("Error[%d]: %s failed parse the specified scenario file %s", ExecutionErrors::SCENARIO_PARSE_ERROR, ex.Name().c_str(), ex.Scenario().c_str()));
      return;
    } catch (xsd::cxx::tree::parsing<char> e) {
      std::cout << e << std::endl;
      console_logger.Info(biogears::asprintf("Error[%d]: %s failed parse the specified scenario file %s", ExecutionErrors::SCENARIO_PARSE_ERROR, ex.Name().c_str(), ex.Scenario().c_str()));
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
    bse.Execute(sce, ex.Computed() + parent_dir + results_file, nullptr);
    console_logger.Info("Completed " + ex.Name());
  } catch (...) {
    console_logger.Error("Failed " + ex.Name());
  }
}

namespace biogears {
Driver::Driver(size_t thread_count)
  : _pool { thread_count }
{
}
//-----------------------------------------------------------------------------
Driver::~Driver()
{
  _pool.stop();
  _pool.join();
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
      break;
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
      break;
    }
  }
}
//-----------------------------------------------------------------------------
void Driver::run()
{
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
  while (!_pool.stop_if_empty()) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}
//-----------------------------------------------------------------------------
void Driver::join()
{
  _pool.join();
}
//-----------------------------------------------------------------------------
void Driver::queue_BGEUnitTest(Executor exec, bool as_subprocess)
{
  //TODO: Add Subprocess suport for BGEUnitTest by porting test_driver to bg-unittest
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
#endif
}
//-----------------------------------------------------------------------------
void Driver::queue_CDMUnitTest(Executor exec, bool as_subprocess)
{
  //TODO: Add Subprocess suport for BGEUnitTest by porting test_driver to bg-unittest
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
#endif
}
//-----------------------------------------------------------------------------
void Driver::queue_Scenario(Executor exec, bool as_subprocess)
{
  enum class PatientType {
    FILE,
    STATE,
    INLINE
  };

  std::function<void(Executor, bool)> scenario_launch;

#if defined(BIOGEARS_SUBPROCESS_SUPPORT)
  if (as_subprocess) {
    scenario_launch = std::bind(subprocess_execute, std::placeholders::_1, std::placeholders::_2);
  } else {
    scenario_launch = std::bind(async_execute, std::placeholders::_1, std::placeholders::_2);
  }
#else
  scenario_launch = std::bind(async_execute, std::placeholders::_1, std::placeholders::_2);
#endif
  std::ifstream ifs { exec.Scenario() };
  if (!ifs.is_open()) {
    ifs.open("Scenarios/" + exec.Scenario());
    if (!ifs.is_open()) {
      std::cerr << "Failed to open Scenarios/" << exec.Scenario() << " terminating\n";
      return;
    }
  }

  std::unique_ptr<mil::tatrc::physiology::datamodel::ScenarioData> scenario;
  try {
    xml_schema::flags xml_flags;
    xml_schema::properties xml_properties;

    xml_properties.schema_location("uri:/mil/tatrc/physiology/datamodel", "xsd/BioGearsDataModel.xsd");
    scenario = mil::tatrc::physiology::datamodel::Scenario(ifs, xml_flags, xml_properties);
  } catch (std::runtime_error e) {
    std::cout << e.what() << "\n";
    return;
  } catch (xsd::cxx::tree::parsing<char> e) {
    std::cout << e << "\n";
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
    exec.State(scenario->EngineStateFile().get());
    _pool.queue_work(std::bind(scenario_launch, std::move(exec), false));
    return;
  } else if (scenario->InitialParameters().present() && scenario->InitialParameters()->PatientFile().present()) {
    const auto patient_file = scenario->InitialParameters()->PatientFile().get();
    std::string nc_patient_file = patient_file;
    std::transform(nc_patient_file.begin(), nc_patient_file.end(), nc_patient_file.begin(), ::tolower);
    if ("all" == nc_patient_file) {
      auto patient_files = biogears::ListFiles("patients", R"(\.xml)");
      for (const std::string& patient_file : patient_files) {
        Executor patientEx { exec };
        patientEx.Patient(patient_file);

        ///----
        std::string trimed_patient_path(trim(patientEx.Patient()));
        auto split_patient_path = split(trimed_patient_path, '/');
        auto patient_no_extension = split(split_patient_path.back(), '.').front();

        std::string trimed_scenario_path(trim(patientEx.Scenario()));
        auto split_scenario_path = split(trimed_scenario_path, '/');
        auto scenario_no_extension = split(split_scenario_path.back(), '.').front();

        if (patientEx.Name().empty()) {
          patientEx.Results({ scenario_no_extension + "-" + patient_no_extension });
          patientEx.Name(scenario_no_extension + "-" + patient_no_extension);
        } else {
          patientEx.Results({ patientEx.Name() + "-" + patient_no_extension });
          patientEx.Name(patientEx.Name() + "-" + patient_no_extension);
        }
        ///----

        _pool.queue_work(std::bind(scenario_launch, std::move(patientEx), true));
      }
    } else {
      exec.Patient(patient_file);
      _pool.queue_work(std::bind(scenario_launch, std::move(exec), false));
    }
  } else if (scenario->InitialParameters().present() && scenario->InitialParameters()->Patient().present()) {
    exec.Patient("");
    exec.State("");
    _pool.queue_work(std::bind(scenario_launch, std::move(exec), false));
  } else {
    std::cout << "Skipping " << exec.Name() << " no patient specified.\n";
    return;
  }
}
//-----------------------------------------------------------------------------
} // NAMESPACE
