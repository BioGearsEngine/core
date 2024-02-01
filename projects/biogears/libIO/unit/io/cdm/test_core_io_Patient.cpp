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
//! @author
//! @date   2023/08/14
//!
//! Unit Test for BioGears Seralization
//!
#include <thread>

#include <gtest/gtest.h>

#include <biogears/cdm/properties/SEProperties.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/utils/Logger.h>

#include <io/cdm/Patient.h>
#include <io/cdm/Property.h>

#ifdef DISABLE_BIOGEARS_Patient_TEST
#define TEST_FIXTURE_NAME DISABLED_PatientFixture
#else
#define TEST_FIXTURE_NAME IO_PatientFixture
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
  virtual void SetUp();

  // Code here will be called immediately after each test (right
  // before the destructor).
  virtual void TearDown();
};

void TEST_FIXTURE_NAME::SetUp()
{
}

void TEST_FIXTURE_NAME::TearDown()
{
}

#define USING_TYPES(name)             \
  using namespace biogears::io;       \
  using SEType = biogears ::SE##name; \
  using CDMType = CDM ::name##Data;

#include <biogears/cdm/patient/SEPatient.h>

// class SEPatient;
//!
//! TYPE Patient
//! static void Marshall(const CDM::Patient& in, SEPatient& out);
//! static void UnMarshall(const SEPatient& in, CDM::Patient& out);
TEST_F(TEST_FIXTURE_NAME, Patient)
{
  USING_TYPES(Patient)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source(&logger), sink(&logger);
  CDMType data;

  auto oxygen = mgr.GetSubstance("Oxygen");
  auto nitrogen = mgr.GetSubstance("Nitrogen");

  source.SetName("unit_test");
  source.SetAnnotation("Seerailization Test User");
  source.SetGender(CDM::enumSex::Female);
  source.SetSex(CDM::enumSex::Female);
  source.GetAge().SetValue(55.0, biogears::TimeUnit::yr);
  source.GetWeight().SetValue(80.0, biogears::MassUnit::kg);
  source.GetHeight().SetValue(175.0, biogears::LengthUnit::cm);
  source.GetAlveoliSurfaceArea().SetValue(16.0, biogears::AreaUnit::cm2);
  source.GetBasalMetabolicRate().SetValue(15.0, biogears::PowerUnit::J_Per_s);
  source.SetBloodType(CDM::enumBloodType::AB);
  source.SetBloodRh(true);
  source.GetBloodVolumeBaseline().SetValue(14.0, biogears::VolumeUnit::L);
  source.GetBodyDensity().SetValue(13.0, biogears::MassPerVolumeUnit::g_Per_L);
  source.GetBodyFatFraction().SetValue(12.0);
  source.GetDiastolicArterialPressureBaseline().SetValue(11.0, biogears::PressureUnit::cmH2O);
  source.GetExpiratoryReserveVolume().SetValue(10.0, biogears::VolumeUnit::mL);
  source.GetFunctionalResidualCapacity().SetValue(9.0, biogears::VolumeUnit::mL);
  source.GetHeartRateBaseline().SetValue(2.0, biogears::FrequencyUnit::Hz);
  source.GetHeartRateMaximum().SetValue(1.0, biogears::FrequencyUnit::Hz);
  source.GetHeartRateMinimum().SetValue(3.0, biogears::FrequencyUnit::Hz);
  source.GetHyperhidrosis().SetValue(-0.3);
  source.GetInspiratoryCapacity().SetValue(4.0, biogears::VolumeUnit::L);
  source.GetInspiratoryReserveVolume().SetValue(5.0, biogears::VolumeUnit::L);
  source.GetLeanBodyMass().SetValue(6.0, biogears::MassUnit::g);
  source.GetMaxWorkRate().SetValue(7.0, biogears::PowerUnit::W);
  source.GetMuscleMass().SetValue(8.0, biogears::MassUnit::g);
  source.GetMeanArterialPressureBaseline().SetValue(3.0, biogears::PressureUnit::mmHg);
  source.GetPainSusceptibility().SetValue(0.5);
  source.GetResidualVolume().SetValue(9.0, biogears::VolumeUnit::L);
  source.GetRespirationRateBaseline().SetValue(3.0, biogears::FrequencyUnit::Hz);
  source.GetRespiratoryDriverAmplitudeBaseline().SetValue(3.0, biogears::PressureUnit::mmHg);
  source.GetRightLungRatio().SetValue(3.20);
  source.GetSkinSurfaceArea().SetValue(3.10, biogears::AreaUnit::cm2);
  source.GetSleepAmount().SetValue(300.0, biogears::TimeUnit::min);
  source.GetSystolicArterialPressureBaseline().SetValue(34.0, biogears::PressureUnit::mmHg);
  source.GetTotalVentilationBaseline().SetValue(33.0, biogears::VolumePerTimeUnit::mL_Per_hr);
  source.GetTidalVolumeBaseline().SetValue(32.0, biogears::VolumeUnit::L);
  source.GetTotalLungCapacity().SetValue(31.0, biogears::VolumeUnit::L);
  source.GetVitalCapacity().SetValue(30.0, biogears::VolumeUnit::L);

  EXPECT_NE(source, sink);

  Patient::Marshall(source, data);
  Patient::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
