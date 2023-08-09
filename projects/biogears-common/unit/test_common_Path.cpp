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

  BIOGEARS_NAMESPACE filesystem::path path_empty;
  BIOGEARS_NAMESPACE filesystem::path path_relative;
  BIOGEARS_NAMESPACE filesystem::path path_absolute_root;

  char const* windows_path_str = "c:/Biogears/7.3.1";
  char const* posix_path_str = "/opt/biogears/7.3.1";
  char const* relative_path_str_1 = "opt/biogears/7.3.1";
  char const* relative_path_str_2 = "../..//Biogears/7.3.1";
};

void TEST_FIXTURE_NAME::SetUp()
{
  using namespace BIOGEARS_NAMESPACE filesystem;
  path_empty = path { "" };
  path_relative = path { "./" };
#ifdef _WIN32
  path_absolute_root = path { "c:/" };
#else
  path_absolute_root = path { "/opt" };
#endif
}

void TEST_FIXTURE_NAME::TearDown()
{
}

TEST_F(TEST_FIXTURE_NAME, Path_Constructor)
{
  using namespace BIOGEARS_NAMESPACE filesystem;

  path path_1 { "" };
  path path_2 { "./" };
  path path_3 { "/" };
  path path_4 { "c:/" };
}

TEST_F(TEST_FIXTURE_NAME, Path_Empty)
{
  using namespace BIOGEARS_NAMESPACE filesystem;

  EXPECT_TRUE(path_empty.empty());
  EXPECT_FALSE(path_relative.empty());
  //This test is inconsistant across paltforms. Need to improve
  //EXPECT_FALSE(path_absolute_root.empty());
}

TEST_F(TEST_FIXTURE_NAME, Path_Concatination)
{
  using namespace BIOGEARS_NAMESPACE filesystem;

#ifdef _WIN32
  auto empty_absolute = path_empty / path("c:\\biogears");
#else
  auto empty_absolute = path_empty / path("/opt/biogears");
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
  using namespace BIOGEARS_NAMESPACE filesystem;

  auto cwd = BIOGEARS_NAMESPACE filesystem::cwd();
  path_relative /= path("biogears") / "runtime" / path("..") / path("..");
  cwd /= path("biogears") / "runtime" / path("..") / path("..");
  auto absolute_root = path_absolute_root / path("biogears") / "runtime" / path("..") / path("..");

  cwd = cwd.make_normal();
  path_relative = path_relative.make_normal();

  EXPECT_EQ(path_relative, path());
  EXPECT_EQ(absolute_root.make_normal(), path_absolute_root);
  EXPECT_EQ(path("./././../../biogears/././../first/second/../third/../../").make_normal(),
            path("").make_normal());
}

TEST_F(TEST_FIXTURE_NAME, Path_Absolute)
{
  using namespace BIOGEARS_NAMESPACE filesystem;

  EXPECT_TRUE(path_absolute_root.is_absolute());
  EXPECT_FALSE(path_relative.is_absolute());
  EXPECT_FALSE(path_empty.is_absolute());

  auto absolute_1 = path { windows_path_str };
  auto absolute_2 = path { posix_path_str };
  auto relative_1 = path { relative_path_str_1 };
  auto relative_2 = path { relative_path_str_2 };

  EXPECT_TRUE(absolute_1.is_absolute());
  EXPECT_TRUE(absolute_2.is_absolute());
  EXPECT_FALSE(relative_1.is_absolute());
  EXPECT_FALSE(relative_2.is_absolute());

  EXPECT_STREQ(absolute_1.c_str(), windows_path_str);
  EXPECT_STREQ(absolute_2.c_str(), posix_path_str);
  EXPECT_STREQ(relative_1.c_str(), relative_path_str_1);
  EXPECT_STREQ(relative_2.c_str(), "../../Biogears/7.3.1");
}

TEST_F(TEST_FIXTURE_NAME, Mode_Switch)
{
  using namespace BIOGEARS_NAMESPACE filesystem;

  auto absolute_1 = path { windows_path_str };
  auto absolute_2 = path { posix_path_str };

  EXPECT_STREQ(absolute_1.c_str(), windows_path_str);
  EXPECT_STREQ(absolute_2.c_str(), posix_path_str);

  EXPECT_EQ(absolute_1.mode(), path::default_path);
  EXPECT_EQ(absolute_2.mode(), path::default_path);

  absolute_1.mode(path::windows_path);
  absolute_2.mode(path::windows_path);

  EXPECT_EQ(absolute_1.mode(), path::windows_path);
  EXPECT_EQ(absolute_2.mode(), path::windows_path);

  EXPECT_STREQ(absolute_1.c_str(), "c:\\Biogears\\7.3.1");
  EXPECT_STREQ(absolute_2.c_str(), "\\opt\\biogears\\7.3.1");

  absolute_1.mode(path::posix_path);
  absolute_2.mode(path::posix_path);

  EXPECT_EQ(absolute_1.mode(), path::posix_path);
  EXPECT_EQ(absolute_2.mode(), path::posix_path);

  EXPECT_STREQ(absolute_1.c_str(), "c:/Biogears/7.3.1");
  EXPECT_STREQ(absolute_2.c_str(), "/opt/biogears/7.3.1");
}

TEST_F(TEST_FIXTURE_NAME, Path_filename)
{
  using namespace BIOGEARS_NAMESPACE filesystem;

  path test_path { "C:/Qt/README.md" };

  EXPECT_EQ(test_path.filename(), "README.md");
}

TEST_F(TEST_FIXTURE_NAME, Path_Basename)
{
  using namespace BIOGEARS_NAMESPACE filesystem;

  path test_path { "C:/Qt/README.md" };

  EXPECT_EQ(test_path.basename(), "README.md");
}

TEST_F(TEST_FIXTURE_NAME, Path_Extension)
{
  using namespace BIOGEARS_NAMESPACE filesystem;

  path test_path { "C:/Qt/README.md" };

  EXPECT_EQ(test_path.extension(), "md");
}

TEST_F(TEST_FIXTURE_NAME, Path_Dirname)
{
  using namespace BIOGEARS_NAMESPACE filesystem;

  path test_path { "C:/Qt/README.md" };
  path work_directory { "test_common_path/dir1/dir2" };
  path test_path3 { "C:/Qt/NonExistantDirectory" };
  auto cwd_path = cwd();
  ASSERT_TRUE(create_directories(work_directory));

  EXPECT_EQ(test_path.dirname(), "C:/Qt");
  EXPECT_EQ(work_directory.dirname(), work_directory);

  cwd_path /= work_directory;

  EXPECT_EQ(cwd_path.dirname(), cwd_path);

  cwd_path = cwd();

  EXPECT_EQ(test_path3.dirname(), "C:/Qt");

  ASSERT_TRUE(rmdir(work_directory));
  ASSERT_TRUE(rmdir(work_directory.parent_path()));
  ASSERT_TRUE(rmdir(work_directory.parent_path().parent_path()));
}

TEST_F(TEST_FIXTURE_NAME, Path_ParentPath)
{
  using namespace BIOGEARS_NAMESPACE filesystem;

  path test_path { "C:/Qt/README.md" };
  path test_path2 { "C:/Qt/5.14.2" };
  path test_path3 { "" };
  path test_path4 { "Qt/5.14.2" };

  EXPECT_STREQ(test_path.c_str(), "C:/Qt/README.md");
  EXPECT_EQ(test_path.parent_path(), "C:/Qt/");
  EXPECT_EQ(test_path2.parent_path(), "C:/Qt/");
  EXPECT_EQ(test_path3.parent_path(), "..");
  EXPECT_EQ(test_path4.parent_path(), "Qt/");
}
TEST_F(TEST_FIXTURE_NAME, Path_IsDirectory)
{
  using namespace BIOGEARS_NAMESPACE filesystem;

  EXPECT_TRUE(path_absolute_root.is_directory());
  EXPECT_TRUE(path_relative.is_directory());
  EXPECT_FALSE(path_empty.is_directory());

  path_absolute_root /= "test.txt";

  EXPECT_FALSE(path_absolute_root.is_directory());
}
