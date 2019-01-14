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


#include <biogears/cdm/utils/testing/SETestSuite.h>
#include <biogears/schema/cdm/Properties.hxx>
#include <biogears/schema/cdm/TestReport.hxx>

namespace biogears {
SETestSuite::SETestSuite(Logger* logger)
  : Loggable(logger)
{
  m_Performed = true;
  m_Name = "";
}

SETestSuite::~SETestSuite()
{
  Clear();
}

void SETestSuite::Clear()
{
  DELETE_VECTOR(m_SuiteEqualError);
  DELETE_VECTOR(m_TestCase);
}

void SETestSuite::Reset()
{
  for (unsigned int i = 0; i < m_TestCase.size(); i++) {
    m_TestCase.at(i)->Reset();
  }
  m_Performed = false;
  m_Name = "";
}

bool SETestSuite::Load(const CDM::TestSuite& in)
{
  Reset();

  std::string sData;
  for (unsigned int i = 0; i < in.Requirement().size(); i++) {
    sData = (std::string)in.Requirement().at(i);
    if (!sData.empty())
      m_Requirements.push_back(sData);
  }

  SETestErrorStatistics* ex;
  CDM::TestErrorStatisticsData* eData;
  for (unsigned int i = 0; i < in.SuiteEqualError().size(); i++) {
    eData = (CDM::TestErrorStatisticsData*)&in.SuiteEqualError().at(i);
    if (eData != nullptr) {
      ex = new SETestErrorStatistics(GetLogger());
      ex->Load(*eData);
    }
    m_SuiteEqualError.push_back(ex);
  }

  SETestCase* tx;
  CDM::TestCase* tData;
  for (unsigned int i = 0; i < in.TestCase().size(); i++) {
    tData = (CDM::TestCase*)&in.SuiteEqualError().at(i);
    if (tData != nullptr) {
      tx = new SETestCase(m_Name, GetLogger());
      tx->Load(*tData);
    }
    m_TestCase.push_back(tx);
  }
  m_Performed = in.Performed();
  m_Name = in.Name();

  return true;
}

std::unique_ptr<CDM::TestSuite> SETestSuite::Unload() const
{
  std::unique_ptr<CDM::TestSuite> data(new CDM::TestSuite());
  Unload(*data);
  return data;
}

void SETestSuite::Unload(CDM::TestSuite& data) const
{
  std::string sData;
  for (unsigned int i = 0; i < m_Requirements.size(); i++) {
    sData = m_Requirements.at(i);
    if (!sData.empty())
      data.Requirement().push_back(sData);
  }
  for (unsigned int i = 0; i < m_SuiteEqualError.size(); i++) {
    data.SuiteEqualError().push_back(*m_SuiteEqualError.at(i)->Unload());
  }
  for (unsigned int i = 0; i < m_TestCase.size(); i++) {
    data.TestCase().push_back(*m_TestCase.at(i)->Unload());
  }

  data.Performed(m_Performed);
  if (GetNumberOfTests() != 0) {
    data.Tests(GetNumberOfTests());
    data.Time(std::unique_ptr<CDM::ScalarTimeData>(GetDuration().Unload()));
    data.Errors(GetNumberOfErrors());
  }

  if (m_Name.compare("") != 0)
    data.Name(m_Name);
}

void SETestSuite::SetName(const std::string& Name)
{
  m_Name = Name;
}

std::string SETestSuite::GetName() const
{
  return m_Name;
}

const char* SETestSuite::GetName_cStr() const
{
  return m_Name.c_str();
}
void SETestSuite::PerformSuite(bool performed)
{
  m_Performed = performed;
}

bool SETestSuite::PerformedSuite()
{
  return m_Performed;
}

const SEScalarTime& SETestSuite::GetDuration() const
{
  double time = 0;
  for (unsigned int i = 0; i < m_TestCase.size(); i++)
    time += m_TestCase.at(i)->GetDuration().GetValue(TimeUnit::s);
  m_Time.SetValue(time, TimeUnit::s);
  return m_Time;
}

std::vector<std::string>& SETestSuite::GetRequirements()
{
  return m_Requirements;
}

SETestCase& SETestSuite::CreateTestCase()
{
  SETestCase* test = new SETestCase(GetLogger());
  m_TestCase.push_back(test);
  return *test;
}

const std::vector<SETestCase*>& SETestSuite::GetTestCases() const
{
  return m_TestCase;
}

int SETestSuite::GetNumberOfErrors() const
{
  size_t count = 0;

  for (size_t i = 0; i < m_TestCase.size(); i++) {
    count += m_TestCase.at(i)->GetFailures().size();
  }

  return static_cast<int>(count); //Major Possibility of a clamp
}

int SETestSuite::GetNumberOfTests() const
{
  int count = 0;

  for (unsigned int i = 0; i < m_TestCase.size(); i++) {
    count++;
  }

  return count;
}
}