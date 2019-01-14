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

#include <biogears/cdm/properties/SEScalarAmount.h>


#ifdef DISABLE_BIOGEARS_SEScalarAmount_TEST
#define TEST_FIXTURE_NAME DISABLED_SEScalarAmount_Fixture
#else
#define TEST_FIXTURE_NAME SEScalarAmount_Fixture
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
  biogears::SEScalarAmount amount = biogears::SEScalarAmount();
  auto ptr = amount.Unload();
  EXPECT_EQ(ptr, nullptr);
}

TEST_F(TEST_FIXTURE_NAME, IsValidUnit)
{
  bool unit0 = biogears::AmountUnit::IsValidUnit("mol");
  bool unit1 = biogears::AmountUnit::IsValidUnit("pmol");
  EXPECT_EQ(unit0, true);
  EXPECT_EQ(unit1, true);
  bool unit5 = biogears::AmountUnit::IsValidUnit("DEADBEEF");
  EXPECT_EQ(unit5, false);
}

TEST_F(TEST_FIXTURE_NAME, GetCompoundUnit)
{
  biogears::AmountUnit mu0 = biogears::AmountUnit::GetCompoundUnit("mol");
  biogears::AmountUnit mu1 = biogears::AmountUnit::GetCompoundUnit("pmol");
  EXPECT_EQ(mu0, biogears::AmountUnit::mol);
  EXPECT_EQ(mu1, biogears::AmountUnit::pmol);
  EXPECT_THROW(biogears::AmountUnit::GetCompoundUnit("DEADBEEF"),biogears::CommonDataModelException);
}