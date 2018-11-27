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

#include "argumentparser.hpp"
#include "ConfigFileParser.hpp"
#include "TestDriver.h"
#include "CDMDriver.h"
#include <biogears/exports.h>

#include <iostream>
#include <string>
#include <thread>

#include "StateGenerator.h"
#include "PatientValidationGenerator.h"
#include "data/EnvironmentGenerator.h"
#include "data/PatientGenerator.h"

#include "data/StabilizationGenerator.h"
#include "data/SubstanceGenerator.h"

#include "data/CompoundGenerator.h"
#include "data/NutritionGenerator.h"

//!
//! \brief Reads command line argument and executes corresponding operation
//! \param argc : Number of command line arguments
//! \param argv : Pointers to command line arguments
//! \return int 0 on success, other numbers on failure
//! 
int main(int argc, char** argv)
{
  namespace bg = biogears;
  ArgumentParser parser;

  parser.addArgument("DATA");         // gen-data
  parser.addArgument("STATES");       // gen-states
  parser.addArgument("SYSTEM");       // run-system-validation
  parser.addArgument("PATIENT");      // run-patient-validation
  parser.addArgument("DRUG");         // run-drug-validation
  parser.addArgument("VERIFICATION"); // run-verification
  parser.addArgument("VALIDATION");   // run-cdm-validation
  
  parser.parse(argc,argv);
  if (argc > 1) {
    if (parser.exists("STATES")) { // gen-states
      biogears::StateGenerator generator{ 5 };

      generator.GenStates();
      generator.run();
      
      while ( !generator.stop_if_empty() )
      {
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
      generator.join();

    } else if (parser.exists("DATA")) { // gen-data
      std::vector<std::unique_ptr<bg::CSVToXMLConvertor>> generators;
      generators.push_back(std::make_unique<bg::SubstanceGenerator>());
      generators.push_back(std::make_unique<bg::EnvironmentGenerator>());
      generators.push_back(std::make_unique<bg::PatientGenerator>());
      generators.push_back(std::make_unique<bg::StabilizationGenerator>());
      generators.push_back(std::make_unique<bg::NutritionGenerator>());
      generators.push_back(std::make_unique<bg::CompoundGenerator>());
      for (auto& gen : generators) {
        gen->parse();
        gen->save();
      }
    } else if (parser.exists("SYSTEM")) { // run-system-validation
      std::vector<std::string> files = biogears::ParseConfigFile("ValidationSystemsShort.config");
      biogears::TestDriver test{ 5 };
      test.RunTests(files);
      test.run();
      while (!test.stop_if_empty()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
      test.join();
    } else if (parser.exists("PATIENT")) { // run-patient-validation
      biogears::PatientValidationGenerator generator{ 5 };
      generator.GenPatientValidation();
      generator.run();
      while (!generator.stop_if_empty()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
      generator.join();
    } else if (parser.exists("DRUG")) { // run-drug-validation
      std::vector<std::string> files = biogears::ParseConfigFile("ValidationDrugsShort.config");
      biogears::TestDriver test{ 5 };
      test.RunTests(files);
      test.run();
      while (!test.stop_if_empty()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
      test.join();
    } else if (parser.exists("VERIFICATION")) { // run-verification
      std::vector<std::string> files = biogears::ParseConfigFile("VerificationScenariosShort.config");
      biogears::TestDriver test{ 5 };
      test.RunTests(files);
      test.run();
      while (!test.stop_if_empty()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
      test.join();
    } else if (parser.exists("VALIDATION")) { // run-cdm-tests
      std::vector<std::string> files = biogears::ParseConfigFile("CDMUnitTestsShort.config");
      biogears::CDMDriver test { 5 };
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
