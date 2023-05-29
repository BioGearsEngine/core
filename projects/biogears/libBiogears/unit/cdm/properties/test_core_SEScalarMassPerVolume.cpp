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
  BIOGEARS_NAMESPACE SEScalarMassPerVolume MassPerVolume = BIOGEARS_NAMESPACE SEScalarMassPerVolume();
  auto ptr = MassPerVolume.Unload();
  EXPECT_EQ(ptr, nullptr);
}

TEST_F(TEST_FIXTURE_NAME, IsValidUnit)
{
  bool unit0  = BIOGEARS_NAMESPACE MassPerVolumeUnit::IsValidUnit("g/dL");
  bool unit1  = BIOGEARS_NAMESPACE MassPerVolumeUnit::IsValidUnit("g/cm^3");
  bool unit2  = BIOGEARS_NAMESPACE MassPerVolumeUnit::IsValidUnit("g/m^3");
  bool unit3  = BIOGEARS_NAMESPACE MassPerVolumeUnit::IsValidUnit("ug/mL");
  bool unit4  = BIOGEARS_NAMESPACE MassPerVolumeUnit::IsValidUnit("mg/m^3");
  bool unit5  = BIOGEARS_NAMESPACE MassPerVolumeUnit::IsValidUnit("kg/m^3");
  bool unit6  = BIOGEARS_NAMESPACE MassPerVolumeUnit::IsValidUnit("ug/L");
  bool unit7  = BIOGEARS_NAMESPACE MassPerVolumeUnit::IsValidUnit("g/L");
  bool unit8  = BIOGEARS_NAMESPACE MassPerVolumeUnit::IsValidUnit("g/mL");
  bool unit9  = BIOGEARS_NAMESPACE MassPerVolumeUnit::IsValidUnit("mg/mL");
  bool unit10 = BIOGEARS_NAMESPACE MassPerVolumeUnit::IsValidUnit("mg/L");
  bool unit11 = BIOGEARS_NAMESPACE MassPerVolumeUnit::IsValidUnit("mg/dL");
  bool unit12 = BIOGEARS_NAMESPACE MassPerVolumeUnit::IsValidUnit("kg/mL");
  bool unit13 = BIOGEARS_NAMESPACE MassPerVolumeUnit::IsValidUnit("kg/L");
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
  bool unit14 = BIOGEARS_NAMESPACE MassPerVolumeUnit::IsValidUnit("DEADBEEF");
  EXPECT_EQ(unit14, false);
}

TEST_F(TEST_FIXTURE_NAME, GetCompoundUnit)
{
  BIOGEARS_NAMESPACE MassPerVolumeUnit mu0  = BIOGEARS_NAMESPACE MassPerVolumeUnit::GetCompoundUnit("g/dL");
  BIOGEARS_NAMESPACE MassPerVolumeUnit mu1  = BIOGEARS_NAMESPACE MassPerVolumeUnit::GetCompoundUnit("g/cm^3");
  BIOGEARS_NAMESPACE MassPerVolumeUnit mu2  = BIOGEARS_NAMESPACE MassPerVolumeUnit::GetCompoundUnit("g/m^3");
  BIOGEARS_NAMESPACE MassPerVolumeUnit mu3  = BIOGEARS_NAMESPACE MassPerVolumeUnit::GetCompoundUnit("ug/mL");
  BIOGEARS_NAMESPACE MassPerVolumeUnit mu4  = BIOGEARS_NAMESPACE MassPerVolumeUnit::GetCompoundUnit("mg/m^3");
  BIOGEARS_NAMESPACE MassPerVolumeUnit mu5  = BIOGEARS_NAMESPACE MassPerVolumeUnit::GetCompoundUnit("kg/m^3");
  BIOGEARS_NAMESPACE MassPerVolumeUnit mu6  = BIOGEARS_NAMESPACE MassPerVolumeUnit::GetCompoundUnit("ug/L");
  BIOGEARS_NAMESPACE MassPerVolumeUnit mu7  = BIOGEARS_NAMESPACE MassPerVolumeUnit::GetCompoundUnit("g/L");
  BIOGEARS_NAMESPACE MassPerVolumeUnit mu8  = BIOGEARS_NAMESPACE MassPerVolumeUnit::GetCompoundUnit("g/mL");
  BIOGEARS_NAMESPACE MassPerVolumeUnit mu9  = BIOGEARS_NAMESPACE MassPerVolumeUnit::GetCompoundUnit("mg/mL");
  BIOGEARS_NAMESPACE MassPerVolumeUnit mu10 = BIOGEARS_NAMESPACE MassPerVolumeUnit::GetCompoundUnit("mg/L");
  BIOGEARS_NAMESPACE MassPerVolumeUnit mu11 = BIOGEARS_NAMESPACE MassPerVolumeUnit::GetCompoundUnit("mg/dL");
  BIOGEARS_NAMESPACE MassPerVolumeUnit mu12 = BIOGEARS_NAMESPACE MassPerVolumeUnit::GetCompoundUnit("kg/mL");
  BIOGEARS_NAMESPACE MassPerVolumeUnit mu13 = BIOGEARS_NAMESPACE MassPerVolumeUnit::GetCompoundUnit("kg/L");
  EXPECT_EQ(mu0, BIOGEARS_NAMESPACE MassPerVolumeUnit::g_Per_dL);
  EXPECT_EQ(mu1, BIOGEARS_NAMESPACE MassPerVolumeUnit::g_Per_cm3);
  EXPECT_EQ(mu2, BIOGEARS_NAMESPACE MassPerVolumeUnit::g_Per_m3);
  EXPECT_EQ(mu3, BIOGEARS_NAMESPACE MassPerVolumeUnit::ug_Per_mL);
  EXPECT_EQ(mu4, BIOGEARS_NAMESPACE MassPerVolumeUnit::mg_Per_m3);
  EXPECT_EQ(mu5, BIOGEARS_NAMESPACE MassPerVolumeUnit::kg_Per_m3);
  EXPECT_EQ(mu6, BIOGEARS_NAMESPACE MassPerVolumeUnit::ug_Per_L);
  EXPECT_EQ(mu7, BIOGEARS_NAMESPACE MassPerVolumeUnit::g_Per_L);
  EXPECT_EQ(mu8, BIOGEARS_NAMESPACE MassPerVolumeUnit::g_Per_mL);
  EXPECT_EQ(mu9, BIOGEARS_NAMESPACE MassPerVolumeUnit::mg_Per_mL);
  EXPECT_EQ(mu10, BIOGEARS_NAMESPACE MassPerVolumeUnit::mg_Per_L);
  EXPECT_EQ(mu11, BIOGEARS_NAMESPACE MassPerVolumeUnit::mg_Per_dL);
  EXPECT_EQ(mu12, BIOGEARS_NAMESPACE MassPerVolumeUnit::kg_Per_mL);
  EXPECT_EQ(mu13, BIOGEARS_NAMESPACE MassPerVolumeUnit::kg_Per_L);
  EXPECT_THROW(BIOGEARS_NAMESPACE MassPerVolumeUnit::GetCompoundUnit("DEADBEEF"),BIOGEARS_NAMESPACE CommonDataModelException);
}