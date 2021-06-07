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
//! @author Nathan Tatum
//! @date   2021 June 3rd
//!
//! Unit Test for Biogears-common Config
//!
#include <limits>
#include <thread>

#include <gtest/gtest.h>

#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/patient/actions/SEBurnWound.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

#ifdef DISABLE_BIOGEARS_SEBurnWound_TEST
#define TEST_FIXTURE_NAME DISABLED_SEBurnWound_Fixture
#else
#define TEST_FIXTURE_NAME SEBurnWound_Fixture
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
  biogears::SEBurnWound* es;
};

void TEST_FIXTURE_NAME::SetUp()
{

}

void TEST_FIXTURE_NAME::TearDown()
{

}

TEST_F(TEST_FIXTURE_NAME, TBSA)
{
  auto action1 = SEBurnWound();
  auto action2 = SEBurnWound();
  auto action3 = SEBurnWound();
  auto action4 = SEBurnWound();

  EXPECT_FALSE(action1.HasTotalBodySurfaceArea());
  EXPECT_FALSE(action2.HasTotalBodySurfaceArea());
  EXPECT_FALSE(action3.HasTotalBodySurfaceArea());
  EXPECT_FALSE(action4.HasTotalBodySurfaceArea());

  action1.GetTotalBodySurfaceArea().SetValue(0.2); // Proper Range
  action2.GetTotalBodySurfaceArea().SetValue(0); // Includes end values in range

  EXPECT_EQ(action1.GetTotalBodySurfaceArea().GetValue(), 0.2);
  EXPECT_EQ(action2.GetTotalBodySurfaceArea().GetValue(), 0.0);

  EXPECT_TRUE(action1.HasTotalBodySurfaceArea());
  EXPECT_TRUE(action2.HasTotalBodySurfaceArea());

  EXPECT_THROW(action1.GetTotalBodySurfaceArea().SetValue(-0.2), biogears::CommonDataModelException); // Too low
  EXPECT_THROW(action2.GetTotalBodySurfaceArea().SetValue(1.2), biogears::CommonDataModelException); // Top High

}

TEST_F(TEST_FIXTURE_NAME, Inflammation)
{
  auto action1 = SEBurnWound();
  auto action2 = SEBurnWound();

  EXPECT_FALSE(action1.HasInflammation());
  EXPECT_FALSE(action2.HasInflammation());

  action1.SetInflammation(true);
  action2.SetInflammation(false);

  EXPECT_TRUE(action1.HasInflammation());
  EXPECT_FALSE(action2.HasInflammation());
}

TEST_F(TEST_FIXTURE_NAME, Compartments)
{
  auto action1 = SEBurnWound();
  auto action2 = SEBurnWound();
  auto action3 = SEBurnWound();
  
  EXPECT_FALSE(action1.HasCompartment());
  EXPECT_FALSE(action2.HasCompartment());
  EXPECT_FALSE(action3.HasCompartment());

  action1.SetCompartment("Trunk");
  action2.SetCompartment("Trunk");
  action2.SetCompartment("RightArm"); //Multiple compartments
  action3.SetCompartment("");

  EXPECT_TRUE(action1.HasCompartment("Trunk"));
  EXPECT_TRUE(action2.HasCompartment("Trunk"));
  EXPECT_FALSE(action3.HasCompartment("Trunk"));

  EXPECT_EQ(action1.GetCompartments()[0], "Trunk");
  EXPECT_EQ(action2.GetCompartments()[0], "Trunk");
  EXPECT_EQ(action2.GetCompartments()[1], "RightArm");
}

TEST_F(TEST_FIXTURE_NAME, Comment)
{
  auto action1 = SEBurnWound();
  auto action2 = SEBurnWound();

  EXPECT_FALSE(action1.HasComment());
  EXPECT_FALSE(action2.HasComment());

  action1.SetComment("20% Burn on the patient's trunk");
  action2.SetComment("");

  EXPECT_TRUE(action1.HasComment());
  EXPECT_FALSE(action2.HasComment());

  EXPECT_EQ(action1.GetComment(), std::string("20% Burn on the patient's trunk"));
  EXPECT_EQ(action2.GetComment(), std::string(""));
}
