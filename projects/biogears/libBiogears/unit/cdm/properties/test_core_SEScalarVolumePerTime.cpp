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
  biogears::SEScalarVolumePerTime VolumePerTime = biogears::SEScalarVolumePerTime();
  auto ptr = VolumePerTime.Unload();
  EXPECT_EQ(ptr, nullptr);
}

TEST_F(TEST_FIXTURE_NAME, IsValidUnit)
{
  bool unit0 = biogears::VolumePerTimeUnit::IsValidUnit("L/s");
  bool unit1 = biogears::VolumePerTimeUnit::IsValidUnit("mL/s");
  bool unit2 = biogears::VolumePerTimeUnit::IsValidUnit("mL/day");
  bool unit3 = biogears::VolumePerTimeUnit::IsValidUnit("L/day");
  bool unit4 = biogears::VolumePerTimeUnit::IsValidUnit("L/min");
  bool unit5 = biogears::VolumePerTimeUnit::IsValidUnit("m^3/s");
  bool unit6 = biogears::VolumePerTimeUnit::IsValidUnit("mL/min");
  bool unit7 = biogears::VolumePerTimeUnit::IsValidUnit("mL/hr");
  EXPECT_EQ(unit0, true);
  EXPECT_EQ(unit1, true);
  EXPECT_EQ(unit2, true);
  EXPECT_EQ(unit3, true);
  EXPECT_EQ(unit4, true);
  EXPECT_EQ(unit5, true);
  EXPECT_EQ(unit6, true);
  EXPECT_EQ(unit7, true);
  bool unit8 = biogears::VolumePerTimeUnit::IsValidUnit("DEADBEEF");
  EXPECT_EQ(unit8, false);
}

TEST_F(TEST_FIXTURE_NAME, GetCompoundUnit)
{
  biogears::VolumePerTimeUnit mu0 = biogears::VolumePerTimeUnit::GetCompoundUnit("L/s");
  biogears::VolumePerTimeUnit mu1 = biogears::VolumePerTimeUnit::GetCompoundUnit("mL/s");
  biogears::VolumePerTimeUnit mu2 = biogears::VolumePerTimeUnit::GetCompoundUnit("mL/day");
  biogears::VolumePerTimeUnit mu3 = biogears::VolumePerTimeUnit::GetCompoundUnit("L/day");
  biogears::VolumePerTimeUnit mu4 = biogears::VolumePerTimeUnit::GetCompoundUnit("L/min");
  biogears::VolumePerTimeUnit mu5 = biogears::VolumePerTimeUnit::GetCompoundUnit("m^3/s");
  biogears::VolumePerTimeUnit mu6 = biogears::VolumePerTimeUnit::GetCompoundUnit("mL/min");
  biogears::VolumePerTimeUnit mu7 = biogears::VolumePerTimeUnit::GetCompoundUnit("mL/hr");
  EXPECT_EQ(mu0, biogears::VolumePerTimeUnit::L_Per_s);
  EXPECT_EQ(mu1, biogears::VolumePerTimeUnit::mL_Per_s);
  EXPECT_EQ(mu2, biogears::VolumePerTimeUnit::mL_Per_day);
  EXPECT_EQ(mu3, biogears::VolumePerTimeUnit::L_Per_day);
  EXPECT_EQ(mu4, biogears::VolumePerTimeUnit::L_Per_min);
  EXPECT_EQ(mu5, biogears::VolumePerTimeUnit::m3_Per_s);
  EXPECT_EQ(mu6, biogears::VolumePerTimeUnit::mL_Per_min);
  EXPECT_EQ(mu7, biogears::VolumePerTimeUnit::mL_Per_hr);
  EXPECT_THROW(biogears::VolumePerTimeUnit::GetCompoundUnit("DEADBEEF"),biogears::CommonDataModelException);
}