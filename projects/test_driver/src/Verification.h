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
#pragma once

#include <biogears/cdm/utils/FileUtils.h>
#include <biogears/cdm/utils/ConfigParser.h>
#include <biogears/cdm/utils/TaskRunner/TaskRunner.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenarioExec.h>

#include <string>
#include <vector>
#include <sstream>
  
namespace
{

  enum class ScenarioRunType
  {
    Run,
    Skip,
    RunRed
  };

  class RunScenarioTask : public Task
  {
  public:
    static std::mutex ms_constructionMutex;

    RunScenarioTask(const std::string& scenarioFile)
      : m_scenarioFile(scenarioFile)
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
      std::cerr << "Unable to create BioGearsEngine" << std::endl;
      return;
    }

    // Run the scenario
    try
    {
      BioGearsScenarioExec exec(*bioGears);
      exec.Execute(m_scenarioFile.c_str(), dataFile.c_str(), nullptr);
    }
    catch (std::exception ex)
    {
      std::cerr << ex.what() << std::endl;
    }
    catch (...)
    {
      std::cerr << "Unable to run scenario " << m_scenarioFile << std::endl;
    }
  }

}

struct VerificationSet
{
  std::string m_subject;
  std::string m_sender;
  std::string m_smtp;
  std::string m_reportDirectory;
  double m_percentDifference = 2.0;
  double m_timeStart_s = 0.0;
  std::vector<std::string> m_recipients;
  std::vector<std::tuple<std::string, std::string>> m_scenarios;
};

class Verification
{
public:
  enum class RunMode
  {
    Default,
    All,
    KnownFailing
  };

  Verification(const std::string& configFile, RunMode mode);
  void Verify();

private:
  void BuildVerificationSets();
  void GenerateVerificationSummary(const VerificationSet& verificationSet, unsigned int groupNumber) const;
  ScenarioRunType GetRunType(const std::string& scenarioName) const;

  std::string m_configFile;
  ConfigParser m_parser;

  std::vector<VerificationSet> m_verificationSets;

  unsigned int m_verificationSummaryCount = 0;
  RunMode m_mode = RunMode::Default;
};