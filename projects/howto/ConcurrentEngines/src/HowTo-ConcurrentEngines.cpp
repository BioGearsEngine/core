/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/



// Include the various types you will be using in your code
#include <biogears/cdm/utils/FileUtils.h>
#include <biogears/cdm/utils/TaskRunner/TaskRunner.h>

#include <biogears/cdm/scenario/SEScenarioExec.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>


using namespace biogears;
//--------------------------------------------------------------------------------------------------
/// \brief
/// This class derives from Task and implements Run() so it can be used in the TaskRunner.  When run,
/// this class creates a BioGears engine and runs a scenario.
//--------------------------------------------------------------------------------------------------
class RunScenarioTask : public Task
{
public:
    static std::mutex ms_constructionMutex;

    RunScenarioTask(const std::string& scenarioFile)
          :  m_scenarioFile(scenarioFile)
    {
    }

    virtual void Run() override;

private:
    std::string m_scenarioFile;     ///< The scenario file to run when this task is executed
};

std::mutex RunScenarioTask::ms_constructionMutex;

//--------------------------------------------------------------------------------------------------
/// \brief
/// This function is called when the task is executed, it creates a BioGearsEngine and executes the scenario
//--------------------------------------------------------------------------------------------------
void RunScenarioTask::Run()
{
  // Set up the log file
	std::string logFile = m_scenarioFile;
	logFile = Replace(logFile, "verification", "bin");
	logFile = Replace(logFile, ".xml", ".log");

	// Set up the verification output file
	std::string dataFile = m_scenarioFile;
	dataFile = Replace(dataFile, "verification", "bin");
	dataFile = Replace(dataFile, ".xml", "Results.csv");

	// Delete any results file that may be there
	remove(dataFile.c_str());

  // Aquire the constrution mutex before we create the BioGearsEngine.  Due to some third-party library
	// initialization constructs not being thread safe, we must not construct BioGearsEngines simultaneously
	// from multiple threads.
  ms_constructionMutex.lock();
	std::unique_ptr<PhysiologyEngine> bioGears = CreateBioGearsEngine(logFile.c_str());
  ms_constructionMutex.unlock();
        
  if (!bioGears)
	{
		std::cerr << "Unable to create BioGearsEngine\n";
		return;
	}

  // Run the scenario
	SEScenarioExec exec(*bioGears);
	exec.Execute(m_scenarioFile.c_str(), dataFile.c_str(), nullptr);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for running concurrent engines
//--------------------------------------------------------------------------------------------------
void HowToConcurrentEngines() 
{
    // Create the task runner, this is the object that will handle running tasks in parallel.
    // By default it attempts to determine the number of cores on the system and creates a thread
    // per core.  If it cannot determine this value, it reverts to single-threaded mode.  Override
    // the thread count by specifying the desired number of threads in the constructor.
    TaskRunner runner;

    // Create tasks to be run and give them to the task runner
    runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("Scenarios/Patient/BasicStandard.xml")));
    runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("Scenarios/Patient/BasicStandard.xml")));
    runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("Scenarios/Patient/BasicStandard.xml")));

    // Run the tasks.  This will launch the desired number of threads which will pull and execute tasks
    // until there are none left.  Run() blocks until all task threads are finished executing.
    runner.Run();
}
