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
#include <thread>

#include <gtest/gtest.h>

#include <biogears/math/angles.h>

#ifdef DISABLE_BIOGEARS_BioGearsScenarioExec_TEST
#define TEST_FIXTURE_NAME DISABLED_BioGearsScenarioExec_Fixture
#else
#define TEST_FIXTURE_NAME BioGearsScenarioExec_Fixture
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

//!
//! Way to much to test so many operations
//! I'm going to test construction for now
//! And we will write a few each day until we get them all
//!
TEST_F(TEST_FIXTURE_NAME, angles)
{
  using namespace biogears;
  using namespace biogears::angle_literals;
  {
    float phi = 3.14159265358979323_f_rad;
    float theta = 180_f_deg;
    EXPECT_NEAR(phi, theta, 0.0001);
  }

  {
    Angle phi = 3.14159265358979323_f_rad;
    Angle theta = 180_f_deg;
    EXPECT_NEAR(phi.in_deg(), theta.in_deg(), 0.0001);
  }
}

TEST_F(TEST_FIXTURE_NAME, angles_quaternains)
{
  using namespace biogears;
  using namespace biogears::angle_literals;

  {
    auto first = Quaternion(1, 1, 1, 1);
    auto second = Quaternion(1, 1, 1, 1);
    EXPECT_EQ(first, second);
  }
}

TEST_F(TEST_FIXTURE_NAME, angles_euler)
{
  using namespace biogears;
  using namespace biogears::angle_literals;

  {
    auto first = EulerAng(1, 1, 1);
    auto second = EulerAng(1, 1, 1);
    EXPECT_EQ(first, second);
  }
}
