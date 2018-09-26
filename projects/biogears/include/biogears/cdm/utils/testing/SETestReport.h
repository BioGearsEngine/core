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
#include <biogears/cdm/utils/testing/SETestSuite.h>

CDM_BIND_DECL(TestReportData)
namespace biogears{
class BIOGEARS_API SETestReport : public Loggable {
public:
  SETestReport(Logger* logger);
  virtual ~SETestReport();

  virtual void Reset(); //reset values
  virtual void Clear(); //clear memory

  bool Load(const CDM::TestReportData& in);
  std::unique_ptr<CDM::TestReportData> Unload() const;

protected:
  void Unload(CDM::TestReportData& data) const;

public:
  bool WriteFile(const std::string& filename);

  SETestSuite& CreateTestSuite();
  const std::vector<SETestSuite*>& GetTestSuites() const;

protected:
  std::vector<SETestSuite*> m_testSuite;
};
}