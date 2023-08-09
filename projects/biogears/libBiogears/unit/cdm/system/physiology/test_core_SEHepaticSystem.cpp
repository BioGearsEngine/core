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
#include <biogears/cdm/system/physiology/SEHepaticSystem.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

#ifdef DISABLE_BIOGEARS_SEHepaticSystem_TEST
#define TEST_FIXTURE_NAME DISABLED_SEHepaticSystem_Fixture
#else
#define TEST_FIXTURE_NAME SEHepaticSystem_Fixture
#endif

USING_BIOGEARS_NAMESPACE
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

  BIOGEARS_NAMESPACE Logger* logger;
  BIOGEARS_NAMESPACE SEHepaticSystem* es;
};

void TEST_FIXTURE_NAME::SetUp()
{
  logger = new BIOGEARS_NAMESPACE Logger;
  es = new BIOGEARS_NAMESPACE SEHepaticSystem(logger);
}

void TEST_FIXTURE_NAME::TearDown()
{
  delete es;
  delete logger;
}
TEST_F(TEST_FIXTURE_NAME, KetoneProductionRate)
{
  EXPECT_FALSE( es->HasKetoneProductionRate());
  es->GetKetoneProductionRate().SetValue(1.0,AmountPerTimeUnit::mmol_Per_min);
  EXPECT_TRUE( es->HasKetoneProductionRate());
  EXPECT_EQ(1.0, es->GetKetoneProductionRate(AmountPerTimeUnit::mmol_Per_min));

  const BIOGEARS_NAMESPACE SEHepaticSystem ces(logger);
  EXPECT_FALSE( ces.HasKetoneProductionRate());
  auto dvalue = ces.GetKetoneProductionRate(AmountPerTimeUnit::mmol_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, HepaticGluconeogenesisRate)
{
  EXPECT_FALSE( es->HasHepaticGluconeogenesisRate());
  es->GetHepaticGluconeogenesisRate().SetValue(1.0, MassPerTimeUnit::g_Per_day);
  EXPECT_TRUE( es->HasHepaticGluconeogenesisRate());
  EXPECT_EQ(1.0, es->GetHepaticGluconeogenesisRate().GetValue(MassPerTimeUnit::g_Per_day));

  const BIOGEARS_NAMESPACE SEHepaticSystem cds(logger);
  EXPECT_FALSE( cds.HasHepaticGluconeogenesisRate());
  auto dvalue = cds.GetHepaticGluconeogenesisRate(MassPerTimeUnit::g_Per_day);
  EXPECT_NE(dvalue, dvalue);
}
