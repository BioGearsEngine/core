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

namespace biogears {
SETestErrorStatistics::SETestErrorStatistics(Logger* logger)
  : Loggable(logger)
{
  m_PercentToleranceVsNumErrorsHistogram = nullptr;
}

SETestErrorStatistics::~SETestErrorStatistics()
{
  Clear();
}

void SETestErrorStatistics::Clear()
{
  SAFE_DELETE(m_PercentToleranceVsNumErrorsHistogram);
}

void SETestErrorStatistics::Reset()
{
}

bool SETestErrorStatistics::Load(const CDM::TestErrorStatisticsData& in)
{
  Reset();
  m_MinimumError = in.MinimumError().get();
  m_MaximumError = in.MaximumError().get();
  m_AverageError = in.AverageError().get();
  m_StandardDeviation = in.StandardDeviation().get();

  std::string dData;
  for (unsigned int i = 0; i < in.Differences().size(); i++) {
    dData = (std::string)in.Differences().at(i);
    if (!dData.empty())
      m_Differences.push_back(dData);
  }
  m_ComputedPropertyID = in.ComputedPropertyID();
  m_ExpectedPropertyID = in.ExpectedPropertyID();
  m_NumberOfErrors = in.NumberOfErrors().get();
  m_PercentTolerance = in.PercentTolerance();
  m_PropertyName = in.PropertyName();
  return IsValid();
}

std::unique_ptr<CDM::TestErrorStatisticsData> SETestErrorStatistics::Unload() const
{
  std::unique_ptr<CDM::TestErrorStatisticsData> data(new CDM::TestErrorStatisticsData());
  Unload(*data);
  return data;
}

void SETestErrorStatistics::Unload(CDM::TestErrorStatisticsData& data) const
{
  if (!std::isnan(m_MinimumError))
    data.MinimumError(m_MinimumError);
  if (!std::isnan(m_MaximumError))
    data.MaximumError(m_MaximumError);
  if (!std::isnan(m_AverageError))
    data.AverageError(m_AverageError);
  if (!std::isnan(m_StandardDeviation))
    data.StandardDeviation(m_StandardDeviation);

  std::string dData;
  for (unsigned int i = 0; i < m_Differences.size(); i++) {
    dData = m_Differences.at(i);
    if (!dData.empty())
      data.Differences().push_back(dData);
  }
  if (!m_ComputedPropertyID.empty())
    data.ComputedPropertyID(m_ComputedPropertyID);
  if (!m_ExpectedPropertyID.empty())
    data.ExpectedPropertyID(m_ExpectedPropertyID);
  data.NumberOfErrors(m_NumberOfErrors);
  data.PercentTolerance(m_PercentTolerance);
  if (!m_PropertyName.empty())
    data.PropertyName(m_PropertyName);
  if (m_PercentToleranceVsNumErrorsHistogram != nullptr) {
    data.PercentToleranceVsNumErrors(std::unique_ptr<CDM::FunctionData>(m_PercentToleranceVsNumErrorsHistogram->Unload()));
  }
}

bool SETestErrorStatistics::IsValid()
{
  return true;
}

void SETestErrorStatistics::SetMinimumError(double MinimumError)
{
  m_MinimumError = MinimumError;
}

double SETestErrorStatistics::GetMinimumError() const
{
  return m_MinimumError;
}

void SETestErrorStatistics::SetMaximumError(double MaximumError)
{
  m_MaximumError = MaximumError;
}

double SETestErrorStatistics::GetMaximumError() const
{
  return m_MaximumError;
}

void SETestErrorStatistics::SetAverageError(double AverageError)
{
  m_AverageError = AverageError;
}

double SETestErrorStatistics::GetAverageError() const
{
  return m_AverageError;
}

void SETestErrorStatistics::SetStandardDeviation(double StandardDeviation)
{
  m_StandardDeviation = StandardDeviation;
}

double SETestErrorStatistics::GetStandardDeviation() const
{
  return m_StandardDeviation;
}

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

const std::vector<std::string>* SETestErrorStatistics::GetDifferences() const
{
  return &m_Differences;
}

void SETestErrorStatistics::SetComputedPropertyID(const std::string& ComputedPropertyID)
{
  m_ComputedPropertyID = ComputedPropertyID;
}

std::string SETestErrorStatistics::GetComputedPropertyID() const
{
  return m_ComputedPropertyID;
}

void SETestErrorStatistics::SetExpectedPropertyID(const std::string& ExpectedPropertyID)
{
  m_ExpectedPropertyID = ExpectedPropertyID;
}

std::string SETestErrorStatistics::GetExpectedPropertyID() const
{
  return m_ExpectedPropertyID;
}

void SETestErrorStatistics::SetNumberOfErrors(int NumberOfErrors)
{
  m_NumberOfErrors = NumberOfErrors;
}

int SETestErrorStatistics::GetNumberOfErrors() const
{
  return m_NumberOfErrors;
}

void SETestErrorStatistics::SetPropertyName(const std::string& PropertyName)
{
  m_PropertyName = PropertyName;
}

std::string SETestErrorStatistics::GetPropertyName() const
{
  return m_PropertyName;
}

bool SETestErrorStatistics::HasPercentTolerancevsNumErrorsHistogram() const
{
  if (m_PercentToleranceVsNumErrorsHistogram == nullptr)
    return false;
  return true;
}

SEFunction& SETestErrorStatistics::GetPercentToleranceVsNumErrorsHistogram()
{
  if (HasPercentTolerancevsNumErrorsHistogram())
    return *m_PercentToleranceVsNumErrorsHistogram;

  m_PercentToleranceVsNumErrorsHistogram = new SEFunction();
  m_PercentToleranceVsNumErrorsHistogram->GetIndependent().push_back(m_PercentTolerance);
  m_PercentToleranceVsNumErrorsHistogram->GetDependent().push_back(m_NumberOfErrors);
  return *m_PercentToleranceVsNumErrorsHistogram;
}

void SETestErrorStatistics::SetPercentToleranceVsNumErrorsHistogram(SEFunction* PercentToleranceVsNumErrors)
{
  m_PercentToleranceVsNumErrorsHistogram = PercentToleranceVsNumErrors;
}
}