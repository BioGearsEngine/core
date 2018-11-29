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
//! Unit Test for NGSS Config
//!
#include <thread>

#include <gtest/gtest.h>

#include <biogears/framework/result.tci.h>
#include <biogears/framework/option.tci.h>

#ifdef DISABLE_BIOGEARS_Result_TEST
  #define TEST_FIXTURE_NAME DISABLED_Result_Fixture
#else
  #define TEST_FIXTURE_NAME Result_Fixture
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

TEST_F(TEST_FIXTURE_NAME, Result_Construction)
{
  struct range
  {
    double high;
    double low;
  };
  range struct_value = range{ 1.0, 100.0 };
  double double_value = 9.9;
  auto doubleResult_value = biogears::Result<double,int>::make(double_value);
  auto doubleResult_null = biogears::Result<double,int>::make_error();

  auto structResult_value = biogears::Result<range,int>::make(range{ 5.,5. });
  auto structResult_null = biogears::Result<range,int>::make_error();

  auto pointerResult_value = biogears::Result<range*,int>::make(&struct_value);
  auto pointerResult_null = biogears::Result<range*,int>::make_error();

  auto  some_value = structResult_value.clone();
  auto  some_null  = structResult_null.clone();

  EXPECT_TRUE( doubleResult_value.is_ok() );
  EXPECT_TRUE( doubleResult_null.is_err() );

  EXPECT_TRUE(pointerResult_value.is_ok());
  EXPECT_TRUE(pointerResult_null.is_err());

  EXPECT_TRUE(structResult_value.is_ok());
  EXPECT_TRUE(structResult_null.is_err());

  EXPECT_FALSE(some_value.is_err());
  EXPECT_FALSE(some_null.is_ok());
}

TEST_F(TEST_FIXTURE_NAME, Result_ok_err)
{
  struct range
  {
    double high;
    double low;
  };
  range struct_value = range{ 1.0, 100.0 };
  double double_value = 9.9;
  auto doubleResult_value = biogears::Result<double, int>::make(double_value);
  auto doubleResult_null = biogears::Result<double, int>::make_error();

  EXPECT_FALSE(doubleResult_value.err());
  EXPECT_TRUE(doubleResult_value.ok());
  EXPECT_FALSE(doubleResult_value.ok());
  EXPECT_FALSE(doubleResult_value.err());

  EXPECT_FALSE(doubleResult_null.ok());
  EXPECT_TRUE(doubleResult_null.err());
  EXPECT_FALSE(doubleResult_null.err());
  EXPECT_FALSE(doubleResult_null.ok());

}

TEST_F(TEST_FIXTURE_NAME, Result_bool_operator)
{

  struct range
  {
    double high;
    double low;
  };
  range struct_value = range{ 1.0, 100.0 };
  double double_value = 9.9;
  auto doubleResult_value = biogears::Result<double, int>::make(double_value);
  auto doubleResult_null = biogears::Result<double, int>::make_error();

  auto structResult_value = biogears::Result<range, int>::make(range{ 5.,5. });
  auto structResult_null = biogears::Result<range, int>::make_error();

  auto pointerResult_value = biogears::Result<range*, int>::make(&struct_value);
  auto pointerResult_null = biogears::Result<range*, int>::make_error();

  auto  some_value = structResult_value.clone();
  auto  some_null = structResult_null.clone();

  EXPECT_TRUE(doubleResult_value);
  EXPECT_FALSE(doubleResult_null);

  EXPECT_TRUE(pointerResult_value);
  EXPECT_FALSE(pointerResult_null);

  EXPECT_TRUE(structResult_value);
  EXPECT_FALSE(structResult_null);

  some_value.unwrap();
  EXPECT_FALSE(some_value);
  EXPECT_FALSE(some_null);
}

TEST_F(TEST_FIXTURE_NAME, Reslt_emplace_creation)
{

  double double_value = 9.9;
  auto doubleResult_value = biogears::Result<double,int>::make(double_value);
  auto doubleResult_null  = biogears::Result<double,int>::make_error();

  auto Result2 = doubleResult_value.clone();
  auto Result3 = doubleResult_value.clone();

  Result2.unwrap();
  EXPECT_FALSE(Result2);
  Result2.emplace(99.9);
  EXPECT_TRUE(Result2);

  EXPECT_TRUE(Result3);
  Result3.emplace_err(1);
  EXPECT_FALSE(Result3);

  Result2.unwrap();
  EXPECT_FALSE(Result2);
  Result2.emplace(double_value);
  EXPECT_TRUE(Result2);

}

TEST_F(TEST_FIXTURE_NAME, Result_unwrap_and_dump)
{

  double double_value = 9.9;
  auto doubleResult_value = biogears::Result<double, int>::make(double_value);
  auto doubleResult_null = biogears::Result<double, int>::make_error(5);

  auto Result2 = doubleResult_value.clone();
  auto Result3 = doubleResult_value.clone();

  EXPECT_TRUE(doubleResult_value);
  EXPECT_EQ(doubleResult_value.unwrap(), 9.9);
  EXPECT_FALSE(doubleResult_value);
  #ifdef _DEBUG
  EXPECT_DEATH(doubleResult_value.unwrap(), "");
  #endif

  EXPECT_TRUE(doubleResult_null.is_err());
  EXPECT_EQ(doubleResult_null.unwrap_err(), 5);
  EXPECT_FALSE(doubleResult_null.is_err());
#ifdef _DEBUG
  EXPECT_DEATH(doubleResult_null.unwrap_err(), "");
#endif

  EXPECT_NO_THROW(Result2.expect("I expect a value"));
  EXPECT_ANY_THROW(Result2.expect("I expect a value"));


  EXPECT_EQ(Result3.unwrap_or(9.0), 9.9);
  EXPECT_EQ(Result3.unwrap_err_or(55),55);
  EXPECT_EQ(Result3.unwrap_or(9.0), 9.0);

  Result3.emplace_ok(9.99);
  auto r1 = Result3.unwrap_or_else( [](void){ return 3.14; });
  EXPECT_EQ( r1, 9.99);
  r1 = Result3.unwrap_or_else([](void) { return 3.14; });
  EXPECT_EQ(r1, 3.14);
  r1 = Result3.unwrap_or_else([](void) { return 3.14; });
  EXPECT_EQ(r1, 3.14);

}

TEST_F(TEST_FIXTURE_NAME, Result_match)
{

  double double_value = 9.9;
  {
    const auto doubleResult_value = biogears::Result<double, int>::make(double_value);
    const auto doubleResult_null = biogears::Result<double, int>::make_error();

    EXPECT_TRUE(doubleResult_value);
    EXPECT_TRUE(doubleResult_value.match<bool>([](const double&) { return true; }, [](void) { return false; }));
    EXPECT_TRUE(doubleResult_value);

    EXPECT_FALSE(doubleResult_null);
    EXPECT_FALSE(doubleResult_null.match<bool>([](const double&) { return true; }, [](void) { return false; }));
    EXPECT_FALSE(doubleResult_null);
  }


  {
    auto doubleResult_value = biogears::Result<double, int>::make(double_value);
    auto doubleResult_null = biogears::Result<double, int>::make_error();

    auto Result2 = doubleResult_value.clone();
    auto Result3 = doubleResult_null.clone();

    EXPECT_TRUE(doubleResult_value);
    EXPECT_TRUE(doubleResult_value.match<bool>([](double&&) { return true; }, [](void) { return false; }));
    EXPECT_FALSE(doubleResult_value);
#ifdef _DEBUG
    EXPECT_DEATH(doubleResult_value.unwrap(), "");
    EXPECT_NO_FATAL_FAILURE(doubleResult_value.match<bool>([](const double&) { return true; }, [](void) { return false; }));
#endif
    EXPECT_FALSE(doubleResult_null);
    EXPECT_FALSE(doubleResult_null.match<bool>([](double&&) { return true; }, [](void) { return false; }));
    EXPECT_FALSE(doubleResult_null);
#ifdef _DEBUG
    EXPECT_DEATH(doubleResult_null.unwrap_err();, "");
    EXPECT_NO_FATAL_FAILURE(doubleResult_null.match<bool>([](const double&) { return true; }, [](void) { return false; }));
#endif

  }

  auto doubleResult_value = biogears::Result<double, int>::make(double_value);
  auto doubleResult_null = biogears::Result<double, int>::make_error();

  char result = 'A';
  doubleResult_value.if_ok ( [&](const double&){ result = 'B'; } );
  EXPECT_EQ(result,'B');
  doubleResult_value.if_err( [&](const int&){ result = 'C'; } );
  EXPECT_EQ(result,'B');

  doubleResult_null.if_err( [&](const int&) { result = 'C'; });
  EXPECT_EQ(result,'C');
  doubleResult_null.if_ok([&](const double&) { result = 'B'; });
  EXPECT_EQ(result,'C');

}
TEST_F(TEST_FIXTURE_NAME, Result_map)
{

  //void map_void(std::function<void(T&&)> func);
  //Result<U, E> map(std::function< Result<U, E>(T&&)> func);
  //void map_err(std::function<void(E&&)> func);
  //Result<T, F> map_err(std::function<F(E&&)> func);

  double double_value = 9.9;
  {
    const auto doubleResult_value = biogears::Result<double, int>::make(double_value);
    const auto doubleResult_null = biogears::Result<double, int>::make_error();


    bool shared_result = false;
    EXPECT_TRUE(doubleResult_value);
    doubleResult_value.map_void( [&](const double&) { shared_result = true; } );
    EXPECT_TRUE(shared_result);
    EXPECT_TRUE(doubleResult_value);

    shared_result = false;
    EXPECT_FALSE(doubleResult_null);
    doubleResult_null.map_void( [&](const double&) { shared_result = true; } );
    EXPECT_FALSE(shared_result);
    EXPECT_FALSE(doubleResult_null);


    EXPECT_TRUE(doubleResult_value);
    auto map_result = doubleResult_value.map<bool>( [](const double&) { return biogears::Result<bool,int>::make(true); } );
    EXPECT_EQ(map_result.unwrap(), true);
    EXPECT_TRUE(doubleResult_value);

    EXPECT_FALSE(doubleResult_null);
    map_result = doubleResult_null.map<bool>( [](const double&) {return  biogears::Result<bool,int>::make(false); } );
    EXPECT_TRUE(map_result.is_err());
    EXPECT_FALSE(doubleResult_null);

    int map_err_result = 0;
    EXPECT_TRUE(doubleResult_value);
    doubleResult_value.map_err([&](const int&) { map_err_result = 10; });
    EXPECT_EQ(map_err_result,0);
    EXPECT_TRUE(doubleResult_value);

    EXPECT_FALSE(doubleResult_null);
    doubleResult_null.map_err([&](const int&) { map_err_result = 15; });
    EXPECT_EQ(map_err_result,15);
    EXPECT_FALSE(doubleResult_null);

    
    EXPECT_TRUE(doubleResult_value);
    auto map_err_t_result = doubleResult_value.map_err<bool>([&](const int&) { return true; });
    EXPECT_EQ(map_err_t_result.unwrap(), double_value);
    EXPECT_TRUE(doubleResult_value);

    EXPECT_FALSE(doubleResult_null);
    map_err_t_result = doubleResult_null.map_err<bool>([&](const int&) { return true; });
    EXPECT_TRUE(map_err_t_result.is_err());
    EXPECT_FALSE(doubleResult_null);
  }


  {
    auto doubleResult_value = biogears::Result<double, int>::make(double_value);
    auto doubleResult_null = biogears::Result<double, int>::make_error();


    bool shared_result = false;
    EXPECT_TRUE(doubleResult_value);
    doubleResult_value.map_void([&](double&&) { shared_result = true; });
    EXPECT_TRUE(shared_result);
    EXPECT_FALSE(doubleResult_value);

    shared_result = false;
    EXPECT_FALSE(doubleResult_null);
    doubleResult_null.map_void([&](double&&) { shared_result = true; });
    EXPECT_FALSE(shared_result);
    EXPECT_FALSE(doubleResult_null);

    doubleResult_value.emplace_ok(double_value);
    doubleResult_null.emplace_err(100);

    EXPECT_TRUE(doubleResult_value);
    auto map_result = doubleResult_value.map<bool>([](double&&) { return biogears::Result<bool, int>::make(true); });
    EXPECT_EQ(map_result.unwrap(), true);
    EXPECT_FALSE(doubleResult_value);

    EXPECT_FALSE(doubleResult_null);
    map_result = doubleResult_null.map<bool>([](double&&) {return  biogears::Result<bool, int>::make(false); });
    EXPECT_TRUE(map_result.is_err());
    EXPECT_FALSE(doubleResult_null);

    doubleResult_value.emplace_ok(double_value);
    doubleResult_null.emplace_err(100);

    int map_err_result = 0;
    EXPECT_TRUE(doubleResult_value);
    doubleResult_value.map_err([&](const int&) { map_err_result = 10; });
    EXPECT_EQ(map_err_result, 0);
    EXPECT_TRUE(doubleResult_value);

    EXPECT_FALSE(doubleResult_null);
    doubleResult_null.map_err([&](const int&) { map_err_result = 15; });
    EXPECT_EQ(map_err_result, 15);
    EXPECT_FALSE(doubleResult_null);

    doubleResult_value.emplace_ok(double_value);
    doubleResult_null.emplace_err(100);

    EXPECT_TRUE(doubleResult_value);
    auto map_err_t_result = doubleResult_value.map_err<bool>([&](const int&) { return true; });
    EXPECT_EQ(map_err_t_result.unwrap(), double_value);
    EXPECT_FALSE(doubleResult_value);

    EXPECT_FALSE(doubleResult_null);
    map_err_t_result = doubleResult_null.map_err<bool>([&](const int&) { return true; });
    EXPECT_TRUE(map_err_t_result.is_err());
    EXPECT_FALSE(doubleResult_null);
  }

}

TEST_F(TEST_FIXTURE_NAME, Result_and_then)
{

  double double_value = 9.9;
  {
    auto doubleResult_value = biogears::Result<double, int>::make(double_value);
    auto doubleResult_null = biogears::Result<double, int>::make_error();

    EXPECT_TRUE(doubleResult_value);
    auto and_then_result = doubleResult_value.and_then<bool>(
      [](double&&)  { return biogears::Result<bool, int>::make(true); }
      );
    EXPECT_TRUE(and_then_result.is_ok());
    EXPECT_FALSE(doubleResult_value);

    EXPECT_FALSE(doubleResult_null);
		and_then_result = doubleResult_null.and_then<bool>(
      [](double&&) { return biogears::Result<bool, int>::make_error(true); }
    );
    EXPECT_TRUE( and_then_result.is_err());
    EXPECT_FALSE(doubleResult_null);

		doubleResult_value.emplace_ok(double_value);
		doubleResult_null.emplace_err(100);

    EXPECT_TRUE(doubleResult_value);
    auto mand_result = doubleResult_value.mand<bool>( biogears::Result<bool, int>::make(true) );
    EXPECT_TRUE(mand_result.is_ok());
    EXPECT_TRUE(doubleResult_value);

    EXPECT_FALSE(doubleResult_null);
		mand_result = doubleResult_null.mand<bool>(  biogears::Result<bool, int>::make_error(5) );
    EXPECT_TRUE(mand_result.is_err());
    EXPECT_FALSE(doubleResult_null);

		doubleResult_value.emplace_ok(double_value);
		doubleResult_null.emplace_err(100);

		EXPECT_TRUE(doubleResult_value);
		auto mor_result = doubleResult_value.mor(biogears::Result<double, int>::make(0));
		EXPECT_TRUE(mor_result.is_ok());
		EXPECT_FALSE(doubleResult_value);

		EXPECT_FALSE(doubleResult_null);
		mor_result = doubleResult_null.mor(biogears::Result<double, int>::make_error(5));
		EXPECT_TRUE(mor_result.is_err());
		EXPECT_FALSE(doubleResult_null);
  
		doubleResult_value.emplace_ok(double_value);
		doubleResult_null.emplace_err(100);

		EXPECT_TRUE(doubleResult_value);
		auto or_else_result = doubleResult_value.or_else<bool>(
		[](int&&) { return biogears::Result<double, bool>::make_error(false);}
		,false
		);
		EXPECT_TRUE(or_else_result.is_ok());
		EXPECT_FALSE(doubleResult_value);

		EXPECT_FALSE(doubleResult_null);
		or_else_result = doubleResult_null.or_else<bool>(
		[](int&&) { return biogears::Result<double, bool>::make_error(false); }
		,false
		);
		EXPECT_TRUE(or_else_result.is_err());
		EXPECT_FALSE(doubleResult_null);
  }


  

  const auto doubleResult_value = biogears::Result<double, int>::make(double_value);
  const auto doubleResult_null = biogears::Result<double, int>::make_error(500);

  auto result = doubleResult_null.and_then<std::string>
    ([](const double&) 
    { return biogears::Result<std::string, int>::make("Divine Protector"); }
    );
 
  EXPECT_EQ(result.unwrap_err(), 500);
  EXPECT_TRUE(doubleResult_value);

  result = doubleResult_value.and_then<std::string>
    ([](const double&) 
    { return biogears::Result<std::string,int>::make("Divine Protector"); }
    );

	EXPECT_EQ( result.unwrap(), std::string("Divine Protector"));
  EXPECT_FALSE(doubleResult_null);
}