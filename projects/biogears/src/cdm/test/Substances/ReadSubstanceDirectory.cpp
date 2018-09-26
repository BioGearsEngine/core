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

#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/test/CommonDataModelTest.h>
#include <biogears/cdm/utils/FileUtils.h>
#include <biogears/cdm/utils/TimingProfile.h>
#include <biogears/cdm/utils/testing/SETestCase.h>
#include <biogears/cdm/utils/testing/SETestReport.h>
#include <biogears/cdm/utils/testing/SETestSuite.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>

namespace biogears {
void CommonDataModelTest::ReadSubstanceDirectory(const std::string& rptDirectory)
{
  TimingProfile pTimer;
  std::string testName = "ReadSubstanceDirectory";
  m_Logger->ResetLogFile(rptDirectory + "/" + testName + ".log");
  SESubstance obj(m_Logger);

  std::string dir = GetCurrentWorkingDirectory();
  dir.append("/substances");

  SETestReport testReport(m_Logger);
  SETestSuite& testSuite = testReport.CreateTestSuite();
  testSuite.SetName(testName);

  SETestCase& testCase = testSuite.CreateTestCase();
  pTimer.Start("Case");
  SESubstanceManager subMgr(m_Logger);
  if (!subMgr.LoadSubstanceDirectory())
    testCase.AddFailure("Unable to load substances");
  testCase.GetDuration().SetValue(pTimer.GetElapsedTime_s("Case"), TimeUnit::s);
  testCase.SetName(obj.GetName());
  // TODO would be nice to listen to errors on the logger and add them to the testCase failures...

  testReport.WriteFile(rptDirectory + "/" + testName + "Report.xml");
}
}