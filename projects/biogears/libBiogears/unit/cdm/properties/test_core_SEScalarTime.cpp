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

#include <biogears/cdm/properties/SEScalarTime.h>


#ifdef DISABLE_BIOGEARS_SEScalarTime_TEST
#define TEST_FIXTURE_NAME DISABLED_SEScalarTime_Fixture
#else
#define TEST_FIXTURE_NAME SEScalarTime_Fixture
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
  BIOGEARS_NAMESPACE SEScalarTime time = BIOGEARS_NAMESPACE SEScalarTime();
  auto ptr = time.Unload();
  EXPECT_EQ(ptr, nullptr);
}

TEST_F(TEST_FIXTURE_NAME, IsValidUnit)
{
  bool unit0 = BIOGEARS_NAMESPACE TimeUnit::IsValidUnit("s");
  bool unit1 = BIOGEARS_NAMESPACE TimeUnit::IsValidUnit("min");
  bool unit2 = BIOGEARS_NAMESPACE TimeUnit::IsValidUnit("hr");
  bool unit3 = BIOGEARS_NAMESPACE TimeUnit::IsValidUnit("day");
  bool unit4 = BIOGEARS_NAMESPACE TimeUnit::IsValidUnit("yr");
  EXPECT_EQ(unit0, true);
  EXPECT_EQ(unit1, true);
  EXPECT_EQ(unit2, true);
  EXPECT_EQ(unit3, true);
  EXPECT_EQ(unit4, true);
  bool unit5 = BIOGEARS_NAMESPACE TimeUnit::IsValidUnit("DEADBEEF");
  EXPECT_EQ(unit5, false);
}

TEST_F(TEST_FIXTURE_NAME, GetCompoundUnit)
{
  BIOGEARS_NAMESPACE TimeUnit mu0 = BIOGEARS_NAMESPACE TimeUnit::GetCompoundUnit("s");
  BIOGEARS_NAMESPACE TimeUnit mu1 = BIOGEARS_NAMESPACE TimeUnit::GetCompoundUnit("min");
  BIOGEARS_NAMESPACE TimeUnit mu2 = BIOGEARS_NAMESPACE TimeUnit::GetCompoundUnit("hr");
  BIOGEARS_NAMESPACE TimeUnit mu3 = BIOGEARS_NAMESPACE TimeUnit::GetCompoundUnit("day");
  BIOGEARS_NAMESPACE TimeUnit mu4 = BIOGEARS_NAMESPACE TimeUnit::GetCompoundUnit("yr");
  EXPECT_EQ(mu0, BIOGEARS_NAMESPACE TimeUnit::s);
  EXPECT_EQ(mu1, BIOGEARS_NAMESPACE TimeUnit::min);
  EXPECT_EQ(mu2, BIOGEARS_NAMESPACE TimeUnit::hr);
  EXPECT_EQ(mu3, BIOGEARS_NAMESPACE TimeUnit::day);
  EXPECT_EQ(mu4, BIOGEARS_NAMESPACE TimeUnit::yr);
  EXPECT_THROW(BIOGEARS_NAMESPACE TimeUnit::GetCompoundUnit("DEADBEEF"),BIOGEARS_NAMESPACE CommonDataModelException);
}