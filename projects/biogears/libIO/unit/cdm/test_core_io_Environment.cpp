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

#include <biogears/cdm/properties/SEProperties.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/utils/Logger.h>

#include <io/cdm/Environment.h>
#include <io/cdm/Property.h>

#ifdef DISABLE_BIOGEARS_Environments_TEST
#define TEST_FIXTURE_NAME DISABLED_EnvironmentsFixture
#else
#define TEST_FIXTURE_NAME IO_EnvironmentsFixture
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

#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/environment/SEEnvironment.h>

class EnvironmentUnitTest : public biogears::SEEnvironment {
public:
  EnvironmentUnitTest(biogears::SESubstanceManager& subMgr)
    : SEEnvironment(subMgr)
  {
  }

  /** @name StateChange
   *   @brief - This method is called when ever there is a state change
   *            Specically a new file has been loaded, configuration action, or the system reset
   *            Engine specific methodology can then update their logic.
   */
  void StateChange() override
  {
  }

  using SEEnvironment::Clear;
  using SEEnvironment::Load;
  using SEEnvironment::ProcessChange;
  using SEEnvironment::Unload;

  using SEEnvironment::GetConditions;
  using SEEnvironment::GetConvectiveHeatLoss;
  using SEEnvironment::GetConvectiveHeatTranferCoefficient;
  using SEEnvironment::GetEvaporativeHeatLoss;
  using SEEnvironment::GetEvaporativeHeatTranferCoefficient;
  using SEEnvironment::GetRadiativeHeatLoss;
  using SEEnvironment::GetRadiativeHeatTranferCoefficient;
  using SEEnvironment::GetRespirationHeatLoss;
  using SEEnvironment::GetSkinHeatLoss;
  using SEEnvironment::SetName;
};

// class SEEnvironmentalConditions;
//!
//! TYPE ThermalApplication
//! static void Marshall(const CDM::EnvironmentalConditions& in, SEEnvironmentalConditions& out);
//! static void UnMarshall(const SEEnvironmentalConditions& in, CDM::EnvironmentalConditions& out);
#include <biogears/cdm/system/environment/SEEnvironmentalConditions.h>
TEST_F(TEST_FIXTURE_NAME, EnvironmentalConditions)
{
  USING_TYPES(EnvironmentalConditions)

  biogears::Logger logger;
  biogears::SESubstanceManager subMgr { &logger };
  ASSERT_TRUE(subMgr.LoadSubstanceDirectory());
  SEType source { subMgr }, sink { subMgr };
  CDMType data;

  auto nitrogen = subMgr.GetSubstance("Nitrogen");
  auto sarin = subMgr.GetSubstance("Sarin");
  ASSERT_NE(nullptr, nitrogen);
  ASSERT_NE(nullptr, sarin);
  source.SetSurroundingType(CDM::enumSurroundingType::Water);

  source.SetName("EnvironmentalConditions");

  source.GetEmissivity().SetValue(0.8);
  source.GetRelativeHumidity().SetValue(0.7);

  source.GetClothingResistance().SetValue(1.0, biogears::HeatResistanceAreaUnit::clo);

  source.GetAirVelocity().SetValue(1.0, biogears::LengthPerTimeUnit::cm_Per_min);

  source.GetAirDensity().SetValue(1.0, biogears::MassPerVolumeUnit::g_Per_L);

  source.GetAtmosphericPressure().SetValue(1.0, biogears::PressureUnit::cmH2O);

  source.GetAmbientTemperature().SetValue(1.0, biogears::TemperatureUnit::C);
  source.GetMeanRadiantTemperature().SetValue(1.0, biogears::TemperatureUnit::C);
  source.GetRespirationAmbientTemperature().SetValue(1.0, biogears::TemperatureUnit::C);

  source.AddAmbientGas(*nitrogen, biogears::SEScalarFraction { 0.05 });
  source.AddAmbientAerosol(*sarin, biogears::SEScalarMassPerVolume { 33, biogears::MassPerVolumeUnit::g_Per_L });
  EXPECT_NE(source, sink);

  Environment::UnMarshall(source, data);
  Environment::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEActiveHeating;
//!
//! TYPE ActiveHeating
//! static void Marshall(const CDM::ActiveHeating& in, SEActiveHeating& out);
//! static void UnMarshall(const SEActiveHeating& in, CDM::ActiveHeating& out);
#include <biogears/cdm/system/environment/SEActiveHeating.h>
TEST_F(TEST_FIXTURE_NAME, ActiveHeating)
{
  USING_TYPES(ActiveHeating)

  biogears::Logger logger;
  biogears::SESubstanceManager subMgr { &logger };
  ASSERT_TRUE(subMgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  source.GetPower().SetValue(1.0, biogears::PowerUnit::W);
  source.GetSurfaceArea().SetValue(1.0, biogears::AreaUnit::m2);
  source.GetSurfaceAreaFraction().SetValue(1.0);

  EXPECT_NE(source, sink);

  Environment::UnMarshall(source, data);
  Environment::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEActiveCooling;
//!
//! TYPE ActiveCooling
//! static void Marshall(const CDM::ActiveCooling& in, SEActiveCooling& out);
//! static void UnMarshall(const SEActiveCooling& in, CDM::ActiveCooling& out);
#include <biogears/cdm/system/environment/SEActiveCooling.h>
TEST_F(TEST_FIXTURE_NAME, ActiveCooling)
{
  USING_TYPES(ActiveCooling)

  biogears::Logger logger;
  biogears::SESubstanceManager subMgr { &logger };
  ASSERT_TRUE(subMgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  source.GetPower().SetValue(1.0, biogears::PowerUnit::W);
  source.GetSurfaceArea().SetValue(1.0, biogears::AreaUnit::m2);
  source.GetSurfaceAreaFraction().SetValue(1.0);

  EXPECT_NE(source, sink);

  Environment::UnMarshall(source, data);
  Environment::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEAppliedTemperature;
//!
//! TYPE AppliedTemperature
//! static void Marshall(const CDM::AppliedTemperature& in, SEAppliedTemperature& out);
//! static void UnMarshall(const SEAppliedTemperature& in, CDM::AppliedTemperature& out);
#include <biogears/cdm/system/environment/SEAppliedTemperature.h>
TEST_F(TEST_FIXTURE_NAME, AppliedTemperature)
{
  USING_TYPES(AppliedTemperature)

  biogears::Logger logger;
  biogears::SESubstanceManager subMgr { &logger };
  ASSERT_TRUE(subMgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  source.GetTemperature().SetValue(1.0, biogears::TemperatureUnit::C);
  source.GetSurfaceArea().SetValue(1.0, biogears::AreaUnit::m2);
  source.GetSurfaceAreaFraction().SetValue(1.0);
  source.SetState(CDM::enumOnOff::On);

  EXPECT_NE(source, sink);

  Environment::UnMarshall(source, data);
  Environment::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEEnvironment;
//!
//! TYPE Environment
//! static void Marshall(const CDM::Environment& in, SEEnvironment& out);
//! static void UnMarshall(const SEEnvironment& in, CDM::Environment& out);
#include <biogears/cdm/system/environment/SEEnvironment.h>
TEST_F(TEST_FIXTURE_NAME, Environment)
{

  using namespace biogears::io;
  using SEType = EnvironmentUnitTest;
  using CDMType = CDM::EnvironmentData;

  biogears::Logger logger;
  biogears::SESubstanceManager subMgr { &logger };
  ASSERT_TRUE(subMgr.LoadSubstanceDirectory());
  SEType source { subMgr }, sink { subMgr };
  CDMType data;

  auto& conditions = source.GetConditions();
  source.SetName("Environment");

  auto nitrogen = subMgr.GetSubstance("Nitrogen");
  auto sarin = subMgr.GetSubstance("Sarin");
  ASSERT_NE(nullptr, nitrogen);
  ASSERT_NE(nullptr, sarin);
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

  source.GetConvectiveHeatLoss().SetValue(5.5, biogears::PowerUnit::W);
  source.GetEvaporativeHeatLoss().SetValue(5.5, biogears::PowerUnit::W);
  source.GetRadiativeHeatLoss().SetValue(5.5, biogears::PowerUnit::W);
  source.GetSkinHeatLoss().SetValue(5.5, biogears::PowerUnit::W);
  source.GetRespirationHeatLoss().SetValue(5.5, biogears::PowerUnit::W);

  source.GetConvectiveHeatTranferCoefficient().SetValue(5.5, biogears::HeatConductancePerAreaUnit::W_Per_m2_C);
  source.GetEvaporativeHeatTranferCoefficient().SetValue(5.5, biogears::HeatConductancePerAreaUnit::W_Per_m2_C);
  source.GetRadiativeHeatTranferCoefficient().SetValue(5.5, biogears::HeatConductancePerAreaUnit::W_Per_m2_C);

  EXPECT_NE(source, sink);

  Environment::UnMarshall(source, data);
  Environment::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}