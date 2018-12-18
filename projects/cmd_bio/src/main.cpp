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

#include "CDMDriver.h"
#include "ConfigFileParser.hpp"
#include "TestDriver.h"
#include "argumentparser.hpp"
#include <biogears/exports.h>

#include <iostream>
#include <string>
#include <thread>

#include "ScenarioDriver.h"
#include "PatientValidationGenerator.h"
#include "StateGenerator.h"
#include "data/EnvironmentGenerator.h"
#include "data/PatientGenerator.h"

#include "data/StabilizationGenerator.h"
#include "data/SubstanceGenerator.h"

#include "data/CompoundGenerator.h"
#include "data/NutritionGenerator.h"

#include <biogears/cdm/utils/FileUtils.h>

//!
//! \brief Reads command line argument and executes corresponding operation
//! \param argc : Number of command line arguments
//! \param argv : Pointers to command line arguments
//! \return int 0 on success, other numbers on failure
//!
int main(int argc, char** argv)
{
  ArgumentParser parser;

  parser.addArgument("DATA"); // gen-data
  parser.addArgument("STATES"); // gen-states
  parser.addArgument("SYSTEM"); // run-system-validation
  parser.addArgument("PATIENT"); // run-patient-validation
  parser.addArgument("DRUG"); // run-drug-validation
  parser.addArgument("VERIFICATION"); // run-verification
  parser.addArgument("VALIDATION"); // run-cdm-validation

  parser.parse(argc, argv);

  unsigned int n = std::thread::hardware_concurrency();

  if (argc > 1) {
    if (parser.exists("STATES")) {
      biogears::ScenarioDriver generator{ n };
      std::vector<std::string> patients = biogears::ParseConfigFile("patientListShort.config");
      generator.LoadPatients(patients, std::string("Scenarios/InitialPatientStateAll.xml"));
      generator.run();
      while (!generator.stop_if_empty()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
      generator.join();
    } else if (parser.exists("DATA")) { // gen-data
      std::vector<std::unique_ptr<biogears::CSVToXMLConvertor>> generators;
      generators.push_back(std::make_unique<biogears::SubstanceGenerator>());
      generators.push_back(std::make_unique<biogears::EnvironmentGenerator>());
      generators.push_back(std::make_unique<biogears::PatientGenerator>());
      generators.push_back(std::make_unique<biogears::StabilizationGenerator>());
      generators.push_back(std::make_unique<biogears::NutritionGenerator>());
      generators.push_back(std::make_unique<biogears::CompoundGenerator>());
      for (auto& gen : generators) {
        gen->parse();
        gen->save();
      }
    } else if (parser.exists("SYSTEM")) { // run-system-validation
      std::vector<std::string> files = biogears::ParseConfigFile("ValidationSystemsShort.config");
      biogears::ScenarioDriver test{ n };
      test.LoadPatients(files, std::string("Scenarios/Validation/Patient-Validation.xml"));
      test.run();
      while (!test.stop_if_empty()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
      test.join();
    } else if (parser.exists("PATIENT")) { // run-patient-validation
      biogears::ScenarioDriver generator{ n };
      std::vector<std::string> patients = biogears::ParseConfigFile("patientListShort.config");
      generator.LoadPatients(patients, std::string("Scenarios/Validation/Patient-Validation.xml"));
      generator.run();
      while (!generator.stop_if_empty()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
      generator.join();
    } else if (parser.exists("DRUG")) { // run-drug-validation
      std::vector<std::string> files = biogears::ParseConfigFile("ValidationDrugsShort.config");
      biogears::ScenarioDriver test{ n };
      test.LoadScenarios(files);
      test.run();
      while (!test.stop_if_empty()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
      test.join();
    } else if (parser.exists("VERIFICATION")) { // run-verification
      std::vector<std::string> files = biogears::ParseConfigFile("VerificationScenariosShort.config");
      biogears::ScenarioDriver test{ n };
      test.LoadScenarios(files);
      test.run();
      while (!test.stop_if_empty()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
      test.join();
    } else if (parser.exists("VALIDATION")) { // run-cdm-tests
      std::vector<std::string> files = biogears::ParseConfigFile("CDMUnitTestsShort.config");
      biogears::CDMDriver test{ n };
      test.RunCDMs(files);
      test.run();
      while (!test.stop_if_empty()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
      test.join();
    } else {
      std::cout << "Input not recognized" << std::endl;
      std::cout << "Usage " + std::string(argv[0]) + ": [STATES|DATA]";
    }
  }
  return 0;
}
