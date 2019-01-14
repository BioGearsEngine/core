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
#ifndef TEST_DRIVER_VERIFICATION_H
#define TEST_DRIVER_VERIFICATION_H

#include <biogears/cdm/utils/FileUtils.h>
#include <biogears/cdm/utils/ConfigParser.h>
#include <biogears/cdm/utils/TaskRunner/TaskRunner.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenarioExec.h>

#include <string>
#include <vector>
#include <sstream>
  
namespace test_driver
{

  enum class ScenarioRunType
  {
    Run,
    Skip,
    RunRed
  };

  class RunScenarioTask : public biogears::Task
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
  test_driver::ScenarioRunType GetRunType(const std::string& scenarioName) const;

  std::string m_configFile;
  biogears::ConfigParser m_parser;

  std::vector<VerificationSet> m_verificationSets;

  unsigned int m_verificationSummaryCount = 0;
  RunMode m_mode = RunMode::Default;
};

#endif //TEST_DRIVER_VERIFICATION_H