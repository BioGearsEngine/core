#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenario.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenarioExec.h>
#include <biogears/cdm/test/CommonDataModelTest.h>
//
#include <biogears/engine/Controller/BioGears.h>

//
#include "CDMDriver.h"
#include "utils/string-helpers.h"
#include <iostream>
#include <vector>
#include <string>

//
namespace biogears {
int RunCDMTests(const std::string testName, const std::string outputDir);

//-------------------------------------------------------------------------------
CDMDriver::CDMDriver(size_t thread_count)
  : _pool(thread_count)
{
}
//-------------------------------------------------------------------------------
CDMDriver::~CDMDriver()
{
}

//-------------------------------------------------------------------------------
//!
//! \brief Iterates through patientFiles, creates a lambda function for each item, and passes those functions to a thread pool
//! 
void CDMDriver::RunCDMs(std::vector<std::string> files)
{
  for (auto& testName : files) {
    std::string outputDir = "./Validation/";
    outputDir.append(testName);
    std::function<void()> work = [=](){ biogears::RunCDMTests(testName,outputDir); };
    _pool.queue_work(work);
  }
}
//-------------------------------------------------------------------------------

//!
//! \brief creates a BioGearsScenarioExec object and executes the scenario
//! \param testName : the name of the patient being used for the scenario
//! \param outputDir : a path to the output directory
//! \return int 0 if no exceptions were encountered, otherwise 1
//!
int RunCDMTests(const std::string testName, const std::string outputDir)
{
  CommonDataModelTest* executor;
  try {
    executor = new CommonDataModelTest;
  } catch (std::exception e) {
    std::cout << e.what();
    return 1;
  }
  executor->RunTest(testName, outputDir);
  delete executor;
  return 0;
}
//-------------------------------------------------------------------------------
//!
//! \brief thread pool begins execution of tasks in queue
//! 
void CDMDriver::run()
{
  _pool.start();
}
//-------------------------------------------------------------------------------
//!
//! \brief stops execution of tasks in queue
//! 
void CDMDriver::stop()
{
  _pool.stop();
}
//-------------------------------------------------------------------------------
//!
//! \brief stops the thread pool if the work queue is empty
//! \return true if the work queue is empty, false otherwise
//! 
bool CDMDriver::stop_if_empty()
{  
  return _pool.stop_if_empty();
}
//-------------------------------------------------------------------------------
//!
//! \brief joins threads in thread pool
//! 
void CDMDriver::join()
{
  _pool.join();
}
} // namespace biogears
