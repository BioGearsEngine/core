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
//! @date   2023/08/14
//!
//! Unit Test for BioGears Seralization
//!
#include <thread>

#include <gtest/gtest.h>

#include <biogears/cdm/properties/SEProperties.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/utils/Logger.h>

#include <io/cdm/InhalerActions.h>
#include <io/cdm/Property.h>

#ifdef DISABLE_BIOGEARS_InhalerActions_TEST
#define TEST_FIXTURE_NAME DISABLED_InhalerActionsFixture
#else
#define TEST_FIXTURE_NAME IO_InhalerActionsFixture
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

#include <biogears/cdm/system/equipment/Inhaler/SEInhaler.h>
#include <biogears/cdm/system/equipment/Inhaler/actions/SEInhalerConfiguration.h>
// class SEInhalerActions;
//!
//! TYPE InhalerActions
//! static void UnMarshall(const CDM::InhalerActions& in, SEInhalerActions& out);
//! static void Marshall(const SEInhalerActions& in, CDM::InhalerActions& out);
TEST_F(TEST_FIXTURE_NAME, InhalerConfiguration)
{
  USING_TYPES(InhalerConfiguration)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source(mgr), sink(mgr);
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  auto& conf = source.GetConfiguration();

  conf.SetState(CDM::enumOnOff::Off);
  conf.GetMeteredDose().SetValue(55.f, biogears::MassUnit::kg);
  conf.GetNozzleLoss().SetValue(22.f);
  conf.GetSpacerVolume().SetValue(55, biogears::VolumeUnit::L);
  conf.SetSubstance(sarin);

  EXPECT_NE(source, sink);

  InhalerActions::Marshall(source, data);
  InhalerActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
