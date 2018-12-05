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

#include <biogears/chrono/stop_watch.tci.h>

#ifdef DISABLE_BIOGEARS_StopWatch_TEST
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

  using  biogears::StopWatch;

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