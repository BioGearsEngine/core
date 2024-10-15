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

#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/properties/SEProperties.h>

#ifdef DISABLE_BIOGEARS_SESubstanceManager_TEST
#define TEST_FIXTURE_NAME DISABLED_SESubstanceManager_Fixture
#else
#define TEST_FIXTURE_NAME SESSubstanceManager_Fixture
#endif

using namespace biogears;
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
  virtual void SetUp() override;

  // Code here will be called immediately after each test (right
  // before the destructor).
  virtual void TearDown() override;

  biogears::Logger* logger;
  biogears::SESubstanceManager* cs;
};
void TEST_FIXTURE_NAME::SetUp()
{
  logger = new biogears::Logger;
  cs = new biogears::SESubstanceManager(logger);
}
void TEST_FIXTURE_NAME::TearDown()
{
}

TEST_F(TEST_FIXTURE_NAME, Clear)
{

}

TEST_F(TEST_FIXTURE_NAME, Reset)
{
  cs->LoadSubstanceDirectory();
  cs->Reset();
}

TEST_F(TEST_FIXTURE_NAME, LoadSubstanceDirectory)
{
  cs->LoadSubstanceDirectory();

}

TEST_F(TEST_FIXTURE_NAME, Substances)
{
}

TEST_F(TEST_FIXTURE_NAME, ActiveSubstances)
{
}


TEST_F(TEST_FIXTURE_NAME, ActiveGases)
{
}

TEST_F(TEST_FIXTURE_NAME, ActiveLiquids)
{
}

TEST_F(TEST_FIXTURE_NAME, ActiveDrugs)
{
}

TEST_F(TEST_FIXTURE_NAME, Compounds)
{
}

TEST_F(TEST_FIXTURE_NAME, ActiveCompounds)
{
}

TEST_F(TEST_FIXTURE_NAME, ReadSubstanceFile)
{
}