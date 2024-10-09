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
#include <biogears/engine/controller/BiogearsSubstances.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
#include <biogears/cdm/properties/SEProperties.h>

#ifdef DISABLE_BIOGEARS_BioGearsSubstances_TEST
#define TEST_FIXTURE_NAME DISABLED_BioGearsSubstances_Fixture
#else
#define TEST_FIXTURE_NAME BioGearsSubstances_Fixture
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
  std::unique_ptr<biogears::BioGearsEngine> bg;
  biogears::BioGearsSubstances* bs;
};
void TEST_FIXTURE_NAME::SetUp()
{
  logger = new biogears::Logger;
  bg = std::make_unique<biogears::BioGearsEngine>(logger);
  bs = new biogears::BioGearsSubstances(*bg);
  bs->LoadSubstanceDirectory();
}
void TEST_FIXTURE_NAME::TearDown()
{
  bg.reset();
  delete logger;
}

TEST_F(TEST_FIXTURE_NAME, Clear)
{
  bs->Clear();
}

TEST_F(TEST_FIXTURE_NAME, Reset)
{

  bs->Reset();
}

TEST_F(TEST_FIXTURE_NAME, LoadSubstanceDirectory)
{
  bs->LoadSubstanceDirectory();
}

TEST_F(TEST_FIXTURE_NAME, Substances)
{
  auto& albumin = bs->GetAlbumin();
  {
      auto albumin2 = albumin;
  }
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