#include <biogears/engine/BioGearsPhysiologyEngine.h>


#include <limits>
#include <thread>

#include <gtest/gtest.h>

#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include "biogears/engine/Controller/BioGears.h"


#ifdef DISABLE_BIOGEARS_MajorSystems_TEST
#define TEST_FIXTURE_NAME DISABLED_Circuit_MajorSystems_Fixture
#else
#define TEST_FIXTURE_NAME Circuit_MajorSystems_Fixture
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

TEST_F(TEST_FIXTURE_NAME, Anesthesia)
{
  std::unique_ptr<PhysiologyEngine> bge = CreateBioGearsEngine("AnesthesiaCircuitUnitTest.log");
  BioGears* bg = dynamic_cast<BioGears*>(bge.get());
  bge->LoadState("./states/StandardMale@0s.xml");
  bge->AdvanceModelTime();

  bg->GetLogger()->Info("AnesthesiaCircuitTest");
  BioGearsCircuits& circuits = bg->GetCircuits();
  SEFluidCircuit& anesthesia = circuits.GetAnesthesiaMachineCircuit();
  biogears::Logger m_Logger("UnitTest.log");
  SEFluidCircuitCalculator fcc{ &m_Logger };

  EXPECT_NO_THROW(fcc.Process(anesthesia, 1.0));
  EXPECT_NO_THROW(fcc.PostProcess(anesthesia));
}

TEST_F(TEST_FIXTURE_NAME, Cardiovascular)
{
  biogears::Logger m_Logger("CardiovascularCircuitTest.log");
  std::unique_ptr<PhysiologyEngine> bge = CreateBioGearsEngine(&m_Logger);
  BioGears* bg = dynamic_cast<BioGears*>(bge.get());
  bge->LoadState("./states/StandardMale@0s.xml");
  bge->AdvanceModelTime();

  bg->GetLogger()->Info("CardiovascularCircuitTest");
  BioGearsCircuits& circuits = bg->GetCircuits();
  SEFluidCircuit& cardiovascular = circuits.GetCardiovascularCircuit();
  SEFluidCircuitCalculator fcc{ &m_Logger };
  //for (int i = 0;i < 10000;++i){
  EXPECT_NO_THROW(fcc.Process(cardiovascular, 1.0));
  EXPECT_NO_THROW(fcc.PostProcess(cardiovascular)); 
  //}
}

//TEST_F(TEST_FIXTURE_NAME, Environment) //Placeholder until I can get a working Environment circuit
//{
//  biogears::Logger m_Logger("EnvironmentCircuitTest.log");
//  std::unique_ptr<PhysiologyEngine> bge = CreateBioGearsEngine(&m_Logger);
//  BioGears* bg = dynamic_cast<BioGears*>(bge.get());
//  bge->LoadState("./states/StandardMale@0s.xml");
//  bge->AdvanceModelTime();
//
//  double testBMR = 1700;   //need this OR we need to connect it to energy to caclulate
//
//  bg->GetLogger()->Info("EnvironmentCircuitTest");
//  BioGearsCircuits& circuits = bg->GetCircuits();
//  SEThermalCircuit& cExthermal = circuits.GetExternalTemperatureCircuit();
//  SEThermalCircuit& cInthermal = circuits.GetInternalTemperatureCircuit();
//  /*SEThermalCircuitNode* exCore = cExthermal.GetNode(BGE::ExternalTemperatureNode::ExternalCore);
//  SEThermalCircuitNode* exSkin = cExthermal.GetNode(BGE::ExternalTemperatureNode::ExternalSkin);
//  SEThermalCircuitNode* inCore = cInthermal.GetNode(BGE::InternalTemperatureNode::InternalCore);
//  SEThermalCircuitNode* inSkin = cInthermal.GetNode(BGE::InternalTemperatureNode::InternalSkin);
//  SEThermalCircuitPath& CoreTemperatureConnection = cThermal.CreatePath(*inCore, *exCore, BGE::CombinedTemperaturePath::InternalCoreToExternalCore);
//  SEThermalCircuitPath& SkinTemperatureConnection = cThermal.CreatePath(*inSkin, *exSkin, BGE::CombinedTemperaturePath::InternalSkinToExternalSkin);*/
//
//  SEThermalCircuitPath* SkinToClothing = cExthermal.GetPath(BGE::ExternalTemperaturePath::ExternalSkinToClothing);
//  SEThermalCircuitPath* ClothingToEnclosure = cExthermal.GetPath(BGE::ExternalTemperaturePath::ClothingToEnclosure);
//  SEThermalCircuitPath* GroundToEnvironment = cExthermal.GetPath(BGE::ExternalTemperaturePath::GroundToEnvironment);
//  SEThermalCircuitPath* ClothingToEnvironment = cExthermal.GetPath(BGE::ExternalTemperaturePath::ClothingToEnvironment);
//  SEThermalCircuitPath* GroundToEnclosure = cExthermal.GetPath(BGE::ExternalTemperaturePath::GroundToEnclosure);
//
//  //You must initialize these resistors or the circuit won't solve
//  //These values were pulled from EnvironmentInput.csv from an older unit test
//  //SkinToClothing->GetResistanceBaseline().SetValue(22.836, HeatResistanceUnit::K_Per_W);  //R Clothing
//  SkinToClothing->GetResistanceBaseline().SetValue(.05, HeatResistanceUnit::K_Per_W); //R Clothing
//  ClothingToEnvironment->GetResistanceBaseline().SetValue(4.379, HeatResistanceUnit::K_Per_W); //R Convection
//  ClothingToEnclosure->GetResistanceBaseline().SetValue(0.039, HeatResistanceUnit::K_Per_W); //R Radiation
//
//  // Make a new source attached to Environment circuit to take the place of the Energy circuit
//  SEThermalCircuitNode* Core = cExthermal.GetNode(BGE::ExternalTemperatureNode::ExternalCore);
//  SEThermalCircuitNode* Skin = cExthermal.GetNode(BGE::ExternalTemperatureNode::ExternalSkin);
//  SEThermalCircuitNode* Ground = cExthermal.GetNode(BGE::ExternalTemperatureNode::ExternalGround);
//
//  SEThermalCircuitNode& MetabolicNode = cExthermal.CreateNode("Metabolic");
//  SEThermalCircuitPath& MetabolicPath = cExthermal.CreatePath(*Ground, MetabolicNode, "GroundToMetabolic");
//  SEThermalCircuitPath& MetabolicToCore = cExthermal.CreatePath(*Core, MetabolicNode, "CoreToMetabolic");
//  SEThermalCircuitPath& MetabolicToSkin = cExthermal.CreatePath(*Skin, MetabolicNode, "SkinToMetabolic");
//
//
//  //Hold BMR constant
//  MetabolicPath.GetHeatSourceBaseline().SetValue(testBMR, PowerUnit::kcal_Per_day);
//  MetabolicPath.GetNextHeatSource().SetValue(testBMR, PowerUnit::kcal_Per_day);
//  SEThermalCircuitCalculator fcc{ &m_Logger };
//
//  Core->GetTemperature().SetValue(37.0, TemperatureUnit::C);
//  Skin->GetTemperature().SetValue(33.0, TemperatureUnit::C);
//  SEThermalCircuitNode* Clothing = cExthermal.GetNode(BGE::ExternalTemperatureNode::Clothing);
//  Clothing->GetTemperature().SetValue(30.0, TemperatureUnit::C);
//  SEThermalCircuitNode* Environment = cExthermal.GetNode(BGE::ExternalTemperatureNode::Ambient);
//  Environment->GetTemperature().SetValue(22.0, TemperatureUnit::C);
//  MetabolicNode.GetTemperature().SetValue(22.0, TemperatureUnit::C);
//
//  cExthermal.SetNextAndCurrentFromBaselines();
//  cExthermal.StateChange();
//
//  EXPECT_NO_THROW(fcc.Process(cExthermal, 1.0));
//  EXPECT_NO_THROW(fcc.PostProcess(cExthermal));
//}

TEST_F(TEST_FIXTURE_NAME, Renal)
{
  biogears::Logger m_Logger("RenalCircuitTest.log");
  std::unique_ptr<PhysiologyEngine> bge = CreateBioGearsEngine(&m_Logger);
  BioGears* bg = dynamic_cast<BioGears*>(bge.get());
  bge->LoadState("./states/StandardMale@0s.xml");
  bge->AdvanceModelTime();

  bg->GetLogger()->Info("RenalCircuitTest");
  BioGearsCircuits& circuits = bg->GetCircuits();
  SEFluidCircuit& renal = circuits.GetRenalCircuit();
  SEFluidCircuitCalculator fcc{ &m_Logger };

  EXPECT_NO_THROW(fcc.Process(renal, 1.0));
  EXPECT_NO_THROW(fcc.PostProcess(renal));
}

TEST_F(TEST_FIXTURE_NAME, Respiratory)
{
  biogears::Logger m_Logger("RespiratoryCircuitTest.log");
  std::unique_ptr<PhysiologyEngine> bge = CreateBioGearsEngine(&m_Logger);
  BioGears* bg = dynamic_cast<BioGears*>(bge.get());
  bge->LoadState("./states/StandardMale@0s.xml");
  bge->AdvanceModelTime();

  bg->GetLogger()->Info("RespiratoryCircuitTest");
  BioGearsCircuits& circuits = bg->GetCircuits();
  SEFluidCircuit& respiratory = circuits.GetRespiratoryCircuit();
  SEFluidCircuitCalculator fcc{ &m_Logger };

  EXPECT_NO_THROW(fcc.Process(respiratory, 1.0));
  EXPECT_NO_THROW(fcc.PostProcess(respiratory));
}

TEST_F(TEST_FIXTURE_NAME, Temperature)
{
  biogears::Logger m_Logger("TemperatureCircuitTest.log");
  std::unique_ptr<PhysiologyEngine> bge = CreateBioGearsEngine(&m_Logger);
  BioGears* bg = dynamic_cast<BioGears*>(bge.get());
  bge->LoadState("./states/StandardMale@0s.xml");
  bge->AdvanceModelTime();

  bg->GetLogger()->Info("TemperatureCircuitTest");
  BioGearsCircuits& circuits = bg->GetCircuits();
  SEThermalCircuit& temperature = circuits.GetTemperatureCircuit();
  SEThermalCircuitCalculator fcc{ &m_Logger };

  EXPECT_NO_THROW(fcc.Process(temperature, 1.0));
  EXPECT_NO_THROW(fcc.PostProcess(temperature));
}
