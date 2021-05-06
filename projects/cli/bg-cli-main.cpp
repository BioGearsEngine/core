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
#include <regex>
#include <string>
#include <thread>

#include "exec/Driver.h"
#include "exec/PopulationGenerator.h"

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
#include <biogears/version.h>

#include <fstream>
#include <iostream>

#ifdef BIOGEARS_IO_PRESENT
#include <biogears/io/io-manager.h>
#endif

bool g_run_patient_validation = false;
bool g_run_drug_validation = false;
bool g_run_system_validation = false;
bool g_run_verification_routine = false;
bool g_run_generate_tables = false;
bool g_run_generate_sepsis_states = false;
bool g_run_generate_patient_states = false;
bool g_run_generate_populations = false;
bool g_run_generate_runtime_directory = false;
bool g_run_generate_data = false;

biogears::Arguments::KeywordValue g_runtime_directory;
biogears::Arguments::MultiwordValue g_requested_table_formats;
biogears::Arguments::MultiwordValue g_patient_population_args;

constexpr int BG_CLI_SUCCESS = 0;
constexpr int BG_CLI_PARSE_ERROR = 1;
constexpr int BG_CLI_MISSING_ARGUMENT = 2;
constexpr int BG_CLI_SHA_FAILURE = 3;
void print_help(int rc)
{
  std::cout << "Usage bg-cli [FLAGS] COMMAND...\n";

  std::cout << "\nCommands:";
  std::cout << "\nConfig config_file...               : Runs a series of parallel biogears scenarios based on the contents of the provided files. For more information see project wiki.";
  std::cout << "\nScenario scenario_file...           : Staright foward way to kick of a CDM::Scenario format xml file.  When multiple scenario files are given they will be run in parallel up to -j jobs";
  std::cout << "\nGenerate [mode]...                  : Use to sepcify one more data elements to generate";
#ifdef BIOGEARS_IO_PRESENT
  std::cout << "\n         runtime  destination       : Generates release rutime files at the specificed path";
#endif
  std::cout << "\n         data                       : Generates xml resource files from various csv templtes in the RUNTIME directory";
  std::cout << "\n         states                     : Generates a stabalized patient state for each known patient";
  std::cout << "\n         sepsis                     : Generates several septic patient sates based on GenSepsisStates.config";
  std::cout << "\n         patients [template N]..| N : Generates a population of N patients for each patient tempalte file";
  std::cout << "\n         tables html|md|xml|web|all : Generates data tables based on prior run validation for patients and systems in one of the requested formats";
  std::cout << "\nValidate [mode]...                  : Runs preexisting BioGears scenarios with the linked engine to validate agianst baseline results";
  std::cout << "\n         patient                    : Equivilant to bg-cli CONFIG ValidationPatient.config";
  std::cout << "\n         drug                       : Equivilant to bg-cli CONFIG ValidationDrug.config";
  std::cout << "\n         system                     : Equivilant to bg-cli CONFIG ValidationSystem.config";
  std::cout << "\n         all                        : Equivilant to bg-cli VALIDATE patient drug system VERIFY";
  std::cout << "\nVerify                              : Equivilant to bg-cli CONFIG VerificatioNScenarios.config";
  std::cout << "\nTest                                : Runs a series of predetermined unit test and generates ar report";
  std::cout << "\n         cdm                        : Equivilant to bg-cli CONFIG CDMUnitTest.config";
  std::cout << "\n         bge                        : Equivilant to bg-cli CONFIG BGEUnitTest.config";

  std::cout << "\n\nFlags:";
  std::cout << "\n-v, VERSION: Print Version.";
  std::cout << "\n-j, JOBS N        : Determines the parallisim of biogears runs N.";
  std::cout << "\n-h, HELP          : Print this message.";
#if defined(BIOGEARS_SUBPROCESS_SUPPORT)
  std::cout << "\n-t                : Use Threads instead of Subprocess.";
#endif
  std::cout << "\nNotes";
  std::cout << "\nbg-cli arguments are not case-sensitive.";
  std::cout << std::endl;
  exit(rc);
}

#ifdef BIOGEARS_IO_PRESENT
bool genRuntime(std::string pathName)
{
  biogears::IOManager iom;
  return iom.generate_runtime_directory(pathName.c_str());
}
#endif

void parse_generate_arguments(biogears::Arguments::MultiwordValue&& args)
{

  auto is_keyword = [](const std::string& v) { return v == "data" || v == "patients" || v == "runtime" || v == "states" || v == "sepsis" || v == "tables"; };
  auto current = args.begin();
  auto end = args.end();
  for (; current != end;  ++current) {
    const auto& arg = *current;
    if (arg == "data") {
      g_run_generate_data = true;
    } else if (arg == "patients") {
      if (current + 1 != end && is_keyword(*(current + 1))) {
        std::cout << "GENERATE patients requires at least one argument.\n";
        print_help(BG_CLI_MISSING_ARGUMENT);
      };
      g_run_generate_populations = true;
      while (current + 1 != end) {
        ++current;
        std::transform(current->begin(), current->end(), current->begin(), ::tolower);
        if (is_keyword(*current)) {
          break;
        }
        g_patient_population_args.push_back(*current);
      }
    } else if (arg == "runtime") {
      if (current + 1 != end && is_keyword(*(current + 1))) {
        std::cout << "GENERATE runtime requires at destination\n";
        print_help(BG_CLI_MISSING_ARGUMENT);
      };
      ++current;
      g_runtime_directory = *current;
      g_run_generate_runtime_directory = true;
    } else if (arg == "states") {
      g_run_generate_patient_states = true;
    } else if (arg == "sepsis") {
      g_run_generate_sepsis_states = true;
    } else if (arg == "tables") {

      while (current + 1 != end) {
        ++current;
        std::transform(current->begin(), current->end(), current->begin(), ::tolower);
        if (*current == "html") {
          g_run_generate_tables = true;
          g_requested_table_formats.push_back("html");
        } else if (*current == "md") {
          g_run_generate_tables = true;
          g_requested_table_formats.push_back("md");
        } else if (*current == "xml") {
          g_run_generate_tables = true;
          g_requested_table_formats.push_back("xml");
        } else if (*current == "web") {
          g_run_generate_tables = true;
          g_requested_table_formats.push_back("web");
        } else if (*current == "all") {
          g_run_generate_tables = true;
          g_requested_table_formats.push_back("all");
        } else {
          --current;
        }
        if (g_requested_table_formats.size() == 0) {
          std::cout << "Scenario must be provided a valid mode html,md,xml,all; but " << *current << " found.\n";
          print_help(BG_CLI_MISSING_ARGUMENT);
        }
      }
      if (g_requested_table_formats.size() == 0) {
        std::cout << "Scenario must be provided a valid mode html,md,xml,all; but " << *current << " found.\n";
        print_help(BG_CLI_MISSING_ARGUMENT);
      }
    }
  }
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
    { "H", "HELP", "GENDATA", "GENSEPSIS", "GENSTATES", "VERIFY", "V", "VERSION" }, /*Options*/
    { "J", "JOBS" }, /*Keywords*/
    { "TEST", "CONFIG", "SCENARIO", "VALIDATE", "GENTABLES", "GENPATIENTS", "GENERATE", "GEN" } /*MultiWords*/
  );

#if defined(BIOGEARS_SUBPROCESS_SUPPORT)
  args.append_options({ "THREADED", "T" });
#endif
#if defined(BIOGEARS_IO_PRESENT)
  args.append_keywords({ "SHA1", "GENRUNTIME" });
#endif

  unsigned int thread_count = std::thread::hardware_concurrency();
  if (!args.parse(argc, argv) ) {
    std::cerr << args.error_msg() << "\n";
    print_help(BG_CLI_PARSE_ERROR);
  } else if (args.empty())  {
     std::cerr << "No arguments supplied\n.";
    print_help(BG_CLI_PARSE_ERROR);
  } else if(  args.Option("HELP") || args.Option("H") ) {
    print_help(BG_CLI_SUCCESS);
  }

  if (args.Option("VERSION") || args.Option("V")) {
    std::cout << "Using " << biogears::branded_version_string_str() << std::endl;
    exit(BG_CLI_SUCCESS);
  }

  if (args.KeywordFound("JOBS")) {
    try {
      thread_count = std::stoi(args.Keyword("JOBS"));
    } catch (std::exception) {
      std::cerr << "Error: JOBS given but " << args.Keyword("JOBS") << " is not a valid Integer.\n";
      print_help(BG_CLI_MISSING_ARGUMENT);
    }
  }
  if (args.KeywordFound("J")) {
    try {
      thread_count = std::stoi(args.Keyword("J"));
    } catch (std::exception) {
      std::cerr << "Error: J given but " << args.Keyword("J") << " is not a valid Integer.\n";
      print_help(BG_CLI_MISSING_ARGUMENT);
    }
  }
#ifdef BIOGEARS_IO_PRESENT
  if (args.KeywordFound("SHA1")) {
    biogears::IOManager iom;
    auto path = args.Keyword("SHA1");
    auto sha1 = iom.calculate_sha1(path.c_str());
    if (sha1.empty()) {
      std::cout << "Failed to calculate SHA1: Unable to read " << args.Keyword("GENRUNTIME") << std::endl;
      exit(BG_CLI_SHA_FAILURE);
    }
    std::cout << path << " : " << sha1;
    if (iom.does_embedded_file_exist(path.c_str())) {
      auto expected_sha1 = iom.get_expected_sha1(path.c_str());
      std::cout << " : ";

      auto result = sha1.compare(expected_sha1);
      if (result == 0) {
        std::cout << "matched";
      } else {
        std::cout << std::string("expected ") + expected_sha1;
      }
    } else {
      std::cout << std::endl;
    }
    exit(BG_CLI_SUCCESS);
  }
#endif

  biogears::Driver driver { argv[0], thread_count };

  //
  //  This reads a predefined configuration file to setup email notification values
  //  TODO: Implement Email Notifications
  const biogears::Config conf { "Email.config", true };
  driver.configure(conf);

  bool as_subprocess = false;
#if defined(BIOGEARS_SUBPROCESS_SUPPORT)
  as_subprocess = true;
  as_subprocess = !(args.Option("THREADED") || args.Option("T"));
#endif

  //Argument Parsing for 2.0 Arguments
  //                     and legacy 1.0 GEN args
  if (args.MultiWordFound("GENERATE")) {
    parse_generate_arguments( args.MultiWord("GENERATE") );
  }
  if (args.MultiWordFound("GEN")) {
    parse_generate_arguments( args.MultiWord("GEN"));
  }
  if (args.Option("GENSTATES")) {
    g_run_generate_patient_states = true;
  }
  if (args.Option("GENSEPSIS")) {
    g_run_generate_sepsis_states = true;
  }
  if (args.MultiWordFound("GENPATIENTS")) {
    g_run_generate_populations = true;
    g_patient_population_args = args.MultiWord("GENPATIENTS");
  }
  if (args.MultiWordFound("GENTABLES")) {
    g_run_generate_tables = true;
    g_requested_table_formats = args.MultiWord("GENTABLES");
  }
  if (args.Option("GENDATA")) {
    g_run_generate_data = true;
  }
#ifdef BIOGEARS_IO_PRESENT
  if (args.KeywordFound("GENRUNTIME")) {
    g_run_generate_runtime_directory = true;
    g_runtime_directory = args.Keyword("GENRUNTIME");
  }
#endif
//
//  Generation routines
//
#ifdef BIOGEARS_IO_PRESENT
  if (g_run_generate_runtime_directory) {
    std::cout << "Generating runtime for " << biogears::full_version_string() << " in " << g_runtime_directory;
    bool success = genRuntime(g_runtime_directory);
    exit((success) ? 0 : 1);
  }
#endif

  if (g_run_generate_patient_states) {
    const biogears::Config runs { "GenStates.config" };
    driver.queue(runs, as_subprocess);
  }
  if (g_run_generate_sepsis_states) {
    const biogears::Config runs { "GenSepsisStates.config" };
    driver.queue(runs, as_subprocess);
  }
  if (g_run_generate_populations) {
    biogears::PopulationGenerator generator { g_patient_population_args };
    generator.Generate();
  }
  if (g_run_generate_data) { // gen-data
    std::vector<std::unique_ptr<biogears::CSVToXMLConvertor>> generators;
    generators.push_back(std::make_unique<biogears::SubstanceGenerator>());
    generators.push_back(std::make_unique<biogears::EnvironmentGenerator>());
    generators.push_back(std::make_unique<biogears::PatientGenerator>());
    generators.push_back(std::make_unique<biogears::StabilizationGenerator>());
    generators.push_back(std::make_unique<biogears::NutritionGenerator>());
    generators.push_back(std::make_unique<biogears::CompoundGenerator>());
    for (auto& gen : generators) {
      std::cout << "Generating Data: " << gen->Path() << gen->Filename() << "\n";
      gen->parse();
      gen->save();
      std::cout << "\n\n";
    }
  }

  if (args.Option("VERIFY")) {
    g_run_verification_routine = true;
  }

#ifdef CMD_BIO_SUPPORT_CIRCUIT_TEST
  if (args.MultiWordFound("TEST")) {
    auto tests = args.MultiWord("TEST");
    for (auto& test : tests) {
      std::transform(test.begin(), test.end(), test.begin(), ::tolower);
      if (test == "cdm") { // run-cdm-tests
        biogears::Config runs { "CDMUnitTests.config" };
        driver.queue(runs, as_subprocess);
      } else if (test == "bge") {
        biogears::Config runs { "BGEUnitTests.config" };
        driver.queue(runs, as_subprocess);
      } else {
        std::cout << "Warning: No Test known as " << test << " exists.\n";
      }
    }
  }
#endif

  if (args.MultiWordFound("VALIDATE")) {
    auto tests = args.MultiWord("VALIDATE");
    for (auto& test : tests) {
      std::transform(test.begin(), test.end(), test.begin(), ::tolower);
      if (test == "patient") {
        g_run_patient_validation = true;
      } else if (test == "drug") {
        g_run_drug_validation = true;
      } else if (test == "system") {
        g_run_system_validation = true;
      } else if (test == "all") {
        g_run_patient_validation = g_run_drug_validation = g_run_system_validation = g_run_verification_routine = true;
      } else {
        std::cout << "Warning: No Validation known as " << test << " exists.\n";
      }
    }
  }

  if (g_run_system_validation) { // run-system-validation
    const auto runs = biogears::Config("ValidationSystems.config");
    driver.queue(runs, as_subprocess);
  }
  if (g_run_patient_validation) { //run-patient-validation
    const auto runs = biogears::Config("ValidationPatients.config");
    driver.queue(runs, as_subprocess);
  }
  if (g_run_drug_validation) { // run-drug-validation
    const auto runs = biogears::Config("ValidationDrugs.config");
    driver.queue(runs, as_subprocess);
  }
  if (g_run_verification_routine) { // run-verification
    const auto runs = biogears::Config("VerificationScenarios.config");
    driver.queue(runs, as_subprocess);
  }

  if (args.MultiWordFound("CONFIG")) {
    auto configs = biogears::Config {};
    for (auto& arg : args.MultiWord("CONFIG")) {
      const auto runs = biogears::Config(arg);
      driver.queue(runs, as_subprocess);
    }
  }

  if (args.MultiWordFound("SCENARIO")) {
    auto configs = biogears::Config {};
    for (auto& arg : args.MultiWord("SCENARIO")) {
      auto ex = biogears::Executor { arg, biogears::EDriver::ScenarioTestDriver };
      ex.Computed("Scenarios/");
      ex.Scenario(arg);
      configs.push_back(ex);
    }
    driver.queue(configs, as_subprocess);
  }

  if (driver.has_work()) {
    driver.run();
    driver.stop_when_empty();
    driver.join();
  }
  //We want Gentables to run after all other work has finished
  if (g_run_generate_tables) {
    biogears::ReportWriter report_writer;
    for (auto& table : g_requested_table_formats) {
      std::transform(table.begin(), table.end(), table.begin(), ::tolower);
      if (table == "html") {
        report_writer.generate_system_tables(biogears::ReportWriter::HTML);
        report_writer.generate_patient_tables(biogears::ReportWriter::HTML);
      } else if (table == "md") {
        report_writer.generate_system_tables(biogears::ReportWriter::MD);
        report_writer.generate_patient_tables(biogears::ReportWriter::MD);
      } else if (table == "xml") {
        report_writer.generate_system_tables(biogears::ReportWriter::XML);
        report_writer.generate_patient_tables(biogears::ReportWriter::XML);
      } else if (table == "web") {
        report_writer.generate_system_tables(biogears::ReportWriter::WEB);
        report_writer.generate_patient_tables(biogears::ReportWriter::WEB);
      } else if (table == "all") {
        report_writer.generate_system_tables(biogears::ReportWriter::HTML);
        report_writer.generate_system_tables(biogears::ReportWriter::MD);
        report_writer.generate_system_tables(biogears::ReportWriter::XML);

        report_writer.generate_patient_tables(biogears::ReportWriter::HTML);
        report_writer.generate_patient_tables(biogears::ReportWriter::MD);
        report_writer.generate_patient_tables(biogears::ReportWriter::XML);
      } else {
        std::cout << "Warning: " << table << " is not a valid keyword.\n";
      }
    }
  }

  return 0;
}
