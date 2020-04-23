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

#include <biogears/cdm/circuit/fluid/SEFluidCircuit.h>
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/compartment/fluid/SEGasCompartmentGraph.h>
#include <biogears/cdm/compartment/substances/SEGasSubstanceQuantity.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/properties/SEScalarFlowCompliance.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarInverseVolume.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/engine/test/BioGearsEngineTest.h>
#include <math.h>

namespace biogears {
void BioGearsEngineTest::RespiratoryCircuitAndTransportTest(RespiratoryConfiguration config, const std::string& sTestDirectory)
{
  TimingProfile tmr;
  tmr.Start("Test");
  //Output files
  DataTrack outTrkCircuit;
  DataTrack outTrkGraph;
  DataTrack aerosolGraphTrk;
  std::ofstream fileCircuit;
  std::ofstream fileGraph;
  std::ofstream fAerosolGraph;

  BioGears bg(sTestDirectory + "/RespiratoryCircuitAndTransportTest.log");
  bg.GetPatient().Load("./patients/StandardMale.xml");
  bg.SetupPatient();
  bg.m_Config->EnableRenal(CDM::enumOnOff::Off);
  bg.m_Config->EnableTissue(CDM::enumOnOff::Off);
  bg.CreateCircuitsAndCompartments();
  bg.GetSubstances().InitializeGasCompartments();
  SEEnvironmentalConditions& env = bg.GetEnvironment().GetConditions();

  SEFluidCircuit* rCircuit = nullptr;
  SEGasCompartmentGraph* rGraph = nullptr;
  SELiquidCompartmentGraph* aGraph = nullptr;
  std::string sCircuitFileName;
  std::string sTransportFileName;
  std::string sAerosolTxptFileName;
  if (config == RespiratorySolo) {
    rCircuit = &bg.GetCircuits().GetRespiratoryCircuit();
    rGraph = &bg.GetCompartments().GetRespiratoryGraph();
    aGraph = nullptr;
    sCircuitFileName = "/RespiratoryCircuitOutput.csv";
    sTransportFileName = "/RespiratoryTransportOutput.csv";
    sAerosolTxptFileName = "";
  } else if (config == RespiratoryWithInhaler) {
    rCircuit = &bg.GetCircuits().GetRespiratoryAndInhalerCircuit();
    rGraph = &bg.GetCompartments().GetRespiratoryAndInhalerGraph();
    aGraph = &bg.GetCompartments().GetAerosolAndInhalerGraph();
    sCircuitFileName = "/RespiratoryAndInhalerCircuitOutput.csv";
    sTransportFileName = "/RespiratoryAndInhalerTransportOutput.csv";
    sAerosolTxptFileName = "/AerosolInhalerTransportOutput.csv";

    // Get an aerosolized substance
    SESubstance* albuterol = bg.GetSubstances().GetSubstance("Albuterol");
    if (albuterol == nullptr) {
      bg.GetLogger()->Error("Could not find the aerosol substance : Albuterol");
    } else {
      bg.GetSubstances().AddActiveSubstance(*albuterol);
      SELiquidCompartment* mouthpiece = bg.GetCompartments().GetLiquidCompartment(BGE::InhalerCompartment::Mouthpiece);
      mouthpiece->GetSubstanceQuantity(*albuterol)->GetMass().SetValue(90, MassUnit::ug);
      mouthpiece->Balance(BalanceLiquidBy::Mass);
    }
  } else if (config == RespiratoryWithMechanicalVentilator) {
    rCircuit = &bg.GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit();
    rGraph = &bg.GetCompartments().GetRespiratoryAndMechanicalVentilatorGraph();
    aGraph = nullptr;
    sCircuitFileName = "/RespiratoryAndMechanicalVentilatorCircuitOutput.csv";
    sTransportFileName = "/RespiratoryAndMechanicalVentilatorTransportOutput.csv";
    sAerosolTxptFileName = "";
  } else {
    return;
  }

  SEFluidCircuitPath* driverPath = rCircuit->GetPath(BGE::RespiratoryPath::EnvironmentToRespiratoryMuscle);
  SEGasTransporter gtxpt(VolumePerTimeUnit::L_Per_s, VolumeUnit::L, VolumeUnit::L, NoUnit::unitless, bg.GetLogger());
  SELiquidTransporter ltxpt(VolumePerTimeUnit::mL_Per_s, VolumeUnit::mL, MassUnit::ug, MassPerVolumeUnit::ug_Per_mL, bg.GetLogger());
  SEFluidCircuitCalculator calc(FlowComplianceUnit::L_Per_cmH2O, VolumePerTimeUnit::L_Per_s, FlowInertanceUnit::cmH2O_s2_Per_L, PressureUnit::cmH2O, VolumeUnit::L, FlowResistanceUnit::cmH2O_s_Per_L, bg.GetLogger());

  //Set the reference not pressure to the standard environment
  //This is needed because we're not setting the Environment during initialization in this unit test
  rCircuit->GetNode(BGE::EnvironmentNode::Ambient)->GetNextPressure().Set(env.GetAtmosphericPressure());
  rCircuit->GetNode(BGE::EnvironmentNode::Ambient)->GetPressure().Set(env.GetAtmosphericPressure());
  //Precharge the stomach to prevent negative volume
  rCircuit->GetNode(BGE::RespiratoryNode::Stomach)->GetNextPressure().Set(env.GetAtmosphericPressure());
  rCircuit->GetNode(BGE::RespiratoryNode::Stomach)->GetPressure().Set(env.GetAtmosphericPressure());

  //Circuit Analysis Test --------------------------------------------------
  //Execution parameters
  double time = 0;
  double deltaT_s = 1.0 / 90.0;
  double runTime_min = 10.0;
  //Drive waveform parameters
  double period = 5.0;
  double alpha = (2 * M_PI) / (period);
  double driverPressure_cmH2O = 0.0;
  double amplitude_cmH2O = 6.0;
  double yOffset = -12.0;

  for (unsigned int i = 0; i < runTime_min * 60.0 / deltaT_s; i++) {
    //PreProcess - Push driver pressure and  variable compliance data into the Circuit
    driverPressure_cmH2O = yOffset + amplitude_cmH2O * sin(alpha * time); //compute new pressure
    driverPath->GetNextPressureSource().SetValue(driverPressure_cmH2O, PressureUnit::cmH2O);

    //Process - Execute the circuit
    calc.Process(*rCircuit, deltaT_s);
    //Execute the substance transport function
    gtxpt.Transport(*rGraph, deltaT_s);
    // Do it again for aerosols
    if (aGraph != nullptr)
      ltxpt.Transport(*aGraph, deltaT_s);
    //convert 'Next' values to current
    calc.PostProcess(*rCircuit);

    outTrkCircuit.Track(time, *rCircuit);
    outTrkGraph.Track(time, *rGraph);
    if (aGraph != nullptr)
      aerosolGraphTrk.Track(time, *aGraph);
    time += deltaT_s;

    if (i == 0) {
      outTrkCircuit.CreateFile(std::string(sTestDirectory + sCircuitFileName).c_str(), fileCircuit);
      outTrkGraph.CreateFile(std::string(sTestDirectory + sTransportFileName).c_str(), fileGraph);
      if (aGraph != nullptr)
        aerosolGraphTrk.CreateFile(std::string(sTestDirectory + sAerosolTxptFileName).c_str(), fAerosolGraph);
    }
    outTrkCircuit.StreamTrackToFile(fileCircuit);
    outTrkGraph.StreamTrackToFile(fileGraph);
    if (aGraph != nullptr)
      aerosolGraphTrk.StreamTrackToFile(fAerosolGraph);
  }
  fileCircuit.close();
  fileGraph.close();
  fAerosolGraph.close();
  std::stringstream ss;
  ss << "It took " << tmr.GetElapsedTime_s("Test") << "s to run";
  bg.GetLogger()->Info(ss.str(), "RespiratoryCircuitAndTransportTest");
}

void BioGearsEngineTest::RespiratoryCircuitAndTransportTest(const std::string& sTestDirectory)
{
  RespiratoryCircuitAndTransportTest(RespiratorySolo, sTestDirectory);
}

void BioGearsEngineTest::RespiratoryWithInhalerCircuitAndTransportTest(const std::string& sTestDirectory)
{
  RespiratoryCircuitAndTransportTest(RespiratoryWithInhaler, sTestDirectory);
}

void BioGearsEngineTest::RespiratoryWithMechanicalVentilatorCircuitAndTransportTest(const std::string& sTestDirectory)
{
  RespiratoryCircuitAndTransportTest(RespiratoryWithMechanicalVentilator, sTestDirectory);
}

void BioGearsEngineTest::RespiratoryDriverTest(const std::string& sTestDirectory)
{
  TimingProfile tmr;
  tmr.Start("Test");
  BioGears bg(sTestDirectory + "/RespiratoryDriverTest.log");
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

  DataTrack trk1;
  SEFluidCircuit& RespCircuit = bg.GetCircuits().GetRespiratoryCircuit();
  SEFluidCircuitCalculator calc(FlowComplianceUnit::L_Per_cmH2O, VolumePerTimeUnit::L_Per_s, FlowInertanceUnit::cmH2O_s2_Per_L, PressureUnit::cmH2O, VolumeUnit::L, FlowResistanceUnit::cmH2O_s_Per_L, bg.GetLogger());

  double deltaT_s = 1.0 / 90.0;

  SEFluidCircuitPath* driverPressurePath = RespCircuit.GetPath(BGE::RespiratoryPath::EnvironmentToRespiratoryMuscle);
  SEFluidCircuitPath* rightPleuralToRespiratoryMuscle = RespCircuit.GetPath(BGE::RespiratoryPath::RightPleuralCavityToRespiratoryMuscle);
  SEFluidCircuitPath* leftPleuralToRespiratoryMuscle = RespCircuit.GetPath(BGE::RespiratoryPath::LeftPleuralCavityToRespiratoryMuscle);

  SEFluidCircuitNode* rightPleuralNode = RespCircuit.GetNode(BGE::RespiratoryNode::RightPleuralCavity);
  SEFluidCircuitNode* leftPleuralNode = RespCircuit.GetNode(BGE::RespiratoryNode::LeftPleuralCavity);
  SEFluidCircuitNode* rightDeadSpaceNode = RespCircuit.GetNode(BGE::RespiratoryNode::RightBronchi);
  SEFluidCircuitNode* leftDeadSpaceNode = RespCircuit.GetNode(BGE::RespiratoryNode::LeftBronchi);
  SEFluidCircuitNode* rightAlveoliNode = RespCircuit.GetNode(BGE::RespiratoryNode::RightAlveoli);
  SEFluidCircuitNode* leftAlveoliNode = RespCircuit.GetNode(BGE::RespiratoryNode::LeftAlveoli);

  RespCircuit.GetNode(BGE::EnvironmentNode::Ambient)->GetNextPressure().SetValue(760, PressureUnit::mmHg);

  driverPressurePath->GetNextPressureSource().SetValue(0.0, PressureUnit::cmH2O);
  double PressureIncrement_cmH2O = 0.1;
  double DriverPressure_cmH2O = 0.0;
  bool bSettled = false;
  bool bIRVReached = false;
  bool bRVReached = false;
  int iTime = 0;

  while (!bIRVReached) {
    double TotalVolume_L = 0.0;
    double PreviousTotalVolume_L = 0.0;
    while (!bSettled) {
      //Set the driver pressure
      //Note: the driver pressure should be negative
      driverPressurePath->GetNextPressureSource().SetValue(DriverPressure_cmH2O, PressureUnit::cmH2O);

      //Variable compliance feedback
      //TODO: Figure out how to use that actual Respiratory function.  For now we'll just copy and paste it in.
      double dRightPleuralCompliance = rightPleuralToRespiratoryMuscle->GetNextCompliance().GetValue(FlowComplianceUnit::L_Per_cmH2O);
      double dLeftPleuralCompliance = leftPleuralToRespiratoryMuscle->GetNextCompliance().GetValue(FlowComplianceUnit::L_Per_cmH2O);
      double dRightPleuralVolumeBaseline = rightPleuralNode->GetVolumeBaseline().GetValue(VolumeUnit::L);
      double dLeftPleuralVolumeBaseline = leftPleuralNode->GetVolumeBaseline().GetValue(VolumeUnit::L);
      double dRightPleuralVolume = rightPleuralNode->GetNextVolume().GetValue(VolumeUnit::L);
      double dLeftPleuralVolume = leftPleuralNode->GetNextVolume().GetValue(VolumeUnit::L);

      dRightPleuralCompliance = (dRightPleuralVolume - dRightPleuralVolumeBaseline) * 5.0 * dRightPleuralCompliance + dRightPleuralCompliance;
      dLeftPleuralCompliance = (dLeftPleuralVolume - dLeftPleuralVolumeBaseline) * 5.0 * dLeftPleuralCompliance + dLeftPleuralCompliance;

      dRightPleuralCompliance = LIMIT(dRightPleuralCompliance, 1e-6, 0.05);
      dLeftPleuralCompliance = LIMIT(dLeftPleuralCompliance, 1e-6, 0.05);

      rightPleuralToRespiratoryMuscle->GetNextCompliance().SetValue(dRightPleuralCompliance, FlowComplianceUnit::L_Per_cmH2O);
      leftPleuralToRespiratoryMuscle->GetNextCompliance().SetValue(dLeftPleuralCompliance, FlowComplianceUnit::L_Per_cmH2O);

      //Process the circuit
      calc.Process(RespCircuit, deltaT_s);
      //Advance time
      calc.PostProcess(RespCircuit);

      //Calculate the total lung volume
      TotalVolume_L = leftDeadSpaceNode->GetNextVolume(VolumeUnit::L) + leftAlveoliNode->GetNextVolume(VolumeUnit::L) + rightDeadSpaceNode->GetNextVolume(VolumeUnit::L) + rightAlveoliNode->GetNextVolume(VolumeUnit::L);

      //Check to see if the circuit has stabilized
      if (std::abs(TotalVolume_L - PreviousTotalVolume_L) < 0.0001) {
        //Output values
        trk1.Track("LungVolume_L", iTime, TotalVolume_L);
        trk1.Track("DriverPressure_cmH2O", iTime, driverPressurePath->GetPressureSource(PressureUnit::cmH2O));
        iTime++;
        bSettled = true;
      }
      PreviousTotalVolume_L = TotalVolume_L;
    }
    bSettled = false;

    //Check to see if we've gone all the way to the max volume
    if (TotalVolume_L >= bg.GetPatient().GetTotalLungCapacity(VolumeUnit::L)) {
      bIRVReached = true;
    } else {
      DriverPressure_cmH2O = DriverPressure_cmH2O - PressureIncrement_cmH2O;
    }
  }
  trk1.WriteTrackToFile(std::string(sTestDirectory + "/RespiratoryDriverOutput.csv").c_str());
  std::stringstream ss;
  ss << "It took " << tmr.GetElapsedTime_s("Test") << "s to run";
  bg.GetLogger()->Info(ss.str(), "RespiratoryDriverTest");
}
}