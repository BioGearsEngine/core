#include "biogears_circuit_tester.h"
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include "biogears/chrono/stop_watch.tci.h"

namespace biogears {
void BioGearsCircuitTester::AnesthesiaCircuitTest()
{
  std::unique_ptr<PhysiologyEngine> bge = CreateBioGearsEngine("AnesthesiaCircuitTest.log");
  BioGears* bg = dynamic_cast<BioGears*>(bge.get());
  bge->LoadState("./states/StandardMale@0s.xml");
  bge->AdvanceModelTime();

  bg->GetLogger()->Info("AnesthesiaCircuitTest");
  BioGearsCircuits& circuits = bg->GetCircuits();
  SEFluidCircuit& anesthesia = circuits.GetAnesthesiaMachineCircuit();
  SEFluidCircuitCalculator fcc{ m_Logger };

  double time_s = 0.0;
  DataTrack& outTrk = bg->GetDataTrack();
  outTrk.CreateFile(std::string("TestResults/AnesthesiaCircuitTestOutput.csv").c_str(), file);
  for (int i = 0; i < 10000; ++i) {
    fcc.Process(anesthesia, 1.0);
    fcc.PostProcess(anesthesia);
    time_s += (1.0);
    outTrk.Track(time_s, anesthesia);
    outTrk.StreamTrackToFile(file);
  }
  file.close();
}

void BioGearsCircuitTester::CardiovascularCircuitTest()
{
  std::unique_ptr<PhysiologyEngine> bge = CreateBioGearsEngine("CardiovascularCircuitTest.log");
  BioGears* bg = dynamic_cast<BioGears*>(bge.get());
  bge->LoadState("./states/StandardMale@0s.xml");
  bge->AdvanceModelTime();

  bg->GetLogger()->Info("CardiovascularCircuitTest");
  BioGearsCircuits& circuits = bg->GetCircuits();
  SEFluidCircuit& Cardiovascular = circuits.GetCardiovascularCircuit();
  SEFluidCircuitCalculator fcc{ m_Logger };

  double time_s = 0.0;
  DataTrack& outTrk = bg->GetDataTrack();
  outTrk.CreateFile(std::string("TestResults/CardiovascularCircuitTestOutput.csv").c_str(), file);
  for (int i = 0; i < 10000; ++i) {
    fcc.Process(Cardiovascular, 1.0);
    fcc.PostProcess(Cardiovascular);
    time_s += (1.0);
    outTrk.Track(time_s, Cardiovascular);
    outTrk.StreamTrackToFile(file);
  }
  file.close();
}

void BioGearsCircuitTester::RenalCircuitTest()
{
  std::unique_ptr<PhysiologyEngine> bge = CreateBioGearsEngine("RenalCircuitTest.log");
  BioGears* bg = dynamic_cast<BioGears*>(bge.get());
  bge->LoadState("./states/StandardMale@0s.xml");
  bge->AdvanceModelTime();

  bg->GetLogger()->Info("RenalCircuitTest");
  BioGearsCircuits& circuits = bg->GetCircuits();
  SEFluidCircuit& Renal = circuits.GetRenalCircuit();
  SEFluidCircuitCalculator fcc{ m_Logger };

  double time_s = 0.0;
  DataTrack& outTrk = bg->GetDataTrack();
  outTrk.CreateFile(std::string("TestResults/RenalCircuitTestOutput.csv").c_str(), file);
  for (int i = 0; i < 10000; ++i) {
    fcc.Process(Renal, 1.0);
    fcc.PostProcess(Renal);
    time_s += (1.0);
    outTrk.Track(time_s, Renal);
    outTrk.StreamTrackToFile(file);
  }
  file.close();
}

void BioGearsCircuitTester::RespiratoryCircuitTest()
{
  std::unique_ptr<PhysiologyEngine> bge = CreateBioGearsEngine("RespiratoryCircuitTest.log");
  BioGears* bg = dynamic_cast<BioGears*>(bge.get());
  bge->LoadState("./states/StandardMale@0s.xml");
  bge->AdvanceModelTime();

  bg->GetLogger()->Info("RespiratoryCircuitTest");
  BioGearsCircuits& circuits = bg->GetCircuits();
  SEFluidCircuit& Respiratory = circuits.GetRespiratoryCircuit();
  SEFluidCircuitCalculator fcc{ m_Logger };

  double time_s = 0.0;
  DataTrack& outTrk = bg->GetDataTrack();
  outTrk.CreateFile(std::string("TestResults/RespiratoryCircuitTestOutput.csv").c_str(), file);
  for (int i = 0; i < 10000; ++i) {
    fcc.Process(Respiratory, 1.0);
    fcc.PostProcess(Respiratory);
    time_s += (1.0);
    outTrk.Track(time_s, Respiratory);
    outTrk.StreamTrackToFile(file);
  }
  file.close();
}

void BioGearsCircuitTester::EnvironmentCircuitTest() // Environment Circuit Test doesn't currently run
{
  SEThermalCircuit* bgc = &(this->GetCircuits().GetInternalTemperatureCircuit());
  SEThermalCircuitCalculator tcc(this->BioGears::m_Logger);
  this->GetPatient().Load("./patients/StandardMale.xml");
  this->SetupPatient();
  this->m_Config->EnableRenal(CDM::enumOnOff::Off);
  this->m_Config->EnableTissue(CDM::enumOnOff::Off);
  this->CreateCircuitsAndCompartments();
  Environment& env = (Environment&)this->GetEnvironment();
  env.Initialize();
  env.GetConditions().Load("./environments/Standard.xml");
  env.StateChange();
  double time_s = 0.0;
  m_DataTrack->CreateFile(std::string("TestResults/TemperatureCircuitTestOutput.csv").c_str(), file);
  for (int i = 0; i < 10000; ++i) {
    env.PreProcess();
    tcc.Process(*bgc, 1.0 / 165.0);
    tcc.PostProcess(*bgc);
    time_s += (1.0 / 165.0);
    m_DataTrack->Track(time_s, *bgc);
    m_DataTrack->StreamTrackToFile(file);
  }
  file.close();
}

void BioGearsCircuitTester::TemperatureCircuitTest()
{
  std::unique_ptr<PhysiologyEngine> bge = CreateBioGearsEngine("TemperatureCircuitTest.log");
  BioGears* bg = dynamic_cast<BioGears*>(bge.get());
  bge->LoadState("./states/StandardMale@0s.xml");
  bge->AdvanceModelTime();

  bg->GetLogger()->Info("TemperatureCircuitTest");
  BioGearsCircuits& circuits = bg->GetCircuits();
  SEThermalCircuit& thermal = circuits.GetInternalTemperatureCircuit();
  SEThermalCircuitCalculator tcc{ m_Logger };

  double time_s = 0.0;
  auto& outTrk = bg->GetDataTrack();
  outTrk.CreateFile(std::string("TestResults/TemperatureCircuitTestOutput.csv").c_str(), file);
  for (int i = 0; i < 10000; ++i) {
    tcc.Process(thermal, 1.0);
    tcc.PostProcess(thermal);
    time_s += (1.0);
    outTrk.Track(time_s, thermal);
    outTrk.StreamTrackToFile(file);
  }
  file.close();
}

//void BioGearsCircuitTester::TemperatureCircuitTest()
//{
//  SEThermalCircuit* bgc = &(this->GetCircuits().GetInternalTemperatureCircuit());
//  SEThermalCircuitCalculator tcc(this->BioGears::m_Logger);
//  SEThermalCircuitPath* GroundToCore = bgc->GetPath(BGE::InternalTemperaturePath::GroundToInternalCore);
//  SEThermalCircuitNode* Core = bgc->GetNode(BGE::InternalTemperatureNode::InternalCore);
//  SEThermalCircuitNode* Skin = bgc->GetNode(BGE::InternalTemperatureNode::InternalSkin);
//  SEThermalCircuitNode* Ground = bgc->GetNode(BGE::InternalTemperatureNode::InternalGround);
//  SEThermalCircuitPath& CoreToEnvironment = bgc->CreatePath(*Core, *Ground, "EnvironmentCore");
//  SEThermalCircuitPath& SkinToEnvironment = bgc->CreatePath(*Skin, *Ground, "EnvironmentSkin");
//  bgc->SetNextAndCurrentFromBaselines();
//  bgc->StateChange();
//  double time_s = 0.0;
//  m_DataTrack->CreateFile(std::string("TestResults/TemperatureCircuitTestOutput.csv").c_str(), file);
//  for (int i = 0; i < 10000; ++i) {
//    tcc.Process(*bgc, 1.0 / 165.0);
//    tcc.PostProcess(*bgc);
//    time_s += (1.0 / 165.0);
//    m_DataTrack->Track(time_s, *bgc);
//    m_DataTrack->StreamTrackToFile(file);
//  }
//  file.close();
//}

//inline void BioGearsCircuitTester::AnesthesiaCircuitTest()
//{
// SEEnvironmentalConditions env(this->BioGears::GetSubstances());
// env.Load("./environments/Standard.xml");
// SEGasCompartment* cEnv = this->GetCompartments().GetGasCompartment(BGE::EnvironmentCompartment::Ambient);
// for (SESubstanceFraction* subFrac : env.GetAmbientGases()) {
//   GetSubstances().AddActiveSubstance(subFrac->GetSubstance());
//   cEnv->GetSubstanceQuantity(subFrac->GetSubstance())->GetVolumeFraction().Set(subFrac->GetFractionAmount());
// }
//  GetSubstances().InitializeGasCompartments();
//}

BioGearsCircuitTester::BioGearsCircuitTester(std::string logFileName)
  : BioGears("TestResults/" + logFileName)
//, BioGearsSubstances(*((BioGears*)this))
{
  this->GetPatient().Load("./patients/StandardMale.xml");
  this->SetupPatient();
  this->m_Config->EnableRenal(CDM::enumOnOff::Off);
  this->m_Config->EnableTissue(CDM::enumOnOff::Off);
  this->CreateCircuitsAndCompartments();
}

inline BioGearsCircuitTester::~BioGearsCircuitTester(){

};
} //end namespace biogears
