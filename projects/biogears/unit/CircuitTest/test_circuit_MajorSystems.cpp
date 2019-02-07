#include <biogears/engine/BioGearsPhysiologyEngine.h>


#include <limits>
#include <thread>

#include <gtest/gtest.h>

#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include "biogears/engine/Controller/BioGears.h"


#ifdef DISABLE_BIOGEARS_MajorSystems_TEST
#define TEST_FIXTURE_NAME DISABLED_MajorSystems_Fixture
#else
#define TEST_FIXTURE_NAME SEMajorSystems_Fixture
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

  EXPECT_NO_THROW(fcc.Process(cardiovascular, 1.0));
  EXPECT_NO_THROW(fcc.PostProcess(cardiovascular)); 
}

TEST_F(TEST_FIXTURE_NAME, Environment) //Placeholder until I can get a working Environment circuit
{

}

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
