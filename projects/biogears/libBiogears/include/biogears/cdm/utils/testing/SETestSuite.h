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
#include <biogears/cdm/utils/testing/SETestCase.h>

CDM_BIND_DECL(TestSuiteData)

namespace biogears {
class SETestReport;
namespace io {
  class TestReport;
}
class BIOGEARS_API SETestSuite : public Loggable {
  friend SETestReport;
  friend io::TestReport;

public:
  SETestSuite(Logger* logger);
  virtual ~SETestSuite();

  virtual void Reset(); //reset values
  virtual void Clear(); //clear memory

  bool Load(const CDM::TestSuiteData& in);
  std::unique_ptr<CDM::TestSuiteData> Unload() const;

  bool operator==(SETestSuite const&) const;
  bool operator!=(SETestSuite const&) const;

protected:
  void Unload(CDM::TestSuiteData& data) const;

public:
  void SetName(const std::string& Name);
  std::string GetName() const;
  const char* GetName_cStr() const;

  void PerformSuite(bool Performed);
  bool PerformedSuite();

  const SEScalarTime& GetDuration() const;

  std::vector<std::string>& GetRequirements();

  SETestCase& CreateTestCase();
  const std::vector<SETestCase*>& GetTestCases() const;

  int GetNumberOfErrors() const;
  int GetNumberOfTests() const;

  std::vector<SETestErrorStatistics*>& GetSuiteEqualError();
  std::vector<SETestCase*>& GetTestCase();

protected:
  bool m_Performed;
  std::string m_Name;
  std::vector<std::string> m_Requirements;
  std::vector<SETestErrorStatistics*> m_SuiteEqualError;
  std::vector<SETestCase*> m_TestCase;
  mutable SEScalarTime m_Duration;
};
}