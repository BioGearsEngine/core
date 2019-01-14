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

#include <tuple>
#include <fstream>
#include <memory>

#include "TestDriver.h"
#include "Verification.h"

#ifdef ANDROID
namespace std {
  template <typename T>
  std::string to_string( T value ) {
    std::stringstream ss;
    ss << value;
    return ss.str();
  }
}
#endif

namespace test_driver
{

  std::mutex RunScenarioTask::ms_constructionMutex;

const std::string subjectKey = "subject";
const std::string senderKey = "sender";
const std::string smtpKey = "smtp";
const std::string reportDirectoryKey = "reportDir";
const std::string recipientsKey = "recipients";
const std::string percentDifferenceKey = "percentDifference";
const std::string timeStartKey = "timeStart";

std::vector<std::string> ParseRecipients(const std::string& recipients)
{
    std::vector<std::string> recipientList;

    std::stringstream ss(recipients);
    std::string token;

    while (std::getline(ss, token, ','))
    {
        std::reverse(begin(token), end(token));
        token.erase(begin(token), std::find_if(begin(token), end(token), [](char c){ return c != ' '; }));
        
        std::reverse(begin(token), end(token));
        token.erase(begin(token), std::find_if(begin(token), end(token), [](char c){ return c != ' '; }));

        recipientList.push_back(token);
    }

    return recipientList;
}
}

using namespace biogears;
using namespace test_driver;
//--------------------------------------------------------------------------------------------------
/// \brief
/// This function is called when the task is executed, it creates a BioGearsEngine and executes the scenario
//--------------------------------------------------------------------------------------------------
void RunScenarioTask::RunScenarioTask::Run()
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

Verification::Verification(const std::string& configFile, RunMode mode)
    : m_configFile(configFile),
      m_parser(m_configFile),
      m_mode(mode)
{
    BuildVerificationSets();
}

void Verification::BuildVerificationSets()
{
    for (const ConfigSet& set : m_parser.GetConfigSets())
    {
        m_verificationSets.push_back(VerificationSet());
        for (const std::pair<std::string, std::string>& configPair : set.GetKeyValues())
        {
            if (configPair.first == subjectKey)
            {
                m_verificationSets.back().m_subject = configPair.second;
            }
            else if (configPair.first == senderKey)
            {
                m_verificationSets.back().m_sender = configPair.second;
            }
            else if (configPair.first == smtpKey)
            {
                m_verificationSets.back().m_smtp = configPair.second;
            }
            else if (configPair.first == reportDirectoryKey)
            {
                m_verificationSets.back().m_reportDirectory = configPair.second;
            }
            else if (configPair.first == recipientsKey)
            {
                m_verificationSets.back().m_recipients = ParseRecipients(configPair.second);
            }
            else if (configPair.first == percentDifferenceKey)
            {
                std::istringstream iss(configPair.second);
                iss >> m_verificationSets.back().m_percentDifference;
            }
            else if (configPair.first == timeStartKey)
            {
                std::istringstream iss(configPair.second);
                iss >> m_verificationSets.back().m_timeStart_s;
            }
            else
            {
                m_verificationSets.back().m_scenarios.push_back(std::make_tuple(configPair.first, configPair.second));
            }
        }
    }
}

void Verification::Verify()
{
    for (const VerificationSet& set : m_verificationSets)
    {
        TaskRunner runner;

        for (const std::tuple<std::string, std::string>& scenarioPair : set.m_scenarios)
        {
            std::string scenarioName;
            std::string scenarioFilePath;

            std::tie(scenarioName, scenarioFilePath) = scenarioPair;

            ScenarioRunType runType = GetRunType(scenarioName);

            if (runType != ScenarioRunType::Skip)
            {
                if ((m_mode == RunMode::All) ||
                    (m_mode == RunMode::KnownFailing &&  runType == ScenarioRunType::RunRed) ||
                    (m_mode == RunMode::Default      && (runType == ScenarioRunType::Run)))
                {
                    scenarioFilePath = "Scenarios/" + scenarioFilePath;

                    runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask(scenarioFilePath)));
                }
            }
        }

        runner.Run();

        GenerateVerificationSummary(set, m_verificationSummaryCount++);
    }
}

void Verification::GenerateVerificationSummary(const VerificationSet& verificationSet, unsigned int groupNumber) const
{
    std::string filePath = std::string("VerificationSets/") + m_configFile;

    std::string::size_type dotIndex = filePath.rfind(".");
    if (dotIndex == std::string::npos)
    {
        filePath += std::to_string(groupNumber);
    }
    else
    {
        filePath.insert(dotIndex, std::to_string(groupNumber));
    }

    std::ofstream out(filePath);

    out << subjectKey << "=" << verificationSet.m_subject << "\n"
        << senderKey << "=" << verificationSet.m_sender << "\n"
        << smtpKey << "=" << verificationSet.m_smtp << "\n"
        << recipientsKey << "=";

    std::ostringstream oss;
    for (const std::string& recipient : verificationSet.m_recipients)
    {
        oss << recipient << ", ";
    }

    std::string recipients = oss.str();
    recipients = recipients.substr(0, recipients.size() - 2);

    out << recipients << "\n"
        << reportDirectoryKey << "=" << verificationSet.m_reportDirectory << "\n"
        << percentDifferenceKey << "=" << verificationSet.m_percentDifference << "\n"
        << timeStartKey << "=" << verificationSet.m_timeStart_s << "\n";

    for (const std::tuple<std::string, std::string>& scenarioPair : verificationSet.m_scenarios)
    {
        std::string scenarioName;
        std::string scenarioFilePath;

        std::tie(scenarioName, scenarioFilePath) = scenarioPair;

        ScenarioRunType runType = GetRunType(scenarioName);

        if (m_mode == RunMode::KnownFailing && runType != ScenarioRunType::RunRed)
        {
            continue;
        }

        std::string runTypeIndicator = "";
        if (runType == ScenarioRunType::RunRed && m_mode == RunMode::Default)
        {
            runTypeIndicator = "*";
        }
        else if (runType == ScenarioRunType::Skip)
        {
            runTypeIndicator = "-";
        }

        out << runTypeIndicator << scenarioName << "=" << scenarioFilePath << "\n";
    }

    out.close();
}

ScenarioRunType Verification::GetRunType(const std::string& scenarioName) const
{
    ScenarioRunType runType = ScenarioRunType::Run;
    if (!scenarioName.empty())
    {
        if (scenarioName[0] == '-')
        {
            runType = ScenarioRunType::Skip;
        }
        else if (scenarioName[0] == '*')
        {
            runType = ScenarioRunType::RunRed;
        }
    }

    return runType;
}
