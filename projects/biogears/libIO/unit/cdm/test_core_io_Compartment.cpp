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
//! static void Marshall(const CDM::GasCompartment& in, SEGasCompartment& out);
//! static void UnMarshall(const SEGasCompartment& in, CDM::GasCompartment& out);
#include <biogears/cdm/compartment/fluid/SEGasCompartment.h>
TEST_F(TEST_FIXTURE_NAME, GasCompartment)
{
  using namespace biogears;
  USING_TYPES(GasCompartment)

  // Compartment
  auto& source = compartmentManager.CreateGasCompartment("source");
  auto& sink = compartmentManager.CreateGasCompartment("sink");
  auto& sub_compartment_1 = compartmentManager.CreateGasCompartment("sub_compartment_1");
  auto& sub_compartment_1_1 = compartmentManager.CreateGasCompartment("sub_compartment_1_1");
  auto& sub_compartment_1_2 = compartmentManager.CreateGasCompartment("sub_compartment_1_2");

  sub_compartment_1.AddChild(sub_compartment_1_1);
  sub_compartment_1.AddChild(sub_compartment_1_2);
  source.AddChild(sub_compartment_1);

  EXPECT_NE(source, sink);

  Compartment::UnMarshall(source, data);
  Compartment::Marshall(data, sink, substanceManager, &circuitManager);

  EXPECT_EQ(source, sink);
};

//! Abstract class SEGasCompartmentLink
//! TYPE GasCompartmentLink
//! static void Marshall(const CDM::GasCompartmentLink& in, SEGasCompartmentLink& out);
//! static void UnMarshall(const SEGasCompartmentLink& in, CDM::GasCompartmentLink& out);
#include <biogears/cdm/compartment/fluid/SEGasCompartmentLink.h>
TEST_F(TEST_FIXTURE_NAME, GasCompartmentLink)
{
  USING_TYPES(GasCompartmentLink)
  CDMType data;

  // Left Compartment
  auto& left = compartmentManager.CreateGasCompartment("Left");
  // Middle Compartment
  auto& middle = compartmentManager.CreateGasCompartment("Middle");
  // Right Compartment
  auto& right = compartmentManager.CreateGasCompartment("Right");
  // Compartment Links

  auto& source = compartmentManager.CreateGasLink(left, middle, "source");
  source.GetFlow().SetValue(2.0, biogears::VolumePerTimeUnit::mL_Per_s);

  auto& sink = compartmentManager.CreateGasLink(middle, right, "sink");
  sink.GetFlow().SetValue(5.5, biogears::VolumePerTimeUnit::mL_Per_s);

  EXPECT_NE(source, sink);

  Compartment::UnMarshall(source, data);
  Compartment::Marshall(data, sink);

  EXPECT_EQ(source, sink);
};

//! Abstract class SEGasCompartmentGraph
//! TYPE GasCompartmentGraph
// static void Marshall(const CDM::GasCompartmentGraphData& in, SEGasCompartmentGraph& out, SECompartmentManager& cmptMgr);
// static void UnMarshall(const SEGasCompartmentGraph& in, CDM::GasCompartmentGraphData& out);
#include <biogears/cdm/compartment/fluid/SEGasCompartmentGraph.h>
TEST_F(TEST_FIXTURE_NAME, GasCompartmentGraph)
{
  USING_TYPES(GasCompartmentGraph)
  CDMType data;

  auto& source = compartmentManager.CreateGasGraph("source");
  auto& sink = compartmentManager.CreateGasGraph("sink");

  // Compartmen
  auto& sub_compartment_1 = compartmentManager.CreateGasCompartment("sub_compartment_1");
  auto& sub_compartment_1_1 = compartmentManager.CreateGasCompartment("sub_compartment_1_1");
  auto& sub_compartment_1_2 = compartmentManager.CreateGasCompartment("sub_compartment_1_2");

  auto& link_1_1_1 = compartmentManager.CreateGasLink(sub_compartment_1, sub_compartment_1_1, "link_1_1_1");
  link_1_1_1.GetFlow().SetValue(2.0, biogears::VolumePerTimeUnit::mL_Per_s);

  auto& link_1_1_2 = compartmentManager.CreateGasLink(sub_compartment_1, sub_compartment_1_2, "link1_1_2");
  link_1_1_2.GetFlow().SetValue(5.5, biogears::VolumePerTimeUnit::mL_Per_s);

  source.AddCompartment(sub_compartment_1);
  source.AddCompartment(sub_compartment_1_1);
  source.AddCompartment(sub_compartment_1_2);

  source.AddLink(link_1_1_1);
  source.AddLink(link_1_1_2);

  EXPECT_NE(source, sink);

  Compartment::UnMarshall(source, data);
  Compartment::Marshall(data, sink, compartmentManager);

  EXPECT_EQ(source, sink);
};