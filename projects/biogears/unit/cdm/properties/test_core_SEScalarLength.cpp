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

#include <biogears/cdm/properties/SEScalarLength.h>


#ifdef DISABLE_BIOGEARS_SEScalarLength_TEST
#define TEST_FIXTURE_NAME DISABLED_SEScalarLength_Fixture
#else
#define TEST_FIXTURE_NAME SEScalarLength_Fixture
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
  biogears::SEScalarLength Length = biogears::SEScalarLength();
  auto ptr = Length.Unload();
  EXPECT_EQ(ptr, nullptr);
}

TEST_F(TEST_FIXTURE_NAME, IsValidUnit)
{
  bool unit0 = biogears::LengthUnit::IsValidUnit("m");
  bool unit1 = biogears::LengthUnit::IsValidUnit("cm");
  bool unit2 = biogears::LengthUnit::IsValidUnit("mm");
  bool unit3 = biogears::LengthUnit::IsValidUnit("um");
  bool unit4 = biogears::LengthUnit::IsValidUnit("in");
  bool unit5 = biogears::LengthUnit::IsValidUnit("ft");
  EXPECT_EQ(unit0, true);
  EXPECT_EQ(unit1, true);
  EXPECT_EQ(unit2, true);
  EXPECT_EQ(unit3, true);
  EXPECT_EQ(unit4, true);
  EXPECT_EQ(unit5, true);
  bool unit6 = biogears::LengthUnit::IsValidUnit("DEADBEEF");
  EXPECT_EQ(unit6, false);
}

TEST_F(TEST_FIXTURE_NAME, GetCompoundUnit)
{
  biogears::LengthUnit mu0 = biogears::LengthUnit::GetCompoundUnit("m");
  biogears::LengthUnit mu1 = biogears::LengthUnit::GetCompoundUnit("cm");
  biogears::LengthUnit mu2 = biogears::LengthUnit::GetCompoundUnit("mm");
  biogears::LengthUnit mu3 = biogears::LengthUnit::GetCompoundUnit("um");
  biogears::LengthUnit mu4 = biogears::LengthUnit::GetCompoundUnit("in");
  biogears::LengthUnit mu5 = biogears::LengthUnit::GetCompoundUnit("ft");
  EXPECT_EQ(mu0, biogears::LengthUnit::m);
  EXPECT_EQ(mu1, biogears::LengthUnit::cm);
  EXPECT_EQ(mu2, biogears::LengthUnit::mm);
  EXPECT_EQ(mu3, biogears::LengthUnit::um);
  EXPECT_EQ(mu4, biogears::LengthUnit::in);
  EXPECT_EQ(mu5, biogears::LengthUnit::ft);
  EXPECT_THROW(biogears::LengthUnit::GetCompoundUnit("DEADBEEF"),biogears::CommonDataModelException);
}