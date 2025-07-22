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
//! @author
//! @date   2021/03/22
//!
//! Unit Test for BioGears Seralization
//!
#include <thread>

#include <gtest/gtest.h>

#include <biogears/cdm/utils/Logger.h>

#include <io/cdm/EnvironmentActions.h>
#include <io/cdm/Property.h>

#ifdef DISABLE_BIOGEARS_EnvironmentActions_TEST
#define TEST_FIXTURE_NAME DISABLED_EnvironmentActionsFixture
#else
#define TEST_FIXTURE_NAME IO_EnvironmentActionsFixture
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

#define USING_TYPES(name)             \
  using namespace biogears::io;       \
  using SEType = biogears ::SE##name; \
  using CDMType = CDM ::name##Data;

#include <biogears/cdm/system/environment/actions/SEEnvironmentAction.h>

// class SEEnvironmentChange;
//!
//! TYPE EnvironmentChange
//! static void UnMarshall(const CDM::EnvironmentChange& in, SEEnvironmentChange& out);
//! static void Marshall(const SEEnvironmentChange& in, CDM::EnvironmentChange& out);
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/environment/actions/SEEnvironmentChange.h>
TEST_F(TEST_FIXTURE_NAME, EnvironmentChange)
{
  USING_TYPES(EnvironmentChange)

  biogears::Logger logger;
  biogears::SESubstanceManager subMgr { &logger };
  ASSERT_TRUE(subMgr.LoadSubstanceDirectory());
  SEType source { subMgr }, sink { subMgr };
  CDMType data;

  source.SetComment("Test Comment");
  source.SetConditionsFile("SubmergedFreezing.xml");
  auto& conditions = source.GetConditions();
  

  //EXPECT_NE(source, sink);

  EnvironmentActions::Marshall(source, data);
  EnvironmentActions::UnMarshall(data, sink);

  //EXPECT_EQ(source, sink);
}

// class SEThermalApplication;
//!
//! TYPE ThermalApplication
//! static void UnMarshall(const CDM::ThermalApplication& in, SEThermalApplication& out);
//! static void Marshall(const SEThermalApplication& in, CDM::ThermalApplication& out);
#include <biogears/cdm/system/environment/actions/SEThermalApplication.h>
TEST_F(TEST_FIXTURE_NAME, ThermalApplication)
{
  USING_TYPES(ThermalApplication)

  SEType source, sink;
  CDMType data;

  //source.SetComment("Test Comment");
  //source.SetType(biogears::SEEnvironmentAssessment::CompleteBloodCount);

  //EXPECT_NE(source, sink);

  EnvironmentActions::Marshall(source, data);
  EnvironmentActions::UnMarshall(data, sink);

  //EXPECT_EQ(source, sink);
}