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
#include <memory>

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/exports.h>

CDM_BIND_DECL(TestErrorStatisticsData)
namespace biogears {
class SETestCase;
class SETestSuite;
class SEFunction;
namespace io {
  class TestReport;
}
// This data class is intended for use in array based data
// Each value in the array is compared and can be tracked
// and this provides a summary of how many values in the array
// were wrong and some statistics to go along with all of those errors
class BIOGEARS_API SETestErrorStatistics : public Loggable {
  friend SETestCase;
  friend SETestSuite;
  friend io::TestReport;

protected:
  SETestErrorStatistics(Logger* logger);

public:
  virtual ~SETestErrorStatistics();

  virtual void Reset(); //reset values
  virtual void Clear(); //clear memory

  bool Load(const CDM::TestErrorStatisticsData& in);
  std::unique_ptr<CDM::TestErrorStatisticsData> Unload() const;

protected:
  void Unload(CDM::TestErrorStatisticsData& data) const;

public:
  bool IsValid();

  void SetPropertyName(const std::string& PropertyName);
  std::string GetPropertyName() const;

  void SetComputedPropertyID(const std::string& ComputedPropertyID);
  std::string GetComputedPropertyID() const;

  void SetExpectedPropertyID(const std::string& ExpectedPropertyID);
  std::string GetExpectedPropertyID() const;

  void SetNumberOfErrors(int NumberOfErrors);
  int GetNumberOfErrors() const;

  void SetMinimumError(double MinimumError);
  double GetMinimumError() const;

  void SetMaximumError(double MaximumError);
  double GetMaximumError() const;

  void SetAverageError(double AverageError);
  double GetAverageError() const;

  void SetStandardDeviation(double StandardDeviation);
  double GetStandardDeviation() const;

  void AddDifference(const std::string& difference);
  const std::vector<std::string>* GetDifferences() const;

  // This a histogram that shows a percent tolerance and how many errors fall with in each bin
  bool HasPercentTolerancevsNumErrorsHistogram() const;
  SEFunction& GetPercentToleranceVsNumErrorsHistogram();
  void SetPercentToleranceVsNumErrorsHistogram(SEFunction* PercentToleranceVsNumErrors);

protected:
  int m_NumberOfErrors;
  double m_MinimumError;
  double m_MaximumError;
  double m_AverageError;
  double m_StandardDeviation;
  double m_PercentTolerance;
  std::string m_PropertyName;
  std::string m_ComputedPropertyID;
  std::string m_ExpectedPropertyID;
  SEFunction* m_PercentToleranceVsNumErrorsHistogram;
  std::vector<std::string> m_Differences;
};
}