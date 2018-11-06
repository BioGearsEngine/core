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

#include <ara/framework/billboard.tci.h>

#ifdef DISABLE_KALE_Billboard_TEST
  #define TEST_FIXTURE_NAME DISABLED_Billboard_Fixture
#else
  #define TEST_FIXTURE_NAME Billboard_Fixture
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

TEST_F(TEST_FIXTURE_NAME, Billboard)
{
	struct testStruct 
	{
		double high = 10.0;
		double low  = 0.0;
	};
	testStruct value;

	ara::BillboardPair<testStruct> test_board = ara::make_billboard<testStruct>( std::move(value) );

	auto viewer = test_board.viewer;
	auto publisher = std::move(test_board.publisher);

	std::cout << std::boolalpha << "Is compound "<< std::is_compound<testStruct>::value << "\n";
	std::cout << std::boolalpha << "Is trivial " << std::is_trivial<testStruct>::value << std::endl;
	EXPECT_DOUBLE_EQ(viewer.get().high, value.high);
	EXPECT_DOUBLE_EQ(publisher.get().low  , value.low);
	value.high = 100.0;
	value.low = -100.0;

	EXPECT_NE(viewer.get().high, value.high);
	EXPECT_NE(viewer.get().low, value.low);

	publisher.put(value);           //<!  Non trivial type can move and copy
	publisher.put(std::move(value));//<!  Non trivial type can move and copy

	EXPECT_DOUBLE_EQ(viewer.get().low, value.low);
	EXPECT_DOUBLE_EQ(publisher.get().high, value.high);
}

TEST_F(TEST_FIXTURE_NAME, Billboard_Viewer_Copy)
{

	double value = 9.0;
	auto test_board = ara::make_billboard<double>(value);

	auto viewer = test_board.viewer;
	auto publisher = std::move(test_board.publisher);


	std::cout << std::boolalpha << "Is compound " << std::is_compound<double>::value << "\n";
	std::cout << std::boolalpha << "Is trivial " << std::is_trivial<double>::value << std::endl;

	auto second_view = viewer;
	auto third_view = viewer.clone();
	decltype(viewer) fourth_view{viewer}  ;

	value = 3.14;
	publisher.put(value);
	publisher.put( std::move(value)); //<! Trivial type doesn't actually move

	EXPECT_DOUBLE_EQ(viewer.get(), value);
	EXPECT_DOUBLE_EQ(second_view.get(), value);
	EXPECT_DOUBLE_EQ(third_view.get(), value);
	EXPECT_DOUBLE_EQ(fourth_view.get(), value);
	EXPECT_DOUBLE_EQ(publisher.get(), value);
}

TEST_F(TEST_FIXTURE_NAME, Billboard_pointer_type)
{

	double value = 9.0;
	auto test_board = ara::make_billboard<double*>(&value);

	auto viewer = test_board.viewer;
	auto publisher = std::move(test_board.publisher);


	std::cout << std::boolalpha << "Is compound " << std::is_compound<double>::value << "\n";
	std::cout << std::boolalpha << "Is trivial " << std::is_trivial<double>::value << std::endl;

	auto second_view = viewer;
	auto third_view = viewer.clone();
	decltype(viewer) fourth_view{viewer}  ;

	value = 3.14;
	publisher.put(&value);
	publisher.put(std::move(&value)); //<! Trivial type doesn't actually move

	EXPECT_DOUBLE_EQ(*viewer.get(), value);
	EXPECT_DOUBLE_EQ(*second_view.get(), value);
	EXPECT_DOUBLE_EQ(*third_view.get(), value);
	EXPECT_DOUBLE_EQ(*fourth_view.get(), value);
	EXPECT_DOUBLE_EQ(*publisher.get(), value);
}