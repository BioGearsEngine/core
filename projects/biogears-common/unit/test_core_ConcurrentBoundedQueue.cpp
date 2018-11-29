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
#include <functional>
#include <future>

#include <gtest/gtest.h>

#include <biogears/container/concurrent_bounded_queue.tci.h>

#ifdef DISABLE_BIOGEARS_ConcurrentBoundedQueue_TEST
  #define TEST_FIXTURE_NAME DISABLED_ConcurrentBoundedQueueFixture
#else
  #define TEST_FIXTURE_NAME ConcurrentBoundedQueueFixture
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

TEST_F(TEST_FIXTURE_NAME, ConcurrentBoundedQueue_push_pop)
{
  using  biogears::ConcurrentBoundedQueue;
  ConcurrentBoundedQueue<int> queue(5);

  for (auto i : { 0, 1, 2, 3, 4}) {

    if (i < 5) {
      EXPECT_EQ(i, queue.unsafe_size());
      EXPECT_TRUE(queue.insert(i));
    } else {
      EXPECT_EQ(5, queue.unsafe_size());
      EXPECT_TRUE(queue.insert(i));
    }
  }

  for (auto i : {0, 1, 2, 3, 4 }) {
    EXPECT_EQ(5 - i, queue.unsafe_size());
    EXPECT_EQ(i, queue.consume());
  }
}

TEST_F(TEST_FIXTURE_NAME, ConcurrentBoundedQueue_active)
{
  using  biogears::ConcurrentBoundedQueue;
  ConcurrentBoundedQueue<int> queue(5);


  EXPECT_TRUE(queue.active());
  queue.abort();
  EXPECT_TRUE(queue.active());
  queue.shutdown();
  EXPECT_FALSE(queue.active());

}

TEST_F(TEST_FIXTURE_NAME, ConcurrentBoundedQueue_block_pop)
{
  using  biogears::ConcurrentBoundedQueue;
  ConcurrentBoundedQueue<int> queue(5);

  auto future = std::async(std::launch::async
  , [&queue]() {
    for (auto i : { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }) {
      std::this_thread::sleep_for(std::chrono::milliseconds(16));
      queue.insert(i);
    }

  });
  for (auto i : { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }) {
    EXPECT_EQ(0, queue.unsafe_size());
    EXPECT_EQ(i, queue.consume());
  }

  EXPECT_TRUE(queue.active());
  queue.abort();
  EXPECT_TRUE(queue.active());
  queue.shutdown();
  EXPECT_FALSE(queue.active());

}

TEST_F(TEST_FIXTURE_NAME, ConcurrentBoundedQueue_shutdown)
{
  using  biogears::ConcurrentBoundedQueue;
  ConcurrentBoundedQueue<int> queue;

  auto push = std::async(std::launch::async
  , [&queue]() {
    for (auto i : { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }) {
      std::this_thread::sleep_for(std::chrono::milliseconds(16));
      if (queue.active()) {
        queue.insert(i);
      } else {
        EXPECT_FALSE(queue.insert(i));
      }
    }

  });

  auto shutdown = std::async(std::launch::async
  , [&queue]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(48));
    queue.shutdown();
    EXPECT_FALSE(queue.active()); //< No Overflow
  });

  int value = 0;
  for (auto i : { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }) {
    EXPECT_EQ(0, queue.unsafe_size());
    value = 10;
    queue.consume(value);
    if (value != 10) {
      EXPECT_EQ(i, value);
    }
  }

  EXPECT_FALSE(queue.active());

}

