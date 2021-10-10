//-------------------------------------------------------------------------------------------
//- Copyright 2017 Applied Research Associates, Inc.
//- Licensed under the ALche License, Version 2.0 (the "License"); you may not use
//- this file except in compliance with the License. You may obtain a copy of the License
//- at:
//- http://www.aLche.org/licenses/LICENSE-2.0
//- Unless required by applicable law or agreed to in writing, software distributed under
//- the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//- CONDITIONS OF ANY KIND, either express or implied. See the License for the
//-  specific language governing permissions and limitations under the License.
//-------------------------------------------------------------------------------------------
//!
//! @author David Lee
//! @date   2017 Aug 3rd
//!
//! Unit Test for Biogears-common Config
//!
#include <limits>
#include <thread>

#include <gtest/gtest.h>

#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/cdm/utils/FileUtils.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGearsEngine.h>

#include <biogears/engine/Controller/BioGears.h>
#include <biogears/engine/Systems/Gastrointestinal.h>

#include <iostream>
#include <string>
//
#include <biogears/cdm/properties/SEScalarNeg1To1.h>
#include <biogears/schema/cdm/EnvironmentConditions.hxx>
//
#ifdef DISABLE_BIOGEARS_Gastrointestinal_TEST
#define TEST_FIXTURE_NAME DISABLED_Circuit_Gastrointestinal_Fixture
#else
#define TEST_FIXTURE_NAME Circuit_Gastrointestinal_Fixture
#endif

using namespace biogears;
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
};

void TEST_FIXTURE_NAME::SetUp()
{
}

void TEST_FIXTURE_NAME::TearDown()
{
}

TEST_F(TEST_FIXTURE_NAME, Gastrointestinal_Circuit)
{
  biogears::BioGears bg("GastrointestinalCircuitTest.log");
  auto m_Circuits = std::unique_ptr<BioGearsCircuits>(new BioGearsCircuits(bg));
  auto m_Compartments = std::unique_ptr<BioGearsCompartments>(new BioGearsCompartments(bg));
  SEFluidCircuit& cCombinedCardiovascular = m_Circuits->GetActiveCardiovascularCircuit();
  SEFluidCircuitNode& SmallIntestineC1 = cCombinedCardiovascular.CreateNode(BGE::ChymeNode::SmallIntestineC1);
  SmallIntestineC1.GetPressure().SetValue(0, PressureUnit::mmHg);
  SmallIntestineC1.GetVolumeBaseline().SetValue(100, VolumeUnit::mL);
  SEFluidCircuitNode* SmallIntestine1 = cCombinedCardiovascular.GetNode(BGE::CardiovascularNode::SmallIntestine1);
  SEFluidCircuitNode* Ground = cCombinedCardiovascular.GetNode(BGE::CardiovascularNode::Ground);
  SEFluidCircuitPath& SmallIntestineC1ToSmallIntestine1 = cCombinedCardiovascular.CreatePath(SmallIntestineC1, *SmallIntestine1, BGE::ChymePath::SmallIntestineC1ToSmallIntestine1);
  SmallIntestineC1ToSmallIntestine1.GetFlowSourceBaseline().SetValue(0, VolumePerTimeUnit::mL_Per_min);
  //SEFluidCircuitPath& GroundToSmallIntestineC1 = cCombinedCardiovascular.CreatePath(*Ground, SmallIntestineC1, BGE::ChymePath::GroundToSmallIntestineC1); //Ok so this line makes the test fail at runtime
  cCombinedCardiovascular.SetNextAndCurrentFromBaselines();
  cCombinedCardiovascular.StateChange();
  SELiquidCompartment& cSmallIntestine = m_Compartments->CreateLiquidCompartment(BGE::ChymeCompartment::SmallIntestine);
  cSmallIntestine.MapNode(SmallIntestineC1);
  SELiquidCompartmentGraph& gCombinedCardiovascular = m_Compartments->GetActiveCardiovascularGraph();
  gCombinedCardiovascular.AddCompartment(cSmallIntestine);
  gCombinedCardiovascular.StateChange();
}

TEST_F(TEST_FIXTURE_NAME, ExternalTemperature_Circuit)
{
  biogears::BioGears bg("ExternalTemperatureCircuitTest.log");
  auto m_Circuits = std::unique_ptr<BioGearsCircuits>(new BioGearsCircuits(bg));
  auto m_Compartments = std::unique_ptr<BioGearsCompartments>(new BioGearsCompartments(bg));
  SEThermalCircuit& exthermal = m_Circuits->GetExternalTemperatureCircuit();
  double dAmbientTemperature_K = 295.4;
  //Nodes
  SEThermalCircuitNode& ThermalEnvironment = exthermal.CreateNode(BGE::ExternalTemperatureNode::Ambient);
  ThermalEnvironment.GetTemperature().SetValue(dAmbientTemperature_K, TemperatureUnit::K);
  SEThermalCircuitNode& Skin = exthermal.CreateNode(BGE::ExternalTemperatureNode::ExternalSkin);
  Skin.GetTemperature().SetValue(dAmbientTemperature_K, TemperatureUnit::K);
  SEThermalCircuitNode& Core = exthermal.CreateNode(BGE::ExternalTemperatureNode::ExternalCore);
  Core.GetTemperature().SetValue(dAmbientTemperature_K, TemperatureUnit::K);
  SEThermalCircuitNode& Clothing = exthermal.CreateNode(BGE::ExternalTemperatureNode::Clothing);
  Clothing.GetTemperature().SetValue(dAmbientTemperature_K, TemperatureUnit::K);
  SEThermalCircuitNode& Enclosure = exthermal.CreateNode(BGE::ExternalTemperatureNode::Enclosure);
  Enclosure.GetTemperature().SetValue(dAmbientTemperature_K, TemperatureUnit::K);
  SEThermalCircuitNode& Active = exthermal.CreateNode(BGE::ExternalTemperatureNode::Active);
  Active.GetTemperature().SetValue(dAmbientTemperature_K, TemperatureUnit::K);
  SEThermalCircuitNode& Ground = exthermal.CreateNode(BGE::ExternalTemperatureNode::ExternalGround);
  Ground.GetTemperature().SetValue(0.0, TemperatureUnit::K);
  Ground.GetNextTemperature().SetValue(0.0, TemperatureUnit::K);
  exthermal.AddReferenceNode(Ground);
  //Paths
  SEThermalCircuitPath& EnvironmentCoreToAbsoluteReferencePath = exthermal.CreatePath(Core, Ground, BGE::ExternalTemperaturePath::ExternalCoreToGround);
  EnvironmentCoreToAbsoluteReferencePath.GetNextHeatSource().SetValue(0.0, PowerUnit::W);
  SEThermalCircuitPath& EnvironmentSkinToAbsoluteReferencePath = exthermal.CreatePath(Skin, Ground, BGE::ExternalTemperaturePath::ExternalSkinToGround);
  EnvironmentSkinToAbsoluteReferencePath.GetNextHeatSource().SetValue(0.0, PowerUnit::W);
  SEThermalCircuitPath& EnvironmentSkinToClothingPath = exthermal.CreatePath(Skin, Clothing, BGE::ExternalTemperaturePath::ExternalSkinToClothing);
  EnvironmentSkinToClothingPath.GetNextResistance().SetValue(0.1, HeatResistanceUnit::K_Per_W);//changed this from dNoResistance
  SEThermalCircuitPath& AbsoluteReferenceToClothingPath = exthermal.CreatePath(Ground, Clothing, BGE::ExternalTemperaturePath::GroundToClothing);
  AbsoluteReferenceToClothingPath.GetNextHeatSource().SetValue(0.0, PowerUnit::W);
  SEThermalCircuitPath& ClothingToAmbientEnvironmentPath = exthermal.CreatePath(Clothing, ThermalEnvironment, BGE::ExternalTemperaturePath::ClothingToEnvironment);
  ClothingToAmbientEnvironmentPath.GetNextResistance().SetValue(0.1, HeatResistanceUnit::K_Per_W);//changed this from dNoResistance
  SEThermalCircuitPath& ClothingToEnclosure = exthermal.CreatePath(Clothing, Enclosure, BGE::ExternalTemperaturePath::ClothingToEnclosure);
  ClothingToEnclosure.GetNextResistance().SetValue(0.1, HeatResistanceUnit::K_Per_W);//changed this from dNoResistance
  SEThermalCircuitPath& AbsoluteReferenceToAmbientEnvironmentPath = exthermal.CreatePath(Ground, ThermalEnvironment, BGE::ExternalTemperaturePath::GroundToEnvironment);
  AbsoluteReferenceToAmbientEnvironmentPath.GetNextTemperatureSource().SetValue(0.0, TemperatureUnit::K);
  SEThermalCircuitPath& AbsoluteReferenceToEnclosurePath = exthermal.CreatePath(Ground, Enclosure, BGE::ExternalTemperaturePath::GroundToEnclosure);
  AbsoluteReferenceToEnclosurePath.GetNextTemperatureSource().SetValue(0.0, TemperatureUnit::K);
  SEThermalCircuitPath& AbsoluteReferenceToActivePath = exthermal.CreatePath(Ground, Active, BGE::ExternalTemperaturePath::GroundToActive);
  AbsoluteReferenceToActivePath.GetNextTemperatureSource().SetValue(0.0, TemperatureUnit::K);
  SEThermalCircuitPath& ActiveToClothing = exthermal.CreatePath(Active, Clothing, BGE::ExternalTemperaturePath::ActiveToClothing);
  ActiveToClothing.SetNextSwitch(CDM::enumOpenClosed::Open);

  exthermal.SetNextAndCurrentFromBaselines();
  exthermal.StateChange();
  
  SEThermalCompartment& cActive = m_Compartments->CreateThermalCompartment(BGE::TemperatureCompartment::Active);
  cActive.MapNode(Active);
  SEThermalCompartment& cClothing = m_Compartments->CreateThermalCompartment(BGE::TemperatureCompartment::Clothing);
  cClothing.MapNode(Clothing);
  SEThermalCompartment& cEnclosure = m_Compartments->CreateThermalCompartment(BGE::TemperatureCompartment::Enclosure);
  cEnclosure.MapNode(Enclosure);
  SEThermalCompartment& cExternalCore = m_Compartments->CreateThermalCompartment(BGE::TemperatureCompartment::ExternalCore);
  cExternalCore.MapNode(Core);
  SEThermalCompartment& cExternalSkin = m_Compartments->CreateThermalCompartment(BGE::TemperatureCompartment::ExternalSkin);
  cExternalSkin.MapNode(Skin);
  SEThermalCompartment& cExternalGround = m_Compartments->CreateThermalCompartment(BGE::TemperatureCompartment::ExternalGround);
  cExternalGround.MapNode(Ground);
  SEThermalCompartment& cAmbient = m_Compartments->CreateThermalCompartment(BGE::TemperatureCompartment::Ambient);
  cAmbient.MapNode(ThermalEnvironment);

  SEThermalCompartmentLink& lActiveToClothing = m_Compartments->CreateThermalLink(cActive, cClothing, BGE::TemperatureLink::ActiveToClothing);
  lActiveToClothing.MapPath(ActiveToClothing);
  SEThermalCompartmentLink& lClothingToEnclosure = m_Compartments->CreateThermalLink(cClothing, cEnclosure, BGE::TemperatureLink::ClothingToEnclosure);
  lClothingToEnclosure.MapPath(ClothingToEnclosure);
  SEThermalCompartmentLink& lClothingToEnvironment = m_Compartments->CreateThermalLink(cClothing, cAmbient, BGE::TemperatureLink::ClothingToEnvironment);
  lClothingToEnvironment.MapPath(ClothingToAmbientEnvironmentPath);
  SEThermalCompartmentLink& lExternalCoreToGround = m_Compartments->CreateThermalLink(cExternalCore, cExternalGround, BGE::TemperatureLink::ExternalCoreToGround);
  lExternalCoreToGround.MapPath(EnvironmentCoreToAbsoluteReferencePath);
  SEThermalCompartmentLink& lGroundToActive = m_Compartments->CreateThermalLink(cExternalGround, cActive, BGE::TemperatureLink::GroundToActive);
  lGroundToActive.MapPath(AbsoluteReferenceToActivePath);
  SEThermalCompartmentLink& lGroundToClothing = m_Compartments->CreateThermalLink(cExternalGround, cClothing, BGE::TemperatureLink::GroundToClothing);
  lGroundToClothing.MapPath(AbsoluteReferenceToClothingPath);
  SEThermalCompartmentLink& lGroundToEnclosure = m_Compartments->CreateThermalLink(cExternalGround, cEnclosure, BGE::TemperatureLink::GroundToEnclosure);
  lGroundToEnclosure.MapPath(AbsoluteReferenceToEnclosurePath);
  SEThermalCompartmentLink& lGroundToEnvironment = m_Compartments->CreateThermalLink(cExternalGround, cAmbient, BGE::TemperatureLink::GroundToEnvironment);
  lGroundToEnvironment.MapPath(AbsoluteReferenceToAmbientEnvironmentPath);
  SEThermalCompartmentLink& lExternalSkinToGround = m_Compartments->CreateThermalLink(cExternalSkin, cExternalGround, BGE::TemperatureLink::ExternalSkinToGround);
  lExternalSkinToGround.MapPath(EnvironmentSkinToAbsoluteReferencePath);
  SEThermalCompartmentLink& lExternalSkinToClothing = m_Compartments->CreateThermalLink(cExternalSkin, cClothing, BGE::TemperatureLink::ExternalSkinToClothing);
  lExternalSkinToClothing.MapPath(EnvironmentSkinToClothingPath);
}

//TEST_F(TEST_FIXTURE_NAME, InternalTemperature_Circuit)
//{
//  biogears::BioGears bg("InternalTemperatureCircuitTest.log");
//  biogears::Logger logger;
//  auto m_Circuits = std::unique_ptr<BioGearsCircuits>(new BioGearsCircuits(bg));
//  auto m_Compartments = std::unique_ptr<BioGearsCompartments>(new BioGearsCompartments(bg));
//  auto m_Patient = std::unique_ptr<SEPatient>(new SEPatient(&logger));
//  auto m_Substances = std::unique_ptr<BioGearsSubstances>(new BioGearsSubstances(bg));
//  auto m_Config = std::unique_ptr<BioGearsConfiguration>(new BioGearsConfiguration(*m_Substances));
//  SEThermalCircuit& cIntemperature = m_Circuits->GetInternalTemperatureCircuit();
//
//  SEThermalCircuitNode& Core = cIntemperature.CreateNode(BGE::InternalTemperatureNode::InternalCore);
//  Core.GetTemperature().SetValue(37.0, TemperatureUnit::C);
//  //Peripheral Temperature
//  SEThermalCircuitNode& Skin = cIntemperature.CreateNode(BGE::InternalTemperatureNode::InternalSkin);
//  Skin.GetTemperature().SetValue(33.0, TemperatureUnit::C);
//  SEThermalCircuitNode& Ground = cIntemperature.CreateNode(BGE::InternalTemperatureNode::InternalGround);
//  Ground.GetTemperature().SetValue(0.0, TemperatureUnit::K);
//  Ground.GetNextTemperature().SetValue(0.0, TemperatureUnit::K);
//  cIntemperature.AddReferenceNode(Ground);
//  /*Metabolic heat generation, leading to a heat source into the core*/
//  SEThermalCircuitPath& TemperatureGroundToCore = cIntemperature.CreatePath(Ground, Core, BGE::InternalTemperaturePath::GroundToInternalCore);
//  TemperatureGroundToCore.GetHeatSourceBaseline().SetValue(0.0, PowerUnit::W);
//  /*Conduction and convection leading a resistance of heat flow from the core to the skin*/
//  SEThermalCircuitPath& CoreToSkin = cIntemperature.CreatePath(Core, Skin, BGE::InternalTemperaturePath::InternalCoreToInternalSkin);
//  CoreToSkin.GetResistanceBaseline().SetValue(0.056, HeatResistanceUnit::K_Per_W);
//
//  double skinMassFraction = 0.09; //0.09 is fraction of mass that the skin takes up in a typical human /cite herman2006physics
//  SEThermalCircuitPath& CoreToTemperatureGround = cIntemperature.CreatePath(Core, Ground, BGE::InternalTemperaturePath::InternalCoreToGround);
//  //CoreToTemperatureGround.GetCapacitanceBaseline().SetValue((1.0 - skinMassFraction) * m_Patient->GetWeight(MassUnit::kg) * m_Config.GetBodySpecificHeat(HeatCapacitancePerMassUnit::J_Per_K_kg), HeatCapacitanceUnit::J_Per_K);
//  Core.GetHeatBaseline().SetValue(CoreToTemperatureGround.GetCapacitanceBaseline().GetValue(HeatCapacitanceUnit::J_Per_K) * Core.GetTemperature().GetValue(TemperatureUnit::K), EnergyUnit::J);
//  // GetValue(HeatCapacitanceUnit::J_Per_K) is the call that results in a throw
//}
