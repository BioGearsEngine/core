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
#include <biogears/cdm/utils/testing/SETestErrorStatistics.h>

#include <biogears/cdm/properties/SEFunction.h>
#include <biogears/schema/cdm/TestReport.hxx>

// Private Include
#include <io/cdm/TestReport.h>

namespace biogears {
SETestErrorStatistics::SETestErrorStatistics(Logger* logger)
  : Loggable(logger)
  , m_PercentTolerance(0.0001)
{
  m_PercentToleranceVsNumErrors = nullptr;
}
//-------------------------------------------------------------------------------
SETestErrorStatistics::~SETestErrorStatistics()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SETestErrorStatistics::Clear()
{
  SAFE_DELETE(m_PercentToleranceVsNumErrors);
}
//-------------------------------------------------------------------------------
void SETestErrorStatistics::Reset()
{
}
//-------------------------------------------------------------------------------
bool SETestErrorStatistics::Load(const CDM::TestErrorStatisticsData& in)
{
  io::TestReport::UnMarshall(in, *this);
  return IsValid();
}
//-------------------------------------------------------------------------------
std::unique_ptr<CDM::TestErrorStatisticsData> SETestErrorStatistics::Unload() const
{
  std::unique_ptr<CDM::TestErrorStatisticsData> data(new CDM::TestErrorStatisticsData());
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SETestErrorStatistics::Unload(CDM::TestErrorStatisticsData& data) const
{
  io::TestReport::Marshall(*this, data);
}
//-------------------------------------------------------------------------------
bool SETestErrorStatistics::IsValid()
{
  return true;
}
//-------------------------------------------------------------------------------
void SETestErrorStatistics::SetMinimumError(double MinimumError)
{
  m_MinimumError = MinimumError;
}
//-------------------------------------------------------------------------------
double SETestErrorStatistics::GetMinimumError() const
{
  return m_MinimumError;
}
//-------------------------------------------------------------------------------
void SETestErrorStatistics::SetMaximumError(double MaximumError)
{
  m_MaximumError = MaximumError;
}
//-------------------------------------------------------------------------------
double SETestErrorStatistics::GetMaximumError() const
{
  return m_MaximumError;
}
//-------------------------------------------------------------------------------
void SETestErrorStatistics::SetAverageError(double AverageError)
{
  m_AverageError = AverageError;
}
//-------------------------------------------------------------------------------
double SETestErrorStatistics::GetAverageError() const
{
  return m_AverageError;
}
//-------------------------------------------------------------------------------
void SETestErrorStatistics::SetStandardDeviation(double StandardDeviation)
{
  m_StandardDeviation = StandardDeviation;
}
//-------------------------------------------------------------------------------
double SETestErrorStatistics::GetStandardDeviation() const
{
  return m_StandardDeviation;
}
//-------------------------------------------------------------------------------
void SETestErrorStatistics::AddDifference(const std::string& difference)
{
  std::string n;
  for (unsigned int i = 0; i < m_Differences.size(); i++) {
    n = m_Differences.at(i);
    if (n == difference)
      return;
  }
  m_Differences.push_back(difference);
}
//-------------------------------------------------------------------------------
void SETestErrorStatistics::SetPercentTolerance(double PercentTolerance)
{
  m_PercentTolerance = PercentTolerance;
}
//-------------------------------------------------------------------------------
double SETestErrorStatistics::GetPercentTolerance() const
{
  return m_PercentTolerance;
}
//-------------------------------------------------------------------------------
const std::vector<std::string>* SETestErrorStatistics::GetDifferences() const
{
  return &m_Differences;
}
//-------------------------------------------------------------------------------
void SETestErrorStatistics::SetComputedPropertyID(const std::string& ComputedPropertyID)
{
  m_ComputedPropertyID = ComputedPropertyID;
}
//-------------------------------------------------------------------------------
std::string SETestErrorStatistics::GetComputedPropertyID() const
{
  return m_ComputedPropertyID;
}
//-------------------------------------------------------------------------------
void SETestErrorStatistics::SetExpectedPropertyID(const std::string& ExpectedPropertyID)
{
  m_ExpectedPropertyID = ExpectedPropertyID;
}
//-------------------------------------------------------------------------------
std::string SETestErrorStatistics::GetExpectedPropertyID() const
{
  return m_ExpectedPropertyID;
}
//-------------------------------------------------------------------------------
void SETestErrorStatistics::SetNumberOfErrors(int NumberOfErrors)
{
  m_NumberOfErrors = NumberOfErrors;
}
//-------------------------------------------------------------------------------
int SETestErrorStatistics::GetNumberOfErrors() const
{
  return m_NumberOfErrors;
}
//-------------------------------------------------------------------------------
void SETestErrorStatistics::SetPropertyName(const std::string& PropertyName)
{
  m_PropertyName = PropertyName;
}
//-------------------------------------------------------------------------------
std::string SETestErrorStatistics::GetPropertyName() const
{
  return m_PropertyName;
}
//-------------------------------------------------------------------------------
bool SETestErrorStatistics::HasPercentToleranceVsNumErrors() const
{
  if (m_PercentToleranceVsNumErrors == nullptr)
    return false;
  return true;
}
//-------------------------------------------------------------------------------
SEFunction& SETestErrorStatistics::GetPercentToleranceVsNumErrors()
{
  if (HasPercentToleranceVsNumErrors())
    return *m_PercentToleranceVsNumErrors;

  m_PercentToleranceVsNumErrors = new SEFunction();
  m_PercentToleranceVsNumErrors->GetIndependent().push_back(m_PercentTolerance);
  m_PercentToleranceVsNumErrors->GetDependent().push_back(m_NumberOfErrors);
  return *m_PercentToleranceVsNumErrors;
}
//-------------------------------------------------------------------------------
void SETestErrorStatistics::SetPercentToleranceVsNumErrors(SEFunction* PercentToleranceVsNumErrors)
{
  m_PercentToleranceVsNumErrors = PercentToleranceVsNumErrors;
}
//-------------------------------------------------------------------------------
bool SETestErrorStatistics::operator==(const SETestErrorStatistics& rhs) const
{
  return m_NumberOfErrors == rhs.m_NumberOfErrors
    && m_MinimumError == rhs.m_MinimumError
    && m_MaximumError == rhs.m_MaximumError
    && m_AverageError == rhs.m_AverageError
    && m_StandardDeviation == rhs.m_StandardDeviation
    && m_PercentTolerance == rhs.m_PercentTolerance
    && m_PropertyName == rhs.m_PropertyName
    && m_ComputedPropertyID == rhs.m_ComputedPropertyID
    && m_ExpectedPropertyID == rhs.m_ExpectedPropertyID
    && ((m_PercentToleranceVsNumErrors && rhs.m_PercentToleranceVsNumErrors)
          ? m_PercentToleranceVsNumErrors->operator==(*rhs.m_PercentToleranceVsNumErrors)
          : m_PercentToleranceVsNumErrors == rhs.m_PercentToleranceVsNumErrors)
    && m_Differences == rhs.m_Differences;
}
bool SETestErrorStatistics::operator!=(const SETestErrorStatistics& rhs) const
{
  return !(this->operator==(rhs));
}
//-------------------------------------------------------------------------------
}