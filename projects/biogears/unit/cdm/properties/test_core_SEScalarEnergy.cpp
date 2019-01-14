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

#include <biogears/cdm/properties/SEScalarEnergy.h>


#ifdef DISABLE_BIOGEARS_SEScalarEnergy_TEST
#define TEST_FIXTURE_NAME DISABLED_SEScalarEnergy_Fixture
#else
#define TEST_FIXTURE_NAME SEScalarEnergy_Fixture
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
  biogears::SEScalarEnergy Energy = biogears::SEScalarEnergy();
  auto ptr = Energy.Unload();
  EXPECT_EQ(ptr, nullptr);
}

TEST_F(TEST_FIXTURE_NAME, IsValidUnit)
{
  bool unit0 = biogears::EnergyUnit::IsValidUnit("J");
  bool unit1 = biogears::EnergyUnit::IsValidUnit("mJ");
  bool unit2 = biogears::EnergyUnit::IsValidUnit("kJ");
  bool unit3 = biogears::EnergyUnit::IsValidUnit("kcal");
  EXPECT_EQ(unit0, true);
  EXPECT_EQ(unit1, true);
  EXPECT_EQ(unit2, true);
  EXPECT_EQ(unit3, true);
  bool unit5 = biogears::EnergyUnit::IsValidUnit("DEADBEEF");
  EXPECT_EQ(unit5, false);
}

TEST_F(TEST_FIXTURE_NAME, GetCompoundUnit)
{
  biogears::EnergyUnit mu0 = biogears::EnergyUnit::GetCompoundUnit("J");
  biogears::EnergyUnit mu1 = biogears::EnergyUnit::GetCompoundUnit("mJ");
  biogears::EnergyUnit mu2 = biogears::EnergyUnit::GetCompoundUnit("kJ");
  biogears::EnergyUnit mu3 = biogears::EnergyUnit::GetCompoundUnit("kcal");
  EXPECT_EQ(mu0, biogears::EnergyUnit::J);
  EXPECT_EQ(mu1, biogears::EnergyUnit::mJ);
  EXPECT_EQ(mu2, biogears::EnergyUnit::kJ);
  EXPECT_EQ(mu3, biogears::EnergyUnit::kcal);
  EXPECT_THROW(biogears::EnergyUnit::GetCompoundUnit("DEADBEEF"),biogears::CommonDataModelException);
}