//
// process_per_connection.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#define NOMINMAX

#include <cmath>
#include <cstdlib>
#include <sys/stat.h>

#include <iostream>

#include "exec/Driver.h"
#include "utils/Executor.h"

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenario.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenarioExec.h>
#include <biogears/filesystem/path.h>
#include <biogears/schema/cdm/Patient.hxx>
#include <biogears/string/manipulation.h>
#include <biogears/version.h>

using namespace biogears;

enum class PatientType {
  FILE,
  STATE,
  INLINE
};

int execute_scenario(Executor& ex, log4cpp::Priority::Value log_level)
{
  std::string trimed_scenario_path(trim(ex.Scenario()));
  auto split_scenario_path = split(trimed_scenario_path, '/');
  auto scenario_no_extension = split(split_scenario_path.back(), '.').front();

  if (ex.Name() == "Undefined") {
    ex.Name(scenario_no_extension);
  }

  //NOTE: This loses non relative prefixes as the split will eat the leading path_separator
  std::string parent_dir;
  for (auto dir = split_scenario_path.begin(); dir != split_scenario_path.end(); ++dir) {
    if (dir + 1 != split_scenario_path.end()) {
      parent_dir.append(*dir);
      parent_dir += "/";
    }
  }

  std::string base_file_name = ex.Results()[0];
  std::string console_log_file = base_file_name + ".log";
  std::string results_log_file = base_file_name + "Results.log";
  std::string results_csv_file = base_file_name + "Results.csv";

  std::unique_ptr<PhysiologyEngine> eng;
  Logger console_logger;
  Logger file_logger(ex.Computed() + parent_dir + console_log_file);
  try {

    file_logger.SetConsoleLogLevel(log_level);
    file_logger.SetConsolesetConversionPattern("%d{%H:%M} [%p] " + ex.Name() + "%m%n");
    console_logger.SetConsolesetConversionPattern("%d{%H:%M} [%p] %m%n");
    console_logger.FormatMessages(false);

    eng = CreateBioGearsEngine(&file_logger);
  } catch (std::exception e) {
    std::cout << e.what() << std::endl;
  }

  BioGearsScenario sce(eng->GetSubstanceManager());

  if (!sce.Load(trim(trimed_scenario_path))) {
    if (!sce.Load("Scenarios/" + trim(trimed_scenario_path))) {
      console_logger.Error("Unable to find  " + trim(trimed_scenario_path));
    }
  }

  if (ex.TrackStabilization()) {
    eng->SetTrackStabilizationFlag(true);
  }
  //-----------------------------------------------------------------------------------
  if (!ex.Patient().empty()) {
    sce.GetInitialParameters().SetPatientFile(ex.Patient());
  } else if (!ex.State().empty()) {
    sce.SetEngineStateFile(ex.State());
  } else {
    std::ifstream ifs { ex.Scenario() };
    if (!ifs.is_open()) {
      ifs.open("Scenarios/" + ex.Scenario());
      if (!ifs.is_open()) {
        std::cerr << "Unable to find " << ex.Scenario() << std::endl;
        return static_cast<int>(ExecutionErrors::SCENARIO_IO_ERROR);
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
      return static_cast<int>(ExecutionErrors::SCENARIO_PARSE_ERROR);
    } catch (xsd::cxx::tree::parsing<char> e) {
      std::cout << e << std::endl;
      return static_cast<int>(ExecutionErrors::SCENARIO_PARSE_ERROR);
    }
    biogears::SEPatient patient { sce.GetLogger() };
    if (scenario->EngineStateFile().present()) {
      sce.SetEngineStateFile(scenario->EngineStateFile().get());
    } else if (scenario->InitialParameters()->PatientFile().present()) {
      sce.GetInitialParameters().SetPatientFile(scenario->InitialParameters()->PatientFile().get());
    } else if (scenario->InitialParameters()->Patient().present()) {
      ex.Patient(scenario->InitialParameters()->Patient().get().Name());
      patient.Load(scenario->InitialParameters()->Patient().get());
      sce.GetInitialParameters().SetPatient(patient);
    } else {
      std::cerr << "A Patient or State file is required to run " << ex.Scenario() << std::endl;
      return static_cast<int>(ExecutionErrors::PATIENT_IO_ERROR);
    }
  }
  //-----------------------------------------------------------------------------------
  try {
    BioGearsScenarioExec bse { *eng };
    bse.Execute(sce, ex.Computed() + parent_dir + results_csv_file, nullptr);
  } catch (...) {
    console_logger.Error("Failed " + ex.Name());
    static_cast<int>(ExecutionErrors::BIOGEARS_RUNTIME_ERROR);
  }

  return static_cast<int>(ExecutionErrors::NONE);
};

std::string basename_(const std::string& p)
{
  return filesystem::path(p).basename().string();
}

#if defined(BIOGEARS_SUBPROCESS_SUPPORT)
#include <boost/program_options.hpp>

// Boost doesn't offer any obvious way to construct a usage string
// from an infinite list of positional parameters.  This hack
// should work in most reasonable cases.
std::vector<std::string> get_unlimited_positional_args_(const boost::program_options::positional_options_description& p)
{
  assert(p.max_total_count() == std::numeric_limits<unsigned>::max());

  std::vector<std::string> parts;

  // reasonable upper limit for number of positional options:
  const int MAX = 1000;
  std::string last = p.name_for_position(MAX);

  for (int i = 0; true; ++i) {
    std::string cur = p.name_for_position(i);
    if (cur == last) {
      parts.push_back(cur);
      return parts;
    }
    parts.push_back(cur);
  }
  return parts; // never get here
}

std::string make_usage_string_(const std::string& program_name, const boost::program_options::options_description& desc, boost::program_options::positional_options_description& p)
{
  std::vector<std::string> parts;
  parts.push_back("Usage: ");
  parts.push_back(program_name);
  if (desc.options().size() > 0) {
    parts.push_back("[options]");
  }
  size_t N = p.max_total_count();
  if (N == std::numeric_limits<unsigned>::max()) {
    std::vector<std::string> args = get_unlimited_positional_args_(p);
    parts.insert(parts.end(), args.begin(), args.end());
  } else {
    for (int i = 0; i < N; ++i) {
      parts.push_back(p.name_for_position(i));
    }
  }
  std::ostringstream oss;
  std::copy(
    parts.begin(),
    parts.end(),
    std::ostream_iterator<std::string>(oss, " "));
  oss << '\n'
      << desc;
  return oss.str();
}

int main(int argc, char* argv[])
{
  using namespace boost::program_options;

  auto log_level = log4cpp::Priority::DEBUG;
  std::string help_message;

  try {
    // Declare the supported options.
    options_description desc("Allowed options");
    desc.add_options()
      ("help,h", "produce help message")
      ("name,n", value<std::string>(), "Set Scenario Name")
      ("driver,d", value<std::string>()->default_value("ScenarioTestDriver"), "Set Scenario Driver \n  BGEUnitTestDriver\n  CDMUnitTestDriver\n  ScenarioTestDriver")
      ("group,g", value<std::string>(), "Set Name of Scenario Group")
      ("patient,p", value<std::string>(), "Specifcy the Initial Patient File")
      ("state,s", value<std::string>(), "Specifcy the Initial Patient State File")
      ("results,r", value<std::string>(), "Specifcy the Results File")
      ("quiet,q", bool_switch()->default_value(false), "Supress most log messages")
      ("version,v", bool_switch()->default_value(false), "Print linked libBioGears version")
      ("track-stabilization", bool_switch()->default_value(false), "Turn on stabilization tracking for the scenario");

    options_description hidden;
    hidden.add_options()("scenario", value<std::string>(), "Specifcy the Scenario File");
    options_description all_options;
    all_options.add(desc);
    all_options.add(hidden);

    positional_options_description p;
    p.add("scenario", -1);

    help_message = make_usage_string_(basename_(argv[0]), desc, p);

    variables_map vm;
    store(command_line_parser(argc, argv).options(all_options).positional(p).run(), vm);
    notify(vm);

    if (vm.count("help") || argc < 2) {
      std::cout << help_message << std::endl;
      return static_cast<int>(ExecutionErrors::NONE);
    }

    if (vm["version"].as<bool>()) {
      std::cout << branded_version_string_str() << std::endl;
      return static_cast<int>(ExecutionErrors::NONE);
    }

    Executor ex;

    if (vm.count("name")) {
      auto name = vm["name"].as<std ::string>();
      if (name.front() == '\'' && name.back() == '\'') {
        name = biogears::trim(name, "'");
      }
      if (name.front() == '\"' && name.back() == '\"') {
        name = biogears::trim(name, "\"");
      }
      ex.Name(name);
    }

    if (vm["track-stabilization"].as<bool>()) {
      ex.TrackStabilization(true);
    }

    if (vm["driver"].as<std::string>() == "BGEUnitTestDriver") {
      ex.Driver(EDriver::BGEUnitTestDriver);
    } else if (vm["driver"].as<std::string>() == "CDMUnitTestDriver") {
      ex.Driver(EDriver::CDMUnitTestDriver);
    } else if (vm["driver"].as<std::string>() == "ScenarioTestDriver") {
      ex.Driver(EDriver::ScenarioTestDriver);
    } else {
      ex.Driver(EDriver::ScenarioTestDriver);
    }

    if (vm["quiet"].as<bool>()) {
      log_level = log4cpp::Priority::WARN;
    }

    if (vm.count("patient")) {
      auto patient = vm["patient"].as<std ::string>();
      if (patient.front() == '\'' && patient.back() == '\'') {
        patient = biogears::trim(patient, "'");
      }

      if (patient.front() == '\"' && patient.back() == '\"') {
        patient = biogears::trim(patient, "\"");
      }
      ex.Patient(patient);
      if (!filesystem::exists(ex.Patient())) {
        ex.Patient("patients/" + ex.Patient());
      }
    }
    if (vm.count("state")) {
      auto state = vm["state"].as<std ::string>();
      if (state.front() == '\'' && state.back() == '\'') {
        state = biogears::trim(state, "'");
      }
      if (state.front() == '\"' && state.back() == '\"') {
        state = biogears::trim(state, "\"");
      }
      ex.State(state);
    }
    if (vm.count("scenario")) {
      auto scenario = vm["scenario"].as<std ::string>();
      if (scenario.front() == '\'' && scenario.back() == '\'') {
        scenario = biogears::trim(scenario, "'");
      }
      if (scenario.front() == '\"' && scenario.back() == '\"') {
        scenario = biogears::trim(scenario, "\"");
      }
      ex.Scenario(scenario);
      if (!filesystem::exists(ex.Scenario())) {
        ex.Scenario("Scenarios/" + ex.Scenario());
      }
    } else {
      std::cerr << "No scenario file provided.";
      std::cerr << help_message << std::endl;
    }

    if (vm.count("results")) {
      auto results = vm["results"].as<std ::string>();
      if (results.front() == '\'' && results.back() == '\'') {
        results = biogears::trim(results, "'");
      }
      if (results.front() == '\"' && results.back() == '\"') {
        results = biogears::trim(results, "\"");
      }
      ex.Results(biogears::split(results, ','));
    } else {
      std::string trimed_scenario_path(trim(ex.Scenario()));
      auto split_scenario_path = split(trimed_scenario_path, '/');
      auto scenario_no_extension = split(split_scenario_path.back(), '.').front();

      ex.Results({ scenario_no_extension });
    }

    if (vm.count("group")) {
      auto group = vm["group"].as<std ::string>();
      if (group.front() == '\'' && group.back() == '\'') {
        group = biogears::trim(group, "'");
      }
      if (group.front() == '\"' && group.back() == '\"') {
        group = biogears::trim(group, "\"");
      }
      ex.Group(group);
    }

    return execute_scenario(ex, log_level);

  } catch (boost::program_options::required_option /*e*/) {
    std::cerr << "\n";
    //<< e.what() << "\n\n";
    std::cout << help_message << std::endl;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}
#else
#include "utils/Arguments.h"
int main(int argc, char* argv[])
{
  auto log_level = log4cpp::Priority::DEBUG;
  std::string help_message;

  biogears::Arguments args(
    { "H", "HELP", "V", "VERSION", "Q", "QUIET" }, /*Options*/
    { "N", "NAME", "D", "DRIVER", "G", "GROUP", "P", "PATIENT", "S", "STATE", "R", "RESULTS", "SCENARIO" }, /*Keywords*/
    {} /*MultiWords*/
  );
  args.set_required_keywords({ "SCENARIO" });

  if (!args.parse(argc, argv) || args.Option("H") || args.Option("HELP") || argc < 2) {
    if (args.Option("H") || args.Option("HELP")) {
      std::cerr << args.usuage_string();
      return static_cast<int>(ExecutionErrors::NONE);
    }
    if (args.Option("VERSION") || args.Option("V")) {
      std::cout << "Using libbiogears-" << biogears::full_version_string() << std::endl;
      return static_cast<int>(ExecutionErrors::NONE);
    }
    std::cerr << args.error_msg();
    std::cerr << args.usuage_string();
    return 1;
  }

  if (args.Option("H") || args.Option("HELP")) {
    std::cerr << args.usuage_string();
    return static_cast<int>(ExecutionErrors::NONE);
  }

  if (args.Option("VERSION") || args.Option("V")) {
    std::cout << "Using libbiogears-" << biogears::full_version_string() << std::endl;
    return static_cast<int>(ExecutionErrors::NONE);
  }

  Executor ex;
  if (args.KeywordFound("NAME") || args.KeywordFound("N")) {
    auto name = args.Keyword("name");
    if (name.front() == '\'' && name.back() == '\'') {
      name = biogears::trim(name, "'");
    }
    if (name.front() == '\"' && name.back() == '\"') {
      name = biogears::trim(name, "\"");
    }
    ex.Name(name);
  }

  if (args.Keyword("DRIVER") == "BGEUnitTestDriver") {
    ex.Driver(EDriver::BGEUnitTestDriver);
  } else if (args.Keyword("driver") == "CDMUnitTestDriver") {
    ex.Driver(EDriver::CDMUnitTestDriver);
  } else if (args.Keyword("driver") == "ScenarioTestDriver") {
    ex.Driver(EDriver::ScenarioTestDriver);
  } else {
    ex.Driver(EDriver::ScenarioTestDriver);
  }

  if (args.Option("quiet")) {
    log_level = log4cpp::Priority::WARN;
  }

  if (args.KeywordFound("PATIENT") || args.KeywordFound("P")) {
    auto patient = args.Keyword("patient");
    if (patient.front() == '\'' && patient.back() == '\'') {
      patient = biogears::trim(patient, "'");
    }

    if (patient.front() == '\"' && patient.back() == '\"') {
      patient = biogears::trim(patient, "\"");
    }
    ex.Patient(patient);
    if (!filesystem::exists(ex.Patient())) {
      ex.Patient("patients/" + ex.Patient());
    }
  }
  if (args.KeywordFound("STATE") || args.KeywordFound("S")) {
    auto state = args.Keyword("state");
    if (state.front() == '\'' && state.back() == '\'') {
      state = biogears::trim(state, "'");
    }
    if (state.front() == '\"' && state.back() == '\"') {
      state = biogears::trim(state, "\"");
    }
    ex.State(state);
  }
  if (args.KeywordFound("SCENARIO")) {
    auto scenario = args.Keyword("scenario");
    if (scenario.front() == '\'' && scenario.back() == '\'') {
      scenario = biogears::trim(scenario, "'");
    }
    if (scenario.front() == '\"' && scenario.back() == '\"') {
      scenario = biogears::trim(scenario, "\"");
    }
    ex.Scenario(scenario);
    if (!filesystem::exists(ex.Scenario())) {
      ex.Scenario("Scenarios/" + ex.Scenario());
    }
  }
  if (args.KeywordFound("RESULTS") || args.KeywordFound("R")) {
    auto results = args.Keyword("results");
    if (results.front() == '\'' && results.back() == '\'') {
      results = biogears::trim(results, "'");
    }
    if (results.front() == '\"' && results.back() == '\"') {
      results = biogears::trim(results, "\"");
    }
    ex.Results(biogears::split(results, ','));
  } else {
    std::string trimed_scenario_path(trim(ex.Scenario()));
    auto split_scenario_path = split(trimed_scenario_path, '/');
    auto scenario_no_extension = split(split_scenario_path.back(), '.').front();

    ex.Results({ scenario_no_extension });
  }

  if (args.KeywordFound("GROUP") || args.KeywordFound("G")) {
    auto group = args.Keyword("group");
    if (group.front() == '\'' && group.back() == '\'') {
      group = biogears::trim(group, "'");
    }
    if (group.front() == '\"' && group.back() == '\"') {
      group = biogears::trim(group, "\"");
    }
    ex.Group(group);
  }

  return execute_scenario(ex, log_level);
}
#endif
