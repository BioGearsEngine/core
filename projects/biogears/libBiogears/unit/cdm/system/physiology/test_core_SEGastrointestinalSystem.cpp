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
#include <limits>
#include <thread>

#include <gtest/gtest.h>

#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/system/physiology/SEGastrointestinalSystem.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

#ifdef DISABLE_BIOGEARS_SEGastrointestinalSystem_TEST
#define TEST_FIXTURE_NAME DISABLED_SEGastrointestinalSystem_Fixture
#else
#define TEST_FIXTURE_NAME SEGastrointestinalSystem_Fixture
#endif

using namespace biogears;
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

  biogears::Logger* logger;
  biogears::SEGastrointestinalSystem* es;
};

void TEST_FIXTURE_NAME::SetUp()
{
  logger = new biogears::Logger;
  es = new biogears::SEGastrointestinalSystem(logger);
}

void TEST_FIXTURE_NAME::TearDown()
{
  delete es;
  delete logger;
}
TEST_F(TEST_FIXTURE_NAME, ChymeAbsorptionRate)
{
  EXPECT_FALSE( es->HasChymeAbsorptionRate());
  es->GetChymeAbsorptionRate().SetValue(1.0,VolumePerTimeUnit::L_Per_day);
  EXPECT_TRUE( es->HasChymeAbsorptionRate());
  EXPECT_EQ(1.0, es->GetChymeAbsorptionRate(VolumePerTimeUnit::L_Per_day));

  const biogears::SEGastrointestinalSystem ces(logger);
  EXPECT_FALSE( ces.HasChymeAbsorptionRate());
  auto dvalue = ces.GetChymeAbsorptionRate(VolumePerTimeUnit::L_Per_day);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, StomachContents)
{
  EXPECT_FALSE( es->HasStomachContents());
  es->GetStomachContents();
  EXPECT_TRUE( es->HasStomachContents());

  const biogears::SEGastrointestinalSystem ces(logger);
  EXPECT_FALSE( ces.HasStomachContents());
  auto dvalue = ces.GetStomachContents();
  EXPECT_EQ(NULL, dvalue);
}
