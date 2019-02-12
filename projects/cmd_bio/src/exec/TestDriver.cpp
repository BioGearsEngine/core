#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenario.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenarioExec.h>

#include "TestDriver.h"
#include "../utils/string-helpers.h"
#include <iostream>
#include <vector>
#include <string>
//
namespace biogears {
int runScenarioTests(const std::string patient, std::string&& XMLString);

//-------------------------------------------------------------------------------
TestDriver::TestDriver(size_t thread_count)
  : _pool(thread_count)
{
}
//-------------------------------------------------------------------------------
TestDriver::~TestDriver()
{
}
//-------------------------------------------------------------------------------
//!
//! \brief Iterates through patientFiles, creates a lambda function for each item, and passes those functions to a thread pool
//! 
void TestDriver::RunTests(std::vector<std::string> files)
{
  for (auto& patient : files) {
    std::function<void()> work = [=](){ biogears::runScenarioTests(patient, std::string("Scenarios/Validation/Patient-Validation.xml")); };
    _pool.queue_work(work);
  }
}
//-------------------------------------------------------------------------------

//!
//! \brief creates a BioGearsScenarioExec object and executes the scenario
//! \param patient : the name of the patient being used for the scenario
//! \param XMLString : a path to the xml scenario being used
//! \return int 0 if no exceptions were encountered, otherwise 1
//!
int runScenarioTests(const std::string patient, std::string&& XMLString)
{
  std::string patientXML(patient);

  std::string patientLog = "-" + patientXML;
  patientLog = findAndReplace(patientLog, ".xml", ".log");

  std::string patientResults = "-" + patientXML;
  patientResults = findAndReplace(patientResults, ".xml", "Results.csv");

  std::string logFile(patient);
  std::string outputFile(patient);
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
  sce.Load(XMLString);
  sce.GetInitialParameters().SetPatientFile(patientXML);

  BioGearsScenarioExec* exec = new BioGearsScenarioExec(*eng);
  exec->Execute(sce, outputFile, nullptr);
  delete exec;

  return 0;
}
//-------------------------------------------------------------------------------
//!
//! \brief thread pool begins execution of tasks in queue
//! 
void TestDriver::run()
{
  _pool.start();
}
//-------------------------------------------------------------------------------
//!
//! \brief stops execution of tasks in queue
//! 
void TestDriver::stop()
{
  _pool.stop();
}
//-------------------------------------------------------------------------------
//!
//! \brief stops the thread pool if the work queue is empty
//! \return true if the work queue is empty, false otherwise
//! 
bool TestDriver::stop_if_empty()
{  
  return _pool.stop_if_empty();
}
//-------------------------------------------------------------------------------
//!
//! \brief joins threads in thread pool
//! 
void TestDriver::join()
{
  _pool.join();
}
} // namespace biogears
