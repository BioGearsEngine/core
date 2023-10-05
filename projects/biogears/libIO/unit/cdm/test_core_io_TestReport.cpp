//-------------------------------------------------------------------------------------------
//- Copyright 2017 Applied Research Associates, Inc.
//- Licensed under the Apache License, Version 2.0 (the "License"); you may not use
//- this file except in compliance with the License. You may obtain a copy of the License
//- at:
//- http://www.apache.org/licenses/LICENSE-2.0
//- Unless required by applicable law or agreed to in writing, software distributed under
//- the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//- CONDITIONS OF ANY KIND, either express or implied. See the License for the
//-  specific language governing permissions and limitations under the License.
//-------------------------------------------------------------------------------------------

//!
//! @author
//! @date   2023/08/14
//!
//! Unit Test for BioGears Seralization
//!
#include <thread>

#include <gtest/gtest.h>

#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/properties/SEFunction.h>
#include <biogears/cdm/properties/SEProperties.h>
#include <biogears/cdm/scenario/requests/SEDataRequestManager.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/utils/Logger.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>

#include <io/cdm/Property.h>
#include <io/cdm/TestReport.h>

#ifdef DISABLE_BIOGEARS_Substance_TEST
#define TEST_FIXTURE_NAME DISABLED_TestReportFixture
#else
#define TEST_FIXTURE_NAME IO_TestReportFixture
#endif

// The fixture for testing class Foo.
class TEST_FIXTURE_NAME : public ::testing::Test {
protected:
  // You can do set-up work for each test here.
  TEST_FIXTURE_NAME()
    : logger()
    , mgr { &logger }
  {
  }

  // You can do clean-up work that doesn't throw exceptions here.
  virtual ~TEST_FIXTURE_NAME() = default;

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  // Code here will be called immediately after the constructor (right
  // before each test).
  virtual void SetUp();

  // Code here will be called immediately after each test (right
  // before the destructor).
  virtual void TearDown();

  biogears::SEDecimalFormat FixtureDecimalFormat;
  biogears::Logger logger;
  biogears::SESubstanceManager mgr;
};

void TEST_FIXTURE_NAME::SetUp()
{
  FixtureDecimalFormat.SetNotation(biogears::DecimalNotation::Fixed);
  FixtureDecimalFormat.SetPrecision(2);

  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
}

void TEST_FIXTURE_NAME::TearDown()
{
}

#define USING_TYPES(name)             \
  using namespace biogears::io;       \
  using SEType = biogears ::SE##name; \
  using CDMType = CDM ::name##Data;

namespace BGE = mil::tatrc::physiology::biogears;

// class  SEGasTestReport
//    TYPE GasTestReport
//   static void Marshall(const CDM::GasTestReportData& in, SEGasTestReport& out);
//   static void UnMarshall(const SEGasTestReport& in, CDM::GasTestReportData& out);
#include <biogears/cdm/utils/testing/SETestErrorStatistics.h>
TEST_F(TEST_FIXTURE_NAME, TestErrorStatistics)
{

  USING_TYPES(TestErrorStatistics) // ABstract Class

  SEType source { &logger }, sink { &logger };
  CDMType data;

  source.SetNumberOfErrors(10);
  source.SetMinimumError(20);
  source.SetMaximumError(35);
  source.SetAverageError(22);
  source.SetStandardDeviation(55);
  source.SetPercentTolerance(0.00001);
  
  source.GetPercentToleranceVsNumErrors().GetDependent().push_back(1);
  source.GetPercentToleranceVsNumErrors().GetDependent().push_back(4);
  source.GetPercentToleranceVsNumErrors().GetIndependent().push_back(2);
  source.GetPercentToleranceVsNumErrors().GetIndependent().push_back(3);

  source.SetPropertyName("Property1");
  source.SetComputedPropertyID("SetComputedPropertyID");
  source.SetExpectedPropertyID("SetExpectedPropertyID");

  source.AddDifference("Dirff1");

  EXPECT_NE(source, sink);

  TestReport::UnMarshall(source, data);
  TestReport::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class  SETestCase
//    TYPE TestCase
//   static void Marshall(const CDM::TestCase& in, SETestCase& out);
//   static void UnMarshall(const SETestCase& in, CDM::TestCase& out);
#include <biogears/cdm/utils/testing/SETestCase.h>
TEST_F(TEST_FIXTURE_NAME, TestCase)
{

  USING_TYPES(TestCase) // ABstract Class

  SEType source { "Source", &logger }, sink { "Sink", &logger };
  CDMType data;

  source.SetName("TestCaseTest");
  source.GetDuration().SetValue(10, biogears::TimeUnit::s);

  source.AddFailure("Failure1");
  source.AddFailure("Failure2");

  auto& stat1 = source.CreateErrorStatistic();

  stat1.AddDifference("Differance");
  stat1.SetNumberOfErrors(10);
  stat1.SetMinimumError(20);
  stat1.SetMaximumError(35);
  stat1.SetAverageError(22);
  stat1.SetStandardDeviation(55);
  stat1.GetPercentToleranceVsNumErrors().GetDependent().push_back(1);
  stat1.GetPercentToleranceVsNumErrors().GetDependent().push_back(4);
  stat1.GetPercentToleranceVsNumErrors().GetIndependent().push_back(2);
  stat1.GetPercentToleranceVsNumErrors().GetIndependent().push_back(3);

  stat1.SetPropertyName("Property1");
  stat1.SetComputedPropertyID("SetComputedPropertyID");
  stat1.SetExpectedPropertyID("SetExpectedPropertyID");  
  
  EXPECT_NE(source, sink);

  TestReport::UnMarshall(source, data);
  TestReport::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class  SETestSuite
//    TYPE TestSuite
//   static void Marshall(const CDM::TestSuite& in, SETestSuite& out);
//   static void UnMarshall(const SETestSuite& in, CDM::TestSuite& out);
#include <biogears/cdm/utils/testing/SETestSuite.h>
TEST_F(TEST_FIXTURE_NAME, TestSuite)
{

  USING_TYPES(TestSuite) // ABstract Class

  SEType source { &logger }, sink { &logger };
  CDMType data;

  source.PerformSuite(true);
  source.SetName("TestSuite1");
  source.GetRequirements().push_back("Requirment1");
  source.GetRequirements().push_back("Requirment2");
  
  auto stats = new biogears::SETestErrorStatistics(&logger);

  stats->SetNumberOfErrors(10);
  stats->SetMinimumError(20);
  stats->SetMaximumError(35);
  stats->SetAverageError(22);
  stats->SetStandardDeviation(55);
  stats->GetPercentToleranceVsNumErrors().GetDependent().push_back(1);
  stats->GetPercentToleranceVsNumErrors().GetDependent().push_back(1);
  stats->GetPercentToleranceVsNumErrors().GetIndependent().push_back(2);
  stats->GetPercentToleranceVsNumErrors().GetIndependent().push_back(3);

  stats->SetPropertyName("Property1");
  stats->SetComputedPropertyID("SetComputedPropertyID");
  stats->SetExpectedPropertyID("SetExpectedPropertyID");

  stats->AddDifference("Dirff1");

  source.GetSuiteEqualError().push_back(stats);
  auto& testcase = source.CreateTestCase();

  testcase.SetName("TestCaseTest");
  testcase.GetDuration().SetValue(10, biogears::TimeUnit::s);

  testcase.AddFailure("Failure1");
  testcase.AddFailure("Failure2");

  auto& stat1 = testcase.CreateErrorStatistic();
  auto& stat2 = testcase.CreateErrorStatistic();
  stat2.GetPercentToleranceVsNumErrors();

  stat1.AddDifference("Differance");
  stat1.SetNumberOfErrors(10);
  stat1.SetMinimumError(20);
  stat1.SetMaximumError(35);
  stat1.SetAverageError(22);
  stat1.SetStandardDeviation(55);
  stat1.GetPercentToleranceVsNumErrors().GetDependent().push_back(1);
  stat1.GetPercentToleranceVsNumErrors().GetDependent().push_back(4);
  stat1.GetPercentToleranceVsNumErrors().GetIndependent().push_back(2);
  stat1.GetPercentToleranceVsNumErrors().GetIndependent().push_back(3);

  stat1.SetPropertyName("Property1");
  stat1.SetComputedPropertyID("SetComputedPropertyID");
  stat1.SetExpectedPropertyID("SetExpectedPropertyID");

  EXPECT_NE(source, sink);

  TestReport::UnMarshall(source, data);
  TestReport::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class  SEGasTestReport
//    TYPE GasTestReport
//   static void Marshall(const CDM::GasTestReportData& in, SEGasTestReport& out);
//   static void UnMarshall(const SEGasTestReport& in, CDM::GasTestReportData& out);
#include <biogears/cdm/utils/testing/SETestReport.h>
TEST_F(TEST_FIXTURE_NAME, TestReport)
{

  USING_TYPES(TestReport) // ABstract Class

  SEType source { &logger }, sink { &logger };
  CDMType data;

  auto testsuite2 = source.CreateTestSuite();

  testsuite2.PerformSuite(true);
  testsuite2.SetName("TestSuite1");
  testsuite2.GetRequirements().push_back("Requirment1");
  testsuite2.GetRequirements().push_back("Requirment2");

  auto stats = new biogears::SETestErrorStatistics(&logger);
  stats->SetNumberOfErrors(10);
  stats->SetMinimumError(20);
  stats->SetMaximumError(35);
  stats->SetAverageError(22);
  stats->SetStandardDeviation(55);
  stats->GetPercentToleranceVsNumErrors().GetDependent().push_back(1);
  stats->GetPercentToleranceVsNumErrors().GetIndependent().push_back(2);
  stats->GetPercentToleranceVsNumErrors().GetIndependent().push_back(3);

  stats->SetPropertyName("Property1");
  stats->SetComputedPropertyID("SetComputedPropertyID");
  stats->SetExpectedPropertyID("SetExpectedPropertyID");

  stats->AddDifference("Dirff1");

  testsuite2.GetSuiteEqualError().push_back(stats);
  auto& testcase = testsuite2.CreateTestCase();

  testcase.SetName("TestCaseTest");
  testcase.GetDuration().SetValue(10, biogears::TimeUnit::s);

  testcase.AddFailure("Failure1");
  testcase.AddFailure("Failure2");

  auto& stat1 = testcase.CreateErrorStatistic();

  stat1.AddDifference("Differance");
  stat1.SetNumberOfErrors(10);
  stat1.SetMinimumError(20);
  stat1.SetMaximumError(35);
  stat1.SetAverageError(22);
  stat1.SetStandardDeviation(55);
  stat1.GetPercentToleranceVsNumErrors().GetDependent().push_back(1);
  stat1.GetPercentToleranceVsNumErrors().GetIndependent().push_back(2);
  stat1.GetPercentToleranceVsNumErrors().GetIndependent().push_back(3);

  stat1.SetPropertyName("Property1");
  stat1.SetComputedPropertyID("SetComputedPropertyID");
  stat1.SetExpectedPropertyID("SetExpectedPropertyID");


  EXPECT_NE(source, sink);

  TestReport::UnMarshall(source, data);
  TestReport::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
