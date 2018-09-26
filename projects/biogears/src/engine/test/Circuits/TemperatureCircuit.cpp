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
#include <biogears/cdm/circuit/SECircuit.h>
#include <biogears/cdm/circuit/SECircuitNode.h>
#include <biogears/cdm/circuit/SECircuitPath.h>
#include <biogears/cdm/circuit/thermal/SEThermalCircuit.h>
#include <biogears/cdm/circuit/thermal/SEThermalCircuitCalculator.h>
#include <biogears/cdm/properties/SEScalarArea.h>
#include <biogears/cdm/properties/SEScalarEnergy.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarHeatCapacitancePerMass.h>
#include <biogears/cdm/properties/SEScalarHeatResistance.h>
#include <biogears/cdm/properties/SEScalarHeatResistanceArea.h>
#include <biogears/cdm/properties/SEScalarLengthPerTime.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPower.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/engine/Systems/Energy.h>
#include <biogears/engine/test/BioGearsEngineTest.h>

namespace biogears {
//This test holds EnvironmentSkin and EnvironmentCore sources constant and varies BMR
void BioGearsEngineTest::InternalTemperatureVariableBMRCircuitTest(const std::string& sTestDirectory)
{
  BioGears bg(sTestDirectory + "/InternalTemperatureVariableBMRCircuitTest.log");
  bg.GetPatient().Load("./patients/StandardMale.xml");
  bg.SetupPatient();
  bg.m_Config->EnableRenal(CDM::enumOnOff::Off);
  bg.m_Config->EnableTissue(CDM::enumOnOff::Off);
  bg.CreateCircuitsAndCompartments();

  SEThermalCircuit& m_TCircuit = bg.GetCircuits().GetInternalTemperatureCircuit();

  double testBMR_W = 100;
  SEThermalCircuitPath* GroundToCore = m_TCircuit.GetPath(BGE::InternalTemperaturePath::GroundToInternalCore);
  GroundToCore->GetHeatSourceBaseline().SetValue(testBMR_W, PowerUnit::W);
  SEThermalCircuitNode* Core = m_TCircuit.GetNode(BGE::InternalTemperatureNode::InternalCore);
  Core->GetTemperature().SetValue(36.85, TemperatureUnit::C);
  Core->GetNextTemperature().SetValue(36.85, TemperatureUnit::C);
  SEThermalCircuitNode* Skin = m_TCircuit.GetNode(BGE::InternalTemperatureNode::InternalSkin);
  Skin->GetTemperature().SetValue(33.0, TemperatureUnit::C);
  Skin->GetNextTemperature().SetValue(33.0, TemperatureUnit::C);
  SEThermalCircuitNode* Ground = m_TCircuit.GetNode(BGE::InternalTemperatureNode::InternalGround);

  //Temporary heat source set until environment exists
  SEThermalCircuitPath& CoreToEnvironment = m_TCircuit.CreatePath(*Core, *Ground, "EnvironmentCore");
  CoreToEnvironment.GetHeatSourceBaseline().SetValue(0.3 * testBMR_W, PowerUnit::W);

  SEThermalCircuitPath& SkinToEnvironment = m_TCircuit.CreatePath(*Skin, *Ground, "EnvironmentSkin");
  SkinToEnvironment.GetHeatSourceBaseline().SetValue(0.7 * testBMR_W, PowerUnit::W);

  m_TCircuit.SetNextAndCurrentFromBaselines();
  m_TCircuit.StateChange();

  DataTrack outTrk;
  std::ofstream file;
  SEThermalCircuitCalculator calc(bg.GetLogger());

  double deltaT_s = 1.0 / 90;
  double inv_deltaT_s = 1.0 / deltaT_s;
  double time_s = 0;

  //For each timestep, simulate, holding EnvironmentSkin and EnvironmentCore constant
  for (unsigned int i = 0; i < (inv_deltaT_s * 900); i++) {
    ////Drive the circuit (step function)
    if (i == (inv_deltaT_s * 60)) {
      //Increase BMR
      GroundToCore->GetHeatSourceBaseline().SetValue((5.0 * testBMR_W), PowerUnit::W);
    } else if (i == (inv_deltaT_s * 180)) {
      //Set to 0
      GroundToCore->GetHeatSourceBaseline().SetValue(0, PowerUnit::W);
    }

    //Process - Execute the circuit
    calc.Process(m_TCircuit, deltaT_s);

    //Convert "next" values to current
    calc.PostProcess(m_TCircuit);

    //Record data
    outTrk.Track(time_s, m_TCircuit);
    if (i == 0) {
      outTrk.CreateFile(std::string(sTestDirectory + "/InternalTemperatureVariableBMRCircuitOutput.csv").c_str(), file);
    }
    outTrk.StreamTrackToFile(file);
    time_s += deltaT_s;
  }
  file.close();
}

//This test holds BMR and EnvironmentCore sources constant and varies EnvironmentSkin
void BioGearsEngineTest::InternalTemperatureVariableSkinCircuitTest(const std::string& sTestDirectory)
{
  BioGears bg(sTestDirectory + "/InternalTemperatureVariableSkinCircuitTest.log");
  bg.GetPatient().Load("./patients/StandardMale.xml");
  bg.SetupPatient();
  bg.m_Config->EnableRenal(CDM::enumOnOff::Off);
  bg.m_Config->EnableTissue(CDM::enumOnOff::Off);
  bg.CreateCircuitsAndCompartments();

  SEThermalCircuit& m_TCircuit = bg.GetCircuits().GetInternalTemperatureCircuit();

  double testBMR = 1700;

  SEThermalCircuitPath* GroundToCore = m_TCircuit.GetPath(BGE::InternalTemperaturePath::GroundToInternalCore);
  GroundToCore->GetHeatSourceBaseline().SetValue(testBMR, PowerUnit::kcal_Per_day);
  SEThermalCircuitNode* Core = m_TCircuit.GetNode(BGE::InternalTemperatureNode::InternalCore);
  Core->GetTemperature().SetValue(36.85, TemperatureUnit::C);
  Core->GetNextTemperature().SetValue(36.85, TemperatureUnit::C);
  SEThermalCircuitNode* Skin = m_TCircuit.GetNode(BGE::InternalTemperatureNode::InternalSkin);
  Skin->GetTemperature().SetValue(33.0, TemperatureUnit::C);
  Skin->GetNextTemperature().SetValue(33.0, TemperatureUnit::C);
  SEThermalCircuitNode* Ground = m_TCircuit.GetNode(BGE::InternalTemperatureNode::InternalGround);

  //Temporary heat source set until environment exists
  SEThermalCircuitPath& CoreToEnvironment = m_TCircuit.CreatePath(*Core, *Ground, "EnvironmentCore");
  CoreToEnvironment.GetHeatSourceBaseline().SetValue(0.3 * testBMR, PowerUnit::kcal_Per_day);

  SEThermalCircuitPath& SkinToEnvironment = m_TCircuit.CreatePath(*Skin, *Ground, "EnvironmentSkin");
  SkinToEnvironment.GetHeatSourceBaseline().SetValue(0.7 * testBMR, PowerUnit::kcal_Per_day);

  m_TCircuit.SetNextAndCurrentFromBaselines();
  m_TCircuit.StateChange();

  DataTrack outTrk;

  std::ofstream file;

  SEThermalCircuitCalculator calc(bg.GetLogger());

  double deltaT_s = 1.0 / 90;
  double inv_deltaT_s = 1.0 / deltaT_s;
  double time_s = 0;

  //For each timestep, simulate, holding BMR and EnvironmentCore constant
  for (unsigned int i = 0; i < (inv_deltaT_s * 120); i++) {
    //Drive the circuit (step function)
    if (i == (inv_deltaT_s * 10)) {
      //Increase EnvironmentSkin (this means that the total of EnvSkin and EnvCore > BMR)
      SkinToEnvironment.GetHeatSourceBaseline().SetValue(0.9 * testBMR, PowerUnit::kcal_Per_day);
    } else if (i == (inv_deltaT_s * 20)) {
      //Increase EnvironmentSkin again
      SkinToEnvironment.GetHeatSourceBaseline().SetValue(1.1 * testBMR, PowerUnit::kcal_Per_day);
    } else if (i == (inv_deltaT_s * 30)) {
      //Decrease EnvironmentSkin to normal
      SkinToEnvironment.GetHeatSourceBaseline().SetValue(.7 * testBMR, PowerUnit::kcal_Per_day);
    } else if (i == (inv_deltaT_s * 40)) {
      //Decrease EnvironmentSkin further
      SkinToEnvironment.GetHeatSourceBaseline().SetValue(.4 * testBMR, PowerUnit::kcal_Per_day);
    } else if (i == (inv_deltaT_s * 50)) {
      //Decrease EnvironmentSkin further
      SkinToEnvironment.GetHeatSourceBaseline().SetValue(.2 * testBMR, PowerUnit::kcal_Per_day);
    } else if (i == (inv_deltaT_s * 60)) {
      //Set to 0
      SkinToEnvironment.GetHeatSourceBaseline().SetValue(0, PowerUnit::kcal_Per_day);
    }

    //Process - Execute the circuit
    calc.Process(m_TCircuit, deltaT_s);

    //Convert "next" values to current
    calc.PostProcess(m_TCircuit);

    //Record data
    outTrk.Track(time_s, m_TCircuit);
    if (i == 0) {
      outTrk.CreateFile(std::string(sTestDirectory + "/InternalTemperatureVariableSkinCircuitOutput.csv").c_str(), file);
    }
    outTrk.StreamTrackToFile(file);
    time_s += deltaT_s;
  }
  file.close();
}

//This test holds BMR and EnvironmentSkin sources constant and varies EnvironmentCore
void BioGearsEngineTest::InternalTemperatureVariableCoreCircuitTest(const std::string& sTestDirectory)
{
  BioGears bg(sTestDirectory + "/InternalTemperatureVariableCoreCircuitTest.log");
  bg.GetPatient().Load("./patients/StandardMale.xml");
  bg.SetupPatient();
  bg.m_Config->EnableRenal(CDM::enumOnOff::Off);
  bg.m_Config->EnableTissue(CDM::enumOnOff::Off);
  bg.CreateCircuitsAndCompartments();

  SEThermalCircuit& m_TCircuit = bg.GetCircuits().GetInternalTemperatureCircuit();

  double testBMR = 1700;

  SEThermalCircuitPath* GroundToCore = m_TCircuit.GetPath(BGE::InternalTemperaturePath::GroundToInternalCore);
  GroundToCore->GetHeatSourceBaseline().SetValue(testBMR, PowerUnit::kcal_Per_day);
  SEThermalCircuitNode* Core = m_TCircuit.GetNode(BGE::InternalTemperatureNode::InternalCore);
  Core->GetTemperature().SetValue(36.85, TemperatureUnit::C);
  Core->GetNextTemperature().SetValue(36.85, TemperatureUnit::C);
  SEThermalCircuitNode* Skin = m_TCircuit.GetNode(BGE::InternalTemperatureNode::InternalSkin);
  Skin->GetTemperature().SetValue(33.0, TemperatureUnit::C);
  Skin->GetNextTemperature().SetValue(33.0, TemperatureUnit::C);
  SEThermalCircuitNode* Ground = m_TCircuit.GetNode(BGE::InternalTemperatureNode::InternalGround);

  SEThermalCircuitPath& CoreToEnvironment = m_TCircuit.CreatePath(*Core, *Ground, "EnvironmentCore");
  CoreToEnvironment.GetHeatSourceBaseline().SetValue(0.3 * testBMR, PowerUnit::kcal_Per_day);

  SEThermalCircuitPath& SkinToEnvironment = m_TCircuit.CreatePath(*Skin, *Ground, "EnvironmentSkin");
  SkinToEnvironment.GetHeatSourceBaseline().SetValue(0.7 * testBMR, PowerUnit::kcal_Per_day);

  m_TCircuit.SetNextAndCurrentFromBaselines();
  m_TCircuit.StateChange();

  DataTrack outTrk;

  std::ofstream file;

  SEThermalCircuitCalculator calc(bg.GetLogger());

  double deltaT_s = 1.0 / 90;
  double inv_deltaT_s = 1.0 / deltaT_s;
  double time_s = 0;

  //For each timestep, simulate, holding BMR and EnvironmentCore constant
  for (unsigned int i = 0; i < (inv_deltaT_s * 120); i++) {
    //Drive the circuit (step function)
    if (i == (inv_deltaT_s * 10)) {
      //Increase EnvironmentCore (this means that the total of EnvSkin and EnvCore > BMR)
      CoreToEnvironment.GetHeatSourceBaseline().SetValue(0.5 * testBMR, PowerUnit::kcal_Per_day);
    } else if (i == (inv_deltaT_s * 20)) {
      //Increase EnvironmentCore again
      CoreToEnvironment.GetHeatSourceBaseline().SetValue(.7 * testBMR, PowerUnit::kcal_Per_day);
    } else if (i == (inv_deltaT_s * 30)) {
      //Decrease EnvironmentCore to normal
      CoreToEnvironment.GetHeatSourceBaseline().SetValue(.3 * testBMR, PowerUnit::kcal_Per_day);
    } else if (i == (inv_deltaT_s * 40)) {
      //Decrease EnvironmentCore further
      CoreToEnvironment.GetHeatSourceBaseline().SetValue(.2 * testBMR, PowerUnit::kcal_Per_day);
    } else if (i == (inv_deltaT_s * 50)) {
      //Decrease EnvironmentCore further
      CoreToEnvironment.GetHeatSourceBaseline().SetValue(.1 * testBMR, PowerUnit::kcal_Per_day);
    } else if (i == (inv_deltaT_s * 60)) {
      //Set to 0
      CoreToEnvironment.GetHeatSourceBaseline().SetValue(0, PowerUnit::kcal_Per_day);
    }

    //Process - Execute the circuit
    calc.Process(m_TCircuit, deltaT_s);

    //Convert "next" values to current
    calc.PostProcess(m_TCircuit);

    //Record data
    outTrk.Track(time_s, m_TCircuit);
    if (i == 0) {
      outTrk.CreateFile(std::string(sTestDirectory + "/InternalTemperatureVariableCoreCircuitOutput.csv").c_str(), file);
    }
    outTrk.StreamTrackToFile(file);
    time_s += deltaT_s;
  }
  file.close();
}

//This test uses a constant BMR in place of the Energy circuit and varies ambient temperature
void BioGearsEngineTest::EnvironmentVariableTemperatureCircuitTest(const std::string& sTestDirectory)
{
  BioGears bg(sTestDirectory + "/EnvironmentVariableTemperatureCircuitTest.log");
  bg.GetPatient().Load("./patients/StandardMale.xml");
  bg.SetupPatient();
  bg.m_Config->EnableRenal(CDM::enumOnOff::Off);
  bg.m_Config->EnableTissue(CDM::enumOnOff::Off);
  bg.CreateCircuitsAndCompartments();

  //Grab the circuit
  SEThermalCircuit& m_ECircuit = bg.GetCircuits().GetExternalTemperatureCircuit();

  double testBMR = 1700;

  SEThermalCircuitPath* SkinToClothing = m_ECircuit.GetPath(BGE::ExternalTemperaturePath::ExternalSkinToClothing);
  SEThermalCircuitPath* ClothingToEnclosure = m_ECircuit.GetPath(BGE::ExternalTemperaturePath::ClothingToEnclosure);
  SEThermalCircuitPath* GroundToEnvironment = m_ECircuit.GetPath(BGE::ExternalTemperaturePath::GroundToEnvironment);
  SEThermalCircuitPath* ClothingToEnvironment = m_ECircuit.GetPath(BGE::ExternalTemperaturePath::ClothingToEnvironment);
  SEThermalCircuitPath* GroundToEnclosure = m_ECircuit.GetPath(BGE::ExternalTemperaturePath::GroundToEnclosure);

  //You must initialize these resistors or the circuit won't solve
  //These values were pulled from EnvironmentInput.csv from an older unit test
  //SkinToClothing->GetResistanceBaseline().SetValue(22.836, HeatResistanceUnit::K_Per_W);  //R Clothing
  SkinToClothing->GetResistanceBaseline().SetValue(.05, HeatResistanceUnit::K_Per_W); //R Clothing
  ClothingToEnvironment->GetResistanceBaseline().SetValue(4.379, HeatResistanceUnit::K_Per_W); //R Convection
  ClothingToEnclosure->GetResistanceBaseline().SetValue(0.039, HeatResistanceUnit::K_Per_W); //R Radiation

  // Make a new source attached to Environment circuit to take the place of the Energy circuit
  SEThermalCircuitNode* Core = m_ECircuit.GetNode(BGE::ExternalTemperatureNode::ExternalCore);
  SEThermalCircuitNode* Skin = m_ECircuit.GetNode(BGE::ExternalTemperatureNode::ExternalSkin);
  SEThermalCircuitNode* Ground = m_ECircuit.GetNode(BGE::ExternalTemperatureNode::ExternalGround);

  SEThermalCircuitNode& MetabolicNode = m_ECircuit.CreateNode("Metabolic");
  SEThermalCircuitPath& MetabolicPath = m_ECircuit.CreatePath(*Ground, MetabolicNode, "GroundToMetabolic");
  SEThermalCircuitPath& MetabolicToCore = m_ECircuit.CreatePath(*Core, MetabolicNode, "CoreToMetabolic");
  SEThermalCircuitPath& MetabolicToSkin = m_ECircuit.CreatePath(*Skin, MetabolicNode, "SkinToMetabolic");

  //Hold BMR constant
  MetabolicPath.GetHeatSourceBaseline().SetValue(testBMR, PowerUnit::kcal_Per_day);
  MetabolicPath.GetNextHeatSource().SetValue(testBMR, PowerUnit::kcal_Per_day);

  //Set some initial temperatures
  Core->GetTemperature().SetValue(37.0, TemperatureUnit::C);
  Skin->GetTemperature().SetValue(33.0, TemperatureUnit::C);
  SEThermalCircuitNode* Clothing = m_ECircuit.GetNode(BGE::ExternalTemperatureNode::Clothing);
  Clothing->GetTemperature().SetValue(30.0, TemperatureUnit::C);
  SEThermalCircuitNode* Environment = m_ECircuit.GetNode(BGE::ExternalTemperatureNode::Ambient);
  Environment->GetTemperature().SetValue(22.0, TemperatureUnit::C);
  MetabolicNode.GetTemperature().SetValue(22.0, TemperatureUnit::C);

  m_ECircuit.SetNextAndCurrentFromBaselines();
  m_ECircuit.StateChange();

  DataTrack outTrk;

  std::ofstream file;

  SEThermalCircuitCalculator calc(bg.GetLogger());

  double deltaT_s = 1.0 / 90;
  double inv_deltaT_s = 1.0 / deltaT_s;
  double time_s = 0;

  //For each timestep, simulate, holding everything constant but change ambient temp
  for (unsigned int i = 0; i < (inv_deltaT_s * 120); i++) {
    //Drive the circuit (step function)
    if (i == (0)) {
      //Initialization above doesn't seem to work for ambient temp, so set it here
      GroundToEnvironment->GetNextTemperatureSource().SetValue(22.0, TemperatureUnit::C);
      GroundToEnclosure->GetNextTemperatureSource().SetValue(22.0, TemperatureUnit::C);
      Clothing->GetNextTemperature().SetValue(22.0, TemperatureUnit::C);
    } else if (i == (inv_deltaT_s * 10)) {
      //Make it cold
      GroundToEnvironment->GetNextTemperatureSource().SetValue(10.0, TemperatureUnit::C);
    } else if (i == (inv_deltaT_s * 20)) {
      //Make it freezing
      //NOTE: At the time of creating this test, using a value of 0.0 C incorrectly sets to 0.0 K, so use 0.1
      GroundToEnvironment->GetNextTemperatureSource().SetValue(0.1, TemperatureUnit::C);
    } else if (i == (inv_deltaT_s * 30)) {
      //Increase to body temp
      GroundToEnvironment->GetNextTemperatureSource().SetValue(37.0, TemperatureUnit::C);
    } else if (i == (inv_deltaT_s * 40)) {
      GroundToEnvironment->GetNextTemperatureSource().SetValue(45.0, TemperatureUnit::C);
    } else if (i == (inv_deltaT_s * 50)) {
      //Make it as hot as you can expect
      GroundToEnvironment->GetNextTemperatureSource().SetValue(55.0, TemperatureUnit::C);
    } else if (i == (inv_deltaT_s * 60)) {
      //Back to freezing
      GroundToEnvironment->GetNextTemperatureSource().SetValue(0.1, TemperatureUnit::C);
    }

    //Process - Execute the circuit
    calc.Process(m_ECircuit, deltaT_s);

    //Convert "next" values to current
    calc.PostProcess(m_ECircuit);

    //Record data
    outTrk.Track(time_s, m_ECircuit);
    if (i == 0) {
      outTrk.CreateFile(std::string(sTestDirectory + "/EnvironmentVariableTemperatureCircuitOutput.csv").c_str(), file);
    }
    outTrk.StreamTrackToFile(file);
    time_s += deltaT_s;
  }
  file.close();
}

//This test uses both the Environment and Energy circuits, varying both BMR and ambient temp
void BioGearsEngineTest::CombinedInternalAndEnvironmentVariableBMRandTemperatureCircuitTest(const std::string& sTestDirectory)
{
  BioGears bg(sTestDirectory + "/CombinedInternalAndEnvironmentVariableBMRandTemperatureCircuitTest.log");
  bg.GetPatient().Load("./patients/StandardMale.xml");
  bg.SetupPatient();
  bg.m_Config->EnableRenal(CDM::enumOnOff::Off);
  bg.m_Config->EnableTissue(CDM::enumOnOff::Off);
  bg.CreateCircuitsAndCompartments();

  SEThermalCircuit& m_TECircuit = bg.GetCircuits().GetTemperatureCircuit();

  SEThermalCircuitPath* MetabolicPath = m_TECircuit.GetPath(BGE::InternalTemperaturePath::GroundToInternalCore);
  SEThermalCircuitPath* CoreToSkin = m_TECircuit.GetPath(BGE::InternalTemperaturePath::InternalCoreToInternalSkin);
  SEThermalCircuitPath* EnvCoreToGround = m_TECircuit.GetPath(BGE::ExternalTemperaturePath::ExternalCoreToGround);
  SEThermalCircuitPath* EnvSkinToGround = m_TECircuit.GetPath(BGE::ExternalTemperaturePath::ExternalSkinToGround);
  SEThermalCircuitPath* EnvSkinToClothing = m_TECircuit.GetPath(BGE::ExternalTemperaturePath::ExternalSkinToClothing);
  SEThermalCircuitPath* ClothingToEnclosure = m_TECircuit.GetPath(BGE::ExternalTemperaturePath::ClothingToEnclosure);
  SEThermalCircuitPath* GroundToEnvironment = m_TECircuit.GetPath(BGE::ExternalTemperaturePath::GroundToEnvironment);
  SEThermalCircuitPath* GroundToEnclosure = m_TECircuit.GetPath(BGE::ExternalTemperaturePath::GroundToEnclosure);
  SEThermalCircuitPath* ClothingToEnvironment = m_TECircuit.GetPath(BGE::ExternalTemperaturePath::ClothingToEnvironment);

  //You must initialize these resistors or the circuit won't solve
  //These values were pulled from EnvironmentInput.csv from an older unit test
  double skinBloodFlow_m3Persec = 4.97E-06;
  double bloodDensity_kgPerm3 = 1050;
  double bloodSpecificHeat_JPerkgK = 3617;
  double alphaScale = 0.79; //tuning parameter for scaling resistance
  double CoreToSkinResistance_KPerW;
  CoreToSkinResistance_KPerW = 1.0 / (alphaScale * bloodDensity_kgPerm3 * bloodSpecificHeat_JPerkgK * skinBloodFlow_m3Persec);

  EnvSkinToClothing->GetResistanceBaseline().SetValue(22.836, HeatResistanceUnit::K_Per_W); //R Clothing (this high value should prevent temp changes)
  ClothingToEnvironment->GetResistanceBaseline().SetValue(4.379, HeatResistanceUnit::K_Per_W); //R Convection
  ClothingToEnclosure->GetResistanceBaseline().SetValue(0.039, HeatResistanceUnit::K_Per_W); //R Radiation
  CoreToSkin->GetResistanceBaseline().SetValue(CoreToSkinResistance_KPerW, HeatResistanceUnit::K_Per_W); //R bloodflow heat transfer

  //Initialize BMR
  double testBMR = 1700;
  MetabolicPath->GetHeatSourceBaseline().SetValue(testBMR, PowerUnit::kcal_Per_day);
  MetabolicPath->GetNextHeatSource().SetValue(testBMR, PowerUnit::kcal_Per_day);

  //Set some initial temperatures
  SEThermalCircuitNode* Core = m_TECircuit.GetNode(BGE::InternalTemperatureNode::InternalCore);
  Core->GetTemperature().SetValue(37.0, TemperatureUnit::C);
  SEThermalCircuitNode* Skin = m_TECircuit.GetNode(BGE::InternalTemperatureNode::InternalSkin);
  Skin->GetTemperature().SetValue(33.0, TemperatureUnit::C);
  SEThermalCircuitNode* Clothing = m_TECircuit.GetNode(BGE::ExternalTemperatureNode::Clothing);
  Clothing->GetTemperature().SetValue(30.0, TemperatureUnit::C);
  SEThermalCircuitNode* Environment = m_TECircuit.GetNode(BGE::ExternalTemperatureNode::Ambient);
  Environment->GetTemperature().SetValue(22.0, TemperatureUnit::C);

  m_TECircuit.SetNextAndCurrentFromBaselines();
  m_TECircuit.StateChange();

  DataTrack outTrk;
  std::ofstream file;
  SEThermalCircuitCalculator calc(bg.GetLogger());

  double deltaT_s = 1.0 / 90;
  double inv_deltaT_s = 1.0 / deltaT_s;
  double time_s = 0;

  //For each timestep, simulate
  //For the first 120 seconds, hold everything constant and vary BMR
  //For the next 120 seconds, hold BMR constant and vary ambient temp
  for (unsigned int i = 0; i < (inv_deltaT_s * 240); i++) {
    //Drive the circuit (step function)
    if (i == 0) {
      //Initialization above doesn't seem to work for ambient temp, so set it here
      GroundToEnvironment->GetNextTemperatureSource().SetValue(22.0, TemperatureUnit::C);
    } else if (i == (inv_deltaT_s * 10)) {
      //Increase BMR
      MetabolicPath->GetHeatSourceBaseline().SetValue((testBMR + .2 * testBMR), PowerUnit::kcal_Per_day);
    } else if (i == (inv_deltaT_s * 20)) {
      //Increase BMR again
      MetabolicPath->GetHeatSourceBaseline().SetValue((testBMR + .4 * testBMR), PowerUnit::kcal_Per_day);
    } else if (i == (inv_deltaT_s * 30)) {
      //Decrease BMR to normal
      MetabolicPath->GetHeatSourceBaseline().SetValue((testBMR), PowerUnit::kcal_Per_day);
    } else if (i == (inv_deltaT_s * 40)) {
      //Decrease BMR further
      MetabolicPath->GetHeatSourceBaseline().SetValue((testBMR - .2 * testBMR), PowerUnit::kcal_Per_day);
    } else if (i == (inv_deltaT_s * 50)) {
      //Decrease BMR further
      MetabolicPath->GetHeatSourceBaseline().SetValue((testBMR - .4 * testBMR), PowerUnit::kcal_Per_day);
    } else if (i == (inv_deltaT_s * 60)) {
      //Set to 0
      MetabolicPath->GetHeatSourceBaseline().SetValue(0, PowerUnit::kcal_Per_day);
    } else if (i == (inv_deltaT_s * 120)) {
      //Set BMR to normal, change ambient temp
      MetabolicPath->GetHeatSourceBaseline().SetValue(testBMR, PowerUnit::kcal_Per_day);
      GroundToEnvironment->GetNextTemperatureSource().SetValue(10.0, TemperatureUnit::C);
    } else if (i == (inv_deltaT_s * 130)) {
      //Make it freezing
      //NOTE: At the time of creating this test, using a value of 0.0 C incorrectly sets to 0.0 K, so use 0.1
      GroundToEnvironment->GetNextTemperatureSource().SetValue(0.1, TemperatureUnit::C);
    } else if (i == (inv_deltaT_s * 140)) {
      //Increase to body temp
      GroundToEnvironment->GetNextTemperatureSource().SetValue(37.0, TemperatureUnit::C);
    } else if (i == (inv_deltaT_s * 150)) {
      //Hotter...
      GroundToEnvironment->GetNextTemperatureSource().SetValue(45.0, TemperatureUnit::C);
    } else if (i == (inv_deltaT_s * 160)) {
      //Make it as hot as you can expect naturally
      GroundToEnvironment->GetNextTemperatureSource().SetValue(55.0, TemperatureUnit::C);
    } else if (i == (inv_deltaT_s * 170)) {
      //Back to freezing
      GroundToEnvironment->GetNextTemperatureSource().SetValue(0.1, TemperatureUnit::C);
    }

    //Process - Execute the circuit
    calc.Process(m_TECircuit, deltaT_s);

    //Convert "next" values to current
    calc.PostProcess(m_TECircuit);

    //Record data
    outTrk.Track(time_s, m_TECircuit);
    if (i == 0) {
      outTrk.CreateFile(std::string(sTestDirectory + "/CombinedInternalAndEnvironmentVariableBMRandTemperatureCircuitOutput.csv").c_str(), file);
    }
    outTrk.StreamTrackToFile(file);
    time_s += deltaT_s;
  }
  file.close();
}

//This test verifies that the Skin temperature drops when drastically lowering other values
void BioGearsEngineTest::CombinedInternalAndEnvironmentSkinTempDropCircuitTest(const std::string& sTestDirectory)
{
  BioGears bg(sTestDirectory + "/CombinedInternalAndEnvironmentSkinTempDropCircuitTest.log");
  bg.GetPatient().Load("./patients/StandardMale.xml");
  bg.SetupPatient();
  bg.m_Config->EnableRenal(CDM::enumOnOff::Off);
  bg.m_Config->EnableTissue(CDM::enumOnOff::Off);
  bg.CreateCircuitsAndCompartments();

  SEThermalCircuit& m_TECircuit = bg.GetCircuits().GetTemperatureCircuit();

  SEThermalCircuitPath* MetabolicPath = m_TECircuit.GetPath(BGE::InternalTemperaturePath::GroundToInternalCore);
  SEThermalCircuitPath* CoreToSkin = m_TECircuit.GetPath(BGE::InternalTemperaturePath::InternalCoreToInternalSkin);
  SEThermalCircuitPath* EnvCoreToGround = m_TECircuit.GetPath(BGE::ExternalTemperaturePath::ExternalCoreToGround);
  SEThermalCircuitPath* EnvSkinToGround = m_TECircuit.GetPath(BGE::ExternalTemperaturePath::ExternalSkinToGround);
  SEThermalCircuitPath* EnvSkinToClothing = m_TECircuit.GetPath(BGE::ExternalTemperaturePath::ExternalSkinToClothing);
  SEThermalCircuitPath* ClothingToEnclosure = m_TECircuit.GetPath(BGE::ExternalTemperaturePath::ClothingToEnclosure);
  SEThermalCircuitPath* GroundToEnvironment = m_TECircuit.GetPath(BGE::ExternalTemperaturePath::GroundToEnvironment);
  SEThermalCircuitPath* GroundToEnclosure = m_TECircuit.GetPath(BGE::ExternalTemperaturePath::GroundToEnclosure);
  SEThermalCircuitPath* ClothingToEnvironment = m_TECircuit.GetPath(BGE::ExternalTemperaturePath::ClothingToEnvironment);

  SEThermalCircuitNode* Clothing = m_TECircuit.GetNode(BGE::ExternalTemperatureNode::Clothing);

  //You must initialize these resistors or the circuit won't solve
  //These values were pulled from EnvironmentInput.csv from an older unit test
  double skinBloodFlow_m3Persec = 4.97E-06;
  double bloodDensity_kgPerm3 = 1050;
  double bloodSpecificHeat_JPerkgK = 3617;
  double alphaScale = 0.79; //tuning parameter for scaling resistance
  double CoreToSkinResistance_KPerW;
  CoreToSkinResistance_KPerW = 1.0 / (alphaScale * bloodDensity_kgPerm3 * bloodSpecificHeat_JPerkgK * skinBloodFlow_m3Persec);

  //EnvironmentSkinToClothing->GetHeatResistanceBaseline()->SetValue(22.836, HeatResistanceUnit::K_Per_W);  //R Clothing (this high resistance really slows cooling)
  EnvSkinToClothing->GetResistanceBaseline().SetValue(.05, HeatResistanceUnit::K_Per_W); //R Clothing
  ClothingToEnvironment->GetResistanceBaseline().SetValue(4.379, HeatResistanceUnit::K_Per_W); //R Convection
  ClothingToEnclosure->GetResistanceBaseline().SetValue(0.039, HeatResistanceUnit::K_Per_W); //R Radiation
  CoreToSkin->GetResistanceBaseline().SetValue(CoreToSkinResistance_KPerW, HeatResistanceUnit::K_Per_W); //R bloodflow heat transfer

  //Initialize BMR
  double testBMR = 1700;
  MetabolicPath->GetHeatSourceBaseline().SetValue(testBMR, PowerUnit::kcal_Per_day);
  MetabolicPath->GetNextHeatSource().SetValue(testBMR, PowerUnit::kcal_Per_day);

  m_TECircuit.SetNextAndCurrentFromBaselines();
  m_TECircuit.StateChange();

  DataTrack outTrk;
  std::ofstream file;
  SEThermalCircuitCalculator calc(bg.GetLogger());

  double deltaT_s = 1.0 / 30;
  double inv_deltaT_s = 1.0 / deltaT_s;
  double time_s = 0;

  //For each timestep, simulate
  //Drop energy inputs after 10 seconds
  for (unsigned int i = 0; i < ((1 / deltaT_s) * 3600); i++) {
    //Drive the circuit (step function)
    if (i == 0) {
      //Initialization above doesn't seem to work for ambient temp, so set it here
      GroundToEnvironment->GetNextTemperatureSource().SetValue(22.0, TemperatureUnit::C);
      GroundToEnclosure->GetNextTemperatureSource().SetValue(22.0, TemperatureUnit::C);
      Clothing->GetNextTemperature().SetValue(22.0, TemperatureUnit::C);
    }
    //Now kill BMR and drop all temperatures
    else if (i == (inv_deltaT_s * 10)) {
      MetabolicPath->GetHeatSourceBaseline().SetValue(0, PowerUnit::kcal_Per_day);
      GroundToEnvironment->GetNextTemperatureSource().SetValue(-40.0, TemperatureUnit::C);
      GroundToEnclosure->GetNextTemperatureSource().SetValue(-40.0, TemperatureUnit::C);
      Clothing->GetNextTemperature().SetValue(-40.0, TemperatureUnit::C);
    }

    //Process - Execute the circuit
    calc.Process(m_TECircuit, deltaT_s);

    //Convert "next" values to current
    calc.PostProcess(m_TECircuit);

    //Record data
    outTrk.Track(time_s, m_TECircuit);
    if (i == 0) {
      outTrk.CreateFile(std::string(sTestDirectory + "/CombinedInternalAndEnvironmentSkinTempDropCircuitOutput.csv").c_str(), file);
    }
    outTrk.StreamTrackToFile(file);
    time_s += deltaT_s;
  }
  file.close();
}

//This test compares Environment circuit output to ISO data
void BioGearsEngineTest::EnvironmentISO7730ComparisonTest(const std::string& sTestDirectory)
{
  BioGears bg(sTestDirectory + "/EnvironmentTemperatureInput.log");
  bg.GetPatient().Load("./patients/StandardMale.xml");
  bg.SetupPatient();
  bg.m_Config->EnableRenal(CDM::enumOnOff::Off);
  bg.m_Config->EnableTissue(CDM::enumOnOff::Off);
  bg.CreateCircuitsAndCompartments();
  Environment& env = (Environment&)bg.GetEnvironment();
  env.Initialize();
  env.GetConditions().Load("./environments/Standard.xml");
  env.StateChange();

  bg.GetEnergy().GetCoreTemperature().SetValue(37.0, TemperatureUnit::C);
  bg.GetEnergy().GetSweatRate().SetValue(1.5e-5, MassPerTimeUnit::kg_Per_s);
  bg.GetRespiratory().GetRespirationRate().SetValue(16.0, FrequencyUnit::Per_min);
  bg.GetRespiratory().GetTotalPulmonaryVentilation().SetValue(16 /*RespirationRate*/ * 0.5 /*TidalVolume_L*/, VolumePerTimeUnit::L_Per_min);
  double dSkinSurfaceArea_M2 = bg.GetPatient().GetSkinSurfaceArea(AreaUnit::m2);

  //Inputs
  double dAirTemperature_C[13] = { 22, 27, 27, 23.5, 23.5, 19, 23.5, 23.5, 23, 23, 22, 27, 27 };
  double dMeanRadiantTemperature_C[13] = { 22, 27, 27, 25.5, 25.5, 19, 23.5, 23.5, 21, 21, 22, 27, 27 };
  double dRelativeAirVelocity_MPerS[13] = { 0.1, 0.1, 0.3, 0.1, 0.3, 0.1, 0.1, 0.3, 0.1, 0.3, 0.1, 0.1, 0.3 };
  double dRelativeHumidity[13] = { 60, 60, 60, 60, 60, 40, 40, 40, 40, 40, 60, 60, 60 };
  double dClothing_clo[13] = { 0.5, 0.5, 0.5, 0.5, 0.5, 1, 1, 1, 1, 1, 0.5, 0.5, 0.5 };
  double dMetabolicRate_met[13] = { 1.2, 1.2, 1.2, 1.2, 1.2, 1.2, 1.2, 1.2, 1.2, 1.2, 1.6, 1.6, 1.6 };

  //Outputs
  double dClothingSurfaceTemperature_C[13];
  double dConvectiveHeatLoss_WPerM2[13];
  double dRadiativeHeatLoss_WPerM2[13];
  double dRespirationHeatLoss_WPerM2[13];
  double dEvaporativeHeatLoss_WPerM2[13];

  double dAmbientTemperature_K = 295.4; //~72F

  SEThermalCircuit& EnvironmentCircuit = bg.GetCircuits().GetExternalTemperatureCircuit();
  SEThermalCircuitNode* ClothingNode = EnvironmentCircuit.GetNode(BGE::ExternalTemperatureNode::Clothing);
  SEThermalCircuitNode* EnvironmentCoreNode = EnvironmentCircuit.GetNode(BGE::ExternalTemperatureNode::ExternalCore);
  SEThermalCircuitNode* EnvironmentSkinNode = EnvironmentCircuit.GetNode(BGE::ExternalTemperatureNode::ExternalSkin);
  SEThermalCircuitNode* AbsoluteReferenceNode = EnvironmentCircuit.GetNode(BGE::ExternalTemperatureNode::ExternalGround);

  // Create some new stuff in the circuit
  SEThermalCircuitNode& MetabolicNode = EnvironmentCircuit.CreateNode("Metabolic");
  MetabolicNode.GetTemperature().SetValue(dAmbientTemperature_K, TemperatureUnit::K);
  SEThermalCircuitPath& MetabolicPath = EnvironmentCircuit.CreatePath(*AbsoluteReferenceNode, MetabolicNode, "AbsoluteReferenceToMetabolic");
  SEThermalCircuitPath& MetabolicToEnvironmentSkinPath = EnvironmentCircuit.CreatePath(MetabolicNode, *EnvironmentSkinNode, "MetabolicToEnvironmentSkin");
  SEThermalCircuitPath& MetabolicToEnvironmentCorePath = EnvironmentCircuit.CreatePath(MetabolicNode, *EnvironmentCoreNode, "MetabolicToEnvironmentCore");

  //Initialize the new source so that circuit solves on first loop
  MetabolicPath.GetNextHeatSource().SetValue(0, PowerUnit::W);

  EnvironmentCircuit.SetNextAndCurrentFromBaselines();
  EnvironmentCircuit.StateChange();

  double deltaT = 1.0 / 90;
  SEThermalCircuitCalculator calc(bg.GetLogger());

  for (unsigned int i = 0; i < 13; i++) {
    env.GetConditions().GetAmbientTemperature().SetValue(dAirTemperature_C[i], TemperatureUnit::C);
    env.GetConditions().GetMeanRadiantTemperature().SetValue(dMeanRadiantTemperature_C[i], TemperatureUnit::C);
    env.GetConditions().GetAirVelocity().SetValue(dRelativeAirVelocity_MPerS[i], LengthPerTimeUnit::m_Per_s);
    env.GetConditions().GetRelativeHumidity().SetValue(dRelativeHumidity[i] / 100.0);
    env.GetConditions().GetClothingResistance().SetValue(dClothing_clo[i], HeatResistanceAreaUnit::clo);
    env.StateChange();

    //Convert from mets and get rid of area
    double dMetabolicRate_W = dMetabolicRate_met[i] * 58.2 * dSkinSurfaceArea_M2;

    //Loop enough times to stabilize everything
    double dClothingTemperatureChange_K = 100.0;
    while (dClothingTemperatureChange_K > 1e-10) {
      MetabolicPath.GetNextHeatSource().SetValue(dMetabolicRate_W, PowerUnit::W);
      env.PreProcess();
      calc.Process(EnvironmentCircuit, deltaT);
      //Get the change in temperature on the clothing node from last time-step
      dClothingTemperatureChange_K = std::abs(ClothingNode->GetNextTemperature().GetValue(TemperatureUnit::K) - ClothingNode->GetTemperature().GetValue(TemperatureUnit::K));
      calc.PostProcess(EnvironmentCircuit);
    }

    dClothingSurfaceTemperature_C[i] = ClothingNode->GetTemperature().GetValue(TemperatureUnit::C);
    dConvectiveHeatLoss_WPerM2[i] = env.GetConvectiveHeatLoss(PowerUnit::W) / dSkinSurfaceArea_M2;
    dRadiativeHeatLoss_WPerM2[i] = env.GetRadiativeHeatLoss(PowerUnit::W) / dSkinSurfaceArea_M2;
    dRespirationHeatLoss_WPerM2[i] = env.GetRespirationHeatLoss(PowerUnit::W) / dSkinSurfaceArea_M2;
    dEvaporativeHeatLoss_WPerM2[i] = env.GetEvaporativeHeatLoss(PowerUnit::W) / dSkinSurfaceArea_M2;
  }

  //Write results to file
  DataTrack trk;
  for (unsigned int i = 0; i < 13; i++) {
    trk.Track("ClothingSurfaceTemperature(C)", i, dClothingSurfaceTemperature_C[i]);
    trk.Track("ConvectiveHeatLoss(W/m^2)", i, dConvectiveHeatLoss_WPerM2[i]);
    trk.Track("RadiativeHeatLoss(W/m^2)", i, dRadiativeHeatLoss_WPerM2[i]);
    trk.Track("RespirationHeatLoss(W/m^2)", i, dRespirationHeatLoss_WPerM2[i]);
    trk.Track("EvaporativeHeatLoss(W/m^2)", i, dEvaporativeHeatLoss_WPerM2[i]);
  }

  std::string sOutputFile = sTestDirectory + "/EnvironmentISO7730Comparison.csv";
  trk.WriteTrackToFile(sOutputFile.c_str());
}
}