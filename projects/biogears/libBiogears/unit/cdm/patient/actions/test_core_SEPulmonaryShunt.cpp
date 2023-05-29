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
//! @author Austin Baird
//! @date   2021 June 10th 
//!
//! Unit Test for Biogears-common Config
//!
#include <limits>
#include <thread>

#include <gtest/gtest.h>

#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/patient/actions/SEPulmonaryShunt.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

#ifdef DISABLE_BIOGEARS_SEPulmonaryShunt_TEST
#define TEST_FIXTURE_NAME DISABLED_SEPulmonaryShunt_Fixture
#else
#define TEST_FIXTURE_NAME SEPulmonaryShunt_Fixture
#endif

using namespace biogears;
// The fixture for testing class SEPulmonaryShunt.
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
  BIOGEARS_NAMESPACE Logger* logger;
  BIOGEARS_NAMESPACE SEPulmonaryShunt* es;
};

void TEST_FIXTURE_NAME::SetUp()
{

}

void TEST_FIXTURE_NAME::TearDown()
{

}

TEST_F(TEST_FIXTURE_NAME, TBSA)
{
  auto action1 = SEPulmonaryShunt();
  auto action2 = SEPulmonaryShunt();
  auto action3 = SEPulmonaryShunt();
  auto action4 = SEPulmonaryShunt();

  EXPECT_FALSE(action1.HasFlowRateScale());
  EXPECT_FALSE(action2.HasFlowRateScale());
  EXPECT_FALSE(action3.HasFlowRateScale());
  EXPECT_FALSE(action4.HasFlowRateScale());

  action1.GetFlowRateScale().SetValue(0.2); // Proper Range
  action2.GetFlowRateScale().SetValue(0); // Includes end values in range

  EXPECT_EQ(action1.GetFlowRateScale().GetValue(), 0.2);
  EXPECT_EQ(action2.GetFlowRateScale().GetValue(), 0.0);

  EXPECT_TRUE(action1.HasFlowRateScale());
  EXPECT_TRUE(action2.HasFlowRateScale());

  EXPECT_THROW(action1.GetFlowRateScale().SetValue(-0.2), BIOGEARS_NAMESPACE CommonDataModelException); // Too low
  EXPECT_THROW(action2.GetFlowRateScale().SetValue(1.2), BIOGEARS_NAMESPACE CommonDataModelException); // Top High

}


TEST_F(TEST_FIXTURE_NAME, Comment)
{
  auto action1 = SEPulmonaryShunt();
  auto action2 = SEPulmonaryShunt();

  EXPECT_FALSE(action1.HasComment());
  EXPECT_FALSE(action2.HasComment());

  action1.SetComment("Pulmonary shunt executed");
  action2.SetComment("");

  EXPECT_TRUE(action1.HasComment());
  EXPECT_FALSE(action2.HasComment());

  EXPECT_EQ(action1.GetComment(), std::string("Pulmonary shunt executed"));
  EXPECT_EQ(action2.GetComment(), std::string(""));
}
