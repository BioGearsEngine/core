//-------------------------------------------------------------------------------------------
//- Copyright 2017 Applied Research Associates, Inc.
//- Licensed under the ALche License, Version 2.0 (the "License"); you may not use
//- this file except in compliance with the License. You may obtain a copy of the License
//- at:
//- http://www.aLche.org/licenses/LICENSE-2.0
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
#include <biogears/cdm/system/physiology/SETissueSystem.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

#ifdef DISABLE_BIOGEARS_SETissueSystem_TEST
#define TEST_FIXTURE_NAME DISABLED_SETissueSystem_Fixture
#else
#define TEST_FIXTURE_NAME SETissueSystem_Fixture
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
  biogears::SETissueSystem* es;
};

void TEST_FIXTURE_NAME::SetUp()
{
  logger = new biogears::Logger;
  es = new biogears::SETissueSystem(logger);
}

void TEST_FIXTURE_NAME::TearDown()
{
  delete es;
  delete logger;
}
TEST_F(TEST_FIXTURE_NAME, ExtracellularFluidVolume)
{
  EXPECT_FALSE( es->HasExtracellularFluidVolume());
  es->GetExtracellularFluidVolume().SetValue(1.0,VolumeUnit::L);
  EXPECT_TRUE( es->HasExtracellularFluidVolume());
  EXPECT_EQ(1.0, es->GetExtracellularFluidVolume(VolumeUnit::L));

  const biogears::SETissueSystem ces(logger);
  EXPECT_FALSE( ces.HasExtracellularFluidVolume());
  auto dvalue = ces.GetExtracellularFluidVolume(VolumeUnit::L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, ExtravascularFluidVolume)
{
  EXPECT_FALSE( es->HasExtravascularFluidVolume());
  es->GetExtravascularFluidVolume().SetValue(1.0,VolumeUnit::L);
  EXPECT_TRUE( es->HasExtravascularFluidVolume());
  EXPECT_EQ(1.0, es->GetExtravascularFluidVolume(VolumeUnit::L));

  const biogears::SETissueSystem ces(logger);
  EXPECT_FALSE( ces.HasExtravascularFluidVolume());
  auto dvalue = ces.GetExtravascularFluidVolume(VolumeUnit::L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, IntracellularFluidVolume)
{
  EXPECT_FALSE( es->HasIntracellularFluidVolume());
  es->GetIntracellularFluidVolume().SetValue(1.0,VolumeUnit::L);
  EXPECT_TRUE( es->HasIntracellularFluidVolume());
  EXPECT_EQ(1.0, es->GetIntracellularFluidVolume(VolumeUnit::L));

  const biogears::SETissueSystem ces(logger);
  EXPECT_FALSE( ces.HasIntracellularFluidVolume());
  auto dvalue = ces.GetIntracellularFluidVolume(VolumeUnit::L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, TotalBodyFluidVolume)
{
  EXPECT_FALSE( es->HasTotalBodyFluidVolume());
  es->GetTotalBodyFluidVolume().SetValue(1.0,VolumeUnit::L);
  EXPECT_TRUE( es->HasTotalBodyFluidVolume());
  EXPECT_EQ(1.0, es->GetTotalBodyFluidVolume(VolumeUnit::L));

  const biogears::SETissueSystem ces(logger);
  EXPECT_FALSE( ces.HasTotalBodyFluidVolume());
  auto dvalue = ces.GetTotalBodyFluidVolume(VolumeUnit::L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, CarbonDioxideProductionRate)
{
  EXPECT_FALSE( es->HasCarbonDioxideProductionRate());
  es->GetCarbonDioxideProductionRate().SetValue(1.0,VolumePerTimeUnit::L_Per_min);
  EXPECT_TRUE( es->HasCarbonDioxideProductionRate());
  EXPECT_EQ(1.0, es->GetCarbonDioxideProductionRate(VolumePerTimeUnit::L_Per_min));

  const biogears::SETissueSystem ces(logger);
  EXPECT_FALSE( ces.HasCarbonDioxideProductionRate());
  auto dvalue = ces.GetCarbonDioxideProductionRate(VolumePerTimeUnit::L_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, OxygenConsumptionRate)
{
  EXPECT_FALSE( es->HasOxygenConsumptionRate());
  es->GetOxygenConsumptionRate().SetValue(1.0,VolumePerTimeUnit::L_Per_min);
  EXPECT_TRUE( es->HasOxygenConsumptionRate());
  EXPECT_EQ(1.0, es->GetOxygenConsumptionRate(VolumePerTimeUnit::L_Per_min));

  const biogears::SETissueSystem ces(logger);
  EXPECT_FALSE( ces.HasOxygenConsumptionRate());
  auto dvalue = ces.GetOxygenConsumptionRate(VolumePerTimeUnit::L_Per_min);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LiverInsulinSetPoint)
{
  EXPECT_FALSE( es->HasLiverInsulinSetPoint());
  es->GetLiverInsulinSetPoint().SetValue(1.0,AmountPerVolumeUnit::ct_Per_L);
  EXPECT_TRUE( es->HasLiverInsulinSetPoint());
  EXPECT_EQ(1.0, es->GetLiverInsulinSetPoint(AmountPerVolumeUnit::ct_Per_L));

  const biogears::SETissueSystem ces(logger);
  EXPECT_FALSE( ces.HasLiverInsulinSetPoint());
  auto dvalue = ces.GetLiverInsulinSetPoint(AmountPerVolumeUnit::ct_Per_L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, MuscleInsulinSetPoint)
{
  EXPECT_FALSE( es->HasMuscleInsulinSetPoint());
  es->GetMuscleInsulinSetPoint().SetValue(1.0,AmountPerVolumeUnit::ct_Per_L);
  EXPECT_TRUE( es->HasMuscleInsulinSetPoint());
  EXPECT_EQ(1.0, es->GetMuscleInsulinSetPoint(AmountPerVolumeUnit::ct_Per_L));

  const biogears::SETissueSystem ces(logger);
  EXPECT_FALSE( ces.HasMuscleInsulinSetPoint());
  auto dvalue = ces.GetMuscleInsulinSetPoint(AmountPerVolumeUnit::ct_Per_L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, FatInsulinSetPoint)
{
  EXPECT_FALSE( es->HasFatInsulinSetPoint());
  es->GetFatInsulinSetPoint().SetValue(1.0,AmountPerVolumeUnit::ct_Per_L);
  EXPECT_TRUE( es->HasFatInsulinSetPoint());
  EXPECT_EQ(1.0, es->GetFatInsulinSetPoint(AmountPerVolumeUnit::ct_Per_L));

  const biogears::SETissueSystem ces(logger);
  EXPECT_FALSE( ces.HasFatInsulinSetPoint());
  auto dvalue = ces.GetFatInsulinSetPoint(AmountPerVolumeUnit::ct_Per_L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, LiverGlucagonSetPoint)
{
  EXPECT_FALSE( es->HasLiverGlucagonSetPoint());
  es->GetLiverGlucagonSetPoint().SetValue(1.0,MassPerVolumeUnit::kg_Per_L);
  EXPECT_TRUE( es->HasLiverGlucagonSetPoint());
  EXPECT_EQ(1.0, es->GetLiverGlucagonSetPoint(MassPerVolumeUnit::kg_Per_L));

  const biogears::SETissueSystem ces(logger);
  EXPECT_FALSE( ces.HasLiverGlucagonSetPoint());
  auto dvalue = ces.GetLiverGlucagonSetPoint(MassPerVolumeUnit::kg_Per_L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, MuscleGlucagonSetPoint)
{
  EXPECT_FALSE( es->HasMuscleGlucagonSetPoint());
  es->GetMuscleGlucagonSetPoint().SetValue(1.0,MassPerVolumeUnit::kg_Per_L);
  EXPECT_TRUE( es->HasMuscleGlucagonSetPoint());
  EXPECT_EQ(1.0, es->GetMuscleGlucagonSetPoint(MassPerVolumeUnit::kg_Per_L));

  const biogears::SETissueSystem ces(logger);
  EXPECT_FALSE( ces.HasMuscleGlucagonSetPoint());
  auto dvalue = ces.GetMuscleGlucagonSetPoint(MassPerVolumeUnit::kg_Per_L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, FatGlucagonSetPoint)
{
  EXPECT_FALSE( es->HasFatGlucagonSetPoint());
  es->GetFatGlucagonSetPoint().SetValue(1.0,MassPerVolumeUnit::kg_Per_L);
  EXPECT_TRUE( es->HasFatGlucagonSetPoint());
  EXPECT_EQ(1.0, es->GetFatGlucagonSetPoint(MassPerVolumeUnit::kg_Per_L));

  const biogears::SETissueSystem ces(logger);
  EXPECT_FALSE( ces.HasFatGlucagonSetPoint());
  auto dvalue = ces.GetFatGlucagonSetPoint(MassPerVolumeUnit::kg_Per_L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, StoredProtein)
{
  EXPECT_FALSE( es->HasStoredProtein());
  es->GetStoredProtein().SetValue(1.0,MassUnit::kg);
  EXPECT_TRUE( es->HasStoredProtein());
  EXPECT_EQ(1.0, es->GetStoredProtein(MassUnit::kg));

  const biogears::SETissueSystem ces(logger);
  EXPECT_FALSE( ces.HasStoredProtein());
  auto dvalue = ces.GetStoredProtein(MassUnit::kg);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, StoredFat)
{
  EXPECT_FALSE( es->HasStoredFat());
  es->GetStoredFat().SetValue(1.0,MassUnit::kg);
  EXPECT_TRUE( es->HasStoredFat());
  EXPECT_EQ(1.0, es->GetStoredFat(MassUnit::kg));

  const biogears::SETissueSystem ces(logger);
  EXPECT_FALSE( ces.HasStoredFat());
  auto dvalue = ces.GetStoredFat(MassUnit::kg);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, DehydrationFraction)
{
  EXPECT_FALSE( es->HasDehydrationFraction());
  es->GetDehydrationFraction().SetValue(1.0);
  EXPECT_TRUE( es->HasDehydrationFraction());
  EXPECT_EQ(1.0, es->GetDehydrationFraction().GetValue());

  const biogears::SETissueSystem ces(logger);
  EXPECT_FALSE( ces.HasDehydrationFraction());
  auto dvalue = ces.GetDehydrationFraction();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, IntracellularFluidPH)
{
  EXPECT_FALSE( es->HasIntracellularFluidPH());
  es->GetIntracellularFluidPH().SetValue(1.0);
  EXPECT_TRUE( es->HasIntracellularFluidPH());
  EXPECT_EQ(1.0, es->GetIntracellularFluidPH().GetValue());

  const biogears::SETissueSystem ces(logger);
  EXPECT_FALSE( ces.HasIntracellularFluidPH());
  auto dvalue = ces.GetIntracellularFluidPH();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RespiratoryExchangeRatio)
{
  EXPECT_FALSE( es->HasRespiratoryExchangeRatio());
  es->GetRespiratoryExchangeRatio().SetValue(1.0);
  EXPECT_TRUE( es->HasRespiratoryExchangeRatio());
  EXPECT_EQ(1.0, es->GetRespiratoryExchangeRatio().GetValue());

  const biogears::SETissueSystem ces(logger);
  EXPECT_FALSE( ces.HasRespiratoryExchangeRatio());
  auto dvalue = ces.GetRespiratoryExchangeRatio();
  EXPECT_NE(dvalue, dvalue);
}
