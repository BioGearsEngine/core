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
//! @date   2021/03/22
//!
//! Unit Test for BioGears Seralization
//!
#include <thread>

#include <gtest/gtest.h>

#include <biogears/cdm/utils/Logger.h>
#include <biogears/cdm/properties/SEProperties.h>

#include <io/cdm/EnvironmentConditions.h>
#include <io/cdm/Property.h>

#ifdef DISABLE_BIOGEARS_EnvironmentConditions_TEST
#define TEST_FIXTURE_NAME DISABLED_EnvironmentConditionsFixture
#else
#define TEST_FIXTURE_NAME IO_EnvironmentConditionsFixture
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

#include <biogears/cdm/system/environment/Conditions/SEEnvironmentCondition.h>

// class SEInitialEnvironment;
//!
//! TYPE InitialEnvironment
//! static void Marshall(const CDM::InitialEnvironment& in, SEInitialEnvironment& out);
//! static void UnMarshall(const SEInitialEnvironment& in, CDM::InitialEnvironment& out);
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/environment/Conditions/SEInitialEnvironment.h>
#include <biogears/cdm/system/environment/SEEnvironmentalConditions.h>
TEST_F(TEST_FIXTURE_NAME, InitialEnvironment)
{
  USING_TYPES(InitialEnvironment)

  biogears::Logger logger;
  biogears::SESubstanceManager subMgr { &logger };
  ASSERT_TRUE(subMgr.LoadSubstanceDirectory());
  SEType source { subMgr }, sink { subMgr };
  CDMType data;

  source.SetComment("Test Comment");
  source.SetConditionsFile("ConditionFile.xml");
  auto& conditions = source.GetConditions();

  auto nitrogen = subMgr.GetSubstance("Nitrogen");
  auto sarin = subMgr.GetSubstance("Sarin");
  ASSERT_NE(nullptr, sarin);
  ASSERT_NE(nullptr, nitrogen);
  conditions.SetSurroundingType(CDM::enumSurroundingType::Water);

  conditions.SetName("EnvironmentalConditions");

  conditions.GetEmissivity().SetValue(0.8);
  conditions.GetRelativeHumidity().SetValue(0.7);

  conditions.GetClothingResistance().SetValue(1.0, biogears::HeatResistanceAreaUnit::clo);

  conditions.GetAirVelocity().SetValue(1.0, biogears::LengthPerTimeUnit::cm_Per_min);

  conditions.GetAirDensity().SetValue(1.0, biogears::MassPerVolumeUnit::g_Per_L);

  conditions.GetAtmosphericPressure().SetValue(1.0, biogears::PressureUnit::cmH2O);

  conditions.GetAmbientTemperature().SetValue(1.0, biogears::TemperatureUnit::C);
  conditions.GetMeanRadiantTemperature().SetValue(1.0, biogears::TemperatureUnit::C);
  conditions.GetRespirationAmbientTemperature().SetValue(1.0, biogears::TemperatureUnit::C);

  conditions.AddAmbientGas(*nitrogen, biogears::SEScalarFraction { 0.05 });
  conditions.AddAmbientAerosol(*sarin, biogears::SEScalarMassPerVolume { 33, biogears::MassPerVolumeUnit::g_Per_L });

  EXPECT_NE(source, sink);

  EnvironmentConditions::Marshall(source, data);
  EnvironmentConditions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
