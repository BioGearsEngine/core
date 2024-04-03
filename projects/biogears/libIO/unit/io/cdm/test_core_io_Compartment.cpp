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

#include <biogears/cdm/circuit/SECircuitManager.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuit.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuitCalculator.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuitNode.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuitPath.h>
#include <biogears/cdm/compartment/SECompartment.h>
#include <biogears/cdm/compartment/SECompartmentLink.h>
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/compartment/fluid/SEGasCompartmentGraph.h>
#include <biogears/cdm/compartment/fluid/SEGasCompartmentLink.h>
#include <biogears/cdm/properties/SEProperties.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/utils/Logger.h>
#include <io/cdm/Compartment.h>
#include <io/cdm/Property.h>

#ifdef DISABLE_BIOGEARS_Compartments_TEST
#define TEST_FIXTURE_NAME DISABLED_CompartmentsFixture
#else
#define TEST_FIXTURE_NAME IO_CompartmentsFixture
#endif

// The fixture for testing class Foo.
class TEST_FIXTURE_NAME : public ::testing::Test {
protected:
  // You can do set-up work for each test here.
  TEST_FIXTURE_NAME()
    : logger()
    , substanceManager(&logger)
    , compartmentManager(substanceManager)
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
  biogears::SECompartmentManager compartmentManager;
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

// class SEGasCompartment;
// class SEGasCompartmentLink;
// class SEGasCompartmentGraph;
// class SELiquidCompartment;
// class SELiquidCompartmentLink;
// class SELiquidCompartmentGraph;
// class SETissueCompartment;
// class SEThermalCompartment;
// class SEThermalCompartmentLink;
// class SECompartmentManager;
//

//! Abstract class SEGasCompartment
//! TYPE GasCompartment
//! static void UnMarshall(const CDM::GasCompartment& in, SEGasCompartment& out);
//! static void Marshall(const SEGasCompartment& in, CDM::GasCompartment& out);
#include <biogears/cdm/compartment/fluid/SEGasCompartment.h>
TEST_F(TEST_FIXTURE_NAME, GasCompartment)
{
  using namespace biogears;
  USING_TYPES(GasCompartment)

  auto& fc1 = compartmentManager.CreateGasCompartment("Compartment1");
  auto& fc2 = compartmentManager.CreateGasCompartment("Compartment2");
  auto& fc3 = compartmentManager.CreateGasCompartment("Compartment3");

  auto& source = compartmentManager.CreateGasCompartment("source");
  auto& sink = compartmentManager.CreateGasCompartment("sink");
  CDMType data;

  EXPECT_NE(source, sink);

  Compartment::Marshall(source, data);
  Compartment::UnMarshall(data, sink, substanceManager);

  EXPECT_EQ(source, sink);
};

//! Abstract class SEGasCompartmentLink
//! TYPE GasCompartmentLink
//! static void UnMarshall(const CDM::GasCompartmentLink& in, SEGasCompartmentLink& out);
//! static void Marshall(const SEGasCompartmentLink& in, CDM::GasCompartmentLink& out);
#include <biogears/cdm/compartment/fluid/SEGasCompartmentLink.h>
TEST_F(TEST_FIXTURE_NAME, GasCompartmentLink)
{
  USING_TYPES(GasCompartmentLink)

  //auto compartment1 = compartmentManager.GetGasCompartment("Compartment One");
  //auto compartment2 = compartmentManager.GetGasCompartment("Compartment Two");
  //auto compartment3 = compartmentManager.GetGasCompartment("Compartment Three");
  //auto compartment4 = compartmentManager.GetGasCompartment("Compartment Four");

  //auto& source = compartmentManager.CreateGasLink(*compartment1, *compartment2, "one->two");
  //auto& sink = compartmentManager.CreateGasLink(*compartment3, *compartment4, "three->four");
  //CDMType data;

  //EXPECT_NE(source, sink);

  //Compartment::Marshall(source, data);
  //Compartment::UnMarshall(data, sink);

  //EXPECT_EQ(source, sink);
};

//! Abstract class SEGasCompartmentGraph
//! TYPE GasCompartmentGraph
// static void UnMarshall(const CDM::GasCompartmentGraphData& in, SEGasCompartmentGraph& out, SECompartmentManager& cmptMgr);
// static void Marshall(const SEGasCompartmentGraph& in, CDM::GasCompartmentGraphData& out);
#include <biogears/cdm/compartment/fluid/SEGasCompartmentGraph.h>
TEST_F(TEST_FIXTURE_NAME, GasCompartmentGraph) {
  // USING_TYPES(GasCompartmentGraph)

  // auto& source = compartmentManager->CreateGasGraph("source");
  // auto& sink = compartmentManager->CreateGasGraph("sink");
  // CDMType data;

  // EXPECT_NE(source, sink);

  // Compartment::Marshall(source, data);
  // Compartment::UnMarshall(data, sink, compartmentManager);

  // EXPECT_EQ(source, sink);
};