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
#include <biogears/cdm/patient/actions/SEEscharotomy.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

#ifdef DISABLE_BIOGEARS_SEEscharotomy_TEST
#define TEST_FIXTURE_NAME DISABLED_SEEscharotomy_Fixture
#else
#define TEST_FIXTURE_NAME SEEscharotomy_Fixture
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
  biogears::SEEscharotomy* es;
};

void TEST_FIXTURE_NAME::SetUp()
{

}

void TEST_FIXTURE_NAME::TearDown()
{

}

TEST_F(TEST_FIXTURE_NAME, Location)
{
  auto action1 = SEEscharotomy();
  auto action2 = SEEscharotomy();

  EXPECT_FALSE(action1.HasLocation());
  EXPECT_FALSE(action2.HasLocation());

  action1.SetLocation("RightLeg");
  action2.SetLocation("RightPupil");

  EXPECT_TRUE(action1.HasLocation());
  EXPECT_TRUE(action2.HasLocation());

  EXPECT_EQ(action1.IsValid(), true);
  EXPECT_EQ(action2.IsValid(), false);

  action2.InvalidateLocation();

  EXPECT_FALSE(action2.HasLocation());

  EXPECT_EQ(action1.GetLocation(), "RightLeg");
  EXPECT_EQ(action2.GetLocation(), "");
}

TEST_F(TEST_FIXTURE_NAME, Comment)
{
  auto action1 = SEEscharotomy();
  auto action2 = SEEscharotomy();

  EXPECT_FALSE(action1.HasComment());
  EXPECT_FALSE(action2.HasComment());

  action1.SetComment("RightLeg Escharotomy");
  action2.SetComment("");

  EXPECT_TRUE(action1.HasComment());
  EXPECT_FALSE(action2.HasComment());

  EXPECT_EQ(action1.GetComment(), std::string("RightLeg Escharotomy"));
  EXPECT_EQ(action2.GetComment(), std::string(""));
}