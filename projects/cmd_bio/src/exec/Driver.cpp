#include "Driver.h"

#include <iostream>

#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenario.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenarioExec.h>
#ifdef CMD_BIO_SUPPORT_CIRCUIT_TEST
#include <biogears/engine/test/BioGearsEngineTest.h>
#include <biogears/cdm/test/CommonDataModelTest.h>
#endif

#include "../utils/string-helpers.h"

#include "../utils/Config.h"
#include "../utils/Executor.h"

namespace biogears {
Driver::Driver(size_t thread_count)
  : _pool{ thread_count }
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
      std::cerr << "Unsupported Driver type " << exec.Driver() << "please update your biogears libraries ";
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
    while ( !_pool.stop_if_empty() )
    {
      std::this_thread::sleep_for( std::chrono::seconds(1) );
    }
}
//-----------------------------------------------------------------------------
void Driver::join()
{
  _pool.join();
}
//-----------------------------------------------------------------------------
void Driver::queue_BGEUnitTest(const Executor& exec)
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
void Driver::queue_CDMUnitTest(const Executor& exec)
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
void Driver::queue_Scenario(const Executor& exec)
{
  const auto& patient_path = exec.Patient();
  std::string scenario_path = exec.Scenario();

  std::string nc_patient_path = exec.Patient();
  std::transform(nc_patient_path.begin(), nc_patient_path.end(), nc_patient_path.begin(), ::tolower);

  auto scenario_launch = [](Executor ex) {
    std::string trimed_patient_path(trim(ex.Patient()));
    auto split_patient_path = split(trimed_patient_path, '/');
    auto patient_no_extension = split(split_patient_path.back(), '.').front();

    std::string trimed_scenario_path(trim(ex.Scenario()));
    auto split_scenario_path = split(trimed_scenario_path, '/');
    auto scenario_no_extension = split(split_scenario_path.back(), '.').front();


    //NOTE: This looses non relative prefixes as the split will eat the leading path_seperator
    std::string parent_dir;
    for (auto dir = split_scenario_path.begin(); dir != split_scenario_path.end(); ++dir) {
      if (dir + 1 != split_scenario_path.end()) {
        parent_dir.append(*dir);
        parent_dir += "/";
      }
    }

    std::string console_file = patient_no_extension + "-" + scenario_no_extension + ".log";
    std::string log_file = patient_no_extension + "-" + scenario_no_extension + "Results.log";
    std::string results_file = patient_no_extension + "-" + scenario_no_extension + "Results.csv";

    std::unique_ptr<PhysiologyEngine> eng;
    try {
      eng = CreateBioGearsEngine(ex.Computed()+parent_dir+console_file);
    } catch (std::exception e) {
      std::cout << e.what();
    }

    BioGearsScenario sce(eng->GetSubstanceManager());
    sce.Load("Scenarios/" + trim(trimed_scenario_path));
    sce.GetInitialParameters().SetPatientFile(trim(trimed_patient_path));

    BioGearsScenarioExec* exec = new BioGearsScenarioExec(*eng);
    exec->Execute(sce, ex.Computed()+parent_dir+results_file, nullptr);
    delete exec;
  };

  if (nc_patient_path == "all") {
    auto patientEx{ exec };
    auto patient_files = biogears::ListFiles("patients", R"(\.xml)");
    for (const std::string& patient_file : patient_files) {
      patientEx.Patient(patient_file);
      _pool.queue_work(std::bind(scenario_launch, patientEx));
    }
  } else {
    _pool.queue_work(std::bind(scenario_launch, exec));
  }
}
//-----------------------------------------------------------------------------
}
