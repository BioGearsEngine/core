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

<<<<<<< HEAD:projects/biogears/libIO/unit/io/cdm/test_core_io_Circuit.cpp
#include <biogears/cdm/Circuit/SECircuit.h>
#include <biogears/cdm/Circuit/SECircuitManager.h>
#include <biogears/cdm/properties/SEProperties.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/utils/Logger.h>
=======
#include <biogears/cdm/circuit/SECircuit.h>
#include <biogears/cdm/circuit/SECircuitManager.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuit.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuitCalculator.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuitNode.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuitPath.h>
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/substance/SESubstanceManager.h>

#include <biogears/cdm/utils/Logger.h>

#include <biogears/cdm/properties/SEProperties.h>
>>>>>>> 07ce2bac (Mid Soluion Fix):projects/biogears/libIO/unit/cdm/test_core_io_Circuit.cpp
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

<<<<<<< HEAD:projects/biogears/libIO/unit/io/cdm/test_core_io_Circuit.cpp
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
//! static void Marshall(const CDM::GasCircuit& in, SEGasCircuit& out);
//! static void UnMarshall(const SEGasCircuit& in, CDM::GasCircuit& out);
#include <biogears/cdm/Circuit/SECircuit.h>
TEST_F(TEST_FIXTURE_NAME, GasCircuit)
{
  EXPECT_TRUE(true);
}
=======
// class SECircuitManager;

// class SEElectricalCircuitNode;
//! TYPE ElectricalCircuitNode
//! static void Marshall(const CDM::ElectricalCircuitNode& in, SEElectricalCircuitNode& out);
//! static void UnMarshall(const SEElectricalCircuitNode& in, CDM::ElectricalCircuitNode& out);
#include <biogears/cdm/circuit/electrical/SEElectricalCircuitNode.h>
TEST_F(TEST_FIXTURE_NAME, ElectricalCircuitNode)
{
  using namespace biogears;
  USING_TYPES(ElectricalCircuitNode)

  //-----------------------------------------------------------
  // Nodes
  auto& source = circuitManager.CreateElectricalNode("Node1");
  auto& sink = circuitManager.CreateElectricalNode("Node2");

  CDMType data;

  EXPECT_NE(source, sink);

  Circuit::UnMarshall(source, data);
  Circuit::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEElectricalCircuitPath;
//! TYPE ElectricalCircuitPath
//! static void Marshall(const CDM::ElectricalCircuitPath& in, SEElectricalCircuitPath& out);
//! static void UnMarshall(const SEElectricalCircuitPath& in, CDM::ElectricalCircuitPath& out);
#include <biogears/cdm/circuit/electrical/SEElectricalCircuitPath.h>
TEST_F(TEST_FIXTURE_NAME, ElectricalCircuitPath)
{
  using namespace biogears;
  USING_TYPES(ElectricalCircuitPath)

  //-----------------------------------------------------------
  // Nodes
  auto& node1 = circuitManager.CreateElectricalNode("Node1");
  auto& node2 = circuitManager.CreateElectricalNode("Node2");
  auto& node3 = circuitManager.CreateElectricalNode("Node3");
  auto& node4 = circuitManager.CreateElectricalNode("Node4");

  SEElectricalCircuitPath& source = circuitManager.CreateElectricalPath(node1, node2, "source");
  source.GetNextResistance().SetValue(25, ElectricResistanceUnit::Ohm);
  SEElectricalCircuitPath& sink = circuitManager.CreateElectricalPath(node3, node4, "sink");
  sink.GetNextResistance().SetValue(25, ElectricResistanceUnit::Ohm);

  CDMType data;

  EXPECT_NE(source, sink);

  Circuit::UnMarshall(source, data);
  Circuit::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEElectricalCircuit;
//! TYPE ElectricalCircuit
//! static void Marshall(const CDM::ElectricalCircuit& in, SEElectricalCircuit& out);
//! static void UnMarshall(const SEElectricalCircuit& in, CDM::ElectricalCircuit& out);
#include <biogears/cdm/circuit/electrical/SEElectricalCircuit.h>
TEST_F(TEST_FIXTURE_NAME, ElectricalCircuit)
{
  using namespace biogears;
  USING_TYPES(ElectricalCircuit)

  CDMType data;

  auto& source = circuitManager.CreateElectricalCircuit("Source");
  auto& sink = circuitManager.CreateElectricalCircuit("Sink");

  source.StateChange();

  //-----------------------------------------------------------
  // Nodes
  SEElectricalCircuitNode& Node1 = source.CreateNode("Node1");
  SEElectricalCircuitNode& Node2 = source.CreateNode("Node2");
  SEElectricalCircuitNode& Node3 = source.CreateNode("Node3");
  SEElectricalCircuitNode& Node4 = source.CreateNode("Node4");

  // Reference node
  source.AddReferenceNode(Node4);
  Node4.GetNextVoltage().SetValue(0, ElectricPotentialUnit::V);
  //-----------------------------------------------------------
  // Paths
  SEElectricalCircuitPath& Path2 = source.CreatePath(Node1, Node2, "Path2");
  Path2.GetNextResistance().SetValue(25, ElectricResistanceUnit::Ohm);
  SEElectricalCircuitPath& Path3 = source.CreatePath(Node2, Node3, "Path3");
  Path3.GetNextResistance().SetValue(25, ElectricResistanceUnit::Ohm);
  SEElectricalCircuitPath& Path4 = source.CreatePath(Node3, Node4, "Path4");
  Path4.GetNextResistance().SetValue(25, ElectricResistanceUnit::Ohm);
  // Pressure source
  SEElectricalCircuitPath& Path1 = source.CreatePath(Node4, Node1, "Path1");
  Path1.GetNextVoltageSource().SetValue(20, ElectricPotentialUnit::V);
  source.StateChange();

  EXPECT_NE(source, sink);

  Circuit::UnMarshall(source, data);
  Circuit::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEFluidCircuitNode;
//! TYPE FluidCircuitNode
//! static void Marshall(const CDM::FluidCircuitNode& in, SEFluidCircuitNode& out);
//! static void UnMarshall(const SEFluidCircuitNode& in, CDM::FluidCircuitNode& out);
#include <biogears/cdm/circuit/fluid/SEFluidCircuitNode.h>
TEST_F(TEST_FIXTURE_NAME, FluidCircuitNode)
{
  using namespace biogears;
  USING_TYPES(FluidCircuitNode)

  //-----------------------------------------------------------
  // Nodes
  auto& source = circuitManager.CreateFluidNode("Node1");
  auto& sink = circuitManager.CreateFluidNode("Node2");

  CDMType data;

  EXPECT_NE(source, sink);

  Circuit::UnMarshall(source, data);
  Circuit::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEFluidCircuitPath;
//! TYPE FluidCircuitPath
//! static void Marshall(const CDM::FluidCircuitPath& in, SEFluidCircuitPath& out);
//! static void UnMarshall(const SEFluidCircuitPath& in, CDM::FluidCircuitPath& out);
#include <biogears/cdm/circuit/fluid/SEFluidCircuitPath.h>
TEST_F(TEST_FIXTURE_NAME, FluidCircuitPath)
{
  using namespace biogears;
  USING_TYPES(FluidCircuitPath)

  //-----------------------------------------------------------
  // Nodes
  auto& node1 = circuitManager.CreateFluidNode("Node1");
  auto& node2 = circuitManager.CreateFluidNode("Node2");
  auto& node3 = circuitManager.CreateFluidNode("Node3");
  auto& node4 = circuitManager.CreateFluidNode("Node4");

  SEFluidCircuitPath& source = circuitManager.CreateFluidPath(node1, node2, "source");
  source.GetNextResistance().SetValue(25, FlowResistanceUnit::cmH2O_s_Per_L);
  SEFluidCircuitPath& sink = circuitManager.CreateFluidPath(node3, node4, "sink");
  sink.GetNextResistance().SetValue(25, FlowResistanceUnit::cmH2O_s_Per_L);

  CDMType data;

  EXPECT_NE(source, sink);

  Circuit::UnMarshall(source, data);
  Circuit::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEFluidCircuit;
//! TYPE FluidCircuit
//! static void Marshall(const CDM::FluidCircuit& in, SEFluidCircuit& out);
//! static void UnMarshall(const SEFluidCircuit& in, CDM::FluidCircuit& out);
#include <biogears/cdm/circuit/fluid/SEFluidCircuit.h>
TEST_F(TEST_FIXTURE_NAME, FluidCircuit)
{
  using namespace biogears;
  USING_TYPES(FluidCircuit)

  CDMType data;

  auto& source = circuitManager.CreateFluidCircuit("Source");
  auto& sink = circuitManager.CreateFluidCircuit("Sink");

  source.StateChange();

  //-----------------------------------------------------------
  // Nodes
  SEFluidCircuitNode& Node1 = source.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = source.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = source.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = source.CreateNode("Node4");

  // Reference node
  source.AddReferenceNode(Node4);
  Node4.GetNextPressure().SetValue(0, PressureUnit::Pa);
  //-----------------------------------------------------------
  // Paths
  SEFluidCircuitPath& Path2 = source.CreatePath(Node1, Node2, "Path2");
  Path2.GetNextResistance().SetValue(25, FlowResistanceUnit::cmH2O_s_Per_L);
  SEFluidCircuitPath& Path3 = source.CreatePath(Node2, Node3, "Path3");
  Path3.GetNextResistance().SetValue(25, FlowResistanceUnit::cmH2O_s_Per_L);
  SEFluidCircuitPath& Path4 = source.CreatePath(Node3, Node4, "Path4");
  Path4.GetNextResistance().SetValue(25, FlowResistanceUnit::cmH2O_s_Per_L);
  // Pressure source
  SEFluidCircuitPath& Path1 = source.CreatePath(Node4, Node1, "Path1");
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);
  source.StateChange();

  EXPECT_NE(source, sink);

  Circuit::UnMarshall(source, data);
  Circuit::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEThermalCircuitNode;
//! TYPE ThermalCircuitNode
//! static void Marshall(const CDM::ThermalCircuitNode& in, SEThermalCircuitNode& out);
//! static void UnMarshall(const SEThermalCircuitNode& in, CDM::ThermalCircuitNode& out);
#include <biogears/cdm/circuit/thermal/SEThermalCircuitNode.h>
TEST_F(TEST_FIXTURE_NAME, ThermalCircuitNode)
{
  using namespace biogears;
  USING_TYPES(ThermalCircuitNode)

  //-----------------------------------------------------------
  // Nodes
  auto& source = circuitManager.CreateThermalNode("Node1");
  auto& sink = circuitManager.CreateThermalNode("Node2");

  CDMType data;

  EXPECT_NE(source, sink);

  Circuit::UnMarshall(source, data);
  Circuit::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEThermalCircuitPath;
//! TYPE ThermalCircuitPath
//! static void Marshall(const CDM::ThermalCircuitPath& in, SEThermalCircuitPath& out);
//! static void UnMarshall(const SEThermalCircuitPath& in, CDM::ThermalCircuitPath& out);
#include <biogears/cdm/circuit/thermal/SEThermalCircuitPath.h>
TEST_F(TEST_FIXTURE_NAME, ThermalCircuitPath)
{
  using namespace biogears;
  USING_TYPES(ThermalCircuitPath)

  //-----------------------------------------------------------
  // Nodes
  auto& node1 = circuitManager.CreateThermalNode("Node1");
  auto& node2 = circuitManager.CreateThermalNode("Node2");
  auto& node3 = circuitManager.CreateThermalNode("Node3");
  auto& node4 = circuitManager.CreateThermalNode("Node4");

  SEThermalCircuitPath& source = circuitManager.CreateThermalPath(node1, node2, "source");
  source.GetNextResistance().SetValue(25, HeatResistanceUnit::C_Per_W);
  SEThermalCircuitPath& sink = circuitManager.CreateThermalPath(node3, node4, "sink");
  sink.GetNextResistance().SetValue(25, HeatResistanceUnit::C_Per_W);

  CDMType data;

  EXPECT_NE(source, sink);

  Circuit::UnMarshall(source, data);
  Circuit::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
// class SEThermalCircuit;
//! TYPE ThermalCircuit
//! static void Marshall(const CDM::ThermalCircuit& in, SEThermalCircuit& out);
//! static void UnMarshall(const SEThermalCircuit& in, CDM::ThermalCircuit& out);
#include <biogears/cdm/circuit/thermal/SEThermalCircuit.h>
TEST_F(TEST_FIXTURE_NAME, ThermalCircuit)
{
  using namespace biogears;
  USING_TYPES(ThermalCircuit)

  CDMType data;

  auto& source = circuitManager.CreateThermalCircuit("Source");
  auto& sink = circuitManager.CreateThermalCircuit("Sink");

  source.StateChange();

  //-----------------------------------------------------------
  // Nodes
  SEThermalCircuitNode& Node1 = source.CreateNode("Node1");
  SEThermalCircuitNode& Node2 = source.CreateNode("Node2");
  SEThermalCircuitNode& Node3 = source.CreateNode("Node3");
  SEThermalCircuitNode& Node4 = source.CreateNode("Node4");

  // Reference node
  source.AddReferenceNode(Node4);
  Node4.GetNextTemperature().SetValue(0, TemperatureUnit::C);
  //-----------------------------------------------------------
  // Paths
  SEThermalCircuitPath& Path2 = source.CreatePath(Node1, Node2, "Path2");
  Path2.GetNextResistance().SetValue(25, HeatResistanceUnit::C_Per_W);
  SEThermalCircuitPath& Path3 = source.CreatePath(Node2, Node3, "Path3");
  Path3.GetNextResistance().SetValue(25, HeatResistanceUnit::C_Per_W);
  SEThermalCircuitPath& Path4 = source.CreatePath(Node3, Node4, "Path4");
  Path4.GetNextResistance().SetValue(25, HeatResistanceUnit::C_Per_W);
  // Pressure source
  SEThermalCircuitPath& Path1 = source.CreatePath(Node4, Node1, "Path1");
  Path1.GetNextTemperatureSource().SetValue(20, TemperatureUnit::C);
  source.StateChange();

  EXPECT_NE(source, sink);

  Circuit::UnMarshall(source, data);
  Circuit::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
>>>>>>> 07ce2bac (Mid Soluion Fix):projects/biogears/libIO/unit/cdm/test_core_io_Circuit.cpp
