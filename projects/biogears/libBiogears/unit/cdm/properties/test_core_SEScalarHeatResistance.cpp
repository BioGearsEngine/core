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

#include <biogears/cdm/properties/SEScalarHeatResistance.h>


#ifdef DISABLE_BIOGEARS_SEScalarHeatResistance_TEST
#define TEST_FIXTURE_NAME DISABLED_SEScalarHeatResistance_Fixture
#else
#define TEST_FIXTURE_NAME SEScalarHeatResistance_Fixture
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
  biogears::SEScalarHeatResistance HeatResistance = biogears::SEScalarHeatResistance();
  auto ptr = HeatResistance.Unload();
  EXPECT_EQ(ptr, nullptr);
}

TEST_F(TEST_FIXTURE_NAME, IsValidUnit)
{
  bool unit0 = biogears::HeatResistanceUnit::IsValidUnit("K/W");
  bool unit1 = biogears::HeatResistanceUnit::IsValidUnit("degC/W");
  bool unit2 = biogears::HeatResistanceUnit::IsValidUnit("K s/kcal");
  bool unit3 = biogears::HeatResistanceUnit::IsValidUnit("degC s/kcal");
  EXPECT_EQ(unit0, true);
  EXPECT_EQ(unit1, true);
  EXPECT_EQ(unit2, true);
  EXPECT_EQ(unit3, true);
  bool unit6 = biogears::HeatResistanceUnit::IsValidUnit("DEADBEEF");
  EXPECT_EQ(unit6, false);
}

TEST_F(TEST_FIXTURE_NAME, GetCompoundUnit)
{
  biogears::HeatResistanceUnit mu0 = biogears::HeatResistanceUnit::GetCompoundUnit("K/W");
  biogears::HeatResistanceUnit mu1 = biogears::HeatResistanceUnit::GetCompoundUnit("degC/W");
  biogears::HeatResistanceUnit mu2 = biogears::HeatResistanceUnit::GetCompoundUnit("K s/kcal");
  biogears::HeatResistanceUnit mu3 = biogears::HeatResistanceUnit::GetCompoundUnit("degC s/kcal");
  EXPECT_EQ(mu0, biogears::HeatResistanceUnit::K_Per_W);
  EXPECT_EQ(mu1, biogears::HeatResistanceUnit::C_Per_W);
  EXPECT_EQ(mu2, biogears::HeatResistanceUnit::K_s_Per_kcal);
  EXPECT_EQ(mu3, biogears::HeatResistanceUnit::C_s_Per_kcal);
  EXPECT_THROW(biogears::HeatResistanceUnit::GetCompoundUnit("DEADBEEF"),biogears::CommonDataModelException);
}