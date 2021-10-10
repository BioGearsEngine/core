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
#include <biogears/cdm/system/physiology/SEEndocrineSystem.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

#ifdef DISABLE_BIOGEARS_SEEndocrineSystem_TEST
#define TEST_FIXTURE_NAME DISABLED_SEEndocrineSystem_Fixture
#else
#define TEST_FIXTURE_NAME SEEndocrineSystem_Fixture
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
  biogears::SEEndocrineSystem* es;
};

void TEST_FIXTURE_NAME::SetUp()
{
  logger = new biogears::Logger;
  es = new biogears::SEEndocrineSystem(logger);
}

void TEST_FIXTURE_NAME::TearDown()
{
  delete es;
  delete logger;
}
TEST_F(TEST_FIXTURE_NAME, InsulinSynthesisRate)
{
  EXPECT_FALSE( es->HasInsulinSynthesisRate());
  es->GetInsulinSynthesisRate().SetValue(1.0,AmountPerTimeUnit::mmol_Per_min);
  EXPECT_TRUE( es->HasInsulinSynthesisRate());
  EXPECT_EQ(1.0, es->GetInsulinSynthesisRate(AmountPerTimeUnit::mmol_Per_min));

  const biogears::SEEndocrineSystem ces(logger);
  EXPECT_FALSE( ces.HasInsulinSynthesisRate());
  auto dvalue = ces.GetInsulinSynthesisRate(AmountPerTimeUnit::mmol_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, GlucagonSynthesisRate)
{
  EXPECT_FALSE( es->HasGlucagonSynthesisRate());
  es->GetGlucagonSynthesisRate().SetValue(1.0, AmountPerTimeUnit::mmol_Per_min);
  EXPECT_TRUE( es->HasGlucagonSynthesisRate());
  EXPECT_EQ(1.0, es->GetGlucagonSynthesisRate(AmountPerTimeUnit::mmol_Per_min));

  const biogears::SEEndocrineSystem ces(logger);
  EXPECT_FALSE( ces.HasGlucagonSynthesisRate());
  auto dvalue = ces.GetGlucagonSynthesisRate(AmountPerTimeUnit::mmol_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
