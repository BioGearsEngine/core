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

#include "exec/Driver.h"

#include "data/CompoundGenerator.h"
#include "data/EnvironmentGenerator.h"
#include "data/NutritionGenerator.h"
#include "data/PatientGenerator.h"
#include "data/StabilizationGenerator.h"
#include "data/SubstanceGenerator.h"

#include "utils/Arguments.h"
#include "utils/Config.h"
#include "utils/ReportWriter.h"

#include <biogears/cdm/utils/ConfigParser.h>
#include <biogears/cdm/utils/FileUtils.h>
#include <biogears/version.h>

#include <fstream>
#include <iostream>

void print_help() {

    std::cout << "Usage cmd_bio [HELP GENDATA, GENSTATES, GENSEPSIS, VERIFY, VERSION]\n"
                                "[THREADS N]\n" 
                                "[TEST FILE [FILE]...], pSCENARIO FILE [FILE]...], VALIDATE patient|drug|system|all], [GENTABLES html|md|xml|web|all]\n\n";

    std::cout << "Flags: \n";
    std::cout << "j : Thread control -j N\n";
    std::cout << "h : Print this message\n";
    std::cout << std::endl;
    exit(0);
}
//!
//! \brief Reads command line argument and executes corresponding operation
//! \param argc : Number of command line arguments
//! \param argv : Pointers to command line arguments
//! \return int 0 on success, other numbers on failure
//!
int main(int argc, char** argv)
{
  biogears::Arguments args(
    { "H","HELP","GENDATA", "GENSEPSIS", "GENSTATES", "VERIFY", "VERSION" } //Options
    ,
    { "J", "THREADS" } //Keywords
    ,
    { "TEST", "SCENARIO", "VALIDATE", "GENTABLES" } //MultiWords
  );
  

  bool run_patient_validation = false;
  bool run_drug_validation = false;
  bool run_system_validation = false;
  bool run_verification = false;

  unsigned int thread_count = std::thread::hardware_concurrency();
  if ( !args.parse(argc, argv) || args.Option("HELP") || args.Option("H") ||  args.empty() ) {
    print_help();
  }

  if (args.Option("VERSION")) {
    std::cout << "Using libbiogears-" << biogears::full_version_string() << std::endl;
    exit(0);
  }
  if (args.KeywordFound("THREADS")) {
    try {
      thread_count = std::stoi(args.Keyword("THREADS"));
    } catch (std::exception) {
      std::cerr << "Error: THREADS given but " << args.Keyword("THREADS") << "is not a valid Integer.\n";
      exit(1);
    }
  }
  if (args.KeywordFound("J")) {
    try {
      thread_count = std::stoi(args.Keyword("J"));
    } catch (std::exception) {
      std::cerr << "Error: J given but " << args.Keyword("J") << "is not a valid Integer.\n";
      exit(1);
    }
  }
  biogears::Driver driver{ thread_count };

  const biogears::Config conf{ "Email.config", true };
  driver.configure(conf);

  if (args.Option("GENSTATES")) {
    const biogears::Config runs{ "GenStates.config" };
    driver.queue(runs);
  }

  if (args.Option("GENSEPSIS")) {
    const biogears::Config runs { "GenSepsisStates.config" };
    driver.queue(runs);
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
      std::cout << "Generating Data:" << gen->Path() << gen->Filename() << "\n";
      gen->parse();
      gen->save();
      std::cout << "\n\n";
    }
  }

  if (args.Option("VERIFY")) {
    run_verification = true;
  }
#ifdef CMD_BIO_SUPPORT_CIRCUIT_TEST
  if (args.MultiWordFound("TEST")) {
    auto tests = args.MultiWord("TEST");
    for (auto& test : tests) {
      std::transform(test.begin(), test.end(), test.begin(), ::tolower);
      if (test == "cdm") { // run-cdm-tests
        biogears::Config runs{ "CDMUnitTests.config" };
        driver.queue(runs);
      } else if (test == "bge") {
        biogears::Config runs{ "BGEUnitTests.config" };
        driver.queue(runs);
      } else {
        std::cout << "Warning: No Test known as " << test << "exists.\n";
      }
    }
  }
#endif
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
      } else if (test == "all") {
        run_patient_validation = run_drug_validation = run_system_validation = run_verification = true;
      } else {
        std::cout << "Warning: No Validation known as " << test << "exists.\n";
      }
    }
  }

  if (run_system_validation) { // run-system-validation
    const auto runs = biogears::Config("ValidationSystems.config");
    driver.queue(runs);
  }
  if (run_patient_validation) { //run-patient-validation
    const auto runs = biogears::Config("ValidationPatients.config");
    driver.queue(runs);
  }
  if (run_drug_validation) { // run-drug-validation
    const auto runs = biogears::Config("ValidationDrugs.config");
    driver.queue(runs);
  }
  if (run_verification) { // run-verification
    const auto runs = biogears::Config("VerificationScenarios.config");
    driver.queue(runs);
  }

  if (args.MultiWordFound("SCENARIO")) {
    auto configs = biogears::Config{  };
    for (auto& arg : args.MultiWord("SCENARIO")) {
      auto ex = biogears::Executor{ arg, biogears::EDriver::ScenarioTestDriver};
      ex.Computed("Scenarios/");
      ex.Scenario(arg);
      configs.push_back(ex);
    }
    driver.queue(configs);
  }

  driver.run();
  driver.stop_when_empty();
  driver.join();


  //We want Gentables to run after all other work has finished
  if (args.MultiWordFound("GENTABLES")) {
    biogears::ReportWriter report_writer;
    auto tables = args.MultiWord("GENTABLES");
    for (auto& table : tables) {
      std::transform(table.begin(), table.end(), table.begin(), ::tolower);
      if (table == "html") {
        report_writer.gen_tables(biogears::ReportWriter::HTML);
      } else if (table == "md") {
        report_writer.gen_tables(biogears::ReportWriter::MD);
      } else if (table == "xml") {
        report_writer.gen_tables(biogears::ReportWriter::XML);
      } else if (table == "web") {
        report_writer.gen_tables(biogears::ReportWriter::WEB);
      } else if (table == "all") {
        report_writer.gen_tables(biogears::ReportWriter::HTML);
        report_writer.gen_tables(biogears::ReportWriter::MD);
        report_writer.gen_tables(biogears::ReportWriter::XML);
      } else {
        std::cout << "Warning: " << table << " is not a valid keyword.\n";
      }
    }
  }


  return 0;
}
