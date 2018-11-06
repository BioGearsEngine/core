//! Copyright (C) Applied Research Associates - All Rights Reserved
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

#include <ara/chrono/stop_watch.tci.h>

#ifdef DISABLE_KALE_StopWatch_TEST
  #define TEST_FIXTURE_NAME DISABLED_StopWatchFixture
#else
  #define TEST_FIXTURE_NAME StopWatchFixture
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

TEST_F(TEST_FIXTURE_NAME, StopWatch)
{
//FAils sometimes when DLL loading is slow
//We could make the intervals longer and easier to manage
//But then it bogs down the test server

  using std::chrono::nanoseconds;
  using std::chrono::milliseconds;
  using std::chrono::seconds;
  using std::chrono::minutes;

  using  std::this_thread::sleep_for;

  using  ara::StopWatch;

  StopWatch<milliseconds> nano_watch;
  StopWatch<seconds> sec_watch;

  nano_watch.lap();
  sec_watch.lap();

  std::this_thread::sleep_for( milliseconds(500) );

  EXPECT_LE (nano_watch.lap(), 550);
  EXPECT_LE (nano_watch.lap_s(), 1);
  EXPECT_LE (sec_watch.lap<seconds>(), 1);

  std::this_thread::sleep_for( milliseconds(500) );

  EXPECT_LE(nano_watch.total(), 1100);
  EXPECT_LE(nano_watch.total_s(), 2);
  EXPECT_LE(sec_watch.total<seconds>(), 2);

  nano_watch.reset();
  sec_watch.reset();

  EXPECT_LE(sec_watch.total_ms(), 1);
  EXPECT_LE(nano_watch.total_ms(), 1);
}