//
// process_per_connection.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <cmath>
#include <cstdlib>

#include <iostream>

#include <boost/process.hpp>

#include "exec/Driver.h"
#include "utils/Executor.h"

#include <biogears/cdm/patient/SEPatient.h>

#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/controller/scenario/BioGearsScenario.h>
#include <biogears/engine/controller/scenario/BioGearsScenarioExec.h>

#include <biogears/schema/cdm/Patient.hxx>
#include <biogears/string/manipulation.h>

using namespace boost::process;
using namespace biogears;

enum class PatientType {
  FILE,
  STATE,
  INLINE
};

int execute_scenario(Executor& ex, bool multi_patient_run, PatientType patient_type, CDM::PatientData patient_data)
{
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
  if (!sce.Load("Scenarios/" + trim(trimed_scenario_path))) {
    console_logger.Error("Unable to find  " + trim(trimed_scenario_path));
  }

  switch (patient_type) {
  case PatientType::FILE:
    sce.GetInitialParameters().SetPatientFile(ex.Patient());
    break;
  case PatientType::STATE:
    sce.SetEngineStateFile(ex.Patient());
    break;
  case PatientType::INLINE: {
    biogears::SEPatient patient { sce.GetLogger() };
    patient.Load(patient_data);
    sce.GetInitialParameters().SetPatient(patient);
  } break;
  }

  console_logger.Info("Starting " + ex.Name());
  try {
    BioGearsScenarioExec bse { *eng };
    bse.Execute(sce, ex.Computed() + parent_dir + results_file, nullptr);
    console_logger.Info("Completed " + ex.Name());
  } catch (...) {
    console_logger.Error("Failed " + ex.Name());
  }
};

#include <boost/program_options.hpp>
int main(int argc, char* argv[])
{
  using namespace boost::program_options;

  try {
  // Declare the supported options.
  options_description desc("Allowed options");
  desc.add_options()("help", "produce help message")
      ("name", value<std::string>(), "Set Scenario Name")
      ("driver", value<std::string>()->default_value("ScenarioTestDriver"), "Set Scenario Driver \n  BGEUnitTestDriver\n  CDMUnitTestDriver\n  ScenarioTestDriver")
      ("group", value<std::string>(), "Set Name of Scenario Group")
      ("patient", value<std::string>(), "Specifcy the Patient File")
      ("scenario", value<std::string>(), "Specifcy the Scenario File")
      ("results", value<std::string>(), "Specifcy the Resoults File");

  variables_map vm;
  store(parse_command_line(argc, argv, desc), vm);
  notify(vm);

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }

  Executor ex;

  if (vm.count("name")) {
    ex.Name(vm["name"].as<std ::string>());
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

  if (vm.count("patient")) {
    ex.Patient(vm["patient"].as<std ::string>());
  }
  if (vm.count("scenario")) {
    ex.Scenario(vm["scenario"].as<std ::string>());
  }
  if (vm.count("results")) { 
    ex.Results(biogears::split( vm["results"].as<std::string>(), ','));
  }
  if (vm.count("group")) {
    ex.Group(vm["group"].as<std ::string>());
  }

  std::cout << ex << std::endl;
  
  } catch (std::exception& e) {
    std::cerr << e.what() << "\n";
  }
}
