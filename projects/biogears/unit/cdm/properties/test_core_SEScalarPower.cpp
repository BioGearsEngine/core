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

#include <biogears/cdm/properties/SEScalarPower.h>


#ifdef DISABLE_BIOGEARS_SEScalarPower_TEST
#define TEST_FIXTURE_NAME DISABLED_SEScalarPower_Fixture
#else
#define TEST_FIXTURE_NAME SEScalarPower_Fixture
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
  biogears::SEScalarPower Power = biogears::SEScalarPower();
  auto ptr = Power.Unload();
  EXPECT_EQ(ptr, nullptr);
}

TEST_F(TEST_FIXTURE_NAME, IsValidUnit)
{
  bool unit0 = biogears::PowerUnit::IsValidUnit("W");
  bool unit1 = biogears::PowerUnit::IsValidUnit("kcal/s");
  bool unit2 = biogears::PowerUnit::IsValidUnit("kcal/hr");
  bool unit3 = biogears::PowerUnit::IsValidUnit("kcal/day");
  bool unit4 = biogears::PowerUnit::IsValidUnit("J/s");
  bool unit5 = biogears::PowerUnit::IsValidUnit("BTU/hr");
  EXPECT_EQ(unit0, true);
  EXPECT_EQ(unit1, true);
  EXPECT_EQ(unit2, true);
  EXPECT_EQ(unit3, true);
  EXPECT_EQ(unit4, true);
  EXPECT_EQ(unit5, true);
  bool unit6 = biogears::PowerUnit::IsValidUnit("DEADBEEF");
  EXPECT_EQ(unit6, false);
}

TEST_F(TEST_FIXTURE_NAME, GetCompoundUnit)
{
  biogears::PowerUnit mu0 = biogears::PowerUnit::GetCompoundUnit("W");
  biogears::PowerUnit mu1 = biogears::PowerUnit::GetCompoundUnit("kcal/s");
  biogears::PowerUnit mu2 = biogears::PowerUnit::GetCompoundUnit("kcal/hr");
  biogears::PowerUnit mu3 = biogears::PowerUnit::GetCompoundUnit("kcal/day");
  biogears::PowerUnit mu4 = biogears::PowerUnit::GetCompoundUnit("J/s");
  biogears::PowerUnit mu5 = biogears::PowerUnit::GetCompoundUnit("BTU/hr");
  EXPECT_EQ(mu0, biogears::PowerUnit::W);
  EXPECT_EQ(mu1, biogears::PowerUnit::kcal_Per_s);
  EXPECT_EQ(mu2, biogears::PowerUnit::kcal_Per_hr);
  EXPECT_EQ(mu3, biogears::PowerUnit::kcal_Per_day);
  EXPECT_EQ(mu4, biogears::PowerUnit::J_Per_s);
  EXPECT_EQ(mu5, biogears::PowerUnit::BTU_Per_hr);
  EXPECT_THROW(biogears::PowerUnit::GetCompoundUnit("DEADBEEF"),biogears::CommonDataModelException);
}