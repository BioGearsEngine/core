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
#include <cmath>

#include <gtest/gtest.h>

#include "biogears/cdm/properties/SEScalar.h"
#include <biogears/cdm/properties/SEProperty.h>
#include <biogears/cdm/utils/unitconversion/UCCommon.h>

#ifdef DISABLE_BIOGEARS_SEScalar_TEST
#define TEST_FIXTURE_NAME DISABLED_SEScalar_Fixture
#else
#define TEST_FIXTURE_NAME SEScalar_Fixture
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
  virtual void SetUp() override;

  // Code here will be called immediately after each test (right
  // before the destructor).
  virtual void TearDown() override;
};

void TEST_FIXTURE_NAME::SetUp()
{
}

void TEST_FIXTURE_NAME::TearDown()
{
}

TEST_F(TEST_FIXTURE_NAME, Value)
{
  biogears::SEScalar scalar1;
#ifdef BIOGEARS_THROW_NAN_EXCEPTIONS
  EXPECT_ANY_THROW(EXPECT_TRUE(std::isnan(scalar1.GetValue())));
#elif defined (NDEBUG)
  EXPECT_TRUE(std::isnan(scalar1.GetValue()));
#else
  ASSERT_DEBUG_DEATH(EXPECT_TRUE(std::isnan(scalar1.GetValue())), ".*");
#endif
  scalar1.SetValue(5.0);
  EXPECT_EQ(5.0, scalar1.GetValue());
}

TEST_F(TEST_FIXTURE_NAME, Validity)
{
  biogears::SEScalar scalar1;
#ifdef BIOGEARS_THROW_NAN_EXCEPTIONS
  EXPECT_ANY_THROW(EXPECT_TRUE(std::isnan(scalar1.GetValue())));
#elif defined (NDEBUG)
  EXPECT_TRUE(std::isnan(scalar1.GetValue()));
#else
  ASSERT_DEBUG_DEATH(EXPECT_TRUE(std::isnan(scalar1.GetValue())), ".*");
#endif
  EXPECT_FALSE(scalar1.IsValid());
  scalar1.SetValue(5.0);
  EXPECT_EQ(5.0, scalar1.GetValue());
  EXPECT_TRUE(scalar1.IsValid());
}

TEST_F(TEST_FIXTURE_NAME, Copy)
{
  biogears::SEScalar scalar_1;
  biogears::SEScalar scalar_2;

  scalar_1.SetValue (3.0);
  scalar_2.SetValue (2.0);
  
  EXPECT_EQ(scalar_1 , 3.0);
  EXPECT_EQ(scalar_2, 2.0);

  scalar_1.Copy(scalar_2);

  EXPECT_EQ(scalar_1, scalar_2);
  EXPECT_EQ(scalar_1, 2.0);
}


TEST_F(TEST_FIXTURE_NAME, IsPositive)
{
  biogears::SEScalar scalar_1;
  biogears::SEScalar scalar_2;
  biogears::SEScalar scalar_3;

  scalar_1.SetValue(3.0);
  scalar_2.SetValue(-2.0);

  EXPECT_TRUE(scalar_1.IsPositive());
  EXPECT_FALSE(scalar_2.IsPositive());
  EXPECT_FALSE(scalar_3.IsPositive());
}

TEST_F(TEST_FIXTURE_NAME, IsNegative)
{
  biogears::SEScalar scalar_1;
  biogears::SEScalar scalar_2;
  biogears::SEScalar scalar_3;

  scalar_1.SetValue(3.0);
  scalar_2.SetValue(-2.0);

  EXPECT_FALSE(scalar_1.IsNegative());
  EXPECT_TRUE(scalar_2.IsNegative());
  EXPECT_FALSE(scalar_3.IsNegative());
}

TEST_F(TEST_FIXTURE_NAME, IsZero)
{
  biogears::SEScalar scalar_1;
  biogears::SEScalar scalar_2;
  biogears::SEScalar scalar_3;

  scalar_1.SetValue(3.0);
  scalar_2.SetValue(0);

  EXPECT_FALSE(scalar_1.IsZero());
  EXPECT_TRUE(scalar_2.IsZero());
  EXPECT_FALSE(scalar_3.IsZero());
}



TEST_F(TEST_FIXTURE_NAME, ReadOnly)
{
  biogears::SEScalar scalar_1;
  biogears::SEScalar scalar_2;
  biogears::SEScalar scalar_3;

  scalar_1.SetValue(3.0);
  scalar_2.SetValue(0);
  scalar_1.SetReadOnly(true);
  scalar_2.SetReadOnly(false);

  EXPECT_TRUE(scalar_1.IsReadOnly());
  EXPECT_FALSE(scalar_2.IsReadOnly());

#if defined(BIOGEARS_THROW_READONLY_EXCEPTIONS)
  EXPECT_ANY_THROW(scalar_1.SetValue(5.5));
  EXPECT_NO_THROW(scalar_2.SetValue(5.5));

  scalar_2.SetValue(45.);
  EXPECT_ANY_THROW(scalar_1.Copy(scalar_2));
  EXPECT_NE(scalar_1, scalar_2);
  EXPECT_NO_THROW(scalar_2.Copy(scalar_1));
  EXPECT_EQ(scalar_1, scalar_2);

  EXPECT_TRUE(scalar_1.IsValid());
  EXPECT_TRUE(scalar_2.IsValid());

  EXPECT_ANY_THROW(scalar_1.Invalidate());
  EXPECT_NO_THROW(scalar_2.Invalidate());

  EXPECT_TRUE(scalar_1.IsValid());
  EXPECT_FALSE(scalar_2.IsValid());
#else
  EXPECT_FALSE(scalar_1.Set(scalar_2));
  EXPECT_TRUE(scalar_2.Set(scalar_1));

  scalar_2.SetValue(45.);
  scalar_1.Copy(scalar_2);
  EXPECT_NE(scalar_1, scalar_2);

  scalar_2.Copy(scalar_1);
  EXPECT_EQ(scalar_1, scalar_2);

  EXPECT_TRUE(scalar_1.IsValid());
  EXPECT_TRUE(scalar_2.IsValid());

  scalar_1.Invalidate();
  scalar_2.Invalidate();

  EXPECT_TRUE(scalar_1.IsValid());
  EXPECT_FALSE(scalar_2.IsValid());
#endif
}

TEST_F(TEST_FIXTURE_NAME, Incrament)
{
  biogears::SEScalar scalar_1;
  biogears::SEScalar scalar_2;
  biogears::SEScalar scalar_3;

  scalar_1.SetValue(3.0);
  scalar_2.SetValue(0.);
  scalar_3.SetValue(10.);
  
  scalar_2.Increment(scalar_1);
  EXPECT_EQ(scalar_2, 3.0);
  scalar_2.IncrementValue(scalar_3.GetValue());
  EXPECT_EQ(scalar_2, 13.0);
  scalar_2+=scalar_2;
  EXPECT_EQ(scalar_2, 26.0);

  EXPECT_EQ(scalar_1+scalar_2+scalar_3, 13.0 + 26.0);
  EXPECT_EQ(scalar_1+10.0+scalar_2, 13.0 + 26.0);

  EXPECT_EQ(scalar_1, 3.0);
  EXPECT_EQ(scalar_2, 26.0);
  EXPECT_EQ(scalar_3, 10.0);

}
TEST_F(TEST_FIXTURE_NAME, Decrement)
{
  biogears::SEScalar scalar_1;
  biogears::SEScalar scalar_2;
  biogears::SEScalar scalar_3;

  scalar_1.SetValue(3.0);
  scalar_2.SetValue(0.);
  scalar_3.SetValue(10.);

  scalar_2.Decrement(scalar_1);
  EXPECT_EQ(scalar_2, -3.0);
  scalar_2.DecrementValue(scalar_3.GetValue());
  EXPECT_EQ(scalar_2, -13.0);
  scalar_2+=scalar_2;
  EXPECT_EQ(scalar_2, -26);
  scalar_2 -= scalar_2;
  EXPECT_EQ(scalar_2, 0);


  EXPECT_EQ(scalar_1 - scalar_2 - scalar_3, 3 -0 - 10 );
  EXPECT_EQ(scalar_1 - 10.0 - scalar_2, 3 - 10 - -0);

  EXPECT_EQ(scalar_1, 3.0);
  EXPECT_EQ(scalar_2, 0);
  EXPECT_EQ(scalar_3, 10.0);
}

TEST_F(TEST_FIXTURE_NAME, Multiply)
{
  biogears::SEScalar scalar_1;
  biogears::SEScalar scalar_2;
  biogears::SEScalar scalar_3;
  biogears::SEScalar scalar_4;

  scalar_1.SetValue(0.0);
  scalar_2.SetValue(1.0);
  scalar_3.SetValue(2.0);
  scalar_4.SetValue(100.0);

  scalar_4.Multiply(scalar_2);
  EXPECT_EQ( 0.0, scalar_4*scalar_1);
  EXPECT_EQ(scalar_4, 100.0);

  scalar_2.Multiply(scalar_3);
  EXPECT_EQ(scalar_2, scalar_3);
  scalar_2 *= scalar_3;
  scalar_2 *= scalar_3;
  EXPECT_EQ(scalar_2, 8.0);

  EXPECT_EQ(1000.0, scalar_4 * 10);
  EXPECT_EQ(1000.0, 10 * scalar_4);
}

TEST_F(TEST_FIXTURE_NAME, Divide)
{
  biogears::SEScalar scalar_1;
  biogears::SEScalar scalar_2;
  biogears::SEScalar scalar_3;
  biogears::SEScalar scalar_4;

  scalar_1.SetValue(0.0);
  scalar_2.SetValue(1.0);
  scalar_3.SetValue(2.0);
  scalar_4.SetValue(100.0);

  scalar_4.Divide(scalar_3);
  EXPECT_EQ(50.0, scalar_4/scalar_2);
  EXPECT_EQ(scalar_4, 50.0);

  scalar_2.Divide(scalar_3);
  EXPECT_EQ(0.5, scalar_2);
  scalar_2 /= scalar_3;
  scalar_2 /= scalar_3;
  EXPECT_NEAR(scalar_2.GetValue(), 0.125, 0.0001);

  EXPECT_EQ(5.0, scalar_4 / 10);
  EXPECT_EQ(0.2, 10 / scalar_4);
  EXPECT_TRUE(!std::isnormal((scalar_4 / scalar_1).GetValue()));
}

TEST_F(TEST_FIXTURE_NAME, Boolean_Operators)
{
  biogears::SEScalar scalar_1{ 0.0 };
  biogears::SEScalar scalar_2{ 1.0 };
  biogears::SEScalar scalar_3{ 2.0 };
  biogears::SEScalar scalar_4{ 4.0 };

  EXPECT_TRUE(scalar_1 == scalar_1);
  EXPECT_FALSE(scalar_1 == scalar_2);
  EXPECT_TRUE(scalar_1 != scalar_2);
  EXPECT_FALSE(scalar_1 != scalar_1);

  EXPECT_TRUE(scalar_1 < scalar_2);
  EXPECT_FALSE(scalar_2 < scalar_1);

  EXPECT_TRUE(scalar_2 <= scalar_3);
  EXPECT_FALSE(scalar_3 <= scalar_2);

  EXPECT_TRUE(scalar_4 > scalar_3);
  EXPECT_FALSE(scalar_3 > scalar_4);

  EXPECT_TRUE(scalar_3 >= scalar_2);
  EXPECT_FALSE(scalar_2 >= scalar_3);
}

TEST_F(TEST_FIXTURE_NAME, Nan)
{
  biogears::SEScalar scalar_1;

  EXPECT_TRUE( std::isnan(scalar_1.dNaN()) );
  EXPECT_NE(scalar_1.NaN, scalar_1.NaN);
}

