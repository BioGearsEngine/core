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
#include <biogears/cdm/system/physiology/SERenalSystem.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

#ifdef DISABLE_BIOGEARS_SERenalSystem_TEST
#define TEST_FIXTURE_NAME DISABLED_SERenalSystem_Fixture
#else
#define TEST_FIXTURE_NAME SERenalSystem_Fixture
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
  biogears::SERenalSystem* es;
};

void TEST_FIXTURE_NAME::SetUp()
{
  logger = new biogears::Logger;
  es = new biogears::SERenalSystem(logger);
}

void TEST_FIXTURE_NAME::TearDown()
{
  delete es;
  delete logger;
}
TEST_F(TEST_FIXTURE_NAME, GlomerularFiltrationRate)
{
  EXPECT_FALSE( es->HasGlomerularFiltrationRate());
  es->GetGlomerularFiltrationRate().SetValue(1.0, VolumePerTimeUnit::L_Per_min);
  EXPECT_TRUE( es->HasGlomerularFiltrationRate());
  EXPECT_EQ(1.0, es->GetGlomerularFiltrationRate(VolumePerTimeUnit::L_Per_min));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasGlomerularFiltrationRate());
  auto dvalue = ces.GetGlomerularFiltrationRate(VolumePerTimeUnit::L_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, FiltrationFraction)
{
  EXPECT_FALSE( es->HasFiltrationFraction());
  es->GetFiltrationFraction().SetValue(1.0);
  EXPECT_TRUE( es->HasFiltrationFraction());
  EXPECT_EQ(1.0, es->GetFiltrationFraction().GetValue());

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasFiltrationFraction());
  auto dvalue = ces.GetFiltrationFraction();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftAfferentArterioleResistance)
{
  EXPECT_FALSE( es->HasLeftAfferentArterioleResistance());
  es->GetLeftAfferentArterioleResistance().SetValue(1.0, FlowResistanceUnit::Pa_s_Per_m3);
  EXPECT_TRUE( es->HasLeftAfferentArterioleResistance());
  EXPECT_EQ(1.0, es->GetLeftAfferentArterioleResistance(FlowResistanceUnit::Pa_s_Per_m3));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasLeftAfferentArterioleResistance());
  auto dvalue = ces.GetLeftAfferentArterioleResistance(FlowResistanceUnit::Pa_s_Per_m3);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftBowmansCapsulesHydrostaticPressure)
{
  EXPECT_FALSE( es->HasLeftBowmansCapsulesHydrostaticPressure());
  es->GetLeftBowmansCapsulesHydrostaticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_TRUE( es->HasLeftBowmansCapsulesHydrostaticPressure());
  EXPECT_EQ(1.0, es->GetLeftBowmansCapsulesHydrostaticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasLeftBowmansCapsulesHydrostaticPressure());
  auto dvalue = ces.GetLeftBowmansCapsulesHydrostaticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftBowmansCapsulesOsmoticPressure)
{
  EXPECT_FALSE( es->HasLeftBowmansCapsulesOsmoticPressure());
  es->GetLeftBowmansCapsulesOsmoticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_TRUE( es->HasLeftBowmansCapsulesOsmoticPressure());
  EXPECT_EQ(1.0, es->GetLeftBowmansCapsulesOsmoticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasLeftBowmansCapsulesOsmoticPressure());
  auto dvalue = ces.GetLeftBowmansCapsulesOsmoticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftEfferentArterioleResistance)
{
  EXPECT_FALSE( es->HasLeftEfferentArterioleResistance());
  es->GetLeftEfferentArterioleResistance().SetValue(1.0, FlowResistanceUnit::Pa_s_Per_m3);
  EXPECT_TRUE( es->HasLeftEfferentArterioleResistance());
  EXPECT_EQ(1.0, es->GetLeftEfferentArterioleResistance(FlowResistanceUnit::Pa_s_Per_m3));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasLeftEfferentArterioleResistance());
  auto dvalue = ces.GetLeftEfferentArterioleResistance(FlowResistanceUnit::Pa_s_Per_m3);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftGlomerularCapillariesHydrostaticPressure)
{
  EXPECT_FALSE( es->HasLeftGlomerularCapillariesHydrostaticPressure());
  es->GetLeftGlomerularCapillariesHydrostaticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_TRUE( es->HasLeftGlomerularCapillariesHydrostaticPressure());
  EXPECT_EQ(1.0, es->GetLeftGlomerularCapillariesHydrostaticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasLeftGlomerularCapillariesHydrostaticPressure());
  auto dvalue = ces.GetLeftGlomerularCapillariesHydrostaticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftGlomerularCapillariesOsmoticPressure)
{
  EXPECT_FALSE( es->HasLeftGlomerularCapillariesOsmoticPressure());
  es->GetLeftGlomerularCapillariesOsmoticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_TRUE( es->HasLeftGlomerularCapillariesOsmoticPressure());
  EXPECT_EQ(1.0, es->GetLeftGlomerularCapillariesOsmoticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasLeftGlomerularCapillariesOsmoticPressure());
  auto dvalue = ces.GetLeftGlomerularCapillariesOsmoticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftGlomerularFiltrationCoefficient)
{
  EXPECT_FALSE( es->HasLeftGlomerularFiltrationCoefficient());
  es->GetLeftGlomerularFiltrationCoefficient().SetValue(1.0, VolumePerTimePressureUnit::L_Per_min_mmHg);
  EXPECT_TRUE( es->HasLeftGlomerularFiltrationCoefficient());
  EXPECT_EQ(1.0, es->GetLeftGlomerularFiltrationCoefficient(VolumePerTimePressureUnit::L_Per_min_mmHg));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasLeftGlomerularFiltrationCoefficient());
  auto dvalue = ces.GetLeftGlomerularFiltrationCoefficient(VolumePerTimePressureUnit::L_Per_min_mmHg);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftReabsorptionFiltrationCoefficient)
{
  EXPECT_FALSE( es->HasLeftReabsorptionFiltrationCoefficient());
  es->GetLeftReabsorptionFiltrationCoefficient().SetValue(1.0, VolumePerTimePressureUnit::L_Per_min_mmHg);
  EXPECT_TRUE( es->HasLeftReabsorptionFiltrationCoefficient());
  EXPECT_EQ(1.0, es->GetLeftReabsorptionFiltrationCoefficient(VolumePerTimePressureUnit::L_Per_min_mmHg));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasLeftReabsorptionFiltrationCoefficient());
  auto dvalue = ces.GetLeftReabsorptionFiltrationCoefficient(VolumePerTimePressureUnit::L_Per_min_mmHg);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightGlomerularFiltrationCoefficient)
{
  EXPECT_FALSE( es->HasRightGlomerularFiltrationCoefficient());
  es->GetRightGlomerularFiltrationCoefficient().SetValue(1.0, VolumePerTimePressureUnit::L_Per_min_mmHg);
  EXPECT_TRUE( es->HasRightGlomerularFiltrationCoefficient());
  EXPECT_EQ(1.0, es->GetRightGlomerularFiltrationCoefficient(VolumePerTimePressureUnit::L_Per_min_mmHg));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasRightGlomerularFiltrationCoefficient());
  auto dvalue = ces.GetRightGlomerularFiltrationCoefficient(VolumePerTimePressureUnit::L_Per_min_mmHg);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightReabsorptionFiltrationCoefficient)
{
  EXPECT_FALSE( es->HasRightReabsorptionFiltrationCoefficient());
  es->GetRightReabsorptionFiltrationCoefficient().SetValue(1.0, VolumePerTimePressureUnit::L_Per_min_mmHg);
  EXPECT_TRUE( es->HasRightReabsorptionFiltrationCoefficient());
  EXPECT_EQ(1.0, es->GetRightReabsorptionFiltrationCoefficient(VolumePerTimePressureUnit::L_Per_min_mmHg));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasRightReabsorptionFiltrationCoefficient());
  auto dvalue = ces.GetRightReabsorptionFiltrationCoefficient(VolumePerTimePressureUnit::L_Per_min_mmHg);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftGlomerularFiltrationSurfaceArea)
{
  EXPECT_FALSE( es->HasLeftGlomerularFiltrationSurfaceArea());
  es->GetLeftGlomerularFiltrationSurfaceArea().SetValue(1.0, AreaUnit::m2);
  EXPECT_TRUE( es->HasLeftGlomerularFiltrationSurfaceArea());
  EXPECT_EQ(1.0, es->GetLeftGlomerularFiltrationSurfaceArea(AreaUnit::m2));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasLeftGlomerularFiltrationSurfaceArea());
  auto dvalue = ces.GetLeftGlomerularFiltrationSurfaceArea(AreaUnit::m2);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftTubularReabsorptionFiltrationSurfaceArea)
{
  EXPECT_FALSE( es->HasLeftTubularReabsorptionFiltrationSurfaceArea());
  es->GetLeftTubularReabsorptionFiltrationSurfaceArea().SetValue(1.0, AreaUnit::m2);
  EXPECT_TRUE( es->HasLeftTubularReabsorptionFiltrationSurfaceArea());
  EXPECT_EQ(1.0, es->GetLeftTubularReabsorptionFiltrationSurfaceArea(AreaUnit::m2));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasLeftTubularReabsorptionFiltrationSurfaceArea());
  auto dvalue = ces.GetLeftTubularReabsorptionFiltrationSurfaceArea(AreaUnit::m2);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightGlomerularFiltrationSurfaceArea)
{
  EXPECT_FALSE( es->HasRightGlomerularFiltrationSurfaceArea());
  es->GetRightGlomerularFiltrationSurfaceArea().SetValue(1.0, AreaUnit::m2);
  EXPECT_TRUE( es->HasRightGlomerularFiltrationSurfaceArea());
  EXPECT_EQ(1.0, es->GetRightGlomerularFiltrationSurfaceArea(AreaUnit::m2));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasRightGlomerularFiltrationSurfaceArea());
  auto dvalue = ces.GetRightGlomerularFiltrationSurfaceArea(AreaUnit::m2);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightTubularReabsorptionFiltrationSurfaceArea)
{
  EXPECT_FALSE( es->HasRightTubularReabsorptionFiltrationSurfaceArea());
  es->GetRightTubularReabsorptionFiltrationSurfaceArea().SetValue(1.0, AreaUnit::m2);
  EXPECT_TRUE( es->HasRightTubularReabsorptionFiltrationSurfaceArea());
  EXPECT_EQ(1.0, es->GetRightTubularReabsorptionFiltrationSurfaceArea(AreaUnit::m2));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasRightTubularReabsorptionFiltrationSurfaceArea());
  auto dvalue = ces.GetRightTubularReabsorptionFiltrationSurfaceArea(AreaUnit::m2);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftGlomerularFluidPermeability)
{
  EXPECT_FALSE( es->HasLeftGlomerularFluidPermeability());
  es->GetLeftGlomerularFluidPermeability().SetValue(1.0, VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2);
  EXPECT_TRUE( es->HasLeftGlomerularFluidPermeability());
  EXPECT_EQ(1.0, es->GetLeftGlomerularFluidPermeability(VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasLeftGlomerularFluidPermeability());
  auto dvalue = ces.GetLeftGlomerularFluidPermeability(VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftTubularReabsorptionFluidPermeability)
{
  EXPECT_FALSE( es->HasLeftTubularReabsorptionFluidPermeability());
  es->GetLeftTubularReabsorptionFluidPermeability().SetValue(1.0, VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2);
  EXPECT_TRUE( es->HasLeftTubularReabsorptionFluidPermeability());
  EXPECT_EQ(1.0, es->GetLeftTubularReabsorptionFluidPermeability(VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasLeftTubularReabsorptionFluidPermeability());
  auto dvalue = ces.GetLeftTubularReabsorptionFluidPermeability(VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightGlomerularFluidPermeability)
{
  EXPECT_FALSE( es->HasRightGlomerularFluidPermeability());
  es->GetRightGlomerularFluidPermeability().SetValue(1.0, VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2);
  EXPECT_TRUE( es->HasRightGlomerularFluidPermeability());
  EXPECT_EQ(1.0, es->GetRightGlomerularFluidPermeability(VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasRightGlomerularFluidPermeability());
  auto dvalue = ces.GetRightGlomerularFluidPermeability(VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightTubularReabsorptionFluidPermeability)
{
  EXPECT_FALSE( es->HasRightTubularReabsorptionFluidPermeability());
  es->GetRightTubularReabsorptionFluidPermeability().SetValue(1.0, VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2);
  EXPECT_TRUE( es->HasRightTubularReabsorptionFluidPermeability());
  EXPECT_EQ(1.0, es->GetRightTubularReabsorptionFluidPermeability(VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasRightTubularReabsorptionFluidPermeability());
  auto dvalue = ces.GetRightTubularReabsorptionFluidPermeability(VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftNetFiltrationPressure)
{
  EXPECT_FALSE( es->HasLeftNetFiltrationPressure());
  es->GetLeftNetFiltrationPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_TRUE( es->HasLeftNetFiltrationPressure());
  EXPECT_EQ(1.0, es->GetLeftNetFiltrationPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasLeftNetFiltrationPressure());
  auto dvalue = ces.GetLeftNetFiltrationPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftNetReabsorptionPressure)
{
  EXPECT_FALSE( es->HasLeftNetReabsorptionPressure());
  es->GetLeftNetReabsorptionPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_TRUE( es->HasLeftNetReabsorptionPressure());
  EXPECT_EQ(1.0, es->GetLeftNetReabsorptionPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasLeftNetReabsorptionPressure());
  auto dvalue = ces.GetLeftNetReabsorptionPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftPeritubularCapillariesHydrostaticPressure)
{
  EXPECT_FALSE( es->HasLeftPeritubularCapillariesHydrostaticPressure());
  es->GetLeftPeritubularCapillariesHydrostaticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_TRUE( es->HasLeftPeritubularCapillariesHydrostaticPressure());
  EXPECT_EQ(1.0, es->GetLeftPeritubularCapillariesHydrostaticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasLeftPeritubularCapillariesHydrostaticPressure());
  auto dvalue = ces.GetLeftPeritubularCapillariesHydrostaticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftTubularHydrostaticPressure)
{
  EXPECT_FALSE( es->HasLeftTubularHydrostaticPressure());
  es->GetLeftTubularHydrostaticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_TRUE( es->HasLeftTubularHydrostaticPressure());
  EXPECT_EQ(1.0, es->GetLeftTubularHydrostaticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasLeftTubularHydrostaticPressure());
  auto dvalue = ces.GetLeftTubularHydrostaticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftTubularOsmoticPressure)
{
  EXPECT_FALSE( es->HasLeftTubularOsmoticPressure());
  es->GetLeftTubularOsmoticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_TRUE( es->HasLeftTubularOsmoticPressure());
  EXPECT_EQ(1.0, es->GetLeftTubularOsmoticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasLeftTubularOsmoticPressure());
  auto dvalue = ces.GetLeftTubularOsmoticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightBowmansCapsulesHydrostaticPressure)
{
  EXPECT_FALSE( es->HasRightBowmansCapsulesHydrostaticPressure());
  es->GetRightBowmansCapsulesHydrostaticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_TRUE( es->HasRightBowmansCapsulesHydrostaticPressure());
  EXPECT_EQ(1.0, es->GetRightBowmansCapsulesHydrostaticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasRightBowmansCapsulesHydrostaticPressure());
  auto dvalue = ces.GetRightBowmansCapsulesHydrostaticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightBowmansCapsulesOsmoticPressure)
{
  EXPECT_FALSE( es->HasRightBowmansCapsulesOsmoticPressure());
  es->GetRightBowmansCapsulesOsmoticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_TRUE( es->HasRightBowmansCapsulesOsmoticPressure());
  EXPECT_EQ(1.0, es->GetRightBowmansCapsulesOsmoticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasRightBowmansCapsulesOsmoticPressure());
  auto dvalue = ces.GetRightBowmansCapsulesOsmoticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightGlomerularCapillariesHydrostaticPressure)
{
  EXPECT_FALSE( es->HasRightGlomerularCapillariesHydrostaticPressure());
  es->GetRightGlomerularCapillariesHydrostaticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_TRUE( es->HasRightGlomerularCapillariesHydrostaticPressure());
  EXPECT_EQ(1.0, es->GetRightGlomerularCapillariesHydrostaticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasRightGlomerularCapillariesHydrostaticPressure());
  auto dvalue = ces.GetRightGlomerularCapillariesHydrostaticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightGlomerularCapillariesOsmoticPressure)
{
  EXPECT_FALSE( es->HasRightGlomerularCapillariesOsmoticPressure());
  es->GetRightGlomerularCapillariesOsmoticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_TRUE( es->HasRightGlomerularCapillariesOsmoticPressure());
  EXPECT_EQ(1.0, es->GetRightGlomerularCapillariesOsmoticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasRightGlomerularCapillariesOsmoticPressure());
  auto dvalue = ces.GetRightGlomerularCapillariesOsmoticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightNetFiltrationPressure)
{
  EXPECT_FALSE( es->HasRightNetFiltrationPressure());
  es->GetRightNetFiltrationPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_TRUE( es->HasRightNetFiltrationPressure());
  EXPECT_EQ(1.0, es->GetRightNetFiltrationPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasRightNetFiltrationPressure());
  auto dvalue = ces.GetRightNetFiltrationPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightNetReabsorptionPressure)
{
  EXPECT_FALSE( es->HasRightNetReabsorptionPressure());
  es->GetRightNetReabsorptionPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_TRUE( es->HasRightNetReabsorptionPressure());
  EXPECT_EQ(1.0, es->GetRightNetReabsorptionPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasRightNetReabsorptionPressure());
  auto dvalue = ces.GetRightNetReabsorptionPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightPeritubularCapillariesHydrostaticPressure)
{
  EXPECT_FALSE( es->HasRightPeritubularCapillariesHydrostaticPressure());
  es->GetRightPeritubularCapillariesHydrostaticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_TRUE( es->HasRightPeritubularCapillariesHydrostaticPressure());
  EXPECT_EQ(1.0, es->GetRightPeritubularCapillariesHydrostaticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasRightPeritubularCapillariesHydrostaticPressure());
  auto dvalue = ces.GetRightPeritubularCapillariesHydrostaticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightPeritubularCapillariesOsmoticPressure)
{
  EXPECT_FALSE( es->HasRightPeritubularCapillariesOsmoticPressure());
  es->GetRightPeritubularCapillariesOsmoticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_TRUE( es->HasRightPeritubularCapillariesOsmoticPressure());
  EXPECT_EQ(1.0, es->GetRightPeritubularCapillariesOsmoticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasRightPeritubularCapillariesOsmoticPressure());
  auto dvalue = ces.GetRightPeritubularCapillariesOsmoticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightTubularHydrostaticPressure)
{
  EXPECT_FALSE( es->HasRightTubularHydrostaticPressure());
  es->GetRightTubularHydrostaticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_TRUE( es->HasRightTubularHydrostaticPressure());
  EXPECT_EQ(1.0, es->GetRightTubularHydrostaticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasRightTubularHydrostaticPressure());
  auto dvalue = ces.GetRightTubularHydrostaticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightTubularOsmoticPressure)
{
  EXPECT_FALSE( es->HasRightTubularOsmoticPressure());
  es->GetRightTubularOsmoticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_TRUE( es->HasRightTubularOsmoticPressure());
  EXPECT_EQ(1.0, es->GetRightTubularOsmoticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasRightTubularOsmoticPressure());
  auto dvalue = ces.GetRightTubularOsmoticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftReabsorptionRate)
{
  EXPECT_FALSE( es->HasLeftReabsorptionRate());
  es->GetLeftReabsorptionRate().SetValue(1.0, VolumePerTimeUnit::L_Per_min);
  EXPECT_TRUE( es->HasLeftReabsorptionRate());
  EXPECT_EQ(1.0, es->GetLeftReabsorptionRate(VolumePerTimeUnit::L_Per_min));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasLeftReabsorptionRate());
  auto dvalue = ces.GetLeftReabsorptionRate(VolumePerTimeUnit::L_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RenalBloodFlow)
{
  EXPECT_FALSE( es->HasRenalBloodFlow());
  es->GetRenalBloodFlow().SetValue(1.0, VolumePerTimeUnit::L_Per_min);
  EXPECT_TRUE( es->HasRenalBloodFlow());
  EXPECT_EQ(1.0, es->GetRenalBloodFlow(VolumePerTimeUnit::L_Per_min));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasRenalBloodFlow());
  auto dvalue = ces.GetRenalBloodFlow(VolumePerTimeUnit::L_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RenalPlasmaFlow)
{
  EXPECT_FALSE( es->HasRenalPlasmaFlow());
  es->GetRenalPlasmaFlow().SetValue(1.0, VolumePerTimeUnit::L_Per_min);
  EXPECT_TRUE( es->HasRenalPlasmaFlow());
  EXPECT_EQ(1.0, es->GetRenalPlasmaFlow(VolumePerTimeUnit::L_Per_min));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasRenalPlasmaFlow());
  auto dvalue = ces.GetRenalPlasmaFlow(VolumePerTimeUnit::L_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightGlomerularFiltrationRate)
{
  EXPECT_FALSE( es->HasRightGlomerularFiltrationRate());
  es->GetRightGlomerularFiltrationRate().SetValue(1.0, VolumePerTimeUnit::L_Per_min);
  EXPECT_TRUE( es->HasRightGlomerularFiltrationRate());
  EXPECT_EQ(1.0, es->GetRightGlomerularFiltrationRate(VolumePerTimeUnit::L_Per_min));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasRightGlomerularFiltrationRate());
  auto dvalue = ces.GetRightGlomerularFiltrationRate(VolumePerTimeUnit::L_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightReabsorptionRate)
{
  EXPECT_FALSE( es->HasRightReabsorptionRate());
  es->GetRightReabsorptionRate().SetValue(1.0, VolumePerTimeUnit::L_Per_min);
  EXPECT_TRUE( es->HasRightReabsorptionRate());
  EXPECT_EQ(1.0, es->GetRightReabsorptionRate(VolumePerTimeUnit::L_Per_min));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasRightReabsorptionRate());
  auto dvalue = ces.GetRightReabsorptionRate(VolumePerTimeUnit::L_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, UrinationRate)
{
  EXPECT_FALSE( es->HasUrinationRate());
  es->GetUrinationRate().SetValue(1.0, VolumePerTimeUnit::L_Per_min);
  EXPECT_TRUE( es->HasUrinationRate());
  EXPECT_EQ(1.0, es->GetUrinationRate(VolumePerTimeUnit::L_Per_min));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasUrinationRate());
  auto dvalue = ces.GetUrinationRate(VolumePerTimeUnit::L_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, UrineProductionRate)
{
  EXPECT_FALSE( es->HasUrineProductionRate());
  es->GetUrineProductionRate().SetValue(1.0, VolumePerTimeUnit::L_Per_min);
  EXPECT_TRUE( es->HasUrineProductionRate());
  EXPECT_EQ(1.0, es->GetUrineProductionRate(VolumePerTimeUnit::L_Per_min));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasUrineProductionRate());
  auto dvalue = ces.GetUrineProductionRate(VolumePerTimeUnit::L_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RenalVascularResistance)
{
  EXPECT_FALSE( es->HasRenalVascularResistance());
  es->GetRenalVascularResistance().SetValue(1.0, FlowResistanceUnit::Pa_s_Per_m3);
  EXPECT_TRUE( es->HasRenalVascularResistance());
  EXPECT_EQ(1.0, es->GetRenalVascularResistance(FlowResistanceUnit::Pa_s_Per_m3));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasRenalVascularResistance());
  auto dvalue = ces.GetRenalVascularResistance(FlowResistanceUnit::Pa_s_Per_m3);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightAfferentArterioleResistance)
{
  EXPECT_FALSE( es->HasRightAfferentArterioleResistance());
  es->GetRightAfferentArterioleResistance().SetValue(1.0, FlowResistanceUnit::Pa_s_Per_m3);
  EXPECT_TRUE( es->HasRightAfferentArterioleResistance());
  EXPECT_EQ(1.0, es->GetRightAfferentArterioleResistance(FlowResistanceUnit::Pa_s_Per_m3));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasRightAfferentArterioleResistance());
  auto dvalue = ces.GetRightAfferentArterioleResistance(FlowResistanceUnit::Pa_s_Per_m3);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightEfferentArterioleResistance)
{
  EXPECT_FALSE( es->HasRightEfferentArterioleResistance());
  es->GetRightEfferentArterioleResistance().SetValue(1.0, FlowResistanceUnit::Pa_s_Per_m3);
  EXPECT_TRUE( es->HasRightEfferentArterioleResistance());
  EXPECT_EQ(1.0, es->GetRightEfferentArterioleResistance(FlowResistanceUnit::Pa_s_Per_m3));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasRightEfferentArterioleResistance());
  auto dvalue = ces.GetRightEfferentArterioleResistance(FlowResistanceUnit::Pa_s_Per_m3);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, UrineOsmolality)
{
  EXPECT_FALSE( es->HasUrineOsmolality());
  es->GetUrineOsmolality().SetValue(1.0, OsmolalityUnit::Osm_Per_kg);
  EXPECT_TRUE( es->HasUrineOsmolality());
  EXPECT_EQ(1.0, es->GetUrineOsmolality(OsmolalityUnit::Osm_Per_kg));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasUrineOsmolality());
  auto dvalue = ces.GetUrineOsmolality(OsmolalityUnit::Osm_Per_kg);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, UrineVolume)
{
  EXPECT_FALSE( es->HasUrineVolume());
  es->GetUrineVolume().SetValue(1.0, VolumeUnit::L);
  EXPECT_TRUE( es->HasUrineVolume());
  EXPECT_EQ(1.0, es->GetUrineVolume(VolumeUnit::L));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasUrineVolume());
  auto dvalue = ces.GetUrineVolume(VolumeUnit::L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, UrineUreaNitrogenConcentration)
{
  EXPECT_FALSE( es->HasUrineUreaNitrogenConcentration());
  es->GetUrineUreaNitrogenConcentration().SetValue(1.0, MassPerVolumeUnit::kg_Per_L);
  EXPECT_TRUE( es->HasUrineUreaNitrogenConcentration());
  EXPECT_EQ(1.0, es->GetUrineUreaNitrogenConcentration(MassPerVolumeUnit::kg_Per_L));

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasUrineUreaNitrogenConcentration());
  auto dvalue = ces.GetUrineUreaNitrogenConcentration(MassPerVolumeUnit::kg_Per_L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftFiltrationFraction)
{
  EXPECT_FALSE( es->HasLeftFiltrationFraction());
  es->GetLeftFiltrationFraction().SetValue(1.0);
  EXPECT_TRUE( es->HasLeftFiltrationFraction());
  EXPECT_EQ(1.0, es->GetLeftFiltrationFraction().GetValue());

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasLeftFiltrationFraction());
  auto dvalue = ces.GetLeftFiltrationFraction();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightFiltrationFraction)
{
  EXPECT_FALSE( es->HasRightFiltrationFraction());
  es->GetRightFiltrationFraction().SetValue(1.0);
  EXPECT_TRUE( es->HasRightFiltrationFraction());
  EXPECT_EQ(1.0, es->GetRightFiltrationFraction().GetValue());

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasRightFiltrationFraction());
  auto dvalue = ces.GetRightFiltrationFraction();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, UrineSpecificGravity)
{
  EXPECT_FALSE( es->HasUrineSpecificGravity());
  es->GetUrineSpecificGravity().SetValue(1.0);
  EXPECT_TRUE( es->HasUrineSpecificGravity());
  EXPECT_EQ(1.0, es->GetUrineSpecificGravity().GetValue());

  const biogears::SERenalSystem ces(logger);
  EXPECT_FALSE( ces.HasUrineSpecificGravity());
  auto dvalue = ces.GetUrineSpecificGravity();
  EXPECT_NE(dvalue, dvalue);
}
//GlomerularFiltrationRate - VolumePerTime
//FiltartionFraction - 
//LeftAfferentArterioleResistance - FlowResistanceUnit
//LeftBowmansCapsulesHydrostaticPressure - PressureUnit
//LeftBowmansCapsulesOsmoticPressure - PressureUnit
//LeftEfferentArterioleResistance - FlowResistanceUnit
//LeftGlomerularCapillariesHydrostaticPressure - PressureUnit
//LeftGlomerularCapillariesOsmoticPressure - PressureUnit
//LeftGlomerularFiltrationCoefficient - VolumePerTimePressureUnit
//LeftGlomerularFiltrationSurfaceArea - AreaUnit
//LeftGlomerularFluidPermeability - VolumePerTimePressureAreaUnit
//LeftFiltrationFraction - 
//LeftNetFiltrationPressure - PressureUnit
//LeftNetReabsorptionPressure - PressureUnit
//LeftPeritubularCapillariesHydrostaticPressure - PressureUnit
//LeftReabsorptionFiltrationCoefficient - VolumePerTimePressureUnit
//LeftReabsorptionRate - VolumePerTimeUnit
//LeftTubularReabsorptionFiltrationSurfaceArea - AreaUnit
//LeftTubularReabsorptionFluidPermeability - VolumePerTimePressureAreaUnit
//LeftTubularHydrostaticPressure - PressureUnit
//LeftTubularOsmoticPressure - PressureUnit
//RenalBloodFlow - VolumePerTimeUnit
//RenalPlasmaFlow - VolumePerTimeUnit
//RenalVascularResistance - FlowResistanceUnit
//RightAfferentArterioleResistance - FlowResistanceUnit
//RightBowmansCapsulesHydrostaticPressure - PressureUnit
//RightBowmansCapsulesOsmoticPressure - PressureUnit
//RightEfferentArterioleResistance - FlowResistanceUnit
//RightGlomerularCapillariesHydrostaticPressure - PressureUnit
//RightGlomerularCapillariesOsmoticPressure - PressureUnit
//RightGlomerularFiltrationCoefficient - VolumePerTimePressureUnit
//RightGlomerularFiltrationRate - VolumePerTimeUnit
//RightGlomerularFiltrationSurfaceArea - AreaUnit
//RightGlomerularFluidPermeability - VolumePerTimePressureAreaUnit
//RightFiltrationFraction 
//RightNetFiltrationPressure - PressureUnit
//RightNetReabsorptionPressure - PressureUnit
//RightPeritubularCapillariesHydrostaticPressure - PressureUnit
//RightPeritubularCapillariesOsmoticPressure - PressureUnit
//RightReabsorptionFiltrationCoefficient - VolumePerTimePressureUnit
//RightReabsorptionRate - VolumePerTimeUnit
//RightTubularReabsorptionFiltrationSurfaceArea - AreaUnit
//RightTubularReabsorptionFluidPermeability - VolumePerTimePressreAreaUnit
//RightTubularHydrostaticPressure - PressureUnit
//RightTubularOsmoticPressure - PressureUnit
//UrinationRate - VolumePerTimeUnit
//UrineOsmolality - OsmolalityUnit
//UrineProductionRate - VolumePerTimeUnit
//UrineSpecificGravity - 
//UrineVolume - VolumeUnit
//UrineUreaNitrogenConcentration - MassPerVolumeUnit