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
#include <chrono>

#include <gtest/gtest.h>

#include <ara/threading/thread_pool.h>


#ifdef DISABLE_KALE_ThreadPool_TEST
  #define TEST_FIXTURE_NAME DISABLED_ThreadPool_Fixture
#else
  #define TEST_FIXTURE_NAME ThreadPool_Fixture
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

TEST_F(TEST_FIXTURE_NAME,ThreadPool_Construction)
{
  auto pool = ara::ThreadPool(5);
}

TEST_F(TEST_FIXTURE_NAME, ThreadPool_run)
{
	auto pool = ara::ThreadPool(5);
	pool.start();
	pool.stop();
	pool.join();
}

TEST_F(TEST_FIXTURE_NAME, ThreadPool_sink_source)
{
	auto pool = ara::ThreadPool(5);
	pool.start();

	auto source = pool.get_source();

	using namespace std::chrono_literals;

	source->insert( [](){ std::this_thread::sleep_for(2s);});
	source->insert( [](){ std::this_thread::sleep_for(2s);});
	source->insert( [](){ std::this_thread::sleep_for(2s);});
	source->insert( [](){ std::this_thread::sleep_for(2s);});
	source->insert( [](){ std::this_thread::sleep_for(2s);});
	source->insert( [](){ std::this_thread::sleep_for(2s);});
	pool.stop();
	pool.join();
}
