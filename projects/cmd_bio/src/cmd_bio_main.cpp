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

#include <biogears/exports.h>

#include <iostream>
#include <string>
#include <thread>

#include "exec/CDMDriver.h"
#include "exec/PatientValidationGenerator.h"
#include "exec/ScenarioDriver.h"
#include "exec/StateGenerator.h"
#include "exec/TestDriver.h"

#include "data/CompoundGenerator.h"
#include "data/EnvironmentGenerator.h"
#include "data/NutritionGenerator.h"
#include "data/PatientGenerator.h"
#include "data/StabilizationGenerator.h"
#include "data/SubstanceGenerator.h"

#include "utils/Arguments.h"
#include "utils/Config.h"

#include "biogears/cdm/utils/ConfigParser.h"
#include <biogears/cdm/utils/FileUtils.h>

//!
//! \brief Reads command line argument and executes corresponding operation
//! \param argc : Number of command line arguments
//! \param argv : Pointers to command line arguments
//! \return int 0 on success, other numbers on failure
//!
int main(int argc, char** argv)
{
  biogears::Arguments args(
    { "GENDATA", "GENSTATES", "VERIFY" } //Options
    ,
    { "THREADS" } //Keywords
    ,
    { "TEST", "SCENARIO" } //MultiWords
  );
  args.parse(argc, argv);

  bool run_patient_validation = false;
  bool run_drug_validation = false;
  bool run_system_validation = false;
  bool run_verification = false;

  unsigned int thread_count = std::thread::hardware_concurrency();
  if (args.KeywordFound("THREADS")) {
    try {
      thread_count = std::stoi(args.Keyword("THREADS"));
    } catch (std::exception) {
      std::cerr << "Error: THREADS given but " << args.Keyword("THREADS") << "is not a valid Integer.\n";
      exit(1);
    }
  }

  if (args.Option("GENSTATES")) {
    biogears::ScenarioDriver generator{ thread_count };
    std::vector<std::string> patients = biogears::ListFiles("patients", R"(\.xml)");
    generator.LoadPatients(patients, std::string("Scenarios/InitialPatientStateAll.xml"));
    generator.run();
    while (!generator.stop_if_empty()) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    generator.join();
  }

  if (args.Option("GENDATA")) { // gen-data
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
  }

  if (args.Option("VERIFY")) {
    run_patient_validation = true;
    run_drug_validation = true;
    run_system_validation = true;
    run_verification = true;
  }
  if (args.MultiWordFound("TEST")) {
    auto tests = args.MultiWord("TEST");
    for (auto& test : tests) {
      std::transform(test.begin(), test.end(), test.begin(), ::tolower);
      if (test == "cdm") { // run-cdm-tests
        biogears::Config runs{ "CDMUnitTests.config" };
        biogears::CDMDriver cdm{ thread_count };
        cdm.RunCDMs(runs);
        cdm.run();
        while (!cdm.stop_if_empty()) {
          std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        cdm.join();
      } else if (test == "bge") {
        biogears::Config runs{ "BGEUnitTests.config" };
        biogears::CDMDriver bge{ thread_count };
        bge.RunCDMs(runs);
        bge.run();
        while (!bge.stop_if_empty()) {
          std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        bge.join();
      } else {
        std::cout << "Warrning: No Test known as " << test << "exists.\n";
      }
    }
  }

  if (args.MultiWordFound("VALIDATE")) {
    auto tests = args.MultiWord("VALIDATE");
    for (auto& test : tests) {
      std::transform(test.begin(), test.end(), test.begin(), ::tolower);
      if (test == "patient") {
        run_patient_validation = true;
      } else if (test == "drug") {
        run_drug_validation = true;
      } else if (test == "system") {
        run_system_validation = true;
      } else if (test == "validation") {
        run_verification = true;
      } else if (test == "all") {
        run_patient_validation = run_drug_validation = run_system_validation = run_verification = true;
      } else {
        std::cout << "Warrning: No Validation known as " << test << "exists.\n";
      }
    }
  }

  if (run_system_validation) { // run-system-validation
    //  std::vector<std::string> files = biogears::Config("ValidationSystemsShort.config");
    //  biogears::ScenarioDriver test{ thread_count };
    //  test.LoadPatients(files, std::string("Scenarios/Validation/Patient-Validation.xml"));
    //  test.run();
    //  while (!test.stop_if_empty()) {
    //    std::this_thread::sleep_for(std::chrono::seconds(1));
    //  }
    //  test.join();
  }
  if (run_patient_validation) {
    // run-patient-validation
    //  biogears::ScenarioDriver generator{ thread_count };
    //  std::vector<std::string> patients = biogears::ListFiles("patients", R"(\.xml)");
    //  generator.LoadPatients(patients, std::string("Scenarios/Validation/Patient-Validation.xml"));
    //  generator.run();
    //  while (!generator.stop_if_empty()) {
    //    std::this_thread::sleep_for(std::chrono::seconds(1));
    //  }
    //  generator.join();
  }
  if (run_drug_validation) { // run-drug-validation
    //  std::vector<std::string> files = biogears::ParseConfigFile("ValidationDrugsShort.config");
    //  biogears::ScenarioDriver test{ thread_count };
    //  test.LoadScenarios(files);
    //  test.run();
    //  while (!test.stop_if_empty()) {
    //    std::this_thread::sleep_for(std::chrono::seconds(1));
    //  }
    //  test.join();
  }
  if (run_verification) { // run-verification
    //  std::vector<std::string> files = biogears::ParseConfigFile("VerificationScenariosShort.config");
    //  biogears::ScenarioDriver test{ thread_count };
    //  test.LoadScenarios(files);
    //  test.run();
    //  while (!test.stop_if_empty()) {
    //    std::this_thread::sleep_for(std::chrono::seconds(1));
    //  }
    //  test.join();
  }

  if (args.MultiWordFound("SCENARIO")) {
    for (auto& arg : args.MultiWord("SCENARIO")) {
      biogears::Config config {arg};
    }
  }

  return 0;
}
