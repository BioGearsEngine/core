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

#include <io/cdm/PatientNutrition.h>
#include <io/cdm/Property.h>

#ifdef DISABLE_BIOGEARS_PatientNutrition_TEST
#define TEST_FIXTURE_NAME DISABLED_PatientNutritionFixture
#else
#define TEST_FIXTURE_NAME PatientNutritionFixture
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

// class SENutrition;
//!
//! TYPE Nutrition
//! static void Marshall(const CDM::Nutrition& in, SENutrition& out);
//! static void UnMarshall(const SENutrition& in, CDM::Nutrition& out);
#include <biogears/cdm/patient/SENutrition.h>
TEST_F(TEST_FIXTURE_NAME, Nutrition)
{
  USING_TYPES(Nutrition)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.SetName("Not Real nutrition");
  source.GetCarbohydrate().SetValue(0.7777, biogears::MassUnit::g);
  source.GetFat().SetValue(0.6666, biogears::MassUnit::g);
  source.GetProtein().SetValue(0.5555, biogears::MassUnit::g);
  source.GetCalcium().SetValue(0.4444, biogears::MassUnit::g);
  source.GetSodium().SetValue(0.2222, biogears::MassUnit::g);
  source.GetWater().SetValue(0.1111, biogears::VolumeUnit::dL);

  EXPECT_NE(source, sink);

  PatientNutrition::UnMarshall(source, data);
  PatientNutrition::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}