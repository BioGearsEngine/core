//public function like test

#include "biogears/cdm/circuit/electrical/SEElectricalCircuitCalculator.h"

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/circuit/SECircuit.h>
#include <biogears/cdm/circuit/SECircuitManager.h>
#include <biogears/engine/Controller/BioGears.h>
#include <biogears/engine/Controller/BioGearsCircuits.h>
#include <string>
#include <vector>
#include <biogears/chrono/stop_watch.tci.h>


class CircuitTester {
public:
  CircuitTester();
  ~CircuitTester();
  void setup(std::string name); //<Setup log files and any required componets
  void step(std::string circuit, int interations); //Solve each circuit
  void teardown(); //< Delete any allocated circuits
  biogears::DataTrack* trk;

protected:
  biogears::BioGears* bg;
  biogears::Logger* m_Logger;
  double timeStep_s;
  double currentTime_s;
};
CircuitTester::CircuitTester()
{
}
CircuitTester::~CircuitTester()
{
}

class FluidCircuitTester : public CircuitTester {
public:
  FluidCircuitTester();
  ~FluidCircuitTester();
  void RunDCTest(std::string name, unsigned int i);
  void RunSINTest(std::string name, unsigned int i);
  void RunPULSETest(std::string name, unsigned int i); //biogears > pulse
  void RunZEROCENTEREDSINTest(std::string name, unsigned int i);
  void SeriesRDCTest(); //0
  void SeriesRCSINTest(); //0
  void SeriesRCPULSETest(); //0
  void SeriesRDCTest2(); //1
  void SeriesRSINTest(); //1
  void SeriesRPULSETest(); //1
  void SeriesRLDCTest(); //2
  void SeriesRLSINTest(); //2
  void SeriesRLPULSETest(); //2
  void ParallelRDCTest(); //3
  void ParallelRSINTest(); //3
  void ParalleRPULSETest(); //3
  void ParallelRCDCTest(); //4
  void ParallelRCSINTest(); //4
  void ParallelRCPULSETest(); //4
  void ParallelRLDCTest(); //5
  void ParallelRLSINTest(); //5
  void ParallelRLPULSETest(); //5
  void ParallelRLSINCenteredTest(); //5
  void SeriesRLCDCTest(); //6
  void ParallelRLCSINTest(); //6
  void ParallelRLCPULSETest(); //6
  void ParallelRLCDCTest(); //7

  void BasicDiodeDCCurrent(); //18
  void step();
  void teardown();
  void BasicCircuitTest();

private:
  void TestCircuitSerialization(const std::string& fileName);
  void TestPreProcess1(double dT, int i);
  void TestPreProcess2(double dT, int i);
  void TestPreProcess3(double dT, int i);
  void TestPreProcess4(double dT, int i);
  bool serialized;
  double outputTime_s;
  biogears::SECircuitManager* m_Circuits;
  biogears::SEFluidCircuitCalculator* fluidCalculator;
  biogears::Logger* m_Logger;
  biogears::SEFluidCircuit* fluidCircuit;
};

FluidCircuitTester::FluidCircuitTester()
{
  trk = new biogears::DataTrack();
  m_Logger = new biogears::Logger("TestResults/FluidCircuit.log");
  m_Circuits = new biogears::SECircuitManager(m_Logger);
  serialized = false;
  outputTime_s = 0.0;
}

FluidCircuitTester::~FluidCircuitTester()
{
  delete m_Circuits;
  delete m_Logger;
}

void FluidCircuitTester::RunDCTest(std::string name, unsigned int i)
{
  biogears::Logger* test_Logger = new biogears::Logger("TestResults/" + name + ".log");
  timeStep_s = (1.0 / 165.0);
  currentTime_s = 0.0;
  fluidCalculator = new biogears::SEFluidCircuitCalculator(test_Logger);
  fluidCircuit = m_Circuits->GetFluidCircuit("Fluid");
  fluidCircuit->SetNextAndCurrentFromBaselines();
  fluidCircuit->StateChange();
  while (currentTime_s < 10) {
    TestPreProcess1(currentTime_s, i);
    fluidCalculator->Process(*fluidCircuit, timeStep_s);
    fluidCalculator->PostProcess(*fluidCircuit);
    trk->Track(currentTime_s, *fluidCircuit);
    if (!serialized && currentTime_s > 8) {
      serialized = true;
      TestCircuitSerialization(name);
      fluidCircuit = m_Circuits->GetFluidCircuit("Fluid");
    }
    currentTime_s += timeStep_s;
  }
}

void FluidCircuitTester::RunSINTest(std::string name, unsigned int i)
{
  biogears::Logger* test_Logger = new biogears::Logger("TestResults/" + name + ".log");
  timeStep_s = (1.0 / 165.0);
  currentTime_s = 0.0;
  fluidCalculator = new biogears::SEFluidCircuitCalculator(test_Logger);
  fluidCircuit = m_Circuits->GetFluidCircuit("Fluid");
  fluidCircuit->SetNextAndCurrentFromBaselines();
  fluidCircuit->StateChange();
  while (currentTime_s < 10) {
    TestPreProcess2(currentTime_s, i);
    fluidCalculator->Process(*fluidCircuit, timeStep_s);
    fluidCalculator->PostProcess(*fluidCircuit);
    trk->Track(currentTime_s, *fluidCircuit);
    if (!serialized && currentTime_s > 8) {
      serialized = true;
      TestCircuitSerialization(name);
      fluidCircuit = m_Circuits->GetFluidCircuit("Fluid");
    }
    currentTime_s += timeStep_s;
  }
}

void FluidCircuitTester::RunPULSETest(std::string name, unsigned int i)
{
  biogears::Logger* test_Logger = new biogears::Logger("TestResults/" + name + ".log");
  timeStep_s = (1.0 / 165.0);
  currentTime_s = 0.0;
  fluidCalculator = new biogears::SEFluidCircuitCalculator(test_Logger);
  fluidCircuit = m_Circuits->GetFluidCircuit("Fluid");
  fluidCircuit->SetNextAndCurrentFromBaselines();
  fluidCircuit->StateChange();
  while (currentTime_s < 10) {
    TestPreProcess3(currentTime_s, i);
    fluidCalculator->Process(*fluidCircuit, timeStep_s);
    fluidCalculator->PostProcess(*fluidCircuit);
    trk->Track(currentTime_s, *fluidCircuit);
    if (!serialized && currentTime_s > 8) {
      serialized = true;
      TestCircuitSerialization(name);
      fluidCircuit = m_Circuits->GetFluidCircuit("Fluid");
    }
    currentTime_s += timeStep_s;
  }
}

void FluidCircuitTester::RunZEROCENTEREDSINTest(std::string name, unsigned int i)
{
  biogears::Logger* test_Logger = new biogears::Logger("TestResults/" + name + ".log");
  timeStep_s = (1.0 / 165.0);
  currentTime_s = 0.0;
  fluidCalculator = new biogears::SEFluidCircuitCalculator(test_Logger);
  fluidCircuit = m_Circuits->GetFluidCircuit("Fluid");
  fluidCircuit->SetNextAndCurrentFromBaselines();
  fluidCircuit->StateChange();
  while (currentTime_s < 10) {
    TestPreProcess4(currentTime_s, i);
    fluidCalculator->Process(*fluidCircuit, timeStep_s);
    fluidCalculator->PostProcess(*fluidCircuit);
    trk->Track(currentTime_s, *fluidCircuit);
    if (!serialized && currentTime_s > 8) {
      serialized = true;
      TestCircuitSerialization(name);
      fluidCircuit = m_Circuits->GetFluidCircuit("Fluid");
    }
    currentTime_s += timeStep_s;
  }
}

void FluidCircuitTester::TestCircuitSerialization(const std::string& fileName)
{
  biogears::ScopedFileSystemLock lock;

  std::ofstream stream(fileName);
  xml_schema::namespace_infomap map;
  map[""].name = "uri:/mil/tatrc/physiology/datamodel";

  CDM::CircuitManager(stream, dynamic_cast<CDM::CircuitManagerData&>(*(m_Circuits->Unload())), map);
  stream.close();
  std::unique_ptr<CDM::ObjectData> bind = biogears::Serializer::ReadFile(fileName, m_Logger);
  CDM::CircuitManagerData* data = dynamic_cast<CDM::CircuitManagerData*>(bind.get());
  if (data != nullptr) {
    m_Circuits->Load(*data);
  }
}

void FluidCircuitTester::TestPreProcess1(double dT, int i)
{
  bool FLOWSOURCE = false;
  bool SWITCHPRESENT = false;
  bool PRESSURESOURCE = false;
  bool MULTISOURCEPRESSURE = false;
  bool MULTISOURCEFLOW = false;
  if (i == 22 || i == 24 || i == 35) {
    MULTISOURCEPRESSURE = true;
  }
  if (i == 23 || i == 25 || i == 35) {
    MULTISOURCEFLOW = true;
  }
  if (i < 11 || i == 13 || i == 27 || i == 29 || i == 31 || i == 33 || i == 34) //check for pressure sources... re order the test order when all are in so these aren't pieces of junk
  {
    PRESSURESOURCE = true;
  }
  if (i == 9 || i == 11 || i == 12 || (i >= 14 && i <= 21) || i == 26 || i == 28 || i == 30 || i == 32 || i == 34) // check for flow sources
  {
    FLOWSOURCE = true;
  }
  if (i == 8 || i == 9 || i == 15 || i == 19 || i == 34) // check for switches
  {
    SWITCHPRESENT = true;
  }
  if (MULTISOURCEPRESSURE) {
    double dPressure1 = 20;
    double dPressure2 = 10;
    m_Circuits->GetFluidPath("Path1")->GetNextPressureSource().SetValue(dPressure1, biogears::PressureUnit::Pa);
    m_Circuits->GetFluidPath("Path3")->GetNextPressureSource().SetValue(dPressure2, biogears::PressureUnit::Pa);
  }
  if (MULTISOURCEFLOW) {
    double dFlow1 = 3;
    double dFlow2 = 2;
    m_Circuits->GetFluidPath("Path7")->GetNextFlowSource().SetValue(dFlow1, biogears::VolumePerTimeUnit::m3_Per_s);
    m_Circuits->GetFluidPath("Path8")->GetNextFlowSource().SetValue(dFlow2, biogears::VolumePerTimeUnit::m3_Per_s);
  }
  if (PRESSURESOURCE) {
    double dPressure = 20;
    m_Circuits->GetFluidPath("Path1")->GetNextPressureSource().SetValue(dPressure, biogears::PressureUnit::Pa);
  }
  if (FLOWSOURCE) {
    double dFlow = 3;
    m_Circuits->GetFluidPath("Path7")->GetNextFlowSource().SetValue(dFlow, biogears::VolumePerTimeUnit::m3_Per_s);
  }
  //Switch Test
  if (SWITCHPRESENT) {
    if (i == 8 || i == 34 || i == 19) {
      if (dT < 5) {
        m_Circuits->GetFluidPath("Path2")->SetNextSwitch(CDM::enumOpenClosed::Open);
      } else {
        m_Circuits->GetFluidPath("Path2")->SetNextSwitch(CDM::enumOpenClosed::Closed);
      }
    } else if (i == 9 || i == 15) {
      if (dT < 5) {
        m_Circuits->GetFluidPath("Path3")->SetNextSwitch(CDM::enumOpenClosed::Open);
      } else {
        m_Circuits->GetFluidPath("Path3")->SetNextSwitch(CDM::enumOpenClosed::Closed);
      }
    }
  }
}

void FluidCircuitTester::TestPreProcess2(double dT, int i)
{
  double ONE = 1;
  bool FLOWSOURCE = false;
  bool SWITCHPRESENT = false;
  bool PRESSURESOURCE = false;
  bool MULTISOURCEPRESSURE = false;
  bool MULTISOURCEFLOW = false;
  if (i == 22 || i == 24 || i == 35) {
    MULTISOURCEPRESSURE = true;
  }
  if (i == 23 || i == 25 || i == 35) {
    MULTISOURCEFLOW = true;
  }
  if (i < 11 || i == 13 || i == 27 || i == 29 || i == 31 || i == 33 || i == 34) //check for pressure sources... re order the test order when all are in so these aren't pieces of junk
  {
    PRESSURESOURCE = true;
  }
  if (i == 9 || i == 11 || i == 12 || (i >= 14 && i <= 21) || i == 26 || i == 28 || i == 30 || i == 32 || i == 34) // check for flow sources
  {
    FLOWSOURCE = true;
  }
  if (i == 8 || i == 9 || i == 15 || i == 19 || i == 34) // check for switches
  {
    SWITCHPRESENT = true;
  }
  if (MULTISOURCEPRESSURE) {
    double dPressure1 = 20 + 20 * sin(dT);
    double dPressure2 = 10 + 10 * sin(dT);
    m_Circuits->GetFluidPath("Path1")->GetNextPressureSource().SetValue(dPressure1, biogears::PressureUnit::Pa);
    m_Circuits->GetFluidPath("Path3")->GetNextPressureSource().SetValue(dPressure2, biogears::PressureUnit::Pa);
  }
  if (MULTISOURCEFLOW) {
    double dFlow1 = 3 * sin(dT);
    double dFlow2 = 2 * sin(dT);
    m_Circuits->GetFluidPath("Path7")->GetNextFlowSource().SetValue(dFlow1, biogears::VolumePerTimeUnit::m3_Per_s);
    m_Circuits->GetFluidPath("Path8")->GetNextFlowSource().SetValue(dFlow2, biogears::VolumePerTimeUnit::m3_Per_s);
  }
  //Sinusoidal source
  if (PRESSURESOURCE)
  //Modify the pressure source
  {
    double dPressure = 20 + 20 * sin(dT);
    m_Circuits->GetFluidPath("Path1")->GetNextPressureSource().SetValue(dPressure, biogears::PressureUnit::Pa); // need to adjust this so path 4 isn't always the source path
  }
  if (FLOWSOURCE)
  //Modify the flow source
  {
    double dFlow = 3 * sin(dT); //can go negative to test negative flows
    m_Circuits->GetFluidPath("Path7")->GetNextFlowSource().SetValue(dFlow, biogears::VolumePerTimeUnit::m3_Per_s); // need to adjust this so path 4 isn't always the source path
  }
  //Switch Test
  if (SWITCHPRESENT) {
    if (i == 8 || i == 34 || i == 19) {
      if (dT < 5) {
        m_Circuits->GetFluidPath("Path2")->SetNextSwitch(CDM::enumOpenClosed::Open);
      } else {
        m_Circuits->GetFluidPath("Path2")->SetNextSwitch(CDM::enumOpenClosed::Closed);
      }
    } else if (i == 9 || i == 15) {
      if (dT < 5) {
        m_Circuits->GetFluidPath("Path3")->SetNextSwitch(CDM::enumOpenClosed::Open);
      } else {
        m_Circuits->GetFluidPath("Path3")->SetNextSwitch(CDM::enumOpenClosed::Closed);
      }
    }
  }
}

void FluidCircuitTester::TestPreProcess3(double dT, int i)
{
  bool FLOWSOURCE = false;
  bool SWITCHPRESENT = false;
  bool PRESSURESOURCE = false;
  bool MULTISOURCEPRESSURE = false;
  bool MULTISOURCEFLOW = false;
  if (i == 22 || i == 24 || i == 35) {
    MULTISOURCEPRESSURE = true;
  }
  if (i == 23 || i == 25 || i == 35) {
    MULTISOURCEFLOW = true;
  }
  if (i < 11 || i == 13 || i == 27 || i == 29 || i == 31 || i == 33 || i == 34) //check for pressure sources... re order the test order when all are in so these aren't pieces of junk
  {
    PRESSURESOURCE = true;
  }
  if (i == 9 || i == 11 || i == 12 || (i >= 14 && i <= 21) || i == 26 || i == 28 || i == 30 || i == 32 || i == 34) // check for flow sources
  {
    FLOWSOURCE = true;
  }
  if (i == 8 || i == 9 || i == 15 || i == 19 || i == 34) // check for switches
  {
    SWITCHPRESENT = true;
  }

  int pulseWidth = 2;
  int intDT = int(floor(dT)); //rounding dT
  bool pulseStatus = true;
  //Pulse train
  //May need to rework this logic, but it should be passable for now.
  if (true) {
    if ((intDT % pulseWidth) >= (pulseWidth / 2)) // seems to work more or less alright
    {
      pulseStatus = false;
    }

    if (pulseStatus) {
      if (PRESSURESOURCE) {
        double dPressure = 20;
        m_Circuits->GetFluidPath("Path1")->GetNextPressureSource().SetValue(dPressure, biogears::PressureUnit::Pa);
      }

      if (FLOWSOURCE) {
        double dFlow = 3;
        m_Circuits->GetFluidPath("Path7")->GetNextFlowSource().SetValue(dFlow, biogears::VolumePerTimeUnit::m3_Per_s);
      }
      if (MULTISOURCEPRESSURE) {
        double dPressure1 = 20;
        double dPressure2 = 10;
        m_Circuits->GetFluidPath("Path1")->GetNextPressureSource().SetValue(dPressure1, biogears::PressureUnit::Pa);
        m_Circuits->GetFluidPath("Path3")->GetNextPressureSource().SetValue(dPressure2, biogears::PressureUnit::Pa);
      }
      if (MULTISOURCEFLOW) {
        double dFlow1 = 3;
        double dFlow2 = 2;
        m_Circuits->GetFluidPath("Path7")->GetNextFlowSource().SetValue(dFlow1, biogears::VolumePerTimeUnit::m3_Per_s);
        m_Circuits->GetFluidPath("Path8")->GetNextFlowSource().SetValue(dFlow2, biogears::VolumePerTimeUnit::m3_Per_s);
      }
    } else {
      if (PRESSURESOURCE) {
        double dPressure = 0;
        m_Circuits->GetFluidPath("Path1")->GetNextPressureSource().SetValue(dPressure, biogears::PressureUnit::Pa);
      }
      if (FLOWSOURCE) {
        double dFlow = 0;
        m_Circuits->GetFluidPath("Path7")->GetNextFlowSource().SetValue(dFlow, biogears::VolumePerTimeUnit::m3_Per_s);
      }
      if (MULTISOURCEPRESSURE) {
        double dPressure1 = 0;
        double dPressure2 = 0;
        m_Circuits->GetFluidPath("Path1")->GetNextPressureSource().SetValue(dPressure1, biogears::PressureUnit::Pa);
        m_Circuits->GetFluidPath("Path3")->GetNextPressureSource().SetValue(dPressure2, biogears::PressureUnit::Pa);
      }
      if (MULTISOURCEFLOW) {
        double dFlow1 = 0;
        double dFlow2 = 0;
        m_Circuits->GetFluidPath("Path7")->GetNextFlowSource().SetValue(dFlow1, biogears::VolumePerTimeUnit::m3_Per_s);
        m_Circuits->GetFluidPath("Path8")->GetNextFlowSource().SetValue(dFlow2, biogears::VolumePerTimeUnit::m3_Per_s);
      }
    }
  }
  //Switch Test
  if (SWITCHPRESENT) {
    if (i == 8 || i == 34 || i == 19) {
      if (dT < 5) {
        m_Circuits->GetFluidPath("Path2")->SetNextSwitch(CDM::enumOpenClosed::Open);
      } else {
        m_Circuits->GetFluidPath("Path2")->SetNextSwitch(CDM::enumOpenClosed::Closed);
      }
    } else if (i == 9 || i == 15) {
      if (dT < 5) {
        m_Circuits->GetFluidPath("Path3")->SetNextSwitch(CDM::enumOpenClosed::Open);
      } else {
        m_Circuits->GetFluidPath("Path3")->SetNextSwitch(CDM::enumOpenClosed::Closed);
      }
    }
  }
}

void FluidCircuitTester::TestPreProcess4(double dT, int i)
{

  //Note: If you don't modify a value, it will remain the same.
  //    Only assign a value for Next if you're changing it.
  //    Never change "current" values, only next.

  // ALL PRESSURE SOURCES MUST EXIST ON PATH 1 UNTIL THIS IS CHANGED TO BE MORE FLEXIBLE
  double ONE = 1;
  bool FLOWSOURCE = false;
  bool SWITCHPRESENT = false;
  bool PRESSURESOURCE = false;
  bool MULTISOURCEPRESSURE = false;
  bool MULTISOURCEFLOW = false;
  if (i == 22 || i == 24 || i == 35) {
    MULTISOURCEPRESSURE = true;
  }
  if (i == 23 || i == 25 || i == 35) {
    MULTISOURCEFLOW = true;
  }
  if (i < 11 || i == 13 || i == 27 || i == 29 || i == 31 || i == 33 || i == 34) //check for pressure sources... re order the test order when all are in so these aren't pieces of junk
  {
    PRESSURESOURCE = true;
  }
  if (i == 9 || i == 11 || i == 12 || (i >= 14 && i <= 21) || i == 26 || i == 28 || i == 30 || i == 32 || i == 34) // check for flow sources
  {
    FLOWSOURCE = true;
  }
  if (i == 8 || i == 9 || i == 15 || i == 19 || i == 34) // check for switches
  {
    SWITCHPRESENT = true;
  }
  if (MULTISOURCEPRESSURE) {
    double dPressure1 = 20 * sin(dT);
    double dPressure2 = 10 * sin(dT);
    m_Circuits->GetFluidPath("Path1")->GetNextPressureSource().SetValue(dPressure1, biogears::PressureUnit::Pa);
    m_Circuits->GetFluidPath("Path3")->GetNextPressureSource().SetValue(dPressure2, biogears::PressureUnit::Pa);
  }
  if (MULTISOURCEFLOW) {
    double dFlow1 = 3 * sin(dT);
    double dFlow2 = 2 * sin(dT);
    m_Circuits->GetFluidPath("Path7")->GetNextFlowSource().SetValue(dFlow1, biogears::VolumePerTimeUnit::m3_Per_s);
    m_Circuits->GetFluidPath("Path8")->GetNextFlowSource().SetValue(dFlow2, biogears::VolumePerTimeUnit::m3_Per_s);
  }
  //Sinusoidal source
  if (PRESSURESOURCE)
  //Modify the pressure source
  {
    double dPressure = 20 * sin(dT);
    m_Circuits->GetFluidPath("Path1")->GetNextPressureSource().SetValue(dPressure, biogears::PressureUnit::Pa); // need to adjust this so path 4 isn't always the source path
  }
  if (FLOWSOURCE)
  //Modify the flow source
  {
    double dFlow = 3 * sin(dT); //can go negative to test negative flows
    m_Circuits->GetFluidPath("Path7")->GetNextFlowSource().SetValue(dFlow, biogears::VolumePerTimeUnit::m3_Per_s); // need to adjust this so path 4 isn't always the source path
  }

  //Switch Test
  if (SWITCHPRESENT) {
    if (i == 8 || i == 34) {
      if (dT < 5) {
        m_Circuits->GetFluidPath("Path2")->SetNextSwitch(CDM::enumOpenClosed::Open);
      } else {
        m_Circuits->GetFluidPath("Path2")->SetNextSwitch(CDM::enumOpenClosed::Closed);
      }
    } else if (i == 9 || i == 15) {
      if (dT < 5) {
        m_Circuits->GetFluidPath("Path3")->SetNextSwitch(CDM::enumOpenClosed::Open);
      } else {
        m_Circuits->GetFluidPath("Path3")->SetNextSwitch(CDM::enumOpenClosed::Closed);
      }
    }
  }
}

void FluidCircuitTester::SeriesRDCTest() //0
{
  biogears::SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  biogears::SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  biogears::SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  biogears::SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  fluidCircuit.AddReferenceNode(Node3);
  Node3.GetPressure().SetValue(0, biogears::PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, biogears::PressureUnit::Pa);
  biogears::SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node3, Node1, "Path1");
  Path1.GetNextPressureSource().SetValue(20, biogears::PressureUnit::Pa);
  biogears::SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1, Node2, "Path2");
  Path2.GetNextResistance().SetValue(1000, biogears::FlowResistanceUnit::Pa_s_Per_m3);
  biogears::SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2, Node3, "Path3");
  Path3.GetNextCompliance().SetValue(0.001, biogears::FlowComplianceUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, biogears::VolumeUnit::m3);
  RunDCTest("SeriesRDCTest",0);
}

void FluidCircuitTester::SeriesRCSINTest() //0
{
  biogears::SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  biogears::SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  biogears::SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  biogears::SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  fluidCircuit.AddReferenceNode(Node3);
  Node3.GetPressure().SetValue(0, biogears::PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, biogears::PressureUnit::Pa);
  biogears::SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node3, Node1, "Path1");
  Path1.GetNextPressureSource().SetValue(20, biogears::PressureUnit::Pa);
  biogears::SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1, Node2, "Path2");
  Path2.GetNextResistance().SetValue(1000, biogears::FlowResistanceUnit::Pa_s_Per_m3);
  biogears::SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2, Node3, "Path3");
  Path3.GetNextCompliance().SetValue(0.001, biogears::FlowComplianceUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, biogears::VolumeUnit::m3);
  RunSINTest("SeriesRCSINTest", 0);
}

void FluidCircuitTester::SeriesRCPULSETest() //0
{
  biogears::SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  biogears::SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  biogears::SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  biogears::SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  fluidCircuit.AddReferenceNode(Node3);
  Node3.GetPressure().SetValue(0, biogears::PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, biogears::PressureUnit::Pa);
  biogears::SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node3, Node1, "Path1");
  Path1.GetNextPressureSource().SetValue(20, biogears::PressureUnit::Pa);
  biogears::SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1, Node2, "Path2");
  Path2.GetNextResistance().SetValue(1000, biogears::FlowResistanceUnit::Pa_s_Per_m3);
  biogears::SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2, Node3, "Path3");
  Path3.GetNextCompliance().SetValue(0.001, biogears::FlowComplianceUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, biogears::VolumeUnit::m3);
  RunPULSETest("SeriesRCPULSETest",0);
}

void FluidCircuitTester::SeriesRDCTest2() //1
{
  biogears::SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  biogears::SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  biogears::SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  biogears::SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  biogears::SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");
  fluidCircuit.AddReferenceNode(Node4);
  Node4.GetPressure().SetValue(0, biogears::PressureUnit::Pa);
  biogears::SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node4, Node1, "Path1");
  Path1.GetNextPressureSource().SetValue(20, biogears::PressureUnit::Pa);
  biogears::SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1, Node2, "Path2");
  Path2.GetNextResistance().SetValue(10, biogears::FlowResistanceUnit::Pa_s_Per_m3);
  biogears::SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2, Node3, "Path3");
  Path3.GetNextResistance().SetValue(20, biogears::FlowResistanceUnit::Pa_s_Per_m3);
  biogears::SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3, Node4, "Path4");
  Path4.GetNextResistance().SetValue(30, biogears::FlowResistanceUnit::Pa_s_Per_m3);
  RunDCTest("SeriesRDCTest2",1);
}

void FluidCircuitTester::SeriesRSINTest() //1
{
  biogears::SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  biogears::SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  biogears::SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  biogears::SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  biogears::SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");
  fluidCircuit.AddReferenceNode(Node4);
  Node4.GetPressure().SetValue(0, biogears::PressureUnit::Pa);
  biogears::SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node4, Node1, "Path1");
  Path1.GetNextPressureSource().SetValue(20, biogears::PressureUnit::Pa);

  biogears::SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1, Node2, "Path2");
  Path2.GetNextResistance().SetValue(10, biogears::FlowResistanceUnit::Pa_s_Per_m3);
  biogears::SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2, Node3, "Path3");
  Path3.GetNextResistance().SetValue(20, biogears::FlowResistanceUnit::Pa_s_Per_m3);
  biogears::SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3, Node4, "Path4");
  Path4.GetNextResistance().SetValue(30, biogears::FlowResistanceUnit::Pa_s_Per_m3);
  RunSINTest("SeriesRSINTest",1);
}

void FluidCircuitTester::SeriesRPULSETest() //1
{
  biogears::SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  biogears::SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  biogears::SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  biogears::SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  biogears::SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");
  fluidCircuit.AddReferenceNode(Node4);
  Node4.GetPressure().SetValue(0, biogears::PressureUnit::Pa);
  biogears::SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node4, Node1, "Path1");
  Path1.GetNextPressureSource().SetValue(20, biogears::PressureUnit::Pa);
  biogears::SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1, Node2, "Path2");
  Path2.GetNextResistance().SetValue(10, biogears::FlowResistanceUnit::Pa_s_Per_m3);
  biogears::SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2, Node3, "Path3");
  Path3.GetNextResistance().SetValue(20, biogears::FlowResistanceUnit::Pa_s_Per_m3);
  biogears::SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3, Node4, "Path4");
  Path4.GetNextResistance().SetValue(30, biogears::FlowResistanceUnit::Pa_s_Per_m3);
  RunPULSETest("SeriesRPULSETest",1);
}

void FluidCircuitTester::SeriesRLDCTest() //2
{
}
void FluidCircuitTester::SeriesRLSINTest() //2
{
}
void FluidCircuitTester::SeriesRLPULSETest() //2
{
}
void FluidCircuitTester::ParallelRDCTest() //3
{
}
void FluidCircuitTester::ParallelRSINTest() //3
{
}
void FluidCircuitTester::ParalleRPULSETest() //3
{
}
void FluidCircuitTester::ParallelRCDCTest() //4
{
}
void FluidCircuitTester::ParallelRCSINTest() //4
{
}
void FluidCircuitTester::ParallelRCPULSETest() //4
{
}
void FluidCircuitTester::ParallelRLDCTest() //5
{
}
void FluidCircuitTester::ParallelRLSINTest() //5
{
}
void FluidCircuitTester::ParallelRLPULSETest() //5
{
}
void FluidCircuitTester::ParallelRLSINCenteredTest() //5
{
}
void FluidCircuitTester::SeriesRLCDCTest() //6
{
}
void FluidCircuitTester::ParallelRLCSINTest() //6
{
}
void FluidCircuitTester::ParallelRLCPULSETest() //6
{
}
void FluidCircuitTester::ParallelRLCDCTest() //7
{
  
}

  void FluidCircuitTester::BasicDiodeDCCurrent() //18
{
  biogears::SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  biogears::SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  biogears::SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  biogears::SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  fluidCircuit.AddReferenceNode(Node3);
  Node3.GetPressure().SetValue(0, biogears::PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, biogears::PressureUnit::Pa);
  biogears::SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node3, Node1, "Path7");
  Path7.GetNextFlowSource().SetValue(3, biogears::VolumePerTimeUnit::m3_Per_s);
  biogears::SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1, Node2, "Path2");
  Path2.GetNextResistance().SetValue(10, biogears::FlowResistanceUnit::Pa_s_Per_m3);
  biogears::SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2, Node3, "Path3");
  Path3.GetNextCompliance().SetValue(0.001, biogears::FlowComplianceUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, biogears::VolumeUnit::m3);
  biogears::SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3, Node2, "Path4");
  Path4.SetNextValve(CDM::enumOpenClosed::Closed);
  RunDCTest("BasicDiodeDCCurrent", 18);
}