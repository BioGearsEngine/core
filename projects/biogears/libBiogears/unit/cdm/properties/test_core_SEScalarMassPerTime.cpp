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

#include <biogears/cdm/properties/SEScalarMassPerTime.h>


#ifdef DISABLE_BIOGEARS_SEScalarMassPerTime_TEST
#define TEST_FIXTURE_NAME DISABLED_SEScalarMassPerTime_Fixture
#else
#define TEST_FIXTURE_NAME SEScalarMassPerTime_Fixture
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
  biogears::SEScalarMassPerTime MassPerTime = biogears::SEScalarMassPerTime();
  auto ptr = MassPerTime.Unload();
  EXPECT_EQ(ptr, nullptr);
}

TEST_F(TEST_FIXTURE_NAME, IsValidUnit)
{
  bool unit0  = biogears::MassPerTimeUnit::IsValidUnit("g/s");
  bool unit1  = biogears::MassPerTimeUnit::IsValidUnit("g/min");
  bool unit2  = biogears::MassPerTimeUnit::IsValidUnit("g/day");
  bool unit3  = biogears::MassPerTimeUnit::IsValidUnit("mg/s");
  bool unit4  = biogears::MassPerTimeUnit::IsValidUnit("mg/min");
  bool unit5  = biogears::MassPerTimeUnit::IsValidUnit("ug/s");
  bool unit6  = biogears::MassPerTimeUnit::IsValidUnit("kg/s");
  bool unit7  = biogears::MassPerTimeUnit::IsValidUnit("ug/min");
  EXPECT_EQ(unit0, true);
  EXPECT_EQ(unit1, true);
  EXPECT_EQ(unit2, true);
  EXPECT_EQ(unit3, true);
  EXPECT_EQ(unit4, true);
  EXPECT_EQ(unit5, true);
  EXPECT_EQ(unit6, true);
  EXPECT_EQ(unit7, true);
  bool unit11 = biogears::MassPerTimeUnit::IsValidUnit("DEADBEEF");
  EXPECT_EQ(unit11, false);
}

TEST_F(TEST_FIXTURE_NAME, GetCompoundUnit)
{
  biogears::MassPerTimeUnit mu0  = biogears::MassPerTimeUnit::GetCompoundUnit("g/s");
  biogears::MassPerTimeUnit mu1  = biogears::MassPerTimeUnit::GetCompoundUnit("g/min");
  biogears::MassPerTimeUnit mu2  = biogears::MassPerTimeUnit::GetCompoundUnit("g/day");
  biogears::MassPerTimeUnit mu3  = biogears::MassPerTimeUnit::GetCompoundUnit("mg/s");
  biogears::MassPerTimeUnit mu4  = biogears::MassPerTimeUnit::GetCompoundUnit("mg/min");
  biogears::MassPerTimeUnit mu5  = biogears::MassPerTimeUnit::GetCompoundUnit("ug/s");
  biogears::MassPerTimeUnit mu6  = biogears::MassPerTimeUnit::GetCompoundUnit("kg/s");
  biogears::MassPerTimeUnit mu7  = biogears::MassPerTimeUnit::GetCompoundUnit("ug/min");
  EXPECT_EQ(mu0, biogears::MassPerTimeUnit::g_Per_s);
  EXPECT_EQ(mu1, biogears::MassPerTimeUnit::g_Per_min);
  EXPECT_EQ(mu2, biogears::MassPerTimeUnit::g_Per_day);
  EXPECT_EQ(mu3, biogears::MassPerTimeUnit::mg_Per_s);
  EXPECT_EQ(mu4, biogears::MassPerTimeUnit::mg_Per_min);
  EXPECT_EQ(mu5, biogears::MassPerTimeUnit::ug_Per_s);
  EXPECT_EQ(mu6, biogears::MassPerTimeUnit::kg_Per_s);
  EXPECT_EQ(mu7, biogears::MassPerTimeUnit::ug_Per_min);
  EXPECT_THROW(biogears::MassPerTimeUnit::GetCompoundUnit("DEADBEEF"),biogears::CommonDataModelException);
}