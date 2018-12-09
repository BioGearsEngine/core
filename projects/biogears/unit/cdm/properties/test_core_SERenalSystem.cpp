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
  virtual void SetUp();

  // Code here will be called immediately after each test (right
  // before the destructor).
  virtual void TearDown();

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
  EXPECT_EQ(false, es->HasGlomerularFiltrationRate());
  es->GetGlomerularFiltrationRate().SetValue(1.0, VolumePerTimeUnit::L_Per_min);
  EXPECT_EQ(true, es->HasGlomerularFiltrationRate());
  EXPECT_EQ(1.0, es->GetGlomerularFiltrationRate(VolumePerTimeUnit::L_Per_min));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasGlomerularFiltrationRate());
  auto dvalue = ces.GetGlomerularFiltrationRate(VolumePerTimeUnit::L_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, FiltrationFraction)
{
  EXPECT_EQ(false, es->HasFiltrationFraction());
  es->GetFiltrationFraction().SetValue(1.0);
  EXPECT_EQ(true, es->HasFiltrationFraction());
  EXPECT_EQ(1.0, es->GetFiltrationFraction().GetValue());

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasFiltrationFraction());
  auto dvalue = ces.GetFiltrationFraction();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftAfferentArterioleResistance)
{
  EXPECT_EQ(false, es->HasLeftAfferentArterioleResistance());
  es->GetLeftAfferentArterioleResistance().SetValue(1.0, FlowResistanceUnit::Pa_s_Per_m3);
  EXPECT_EQ(true, es->HasLeftAfferentArterioleResistance());
  EXPECT_EQ(1.0, es->GetLeftAfferentArterioleResistance(FlowResistanceUnit::Pa_s_Per_m3));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasLeftAfferentArterioleResistance());
  auto dvalue = ces.GetLeftAfferentArterioleResistance(FlowResistanceUnit::Pa_s_Per_m3);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftBowmansCapsulesHydrostaticPressure)
{
  EXPECT_EQ(false, es->HasLeftBowmansCapsulesHydrostaticPressure());
  es->GetLeftBowmansCapsulesHydrostaticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, es->HasLeftBowmansCapsulesHydrostaticPressure());
  EXPECT_EQ(1.0, es->GetLeftBowmansCapsulesHydrostaticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasLeftBowmansCapsulesHydrostaticPressure());
  auto dvalue = ces.GetLeftBowmansCapsulesHydrostaticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftBowmansCapsulesOsmoticPressure)
{
  EXPECT_EQ(false, es->HasLeftBowmansCapsulesOsmoticPressure());
  es->GetLeftBowmansCapsulesOsmoticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, es->HasLeftBowmansCapsulesOsmoticPressure());
  EXPECT_EQ(1.0, es->GetLeftBowmansCapsulesOsmoticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasLeftBowmansCapsulesOsmoticPressure());
  auto dvalue = ces.GetLeftBowmansCapsulesOsmoticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftEfferentArterioleResistance)
{
  EXPECT_EQ(false, es->HasLeftEfferentArterioleResistance());
  es->GetLeftEfferentArterioleResistance().SetValue(1.0, FlowResistanceUnit::Pa_s_Per_m3);
  EXPECT_EQ(true, es->HasLeftEfferentArterioleResistance());
  EXPECT_EQ(1.0, es->GetLeftEfferentArterioleResistance(FlowResistanceUnit::Pa_s_Per_m3));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasLeftEfferentArterioleResistance());
  auto dvalue = ces.GetLeftEfferentArterioleResistance(FlowResistanceUnit::Pa_s_Per_m3);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftGlomerularCapillariesHydrostaticPressure)
{
  EXPECT_EQ(false, es->HasLeftGlomerularCapillariesHydrostaticPressure());
  es->GetLeftGlomerularCapillariesHydrostaticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, es->HasLeftGlomerularCapillariesHydrostaticPressure());
  EXPECT_EQ(1.0, es->GetLeftGlomerularCapillariesHydrostaticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasLeftGlomerularCapillariesHydrostaticPressure());
  auto dvalue = ces.GetLeftGlomerularCapillariesHydrostaticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftGlomerularCapillariesOsmoticPressure)
{
  EXPECT_EQ(false, es->HasLeftGlomerularCapillariesOsmoticPressure());
  es->GetLeftGlomerularCapillariesOsmoticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, es->HasLeftGlomerularCapillariesOsmoticPressure());
  EXPECT_EQ(1.0, es->GetLeftGlomerularCapillariesOsmoticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasLeftGlomerularCapillariesOsmoticPressure());
  auto dvalue = ces.GetLeftGlomerularCapillariesOsmoticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftGlomerularFiltrationCoefficient)
{
  EXPECT_EQ(false, es->HasLeftGlomerularFiltrationCoefficient());
  es->GetLeftGlomerularFiltrationCoefficient().SetValue(1.0, VolumePerTimePressureUnit::L_Per_min_mmHg);
  EXPECT_EQ(true, es->HasLeftGlomerularFiltrationCoefficient());
  EXPECT_EQ(1.0, es->GetLeftGlomerularFiltrationCoefficient(VolumePerTimePressureUnit::L_Per_min_mmHg));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasLeftGlomerularFiltrationCoefficient());
  auto dvalue = ces.GetLeftGlomerularFiltrationCoefficient(VolumePerTimePressureUnit::L_Per_min_mmHg);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftReabsorptionFiltrationCoefficient)
{
  EXPECT_EQ(false, es->HasLeftReabsorptionFiltrationCoefficient());
  es->GetLeftReabsorptionFiltrationCoefficient().SetValue(1.0, VolumePerTimePressureUnit::L_Per_min_mmHg);
  EXPECT_EQ(true, es->HasLeftReabsorptionFiltrationCoefficient());
  EXPECT_EQ(1.0, es->GetLeftReabsorptionFiltrationCoefficient(VolumePerTimePressureUnit::L_Per_min_mmHg));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasLeftReabsorptionFiltrationCoefficient());
  auto dvalue = ces.GetLeftReabsorptionFiltrationCoefficient(VolumePerTimePressureUnit::L_Per_min_mmHg);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightGlomerularFiltrationCoefficient)
{
  EXPECT_EQ(false, es->HasRightGlomerularFiltrationCoefficient());
  es->GetRightGlomerularFiltrationCoefficient().SetValue(1.0, VolumePerTimePressureUnit::L_Per_min_mmHg);
  EXPECT_EQ(true, es->HasRightGlomerularFiltrationCoefficient());
  EXPECT_EQ(1.0, es->GetRightGlomerularFiltrationCoefficient(VolumePerTimePressureUnit::L_Per_min_mmHg));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasRightGlomerularFiltrationCoefficient());
  auto dvalue = ces.GetRightGlomerularFiltrationCoefficient(VolumePerTimePressureUnit::L_Per_min_mmHg);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightReabsorptionFiltrationCoefficient)
{
  EXPECT_EQ(false, es->HasRightReabsorptionFiltrationCoefficient());
  es->GetRightReabsorptionFiltrationCoefficient().SetValue(1.0, VolumePerTimePressureUnit::L_Per_min_mmHg);
  EXPECT_EQ(true, es->HasRightReabsorptionFiltrationCoefficient());
  EXPECT_EQ(1.0, es->GetRightReabsorptionFiltrationCoefficient(VolumePerTimePressureUnit::L_Per_min_mmHg));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasRightReabsorptionFiltrationCoefficient());
  auto dvalue = ces.GetRightReabsorptionFiltrationCoefficient(VolumePerTimePressureUnit::L_Per_min_mmHg);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftGlomerularFiltrationSurfaceArea)
{
  EXPECT_EQ(false, es->HasLeftGlomerularFiltrationSurfaceArea());
  es->GetLeftGlomerularFiltrationSurfaceArea().SetValue(1.0, AreaUnit::m2);
  EXPECT_EQ(true, es->HasLeftGlomerularFiltrationSurfaceArea());
  EXPECT_EQ(1.0, es->GetLeftGlomerularFiltrationSurfaceArea(AreaUnit::m2));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasLeftGlomerularFiltrationSurfaceArea());
  auto dvalue = ces.GetLeftGlomerularFiltrationSurfaceArea(AreaUnit::m2);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftTubularReabsorptionFiltrationSurfaceArea)
{
  EXPECT_EQ(false, es->HasLeftTubularReabsorptionFiltrationSurfaceArea());
  es->GetLeftTubularReabsorptionFiltrationSurfaceArea().SetValue(1.0, AreaUnit::m2);
  EXPECT_EQ(true, es->HasLeftTubularReabsorptionFiltrationSurfaceArea());
  EXPECT_EQ(1.0, es->GetLeftTubularReabsorptionFiltrationSurfaceArea(AreaUnit::m2));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasLeftTubularReabsorptionFiltrationSurfaceArea());
  auto dvalue = ces.GetLeftTubularReabsorptionFiltrationSurfaceArea(AreaUnit::m2);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightGlomerularFiltrationSurfaceArea)
{
  EXPECT_EQ(false, es->HasRightGlomerularFiltrationSurfaceArea());
  es->GetRightGlomerularFiltrationSurfaceArea().SetValue(1.0, AreaUnit::m2);
  EXPECT_EQ(true, es->HasRightGlomerularFiltrationSurfaceArea());
  EXPECT_EQ(1.0, es->GetRightGlomerularFiltrationSurfaceArea(AreaUnit::m2));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasRightGlomerularFiltrationSurfaceArea());
  auto dvalue = ces.GetRightGlomerularFiltrationSurfaceArea(AreaUnit::m2);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightTubularReabsorptionFiltrationSurfaceArea)
{
  EXPECT_EQ(false, es->HasRightTubularReabsorptionFiltrationSurfaceArea());
  es->GetRightTubularReabsorptionFiltrationSurfaceArea().SetValue(1.0, AreaUnit::m2);
  EXPECT_EQ(true, es->HasRightTubularReabsorptionFiltrationSurfaceArea());
  EXPECT_EQ(1.0, es->GetRightTubularReabsorptionFiltrationSurfaceArea(AreaUnit::m2));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasRightTubularReabsorptionFiltrationSurfaceArea());
  auto dvalue = ces.GetRightTubularReabsorptionFiltrationSurfaceArea(AreaUnit::m2);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftGlomerularFluidPermeability)
{
  EXPECT_EQ(false, es->HasLeftGlomerularFluidPermeability());
  es->GetLeftGlomerularFluidPermeability().SetValue(1.0, VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2);
  EXPECT_EQ(true, es->HasLeftGlomerularFluidPermeability());
  EXPECT_EQ(1.0, es->GetLeftGlomerularFluidPermeability(VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasLeftGlomerularFluidPermeability());
  auto dvalue = ces.GetLeftGlomerularFluidPermeability(VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftTubularReabsorptionFluidPermeability)
{
  EXPECT_EQ(false, es->HasLeftTubularReabsorptionFluidPermeability());
  es->GetLeftTubularReabsorptionFluidPermeability().SetValue(1.0, VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2);
  EXPECT_EQ(true, es->HasLeftTubularReabsorptionFluidPermeability());
  EXPECT_EQ(1.0, es->GetLeftTubularReabsorptionFluidPermeability(VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasLeftTubularReabsorptionFluidPermeability());
  auto dvalue = ces.GetLeftTubularReabsorptionFluidPermeability(VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightGlomerularFluidPermeability)
{
  EXPECT_EQ(false, es->HasRightGlomerularFluidPermeability());
  es->GetRightGlomerularFluidPermeability().SetValue(1.0, VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2);
  EXPECT_EQ(true, es->HasRightGlomerularFluidPermeability());
  EXPECT_EQ(1.0, es->GetRightGlomerularFluidPermeability(VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasRightGlomerularFluidPermeability());
  auto dvalue = ces.GetRightGlomerularFluidPermeability(VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightTubularReabsorptionFluidPermeability)
{
  EXPECT_EQ(false, es->HasRightTubularReabsorptionFluidPermeability());
  es->GetRightTubularReabsorptionFluidPermeability().SetValue(1.0, VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2);
  EXPECT_EQ(true, es->HasRightTubularReabsorptionFluidPermeability());
  EXPECT_EQ(1.0, es->GetRightTubularReabsorptionFluidPermeability(VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasRightTubularReabsorptionFluidPermeability());
  auto dvalue = ces.GetRightTubularReabsorptionFluidPermeability(VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftNetFiltrationPressure)
{
  EXPECT_EQ(false, es->HasLeftNetFiltrationPressure());
  es->GetLeftNetFiltrationPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, es->HasLeftNetFiltrationPressure());
  EXPECT_EQ(1.0, es->GetLeftNetFiltrationPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasLeftNetFiltrationPressure());
  auto dvalue = ces.GetLeftNetFiltrationPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftNetReabsorptionPressure)
{
  EXPECT_EQ(false, es->HasLeftNetReabsorptionPressure());
  es->GetLeftNetReabsorptionPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, es->HasLeftNetReabsorptionPressure());
  EXPECT_EQ(1.0, es->GetLeftNetReabsorptionPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasLeftNetReabsorptionPressure());
  auto dvalue = ces.GetLeftNetReabsorptionPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftPeritubularCapillariesHydrostaticPressure)
{
  EXPECT_EQ(false, es->HasLeftPeritubularCapillariesHydrostaticPressure());
  es->GetLeftPeritubularCapillariesHydrostaticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, es->HasLeftPeritubularCapillariesHydrostaticPressure());
  EXPECT_EQ(1.0, es->GetLeftPeritubularCapillariesHydrostaticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasLeftPeritubularCapillariesHydrostaticPressure());
  auto dvalue = ces.GetLeftPeritubularCapillariesHydrostaticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftTubularHydrostaticPressure)
{
  EXPECT_EQ(false, es->HasLeftTubularHydrostaticPressure());
  es->GetLeftTubularHydrostaticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, es->HasLeftTubularHydrostaticPressure());
  EXPECT_EQ(1.0, es->GetLeftTubularHydrostaticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasLeftTubularHydrostaticPressure());
  auto dvalue = ces.GetLeftTubularHydrostaticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftTubularOsmoticPressure)
{
  EXPECT_EQ(false, es->HasLeftTubularOsmoticPressure());
  es->GetLeftTubularOsmoticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, es->HasLeftTubularOsmoticPressure());
  EXPECT_EQ(1.0, es->GetLeftTubularOsmoticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasLeftTubularOsmoticPressure());
  auto dvalue = ces.GetLeftTubularOsmoticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightBowmansCapsulesHydrostaticPressure)
{
  EXPECT_EQ(false, es->HasRightBowmansCapsulesHydrostaticPressure());
  es->GetRightBowmansCapsulesHydrostaticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, es->HasRightBowmansCapsulesHydrostaticPressure());
  EXPECT_EQ(1.0, es->GetRightBowmansCapsulesHydrostaticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasRightBowmansCapsulesHydrostaticPressure());
  auto dvalue = ces.GetRightBowmansCapsulesHydrostaticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightBowmansCapsulesOsmoticPressure)
{
  EXPECT_EQ(false, es->HasRightBowmansCapsulesOsmoticPressure());
  es->GetRightBowmansCapsulesOsmoticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, es->HasRightBowmansCapsulesOsmoticPressure());
  EXPECT_EQ(1.0, es->GetRightBowmansCapsulesOsmoticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasRightBowmansCapsulesOsmoticPressure());
  auto dvalue = ces.GetRightBowmansCapsulesOsmoticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightGlomerularCapillariesHydrostaticPressure)
{
  EXPECT_EQ(false, es->HasRightGlomerularCapillariesHydrostaticPressure());
  es->GetRightGlomerularCapillariesHydrostaticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, es->HasRightGlomerularCapillariesHydrostaticPressure());
  EXPECT_EQ(1.0, es->GetRightGlomerularCapillariesHydrostaticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasRightGlomerularCapillariesHydrostaticPressure());
  auto dvalue = ces.GetRightGlomerularCapillariesHydrostaticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightGlomerularCapillariesOsmoticPressure)
{
  EXPECT_EQ(false, es->HasRightGlomerularCapillariesOsmoticPressure());
  es->GetRightGlomerularCapillariesOsmoticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, es->HasRightGlomerularCapillariesOsmoticPressure());
  EXPECT_EQ(1.0, es->GetRightGlomerularCapillariesOsmoticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasRightGlomerularCapillariesOsmoticPressure());
  auto dvalue = ces.GetRightGlomerularCapillariesOsmoticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightNetFiltrationPressure)
{
  EXPECT_EQ(false, es->HasRightNetFiltrationPressure());
  es->GetRightNetFiltrationPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, es->HasRightNetFiltrationPressure());
  EXPECT_EQ(1.0, es->GetRightNetFiltrationPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasRightNetFiltrationPressure());
  auto dvalue = ces.GetRightNetFiltrationPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightNetReabsorptionPressure)
{
  EXPECT_EQ(false, es->HasRightNetReabsorptionPressure());
  es->GetRightNetReabsorptionPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, es->HasRightNetReabsorptionPressure());
  EXPECT_EQ(1.0, es->GetRightNetReabsorptionPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasRightNetReabsorptionPressure());
  auto dvalue = ces.GetRightNetReabsorptionPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightPeritubularCapillariesHydrostaticPressure)
{
  EXPECT_EQ(false, es->HasRightPeritubularCapillariesHydrostaticPressure());
  es->GetRightPeritubularCapillariesHydrostaticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, es->HasRightPeritubularCapillariesHydrostaticPressure());
  EXPECT_EQ(1.0, es->GetRightPeritubularCapillariesHydrostaticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasRightPeritubularCapillariesHydrostaticPressure());
  auto dvalue = ces.GetRightPeritubularCapillariesHydrostaticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightPeritubularCapillariesOsmoticPressure)
{
  EXPECT_EQ(false, es->HasRightPeritubularCapillariesOsmoticPressure());
  es->GetRightPeritubularCapillariesOsmoticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, es->HasRightPeritubularCapillariesOsmoticPressure());
  EXPECT_EQ(1.0, es->GetRightPeritubularCapillariesOsmoticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasRightPeritubularCapillariesOsmoticPressure());
  auto dvalue = ces.GetRightPeritubularCapillariesOsmoticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightTubularHydrostaticPressure)
{
  EXPECT_EQ(false, es->HasRightTubularHydrostaticPressure());
  es->GetRightTubularHydrostaticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, es->HasRightTubularHydrostaticPressure());
  EXPECT_EQ(1.0, es->GetRightTubularHydrostaticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasRightTubularHydrostaticPressure());
  auto dvalue = ces.GetRightTubularHydrostaticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightTubularOsmoticPressure)
{
  EXPECT_EQ(false, es->HasRightTubularOsmoticPressure());
  es->GetRightTubularOsmoticPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, es->HasRightTubularOsmoticPressure());
  EXPECT_EQ(1.0, es->GetRightTubularOsmoticPressure(PressureUnit::Pa));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasRightTubularOsmoticPressure());
  auto dvalue = ces.GetRightTubularOsmoticPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftReabsorptionRate)
{
  EXPECT_EQ(false, es->HasLeftReabsorptionRate());
  es->GetLeftReabsorptionRate().SetValue(1.0, VolumePerTimeUnit::L_Per_min);
  EXPECT_EQ(true, es->HasLeftReabsorptionRate());
  EXPECT_EQ(1.0, es->GetLeftReabsorptionRate(VolumePerTimeUnit::L_Per_min));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasLeftReabsorptionRate());
  auto dvalue = ces.GetLeftReabsorptionRate(VolumePerTimeUnit::L_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RenalBloodFlow)
{
  EXPECT_EQ(false, es->HasRenalBloodFlow());
  es->GetRenalBloodFlow().SetValue(1.0, VolumePerTimeUnit::L_Per_min);
  EXPECT_EQ(true, es->HasRenalBloodFlow());
  EXPECT_EQ(1.0, es->GetRenalBloodFlow(VolumePerTimeUnit::L_Per_min));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasRenalBloodFlow());
  auto dvalue = ces.GetRenalBloodFlow(VolumePerTimeUnit::L_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RenalPlasmaFlow)
{
  EXPECT_EQ(false, es->HasRenalPlasmaFlow());
  es->GetRenalPlasmaFlow().SetValue(1.0, VolumePerTimeUnit::L_Per_min);
  EXPECT_EQ(true, es->HasRenalPlasmaFlow());
  EXPECT_EQ(1.0, es->GetRenalPlasmaFlow(VolumePerTimeUnit::L_Per_min));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasRenalPlasmaFlow());
  auto dvalue = ces.GetRenalPlasmaFlow(VolumePerTimeUnit::L_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightGlomerularFiltrationRate)
{
  EXPECT_EQ(false, es->HasRightGlomerularFiltrationRate());
  es->GetRightGlomerularFiltrationRate().SetValue(1.0, VolumePerTimeUnit::L_Per_min);
  EXPECT_EQ(true, es->HasRightGlomerularFiltrationRate());
  EXPECT_EQ(1.0, es->GetRightGlomerularFiltrationRate(VolumePerTimeUnit::L_Per_min));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasRightGlomerularFiltrationRate());
  auto dvalue = ces.GetRightGlomerularFiltrationRate(VolumePerTimeUnit::L_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightReabsorptionRate)
{
  EXPECT_EQ(false, es->HasRightReabsorptionRate());
  es->GetRightReabsorptionRate().SetValue(1.0, VolumePerTimeUnit::L_Per_min);
  EXPECT_EQ(true, es->HasRightReabsorptionRate());
  EXPECT_EQ(1.0, es->GetRightReabsorptionRate(VolumePerTimeUnit::L_Per_min));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasRightReabsorptionRate());
  auto dvalue = ces.GetRightReabsorptionRate(VolumePerTimeUnit::L_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, UrinationRate)
{
  EXPECT_EQ(false, es->HasUrinationRate());
  es->GetUrinationRate().SetValue(1.0, VolumePerTimeUnit::L_Per_min);
  EXPECT_EQ(true, es->HasUrinationRate());
  EXPECT_EQ(1.0, es->GetUrinationRate(VolumePerTimeUnit::L_Per_min));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasUrinationRate());
  auto dvalue = ces.GetUrinationRate(VolumePerTimeUnit::L_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, UrineProductionRate)
{
  EXPECT_EQ(false, es->HasUrineProductionRate());
  es->GetUrineProductionRate().SetValue(1.0, VolumePerTimeUnit::L_Per_min);
  EXPECT_EQ(true, es->HasUrineProductionRate());
  EXPECT_EQ(1.0, es->GetUrineProductionRate(VolumePerTimeUnit::L_Per_min));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasUrineProductionRate());
  auto dvalue = ces.GetUrineProductionRate(VolumePerTimeUnit::L_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RenalVascularResistance)
{
  EXPECT_EQ(false, es->HasRenalVascularResistance());
  es->GetRenalVascularResistance().SetValue(1.0, FlowResistanceUnit::Pa_s_Per_m3);
  EXPECT_EQ(true, es->HasRenalVascularResistance());
  EXPECT_EQ(1.0, es->GetRenalVascularResistance(FlowResistanceUnit::Pa_s_Per_m3));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasRenalVascularResistance());
  auto dvalue = ces.GetRenalVascularResistance(FlowResistanceUnit::Pa_s_Per_m3);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightAfferentArterioleResistance)
{
  EXPECT_EQ(false, es->HasRightAfferentArterioleResistance());
  es->GetRightAfferentArterioleResistance().SetValue(1.0, FlowResistanceUnit::Pa_s_Per_m3);
  EXPECT_EQ(true, es->HasRightAfferentArterioleResistance());
  EXPECT_EQ(1.0, es->GetRightAfferentArterioleResistance(FlowResistanceUnit::Pa_s_Per_m3));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasRightAfferentArterioleResistance());
  auto dvalue = ces.GetRightAfferentArterioleResistance(FlowResistanceUnit::Pa_s_Per_m3);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightEfferentArterioleResistance)
{
  EXPECT_EQ(false, es->HasRightEfferentArterioleResistance());
  es->GetRightEfferentArterioleResistance().SetValue(1.0, FlowResistanceUnit::Pa_s_Per_m3);
  EXPECT_EQ(true, es->HasRightEfferentArterioleResistance());
  EXPECT_EQ(1.0, es->GetRightEfferentArterioleResistance(FlowResistanceUnit::Pa_s_Per_m3));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasRightEfferentArterioleResistance());
  auto dvalue = ces.GetRightEfferentArterioleResistance(FlowResistanceUnit::Pa_s_Per_m3);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, UrineOsmolality)
{
  EXPECT_EQ(false, es->HasUrineOsmolality());
  es->GetUrineOsmolality().SetValue(1.0, OsmolalityUnit::Osm_Per_kg);
  EXPECT_EQ(true, es->HasUrineOsmolality());
  EXPECT_EQ(1.0, es->GetUrineOsmolality(OsmolalityUnit::Osm_Per_kg));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasUrineOsmolality());
  auto dvalue = ces.GetUrineOsmolality(OsmolalityUnit::Osm_Per_kg);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, UrineVolume)
{
  EXPECT_EQ(false, es->HasUrineVolume());
  es->GetUrineVolume().SetValue(1.0, VolumeUnit::L);
  EXPECT_EQ(true, es->HasUrineVolume());
  EXPECT_EQ(1.0, es->GetUrineVolume(VolumeUnit::L));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasUrineVolume());
  auto dvalue = ces.GetUrineVolume(VolumeUnit::L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, UrineUreaNitrogenConcentration)
{
  EXPECT_EQ(false, es->HasUrineUreaNitrogenConcentration());
  es->GetUrineUreaNitrogenConcentration().SetValue(1.0, MassPerVolumeUnit::kg_Per_L);
  EXPECT_EQ(true, es->HasUrineUreaNitrogenConcentration());
  EXPECT_EQ(1.0, es->GetUrineUreaNitrogenConcentration(MassPerVolumeUnit::kg_Per_L));

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasUrineUreaNitrogenConcentration());
  auto dvalue = ces.GetUrineUreaNitrogenConcentration(MassPerVolumeUnit::kg_Per_L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LeftFiltrationFraction)
{
  EXPECT_EQ(false, es->HasLeftFiltrationFraction());
  es->GetLeftFiltrationFraction().SetValue(1.0);
  EXPECT_EQ(true, es->HasLeftFiltrationFraction());
  EXPECT_EQ(1.0, es->GetLeftFiltrationFraction().GetValue());

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasLeftFiltrationFraction());
  auto dvalue = ces.GetLeftFiltrationFraction();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RightFiltrationFraction)
{
  EXPECT_EQ(false, es->HasRightFiltrationFraction());
  es->GetRightFiltrationFraction().SetValue(1.0);
  EXPECT_EQ(true, es->HasRightFiltrationFraction());
  EXPECT_EQ(1.0, es->GetRightFiltrationFraction().GetValue());

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasRightFiltrationFraction());
  auto dvalue = ces.GetRightFiltrationFraction();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, UrineSpecificGravity)
{
  EXPECT_EQ(false, es->HasUrineSpecificGravity());
  es->GetUrineSpecificGravity().SetValue(1.0);
  EXPECT_EQ(true, es->HasUrineSpecificGravity());
  EXPECT_EQ(1.0, es->GetUrineSpecificGravity().GetValue());

  const biogears::SERenalSystem ces(logger);
  EXPECT_EQ(false, ces.HasUrineSpecificGravity());
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