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

#include <biogears/cdm/properties/SEScalarVolume.h>


#ifdef DISABLE_BIOGEARS_SEScalarVolume_TEST
#define TEST_FIXTURE_NAME DISABLED_SEScalarVolume_Fixture
#else
#define TEST_FIXTURE_NAME SEScalarVolume_Fixture
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
  BIOGEARS_NAMESPACE SEScalarVolume Volume = BIOGEARS_NAMESPACE SEScalarVolume();
  auto ptr = Volume.Unload();
  EXPECT_EQ(ptr, nullptr);
}

TEST_F(TEST_FIXTURE_NAME, IsValidUnit)
{
  bool unit0 = BIOGEARS_NAMESPACE VolumeUnit::IsValidUnit("L");
  bool unit1 = BIOGEARS_NAMESPACE VolumeUnit::IsValidUnit("dL");
  bool unit2 = BIOGEARS_NAMESPACE VolumeUnit::IsValidUnit("mL");
  bool unit3 = BIOGEARS_NAMESPACE VolumeUnit::IsValidUnit("uL");
  bool unit4 = BIOGEARS_NAMESPACE VolumeUnit::IsValidUnit("m^3");
  EXPECT_EQ(unit0, true);
  EXPECT_EQ(unit1, true);
  EXPECT_EQ(unit2, true);
  EXPECT_EQ(unit3, true);
  EXPECT_EQ(unit4, true);
  bool unit5 = BIOGEARS_NAMESPACE VolumeUnit::IsValidUnit("DEADBEEF");
  EXPECT_EQ(unit5, false);
}

TEST_F(TEST_FIXTURE_NAME, GetCompoundUnit)
{
  BIOGEARS_NAMESPACE VolumeUnit mu0 = BIOGEARS_NAMESPACE VolumeUnit::GetCompoundUnit("L");
  BIOGEARS_NAMESPACE VolumeUnit mu1 = BIOGEARS_NAMESPACE VolumeUnit::GetCompoundUnit("dL");
  BIOGEARS_NAMESPACE VolumeUnit mu2 = BIOGEARS_NAMESPACE VolumeUnit::GetCompoundUnit("mL");
  BIOGEARS_NAMESPACE VolumeUnit mu3 = BIOGEARS_NAMESPACE VolumeUnit::GetCompoundUnit("uL");
  BIOGEARS_NAMESPACE VolumeUnit mu4 = BIOGEARS_NAMESPACE VolumeUnit::GetCompoundUnit("m^3");
  EXPECT_EQ(mu0, BIOGEARS_NAMESPACE VolumeUnit::L);
  EXPECT_EQ(mu1, BIOGEARS_NAMESPACE VolumeUnit::dL);
  EXPECT_EQ(mu2, BIOGEARS_NAMESPACE VolumeUnit::mL);
  EXPECT_EQ(mu3, BIOGEARS_NAMESPACE VolumeUnit::uL);
  EXPECT_EQ(mu4, BIOGEARS_NAMESPACE VolumeUnit::m3);
  EXPECT_THROW(BIOGEARS_NAMESPACE VolumeUnit::GetCompoundUnit("DEADBEEF"),BIOGEARS_NAMESPACE CommonDataModelException);
}