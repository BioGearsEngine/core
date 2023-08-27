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

#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/properties/SEHistogramFractionVsLength.h>
#include <biogears/cdm/properties/SEProperties.h>
#include <biogears/cdm/scenario/requests/SEDataRequestManager.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/utils/Logger.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>

#include <io/cdm/Property.h>
#include <io/cdm/System.h>

#ifdef DISABLE_BIOGEARS_Substance_TEST
#define TEST_FIXTURE_NAME DISABLED_SystemFixture
#else
#define TEST_FIXTURE_NAME IO_SystemFixture
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

  biogears::SEDecimalFormat FixtureDecimalFormat;
};

void TEST_FIXTURE_NAME::SetUp()
{
  FixtureDecimalFormat.SetNotation(biogears::DecimalNotation::Fixed);
  FixtureDecimalFormat.SetPrecision(2);
}

void TEST_FIXTURE_NAME::TearDown()
{
}

#define USING_TYPES(name)             \
  using namespace biogears::io;       \
  using SEType = biogears ::SE##name; \
  using CDMType = CDM ::name##Data;

// class  SEGasSystem
//    TYPE GasSystem
//   static void Marshall(const CDM::GasSystemData& in, SEGasSystem& out);
//   static void UnMarshall(const SEGasSystem& in, CDM::GasSystemData& out);
#include <biogears/cdm/system/SESystem.h>
namespace BGE = mil::tatrc::physiology::biogears;

TEST_F(TEST_FIXTURE_NAME, System)
{

  USING_TYPES(System)//ABstract Class

  // SEType source { }, sink {  };
  // CDMType data;

  // EXPECT_NE(source, sink);

  // System::UnMarshall(source, data);
  // System::Marshall(data, sink);

  // EXPECT_EQ(source, sink);

  EXPECT_TRUE(true);
}

