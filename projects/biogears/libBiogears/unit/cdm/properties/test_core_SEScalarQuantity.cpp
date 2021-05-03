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
#include <biogears/cdm/utils/unitconversion/CompoundUnit.h>
#include "biogears/cdm/properties/SEScalarMass.h"

#ifdef DISABLE_BIOGEARS_SEScalarQuantity_TEST
#define TEST_FIXTURE_NAME DISABLED_SEScalarQuantity_Fixture
#else
#define TEST_FIXTURE_NAME SEScalarQuantity_Fixture
#endif

using namespace biogears;

  class TestUnit : public CCompoundUnit {
  public:
    static const TestUnit ug;
    static const TestUnit mg;
    static const TestUnit g;
    static const TestUnit kg;
    static const TestUnit lb;

    TestUnit(const std::string& u)
      : CCompoundUnit(u)
    {
    }
    virtual ~TestUnit() = default;
      
    static bool IsValidUnit(const char* unit) {
      if (strcmp(g.GetString(),unit) == 0)
        return true;
      if (strcmp(ug.GetString(),unit) == 0)
        return true;
      if (strcmp(mg.GetString(),unit) == 0)
        return true;
      if (strcmp(kg.GetString(),unit) == 0)
        return true;
      if (strcmp(lb.GetString(),unit) == 0)
        return true;
      return false;
    }

    static bool IsValidUnit(const std::string& unit) {
      return IsValidUnit(unit.c_str());
    }

    static const TestUnit& GetCompoundUnit(const char* unit)
    {
      if (strcmp(g.GetString(),unit) == 0)
        return g;
      if (strcmp(ug.GetString(),unit) == 0)
        return ug;
      if (strcmp(mg.GetString(),unit) == 0)
        return mg;
      if (strcmp(kg.GetString(),unit) == 0)
        return kg;
      if (strcmp(lb.GetString(),unit) == 0)
        return lb;
      std::stringstream err;
      err << unit << " is not a valid Mass unit";
      throw CommonDataModelException(err.str());
    }

    static const TestUnit& GetCompoundUnit(const std::string& unit)
    {
      return GetCompoundUnit(unit.c_str());
    }
  };

  const TestUnit TestUnit::ug = TestUnit{ "ug" };
  const TestUnit TestUnit::mg = TestUnit{ "mg" };
  const TestUnit TestUnit::g = TestUnit{ "g" };
  const TestUnit TestUnit::kg = TestUnit{ "kg" };
  const TestUnit TestUnit::lb = TestUnit{ "lb" };

  class TestScalarQuantity : public SEScalarQuantity<TestUnit> {
  public:
    TestScalarQuantity() = default;
    TestScalarQuantity(double v, const std::string& u)
      : SEScalarQuantity(v, TestUnit{u})
    {}
    TestScalarQuantity(double v, const TestUnit& u)
      : SEScalarQuantity(v, u)
    {}

    virtual ~TestScalarQuantity() = default;

    CDM::ScalarMassData* Unload() const override
    {
      if (!IsValid())
        return nullptr;
      CDM::ScalarMassData* data(new CDM::ScalarMassData());
      SEScalarQuantity::Unload(*data);
      return data;
    }
  };


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

  TestUnit lbs{ "lb" };

  TestScalarQuantity v_1{ 1, TestUnit::g };
  TestScalarQuantity v_2{ 1, TestUnit::kg };

  TestScalarQuantity v_3{ 1, lbs };
  TestScalarQuantity v_4{ 1, lbs };
};

void TEST_FIXTURE_NAME::SetUp()
{
}

void TEST_FIXTURE_NAME::TearDown()
{
}


TEST_F(TEST_FIXTURE_NAME, Constructor)
{
  TestScalarQuantity tq_1{ 1, TestUnit::g };
  TestScalarQuantity tq_2{ 1, TestUnit::kg };

  //LIFE TIME OF UNIT Must out live the UnitScalar
  auto lbs = TestUnit{ "lb" };
  TestScalarQuantity tq_3{ 1,lbs};

}

TEST_F(TEST_FIXTURE_NAME, Value_and_Conversion)
{
  EXPECT_EQ(1, v_1.GetValue(TestUnit::g));
  EXPECT_EQ(.001, v_1.GetValue(TestUnit::kg));

  EXPECT_EQ( 1000, v_2.GetValue(TestUnit::g));
  EXPECT_EQ( 1, v_2.GetValue(TestUnit::kg));

  EXPECT_NEAR(453.592, v_3.GetValue(TestUnit::g),.01);
  EXPECT_EQ(1, v_3.GetValue(TestUnit::lb));
}

TEST_F(TEST_FIXTURE_NAME, Validity)
{
  TestScalarQuantity tq_1;
  EXPECT_FALSE(tq_1.IsValid());
  tq_1.SetValue( 1.0, TestUnit::kg);
  EXPECT_TRUE(tq_1.IsValid());
  tq_1.SetValue(std::numeric_limits<double>::infinity(), TestUnit::kg);
  EXPECT_TRUE(tq_1.IsValid());
  tq_1.SetValue(-1 * std::numeric_limits<double>::infinity(), TestUnit::kg);
  EXPECT_TRUE(tq_1.IsValid());
  tq_1.SetValue(std::strtod("NAN",nullptr), TestUnit::kg);
  EXPECT_FALSE(tq_1.IsValid());
}

TEST_F(TEST_FIXTURE_NAME, Copy)
{

  v_1.SetValue(3.0, TestUnit::kg);
  v_2.SetValue(2.0, TestUnit::g);

  EXPECT_EQ(v_1.GetValue(TestUnit::kg), 3.0);
  EXPECT_EQ(v_2.GetValue(TestUnit::g), 2.0);

  v_1.Copy(v_2);

  EXPECT_EQ(v_1, v_2);
  EXPECT_EQ(v_1.GetValue(TestUnit::g), 2.0);
}


TEST_F(TEST_FIXTURE_NAME, IsPositive)
{

  v_1.SetValue(3.0, TestUnit::kg);
  v_2.SetValue(-2.0, TestUnit::kg);
  v_3.SetValue(std::strtod("NAN", nullptr), TestUnit::kg);

  EXPECT_TRUE(v_1.IsPositive());
  EXPECT_FALSE(v_2.IsPositive());
  EXPECT_FALSE(v_3.IsPositive());
}

TEST_F(TEST_FIXTURE_NAME, IsNegative)
{
  v_1.SetValue(3.0, TestUnit::kg);
  v_2.SetValue(-2.0, TestUnit::kg);
  v_3.SetValue(std::strtod("NAN", nullptr), TestUnit::kg);

  EXPECT_FALSE(v_1.IsNegative());
  EXPECT_TRUE(v_2.IsNegative());
  EXPECT_FALSE(v_3.IsNegative());
}

TEST_F(TEST_FIXTURE_NAME, IsZero)
{
  v_1.SetValue(3.0, TestUnit::kg);
  v_2.SetValue(0.0, TestUnit::kg);
  v_3.SetValue( std::strtod("NAN", nullptr), TestUnit::kg);

  EXPECT_FALSE(v_1.IsZero());
  EXPECT_TRUE(v_2.IsZero());
  EXPECT_FALSE(v_3.IsZero());
}


TEST_F(TEST_FIXTURE_NAME, ReadOnly)
{
  v_1.SetValue(3.0, TestUnit::g);
  v_2.SetValue(0., TestUnit::g);
  v_1.SetReadOnly(true);
  v_2.SetReadOnly(false);

  EXPECT_TRUE(v_1.IsReadOnly());
  EXPECT_FALSE(v_2.IsReadOnly());

#if defined(BIOGEARS_THROW_READONLY_EXCEPTIONS)
  EXPECT_ANY_THROW(v_1.SetValue(5.5, TestUnit::g));
  EXPECT_NO_THROW(v_2.SetValue(5.5, TestUnit::g));

  v_2.SetValue(45., TestUnit::g);
  EXPECT_ANY_THROW(v_1.Copy(v_2));
  EXPECT_NE(v_1, v_2);
  EXPECT_NO_THROW(v_2.Copy(v_1));
  EXPECT_EQ(v_1, v_2);

  EXPECT_TRUE(v_1.IsValid());
  EXPECT_TRUE(v_2.IsValid());

  EXPECT_ANY_THROW(v_1.Invalidate());
  EXPECT_NO_THROW(v_2.Invalidate());

  EXPECT_TRUE(v_1.IsValid());
  EXPECT_FALSE(v_2.IsValid());
#else
  EXPECT_FALSE(v_1.Set(v_2));
  EXPECT_TRUE(v_2.Set(v_1));

  v_2.SetValue(45., TestUnit::g);
  v_1.Copy(v_2);
  EXPECT_NE(v_1, v_2);

  v_2.Copy(v_1);
  EXPECT_EQ(v_1, v_2);

  EXPECT_TRUE(v_1.IsValid());
  EXPECT_TRUE(v_2.IsValid());

  v_1.Invalidate();
  v_2.Invalidate();

  EXPECT_TRUE(v_1.IsValid());
  EXPECT_FALSE(v_2.IsValid());
#endif
}

TEST_F(TEST_FIXTURE_NAME, Incrament)
{

  v_1.SetValue(3.0, TestUnit::g);
  v_2.SetValue(0., TestUnit::g);
  v_3.SetValue(10., TestUnit::g);

  v_2.Increment(v_1);
  EXPECT_EQ(v_2, TestScalarQuantity(3.0, TestUnit::g));
  v_2.IncrementValue(v_3.GetValue(TestUnit::g), TestUnit::g);
  EXPECT_EQ(v_2, TestScalarQuantity(13.0,TestUnit::g));
  v_2 += v_2;
  EXPECT_EQ(v_2, TestScalarQuantity(26.0, TestUnit::g));

  EXPECT_EQ(v_1 + v_2 + v_3, TestScalarQuantity(13.0 + 26.0, TestUnit::g));
  EXPECT_EQ(v_1 + TestScalarQuantity( 10.0,TestUnit::g ) +v_2, TestScalarQuantity(13.0 + 26.0, TestUnit::g));

  EXPECT_EQ(v_1, TestScalarQuantity(3.0, TestUnit::g));
  EXPECT_EQ(v_2, TestScalarQuantity(26.0, TestUnit::g));
  EXPECT_EQ(v_3, TestScalarQuantity(10.0, TestUnit::g));

}
TEST_F(TEST_FIXTURE_NAME, Decrement)
{
  v_1.SetValue(3.0, TestUnit::g);
  v_2.SetValue(0., TestUnit::g);
  v_3.SetValue(10., TestUnit::g);

  v_2.Decrement(v_1);
  EXPECT_EQ(v_2, TestScalarQuantity(-3.0, TestUnit::g) );
  v_2.DecrementValue(v_3.GetValue(TestUnit::g), TestUnit::g);
  EXPECT_EQ(v_2, TestScalarQuantity(-13.0, TestUnit::g));
  v_2 += v_2;
  EXPECT_EQ(v_2, TestScalarQuantity(-26.0, TestUnit::g));
  v_2 -= v_2;
  EXPECT_EQ(v_2, TestScalarQuantity( 0.0, TestUnit::g));


  EXPECT_EQ(v_1 - v_2 - v_3, TestScalarQuantity(3 - 10 - -0, TestUnit::g));
  EXPECT_EQ(v_1 - TestScalarQuantity(10.0, TestUnit::g) -v_2, TestScalarQuantity(3 - 10 - -0, TestUnit::g));

  EXPECT_EQ(v_1, TestScalarQuantity(3.0, TestUnit::g));
  EXPECT_EQ(v_2, TestScalarQuantity(0.0, TestUnit::g));
  EXPECT_EQ(v_3, TestScalarQuantity(10.0, TestUnit::g));
}

TEST_F(TEST_FIXTURE_NAME, Multiply)
{
  biogears::SEScalar v_1;
  biogears::SEScalar v_2;
  biogears::SEScalar v_3;
  biogears::SEScalar v_4;

  v_1.SetValue(0.0);
  v_2.SetValue(1.0);
  v_3.SetValue(2.0);
  v_4.SetValue(100.0);

  v_4.Multiply(v_2);
  EXPECT_EQ(0.0, v_4*v_1);
  EXPECT_EQ(v_4, 100.0);

  v_2.Multiply(v_3);
  EXPECT_EQ(v_2, v_3);
  v_2 *= v_3;
  v_2 *= v_3;
  EXPECT_EQ(v_2, 8.0);

  EXPECT_EQ(1000.0, v_4 * 10);
  EXPECT_EQ(1000.0, 10 * v_4);
}

TEST_F(TEST_FIXTURE_NAME, Divide)
{
  biogears::SEScalar v_1;
  biogears::SEScalar v_2;
  biogears::SEScalar v_3;
  biogears::SEScalar v_4;

  v_1.SetValue(0.0);
  v_2.SetValue(1.0);
  v_3.SetValue(2.0);
  v_4.SetValue(100.0);

  v_4.Divide(v_3);
  EXPECT_EQ(50.0, v_4 / v_2);
  EXPECT_EQ(v_4, 50.0);

  v_2.Divide(v_3);
  EXPECT_EQ(0.5, v_2);
  v_2 /= v_3;
  v_2 /= v_3;
  EXPECT_NEAR(v_2.GetValue(), 0.125, 0.0001);

  EXPECT_EQ(5.0, v_4 / 10);
  EXPECT_EQ(0.2, 10 / v_4);
  EXPECT_TRUE(!std::isnormal((v_4 / v_1).GetValue()));
}

TEST_F(TEST_FIXTURE_NAME, Boolean_Operators)
{
  v_1.SetValue( 0.0, TestUnit::g);
  v_2.SetValue( 1.0, TestUnit::g);
  v_3.SetValue( 2.0, TestUnit::g);
  v_4.SetValue( 4.0, TestUnit::g);

  EXPECT_TRUE(v_1 == v_1);
  EXPECT_TRUE(v_1 != v_2);
  EXPECT_TRUE(v_1 < v_2);
  EXPECT_TRUE(v_2 <= v_3);
  EXPECT_TRUE(v_4 > v_3);
  EXPECT_TRUE(v_3 >= v_2);
}

TEST_F(TEST_FIXTURE_NAME, Nan)
{
  biogears::SEScalar v_1;

  EXPECT_TRUE(std::isnan(v_1.dNaN()));
  EXPECT_NE(v_1.NaN, v_1.NaN);
}