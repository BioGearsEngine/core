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
  BIOGEARS_NAMESPACE SEScalarMassPerTime MassPerTime = BIOGEARS_NAMESPACE SEScalarMassPerTime();
  auto ptr = MassPerTime.Unload();
  EXPECT_EQ(ptr, nullptr);
}

TEST_F(TEST_FIXTURE_NAME, IsValidUnit)
{
  bool unit0  = BIOGEARS_NAMESPACE MassPerTimeUnit::IsValidUnit("g/s");
  bool unit1  = BIOGEARS_NAMESPACE MassPerTimeUnit::IsValidUnit("g/min");
  bool unit2  = BIOGEARS_NAMESPACE MassPerTimeUnit::IsValidUnit("g/day");
  bool unit3  = BIOGEARS_NAMESPACE MassPerTimeUnit::IsValidUnit("mg/s");
  bool unit4  = BIOGEARS_NAMESPACE MassPerTimeUnit::IsValidUnit("mg/min");
  bool unit5  = BIOGEARS_NAMESPACE MassPerTimeUnit::IsValidUnit("ug/s");
  bool unit6  = BIOGEARS_NAMESPACE MassPerTimeUnit::IsValidUnit("kg/s");
  bool unit7  = BIOGEARS_NAMESPACE MassPerTimeUnit::IsValidUnit("ug/min");
  EXPECT_EQ(unit0, true);
  EXPECT_EQ(unit1, true);
  EXPECT_EQ(unit2, true);
  EXPECT_EQ(unit3, true);
  EXPECT_EQ(unit4, true);
  EXPECT_EQ(unit5, true);
  EXPECT_EQ(unit6, true);
  EXPECT_EQ(unit7, true);
  bool unit11 = BIOGEARS_NAMESPACE MassPerTimeUnit::IsValidUnit("DEADBEEF");
  EXPECT_EQ(unit11, false);
}

TEST_F(TEST_FIXTURE_NAME, GetCompoundUnit)
{
  BIOGEARS_NAMESPACE MassPerTimeUnit mu0  = BIOGEARS_NAMESPACE MassPerTimeUnit::GetCompoundUnit("g/s");
  BIOGEARS_NAMESPACE MassPerTimeUnit mu1  = BIOGEARS_NAMESPACE MassPerTimeUnit::GetCompoundUnit("g/min");
  BIOGEARS_NAMESPACE MassPerTimeUnit mu2  = BIOGEARS_NAMESPACE MassPerTimeUnit::GetCompoundUnit("g/day");
  BIOGEARS_NAMESPACE MassPerTimeUnit mu3  = BIOGEARS_NAMESPACE MassPerTimeUnit::GetCompoundUnit("mg/s");
  BIOGEARS_NAMESPACE MassPerTimeUnit mu4  = BIOGEARS_NAMESPACE MassPerTimeUnit::GetCompoundUnit("mg/min");
  BIOGEARS_NAMESPACE MassPerTimeUnit mu5  = BIOGEARS_NAMESPACE MassPerTimeUnit::GetCompoundUnit("ug/s");
  BIOGEARS_NAMESPACE MassPerTimeUnit mu6  = BIOGEARS_NAMESPACE MassPerTimeUnit::GetCompoundUnit("kg/s");
  BIOGEARS_NAMESPACE MassPerTimeUnit mu7  = BIOGEARS_NAMESPACE MassPerTimeUnit::GetCompoundUnit("ug/min");
  EXPECT_EQ(mu0, BIOGEARS_NAMESPACE MassPerTimeUnit::g_Per_s);
  EXPECT_EQ(mu1, BIOGEARS_NAMESPACE MassPerTimeUnit::g_Per_min);
  EXPECT_EQ(mu2, BIOGEARS_NAMESPACE MassPerTimeUnit::g_Per_day);
  EXPECT_EQ(mu3, BIOGEARS_NAMESPACE MassPerTimeUnit::mg_Per_s);
  EXPECT_EQ(mu4, BIOGEARS_NAMESPACE MassPerTimeUnit::mg_Per_min);
  EXPECT_EQ(mu5, BIOGEARS_NAMESPACE MassPerTimeUnit::ug_Per_s);
  EXPECT_EQ(mu6, BIOGEARS_NAMESPACE MassPerTimeUnit::kg_Per_s);
  EXPECT_EQ(mu7, BIOGEARS_NAMESPACE MassPerTimeUnit::ug_Per_min);
  EXPECT_THROW(BIOGEARS_NAMESPACE MassPerTimeUnit::GetCompoundUnit("DEADBEEF"),BIOGEARS_NAMESPACE CommonDataModelException);
}