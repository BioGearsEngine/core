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

#include <biogears/cdm/properties/SEScalarPressure.h>


#ifdef DISABLE_BIOGEARS_SEScalarPressure_TEST
#define TEST_FIXTURE_NAME DISABLED_SEScalarPressure_Fixture
#else
#define TEST_FIXTURE_NAME SEScalarPressure_Fixture
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
  biogears::SEScalarPressure Pressure = biogears::SEScalarPressure();
  auto ptr = Pressure.Unload();
  EXPECT_EQ(ptr, nullptr);
}

TEST_F(TEST_FIXTURE_NAME, IsValidUnit)
{
  bool unit0 = biogears::PressureUnit::IsValidUnit("Pa");
  bool unit1 = biogears::PressureUnit::IsValidUnit("mmHg");
  bool unit2 = biogears::PressureUnit::IsValidUnit("cmH2O");
  bool unit3 = biogears::PressureUnit::IsValidUnit("psi");
  bool unit4 = biogears::PressureUnit::IsValidUnit("atm");
  EXPECT_EQ(unit0, true);
  EXPECT_EQ(unit1, true);
  EXPECT_EQ(unit2, true);
  EXPECT_EQ(unit3, true);
  EXPECT_EQ(unit4, true);
  bool unit5 = biogears::PressureUnit::IsValidUnit("DEADBEEF");
  EXPECT_EQ(unit5, false);
}

TEST_F(TEST_FIXTURE_NAME, GetCompoundUnit)
{
  biogears::PressureUnit mu0 = biogears::PressureUnit::GetCompoundUnit("Pa");
  biogears::PressureUnit mu1 = biogears::PressureUnit::GetCompoundUnit("mmHg");
  biogears::PressureUnit mu2 = biogears::PressureUnit::GetCompoundUnit("cmH2O");
  biogears::PressureUnit mu3 = biogears::PressureUnit::GetCompoundUnit("psi");
  biogears::PressureUnit mu4 = biogears::PressureUnit::GetCompoundUnit("atm");
  EXPECT_EQ(mu0, biogears::PressureUnit::Pa);
  EXPECT_EQ(mu1, biogears::PressureUnit::mmHg);
  EXPECT_EQ(mu2, biogears::PressureUnit::cmH2O);
  EXPECT_EQ(mu3, biogears::PressureUnit::psi);
  EXPECT_EQ(mu4, biogears::PressureUnit::atm);
  EXPECT_THROW(biogears::PressureUnit::GetCompoundUnit("DEADBEEF"),biogears::CommonDataModelException);
}