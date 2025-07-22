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

#include <biogears/cdm/properties/SEScalarLengthPerTime.h>


#ifdef DISABLE_BIOGEARS_SEScalarLengthPerTime_TEST
#define TEST_FIXTURE_NAME DISABLED_SEScalarLengthPerTime_Fixture
#else
#define TEST_FIXTURE_NAME SEScalarLengthPerTime_Fixture
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
  bool unit0 = biogears::LengthPerTimeUnit::IsValidUnit("m/s");
  bool unit1 = biogears::LengthPerTimeUnit::IsValidUnit("cm/s");
  bool unit2 = biogears::LengthPerTimeUnit::IsValidUnit("m/min");
  bool unit3 = biogears::LengthPerTimeUnit::IsValidUnit("cm/min");
  bool unit4 = biogears::LengthPerTimeUnit::IsValidUnit("ft/s");
  bool unit5 = biogears::LengthPerTimeUnit::IsValidUnit("ft/min");
  EXPECT_EQ(unit0, true);
  EXPECT_EQ(unit1, true);
  EXPECT_EQ(unit2, true);
  EXPECT_EQ(unit3, true);
  EXPECT_EQ(unit4, true);
  EXPECT_EQ(unit5, true);
  bool unit6 = biogears::LengthPerTimeUnit::IsValidUnit("DEADBEEF");
  EXPECT_EQ(unit6, false);
}

TEST_F(TEST_FIXTURE_NAME, GetCompoundUnit)
{
  biogears::LengthPerTimeUnit mu0 = biogears::LengthPerTimeUnit::GetCompoundUnit("m/s");
  biogears::LengthPerTimeUnit mu1 = biogears::LengthPerTimeUnit::GetCompoundUnit("cm/s");
  biogears::LengthPerTimeUnit mu2 = biogears::LengthPerTimeUnit::GetCompoundUnit("m/min");
  biogears::LengthPerTimeUnit mu3 = biogears::LengthPerTimeUnit::GetCompoundUnit("cm/min");
  biogears::LengthPerTimeUnit mu4 = biogears::LengthPerTimeUnit::GetCompoundUnit("ft/s");
  biogears::LengthPerTimeUnit mu5 = biogears::LengthPerTimeUnit::GetCompoundUnit("ft/min");
  EXPECT_EQ(mu0, biogears::LengthPerTimeUnit::m_Per_s);
  EXPECT_EQ(mu1, biogears::LengthPerTimeUnit::cm_Per_s);
  EXPECT_EQ(mu2, biogears::LengthPerTimeUnit::m_Per_min);
  EXPECT_EQ(mu3, biogears::LengthPerTimeUnit::cm_Per_min);
  EXPECT_EQ(mu4, biogears::LengthPerTimeUnit::ft_Per_s);
  EXPECT_EQ(mu5, biogears::LengthPerTimeUnit::ft_Per_min);
  EXPECT_THROW(biogears::LengthPerTimeUnit::GetCompoundUnit("DEADBEEF"),biogears::CommonDataModelException);
}