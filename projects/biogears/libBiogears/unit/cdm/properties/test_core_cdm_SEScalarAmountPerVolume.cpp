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

#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>


#ifdef DISABLE_BIOGEARS_SEScalarAmountPerVolume_TEST
#define TEST_FIXTURE_NAME DISABLED_SEScalarAmountPerVolume_Fixture
#else
#define TEST_FIXTURE_NAME SEScalarAmountPerVolume_Fixture
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

TEST_F(TEST_FIXTURE_NAME, IsValidUnit)
{
  bool unit0 = biogears::AmountPerVolumeUnit::IsValidUnit("mol/L");
  bool unit1 = biogears::AmountPerVolumeUnit::IsValidUnit("mol/mL");
  bool unit2 = biogears::AmountPerVolumeUnit::IsValidUnit("mmol/L");
  bool unit3 = biogears::AmountPerVolumeUnit::IsValidUnit("mmol/mL");
  bool unit4 = biogears::AmountPerVolumeUnit::IsValidUnit("ct/L");
  bool unit5 = biogears::AmountPerVolumeUnit::IsValidUnit("ct/uL");
  EXPECT_EQ(unit0, true);
  EXPECT_EQ(unit1, true);
  EXPECT_EQ(unit2, true);
  EXPECT_EQ(unit3, true);
  EXPECT_EQ(unit4, true);
  EXPECT_EQ(unit5, true);
  bool unit6 = biogears::AmountPerVolumeUnit::IsValidUnit("DEADBEEF");
  EXPECT_EQ(unit6, false);
}

TEST_F(TEST_FIXTURE_NAME, GetCompoundUnit)
{
  biogears::AmountPerVolumeUnit mu0 = biogears::AmountPerVolumeUnit::GetCompoundUnit("mol/L");
  biogears::AmountPerVolumeUnit mu1 = biogears::AmountPerVolumeUnit::GetCompoundUnit("mol/mL");
  biogears::AmountPerVolumeUnit mu2 = biogears::AmountPerVolumeUnit::GetCompoundUnit("mmol/L");
  biogears::AmountPerVolumeUnit mu3 = biogears::AmountPerVolumeUnit::GetCompoundUnit("mmol/mL");
  biogears::AmountPerVolumeUnit mu4 = biogears::AmountPerVolumeUnit::GetCompoundUnit("ct/L");
  biogears::AmountPerVolumeUnit mu5 = biogears::AmountPerVolumeUnit::GetCompoundUnit("ct/uL");
  EXPECT_EQ(mu0, biogears::AmountPerVolumeUnit::mol_Per_L);
  EXPECT_EQ(mu1, biogears::AmountPerVolumeUnit::mol_Per_mL);
  EXPECT_EQ(mu2, biogears::AmountPerVolumeUnit::mmol_Per_L);
  EXPECT_EQ(mu3, biogears::AmountPerVolumeUnit::mmol_Per_mL);
  EXPECT_EQ(mu4, biogears::AmountPerVolumeUnit::ct_Per_L);
  EXPECT_EQ(mu5, biogears::AmountPerVolumeUnit::ct_Per_uL);
  EXPECT_THROW(biogears::AmountPerVolumeUnit::GetCompoundUnit("DEADBEEF"),biogears::CommonDataModelException);
}