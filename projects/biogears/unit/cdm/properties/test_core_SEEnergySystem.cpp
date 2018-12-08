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
#include <biogears/cdm/system/physiology/SEEnergySystem.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

#ifdef DISABLE_BIOGEARS_SEEnergySystem_TEST
#define TEST_FIXTURE_NAME DISABLED_SEEnergySystem_Fixture
#else
#define TEST_FIXTURE_NAME SEEnergySystem_Fixture
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
  biogears::SEEnergySystem* ds;
};

void TEST_FIXTURE_NAME::SetUp()
{
  logger = new biogears::Logger;
  ds = new biogears::SEEnergySystem(logger);
}

void TEST_FIXTURE_NAME::TearDown()
{
  delete ds;
  delete logger;
}
TEST_F(TEST_FIXTURE_NAME, AchievedExerciseLevel)
{
  EXPECT_EQ(false, ds->HasAchievedExerciseLevel());
  ds->GetAchievedExerciseLevel().SetValue(1.0);
  EXPECT_EQ(true, ds->HasAchievedExerciseLevel());
  EXPECT_EQ(1.0, ds->GetAchievedExerciseLevel().GetValue());
  
  const biogears::SEEnergySystem cds(logger);
  EXPECT_EQ(false, cds.HasAchievedExerciseLevel());
  auto dvalue = cds.GetAchievedExerciseLevel();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, ChlorideLostToSweat)
{
  EXPECT_EQ(false, ds->HasChlorideLostToSweat());
  ds->GetChlorideLostToSweat().SetValue(1.0, MassUnit::g);
  EXPECT_EQ(true, ds->HasChlorideLostToSweat());
  EXPECT_EQ(1.0, ds->GetChlorideLostToSweat().GetValue(MassUnit::g));

  const biogears::SEEnergySystem cds(logger);
  EXPECT_EQ(false, cds.HasChlorideLostToSweat());
  auto dvalue = cds.GetChlorideLostToSweat(MassUnit::g);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, CoreTemperature)
{
  EXPECT_EQ(false, ds->HasCoreTemperature());
  ds->GetCoreTemperature().SetValue(1.0, TemperatureUnit::C);
  EXPECT_EQ(true, ds->HasCoreTemperature());
  EXPECT_EQ(1.0, ds->GetCoreTemperature().GetValue(TemperatureUnit::C));

  const biogears::SEEnergySystem cds(logger);
  EXPECT_EQ(false, cds.HasCoreTemperature());
  auto dvalue = cds.GetCoreTemperature(TemperatureUnit::C);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, CreatinineProductionRate)
{
  EXPECT_EQ(false, ds->HasCreatinineProductionRate());
  ds->GetCreatinineProductionRate().SetValue(1.0, AmountPerTimeUnit::mmol_Per_min);
  EXPECT_EQ(true, ds->HasCreatinineProductionRate());
  EXPECT_EQ(1.0, ds->GetCreatinineProductionRate().GetValue(AmountPerTimeUnit::mmol_Per_min));

  const biogears::SEEnergySystem cds(logger);
  EXPECT_EQ(false, cds.HasCreatinineProductionRate());
  auto dvalue = cds.GetCreatinineProductionRate(AmountPerTimeUnit::mmol_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, ExerciseMeanArterialPressureDelta)
{
  EXPECT_EQ(false, ds->HasExerciseMeanArterialPressureDelta());
  ds->GetExerciseMeanArterialPressureDelta().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, ds->HasExerciseMeanArterialPressureDelta());
  EXPECT_EQ(1.0, ds->GetExerciseMeanArterialPressureDelta().GetValue(PressureUnit::Pa));

  const biogears::SEEnergySystem cds(logger);
  EXPECT_EQ(false, cds.HasExerciseMeanArterialPressureDelta());
  auto dvalue = cds.GetExerciseMeanArterialPressureDelta(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, FatigueLevel)
{
  EXPECT_EQ(false, ds->HasFatigueLevel());
  ds->GetFatigueLevel().SetValue(1.0);
  EXPECT_EQ(true, ds->HasFatigueLevel());
  EXPECT_EQ(1.0, ds->GetFatigueLevel().GetValue());

  const biogears::SEEnergySystem cds(logger);
  EXPECT_EQ(false, cds.HasFatigueLevel());
  auto dvalue = cds.GetFatigueLevel();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LactateProductionRate)
{
  EXPECT_EQ(false, ds->HasLactateProductionRate());
  ds->GetLactateProductionRate().SetValue(1.0, AmountPerTimeUnit::mmol_Per_min);
  EXPECT_EQ(true, ds->HasLactateProductionRate());
  EXPECT_EQ(1.0, ds->GetLactateProductionRate().GetValue(AmountPerTimeUnit::mmol_Per_min));

  const biogears::SEEnergySystem cds(logger);
  EXPECT_EQ(false, cds.HasLactateProductionRate());
  auto dvalue = cds.GetLactateProductionRate(AmountPerTimeUnit::mmol_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, PotassiumLostToSweat)
{
  EXPECT_EQ(false, ds->HasPotassiumLostToSweat());
  ds->GetPotassiumLostToSweat().SetValue(1.0, MassUnit::g);
  EXPECT_EQ(true, ds->HasPotassiumLostToSweat());
  EXPECT_EQ(1.0, ds->GetPotassiumLostToSweat().GetValue(MassUnit::g));

  const biogears::SEEnergySystem cds(logger);
  EXPECT_EQ(false, cds.HasPotassiumLostToSweat());
  auto dvalue = cds.GetPotassiumLostToSweat(MassUnit::g);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, SkinTemperature)
{
  EXPECT_EQ(false, ds->HasSkinTemperature());
  ds->GetSkinTemperature().SetValue(1.0, TemperatureUnit::C);
  EXPECT_EQ(true, ds->HasSkinTemperature());
  EXPECT_EQ(1.0, ds->GetSkinTemperature().GetValue(TemperatureUnit::C));

  const biogears::SEEnergySystem cds(logger);
  EXPECT_EQ(false, cds.HasSkinTemperature());
  auto dvalue = cds.GetSkinTemperature(TemperatureUnit::C);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, SodiumLostToSweat)
{
  EXPECT_EQ(false, ds->HasSodiumLostToSweat());
  ds->GetSodiumLostToSweat().SetValue(1.0, MassUnit::g);
  EXPECT_EQ(true, ds->HasSodiumLostToSweat());
  EXPECT_EQ(1.0, ds->GetSodiumLostToSweat().GetValue(MassUnit::g));

  const biogears::SEEnergySystem cds(logger);
  EXPECT_EQ(false, cds.HasSodiumLostToSweat());
  auto dvalue = cds.GetSodiumLostToSweat(MassUnit::g);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, SweatRate)
{
  EXPECT_EQ(false, ds->HasSweatRate());
  ds->GetSweatRate().SetValue(1.0, MassPerTimeUnit::g_Per_day);
  EXPECT_EQ(true, ds->HasSweatRate());
  EXPECT_EQ(1.0, ds->GetSweatRate().GetValue(MassPerTimeUnit::g_Per_day));

  const biogears::SEEnergySystem cds(logger);
  EXPECT_EQ(false, cds.HasSweatRate());
  auto dvalue = cds.GetSweatRate(MassPerTimeUnit::g_Per_day);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, TotalMetabolicRate)
{
  EXPECT_EQ(false, ds->HasTotalMetabolicRate());
  ds->GetTotalMetabolicRate().SetValue(1.0, PowerUnit::BTU_Per_hr);
  EXPECT_EQ(true, ds->HasTotalMetabolicRate());
  EXPECT_EQ(1.0, ds->GetTotalMetabolicRate().GetValue(PowerUnit::BTU_Per_hr));

  const biogears::SEEnergySystem cds(logger);
  EXPECT_EQ(false, cds.HasTotalMetabolicRate());
  auto dvalue = cds.GetTotalMetabolicRate(PowerUnit::BTU_Per_hr);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, TotalWorkRateLevel)
{
  EXPECT_EQ(false, ds->HasTotalWorkRateLevel());
  ds->GetTotalWorkRateLevel().SetValue(1.0);
  EXPECT_EQ(true, ds->HasTotalWorkRateLevel());
  EXPECT_EQ(1.0, ds->GetTotalWorkRateLevel().GetValue());

  const biogears::SEEnergySystem cds(logger);
  EXPECT_EQ(false, cds.HasTotalWorkRateLevel());
  auto dvalue = cds.GetTotalWorkRateLevel();
  EXPECT_NE(dvalue, dvalue);
}