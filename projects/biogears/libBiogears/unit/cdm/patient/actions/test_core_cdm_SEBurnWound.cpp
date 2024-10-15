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
#include <algorithm>
#include <numeric>

#include <gtest/gtest.h>

#include <biogears/cdm/patient/actions/SEBurnWound.h>
#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/properties/SEProperties.h>

#define TEST_FIXTURE_NAME SEBurnWound_Fixture

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

  action1.AddCompartment("Trunk");
  action1.SetTotalBodySurfaceArea(0.2); // Proper Range
  action2.SetTotalBodySurfaceArea(0); // Includes end values in range

  EXPECT_EQ(action1.GetTotalBodySurfaceArea(), 0.2);
  EXPECT_EQ(action2.GetTotalBodySurfaceArea(), 0.0);

  EXPECT_TRUE(action1.HasTotalBodySurfaceArea());
  EXPECT_TRUE(action2.HasTotalBodySurfaceArea());

  EXPECT_THROW(action1.SetTotalBodySurfaceArea(-0.2), biogears::CommonDataModelException); // Too low
  EXPECT_NO_THROW(action1.SetTotalBodySurfaceArea(1.2)); // We CLAMP our input to allowable
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

  action1.AddCompartment("Trunk");
  action2.AddCompartment("Trunk");
  action2.AddCompartment("RightArm"); // Multiple compartments
  action3.AddCompartment("");

  EXPECT_TRUE(action1.HasCompartment("Trunk"));
  EXPECT_TRUE(action2.HasCompartment("Trunk"));
  EXPECT_FALSE(action3.HasCompartment("Trunk"));

  EXPECT_EQ(action1.GetCompartments()[0], "Trunk");
  EXPECT_EQ(action2.GetCompartments()[0], "Trunk");
  EXPECT_EQ(action2.GetCompartments()[1], "RightArm");
}

TEST_F(TEST_FIXTURE_NAME, tbsa_distribution)
{
  auto action1 = SEBurnWound();
  auto action2 = SEBurnWound();
  auto action3 = SEBurnWound();

  action1.AddCompartment("Trunk");
  action1.AddCompartment("LeftArm");
  action1.AddCompartment("RightArm");
  action1.AddCompartment("LeftLeg");
  action1.AddCompartment("RightLeg");

  action2.AddCompartment("Trunk");
  action2.AddCompartment("RightArm"); // Multiple compartments

  action3.AddCompartment("LeftArm");
  action3.AddCompartment("RightArm");
  action3.AddCompartment("LeftLeg");
  action3.AddCompartment("RightLeg");

  action1.SetTotalBodySurfaceArea(.99);
  action2.SetTotalBodySurfaceArea(.99);
  action3.SetTotalBodySurfaceArea(.99);

  EXPECT_EQ(action1.GetTotalBodySurfaceArea(), MAXIMUM_TRUNK + MAXIMUM_LEFT_ARM + MAXIMUM_LEFT_LEG + +MAXIMUM_RIGHT_ARM + MAXIMUM_RIGHT_LEG);
  EXPECT_EQ(action2.GetTotalBodySurfaceArea(), MAXIMUM_TRUNK + MAXIMUM_RIGHT_ARM);
  EXPECT_EQ(action3.GetTotalBodySurfaceArea(), MAXIMUM_LEFT_ARM + MAXIMUM_LEFT_LEG + +MAXIMUM_RIGHT_ARM + MAXIMUM_RIGHT_LEG);

  action1.SetTotalBodySurfaceArea(.75);
  action2.SetTotalBodySurfaceArea(.27);
  action3.SetTotalBodySurfaceArea(MAXIMUM_LEFT_LEG + +MAXIMUM_RIGHT_ARM + MAXIMUM_RIGHT_LEG);

  auto dist1 = action1.GetTBSACompartmentDistribution();
  auto dist2 = action2.GetTBSACompartmentDistribution();
  auto dist3 = action3.GetTBSACompartmentDistribution();

  EXPECT_NEAR(dist1[0], MAXIMUM_LEFT_ARM, 0.0001);
  EXPECT_NEAR(dist1[1], MAXIMUM_RIGHT_ARM, 0.0001);
  EXPECT_NEAR(dist1[2], .21, 0.0001);
  EXPECT_NEAR(dist1[3], MAXIMUM_LEFT_LEG, 0.0001);
  EXPECT_NEAR(dist1[4], MAXIMUM_RIGHT_LEG, 0.0001);
  EXPECT_NEAR(std::accumulate(dist1.begin(), dist1.end(), 0.), action1.GetTotalBodySurfaceArea(), 0.001);
  
  EXPECT_NEAR(dist2[0], 0., 0.0001);
  EXPECT_NEAR(dist2[1], MAXIMUM_RIGHT_ARM, 0.0001);
  EXPECT_NEAR(dist2[2], .18, 0.0001);
  EXPECT_NEAR(dist2[3], 0., 0.0001);
  EXPECT_NEAR(dist2[4], 0., 0.0001);
  EXPECT_NEAR(std::accumulate(dist2.begin(), dist2.end(), 0.), action2.GetTotalBodySurfaceArea(), 0.001);

  EXPECT_NEAR(dist3[0], MAXIMUM_RIGHT_ARM, 0.0001);
  EXPECT_NEAR(dist3[1], MAXIMUM_RIGHT_ARM, 0.0001);
  EXPECT_NEAR(dist3[2], 0., 0.0001);
  EXPECT_NEAR(dist3[3], .135, 0.0001);
  EXPECT_NEAR(dist3[4], .135, 0.0001);
  EXPECT_NEAR(std::accumulate(dist3.begin(), dist3.end(), 0.), action3.GetTotalBodySurfaceArea(), 0.001);

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
