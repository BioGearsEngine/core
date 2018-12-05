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

#include <biogears/threading/thread_work_channel.tci.h>
#include <biogears/container/concurrent_queue.tci.h>

#ifdef DISABLE_BIOGEARS_WorkChannel_TEST
  #define TEST_FIXTURE_NAME DISABLED_WorkChannel_Fixture
#else
  #define TEST_FIXTURE_NAME WorkChannel_Fixture
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

TEST_F(TEST_FIXTURE_NAME, WorkChannel_Construction)
{
  auto channel = biogears::ThreadWorkChannel<void(void)>::make_shared();
  
  auto source = channel->as_source();
  auto sink   = channel->as_sink();
}

TEST_F(TEST_FIXTURE_NAME, WorkChannel_push_pop)
{
  auto channel = biogears::ThreadWorkChannel<int(void)>::make_shared();

  int value = 5;
  auto source = channel->as_source();
  auto sink = channel->as_sink();

	source->insert([](){return 1;});
	source->insert([](){return 2;});
	source->insert([](){return 3;});

	EXPECT_EQ(sink->consume()(), 1);
	EXPECT_EQ(sink->consume()(), 2);
	EXPECT_EQ(sink->consume()(), 3);
}
