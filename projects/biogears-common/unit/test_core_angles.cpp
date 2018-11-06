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

#include <ara/math/angles.h>

#ifdef DISABLE_KALE_Angles_TEST
  #define TEST_FIXTURE_NAME DISABLED_Angles_Fixture
#else
  #define TEST_FIXTURE_NAME Angles_Fixture
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

//!
//! Way to much to test so many operations
//! I'm going to test construction for now
//! And we will write a few each day until we get them all
//! 
TEST_F(TEST_FIXTURE_NAME, angles)
{
  using namespace ara;
  using namespace ara::angle_literals;
	{
		float phi = 3.14159265358979323_f_rad;
		float theta = 180_f_deg;
		EXPECT_NEAR(phi, theta, 0.0001);
	}

	{
		Angle phi = 3.14159265358979323_f_rad;
		Angle theta = 180_f_deg;
		EXPECT_NEAR(phi.in_deg(), theta.in_deg(), 0.0001);
	}
}

TEST_F(TEST_FIXTURE_NAME, angles_quaternains)
{
	using namespace ara;
	using namespace ara::angle_literals;

	{
		auto first = Quaternion(1,1,1,1);
		auto second = Quaternion(1,1,1,1);
		EXPECT_EQ(first, second);
	}
}

TEST_F(TEST_FIXTURE_NAME, angles_euler)
{
	using namespace ara;
	using namespace ara::angle_literals;

	{
		auto first = EulerAng(1, 1, 1);
		auto second = EulerAng(1, 1, 1);
		EXPECT_EQ(first, second);
	}
}
