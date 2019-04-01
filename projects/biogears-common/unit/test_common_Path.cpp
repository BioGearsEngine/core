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

#include <biogears/filesystem/path.h>

#ifdef DISABLE_BIOGEARS_Path_TEST
#define TEST_FIXTURE_NAME DISABLED_PathFixture
#else
#define TEST_FIXTURE_NAME PathFixture
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

  biogears::filesystem::path path_empty;
  biogears::filesystem::path path_relative;
  biogears::filesystem::path path_absolute_root;
};

void TEST_FIXTURE_NAME::SetUp()
{
  using namespace biogears::filesystem;
  path_empty = path{ "" };
  path_relative = path{ "./" };
#ifdef _WIN32
  path_absolute_root = path{ "c:/" };
#else
  path_absolute_root = path{ "/" };
#endif
}

void TEST_FIXTURE_NAME::TearDown()
{
}

TEST_F(TEST_FIXTURE_NAME, Path_Constructor)
{
  using namespace biogears::filesystem;

  path path_1{ "" };
  path path_2{ "./" };
  path path_3{ "/" };
  path path_4{ "c:/" };
}

TEST_F(TEST_FIXTURE_NAME, Path_Empty)
{
  using namespace biogears::filesystem;

  EXPECT_TRUE(path_empty.empty());
  EXPECT_FALSE(path_relative.empty());
  //This test is inconsistant across paltforms. Need to improve
  //EXPECT_FALSE(path_absolute_root.empty());
}

TEST_F(TEST_FIXTURE_NAME, Path_Concatination)
{
  using namespace biogears::filesystem;

#ifdef _WIN32
  auto empty_absolute = path_empty / path("c:\\biogears");
#else
  auto empty_absolute = path_empty / path("biogears");
#endif
  auto empty_relative = path_empty / path("biogears");

  path_relative /= path("biogears");
  path_absolute_root /= path("biogears");

  EXPECT_NE(empty_relative, path_relative);
  EXPECT_EQ(empty_relative.make_normal(), path_relative.make_normal());

  EXPECT_EQ(empty_absolute, path_absolute_root);
}

TEST_F(TEST_FIXTURE_NAME, Path_Normalize)
{
  using namespace biogears::filesystem;

  auto cwd = path::getcwd();
  path_relative /= path("biogears") / "runtime" / path("..") / path("..");
  cwd /= path("biogears") / "runtime" / path("..") / path("..");
  auto absolute_root = path_absolute_root / path("biogears") / "runtime" / path("..") / path("..");

  cwd = cwd.make_normal();
  path_relative = path_relative.make_normal();

  EXPECT_EQ(cwd, path::getcwd());
  EXPECT_EQ(path_relative, path());
  EXPECT_EQ(absolute_root.make_normal(),path_absolute_root);
  EXPECT_EQ(path("./././../../biogears/././../first/second/../third/../../").make_normal(),
            path("").make_normal());
}

TEST_F(TEST_FIXTURE_NAME, Path_Absolute)
{
  using namespace biogears::filesystem;


  EXPECT_TRUE(path_absolute_root.is_absolute());
  EXPECT_FALSE(path_relative.is_absolute());
  EXPECT_FALSE(path_empty.is_absolute());
}


TEST_F(TEST_FIXTURE_NAME, Path_IsDirectory)
{
  using namespace biogears::filesystem;


  EXPECT_TRUE(path_absolute_root.is_directory());
  EXPECT_TRUE(path_relative.is_directory());
  EXPECT_FALSE(path_empty.is_directory());

  path_absolute_root /= "test.txt";

   EXPECT_FALSE(path_absolute_root.is_directory());
}
