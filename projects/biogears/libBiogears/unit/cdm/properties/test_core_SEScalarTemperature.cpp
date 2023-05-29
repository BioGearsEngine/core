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

#include <biogears/cdm/properties/SEScalarTemperature.h>


#ifdef DISABLE_BIOGEARS_SEScalarTemperature_TEST
#define TEST_FIXTURE_NAME DISABLED_SEScalarTemperature_Fixture
#else
#define TEST_FIXTURE_NAME SEScalarTemperature_Fixture
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
  BIOGEARS_NAMESPACE SEScalarTemperature Temperature = BIOGEARS_NAMESPACE SEScalarTemperature();
  auto ptr = Temperature.Unload();
  EXPECT_EQ(ptr, nullptr);
}

TEST_F(TEST_FIXTURE_NAME, IsValidUnit)
{
  bool unit0 = BIOGEARS_NAMESPACE TemperatureUnit::IsValidUnit("degF");
  bool unit1 = BIOGEARS_NAMESPACE TemperatureUnit::IsValidUnit("degC");
  bool unit2 = BIOGEARS_NAMESPACE TemperatureUnit::IsValidUnit("K");
  bool unit3 = BIOGEARS_NAMESPACE TemperatureUnit::IsValidUnit("degR");
  EXPECT_EQ(unit0, true);
  EXPECT_EQ(unit1, true);
  EXPECT_EQ(unit2, true);
  EXPECT_EQ(unit3, true);
  bool unit5 = BIOGEARS_NAMESPACE TemperatureUnit::IsValidUnit("DEADBEEF");
  EXPECT_EQ(unit5, false);
}

TEST_F(TEST_FIXTURE_NAME, GetCompoundUnit)
{
  BIOGEARS_NAMESPACE TemperatureUnit mu0 = BIOGEARS_NAMESPACE TemperatureUnit::GetCompoundUnit("degF");
  BIOGEARS_NAMESPACE TemperatureUnit mu1 = BIOGEARS_NAMESPACE TemperatureUnit::GetCompoundUnit("degC");
  BIOGEARS_NAMESPACE TemperatureUnit mu2 = BIOGEARS_NAMESPACE TemperatureUnit::GetCompoundUnit("K");
  BIOGEARS_NAMESPACE TemperatureUnit mu3 = BIOGEARS_NAMESPACE TemperatureUnit::GetCompoundUnit("degR");
  EXPECT_EQ(mu0, BIOGEARS_NAMESPACE TemperatureUnit::F);
  EXPECT_EQ(mu1, BIOGEARS_NAMESPACE TemperatureUnit::C);
  EXPECT_EQ(mu2, BIOGEARS_NAMESPACE TemperatureUnit::K);
  EXPECT_EQ(mu3, BIOGEARS_NAMESPACE TemperatureUnit::R);
  EXPECT_THROW(BIOGEARS_NAMESPACE TemperatureUnit::GetCompoundUnit("DEADBEEF"),BIOGEARS_NAMESPACE CommonDataModelException);
}