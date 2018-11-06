//! Copyright/' (C) Applied Research Associates - All Rights Reserved
//! Unauthorized copying of this file, via any medium is strictly prohibited
//! Proprietary and confidential
//!
//! This file is subject to the terms and conditions defined in file
//! 'LICENSE.txt', which is part of this source code package

//!
//! @author David Lee
//! @date   2017 Aug 3rd
//!
//! Unit Test for NGSS Config
//!
#include <thread>

#include <gtest/gtest.h>

#include <ara/framework/option.tci.h>
#include <ara/framework/result.tci.h>

#ifdef DISABLE_KALE_Option_TEST
  #define TEST_FIXTURE_NAME DISABLED_Option_Fixture
#else
  #define TEST_FIXTURE_NAME Option_Fixture
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

TEST_F(TEST_FIXTURE_NAME, Option_Construction)
{
  struct range 
  {
    double high;
    double low;
  };
  range struct_value = range{ 1.0, 100.0 };
  double double_value     = 9.9;
  auto doubleOption_value = ara::Option<double>(double_value);
  auto doubleOption_null = ara::Option<double>();

  auto structOption_value = ara::Option<range>(range{5.,5.});
  auto structOption_null = ara::Option<range>();

  auto pointerOption_value = ara::Option<range*>(&struct_value);
  auto pointerOption_null = ara::Option<range*>();

  auto someOption_value = ara::Option<double>::make(1.);

  EXPECT_TRUE( doubleOption_value.is_some() );
  EXPECT_TRUE( doubleOption_null.is_none() );

  EXPECT_TRUE(pointerOption_value.is_some());
  EXPECT_TRUE(pointerOption_null.is_none());

  EXPECT_TRUE(structOption_value.is_some());
  EXPECT_TRUE(structOption_null.is_none());

  EXPECT_TRUE(someOption_value.is_some());
  
}

TEST_F(TEST_FIXTURE_NAME, Option_bool_operator)
{

  struct range
  {
    double high;
    double low;
  };
  range struct_value = range{ 1.0, 100.0 };
  double double_value = 9.9;
  auto doubleOption_value = ara::Option<double>(double_value);
  auto doubleOption_null = ara::Option<double>();

  auto structOption_value = ara::Option<range>(range{ 5.,5. });
  auto structOption_null = ara::Option<range>();

  auto pointerOption_value = ara::Option<range*>(&struct_value);
  auto pointerOption_null = ara::Option<range*>();

  auto someOption_value = ara::Option<double>::make(1.);

  EXPECT_TRUE(doubleOption_value);
  EXPECT_FALSE(doubleOption_null);

  EXPECT_TRUE(pointerOption_value);
  EXPECT_FALSE(pointerOption_null);

  EXPECT_TRUE(structOption_value);
  EXPECT_FALSE(structOption_null);

  EXPECT_TRUE(someOption_value); 

  EXPECT_EQ(doubleOption_value , doubleOption_value);
  EXPECT_NE(doubleOption_value , doubleOption_null);
}

TEST_F(TEST_FIXTURE_NAME, Option_clone_and_assignment)
{

  double double_value = 9.9;
  auto doubleOption_value = ara::Option<double>(double_value);
  auto doubleOption_null = ara::Option<double>();

  auto option2 = doubleOption_value.clone();
  auto option3 = doubleOption_null.clone();

  EXPECT_TRUE(option2);
  EXPECT_FALSE(option3);

  option3 = option2;
  EXPECT_TRUE(option3);

}

TEST_F(TEST_FIXTURE_NAME, Option_unwrap_and_dump)
{

  {
    double double_value = 9.9;
    auto doubleOption_value = ara::Option<double*>(&double_value);
    auto doubleOption_null = ara::Option<double*>();

    auto unwraped_value = doubleOption_value.unwrap();
    EXPECT_EQ(*unwraped_value, double_value);
    ASSERT_THROW(doubleOption_value.expect("Empty option should throw an expection."), std::runtime_error);
    #ifdef _DEBUG
    EXPECT_DEATH(doubleOption_value.unwrap(), "");
    #endif
  }

  {
    struct range {
      double first;
      std::vector<double> second;

      range():first(99),second(9,100){};
      range(double f,std::vector<double> s):first(f),second(s){};
    } value = { 10., {1.,2.,3.,4.,5.} } ;
    
    range or_range{ 1., {0.,0.,0.,0.} };
    auto option = ara::Option<range>(value);

    
    auto r1 = option.unwrap_or(or_range);
    EXPECT_EQ(r1.first, value.first);
    auto r2 = option.unwrap_or(or_range);
    EXPECT_EQ(r2.first, or_range.first);
    auto r3 = option.unwrap_or_default();
    EXPECT_EQ( range().first, r3.first);
    auto r4 = option.unwrap_or_else( [](){ return range{ 2.715, std::vector<double>{3.14,10} }; } );
    EXPECT_EQ(r4.first,  2.715);
  }
}

TEST_F(TEST_FIXTURE_NAME, Option_match)
{
  struct range {
    double first;
    std::vector<double> second;

    range() :first(99), second(9, 100) {};
    range(double f, std::vector<double> s) :first(f), second(s) {};
  } value = { 10., {1.,2.,3.,4.,5.} };
  {
    range or_range = { 1., {0.,0.,0.,0.} };
    const auto option = ara::Option<range>(value);
    const auto none = ara::Option<range>();

    auto r1 = option.match<double>([](const range&) { return 0.; }, []() { return 1.0; });
    EXPECT_EQ(r1, 0.);
    EXPECT_TRUE(option);
    double void_result = 55.;
    option.match_void([&](const range&) { void_result = 100; }, [&]() { void_result = 0; });
    EXPECT_TRUE(option);
    EXPECT_EQ(void_result, 100.);
    none.match_void([&](const range&) { void_result = 100; }, [&]() { void_result = 0; });
    EXPECT_EQ(void_result, .0);
  }

  {
    range or_range = { 1., {0.,0.,0.,0.} };
    auto option = ara::Option<range>(value);
    auto none = ara::Option<range>();

    auto r1 = option.match<double>([](const range&) { return 0.; }, []() { return 1.0; });
    EXPECT_FALSE(option);
    EXPECT_EQ(r1, 0.);
    double void_result = 55.;
    option.match_void([&](const range&) { void_result = 100; }, [&]() { void_result = 0; });
    EXPECT_EQ(void_result, 0.);
    EXPECT_FALSE(none);
    none.emplace(1., std::vector<double>{0.,0.,0.,0.});
    EXPECT_TRUE(none);
    none.match_void([&](const range&) { void_result = 100; }, [&]() { void_result = 0; });
    EXPECT_FALSE(none);
    EXPECT_EQ(void_result, 100.0);
  }
}
TEST_F(TEST_FIXTURE_NAME, Option_map)
{
    struct range {
      double first;
      std::vector<double> second;

      range() :first(99), second(9, 100) {};
      range(double f, std::vector<double> s) :first(f), second(s) {};
    } value = { 10., {1.,2.,3.,4.,5.} };
    {
      range or_range = { 1., {0.,0.,0.,0.} };
      const auto option = ara::Option<range>(value);
      const auto none = ara::Option<range>();

      auto r1 = option.map<double>( [](const range&) { return 0.; } );
      EXPECT_EQ(r1.unwrap(), 0.);
      EXPECT_TRUE(option);
      EXPECT_FALSE(r1);
      double void_result = 55.;
      option.map_void([&](const range&) { void_result = 100.; });
      EXPECT_TRUE(option);
      EXPECT_EQ(void_result, 100.);
      auto map_or_result = option.map_or<double>( 555., [&](const range&) { return 100.; });
      EXPECT_EQ(map_or_result, 100.0);
      EXPECT_TRUE(option);
      auto map_or_else_result = none.map_or_else<double>( [&]() { return 555.; }, [&](const range&) { return 150.; } );
      EXPECT_EQ(map_or_else_result, 555.);
    }

    {
      range or_range = { 1., {0.,0.,0.,0.} };
      auto option = ara::Option<range>(value);
      auto none = ara::Option<range>();

      auto r1 = option.map<double>([](range&&) { return 0.; });
      EXPECT_EQ(r1.unwrap(), 0.);
      EXPECT_FALSE(option);
      EXPECT_FALSE(r1);
      double void_result = 55.;
      option.map_void([&](range&&) { void_result = 100.; });
      EXPECT_FALSE(option);
      EXPECT_EQ(void_result, 55.);
      auto map_or_result = none.map_or<double>(555., [&](range&&) { return 100.; });
      EXPECT_EQ(map_or_result, 555.0);
      auto map_or_else_result = none.map_or_else<double>([&]() { return 0.; }, [&](range&&) { return 150.; });
      EXPECT_EQ(map_or_else_result, 0.0);
    }
}

TEST_F(TEST_FIXTURE_NAME, Option_and_then)
{
  struct range {
    double first;
    std::vector<double> second;

    range() :first(99), second(9, 100) {};
    range(double f, std::vector<double> s) :first(f), second(s) {};
  } value = { 10., {1.,2.,3.,4.,5.} };
  {
    range or_range = { 1., {0.,0.,0.,0.} };
    const auto const_option = ara::Option<range>(value);
    auto option = ara::Option<range>(value);

    auto r1 = const_option.and_then<double> ( [](const range& val) { return ara::Option<double>( val.first); } );
    EXPECT_TRUE(r1);
    EXPECT_TRUE(const_option);

    auto r2 = option.and_then<double>([](range&& val) { return ara::Option<double>(val.first); });
    EXPECT_TRUE(r2);
    EXPECT_FALSE(option);

    auto r3 = option.and_then<double>([](range&& val) { return ara::Option<double>(val.first); });
    EXPECT_FALSE(r3);
    EXPECT_FALSE(option);

    auto r4 = const_option.mand<double>( ara::Option<double>(5.00) );
    EXPECT_TRUE(r4);
    EXPECT_TRUE(const_option);

    auto r5 = option.mand<double>(ara::Option<double>(105.00));
    EXPECT_FALSE(r5);
    EXPECT_FALSE(option);

    auto r6 = option.mor( ara::Option<range>( 95.0, std::vector<double>{} ) );
    EXPECT_TRUE(r6);
    EXPECT_FALSE(option);
    EXPECT_EQ(95.00, r6.unwrap().first);

    auto r7 = option.or_else( [](void){ return ara::Option<range>();  } );
    EXPECT_FALSE(r7);
    EXPECT_FALSE(option);

  }

}