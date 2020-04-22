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

#include <iostream>

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
#include <xsd/cxx/tree/exceptions.hxx>

#include "../utils/Config.h"
#include "../utils/Executor.h"
#include "biogears/string/manipulation.h"

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
void Driver::queue(const Config& runs)
{
  for (auto& exec : runs) {
    switch (exec.Driver()) {
    case EDriver::Undefined:
      std::cerr << "Unable to queue Undefined Executor for " << exec.Name() << "\n";
      break;
    case EDriver::BGEUnitTestDriver:
      queue_BGEUnitTest(exec);
      break;
    case EDriver::CDMUnitTestDriver:
      queue_CDMUnitTest(exec);
      break;
    case EDriver::ScenarioTestDriver:
      queue_Scenario(exec);
      break;
    default:
      std::cerr << "Unsupported Driver type " << exec.Driver() << " please update your biogears libraries ";
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
void Driver::queue_BGEUnitTest(Executor exec)
{
#ifdef CMD_BIO_SUPPORT_CIRCUIT_TEST
  _pool.queue_work(
    [=]() {
      BioGearsEngineTest* executor;
      try {
        executor = new BioGearsEngineTest;
      } catch (const std::exception& e) {
        std::cout << e.what();
        return 1;
      }
      executor->RunTest(exec.Name(), exec.Computed());
      delete executor;
      return 0;
    });
#endif
}

//-----------------------------------------------------------------------------
void Driver::queue_CDMUnitTest(Executor exec)
{
#ifdef CMD_BIO_SUPPORT_CIRCUIT_TEST
  _pool.queue_work(
    [&]() {
      CommonDataModelTest* executor;
      try {
        executor = new CommonDataModelTest;
      } catch (const std::exception& e) {
        std::cout << e.what();
        return 1;
      }
      executor->RunTest(exec.Name(), exec.Computed());
      delete executor;
      return 0;
    });
#endif
}
//-----------------------------------------------------------------------------
void Driver::queue_Scenario(Executor exec)
{
  enum class PatientType {
    FILE,
    STATE,
    INLINE
  };
  CDM::PatientData empty_patient_data; //TODO: Git Rid of PatientType and just treat everything line an inline PatientData by passing it in
  auto scenario_launch = [](Executor& ex, bool multi_patient_run, PatientType patient_type,CDM::PatientData patient_data) {
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
      //TODO::LOG ERROR Scenario File could not be loaded
    }

    switch (patient_type) {
    case PatientType::FILE:
      sce.GetInitialParameters().SetPatientFile(ex.Patient());
      break;
    case PatientType::STATE:
      sce.SetEngineStateFile(ex.Patient());
      break;
    case PatientType::INLINE: {
      biogears::SEPatient patient{ sce.GetLogger() };
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

  std::ifstream ifs { exec.Scenario() };
  if (!ifs.is_open()) {
    ifs.open("Scenarios/" + exec.Scenario());
    if (!ifs.is_open()) {
      //TODO: Log A Clean Error about Scenario not being found.
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
    std::cout << e.what();
    return;
  } catch (xsd::cxx::tree::parsing<char> e) {
    std::cout << e;
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
    exec.Patient(scenario->EngineStateFile().get());
    _pool.queue_work(std::bind(scenario_launch, std::move(exec), false, PatientType::STATE, empty_patient_data));
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
        _pool.queue_work(std::bind(scenario_launch, std::move(patientEx), true, PatientType::FILE, empty_patient_data));
      }
    } else {
      exec.Patient(patient_file);
      _pool.queue_work(std::bind(scenario_launch, std::move(exec), false, PatientType::FILE, empty_patient_data));
    }
  } else if (scenario->InitialParameters().present() && scenario->InitialParameters()->Patient().present()) {
    exec.Patient(scenario->InitialParameters()->Patient().get().Name());
    _pool.queue_work(std::bind(scenario_launch, std::move(exec), false, PatientType::INLINE, scenario->InitialParameters()->Patient().get()));
  } else {
    std::cout << "Skipping " << exec.Name() << " no patient specified.\n";
    return;
  }
}
//-----------------------------------------------------------------------------
} // NAMESPACE
