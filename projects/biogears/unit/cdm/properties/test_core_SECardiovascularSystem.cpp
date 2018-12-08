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

#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

#ifdef DISABLE_BIOGEARS_SECardiovascularSystem_TEST
#define TEST_FIXTURE_NAME DISABLED_SECardiovascularSystem_Fixture
#else
#define TEST_FIXTURE_NAME SESCardiovascularSystem_Fixture
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
  virtual void SetUp();

  // Code here will be called immediately after each test (right
  // before the destructor).
  virtual void TearDown();

  biogears::Logger* logger;
  biogears::SECardiovascularSystem* cs;
};
void TEST_FIXTURE_NAME::SetUp()
{
  logger = new biogears::Logger;
  cs = new biogears::SECardiovascularSystem(logger);
}
void TEST_FIXTURE_NAME::TearDown()
{
}
TEST_F(TEST_FIXTURE_NAME, ArterialPressure)
{
  EXPECT_EQ(false, cs->HasArterialPressure());
  cs->GetArterialPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, cs->HasArterialPressure());
  EXPECT_EQ(1.0, cs->GetArterialPressure(PressureUnit::Pa));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasArterialPressure());
  auto dvalue = ccs.GetArterialPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, BloodVolume)
{
  EXPECT_EQ(false, cs->HasBloodVolume());
  cs->GetBloodVolume().SetValue(1.0, VolumeUnit::L);
  EXPECT_EQ(true, cs->HasBloodVolume());
  EXPECT_EQ(1.0, cs->GetBloodVolume(VolumeUnit::L));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasBloodVolume());
  auto dvalue = ccs.GetBloodVolume(VolumeUnit::L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, CardiacOutput)
{
  EXPECT_EQ(false, cs->HasCardiacOutput());
  cs->GetCardiacOutput().SetValue(1.0, VolumePerTimeUnit::L_Per_day);
  EXPECT_EQ(true, cs->HasCardiacOutput());
  EXPECT_EQ(1.0, cs->GetCardiacOutput(VolumePerTimeUnit::L_Per_day));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasCardiacOutput());
  auto dvalue = ccs.GetCardiacOutput(VolumePerTimeUnit::L_Per_day);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, CardiacIndex)
{
  EXPECT_EQ(false, cs->HasCardiacIndex());
  cs->GetCardiacIndex().SetValue(1.0, VolumePerTimeAreaUnit::L_Per_min_m2);
  EXPECT_EQ(true, cs->HasCardiacIndex());
  EXPECT_EQ(1.0, cs->GetCardiacIndex(VolumePerTimeAreaUnit::L_Per_min_m2));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasCardiacIndex());
  auto dvalue = ccs.GetCardiacIndex(VolumePerTimeAreaUnit::L_Per_min_m2);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, CentralVenousPressure)
{
  EXPECT_EQ(false, cs->HasCentralVenousPressure());
  cs->GetCentralVenousPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, cs->HasCentralVenousPressure());
  EXPECT_EQ(1.0, cs->GetCentralVenousPressure(PressureUnit::Pa));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasCentralVenousPressure());
  auto dvalue = ccs.GetCentralVenousPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, CerebralBloodFlow)
{
  EXPECT_EQ(false, cs->HasCerebralBloodFlow());
  cs->GetCerebralBloodFlow().SetValue(1.0, VolumePerTimeUnit::L_Per_day);
  EXPECT_EQ(true, cs->HasCerebralBloodFlow());
  EXPECT_EQ(1.0, cs->GetCerebralBloodFlow(VolumePerTimeUnit::L_Per_day));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasCerebralBloodFlow());
  auto dvalue = ccs.GetCerebralBloodFlow(VolumePerTimeUnit::L_Per_day);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, CerebralPerfusionPressure)
{
  EXPECT_EQ(false, cs->HasCerebralPerfusionPressure());
  cs->GetCerebralPerfusionPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, cs->HasCerebralPerfusionPressure());
  EXPECT_EQ(1.0, cs->GetCerebralPerfusionPressure(PressureUnit::Pa));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasCerebralPerfusionPressure());
  auto dvalue = ccs.GetCerebralPerfusionPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, DiastolicArterialPressure)
{
  EXPECT_EQ(false, cs->HasDiastolicArterialPressure());
  cs->GetDiastolicArterialPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, cs->HasDiastolicArterialPressure());
  EXPECT_EQ(1.0, cs->GetDiastolicArterialPressure(PressureUnit::Pa));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasDiastolicArterialPressure());
  auto dvalue = ccs.GetDiastolicArterialPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, HeartEjectionFraction)
{
  EXPECT_EQ(false, cs->HasHeartEjectionFraction());
  cs->GetHeartEjectionFraction().SetValue(1.0);
  EXPECT_EQ(true, cs->HasHeartEjectionFraction());
  EXPECT_EQ(1.0, cs->GetHeartEjectionFraction().GetValue());

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasHeartEjectionFraction());
  auto dvalue = ccs.GetHeartEjectionFraction();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, HeartRate)
{
  EXPECT_EQ(false, cs->HasHeartRate());
  cs->GetHeartRate().SetValue(1.0, FrequencyUnit::Hz);
  EXPECT_EQ(true, cs->HasHeartRate());
  EXPECT_EQ(1.0, cs->GetHeartRate(FrequencyUnit::Hz));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasHeartRate());
  auto dvalue = ccs.GetHeartRate(FrequencyUnit::Hz);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, HeartRhythm)
{
  EXPECT_EQ(false, cs->HasHeartRhythm());
  cs->GetHeartRhythm();
  cs->SetHeartRhythm(CDM::enumHeartRhythm::Asystole);
  EXPECT_EQ(true, cs->HasHeartRhythm());
  EXPECT_EQ(CDM::enumHeartRhythm::Asystole, cs->GetHeartRhythm());

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasHeartRhythm());
  auto dvalue = ccs.GetHeartRhythm();
  EXPECT_EQ((CDM::enumHeartRhythm::value)-1, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, HeartStrokeVolume)
{
  EXPECT_EQ(false, cs->HasHeartStrokeVolume());
  cs->GetHeartStrokeVolume().SetValue(1.0, VolumeUnit::L);
  EXPECT_EQ(true, cs->HasHeartStrokeVolume());
  EXPECT_EQ(1.0, cs->GetHeartStrokeVolume(VolumeUnit::L));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasHeartStrokeVolume());
  auto dvalue = ccs.GetHeartStrokeVolume(VolumeUnit::L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, IntracranialPressure)
{
  EXPECT_EQ(false, cs->HasIntracranialPressure());
  cs->GetIntracranialPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, cs->HasIntracranialPressure());
  EXPECT_EQ(1.0, cs->GetIntracranialPressure(PressureUnit::Pa));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasIntracranialPressure());
  auto dvalue = ccs.GetIntracranialPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, MeanArterialPressure)
{
  EXPECT_EQ(false, cs->HasMeanArterialPressure());
  cs->GetMeanArterialPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, cs->HasMeanArterialPressure());
  EXPECT_EQ(1.0, cs->GetMeanArterialPressure(PressureUnit::Pa));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasMeanArterialPressure());
  auto dvalue = ccs.GetMeanArterialPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, MeanArterialCarbonDioxidePartialPressure)
{
  EXPECT_EQ(false, cs->HasMeanArterialCarbonDioxidePartialPressure());
  cs->GetMeanArterialCarbonDioxidePartialPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, cs->HasMeanArterialCarbonDioxidePartialPressure());
  EXPECT_EQ(1.0, cs->GetMeanArterialCarbonDioxidePartialPressure(PressureUnit::Pa));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasMeanArterialCarbonDioxidePartialPressure());
  auto dvalue = ccs.GetMeanArterialCarbonDioxidePartialPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, MeanArterialCarbonDioxidePartialPressureDelta)
{
  EXPECT_EQ(false, cs->HasMeanArterialCarbonDioxidePartialPressureDelta());
  cs->GetMeanArterialCarbonDioxidePartialPressureDelta().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, cs->HasMeanArterialCarbonDioxidePartialPressureDelta());
  EXPECT_EQ(1.0, cs->GetMeanArterialCarbonDioxidePartialPressureDelta(PressureUnit::Pa));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasMeanArterialCarbonDioxidePartialPressureDelta());
  auto dvalue = ccs.GetMeanArterialCarbonDioxidePartialPressureDelta(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, MeanCentralVenousPressure)
{
  EXPECT_EQ(false, cs->HasMeanCentralVenousPressure());
  cs->GetMeanCentralVenousPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, cs->HasMeanCentralVenousPressure());
  EXPECT_EQ(1.0, cs->GetMeanCentralVenousPressure(PressureUnit::Pa));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasMeanCentralVenousPressure());
  auto dvalue = ccs.GetMeanCentralVenousPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, MeanSkinFlow)
{
  EXPECT_EQ(false, cs->HasMeanSkinFlow());
  cs->GetMeanSkinFlow().SetValue(1.0, VolumePerTimeUnit::L_Per_day);
  EXPECT_EQ(true, cs->HasMeanSkinFlow());
  EXPECT_EQ(1.0, cs->GetMeanSkinFlow(VolumePerTimeUnit::L_Per_day));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasMeanSkinFlow());
  auto dvalue = ccs.GetMeanSkinFlow(VolumePerTimeUnit::L_Per_day);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, PulmonaryArterialPressure)
{
  EXPECT_EQ(false, cs->HasPulmonaryArterialPressure());
  cs->GetPulmonaryArterialPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, cs->HasPulmonaryArterialPressure());
  EXPECT_EQ(1.0, cs->GetPulmonaryArterialPressure(PressureUnit::Pa));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasPulmonaryArterialPressure());
  auto dvalue = ccs.GetPulmonaryArterialPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, PulmonaryCapillariesWedgePressure)
{
  EXPECT_EQ(false, cs->HasPulmonaryCapillariesWedgePressure());
  cs->GetPulmonaryCapillariesWedgePressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, cs->HasPulmonaryCapillariesWedgePressure());
  EXPECT_EQ(1.0, cs->GetPulmonaryCapillariesWedgePressure(PressureUnit::Pa));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasPulmonaryCapillariesWedgePressure());
  auto dvalue = ccs.GetPulmonaryCapillariesWedgePressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, PulmonaryDiastolicArterialPressure)
{
  EXPECT_EQ(false, cs->HasPulmonaryDiastolicArterialPressure());
  cs->GetPulmonaryDiastolicArterialPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, cs->HasPulmonaryDiastolicArterialPressure());
  EXPECT_EQ(1.0, cs->GetPulmonaryDiastolicArterialPressure(PressureUnit::Pa));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasPulmonaryDiastolicArterialPressure());
  auto dvalue = ccs.GetPulmonaryDiastolicArterialPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, PulmonaryMeanArterialPressure)
{
  EXPECT_EQ(false, cs->HasPulmonaryMeanArterialPressure());
  cs->GetPulmonaryMeanArterialPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, cs->HasPulmonaryMeanArterialPressure());
  EXPECT_EQ(1.0, cs->GetPulmonaryMeanArterialPressure(PressureUnit::Pa));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasPulmonaryMeanArterialPressure());
  auto dvalue = ccs.GetPulmonaryMeanArterialPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, PulmonaryMeanCapillaryFlow)
{
  EXPECT_EQ(false, cs->HasPulmonaryMeanCapillaryFlow());
  cs->GetPulmonaryMeanCapillaryFlow().SetValue(1.0, VolumePerTimeUnit::L_Per_day);
  EXPECT_EQ(true, cs->HasPulmonaryMeanCapillaryFlow());
  EXPECT_EQ(1.0, cs->GetPulmonaryMeanCapillaryFlow(VolumePerTimeUnit::L_Per_day));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasPulmonaryMeanCapillaryFlow());
  auto dvalue = ccs.GetPulmonaryMeanCapillaryFlow(VolumePerTimeUnit::L_Per_day);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, PulmonaryMeanShuntFlow)
{
  EXPECT_EQ(false, cs->HasPulmonaryMeanShuntFlow());
  cs->GetPulmonaryMeanShuntFlow().SetValue(1.0, VolumePerTimeUnit::L_Per_day);
  EXPECT_EQ(true, cs->HasPulmonaryMeanShuntFlow());
  EXPECT_EQ(1.0, cs->GetPulmonaryMeanShuntFlow(VolumePerTimeUnit::L_Per_day));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasPulmonaryMeanShuntFlow());
  auto dvalue = ccs.GetPulmonaryMeanShuntFlow(VolumePerTimeUnit::L_Per_day);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, SystemicVascularResistance)
{
  EXPECT_EQ(false, cs->HasSystemicVascularResistance());
  cs->GetSystemicVascularResistance().SetValue(1.0,FlowResistanceUnit::cmH2O_s_Per_L);
  EXPECT_EQ(true, cs->HasSystemicVascularResistance());
  EXPECT_EQ(1.0, cs->GetSystemicVascularResistance(FlowResistanceUnit::cmH2O_s_Per_L));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasSystemicVascularResistance());
  auto dvalue = ccs.GetSystemicVascularResistance(FlowResistanceUnit::cmH2O_s_Per_L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, PulmonarySystolicArterialPressure)
{
  EXPECT_EQ(false, cs->HasPulmonarySystolicArterialPressure());
  cs->GetPulmonarySystolicArterialPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, cs->HasPulmonarySystolicArterialPressure());
  EXPECT_EQ(1.0, cs->GetPulmonarySystolicArterialPressure(PressureUnit::Pa));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasPulmonarySystolicArterialPressure());
  auto dvalue = ccs.GetPulmonarySystolicArterialPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, PulmonaryVascularResistance)
{
  EXPECT_EQ(false, cs->HasPulmonaryVascularResistance());
  cs->GetPulmonaryVascularResistance().SetValue(1.0, FlowResistanceUnit::cmH2O_s_Per_L);
  EXPECT_EQ(true, cs->HasPulmonaryVascularResistance());
  EXPECT_EQ(1.0, cs->GetPulmonaryVascularResistance(FlowResistanceUnit::cmH2O_s_Per_L));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasPulmonaryVascularResistance());
  auto dvalue = ccs.GetPulmonaryVascularResistance(FlowResistanceUnit::cmH2O_s_Per_L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, PulmonaryVascularResistanceIndex)
{
  EXPECT_EQ(false, cs->HasPulmonaryVascularResistanceIndex());
  cs->GetPulmonaryVascularResistanceIndex().SetValue(1.0, PressureTimePerVolumeAreaUnit::dyn_s_Per_cm5_m2);
  EXPECT_EQ(true, cs->HasPulmonaryVascularResistanceIndex());
  EXPECT_EQ(1.0, cs->GetPulmonaryVascularResistanceIndex(PressureTimePerVolumeAreaUnit::dyn_s_Per_cm5_m2));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasPulmonaryVascularResistanceIndex());
  auto dvalue = ccs.GetPulmonaryVascularResistanceIndex(PressureTimePerVolumeAreaUnit::dyn_s_Per_cm5_m2);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, PulsePressure)
{
  EXPECT_EQ(false, cs->HasPulsePressure());
  cs->GetPulsePressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, cs->HasPulsePressure());
  EXPECT_EQ(1.0, cs->GetPulsePressure(PressureUnit::Pa));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasPulsePressure());
  auto dvalue = ccs.GetPulsePressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, SystolicArterialPressure)
{
  EXPECT_EQ(false, cs->HasSystolicArterialPressure());
  cs->GetSystolicArterialPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, cs->HasSystolicArterialPressure());
  EXPECT_EQ(1.0, cs->GetSystolicArterialPressure(PressureUnit::Pa));

  const biogears::SECardiovascularSystem ccs(logger);
  EXPECT_EQ(false, ccs.HasSystolicArterialPressure());
  auto dvalue = ccs.GetSystolicArterialPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
