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

#include <biogears/cdm/Circuit/SECircuit.h>
#include <biogears/cdm/Circuit/SECircuitManager.h>
#include <biogears/cdm/properties/SEProperties.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/utils/Logger.h>
#include <io/cdm/Circuit.h>
#include <io/cdm/Property.h>

#ifdef DISABLE_BIOGEARS_Circuits_TEST
#define TEST_FIXTURE_NAME DISABLED_CircuitsFixture
#else
#define TEST_FIXTURE_NAME IO_CircuitsFixture
#endif

// The fixture for testing class Foo.
class TEST_FIXTURE_NAME : public ::testing::Test {
protected:
  // You can do set-up work for each test here.
  TEST_FIXTURE_NAME()
    : logger()
    , substanceManager(&logger)
    , circuitManager(&logger)
  {
  }

  // You can do clean-up work that doesn't throw exceptions here.
  virtual ~TEST_FIXTURE_NAME()
    = default;

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  // Code here will be called immediately after the constructor (right
  // before each test).
  virtual void SetUp();

  // Code here will be called immediately after each test (right
  // before the destructor).
  virtual void TearDown();

  biogears::Logger logger;
  biogears::SESubstanceManager substanceManager;
  biogears::SECircuitManager circuitManager;
};

void TEST_FIXTURE_NAME::SetUp()
{
  substanceManager.LoadSubstanceDirectory();
}

void TEST_FIXTURE_NAME::TearDown()
{
}

#define USING_TYPES(name)             \
  using namespace biogears::io;       \
  using SEType = biogears ::SE##name; \
  using CDMType = CDM ::name##Data;

template <CIRCUIT_TEMPLATE>
class SECircuit;
template <CIRCUIT_NODE_TEMPLATE>
class SECircuitNode;
template <CIRCUIT_PATH_TEMPLATE>
class SECircuitPath;
class SEElectricalCircuit;
class SEElectricalCircuitNode;
class SEElectricalCircuitPath;
class SEFluidCircuit;
class SEFluidCircuitNode;
class SEFluidCircuitPath;
class SEThermalCircuit;
class SEThermalCircuitNode;
class SEThermalCircuitPath;
class SECircuitManager;
;
//

//! Abstract class SEGasCircuit
//! TYPE GasCircuit
//! static void UnMarshall(const CDM::GasCircuit& in, SEGasCircuit& out);
//! static void Marshall(const SEGasCircuit& in, CDM::GasCircuit& out);
#include <biogears/cdm/Circuit/SECircuit.h>
TEST_F(TEST_FIXTURE_NAME, GasCircuit)
{
  EXPECT_TRUE(true);
}