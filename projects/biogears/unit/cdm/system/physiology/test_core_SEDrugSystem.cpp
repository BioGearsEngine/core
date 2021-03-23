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
#include <biogears/cdm/system/physiology/SEDrugSystem.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

#ifdef DISABLE_BIOGEARS_SEDrugSystem_TEST
#define TEST_FIXTURE_NAME DISABLED_SEDrugSystem_Fixture
#else
#define TEST_FIXTURE_NAME SEDrugSystem_Fixture
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
  biogears::SEDrugSystem* ds;
};

void TEST_FIXTURE_NAME::SetUp()
{
  logger = new biogears::Logger;
  ds = new biogears::SEDrugSystem(logger);
}

void TEST_FIXTURE_NAME::TearDown()
{
  delete ds;
  delete logger;
}
TEST_F(TEST_FIXTURE_NAME, BronchodilationLevel)
{
  EXPECT_FALSE(ds->HasBronchodilationLevel());
  ds->GetBronchodilationLevel().SetValue(1.0);
  EXPECT_TRUE(ds->HasBronchodilationLevel());
  EXPECT_EQ(1.0,ds->GetBronchodilationLevel().GetValue());

  const biogears::SEDrugSystem cds(logger);
  EXPECT_FALSE( cds.HasBronchodilationLevel());
  auto dvalue = cds.GetBronchodilationLevel();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, HeartRateChange)
{
  EXPECT_FALSE( ds->HasHeartRateChange());
  ds->GetHeartRateChange().SetValue(1.0,FrequencyUnit::Hz);
  EXPECT_TRUE( ds->HasHeartRateChange());
  EXPECT_EQ(1.0, ds->GetHeartRateChange(FrequencyUnit::Hz));

  const biogears::SEDrugSystem cds(logger);
  EXPECT_FALSE( cds.HasHeartRateChange());
  auto dvalue = cds.GetHeartRateChange(FrequencyUnit::Hz);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, MeanBloodPressureChange)
{
  EXPECT_FALSE( ds->HasMeanBloodPressureChange());
  ds->GetMeanBloodPressureChange().SetValue(1.0,PressureUnit::Pa);
  EXPECT_TRUE( ds->HasMeanBloodPressureChange());
  EXPECT_EQ(1.0, ds->GetMeanBloodPressureChange(PressureUnit::Pa));

  const biogears::SEDrugSystem cds(logger);
  EXPECT_FALSE( cds.HasMeanBloodPressureChange());
  auto dvalue = cds.GetMeanBloodPressureChange(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, NeuromuscularBlockLevel)
{
  EXPECT_FALSE( ds->HasNeuromuscularBlockLevel());
  ds->GetNeuromuscularBlockLevel().SetValue(1.0);
  EXPECT_TRUE( ds->HasNeuromuscularBlockLevel());
  EXPECT_EQ(1.0, ds->GetNeuromuscularBlockLevel().GetValue());

  const biogears::SEDrugSystem cds(logger);
  EXPECT_FALSE( cds.HasNeuromuscularBlockLevel());
  auto dvalue = cds.GetNeuromuscularBlockLevel();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, PulsePressureChange)
{
  EXPECT_FALSE( ds->HasPulsePressureChange());
  ds->GetPulsePressureChange().SetValue(1.0, PressureUnit::Pa);
  EXPECT_TRUE( ds->HasPulsePressureChange());
  EXPECT_EQ(1.0, ds->GetPulsePressureChange(PressureUnit::Pa));

  const biogears::SEDrugSystem cds(logger);
  EXPECT_FALSE( cds.HasPulsePressureChange());
  auto dvalue = cds.GetPulsePressureChange(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RespirationRateChange)
{
  EXPECT_FALSE( ds->HasRespirationRateChange());
  ds->GetRespirationRateChange().SetValue(1.0, FrequencyUnit::Hz);
  EXPECT_TRUE( ds->HasRespirationRateChange());
  EXPECT_EQ(1.0, ds->GetRespirationRateChange(FrequencyUnit::Hz));

  const biogears::SEDrugSystem cds(logger);
  EXPECT_FALSE( cds.HasRespirationRateChange());
  auto dvalue = cds.GetRespirationRateChange(FrequencyUnit::Hz);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, SedationLevel)
{
  EXPECT_FALSE( ds->HasSedationLevel());
  ds->GetSedationLevel().SetValue(1.0);
  EXPECT_TRUE( ds->HasSedationLevel());
  EXPECT_EQ(1.0, ds->GetSedationLevel().GetValue());

  const biogears::SEDrugSystem cds(logger);
  EXPECT_FALSE( cds.HasSedationLevel());
  auto dvalue = cds.GetSedationLevel();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, TidalVolumeChange)
{
  EXPECT_FALSE( ds->HasTidalVolumeChange());
  ds->GetTidalVolumeChange().SetValue(1.0,VolumeUnit::L);
  EXPECT_TRUE( ds->HasTidalVolumeChange());
  EXPECT_EQ(1.0, ds->GetTidalVolumeChange(VolumeUnit::L));

  const biogears::SEDrugSystem cds(logger);
  EXPECT_FALSE( cds.HasTidalVolumeChange());
  auto dvalue = cds.GetTidalVolumeChange(VolumeUnit::L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, TubularPermeabilityChange)
{
  EXPECT_FALSE( ds->HasTubularPermeabilityChange());
  ds->GetTubularPermeabilityChange().SetValue(1.0);
  EXPECT_TRUE( ds->HasTubularPermeabilityChange());
  EXPECT_EQ(1.0, ds->GetTubularPermeabilityChange().GetValue());

  const biogears::SEDrugSystem cds(logger);
  EXPECT_FALSE( cds.HasTubularPermeabilityChange());
  auto dvalue = cds.GetTubularPermeabilityChange();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, CentralNervousResponse)
{
  EXPECT_FALSE( ds->HasCentralNervousResponse());
  ds->GetCentralNervousResponse().SetValue(1.0);
  EXPECT_TRUE( ds->HasCentralNervousResponse());
  EXPECT_EQ(1.0, ds->GetCentralNervousResponse().GetValue());

  const biogears::SEDrugSystem cds(logger);
  EXPECT_FALSE( cds.HasCentralNervousResponse());
  auto dvalue = cds.GetCentralNervousResponse();
  EXPECT_NE(dvalue, dvalue);
}
