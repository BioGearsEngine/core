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
#include <functional>
#include <numeric>

#include "io/cdm/Property.h"

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
//-------------------------------------------------------------------------------
SETestSuite::~SETestSuite()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SETestSuite::Clear()
{
  DELETE_VECTOR(m_SuiteEqualError);
  DELETE_VECTOR(m_TestCase);
}
//-------------------------------------------------------------------------------
void SETestSuite::Reset()
{
  for (unsigned int i = 0; i < m_TestCase.size(); i++) {
    m_TestCase.at(i)->Reset();
  }
  m_Performed = false;
  m_Name = "";
}
//-------------------------------------------------------------------------------
void SETestSuite::SetName(const std::string& Name)
{
  m_Name = Name;
}
//-------------------------------------------------------------------------------
std::string SETestSuite::GetName() const
{
  return m_Name;
}
//-------------------------------------------------------------------------------
const char* SETestSuite::GetName_cStr() const
{
  return m_Name.c_str();
}
//-------------------------------------------------------------------------------
void SETestSuite::PerformSuite(bool performed)
{
  m_Performed = performed;
}
//-------------------------------------------------------------------------------
bool SETestSuite::PerformedSuite()
{
  return m_Performed;
}
//-------------------------------------------------------------------------------
const SEScalarTime& SETestSuite::GetDuration() const
{
  double Duration = 0;
  for (unsigned int i = 0; i < m_TestCase.size(); i++)
    Duration += m_TestCase.at(i)->GetDuration().GetValue(TimeUnit::s);
  m_Duration.SetValue(Duration, TimeUnit::s);
  return m_Duration;
}
//-------------------------------------------------------------------------------
std::vector<std::string>& SETestSuite::GetRequirements()
{
  return m_Requirements;
}
//-------------------------------------------------------------------------------
SETestCase& SETestSuite::CreateTestCase()
{
  SETestCase* test = new SETestCase(GetLogger());
  m_TestCase.push_back(test);
  return *test;
}
//-------------------------------------------------------------------------------
const std::vector<SETestCase*>& SETestSuite::GetTestCases() const
{
  return m_TestCase;
}
//-------------------------------------------------------------------------------
int SETestSuite::GetNumberOfErrors() const
{
  return std::accumulate(m_TestCase.begin(), m_TestCase.end(),
                         0, [](int a, biogears::SETestCase* b) { return a + static_cast<int>(b->GetFailures().size()); });
}
//-------------------------------------------------------------------------------
int SETestSuite::GetNumberOfTests() const
{
  return static_cast<int>(m_TestCase.size());
}
//-------------------------------------------------------------------------------
std::vector<SETestErrorStatistics*>& SETestSuite::GetSuiteEqualError()
{
  return m_SuiteEqualError;
}
//-------------------------------------------------------------------------------
std::vector<SETestCase*>& SETestSuite::GetTestCase()
{
  return m_TestCase;
}
//-------------------------------------------------------------------------------
bool SETestSuite::operator==(SETestSuite const& rhs) const
{
  if (this == &rhs)
    return true;

  bool equivilant = m_Performed == rhs.m_Performed
    && m_Name == rhs.m_Name
    && m_Requirements == rhs.m_Requirements
    && m_SuiteEqualError.size() == rhs.m_SuiteEqualError.size()
    && m_TestCase.size() == rhs.m_TestCase.size();

  for (auto i = 0; equivilant && i < m_SuiteEqualError.size(); ++i) {
    equivilant &= (m_SuiteEqualError[i] && rhs.m_SuiteEqualError[i])
      ? m_SuiteEqualError[i]->operator==(*rhs.m_SuiteEqualError[i])
      : m_SuiteEqualError[i] == rhs.m_SuiteEqualError[i];
  }

  for (auto i = 0; equivilant && i < m_TestCase.size(); ++i) {
    equivilant &= (m_TestCase[i] && rhs.m_TestCase[i])
      ? m_TestCase[i]->operator==(*rhs.m_TestCase[i])
      : m_TestCase[i] == rhs.m_TestCase[i];
  }

  return equivilant;
}
bool SETestSuite::operator!=(SETestSuite const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}