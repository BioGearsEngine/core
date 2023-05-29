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

#include <biogears/cdm/properties/SEScalarVolumePerTime.h>


#ifdef DISABLE_BIOGEARS_SEScalarVolumePerTime_TEST
#define TEST_FIXTURE_NAME DISABLED_SEScalarVolumePerTime_Fixture
#else
#define TEST_FIXTURE_NAME SEScalarVolumePerTime_Fixture
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
  BIOGEARS_NAMESPACE SEScalarVolumePerTime VolumePerTime = BIOGEARS_NAMESPACE SEScalarVolumePerTime();
  auto ptr = VolumePerTime.Unload();
  EXPECT_EQ(ptr, nullptr);
}

TEST_F(TEST_FIXTURE_NAME, IsValidUnit)
{
  bool unit0 = BIOGEARS_NAMESPACE VolumePerTimeUnit::IsValidUnit("L/s");
  bool unit1 = BIOGEARS_NAMESPACE VolumePerTimeUnit::IsValidUnit("mL/s");
  bool unit2 = BIOGEARS_NAMESPACE VolumePerTimeUnit::IsValidUnit("mL/day");
  bool unit3 = BIOGEARS_NAMESPACE VolumePerTimeUnit::IsValidUnit("L/day");
  bool unit4 = BIOGEARS_NAMESPACE VolumePerTimeUnit::IsValidUnit("L/min");
  bool unit5 = BIOGEARS_NAMESPACE VolumePerTimeUnit::IsValidUnit("m^3/s");
  bool unit6 = BIOGEARS_NAMESPACE VolumePerTimeUnit::IsValidUnit("mL/min");
  bool unit7 = BIOGEARS_NAMESPACE VolumePerTimeUnit::IsValidUnit("mL/hr");
  EXPECT_EQ(unit0, true);
  EXPECT_EQ(unit1, true);
  EXPECT_EQ(unit2, true);
  EXPECT_EQ(unit3, true);
  EXPECT_EQ(unit4, true);
  EXPECT_EQ(unit5, true);
  EXPECT_EQ(unit6, true);
  EXPECT_EQ(unit7, true);
  bool unit8 = BIOGEARS_NAMESPACE VolumePerTimeUnit::IsValidUnit("DEADBEEF");
  EXPECT_EQ(unit8, false);
}

TEST_F(TEST_FIXTURE_NAME, GetCompoundUnit)
{
  BIOGEARS_NAMESPACE VolumePerTimeUnit mu0 = BIOGEARS_NAMESPACE VolumePerTimeUnit::GetCompoundUnit("L/s");
  BIOGEARS_NAMESPACE VolumePerTimeUnit mu1 = BIOGEARS_NAMESPACE VolumePerTimeUnit::GetCompoundUnit("mL/s");
  BIOGEARS_NAMESPACE VolumePerTimeUnit mu2 = BIOGEARS_NAMESPACE VolumePerTimeUnit::GetCompoundUnit("mL/day");
  BIOGEARS_NAMESPACE VolumePerTimeUnit mu3 = BIOGEARS_NAMESPACE VolumePerTimeUnit::GetCompoundUnit("L/day");
  BIOGEARS_NAMESPACE VolumePerTimeUnit mu4 = BIOGEARS_NAMESPACE VolumePerTimeUnit::GetCompoundUnit("L/min");
  BIOGEARS_NAMESPACE VolumePerTimeUnit mu5 = BIOGEARS_NAMESPACE VolumePerTimeUnit::GetCompoundUnit("m^3/s");
  BIOGEARS_NAMESPACE VolumePerTimeUnit mu6 = BIOGEARS_NAMESPACE VolumePerTimeUnit::GetCompoundUnit("mL/min");
  BIOGEARS_NAMESPACE VolumePerTimeUnit mu7 = BIOGEARS_NAMESPACE VolumePerTimeUnit::GetCompoundUnit("mL/hr");
  EXPECT_EQ(mu0, BIOGEARS_NAMESPACE VolumePerTimeUnit::L_Per_s);
  EXPECT_EQ(mu1, BIOGEARS_NAMESPACE VolumePerTimeUnit::mL_Per_s);
  EXPECT_EQ(mu2, BIOGEARS_NAMESPACE VolumePerTimeUnit::mL_Per_day);
  EXPECT_EQ(mu3, BIOGEARS_NAMESPACE VolumePerTimeUnit::L_Per_day);
  EXPECT_EQ(mu4, BIOGEARS_NAMESPACE VolumePerTimeUnit::L_Per_min);
  EXPECT_EQ(mu5, BIOGEARS_NAMESPACE VolumePerTimeUnit::m3_Per_s);
  EXPECT_EQ(mu6, BIOGEARS_NAMESPACE VolumePerTimeUnit::mL_Per_min);
  EXPECT_EQ(mu7, BIOGEARS_NAMESPACE VolumePerTimeUnit::mL_Per_hr);
  EXPECT_THROW(BIOGEARS_NAMESPACE VolumePerTimeUnit::GetCompoundUnit("DEADBEEF"),BIOGEARS_NAMESPACE CommonDataModelException);
}