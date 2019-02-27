#include "Driver.h"

#include <iostream>

#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/test/CommonDataModelTest.h>
#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenario.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenarioExec.h>
#include <biogears/engine/test/BioGearsEngineTest.h>

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
void Driver::join()
{
  _pool.join();
}
//-----------------------------------------------------------------------------
void Driver::queue_BGEUnitTest(const Executor& exec)
{
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
}

//-----------------------------------------------------------------------------
void Driver::queue_CDMUnitTest(const Executor& exec)
{
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
}
//-----------------------------------------------------------------------------
void Driver::queue_Scenario(const Executor& exec)
{

  _pool.queue_work(
    [=]() {
      std::string& patient = exec.Name();
      std::string XMLString;

      std::string patientXML(trim(patient));

      std::string patientLog = "-" + patientXML;
      patientLog = findAndReplace(patientLog, ".xml", ".log");

      std::string patientResults = "-" + patientXML;
      patientResults = findAndReplace(patientResults, ".xml", "Results.csv");

      std::string logFile(trim(patient));
      std::string outputFile(trim(patient));
      logFile = findAndReplace(logFile, ".xml", "Results.log");
      outputFile = findAndReplace(outputFile, ".xml", "Results.csv");

      std::unique_ptr<PhysiologyEngine> eng;
      try {
        eng = CreateBioGearsEngine(logFile);
      } catch (std::exception e) {
        std::cout << e.what();
        return 1;
      }
      DataTrack* trk = &eng->GetEngineTrack()->GetDataTrack();
      BioGearsScenario sce(eng->GetSubstanceManager());
      sce.Load(trim(XMLString));
      sce.GetInitialParameters().SetPatientFile(trim(patientXML));

      BioGearsScenarioExec* exec = new BioGearsScenarioExec(*eng);
      exec->Execute(sce, outputFile, nullptr);
      delete exec;

      return 0;
    });
}
//-----------------------------------------------------------------------------
}
