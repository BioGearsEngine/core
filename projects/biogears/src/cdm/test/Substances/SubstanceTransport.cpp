/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#include <biogears/cdm/circuit/fluid/SEFluidCircuit.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuitCalculator.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuitNode.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuitPath.h>
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/compartment/fluid/SEFluidCompartment.h>
#include <biogears/cdm/compartment/fluid/SEFluidCompartmentLink.h>
#include <biogears/cdm/compartment/fluid/SEGasCompartmentGraph.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartmentGraph.h>
#include <biogears/cdm/compartment/substances/SELiquidSubstanceQuantity.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/test/CommonDataModelTest.h>
#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>

namespace biogears {
void CommonDataModelTest::LargeFlowTransportTest(const std::string& sTestDirectory)
{
  m_Logger->ResetLogFile(sTestDirectory + "/LargeFlowTransportTest.log");
  SEFluidCircuit& circuit = m_Circuits.CreateFluidCircuit("Circuit");
  SEFluidCircuitCalculator CircuitCalculator(m_Logger);

  // Only give volumes to node that have mass & concentration
  SEFluidCircuitNode& GroundNode = circuit.CreateNode("Ground");
  GroundNode.GetPressure().SetValue(0.0, PressureUnit::mmHg);
  GroundNode.GetVolumeBaseline().SetValue(100.0, VolumeUnit::L);
  circuit.AddReferenceNode(GroundNode);
  SEFluidCircuitNode& Node1 = circuit.CreateNode("Node1");
  Node1.GetVolumeBaseline().SetValue(7.5, VolumeUnit::L);
  SEFluidCircuitNode& Node2 = circuit.CreateNode("Node2");
  Node2.GetVolumeBaseline().SetValue(20.0, VolumeUnit::L);
  // Initialize flow sources that drive the circuit hemodynamics
  SEFluidCircuitPath& groundToNode1 = circuit.CreatePath(GroundNode, Node1, "GroundToNode1");
  groundToNode1.GetFlowSourceBaseline().SetValue(10.0, VolumePerTimeUnit::L_Per_s);
  SEFluidCircuitPath& Node1ToNode2 = circuit.CreatePath(Node1, Node2, "Node1ToNode2");
  SEFluidCircuitPath& Node2ToGround = circuit.CreatePath(Node2, GroundNode, "Node2ToGround");
  circuit.SetNextAndCurrentFromBaselines();
  circuit.StateChange();

  SESubstanceManager subMgr(m_Logger);
  subMgr.LoadSubstanceDirectory();
  SESubstance* CO2 = subMgr.GetSubstance("CarbonDioxide");

  SECompartmentManager cmptMgr(subMgr);
  cmptMgr.AddLiquidCompartmentSubstance(*CO2);
  // Create a Compartment graph for transport
  // Initialize Substances via Volume Fraction
  SELiquidCompartmentGraph* graph = &cmptMgr.CreateLiquidGraph("Graph");
  SELiquidCompartment& GroundCmpt = cmptMgr.CreateLiquidCompartment(GroundNode.GetName());
  GroundCmpt.MapNode(GroundNode);
  graph->AddCompartment(GroundCmpt);
  GroundCmpt.GetSubstanceQuantity(*CO2)->GetConcentration().SetValue(1.0, MassPerVolumeUnit::g_Per_L);
  GroundCmpt.Balance(BalanceLiquidBy::Concentration);
  SELiquidCompartment& Cmpt1 = cmptMgr.CreateLiquidCompartment(Node1.GetName());
  Cmpt1.MapNode(Node1);
  graph->AddCompartment(Cmpt1);
  Cmpt1.GetSubstanceQuantity(*CO2)->GetConcentration().SetValue(0.5, MassPerVolumeUnit::g_Per_L);
  Cmpt1.Balance(BalanceLiquidBy::Concentration);
  SELiquidCompartment& Cmpt2 = cmptMgr.CreateLiquidCompartment(Node2.GetName());
  Cmpt2.MapNode(Node2);
  graph->AddCompartment(Cmpt2);
  Cmpt2.GetSubstanceQuantity(*CO2)->GetConcentration().SetValue(0.0, MassPerVolumeUnit::g_Per_L);
  Cmpt2.Balance(BalanceLiquidBy::Concentration);
  // Make our paths, remember to create your NodeCompartments before you create PathLinks
  SELiquidCompartmentLink& GroundToCmpt1 = cmptMgr.CreateLiquidLink(GroundCmpt, Cmpt1, groundToNode1.GetName());
  GroundToCmpt1.MapPath(groundToNode1);
  SELiquidCompartmentLink& Cmpt1ToCmpt2 = cmptMgr.CreateLiquidLink(Cmpt1, Cmpt2, std::string{ Cmpt1.GetName() } +"To" + Cmpt2.GetName());
  Cmpt1ToCmpt2.MapPath(Node1ToNode2); // Need to map it yourself, CDM does not know where you want to put it...
  SELiquidCompartmentLink& Cmpt2ToGround = cmptMgr.CreateLiquidLink(Cmpt2, GroundCmpt, std::string{ Cmpt2.GetName() } +"To" + GroundCmpt.GetName());
  Cmpt2ToGround.MapPath(Node2ToGround); // Need to map it yourself, CDM does not know where you want to put it...
  graph->AddLink(GroundToCmpt1);
  graph->AddLink(Cmpt1ToCmpt2);
  graph->AddLink(Cmpt2ToGround);
  cmptMgr.StateChange();

  SELiquidTransporter Transporter(VolumePerTimeUnit::mL_Per_s, VolumeUnit::mL, MassUnit::ug, MassPerVolumeUnit::ug_Per_mL, m_Logger);

  DataTrack outTrk;
  std::ofstream file;
  bool serialized = false;
  double time = 0;
  double deltaT = 1.0;
  while (time < 10.0) {
    double totalMass = 0.0;
    for (SELiquidCompartment* cmpt : graph->GetCompartments()) {
      outTrk.Probe(std::string{ cmpt->GetName() }+ "Mass(g)", cmpt->GetSubstanceQuantity(*CO2)->GetMass(MassUnit::g));
      outTrk.Probe(std::string{ cmpt->GetName() }+ "Concentration(g/L)", cmpt->GetSubstanceQuantity(*CO2)->GetConcentration(MassPerVolumeUnit::g_Per_L));
      outTrk.Probe(std::string{ cmpt->GetName() }+ "Volume(L)", cmpt->GetVolume(VolumeUnit::L));
      totalMass += cmpt->GetSubstanceQuantity(*CO2)->GetMass(MassUnit::g);
    }
    for (SELiquidCompartmentLink* link : graph->GetLinks()) {
      if (link->HasFlow())
        outTrk.Probe(std::string{ link->GetName()  }+ "Flow(L/s)", link->GetFlow(VolumePerTimeUnit::L_Per_s));
      else
        outTrk.Probe(std::string{ link->GetName()  }+ "Flow(L/s)", 0);
    }
    outTrk.Probe("TotalMass(g)", totalMass);
    if (time == 0)
      outTrk.CreateFile(std::string(sTestDirectory + "/LargeFlowTransport.csv").c_str(), file);
    outTrk.StreamProbesToFile(time, file);

    CircuitCalculator.Process(circuit, deltaT);
    Transporter.Transport(*graph, deltaT);
    CircuitCalculator.PostProcess(circuit);
    time += deltaT;

    if (!serialized && time > 8.0) {
      serialized = true;
      TestCompartmentSerialization(cmptMgr, sTestDirectory + "/LargeTransportCompartments.xml");
      graph = cmptMgr.GetLiquidGraph("Graph");
    }
  }
  file.close();
  outTrk.Clear();
  m_Circuits.Clear();
}

void CommonDataModelTest::LiquidTransportTest(const std::string& rptDirectory)
{
  Logger logger(rptDirectory + "/LiquidTransportTest.log");
  SEFluidCircuit& circuit = m_Circuits.CreateFluidCircuit("Circuit");
  SEFluidCircuitCalculator CircuitCalculator(&logger);

  SEFluidCircuitNode& Ground = circuit.CreateNode("Ground");
  circuit.AddReferenceNode(Ground);
  Ground.GetPressure().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitNode& Node1 = circuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = circuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = circuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = circuit.CreateNode("Node4");
  SEFluidCircuitNode& Node5 = circuit.CreateNode("Node5");
  SEFluidCircuitNode& Node6 = circuit.CreateNode("Node6");
  SEFluidCircuitNode& Node7 = circuit.CreateNode("Node7");
  SEFluidCircuitNode& Node8 = circuit.CreateNode("Node8");
  SEFluidCircuitNode& Node9 = circuit.CreateNode("Node9");
  SEFluidCircuitNode& Node10 = circuit.CreateNode("Node10"); //Center Node
  SEFluidCircuitNode& Node11 = circuit.CreateNode("Node11");
  SEFluidCircuitNode& Node12 = circuit.CreateNode("Node12");
  SEFluidCircuitNode& Node13 = circuit.CreateNode("Node13");
  SEFluidCircuitNode& Node14 = circuit.CreateNode("Node14");
  SEFluidCircuitNode& Node15 = circuit.CreateNode("Node15");
  SEFluidCircuitNode& Node16 = circuit.CreateNode("Node16");
  SEFluidCircuitNode& Node17 = circuit.CreateNode("Node17");
  SEFluidCircuitNode& Node18 = circuit.CreateNode("Node18");
  SEFluidCircuitNode& Node19 = circuit.CreateNode("Node19");

  //Only give volumes to node that have mass & concentration
  Ground.GetVolumeBaseline().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::mL);
  Node1.GetVolumeBaseline().SetValue(1.0, VolumeUnit::mL);
  Node4.GetVolumeBaseline().SetValue(1.0, VolumeUnit::mL);
  Node5.GetVolumeBaseline().SetValue(1.0, VolumeUnit::mL);
  Node8.GetVolumeBaseline().SetValue(1.0, VolumeUnit::mL);
  Node11.GetVolumeBaseline().SetValue(1.0, VolumeUnit::mL);
  Node13.GetVolumeBaseline().SetValue(1.0, VolumeUnit::mL);
  Node15.GetVolumeBaseline().SetValue(1.0, VolumeUnit::mL);
  Node16.GetVolumeBaseline().SetValue(1.0, VolumeUnit::mL);
  Node19.GetVolumeBaseline().SetValue(1.0, VolumeUnit::mL);

  //Initialize flow sources that drive the transport process
  SEFluidCircuitPath& groundToNode1 = circuit.CreatePath(Ground, Node1, "GroundToNode1");
  groundToNode1.GetFlowSourceBaseline().SetValue(1.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& groundToNode2 = circuit.CreatePath(Ground, Node2, "GroundToNode2");
  groundToNode2.GetFlowSourceBaseline().SetValue(0.5, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& groundToNode3 = circuit.CreatePath(Ground, Node3, "GroundToNode3");
  groundToNode3.GetFlowSourceBaseline().SetValue(1.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& groundToNode4 = circuit.CreatePath(Ground, Node4, "GroundToNode4");
  groundToNode4.GetFlowSourceBaseline().SetValue(0.5, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& groundToNode5 = circuit.CreatePath(Ground, Node5, "GroundToNode5");
  groundToNode5.GetFlowSourceBaseline().SetValue(1.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& groundToNode6 = circuit.CreatePath(Ground, Node6, "GroundToNode6");
  groundToNode6.GetFlowSourceBaseline().SetValue(0.5, VolumePerTimeUnit::mL_Per_s);

  SEFluidCircuitPath& Node1ToNode7 = circuit.CreatePath(Node1, Node7, "Node1ToNode7");
  SEFluidCircuitPath& Node2ToNode7 = circuit.CreatePath(Node2, Node7, "Node2ToNode7");
  SEFluidCircuitPath& Node3ToNode8 = circuit.CreatePath(Node3, Node8, "Node3ToNode8");
  SEFluidCircuitPath& Node4ToNode8 = circuit.CreatePath(Node4, Node8, "Node4ToNode8");
  SEFluidCircuitPath& Node5ToNode9 = circuit.CreatePath(Node5, Node9, "Node5ToNode9");
  SEFluidCircuitPath& Node6ToNode9 = circuit.CreatePath(Node6, Node9, "Node6ToNode9");
  SEFluidCircuitPath& Node7ToNode10 = circuit.CreatePath(Node7, Node10, "Node7ToNode10");
  SEFluidCircuitPath& Node8ToNode10 = circuit.CreatePath(Node8, Node10, "Node8ToNode10");
  SEFluidCircuitPath& Node9ToNode10 = circuit.CreatePath(Node9, Node10, "Node9ToNode10");
  SEFluidCircuitPath& Node10ToNode11 = circuit.CreatePath(Node10, Node11, "Node10ToNode11");
  SEFluidCircuitPath& Node10ToNode12 = circuit.CreatePath(Node10, Node12, "Node10ToNode12");
  SEFluidCircuitPath& Node10ToNode13 = circuit.CreatePath(Node10, Node13, "Node10ToNode13");
  SEFluidCircuitPath& Node11ToNode14 = circuit.CreatePath(Node11, Node14, "Node11ToNode14");
  SEFluidCircuitPath& Node11ToNode15 = circuit.CreatePath(Node11, Node15, "Node11ToNode15");
  SEFluidCircuitPath& Node12ToNode16 = circuit.CreatePath(Node12, Node16, "Node12ToNode16");
  SEFluidCircuitPath& Node12ToNode17 = circuit.CreatePath(Node12, Node17, "Node12ToNode17");
  SEFluidCircuitPath& Node13ToNode18 = circuit.CreatePath(Node13, Node18, "Node13ToNode18");
  SEFluidCircuitPath& Node13ToNode19 = circuit.CreatePath(Node13, Node19, "Node13ToNode19");

  SEFluidCircuitPath& Node14ToGround = circuit.CreatePath(Node14, Ground, "Node14ToGround");
  Node14ToGround.GetFlowSourceBaseline().SetValue(0.5, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& Node15ToGround = circuit.CreatePath(Node15, Ground, "Node15ToGround");
  Node15ToGround.GetFlowSourceBaseline().SetValue(1.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& Node16ToGround = circuit.CreatePath(Node16, Ground, "Node16ToGround");
  Node16ToGround.GetFlowSourceBaseline().SetValue(0.5, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& Node17ToGround = circuit.CreatePath(Node17, Ground, "Node17ToGround");
  Node17ToGround.GetFlowSourceBaseline().SetValue(1.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& Node18ToGround = circuit.CreatePath(Node18, Ground, "Node18ToGround");
  Node18ToGround.GetFlowSourceBaseline().SetValue(0.5, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& Node19ToGround = circuit.CreatePath(Node19, Ground, "Node19ToGround");
  Node19ToGround.GetFlowSourceBaseline().SetValue(1.0, VolumePerTimeUnit::mL_Per_s);
  circuit.SetNextAndCurrentFromBaselines();
  circuit.StateChange();

  SESubstanceManager subMgr(&logger);
  subMgr.LoadSubstanceDirectory();
  SESubstance* CO2 = subMgr.GetSubstance("CarbonDioxide");
  SESubstance* Hb = subMgr.GetSubstance("Hemoglobin");

  SECompartmentManager cmptMgr(subMgr);
  cmptMgr.AddLiquidCompartmentSubstance(*CO2);
  cmptMgr.AddLiquidCompartmentSubstance(*Hb);

  // Create the transport graph
  SELiquidCompartmentGraph* graph = &cmptMgr.CreateLiquidGraph("Graph");
  SELiquidCompartment& GroundCmpt = cmptMgr.CreateLiquidCompartment(Ground.GetName());
  GroundCmpt.MapNode(Ground);
  graph->AddCompartment(GroundCmpt);
  GroundCmpt.GetVolume().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::mL);
  SELiquidSubstanceQuantity* gndQ = GroundCmpt.GetSubstanceQuantity(*CO2);
  SELiquidSubstanceQuantity* gndHbQ = GroundCmpt.GetSubstanceQuantity(*Hb);
  gndQ->GetConcentration().SetValue(2.0, MassPerVolumeUnit::mg_Per_mL);
  gndHbQ->GetConcentration().SetValue(2.0, MassPerVolumeUnit::mg_Per_mL);
  gndQ->GetMass().SetValue(std::numeric_limits<double>::infinity(), MassUnit::mg);
  gndHbQ->GetMass().SetValue(std::numeric_limits<double>::infinity(), MassUnit::mg);
  for (SEFluidCircuitNode* n : circuit.GetNodes()) {
    if (n == &Ground)
      continue;
    SELiquidCompartment& cmpt = cmptMgr.CreateLiquidCompartment(n->GetName());
    cmpt.MapNode(*n);
    graph->AddCompartment(cmpt);
    if (cmpt.HasVolume()) {
      cmpt.GetSubstanceQuantity(*CO2)->GetConcentration().SetValue(1.0, MassPerVolumeUnit::g_Per_L);
      cmpt.GetSubstanceQuantity(*Hb)->GetConcentration().SetValue(1.0, MassPerVolumeUnit::g_Per_L);
      cmpt.Balance(BalanceLiquidBy::Concentration);
    } else {
      cmpt.GetSubstanceQuantity(*CO2);
      cmpt.GetSubstanceQuantity(*Hb);
    }
  }
  for (SEFluidCircuitPath* p : circuit.GetPaths()) {
    SELiquidCompartmentLink& link = cmptMgr.CreateLiquidLink(*cmptMgr.GetLiquidCompartment(p->GetSourceNode().GetName()),
      *cmptMgr.GetLiquidCompartment(p->GetTargetNode().GetName()), p->GetName());
    link.MapPath(*p);
    graph->AddLink(link);
  }
  cmptMgr.StateChange();

  bool serialized = false;
  SELiquidTransporter Transporter(VolumePerTimeUnit::mL_Per_s, VolumeUnit::mL, MassUnit::mg, MassPerVolumeUnit::mg_Per_mL, &logger);
  double deltaT_s = 1.0 / 165;
  DataTrack outTrk;
  std::ofstream file;
  double time = 0;
  while (time < 10.0) {
    CircuitCalculator.Process(circuit, deltaT_s);
    Transporter.Transport(*graph, deltaT_s);
    CircuitCalculator.PostProcess(circuit);

    for (SELiquidCompartmentLink* link : graph->GetLinks()) {
      outTrk.Probe(std::string{ link->GetName()  }+ "Flow", link->GetFlow(VolumePerTimeUnit::mL_Per_s));
    }
    for (SELiquidCompartment* cmpt : graph->GetCompartments()) {
      outTrk.Probe(std::string{ cmpt->GetName() }+ "Volume", cmpt->HasVolume() ? cmpt->GetVolume(VolumeUnit::mL) : SEScalar::dNaN());
      outTrk.Probe(std::string{ cmpt->GetName() }+ "Pressure", cmpt->HasPressure() ? cmpt->GetPressure(PressureUnit::mmHg) : SEScalar::dNaN());
      SELiquidSubstanceQuantity* CO2Q = cmpt->GetSubstanceQuantity(*CO2);
      SELiquidSubstanceQuantity* HbQ = cmpt->GetSubstanceQuantity(*Hb);
      outTrk.Probe(std::string{ cmpt->GetName() }+ "MassCO2", CO2Q->HasMass() ? CO2Q->GetMass(MassUnit::mg) : SEScalar::dNaN());
      outTrk.Probe(std::string{ cmpt->GetName() }+ "MassHb", HbQ->HasMass() ? HbQ->GetMass(MassUnit::mg) : SEScalar::dNaN());
      outTrk.Probe(std::string{ cmpt->GetName() }+ "ConcentrationCO2", CO2Q->HasConcentration() ? CO2Q->GetConcentration(MassPerVolumeUnit::mg_Per_mL) : SEScalar::dNaN());
      outTrk.Probe(std::string{ cmpt->GetName() }+ "ConcentrationHb", HbQ->HasConcentration() ? HbQ->GetConcentration(MassPerVolumeUnit::mg_Per_mL) : SEScalar::dNaN());
    }

    if (!serialized && time > 8.0) {
      serialized = true;
      TestCompartmentSerialization(cmptMgr, rptDirectory + "/LiquidTransportTest.xml");
      graph = cmptMgr.GetLiquidGraph("Graph");
    }

    if (time == 0)
      outTrk.CreateFile(std::string(rptDirectory + "/LiquidTransport.csv").c_str(), file);
    outTrk.StreamProbesToFile(time, file);
    time += deltaT_s;
  }
  file.close();
  outTrk.Clear();
  m_Circuits.Clear();
}

void CommonDataModelTest::GasTransportTest(const std::string& rptDirectory)
{
  Logger logger(rptDirectory + "/GasTransportTest.log");
  SEFluidCircuit& circuit = m_Circuits.CreateFluidCircuit("Circuit");
  SEFluidCircuitCalculator CircuitCalculator(&logger);

  SEFluidCircuitNode& Ground = circuit.CreateNode("Ground");
  circuit.AddReferenceNode(Ground);
  Ground.GetPressure().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitNode& Node1 = circuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = circuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = circuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = circuit.CreateNode("Node4");
  SEFluidCircuitNode& Node5 = circuit.CreateNode("Node5");
  SEFluidCircuitNode& Node6 = circuit.CreateNode("Node6");
  SEFluidCircuitNode& Node7 = circuit.CreateNode("Node7");
  SEFluidCircuitNode& Node8 = circuit.CreateNode("Node8");
  SEFluidCircuitNode& Node9 = circuit.CreateNode("Node9");
  SEFluidCircuitNode& Node10 = circuit.CreateNode("Node10"); //Center Node
  SEFluidCircuitNode& Node11 = circuit.CreateNode("Node11");
  SEFluidCircuitNode& Node12 = circuit.CreateNode("Node12");
  SEFluidCircuitNode& Node13 = circuit.CreateNode("Node13");
  SEFluidCircuitNode& Node14 = circuit.CreateNode("Node14");
  SEFluidCircuitNode& Node15 = circuit.CreateNode("Node15");
  SEFluidCircuitNode& Node16 = circuit.CreateNode("Node16");
  SEFluidCircuitNode& Node17 = circuit.CreateNode("Node17");
  SEFluidCircuitNode& Node18 = circuit.CreateNode("Node18");
  SEFluidCircuitNode& Node19 = circuit.CreateNode("Node19");

  //Only give volumes to node that have mass & concentration
  Ground.GetVolumeBaseline().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::mL);
  Node1.GetVolumeBaseline().SetValue(1.0, VolumeUnit::mL);
  Node4.GetVolumeBaseline().SetValue(1.0, VolumeUnit::mL);
  Node5.GetVolumeBaseline().SetValue(1.0, VolumeUnit::mL);
  Node8.GetVolumeBaseline().SetValue(1.0, VolumeUnit::mL);
  Node11.GetVolumeBaseline().SetValue(1.0, VolumeUnit::mL);
  Node13.GetVolumeBaseline().SetValue(1.0, VolumeUnit::mL);
  Node15.GetVolumeBaseline().SetValue(1.0, VolumeUnit::mL);
  Node16.GetVolumeBaseline().SetValue(1.0, VolumeUnit::mL);
  Node19.GetVolumeBaseline().SetValue(1.0, VolumeUnit::mL);

  //Initialize flow sources that drive the transport process
  SEFluidCircuitPath& groundToNode1 = circuit.CreatePath(Ground, Node1, "GroundToNode1");
  groundToNode1.GetFlowSourceBaseline().SetValue(1.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& groundToNode2 = circuit.CreatePath(Ground, Node2, "GroundToNode2");
  groundToNode2.GetFlowSourceBaseline().SetValue(0.5, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& groundToNode3 = circuit.CreatePath(Ground, Node3, "GroundToNode3");
  groundToNode3.GetFlowSourceBaseline().SetValue(1.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& groundToNode4 = circuit.CreatePath(Ground, Node4, "GroundToNode4");
  groundToNode4.GetFlowSourceBaseline().SetValue(0.5, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& groundToNode5 = circuit.CreatePath(Ground, Node5, "GroundToNode5");
  groundToNode5.GetFlowSourceBaseline().SetValue(1.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& groundToNode6 = circuit.CreatePath(Ground, Node6, "GroundToNode6");
  groundToNode6.GetFlowSourceBaseline().SetValue(0.5, VolumePerTimeUnit::mL_Per_s);

  SEFluidCircuitPath& Node1ToNode7 = circuit.CreatePath(Node1, Node7, "Node1ToNode7");
  SEFluidCircuitPath& Node2ToNode7 = circuit.CreatePath(Node2, Node7, "Node2ToNode7");
  SEFluidCircuitPath& Node3ToNode8 = circuit.CreatePath(Node3, Node8, "Node3ToNode8");
  SEFluidCircuitPath& Node4ToNode8 = circuit.CreatePath(Node4, Node8, "Node4ToNode8");
  SEFluidCircuitPath& Node5ToNode9 = circuit.CreatePath(Node5, Node9, "Node5ToNode9");
  SEFluidCircuitPath& Node6ToNode9 = circuit.CreatePath(Node6, Node9, "Node6ToNode9");
  SEFluidCircuitPath& Node7ToNode10 = circuit.CreatePath(Node7, Node10, "Node7ToNode10");
  SEFluidCircuitPath& Node8ToNode10 = circuit.CreatePath(Node8, Node10, "Node8ToNode10");
  SEFluidCircuitPath& Node9ToNode10 = circuit.CreatePath(Node9, Node10, "Node9ToNode10");
  SEFluidCircuitPath& Node10ToNode11 = circuit.CreatePath(Node10, Node11, "Node10ToNode11");
  SEFluidCircuitPath& Node10ToNode12 = circuit.CreatePath(Node10, Node12, "Node10ToNode12");
  SEFluidCircuitPath& Node10ToNode13 = circuit.CreatePath(Node10, Node13, "Node10ToNode13");
  SEFluidCircuitPath& Node11ToNode14 = circuit.CreatePath(Node11, Node14, "Node11ToNode14");
  SEFluidCircuitPath& Node11ToNode15 = circuit.CreatePath(Node11, Node15, "Node11ToNode15");
  SEFluidCircuitPath& Node12ToNode16 = circuit.CreatePath(Node12, Node16, "Node12ToNode16");
  SEFluidCircuitPath& Node12ToNode17 = circuit.CreatePath(Node12, Node17, "Node12ToNode17");
  SEFluidCircuitPath& Node13ToNode18 = circuit.CreatePath(Node13, Node18, "Node13ToNode18");
  SEFluidCircuitPath& Node13ToNode19 = circuit.CreatePath(Node13, Node19, "Node13ToNode19");

  SEFluidCircuitPath& Node14ToGround = circuit.CreatePath(Node14, Ground, "Node14ToGround");
  Node14ToGround.GetFlowSourceBaseline().SetValue(0.5, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& Node15ToGround = circuit.CreatePath(Node15, Ground, "Node15ToGround");
  Node15ToGround.GetFlowSourceBaseline().SetValue(1.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& Node16ToGround = circuit.CreatePath(Node16, Ground, "Node16ToGround");
  Node16ToGround.GetFlowSourceBaseline().SetValue(0.5, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& Node17ToGround = circuit.CreatePath(Node17, Ground, "Node17ToGround");
  Node17ToGround.GetFlowSourceBaseline().SetValue(1.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& Node18ToGround = circuit.CreatePath(Node18, Ground, "Node18ToGround");
  Node18ToGround.GetFlowSourceBaseline().SetValue(0.5, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& Node19ToGround = circuit.CreatePath(Node19, Ground, "Node19ToGround");
  Node19ToGround.GetFlowSourceBaseline().SetValue(1.0, VolumePerTimeUnit::mL_Per_s);
  circuit.SetNextAndCurrentFromBaselines();
  circuit.StateChange();

  SESubstanceManager subMgr(&logger);
  subMgr.LoadSubstanceDirectory();
  SESubstance* O2 = subMgr.GetSubstance("Oxygen");
  SESubstance* CO2 = subMgr.GetSubstance("CarbonDioxide");

  SECompartmentManager cmptMgr(subMgr);
  cmptMgr.AddGasCompartmentSubstance(*O2);
  cmptMgr.AddGasCompartmentSubstance(*CO2);
  // Create the transport graph
  SEGasCompartmentGraph* graph = &cmptMgr.CreateGasGraph("Graph");
  SEGasCompartment& GroundCmpt = cmptMgr.CreateGasCompartment(Ground.GetName());
  GroundCmpt.MapNode(Ground);
  graph->AddCompartment(GroundCmpt);
  GroundCmpt.GetVolume().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::mL);
  SEGasSubstanceQuantity* gndO2 = GroundCmpt.GetSubstanceQuantity(*O2);
  gndO2->GetVolumeFraction().SetValue(0.25);
  gndO2->GetVolume().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::mL);
  SEGasSubstanceQuantity* gndCO2 = GroundCmpt.GetSubstanceQuantity(*CO2);
  gndCO2->GetVolumeFraction().SetValue(0.75);
  gndCO2->GetVolume().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::mL);

  for (SEFluidCircuitNode* n : circuit.GetNodes()) {
    if (n == &Ground)
      continue;
    SEGasCompartment& cmpt = cmptMgr.CreateGasCompartment(n->GetName());
    cmpt.MapNode(*n);
    graph->AddCompartment(cmpt);
    if (cmpt.HasVolume()) {
      cmpt.GetSubstanceQuantity(*O2)->GetVolume().SetValue(0.5, VolumeUnit::mL);
      cmpt.GetSubstanceQuantity(*CO2)->GetVolume().SetValue(0.5, VolumeUnit::mL);
      cmpt.Balance(BalanceGasBy::Volume);
    }
  }
  for (SEFluidCircuitPath* p : circuit.GetPaths()) {
    SEGasCompartmentLink& link = cmptMgr.CreateGasLink(*cmptMgr.GetGasCompartment(p->GetSourceNode().GetName()),
      *cmptMgr.GetGasCompartment(p->GetTargetNode().GetName()), p->GetName());
    link.MapPath(*p);
    graph->AddLink(link);
  }
  cmptMgr.StateChange();

  bool serialized = false;
  SEGasTransporter Transporter(VolumePerTimeUnit::mL_Per_s, VolumeUnit::mL, VolumeUnit::mL, NoUnit::unitless, &logger);
  double deltaT_s = 1.0 / 165;
  DataTrack outTrk;
  std::ofstream file;
  double time = 0;
  while (time < 10.0) {
    CircuitCalculator.Process(circuit, deltaT_s);
    Transporter.Transport(*graph, deltaT_s);
    CircuitCalculator.PostProcess(circuit);

    for (SEGasCompartmentLink* link : graph->GetLinks()) {
      outTrk.Probe(std::string{ link->GetName()  }+ "Flow", link->GetFlow(VolumePerTimeUnit::mL_Per_s));
    }
    for (SEGasCompartment* cmpt : graph->GetCompartments()) {
      outTrk.Probe(std::string{ cmpt->GetName() }+ "Volume", cmpt->HasVolume() ? cmpt->GetVolume(VolumeUnit::mL) : SEScalar::dNaN());
      outTrk.Probe(std::string{ cmpt->GetName() }+ "Pressure", cmpt->HasPressure() ? cmpt->GetPressure(PressureUnit::mmHg) : SEScalar::dNaN());
      SEGasSubstanceQuantity* O2Q = cmpt->GetSubstanceQuantity(*O2);
      outTrk.Probe(std::string{ cmpt->GetName() }+ "O2-Volume", O2Q->HasVolume() ? O2Q->GetVolume(VolumeUnit::mL) : SEScalar::dNaN());
      outTrk.Probe(std::string{ cmpt->GetName() }+ "O2-VolumeFraction", O2Q->HasVolumeFraction() ? O2Q->GetVolumeFraction().GetValue() : SEScalar::dNaN());
      SEGasSubstanceQuantity* CO2Q = cmpt->GetSubstanceQuantity(*CO2);
      outTrk.Probe(std::string{ cmpt->GetName() }+ "CO2-Volume", CO2Q->HasVolume() ? CO2Q->GetVolume(VolumeUnit::mL) : SEScalar::dNaN());
      outTrk.Probe(std::string{ cmpt->GetName() }+ "CO2-VolumeFraction", CO2Q->HasVolumeFraction() ? CO2Q->GetVolumeFraction().GetValue() : SEScalar::dNaN());
      // Delete this when rebaselined
      outTrk.Probe(std::string{ cmpt->GetName() }+ "Volume", CO2Q->HasVolume() ? CO2Q->GetVolume(VolumeUnit::mL) : SEScalar::dNaN());
      outTrk.Probe(std::string{ cmpt->GetName() }+ "VolumeFraction", CO2Q->HasVolumeFraction() ? CO2Q->GetVolumeFraction().GetValue() : SEScalar::dNaN());
    }

    if (!serialized && time > 8.0) {
      serialized = true;
      TestCompartmentSerialization(cmptMgr, rptDirectory + "/GasTransportTest.xml");
      graph = cmptMgr.GetGasGraph("Graph");
    }

    if (time == 0)
      outTrk.CreateFile(std::string(rptDirectory + "/GasTransport.csv").c_str(), file);
    outTrk.StreamProbesToFile(time, file);
    time += deltaT_s;
  }
  file.close();
  outTrk.Clear();
  m_Circuits.Clear();
}
}