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
//! @author David Lee
//! @date   2017 Aug 3rd
//!
//! Unit Test for Biogears-common Config
//!
#include <thread>

#include <gtest/gtest.h>

#include <biogears/cdm/properties/SEScalarFlowCompliance.h>


#ifdef DISABLE_BIOGEARS_SEScalarFlowCompliance_TEST
#define TEST_FIXTURE_NAME DISABLED_SEScalarFlowCompliance_Fixture
#else
#define TEST_FIXTURE_NAME SEScalarFlowCompliance_Fixture
#endif

// The fixture for testing class Foo.
class TEST_FIXTURE_NAME : public ::testing::Test {
protected:
  // You can do set-up work for each test here.
  TEST_FIXTURE_NAME() = default;

  // You can do clean-up work that doesn't throw exceptions here.
  virtual ~TEST_FIXTURE_NAME() = default;

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  // Code here will be called immediately after the constructor (right
  // before each test).
  virtual void SetUp() override;

  // Code here will be called immediately after each test (right
  // before the destructor).
  virtual void TearDown() override;
};

void TEST_FIXTURE_NAME::SetUp()
{
}

void TEST_FIXTURE_NAME::TearDown()
{
}

TEST_F(TEST_FIXTURE_NAME, Unload)
{
  BIOGEARS_NAMESPACE SEScalarFlowCompliance FlowCompliance = BIOGEARS_NAMESPACE SEScalarFlowCompliance();
  auto ptr = FlowCompliance.Unload();
  EXPECT_EQ(ptr, nullptr);
}

TEST_F(TEST_FIXTURE_NAME, IsValidUnit)
{
  bool unit0 = BIOGEARS_NAMESPACE FlowComplianceUnit::IsValidUnit("L/cmH2O");
  bool unit1 = BIOGEARS_NAMESPACE FlowComplianceUnit::IsValidUnit("mL/mmHg");
  bool unit2 = BIOGEARS_NAMESPACE FlowComplianceUnit::IsValidUnit("mL/cmH2O");
  bool unit3 = BIOGEARS_NAMESPACE FlowComplianceUnit::IsValidUnit("m^3/Pa");
  EXPECT_EQ(unit0, true);
  EXPECT_EQ(unit1, true);
  EXPECT_EQ(unit2, true);
  EXPECT_EQ(unit3, true);
  bool unit6 = BIOGEARS_NAMESPACE FlowComplianceUnit::IsValidUnit("DEADBEEF");
  EXPECT_EQ(unit6, false);
}

TEST_F(TEST_FIXTURE_NAME, GetCompoundUnit)
{
  BIOGEARS_NAMESPACE FlowComplianceUnit mu0 = BIOGEARS_NAMESPACE FlowComplianceUnit::GetCompoundUnit("L/cmH2O");
  BIOGEARS_NAMESPACE FlowComplianceUnit mu1 = BIOGEARS_NAMESPACE FlowComplianceUnit::GetCompoundUnit("mL/mmHg");
  BIOGEARS_NAMESPACE FlowComplianceUnit mu2 = BIOGEARS_NAMESPACE FlowComplianceUnit::GetCompoundUnit("mL/cmH2O");
  BIOGEARS_NAMESPACE FlowComplianceUnit mu3 = BIOGEARS_NAMESPACE FlowComplianceUnit::GetCompoundUnit("m^3/Pa");
  EXPECT_EQ(mu0, BIOGEARS_NAMESPACE FlowComplianceUnit::L_Per_cmH2O);
  EXPECT_EQ(mu1, BIOGEARS_NAMESPACE FlowComplianceUnit::mL_Per_mmHg);
  EXPECT_EQ(mu2, BIOGEARS_NAMESPACE FlowComplianceUnit::mL_Per_cmH2O);
  EXPECT_EQ(mu3, BIOGEARS_NAMESPACE FlowComplianceUnit::m3_Per_Pa);
  EXPECT_THROW(BIOGEARS_NAMESPACE FlowComplianceUnit::GetCompoundUnit("DEADBEEF"),BIOGEARS_NAMESPACE CommonDataModelException);
}