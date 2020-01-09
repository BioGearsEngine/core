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
#include <biogears/cdm/system/physiology/SENervousSystem.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

#ifdef DISABLE_BIOGEARS_SENervousSystem_TEST
#define TEST_FIXTURE_NAME DISABLED_Core_SENervousSystem_Fixture
#else
#define TEST_FIXTURE_NAME Core_SENervousSystem_Fixture
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
  biogears::SENervousSystem* es;
};

void TEST_FIXTURE_NAME::SetUp()
{
  logger = new biogears::Logger;
  es = new biogears::SENervousSystem(logger);
}

void TEST_FIXTURE_NAME::TearDown()
{
  delete es;
  delete logger;
}
TEST_F(TEST_FIXTURE_NAME, HeartRateScale)
{
  EXPECT_FALSE( es->HasHeartRateScale());
  es->GetHeartRateScale().SetValue(1.0);
  EXPECT_TRUE( es->HasHeartRateScale());
  EXPECT_EQ(1.0, es->GetHeartRateScale().GetValue());

  const biogears::SENervousSystem ces(logger);
  EXPECT_FALSE( ces.HasHeartRateScale());
  auto dvalue = ces.GetHeartRateScale();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, HeartElastanceScale)
{
  EXPECT_FALSE( es->HasHeartElastanceScale());
  es->GetHeartElastanceScale().SetValue(1.0);
  EXPECT_TRUE( es->HasHeartElastanceScale());
  EXPECT_EQ(1.0, es->GetHeartElastanceScale().GetValue());

  const biogears::SENervousSystem ces(logger);
  EXPECT_FALSE( ces.HasHeartElastanceScale());
  auto dvalue = ces.GetHeartElastanceScale();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, ResistanceScaleExtrasplanchnic)
{
  EXPECT_FALSE( es->HasResistanceScaleExtrasplanchnic());
  es->GetResistanceScaleExtrasplanchnic().SetValue(1.0);
  EXPECT_TRUE( es->HasResistanceScaleExtrasplanchnic());
  EXPECT_EQ(1.0, es->GetResistanceScaleExtrasplanchnic().GetValue());

  const biogears::SENervousSystem ces(logger);
  EXPECT_FALSE( ces.HasResistanceScaleExtrasplanchnic());
  auto dvalue = ces.GetResistanceScaleExtrasplanchnic();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, ResistanceScaleMuscle)
{
  EXPECT_FALSE(es->HasResistanceScaleMuscle());
  es->GetResistanceScaleMuscle().SetValue(1.0);
  EXPECT_TRUE(es->HasResistanceScaleMuscle());
  EXPECT_EQ(1.0, es->GetResistanceScaleMuscle().GetValue());

  const biogears::SENervousSystem ces(logger);
  EXPECT_FALSE(ces.HasResistanceScaleMuscle());
  auto dvalue = ces.GetResistanceScaleMuscle();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, ResistanceScaleMyocardium)
{
  EXPECT_FALSE(es->HasResistanceScaleMyocardium());
  es->GetResistanceScaleMyocardium().SetValue(1.0);
  EXPECT_TRUE(es->HasResistanceScaleMyocardium());
  EXPECT_EQ(1.0, es->GetResistanceScaleMyocardium().GetValue());

  const biogears::SENervousSystem ces(logger);
  EXPECT_FALSE(ces.HasResistanceScaleMyocardium());
  auto dvalue = ces.GetResistanceScaleMyocardium();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, ResistanceScaleSplanchnic)
{
  EXPECT_FALSE(es->HasResistanceScaleSplanchnic());
  es->GetResistanceScaleSplanchnic().SetValue(1.0);
  EXPECT_TRUE(es->HasResistanceScaleSplanchnic());
  EXPECT_EQ(1.0, es->GetResistanceScaleSplanchnic().GetValue());

  const biogears::SENervousSystem ces(logger);
  EXPECT_FALSE(ces.HasResistanceScaleSplanchnic());
  auto dvalue = ces.GetResistanceScaleSplanchnic();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, ComplianceScale)
{
  EXPECT_FALSE( es->HasComplianceScale());
  es->GetComplianceScale().SetValue(1.0);
  EXPECT_TRUE( es->HasComplianceScale());
  EXPECT_EQ(1.0, es->GetComplianceScale().GetValue());

  const biogears::SENervousSystem ces(logger);
  EXPECT_FALSE( ces.HasComplianceScale());
  auto dvalue = ces.GetComplianceScale();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, PainVisualAnalogueScale)
{
  EXPECT_FALSE( es->HasPainVisualAnalogueScale());
  es->GetPainVisualAnalogueScale().SetValue(1.0);
  EXPECT_TRUE( es->HasPainVisualAnalogueScale());
  EXPECT_EQ(1.0, es->GetPainVisualAnalogueScale().GetValue());

  const biogears::SENervousSystem ces(logger);
  EXPECT_FALSE( ces.HasPainVisualAnalogueScale());
  auto dvalue = ces.GetPainVisualAnalogueScale();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftEyePupillaryResponse)
{
  EXPECT_FALSE( es->HasLeftEyePupillaryResponse());
  es->GetLeftEyePupillaryResponse();
  EXPECT_TRUE( es->HasLeftEyePupillaryResponse());
  es->RemoveLeftEyePupillaryResponse();
  EXPECT_FALSE( es->HasLeftEyePupillaryResponse());

  const biogears::SENervousSystem ces(logger);
  EXPECT_FALSE( ces.HasLeftEyePupillaryResponse());
  auto dvalue = ces.GetLeftEyePupillaryResponse(); // so this returns a NULL instead of a nan
  EXPECT_EQ(NULL, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightEyePupillaryResponse)
{
  EXPECT_FALSE( es->HasRightEyePupillaryResponse());
  es->GetRightEyePupillaryResponse();
  EXPECT_TRUE( es->HasRightEyePupillaryResponse());
  es->RemoveRightEyePupillaryResponse();
  EXPECT_FALSE( es->HasRightEyePupillaryResponse());

  const biogears::SENervousSystem cds(logger);
  EXPECT_FALSE( cds.HasRightEyePupillaryResponse());
  auto dvalue = cds.GetRightEyePupillaryResponse(); // so this returns a NULL instead of a nan
  EXPECT_EQ(NULL, dvalue);
}
//LeftEyePupillaryResponse
//RightEyePupillaryResponse
