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

#include <biogears/cdm/properties/SEScalarMassPerVolume.h>


#ifdef DISABLE_BIOGEARS_SEScalarMassPerVolume_TEST
#define TEST_FIXTURE_NAME DISABLED_SEScalarMassPerVolume_Fixture
#else
#define TEST_FIXTURE_NAME SEScalarMassPerVolume_Fixture
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
  biogears::SEScalarMassPerVolume MassPerVolume = biogears::SEScalarMassPerVolume();
  auto ptr = MassPerVolume.Unload();
  EXPECT_EQ(ptr, nullptr);
}

TEST_F(TEST_FIXTURE_NAME, IsValidUnit)
{
  bool unit0  = biogears::MassPerVolumeUnit::IsValidUnit("g/dL");
  bool unit1  = biogears::MassPerVolumeUnit::IsValidUnit("g/cm^3");
  bool unit2  = biogears::MassPerVolumeUnit::IsValidUnit("g/m^3");
  bool unit3  = biogears::MassPerVolumeUnit::IsValidUnit("ug/mL");
  bool unit4  = biogears::MassPerVolumeUnit::IsValidUnit("mg/m^3");
  bool unit5  = biogears::MassPerVolumeUnit::IsValidUnit("kg/m^3");
  bool unit6  = biogears::MassPerVolumeUnit::IsValidUnit("ug/L");
  bool unit7  = biogears::MassPerVolumeUnit::IsValidUnit("g/L");
  bool unit8  = biogears::MassPerVolumeUnit::IsValidUnit("g/mL");
  bool unit9  = biogears::MassPerVolumeUnit::IsValidUnit("mg/mL");
  bool unit10 = biogears::MassPerVolumeUnit::IsValidUnit("mg/L");
  bool unit11 = biogears::MassPerVolumeUnit::IsValidUnit("mg/dL");
  bool unit12 = biogears::MassPerVolumeUnit::IsValidUnit("kg/mL");
  bool unit13 = biogears::MassPerVolumeUnit::IsValidUnit("kg/L");
  EXPECT_EQ(unit0, true);
  EXPECT_EQ(unit1, true);
  EXPECT_EQ(unit2, true);
  EXPECT_EQ(unit3, true);
  EXPECT_EQ(unit4, true);
  EXPECT_EQ(unit5, true);
  EXPECT_EQ(unit6, true);
  EXPECT_EQ(unit7, true);
  EXPECT_EQ(unit8, true);
  EXPECT_EQ(unit9, true);
  EXPECT_EQ(unit10, true);
  EXPECT_EQ(unit11, true);
  EXPECT_EQ(unit12, true);
  EXPECT_EQ(unit13, true);
  bool unit14 = biogears::MassPerVolumeUnit::IsValidUnit("DEADBEEF");
  EXPECT_EQ(unit14, false);
}

TEST_F(TEST_FIXTURE_NAME, GetCompoundUnit)
{
  biogears::MassPerVolumeUnit mu0  = biogears::MassPerVolumeUnit::GetCompoundUnit("g/dL");
  biogears::MassPerVolumeUnit mu1  = biogears::MassPerVolumeUnit::GetCompoundUnit("g/cm^3");
  biogears::MassPerVolumeUnit mu2  = biogears::MassPerVolumeUnit::GetCompoundUnit("g/m^3");
  biogears::MassPerVolumeUnit mu3  = biogears::MassPerVolumeUnit::GetCompoundUnit("ug/mL");
  biogears::MassPerVolumeUnit mu4  = biogears::MassPerVolumeUnit::GetCompoundUnit("mg/m^3");
  biogears::MassPerVolumeUnit mu5  = biogears::MassPerVolumeUnit::GetCompoundUnit("kg/m^3");
  biogears::MassPerVolumeUnit mu6  = biogears::MassPerVolumeUnit::GetCompoundUnit("ug/L");
  biogears::MassPerVolumeUnit mu7  = biogears::MassPerVolumeUnit::GetCompoundUnit("g/L");
  biogears::MassPerVolumeUnit mu8  = biogears::MassPerVolumeUnit::GetCompoundUnit("g/mL");
  biogears::MassPerVolumeUnit mu9  = biogears::MassPerVolumeUnit::GetCompoundUnit("mg/mL");
  biogears::MassPerVolumeUnit mu10 = biogears::MassPerVolumeUnit::GetCompoundUnit("mg/L");
  biogears::MassPerVolumeUnit mu11 = biogears::MassPerVolumeUnit::GetCompoundUnit("mg/dL");
  biogears::MassPerVolumeUnit mu12 = biogears::MassPerVolumeUnit::GetCompoundUnit("kg/mL");
  biogears::MassPerVolumeUnit mu13 = biogears::MassPerVolumeUnit::GetCompoundUnit("kg/L");
  EXPECT_EQ(mu0, biogears::MassPerVolumeUnit::g_Per_dL);
  EXPECT_EQ(mu1, biogears::MassPerVolumeUnit::g_Per_cm3);
  EXPECT_EQ(mu2, biogears::MassPerVolumeUnit::g_Per_m3);
  EXPECT_EQ(mu3, biogears::MassPerVolumeUnit::ug_Per_mL);
  EXPECT_EQ(mu4, biogears::MassPerVolumeUnit::mg_Per_m3);
  EXPECT_EQ(mu5, biogears::MassPerVolumeUnit::kg_Per_m3);
  EXPECT_EQ(mu6, biogears::MassPerVolumeUnit::ug_Per_L);
  EXPECT_EQ(mu7, biogears::MassPerVolumeUnit::g_Per_L);
  EXPECT_EQ(mu8, biogears::MassPerVolumeUnit::g_Per_mL);
  EXPECT_EQ(mu9, biogears::MassPerVolumeUnit::mg_Per_mL);
  EXPECT_EQ(mu10, biogears::MassPerVolumeUnit::mg_Per_L);
  EXPECT_EQ(mu11, biogears::MassPerVolumeUnit::mg_Per_dL);
  EXPECT_EQ(mu12, biogears::MassPerVolumeUnit::kg_Per_mL);
  EXPECT_EQ(mu13, biogears::MassPerVolumeUnit::kg_Per_L);
  EXPECT_THROW(biogears::MassPerVolumeUnit::GetCompoundUnit("DEADBEEF"),biogears::CommonDataModelException);
}