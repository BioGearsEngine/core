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
#define _USE_MATH_DEFINES
#include <cmath>

#include <biogears/cdm/circuit/fluid/SEFluidCircuit.h>
#include <biogears/cdm/compartment/fluid/SEFluidCompartmentLink.h>
#include <biogears/cdm/compartment/fluid/SEGasCompartmentGraph.h>
#include <biogears/cdm/properties/SEScalarFlowCompliance.h>
#include <biogears/cdm/properties/SEScalarFlowResistance.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/engine/test/BioGearsEngineTest.h>

namespace biogears {
//--------------------------------------------------------------------------------------------------
/// \brief
/// Runs the test for the circuit math in the anesthesia machine
///
/// \param  sTestDirectory  Contains the path to the directory where the output file is stored
///
/// \details
/// Tests the anesthesia machine circuit mechanics by reading in a text file that contains the driver
/// and variable values for the circuit elements.The outputs are the resultant flows and pressures
/// on the circuit nodes and paths. These are then stored in a file in sTestDirectory
//--------------------------------------------------------------------------------------------------
void BioGearsEngineTest::AnesthesiaMachineCircuitAndTransportTest(RespiratoryConfiguration config, const std::string& sTestDirectory)
{
  TimingProfile tmr;
  tmr.Start("Test");
  //Output files
  DataTrack outTrkCircuit;
  DataTrack outTrkGraph;
  std::ofstream fileCircuit;
  std::ofstream fileGraph;

  BioGears bg(sTestDirectory + "/AnesthesiaMachineCircuitAndTransportTest.log");
  bg.GetPatient().Load("./patients/StandardMale.xml");
  bg.SetupPatient();
  bg.m_Config->EnableRenal(CDM::enumOnOff::Off);
  bg.m_Config->EnableTissue(CDM::enumOnOff::Off);
  bg.CreateCircuitsAndCompartments();
  SEEnvironmentalConditions env(bg.GetSubstances());
  env.Load("./environments/Standard.xml");
  SEGasCompartment* cEnv = bg.GetCompartments().GetGasCompartment(BGE::EnvironmentCompartment::Ambient);
  for (SESubstanceFraction* subFrac : env.GetAmbientGases()) {
    bg.GetSubstances().AddActiveSubstance(subFrac->GetSubstance());
    cEnv->GetSubstanceQuantity(subFrac->GetSubstance())->GetVolumeFraction().Set(subFrac->GetFractionAmount());
  }
  bg.GetSubstances().InitializeGasCompartments();

  //set environment pressure
  SEFluidCircuitNode* nEnvironment = bg.GetCircuits().GetFluidNode(BGE::EnvironmentNode::Ambient);
  nEnvironment->GetPressure().Set(env.GetAtmosphericPressure());
  nEnvironment->GetNextPressure().Set(env.GetAtmosphericPressure());

  //Grab the anesthesia circuits and graphs
  SEFluidCircuit* amCircuit = nullptr;
  SEGasCompartmentGraph* amGraph = nullptr;
  std::string sCircuitFileName;
  std::string sTransportFileName;
  if (config == AnesthesiaMachineSolo) {
    amCircuit = &bg.GetCircuits().GetAnesthesiaMachineCircuit();
    amGraph = &bg.GetCompartments().GetAnesthesiaMachineGraph();
    sCircuitFileName = "/AnesthesiaMachineCircuitOutput.csv";
    sTransportFileName = "/AnesthesiaMachineTransportOutput.csv";

    //Allow things to flow to ground, since the respiratory circuit isn't here
    //This approximates the total respiratory system resistance
    SEFluidCircuitPath* AnesthesiaConnectionToEnvironment = amCircuit->GetPath(BGE::AnesthesiaMachinePath::AnesthesiaConnectionToEnvironment);
    AnesthesiaConnectionToEnvironment->GetResistanceBaseline().SetValue(1.5, FlowResistanceUnit::cmH2O_s_Per_L);
    AnesthesiaConnectionToEnvironment->GetNextResistance().SetValue(1.5, FlowResistanceUnit::cmH2O_s_Per_L);
  } else if (config == RespiratoryWithAnesthesiaMachine) {
    bg.GetSubstances().InitializeGasCompartments();

    amCircuit = &bg.GetCircuits().GetRespiratoryAndAnesthesiaMachineCircuit();
    amGraph = &bg.GetCompartments().GetRespiratoryAndAnesthesiaMachineGraph();
    sCircuitFileName = "/RespiratoryAndAnesthesiaMachineCircuitOutput.csv";
    sTransportFileName = "/RespiratoryAndAnesthesiaMachineTransportOutput.csv";

    //Precharge the stomach to prevent negative volume
    amCircuit->GetNode(BGE::RespiratoryNode::Stomach)->GetNextPressure().Set(env.GetAtmosphericPressure());
    amCircuit->GetNode(BGE::RespiratoryNode::Stomach)->GetPressure().Set(env.GetAtmosphericPressure());

    SEFluidCircuitPath* driverPath = amCircuit->GetPath(BGE::RespiratoryPath::EnvironmentToRespiratoryMuscle);
    driverPath->GetPressureSourceBaseline().SetValue(-12.0, PressureUnit::cmH2O);
    driverPath->GetNextPressureSource().SetValue(-12.0, PressureUnit::cmH2O);
  } else {
    return;
  }

  // Pull Pressure Source Paths
  SEFluidCircuitPath* EnvironmentToVentilatorPath = amCircuit->GetPath(BGE::AnesthesiaMachinePath::EnvironmentToVentilator);
  SEFluidCircuitPath* EnvironmentToReliefValve = amCircuit->GetPath(BGE::AnesthesiaMachinePath::EnvironmentToReliefValve);
  SEFluidCircuitPath* GasSourceToGasInlet = amCircuit->GetPath(BGE::AnesthesiaMachinePath::GasSourceToGasInlet);
  SEFluidCircuitPath* SelectorToEnvironment = amCircuit->GetPath(BGE::AnesthesiaMachinePath::SelectorToEnvironment);
  SEFluidCircuitPath* EnvironmentToGasSource = amCircuit->GetPath(BGE::AnesthesiaMachinePath::EnvironmentToGasSource);

  SEGasTransporter txpt(VolumePerTimeUnit::L_Per_s, VolumeUnit::L, VolumeUnit::L, NoUnit::unitless, bg.GetLogger());
  SEFluidCircuitCalculator calc(FlowComplianceUnit::L_Per_cmH2O, VolumePerTimeUnit::L_Per_s, FlowInertanceUnit::cmH2O_s2_Per_L, PressureUnit::cmH2O, VolumeUnit::L, FlowResistanceUnit::cmH2O_s_Per_L, bg.GetLogger());

  //Execution parameters
  double time = 0;
  double deltaT_s = 1.0 / 90.0;
  double runTime_min = 10.0;
  //Drive waveform parameters
  double period = 5.0;
  double alpha = (2 * M_PI) / (period);
  double driverPressure_cmH2O = 0.0;
  double amplitude_cmH2O = 6.0;
  double yOffset = 6.0;

  for (unsigned int i = 0; i < runTime_min * 60.0 / deltaT_s; i++) {
    //PreProcess
    //Set some constant elements - they're inside the loop to overwrite any baselines
    EnvironmentToReliefValve->GetNextPressureSource().SetValue(100.0, PressureUnit::cmH2O);
    GasSourceToGasInlet->GetNextFlowSource().SetValue(5.0, VolumePerTimeUnit::L_Per_min);
    SelectorToEnvironment->GetNextFlowSource().SetValue(5.0, VolumePerTimeUnit::L_Per_min);

    //Set the driver pressure
    driverPressure_cmH2O = yOffset + amplitude_cmH2O * sin(alpha * time); //compute new pressure
    EnvironmentToVentilatorPath->GetNextPressureSource().SetValue(driverPressure_cmH2O, PressureUnit::cmH2O);

    //Process - Execute the circuit
    calc.Process(*amCircuit, deltaT_s);
    //Execute the substance transport function
    txpt.Transport(*amGraph, deltaT_s);
    //convert 'Next' values to current
    calc.PostProcess(*amCircuit);

    outTrkCircuit.Track(time, *amCircuit);
    outTrkGraph.Track(time, *amGraph);
    time += deltaT_s;

    if (i == 0) {
      outTrkCircuit.CreateFile(std::string(sTestDirectory + sCircuitFileName).c_str(), fileCircuit);
      outTrkGraph.CreateFile(std::string(sTestDirectory + sTransportFileName).c_str(), fileGraph);
    }
    outTrkCircuit.StreamTrackToFile(fileCircuit);
    outTrkGraph.StreamTrackToFile(fileGraph);
  }
  fileCircuit.close();
  fileGraph.close();
  std::stringstream ss;
  ss << "It took " << tmr.GetElapsedTime_s("Test") << "s to run";
  bg.GetLogger()->Info(ss.str(), "AnesthesiaMachineCircuitAndTransportTest");
}

void BioGearsEngineTest::AnesthesiaMachineCircuitAndTransportTest(const std::string& sTestDirectory)
{
  AnesthesiaMachineCircuitAndTransportTest(AnesthesiaMachineSolo, sTestDirectory);
}

void BioGearsEngineTest::RespiratoryWithAnesthesiaMachineCircuitAndTransportTest(const std::string& sTestDirectory)
{
  AnesthesiaMachineCircuitAndTransportTest(RespiratoryWithAnesthesiaMachine, sTestDirectory);
}
}