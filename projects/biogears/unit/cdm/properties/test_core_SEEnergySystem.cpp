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
  virtual void SetUp() override;

  // Code here will be called immediately after each test (right
  // before the destructor).
  virtual void TearDown() override;

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
  EXPECT_FALSE( ds->HasAchievedExerciseLevel());
  ds->GetAchievedExerciseLevel().SetValue(1.0);
  EXPECT_TRUE( ds->HasAchievedExerciseLevel());
  EXPECT_EQ(1.0, ds->GetAchievedExerciseLevel().GetValue());
  
  const biogears::SEEnergySystem cds(logger);
  EXPECT_FALSE( cds.HasAchievedExerciseLevel());
  auto dvalue = cds.GetAchievedExerciseLevel();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, ChlorideLostToSweat)
{
  EXPECT_FALSE( ds->HasChlorideLostToSweat());
  ds->GetChlorideLostToSweat().SetValue(1.0, MassUnit::g);
  EXPECT_TRUE( ds->HasChlorideLostToSweat());
  EXPECT_EQ(1.0, ds->GetChlorideLostToSweat().GetValue(MassUnit::g));

  const biogears::SEEnergySystem cds(logger);
  EXPECT_FALSE( cds.HasChlorideLostToSweat());
  auto dvalue = cds.GetChlorideLostToSweat(MassUnit::g);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, CoreTemperature)
{
  EXPECT_FALSE( ds->HasCoreTemperature());
  ds->GetCoreTemperature().SetValue(1.0, TemperatureUnit::C);
  EXPECT_TRUE( ds->HasCoreTemperature());
  EXPECT_EQ(1.0, ds->GetCoreTemperature().GetValue(TemperatureUnit::C));

  const biogears::SEEnergySystem cds(logger);
  EXPECT_FALSE( cds.HasCoreTemperature());
  auto dvalue = cds.GetCoreTemperature(TemperatureUnit::C);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, CreatinineProductionRate)
{
  EXPECT_FALSE( ds->HasCreatinineProductionRate());
  ds->GetCreatinineProductionRate().SetValue(1.0, AmountPerTimeUnit::mmol_Per_min);
  EXPECT_TRUE( ds->HasCreatinineProductionRate());
  EXPECT_EQ(1.0, ds->GetCreatinineProductionRate().GetValue(AmountPerTimeUnit::mmol_Per_min));

  const biogears::SEEnergySystem cds(logger);
  EXPECT_FALSE( cds.HasCreatinineProductionRate());
  auto dvalue = cds.GetCreatinineProductionRate(AmountPerTimeUnit::mmol_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, ExerciseMeanArterialPressureDelta)
{
  EXPECT_FALSE( ds->HasExerciseMeanArterialPressureDelta());
  ds->GetExerciseMeanArterialPressureDelta().SetValue(1.0, PressureUnit::Pa);
  EXPECT_TRUE( ds->HasExerciseMeanArterialPressureDelta());
  EXPECT_EQ(1.0, ds->GetExerciseMeanArterialPressureDelta().GetValue(PressureUnit::Pa));

  const biogears::SEEnergySystem cds(logger);
  EXPECT_FALSE( cds.HasExerciseMeanArterialPressureDelta());
  auto dvalue = cds.GetExerciseMeanArterialPressureDelta(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, FatigueLevel)
{
  EXPECT_FALSE( ds->HasFatigueLevel());
  ds->GetFatigueLevel().SetValue(1.0);
  EXPECT_TRUE( ds->HasFatigueLevel());
  EXPECT_EQ(1.0, ds->GetFatigueLevel().GetValue());

  const biogears::SEEnergySystem cds(logger);
  EXPECT_FALSE( cds.HasFatigueLevel());
  auto dvalue = cds.GetFatigueLevel();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LactateProductionRate)
{
  EXPECT_FALSE( ds->HasLactateProductionRate());
  ds->GetLactateProductionRate().SetValue(1.0, AmountPerTimeUnit::mmol_Per_min);
  EXPECT_TRUE( ds->HasLactateProductionRate());
  EXPECT_EQ(1.0, ds->GetLactateProductionRate().GetValue(AmountPerTimeUnit::mmol_Per_min));

  const biogears::SEEnergySystem cds(logger);
  EXPECT_FALSE( cds.HasLactateProductionRate());
  auto dvalue = cds.GetLactateProductionRate(AmountPerTimeUnit::mmol_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, PotassiumLostToSweat)
{
  EXPECT_FALSE( ds->HasPotassiumLostToSweat());
  ds->GetPotassiumLostToSweat().SetValue(1.0, MassUnit::g);
  EXPECT_TRUE( ds->HasPotassiumLostToSweat());
  EXPECT_EQ(1.0, ds->GetPotassiumLostToSweat().GetValue(MassUnit::g));

  const biogears::SEEnergySystem cds(logger);
  EXPECT_FALSE( cds.HasPotassiumLostToSweat());
  auto dvalue = cds.GetPotassiumLostToSweat(MassUnit::g);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, SkinTemperature)
{
  EXPECT_FALSE( ds->HasSkinTemperature());
  ds->GetSkinTemperature().SetValue(1.0, TemperatureUnit::C);
  EXPECT_TRUE( ds->HasSkinTemperature());
  EXPECT_EQ(1.0, ds->GetSkinTemperature().GetValue(TemperatureUnit::C));

  const biogears::SEEnergySystem cds(logger);
  EXPECT_FALSE( cds.HasSkinTemperature());
  auto dvalue = cds.GetSkinTemperature(TemperatureUnit::C);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, SodiumLostToSweat)
{
  EXPECT_FALSE( ds->HasSodiumLostToSweat());
  ds->GetSodiumLostToSweat().SetValue(1.0, MassUnit::g);
  EXPECT_TRUE( ds->HasSodiumLostToSweat());
  EXPECT_EQ(1.0, ds->GetSodiumLostToSweat().GetValue(MassUnit::g));

  const biogears::SEEnergySystem cds(logger);
  EXPECT_FALSE( cds.HasSodiumLostToSweat());
  auto dvalue = cds.GetSodiumLostToSweat(MassUnit::g);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, SweatRate)
{
  EXPECT_FALSE( ds->HasSweatRate());
  ds->GetSweatRate().SetValue(1.0, MassPerTimeUnit::g_Per_day);
  EXPECT_TRUE( ds->HasSweatRate());
  EXPECT_EQ(1.0, ds->GetSweatRate().GetValue(MassPerTimeUnit::g_Per_day));

  const biogears::SEEnergySystem cds(logger);
  EXPECT_FALSE( cds.HasSweatRate());
  auto dvalue = cds.GetSweatRate(MassPerTimeUnit::g_Per_day);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, TotalMetabolicRate)
{
  EXPECT_FALSE( ds->HasTotalMetabolicRate());
  ds->GetTotalMetabolicRate().SetValue(1.0, PowerUnit::BTU_Per_hr);
  EXPECT_TRUE( ds->HasTotalMetabolicRate());
  EXPECT_EQ(1.0, ds->GetTotalMetabolicRate().GetValue(PowerUnit::BTU_Per_hr));

  const biogears::SEEnergySystem cds(logger);
  EXPECT_FALSE( cds.HasTotalMetabolicRate());
  auto dvalue = cds.GetTotalMetabolicRate(PowerUnit::BTU_Per_hr);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, TotalWorkRateLevel)
{
  EXPECT_FALSE( ds->HasTotalWorkRateLevel());
  ds->GetTotalWorkRateLevel().SetValue(1.0);
  EXPECT_TRUE( ds->HasTotalWorkRateLevel());
  EXPECT_EQ(1.0, ds->GetTotalWorkRateLevel().GetValue());

  const biogears::SEEnergySystem cds(logger);
  EXPECT_FALSE( cds.HasTotalWorkRateLevel());
  auto dvalue = cds.GetTotalWorkRateLevel();
  EXPECT_NE(dvalue, dvalue);
}