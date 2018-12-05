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

#include <biogears/cdm/utils/unitconversion/CompoundUnit.h>
#include <biogears/cdm/properties/SEScalarMass.h>

#ifdef DISABLE_BIOGEARS_CompoundUnit_TEST
#define TEST_FIXTURE_NAME DISABLED_CompoundUnit_Fixture
#else
#define TEST_FIXTURE_NAME CompoundUnit_Fixture
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

TEST_F(TEST_FIXTURE_NAME, OperatorMultiplicationAssignment)
{
  biogears::MassUnit unit1 = biogears::MassUnit("g");
  biogears::MassUnit unit2 = biogears::MassUnit("ug");
  unit1 *= unit2;
  biogears::MassUnit unit3 = biogears::MassUnit("g");
  biogears::MassUnit unit4 = biogears::MassUnit("ug");
  unit4 *= unit3;
  EXPECT_EQ(unit1,unit4); // testing commutative property
  biogears::MassUnit unit5  = biogears::MassUnit("g");
  biogears::MassUnit unit6  = biogears::MassUnit("ug");
  biogears::MassUnit unit7  = biogears::MassUnit("mg");
  biogears::MassUnit unit8  = biogears::MassUnit("g");
  biogears::MassUnit unit9  = biogears::MassUnit("ug");
  biogears::MassUnit unit10 = biogears::MassUnit("mg");
  unit5  *= unit6;
  unit5  *= unit7;
  unit10 *= unit9;
  unit10 *= unit8;
  EXPECT_EQ(unit5,unit10);
}

TEST_F(TEST_FIXTURE_NAME, OperatorDivisionAssignment)
{
  biogears::MassUnit unit5 = biogears::MassUnit("g");
  biogears::MassUnit unit6 = biogears::MassUnit("ug");
  biogears::MassUnit unit7 = biogears::MassUnit("mg");
  biogears::MassUnit unit8 = biogears::MassUnit("g");
  biogears::MassUnit unit9 = biogears::MassUnit("ug");
  biogears::MassUnit unit10 = biogears::MassUnit("mg");
  unit7 /= unit5;
  unit10 /= unit8;
  EXPECT_EQ(unit7, unit10);
}

TEST_F(TEST_FIXTURE_NAME, Raise)
{
  biogears::MassUnit unit1 = biogears::MassUnit("g");
  biogears::MassUnit unit2 = biogears::MassUnit("g");
  unit1.Raise(1.0);
  EXPECT_EQ(unit1,unit2);

}