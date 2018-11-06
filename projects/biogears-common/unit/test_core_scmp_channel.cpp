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
#include <functional>
#include <future>
#include <memory>

#include <gtest/gtest.h>

#include <ara/container/concurrent_ringbuffer.tci.h>
#include <ara/framework/scmp/scmp_channel.tci.h>

#ifdef DISABLE_KALE_scmp_channel_TEST
  #define TEST_FIXTURE_NAME DISABLED_scmp_channelFixture
#else
  #define TEST_FIXTURE_NAME scmp_channelFixture
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

TEST_F(TEST_FIXTURE_NAME, scmp_channel_push_pop)
{
  using  ara::scmp::Channel;
  using  ara::scmp::Source;

  using container = ara::ConcurrentRingbuffer<int>;

  Channel<container> channel{ 5 };
  auto  source = channel.as_source();
  for (auto i : { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }) {

    if (i < 5) {
      EXPECT_FALSE(source.insert(i));
    } else {
      EXPECT_TRUE(source.insert(i));
    }
  }

  for (auto i : { 5, 6, 7, 8, 9 }) {
    EXPECT_EQ(i, channel.consume());
  }
}

TEST_F(TEST_FIXTURE_NAME, scmp_channel_active)
{
  using  ara::scmp::Channel;
  using  ara::scmp::Source;

  using container = ara::ConcurrentRingbuffer<int>;

  Channel<container> channel{ 5 };
  auto  source = channel.as_source();


  EXPECT_TRUE(channel.active());
  channel.abort();
  EXPECT_TRUE(source.active());
  channel.shutdown();
  EXPECT_FALSE(source.active());
  EXPECT_FALSE(channel.active());

}

TEST_F(TEST_FIXTURE_NAME, scmp_channel_block_pop)
{
  using  ara::scmp::Channel;
  using  ara::scmp::Source;

  using container = ara::ConcurrentRingbuffer<int>;

  Channel<container> channel{ 5 };
  auto  source = channel.as_source();

  auto future = std::async(std::launch::async
  , [=]() mutable {
    for (auto i : { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 })
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(16));
      source.insert(i);
    }

  });
  for (auto i : { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }) {
    EXPECT_EQ(0, channel.size());
    EXPECT_EQ(i, channel.consume());
  }

  EXPECT_TRUE(channel.active());
  channel.abort();
  EXPECT_TRUE(channel.active());
  channel.shutdown();
  EXPECT_FALSE(channel.active());

}

TEST_F(TEST_FIXTURE_NAME, scmp_channel_shutdown)
{
  using  ara::scmp::Channel;
  using  ara::scmp::Source;

  using container = ara::ConcurrentRingbuffer<int>;

  Channel<container> channel{ 5 };
  auto  source = channel.as_source();
  auto  second_source = source;
  auto push = std::async(std::launch::async
  , [second_source]() mutable {
    for (auto i : { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 })
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(32));
      second_source.insert(0);
    }

  });

  auto shutdown = std::async(std::launch::async
  , [&channel]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(48));
    channel.shutdown();
    EXPECT_FALSE(channel.active()); //< No Overflow
  });

  int value = 0;
  for (auto i : { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }) {
    EXPECT_EQ(0, channel.size());
    value = 10;
    channel.consume(value);
    if (value != 10) {
      EXPECT_EQ(i, value);
    }
  }

  EXPECT_FALSE(channel.active());

}

