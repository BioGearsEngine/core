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

#include <ara/threading/owning_mutex.tci.h>

#ifdef DISABLE_KALE_OwningMutex_TEST
  #define TEST_FIXTURE_NAME DISABLED_OwningMutex_Fixture
#else
  #define TEST_FIXTURE_NAME OwningMutex_Fixture
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

TEST_F(TEST_FIXTURE_NAME, OwningMutex_Construction)
{
  using namespace ara;
  std::string value = "Donkey_King_Country";
  OwningMutex<std::string> om1(value);
  OwningMutex<std::string> om2( std::move(value) );
  OwningMutex<std::string> om3( "Wonky_Tonkey");

}

TEST_F(TEST_FIXTURE_NAME, OwningMutex_Aquire)
{
  using namespace ara;
  std::string value = "Donkey_King_Country";
  auto mutex = OwningMutex<std::string>::make(value);
	auto acquired = mutex.acquire();
  EXPECT_EQ( acquired.get(), value );
}

TEST_F(TEST_FIXTURE_NAME, OwningMutex_try_aquire)
{
  using namespace ara;
  std::string value = "Donkey_King_Country";

  OwningMutex<std::string> mutex(value);
  auto aquire = mutex.try_acquire();
  auto missed = mutex.try_acquire();

  EXPECT_TRUE(aquire);
  EXPECT_FALSE(missed);
}

#ifndef __GNUC__
TEST_F(TEST_FIXTURE_NAME, OwningMutex_only_one)
{
  using namespace ara;
  using namespace std::chrono_literals;

  std::string value = "Donkey_King_Country";

  OwningMutex<std::string> mutex(value);
  

	std::function<void(void)> thread_main = [&guard = mutex.acquire()](void) {
		guard.get();
		std::this_thread::sleep_for(2s);
	};
  auto thread = std::thread( std::bind( thread_main ));

  auto locked = mutex.acquire();
	if(thread.joinable())
	{  thread.join();  }

}
#endif

TEST_F(TEST_FIXTURE_NAME, OwningMutex_shared_mutex)
{
  using namespace ara;
  using namespace std::chrono_literals;
  std::string value = "Donkey_King_Country";

  auto shared_mutex = make_shared_mutex<std::string>(value);

  auto thread = std::thread([=]() {
		auto aquired = shared_mutex->acquire();
    std::this_thread::sleep_for(2s);
  });
	std::this_thread::sleep_for(4s);
  auto lock = shared_mutex->acquire();
	if ( thread.joinable())
	{
		thread.join();
	}

}