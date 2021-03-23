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
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

#ifdef DISABLE_BIOGEARS_SERespiratorySystem_TEST
#define TEST_FIXTURE_NAME DISABLED_SERespiratorySystem_Fixture
#else
#define TEST_FIXTURE_NAME SERespiratorySystem_Fixture
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
  biogears::SERespiratorySystem* es;
};

void TEST_FIXTURE_NAME::SetUp()
{
  logger = new biogears::Logger;
  es = new biogears::SERespiratorySystem(logger);
}

void TEST_FIXTURE_NAME::TearDown()
{
  delete es;
  delete logger;
}
TEST_F(TEST_FIXTURE_NAME, AlveolarArterialGradient)
{
  EXPECT_FALSE( es->HasAlveolarArterialGradient());
  es->GetAlveolarArterialGradient().SetValue(1.0,PressureUnit::Pa);
  EXPECT_TRUE( es->HasAlveolarArterialGradient());
  EXPECT_EQ(1.0, es->GetAlveolarArterialGradient(PressureUnit::Pa));

  const biogears::SERespiratorySystem ces(logger);
  EXPECT_FALSE( ces.HasAlveolarArterialGradient());
  auto dvalue = ces.GetAlveolarArterialGradient(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, CarricoIndex)
{
  EXPECT_FALSE( es->HasCarricoIndex());
  es->GetCarricoIndex().SetValue(1.0,PressureUnit::Pa);
  EXPECT_TRUE( es->HasCarricoIndex());
  EXPECT_EQ(1.0, es->GetCarricoIndex(PressureUnit::Pa));

  const biogears::SERespiratorySystem ces(logger);
  EXPECT_FALSE( ces.HasCarricoIndex());
  auto dvalue = ces.GetCarricoIndex(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, EndTidalCarbonDioxidePressure)
{
  EXPECT_FALSE( es->HasEndTidalCarbonDioxidePressure());
  es->GetEndTidalCarbonDioxidePressure().SetValue(1.0,PressureUnit::Pa);
  EXPECT_TRUE( es->HasEndTidalCarbonDioxidePressure());
  EXPECT_EQ(1.0, es->GetEndTidalCarbonDioxidePressure(PressureUnit::Pa));

  const biogears::SERespiratorySystem ces(logger);
  EXPECT_FALSE( ces.HasEndTidalCarbonDioxidePressure());
  auto dvalue = ces.GetEndTidalCarbonDioxidePressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RespirationDriverPressure)
{
  EXPECT_FALSE( es->HasRespirationDriverPressure());
  es->GetRespirationDriverPressure().SetValue(1.0,PressureUnit::Pa);
  EXPECT_TRUE( es->HasRespirationDriverPressure());
  EXPECT_EQ(1.0, es->GetRespirationDriverPressure(PressureUnit::Pa));

  const biogears::SERespiratorySystem ces(logger);
  EXPECT_FALSE( ces.HasRespirationDriverPressure());
  auto dvalue = ces.GetRespirationDriverPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RespirationMusclePressure)
{
  EXPECT_FALSE( es->HasRespirationMusclePressure());
  es->GetRespirationMusclePressure().SetValue(1.0,PressureUnit::Pa);
  EXPECT_TRUE( es->HasRespirationMusclePressure());
  EXPECT_EQ(1.0, es->GetRespirationMusclePressure(PressureUnit::Pa));

  const biogears::SERespiratorySystem ces(logger);
  EXPECT_FALSE( ces.HasRespirationMusclePressure());
  auto dvalue = ces.GetRespirationMusclePressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, TranspulmonaryPressure)
{
  EXPECT_FALSE( es->HasTranspulmonaryPressure());
  es->GetTranspulmonaryPressure().SetValue(1.0,PressureUnit::Pa);
  EXPECT_TRUE( es->HasTranspulmonaryPressure());
  EXPECT_EQ(1.0, es->GetTranspulmonaryPressure(PressureUnit::Pa));

  const biogears::SERespiratorySystem ces(logger);
  EXPECT_FALSE( ces.HasTranspulmonaryPressure());
  auto dvalue = ces.GetTranspulmonaryPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, ExpiratoryFlow)
{
  EXPECT_FALSE( es->HasExpiratoryFlow());
  es->GetExpiratoryFlow().SetValue(1.0,VolumePerTimeUnit::L_Per_min);
  EXPECT_TRUE( es->HasExpiratoryFlow());
  EXPECT_EQ(1.0, es->GetExpiratoryFlow(VolumePerTimeUnit::L_Per_min));

  const biogears::SERespiratorySystem ces(logger);
  EXPECT_FALSE( ces.HasExpiratoryFlow());
  auto dvalue = ces.GetExpiratoryFlow(VolumePerTimeUnit::L_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, InspiratoryFlow)
{
  EXPECT_FALSE( es->HasInspiratoryFlow());
  es->GetInspiratoryFlow().SetValue(1.0,VolumePerTimeUnit::L_Per_min);
  EXPECT_TRUE( es->HasInspiratoryFlow());
  EXPECT_EQ(1.0, es->GetInspiratoryFlow(VolumePerTimeUnit::L_Per_min));

  const biogears::SERespiratorySystem ces(logger);
  EXPECT_FALSE( ces.HasInspiratoryFlow());
  auto dvalue = ces.GetInspiratoryFlow(VolumePerTimeUnit::L_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, TargetPulmonaryVentilation)
{
  EXPECT_FALSE( es->HasTargetPulmonaryVentilation());
  es->GetTargetPulmonaryVentilation().SetValue(1.0,VolumePerTimeUnit::L_Per_min);
  EXPECT_TRUE( es->HasTargetPulmonaryVentilation());
  EXPECT_EQ(1.0, es->GetTargetPulmonaryVentilation(VolumePerTimeUnit::L_Per_min));

  const biogears::SERespiratorySystem ces(logger);
  EXPECT_FALSE( ces.HasTargetPulmonaryVentilation());
  auto dvalue = ces.GetTargetPulmonaryVentilation(VolumePerTimeUnit::L_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, TotalAlveolarVentilation)
{
  EXPECT_FALSE( es->HasTotalAlveolarVentilation());
  es->GetTotalAlveolarVentilation().SetValue(1.0,VolumePerTimeUnit::L_Per_min);
  EXPECT_TRUE( es->HasTotalAlveolarVentilation());
  EXPECT_EQ(1.0, es->GetTotalAlveolarVentilation(VolumePerTimeUnit::L_Per_min));

  const biogears::SERespiratorySystem ces(logger);
  EXPECT_FALSE( ces.HasTotalAlveolarVentilation());
  auto dvalue = ces.GetTotalAlveolarVentilation(VolumePerTimeUnit::L_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, TotalDeadSpaceVentilation)
{
  EXPECT_FALSE( es->HasTotalDeadSpaceVentilation());
  es->GetTotalDeadSpaceVentilation().SetValue(1.0,VolumePerTimeUnit::L_Per_min);
  EXPECT_TRUE( es->HasTotalDeadSpaceVentilation());
  EXPECT_EQ(1.0, es->GetTotalDeadSpaceVentilation(VolumePerTimeUnit::L_Per_min));

  const biogears::SERespiratorySystem ces(logger);
  EXPECT_FALSE( ces.HasTotalDeadSpaceVentilation());
  auto dvalue = ces.GetTotalDeadSpaceVentilation(VolumePerTimeUnit::L_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, TidalVolume)
{
  EXPECT_FALSE( es->HasTidalVolume());
  es->GetTidalVolume().SetValue(1.0,VolumeUnit::L);
  EXPECT_TRUE( es->HasTidalVolume());
  EXPECT_EQ(1.0, es->GetTidalVolume(VolumeUnit::L));

  const biogears::SERespiratorySystem ces(logger);
  EXPECT_FALSE( ces.HasTidalVolume());
  auto dvalue = ces.GetTidalVolume(VolumeUnit::L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, TotalLungVolume)
{
  EXPECT_FALSE( es->HasTotalLungVolume());
  es->GetTotalLungVolume().SetValue(1.0,VolumeUnit::L);
  EXPECT_TRUE( es->HasTotalLungVolume());
  EXPECT_EQ(1.0, es->GetTotalLungVolume(VolumeUnit::L));

  const biogears::SERespiratorySystem ces(logger);
  EXPECT_FALSE( ces.HasTotalLungVolume());
  auto dvalue = ces.GetTotalLungVolume(VolumeUnit::L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, PulmonaryCompliance)
{
  EXPECT_FALSE( es->HasPulmonaryCompliance());
  es->GetPulmonaryCompliance().SetValue(1.0,FlowComplianceUnit::L_Per_cmH2O);
  EXPECT_TRUE( es->HasPulmonaryCompliance());
  EXPECT_EQ(1.0, es->GetPulmonaryCompliance(FlowComplianceUnit::L_Per_cmH2O));

  const biogears::SERespiratorySystem ces(logger);
  EXPECT_FALSE( ces.HasPulmonaryCompliance());
  auto dvalue = ces.GetPulmonaryCompliance(FlowComplianceUnit::L_Per_cmH2O);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, PulmonaryResistance)
{
  EXPECT_FALSE( es->HasPulmonaryResistance());
  es->GetPulmonaryResistance().SetValue(1.0,FlowResistanceUnit::Pa_s_Per_m3);
  EXPECT_TRUE( es->HasPulmonaryResistance());
  EXPECT_EQ(1.0, es->GetPulmonaryResistance(FlowResistanceUnit::Pa_s_Per_m3));

  const biogears::SERespiratorySystem ces(logger);
  EXPECT_FALSE( ces.HasPulmonaryResistance());
  auto dvalue = ces.GetPulmonaryResistance(FlowResistanceUnit::Pa_s_Per_m3);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RespirationRate)
{
  EXPECT_FALSE( es->HasRespirationRate());
  es->GetRespirationRate().SetValue(1.0,FrequencyUnit::Hz);
  EXPECT_TRUE( es->HasRespirationRate());
  EXPECT_EQ(1.0, es->GetRespirationRate(FrequencyUnit::Hz));

  const biogears::SERespiratorySystem ces(logger);
  EXPECT_FALSE( ces.HasRespirationRate());
  auto dvalue = ces.GetRespirationRate(FrequencyUnit::Hz);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, EndTidalCarbonDioxideFraction)
{
  EXPECT_FALSE( es->HasEndTidalCarbonDioxideFraction());
  es->GetEndTidalCarbonDioxideFraction().SetValue(1.0);
  EXPECT_TRUE( es->HasEndTidalCarbonDioxideFraction());
  EXPECT_EQ(1.0, es->GetEndTidalCarbonDioxideFraction().GetValue());

  const biogears::SERespiratorySystem ces(logger);
  EXPECT_FALSE( ces.HasEndTidalCarbonDioxideFraction());
  auto dvalue = ces.GetEndTidalCarbonDioxideFraction();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, InspiratoryExpiratoryRatio)
{
  EXPECT_FALSE( es->HasInspiratoryExpiratoryRatio());
  es->GetInspiratoryExpiratoryRatio().SetValue(1.0);
  EXPECT_TRUE( es->HasInspiratoryExpiratoryRatio());
  EXPECT_EQ(1.0, es->GetInspiratoryExpiratoryRatio().GetValue());

  const biogears::SERespiratorySystem ces(logger);
  EXPECT_FALSE( ces.HasInspiratoryExpiratoryRatio());
  auto dvalue = ces.GetInspiratoryExpiratoryRatio();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, SpecificVentilation)
{
  EXPECT_FALSE( es->HasSpecificVentilation());
  es->GetSpecificVentilation().SetValue(1.0);
  EXPECT_TRUE( es->HasSpecificVentilation());
  EXPECT_EQ(1.0, es->GetSpecificVentilation().GetValue());

  const biogears::SERespiratorySystem ces(logger);
  EXPECT_FALSE( ces.HasSpecificVentilation());
  auto dvalue = ces.GetSpecificVentilation();
  EXPECT_NE(dvalue, dvalue);
}
