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
#include <biogears/engine/Controller/BioGearsEngine.h>

#include <memory>
// Project Includes

#include <biogears/filesystem/path.h>
#include <biogears/string/manipulation.h>

#include "io/biogears/BioGears.h"
#include "io/cdm/Circuit.h"
#include "io/cdm/Compartment.h"
#include "io/cdm/Conditions.h"
#include "io/cdm/Patient.h"
#include "io/cdm/PatientAssessments.h"

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/circuit/SECircuit.h>
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/engine/PhysiologyEngineStabilization.h>
#include <biogears/cdm/enums/SEPatientAssessmentEnums.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/patient/actions/SEPatientAssessmentRequest.h>
#include <biogears/cdm/patient/assessments/SEArterialBloodGasAnalysis.h>
#include <biogears/cdm/patient/assessments/SECompleteBloodCount.h>
#include <biogears/cdm/patient/assessments/SEComprehensiveMetabolicPanel.h>
#include <biogears/cdm/patient/assessments/SEProthrombinTime.h>
#include <biogears/cdm/patient/assessments/SEPsychomotorVigilanceTask.h>
#include <biogears/cdm/patient/assessments/SEPulmonaryFunctionTest.h>
#include <biogears/cdm/patient/assessments/SESequentialOrganFailureAssessment.h>
#include <biogears/cdm/patient/assessments/SEUrinalysis.h>
#include <biogears/cdm/scenario/SECondition.h>
#include <biogears/cdm/scenario/SEScenario.h>
#include <biogears/cdm/scenario/SESerializeState.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/utils/Logger.h>
#include <biogears/container/Tree.tci.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
#include <biogears/engine/Controller/BioGearsEnums.h>
#include <biogears/engine/Equipment/ECG.h>
#include <biogears/io/io-manager.h>
#include <io/cdm/Property.h>

#if defined(BIOGEARS_IO_PRESENT) && defined(BIOGEARS_IO_EMBED_STATES)
#include <biogears/io/directories/states.h>
#endif

#include <biogears/version.h>

namespace BGE = mil::tatrc::physiology::biogears;

namespace biogears {
//-------------------------------------------------------------------------------
BioGearsEngine::BioGearsEngine(Logger* logger)
  : BioGears(logger)
  , m_EngineTrack(*this)
{
  m_State = EngineState::NotReady;
  m_EventHandler = nullptr;
  m_DataTrack = &m_EngineTrack.GetDataTrack();
}
//-------------------------------------------------------------------------------
BioGearsEngine::BioGearsEngine(Logger* logger, const std::string& working_dir)
  : BioGears(logger, working_dir)
  , m_EngineTrack(*this)
{
  m_Logger->GetIoManager().lock()->SetBioGearsWorkingDirectory(working_dir);
  m_State = EngineState::NotReady;
  m_EventHandler = nullptr;
  m_DataTrack = &m_EngineTrack.GetDataTrack();
}
//-------------------------------------------------------------------------------
BioGearsEngine::BioGearsEngine(Logger* logger, const char* working_dir)
  : BioGearsEngine(logger, std::string { working_dir })
{
}
//-------------------------------------------------------------------------------
BioGearsEngine::BioGearsEngine(const char* logFileName)
  : BioGearsEngine(std::string { logFileName })
{
}
//-------------------------------------------------------------------------------
BioGearsEngine::BioGearsEngine(const std::string& logFileName)
  : BioGears(logFileName)
  , m_EngineTrack(*this)
{
  m_State = EngineState::NotReady;
  m_EventHandler = nullptr;
  m_DataTrack = &m_EngineTrack.GetDataTrack();
}
//-------------------------------------------------------------------------------
BioGearsEngine::BioGearsEngine(const std::string& logFileName, const std::string& working_dir)
  : BioGears(logFileName, working_dir)
  , m_EngineTrack(*this)
{
  m_Logger->GetIoManager().lock()->SetBioGearsWorkingDirectory(working_dir);
  m_State = EngineState::NotReady;
  m_EventHandler = nullptr;
  m_DataTrack = &m_EngineTrack.GetDataTrack();
}
//-------------------------------------------------------------------------------
BioGearsEngine::BioGearsEngine(const char* logFileName, const char* working_dir)
  : BioGears(logFileName, working_dir)
  , m_EngineTrack(*this)
{
  m_State = EngineState::NotReady;
  m_EventHandler = nullptr;
  m_DataTrack = &m_EngineTrack.GetDataTrack();
}
//-------------------------------------------------------------------------------
BioGearsEngine::~BioGearsEngine()
{
}
//-------------------------------------------------------------------------------
Logger* BioGearsEngine::GetLogger()
{
  return BioGears::GetLogger();
}
//-------------------------------------------------------------------------------
PhysiologyEngineTrack* BioGearsEngine::GetEngineTrack()
{
  return &m_EngineTrack;
}
//-------------------------------------------------------------------------------
//!
//!  \param char* - Coniocal Path to be loaded
//!  \param SEScalarTime*  -  Overrides the internal states Simulation time with the given. Defaults to nullptr which assumes the state files time.
//!
//!  Load sate is biogears_io aware.  If the file path given is embedded it will fall back
//!  to the embeded version if non of the default search locations are found.
bool BioGearsEngine::LoadState(const char* file, const SEScalarTime* simTime)
{
  return LoadState(std::string { file }, simTime);
}
//-------------------------------------------------------------------------------
//!
//!  Override of LoadState
//! \\see   BioGearsEngine::LoadState(const char* file, const SEScalarTime* simTime)
bool BioGearsEngine::LoadState(const std::string& file, const SEScalarTime* simTime)
{

  std::unique_ptr<CDM::ObjectData> obj;

  auto io = m_Logger->GetIoManager().lock();
  auto possible_path = io->FindStateFile(file.c_str());
  if (possible_path.empty()) {
#if defined(BIOGEARS_IO_PRESENT) && defined(BIOGEARS_IO_EMBED_STATES)
    size_t content_size;
    auto content = io::get_embedded_states_file(file.c_str(), content_size);
    obj = Serializer::ReadBuffer((XMLByte*)content, content_size, m_Logger);
#endif
  } else {
    obj = Serializer::ReadFile(possible_path, m_Logger);
  }

  auto state = dynamic_cast<const CDM::BioGearsStateData*>(obj.get());
  if (state != nullptr) {
    return LoadState(*state, simTime);
  }
  m_Logger->Error("File does not contain a valid PhysiologyEngineState");
  return false;
}
//-------------------------------------------------------------------------------
//!
//!  \param char const* buffer -- String literal ASCII encoding of a biogears EngineState file
//!  \param size_t - Ammount of data in the buffer
//!
//!  \note This function allows integrators to perform IO for Biogears and avoid
//!        assumptons of the underlying filesystem.
bool BioGearsEngine::LoadState(char const* buffer, size_t size)
{

  std::unique_ptr<CDM::ObjectData> obj = Serializer::ReadBuffer((XMLByte const*)buffer, size, GetLogger());
  auto state = dynamic_cast<const CDM::BioGearsStateData*>(obj.get());
  if (state != nullptr) {
    return LoadState(*state, nullptr);
  }
  m_Logger->Error("File does not contain a valid PhysiologyEngineState");
  return false;
}
//-------------------------------------------------------------------------------
//!
//!  Override of LoadState
//! \\see   BioGearsEngine::LoadState(const char* file, const SEScalarTime* simTime)
bool BioGearsEngine::LoadState(const CDM::PhysiologyEngineStateData& state, const SEScalarTime* simTime)
{
  auto requests = GetEngineTrack()->GetDataRequestManager().GetDataRequests();
  auto resultsFile = GetEngineTrack()->GetDataRequestManager().GetResultsFilename();
  std::vector<std::unique_ptr<CDM::DataRequestData>> dataVector;
  for (auto& dr : requests) {
    dataVector.emplace_back(dr->Unload());
  }
  BioGears::SetUp();
  m_EngineTrack = PhysiologyEngineTrack(*this);
  m_DataTrack = &m_EngineTrack.GetDataTrack();
  for (auto& data : dataVector) {
    m_EngineTrack.GetDataRequestManager().CreateFromBind(*data, *m_Substances);
  }
  m_EngineTrack.GetDataRequestManager().SetResultsFilename(resultsFile);
  m_ss.str("");
  m_ss.clear();

  m_State = EngineState::NotReady;

  const CDM::BioGearsStateData* bgState = dynamic_cast<const CDM::BioGearsStateData*>(&state);
  if (bgState == nullptr) {
    m_Logger->Error("State data is not a BioGearsStateData object");
    return false;
  }

  // if (state.DataRequests().present()) {
  //   m_EngineTrack.GetDataRequestManager().Clear();
  //   m_EngineTrack.GetDataRequestManager().Load(state.DataRequests().get(), *m_Substances);
  //   m_EngineTrack.ForceConnection(); // I don't want to rest the file because I would loose all my data
  // }

  if (simTime != nullptr) {
    m_CurrentTime->Set(*simTime);
    m_SimulationTime->Set(*simTime);
  } else {
    if (state.SimulationTime().present()) {
      io::Property::UnMarshall(state.SimulationTime(), *m_CurrentTime);
      io::Property::UnMarshall(state.SimulationTime(), *m_SimulationTime);
    } else {
      m_CurrentTime->SetValue(0, TimeUnit::s);
      m_SimulationTime->SetValue(0, TimeUnit::s);
    }
  }

  io::BioGears::UnMarshall(bgState->AirwayMode(), m_AirwayMode);
  io::Property::UnMarshall(bgState->Intubation(), m_Intubation);

  /// Patient //
  if (!bgState->Patient().present()) {
    m_ss << "BioGearsState must have a patient" << std::endl;
  } else {
    try {
      io::Patient::UnMarshall(bgState->Patient().get(), *m_Patient);
    } catch (CommonDataModelException ex) {
      m_ss << "Error loading patient data: " << ex.what() << std::endl;
    }
  }
  // Conditions //
  m_Conditions->Clear();
  for (const CDM::ConditionData& cData : bgState->Condition()) {
    if (!m_Conditions->ProcessCondition(cData, *this)) {
      m_ss << "Unable to load condition" << std::endl;
    }
  }
  // Actions //
  m_Actions->Clear();
  for (const CDM::ActionData& cData : bgState->ActiveAction()) {
    if (!m_Actions->ProcessAction(cData, *this)) {
      m_ss << "Unable to load action" << std::endl;
    }
  }
  // Substances //
  for (const CDM::SubstanceData& subData : bgState->ActiveSubstance()) {
    SESubstance* sub = m_Substances->GetSubstance(subData.Name());
    if (sub == nullptr) {
      sub = new SESubstance(GetLogger());
      m_Substances->AddSubstance(*sub);
    }
    if (!sub->Load(subData)) {
      m_ss << "Unable to load substance" << subData.Name() << std::endl;
    }
    m_Substances->AddActiveSubstance(*sub);
  }
  // Compounds //
  for (const CDM::SubstanceCompoundData& cmpdData : bgState->ActiveSubstanceCompound()) {
    SESubstanceCompound* cmpd = m_Substances->GetCompound(cmpdData.Name());
    if (cmpd == nullptr) {
      cmpd = new SESubstanceCompound(GetLogger());
      m_Substances->AddCompound(*cmpd);
    }
    if (!cmpd->Load(cmpdData, *m_Substances)) {
      m_ss << "Unable to load compound" << cmpdData.Name() << std::endl;
    }
  }
  // Circuit Manager //
  if (!bgState->CircuitManager().present()) {
    m_ss << "BioGearsState must have a circuit manager" << std::endl;
  } else {
    const CDM::CircuitManagerData* cmptMgrData = dynamic_cast<const CDM::CircuitManagerData*>(&bgState->CircuitManager().get());
    if (cmptMgrData == nullptr) {
      m_ss << "BioGearsState must have a BioGears circuit manager" << std::endl;
    } else if (!m_Circuits->Load(*cmptMgrData)) {
      m_ss << "Error loading circuit manager data" << std::endl;
    }
  }
  // Compartment Manager //
  if (!bgState->CompartmentManager().present()) {
    m_ss << "BioGearsState must have a compartment manager" << std::endl;
  } else {
    const CDM::CompartmentManagerData* cmptMgrData = dynamic_cast<const CDM::CompartmentManagerData*>(&bgState->CompartmentManager().get());
    if (cmptMgrData == nullptr) {
      m_ss << "BioGearsState must have a BioGears compartment manager" << std::endl;
    } else if (!m_Compartments->Load(*cmptMgrData, m_Circuits.get())) {
      m_ss << "Error loading compartment manager data" << std::endl;
    }
  }
  // Configuration //
  if (!bgState->Configuration().present()) {
    m_ss << "BioGearsState must have a configuration" << std::endl;
  } else {
    const CDM::BioGearsConfigurationData* confData = dynamic_cast<const CDM::BioGearsConfigurationData*>(&bgState->Configuration().get());
    if (confData == nullptr) {
      m_ss << "BioGearsState must have a BioGears configuration" << std::endl;
    } else if (!m_Config->Load(*confData)) {
      m_ss << "Error loading configuration data" << std::endl;
    }
  }
  // Now, Let's see if there is anything to merge into our base configuration
  // At this point I don't think we should be doing this... but maybe you want to...
  // m_Logger->Info("Merging OnDisk Configuration");
  // BioGearsConfiguration cFile(*m_Substances);
  // cFile.Load("BioGearsConfiguration.xml");
  // m_Config->Merge(cFile);

  /////////////
  // Systems //
  /////////////
  // Physiology
  const CDM::BioGearsBloodChemistrySystemData* bcData = nullptr;
  const CDM::BioGearsCardiovascularSystemData* cvData = nullptr;
  const CDM::BioGearsDrugSystemData* dData = nullptr;
  const CDM::BioGearsEndocrineSystemData* ndoData = nullptr;
  const CDM::BioGearsEnergySystemData* nrgData = nullptr;
  const CDM::BioGearsGastrointestinalSystemData* gasData = nullptr;
  const CDM::BioGearsHepaticSystemData* hepData = nullptr;
  const CDM::BioGearsNervousSystemData* nrvData = nullptr;
  const CDM::BioGearsRenalSystemData* rnlData = nullptr;
  const CDM::BioGearsRespiratorySystemData* rspData = nullptr;
  const CDM::BioGearsTissueSystemData* tsuData = nullptr;
  // Environment
  const CDM::BioGearsEnvironmentData* envData = nullptr;
  // Equipment
  const CDM::BioGearsAnesthesiaMachineData* amData = nullptr;
  const CDM::BioGearsElectroCardioGramData* ecgData = nullptr;
  const CDM::BioGearsInhalerData* nhlData = nullptr;
  for (const CDM::SystemData& sysData : bgState->System()) {
    if (bcData == nullptr) {
      bcData = dynamic_cast<const CDM::BioGearsBloodChemistrySystemData*>(&sysData);
      if (bcData != nullptr && !m_BloodChemistrySystem->Load(*bcData)) {
        m_ss << "Error loading Blood Chemistry data" << std::endl;
      }
    }
    if (cvData == nullptr) {
      cvData = dynamic_cast<const CDM::BioGearsCardiovascularSystemData*>(&sysData);
      if (cvData != nullptr && !m_CardiovascularSystem->Load(*cvData)) {
        m_ss << "Error loading Cardiovascular data" << std::endl;
      }
    }
    if (dData == nullptr) {
      dData = dynamic_cast<const CDM::BioGearsDrugSystemData*>(&sysData);
      if (dData != nullptr && !m_DrugSystem->Load(*dData)) {
        m_ss << "Error loading Drug data" << std::endl;
      }
    }
    if (ndoData == nullptr) {
      ndoData = dynamic_cast<const CDM::BioGearsEndocrineSystemData*>(&sysData);
      if (ndoData != nullptr && !m_EndocrineSystem->Load(*ndoData)) {
        m_ss << "Error loading Endocrine data" << std::endl;
      }
    }
    if (nrgData == nullptr) {
      nrgData = dynamic_cast<const CDM::BioGearsEnergySystemData*>(&sysData);
      if (nrgData != nullptr && !m_EnergySystem->Load(*nrgData)) {
        m_ss << "Error loading Energy data" << std::endl;
      }
    }
    if (gasData == nullptr) {
      gasData = dynamic_cast<const CDM::BioGearsGastrointestinalSystemData*>(&sysData);
      if (gasData != nullptr && !m_GastrointestinalSystem->Load(*gasData)) {
        m_ss << "Error loading Gastrointestinal data" << std::endl;
      }
    }
    if (hepData == nullptr) {
      hepData = dynamic_cast<const CDM::BioGearsHepaticSystemData*>(&sysData);
      if (hepData != nullptr && !m_HepaticSystem->Load(*hepData)) {
        m_ss << "Error loading Hepatic data" << std::endl;
      }
    }
    if (nrvData == nullptr) {
      nrvData = dynamic_cast<const CDM::BioGearsNervousSystemData*>(&sysData);
      if (nrvData != nullptr && !m_NervousSystem->Load(*nrvData)) {
        m_ss << "Error loading Nervous data" << std::endl;
      }
    }
    if (rnlData == nullptr) {
      rnlData = dynamic_cast<const CDM::BioGearsRenalSystemData*>(&sysData);
      if (rnlData != nullptr && !m_RenalSystem->Load(*rnlData)) {
        m_ss << "Error loading Renal data" << std::endl;
      }
    }
    if (rspData == nullptr) {
      rspData = dynamic_cast<const CDM::BioGearsRespiratorySystemData*>(&sysData);
      if (rspData != nullptr && !m_RespiratorySystem->Load(*rspData)) {
        m_ss << "Error loading Respiratory data" << std::endl;
      }
    }
    if (tsuData == nullptr) {
      tsuData = dynamic_cast<const CDM::BioGearsTissueSystemData*>(&sysData);
      if (tsuData != nullptr && !m_TissueSystem->Load(*tsuData)) {
        m_ss << "Error loading Tissue data" << std::endl;
      }
    }
    if (envData == nullptr) {
      envData = dynamic_cast<const CDM::BioGearsEnvironmentData*>(&sysData);
      if (envData != nullptr && !m_Environment->Load(*envData)) {
        m_ss << "Error loading Environment data" << std::endl;
      }
    }
    if (amData == nullptr) {
      amData = dynamic_cast<const CDM::BioGearsAnesthesiaMachineData*>(&sysData);
      if (amData != nullptr && !m_AnesthesiaMachine->Load(*amData)) {
        m_ss << "Error loading Anesthesia Machine data" << std::endl;
      }
    }
    if (ecgData == nullptr) {
      ecgData = dynamic_cast<const CDM::BioGearsElectroCardioGramData*>(&sysData);
      if (ecgData != nullptr && !m_ECG->Load(*ecgData)) {
        m_ss << "Error loading ECG data" << std::endl;
      }
    }
    if (nhlData == nullptr) {
      nhlData = dynamic_cast<const CDM::BioGearsInhalerData*>(&sysData);
      if (nhlData != nullptr && !m_Inhaler->Load(*nhlData)) {
        m_ss << "Error loading Inhaler data" << std::endl;
      }
    }
  }
  // Make sure we had all systems in the state
  if (bcData == nullptr) {
    m_ss << "Missing Blood Chemistry State" << std::endl;
  }
  if (cvData == nullptr) {
    m_ss << "Missing Cardiovascular State" << std::endl;
  }
  if (dData == nullptr) {
    m_ss << "Missing Drug State" << std::endl;
  }
  if (ndoData == nullptr) {
    m_ss << "Missing Endocrine State" << std::endl;
  }
  if (nrgData == nullptr) {
    m_ss << "Missing Energy State" << std::endl;
  }
  if (gasData == nullptr) {
    m_ss << "Missing Gastrointestinal State" << std::endl;
  }
  if (hepData == nullptr) {
    m_ss << "Missing Hepatic State" << std::endl;
  }
  if (nrvData == nullptr) {
    m_ss << "Missing Nervous State" << std::endl;
  }
  if (rnlData == nullptr) {
    m_ss << "Missing Renal State" << std::endl;
  }
  if (rspData == nullptr) {
    m_ss << "Missing Respiratory State" << std::endl;
  }
  if (tsuData == nullptr) {
    m_ss << "Missing Tissue State" << std::endl;
  }
  if (envData == nullptr) {
    m_ss << "Missing Environment State" << std::endl;
  }
  if (amData == nullptr) {
    m_ss << "Missing Anesthesia Machine State" << std::endl;
  }
  if (ecgData == nullptr) {
    m_ss << "Missing ECG State" << std::endl;
  }
  if (nhlData == nullptr) {
    m_ss << "Missing Inhaler State" << std::endl;
  }

  if (!m_ss.str().empty()) {
    m_Logger->Error(m_ss.str(), "BioGearsEngine");
    m_ss.str("");
    m_ss.clear();
    return false;
  }
  // Make sure links are correct with our modes
  m_Compartments->UpdateAirwayGraph();
  m_Compartments->GetActiveRespiratoryGraph();
  m_Compartments->GetActiveAerosolGraph();

  // It helps to unload what you just loaded and to a compare if you have issues
  // SaveStateToFile("WhatIJustLoaded.xml");

  // Good to go, save it off and carry on!
  m_State = EngineState::Active;
  if (state.Patient().present()) {
    m_Logger->Info("Biogears has sucesfully loaded " + state.Patient()->Name() + ".");
  } else {
    m_Logger->Info("Biogears has sucesfully loaded the given statefile");
  }

  return true; // return CheckDataRequirements/IsValid() or something
}
//-------------------------------------------------------------------------------
std::unique_ptr<CDM::PhysiologyEngineStateData> BioGearsEngine::GetStateData()
{

  std::unique_ptr<CDM::PhysiologyEngineStateData> state(new CDM::BioGearsStateData());

  state->contentVersion(branded_version_string());

  state->SimulationTime(std::make_unique<CDM::ScalarTimeData>());
  io::Property::Marshall(*m_SimulationTime, state->SimulationTime());

  if (m_EngineTrack.GetDataRequestManager().HasDataRequests()) {
    state->DataRequests(std::unique_ptr<CDM::DataRequestManagerData>(m_EngineTrack.GetDataRequestManager().Unload()));
  }

  ((CDM::BioGearsStateData*)state.get())->AirwayMode((std::make_unique<CDM::enumBioGearsAirwayMode>()));
  io::BioGears::Marshall(m_AirwayMode, ((CDM::BioGearsStateData*)state.get())->AirwayMode());

  ((CDM::BioGearsStateData*)state.get())->Intubation((std::make_unique<CDM::enumOnOff>()));
  io::Property::Marshall(m_Intubation, ((CDM::BioGearsStateData*)state.get())->Intubation());

  // Patient
  state->Patient(std::make_unique<CDM::PatientData>());
  io::Patient::Marshall(*m_Patient, ((CDM::BioGearsStateData*)state.get())->Patient());
  // Conditions
  auto conditions = io::Conditions::condition_data_factory(*m_Conditions);

  for (auto& cData : conditions) {
    state->Condition().push_back(std::move(cData));
  }
  // Actions
  std::vector<CDM::ActionData*> activeActions;
  m_Actions->Unload(activeActions);
  for (CDM::ActionData* aData : activeActions) {
    state->ActiveAction().push_back(std::unique_ptr<CDM::ActionData>(aData));
  }
  // Active Substances/Compounds
  for (SESubstance* s : m_Substances->GetActiveSubstances()) {
    state->ActiveSubstance().push_back(std::unique_ptr<CDM::SubstanceData>(s->Unload()));
  }
  for (SESubstanceCompound* c : m_Substances->GetActiveCompounds()) {
    state->ActiveSubstanceCompound().push_back(std::unique_ptr<CDM::SubstanceCompoundData>(c->Unload()));
  }
  // Systems
  state->System().push_back(std::unique_ptr<CDM::BioGearsBloodChemistrySystemData>(m_BloodChemistrySystem->Unload()));
  state->System().push_back(std::unique_ptr<CDM::BioGearsCardiovascularSystemData>(m_CardiovascularSystem->Unload()));
  state->System().push_back(std::unique_ptr<CDM::BioGearsDrugSystemData>(m_DrugSystem->Unload()));
  state->System().push_back(std::unique_ptr<CDM::BioGearsEndocrineSystemData>(m_EndocrineSystem->Unload()));
  state->System().push_back(std::unique_ptr<CDM::BioGearsEnergySystemData>(m_EnergySystem->Unload()));
  state->System().push_back(std::unique_ptr<CDM::BioGearsGastrointestinalSystemData>(m_GastrointestinalSystem->Unload()));
  state->System().push_back(std::unique_ptr<CDM::BioGearsHepaticSystemData>(m_HepaticSystem->Unload()));
  state->System().push_back(std::unique_ptr<CDM::BioGearsNervousSystemData>(m_NervousSystem->Unload()));
  state->System().push_back(std::unique_ptr<CDM::BioGearsRenalSystemData>(m_RenalSystem->Unload()));
  state->System().push_back(std::unique_ptr<CDM::BioGearsRespiratorySystemData>(m_RespiratorySystem->Unload()));
  state->System().push_back(std::unique_ptr<CDM::BioGearsTissueSystemData>(m_TissueSystem->Unload()));
  state->System().push_back(std::unique_ptr<CDM::BioGearsEnvironmentData>(m_Environment->Unload()));
  state->System().push_back(std::unique_ptr<CDM::BioGearsAnesthesiaMachineData>(m_AnesthesiaMachine->Unload()));
  state->System().push_back(std::unique_ptr<CDM::BioGearsElectroCardioGramData>(m_ECG->Unload()));
  state->System().push_back(std::unique_ptr<CDM::BioGearsInhalerData>(m_Inhaler->Unload()));
  // Compartments
  // state->CompartmentManager(std::unique_ptr<CDM::CompartmentManagerData>(m_Compartments->Unload()));
  auto compartments = std::make_unique<CDM::CompartmentManagerData>();
  io::Compartment::Marshall(*static_cast<SECompartmentManager*>(m_Compartments.get()), *compartments);
  state->CompartmentManager(std::move(compartments));
  // Configuration
  state->Configuration(std::unique_ptr<CDM::PhysiologyEngineConfigurationData>(m_Config->Unload()));
  // Circuits

  // state->CircuitManager(std::unique_ptr<CDM::CircuitManagerData>(m_Circuits->Unload()));
  auto circuits = std::make_unique<CDM::CircuitManagerData>();
  io::Circuit::Marshall(*static_cast<SECircuitManager*>(m_Circuits.get()), *circuits);
  state->CircuitManager(std::move(circuits));

  return state;
}
//-------------------------------------------------------------------------------
void BioGearsEngine::SaveStateToFile(const char* file)
{
  SaveStateToFile(std::string { file });
}

//-------------------------------------------------------------------------------
void BioGearsEngine::SaveStateToFile(const std::string& file)
{
  auto state = GetStateData();

  if (!file.empty()) {
    filesystem::path qualified_path = m_Logger->GetIoManager().lock()->ResolveStateFileLocation(file);
    filesystem::create_directories(qualified_path.parent_path());
    // Write out the engine state
    std::ofstream stream(qualified_path);
    // Write out the xml file
    xml_schema::namespace_infomap map;
    map[""].name = "uri:/mil/tatrc/physiology/datamodel";
    try {
      BioGearsState(stream, dynamic_cast<CDM::BioGearsStateData&>(*state), map);
    } catch (std::exception& ex) {
      m_Logger->Fatal(ex.what());
    }
    stream.close();
  }
}
//-------------------------------------------------------------------------------
bool BioGearsEngine::InitializeEngine(const char* patientFile)
{
  return InitializeEngine(std::string { patientFile }, nullptr, nullptr);
}
//-------------------------------------------------------------------------------
bool BioGearsEngine::InitializeEngine(const char* patientFile, const std::vector<const SECondition*>* conditions)
{
  return InitializeEngine(std::string { patientFile }, conditions, nullptr);
}
//-------------------------------------------------------------------------------
bool BioGearsEngine::InitializeEngine(const char* patientFile, const std::vector<const SECondition*>* conditions, const PhysiologyEngineConfiguration* config)
{
  return InitializeEngine(std::string { patientFile }, conditions, config);
}
//-------------------------------------------------------------------------------
bool BioGearsEngine::InitializeEngine(const std::string& patientFile, const std::vector<const SECondition*>* conditions, const PhysiologyEngineConfiguration* config)
{

  auto io = m_Logger->GetIoManager().lock();
  if (m_Patient == nullptr) {
  }
  if (!m_Patient->Load(patientFile)) {
    return false;
  }
  return InitializeEngine(conditions, config);
}
//-------------------------------------------------------------------------------
bool BioGearsEngine::InitializeEngine(const SEPatient& patient, const std::vector<const SECondition*>* conditions, const PhysiologyEngineConfiguration* config)
{
  CDM_PATIENT_COPY(Patient, patient, *m_Patient);
  // We need logic here that makes sure we have what we need
  // and notify we are ignoring anything provided we won't use
  return InitializeEngine(conditions, config);
}
//-------------------------------------------------------------------------------
bool BioGearsEngine::InitializeEngine(const std::vector<const SECondition*>* conditions, const PhysiologyEngineConfiguration* config)
{
  m_EngineTrack.ResetFile();
  m_State = EngineState::Initialization;
  if (!BioGears::Initialize(config)) {
    return false;
  }

  // We don't capture events during initialization
  m_Patient->ForwardEvents(nullptr);
  m_AnesthesiaMachine->ForwardEvents(nullptr);

  // Stabilize the engine to a resting state (with a standard meal and environment)
  if (!m_Config->HasStabilizationCriteria()) {
    m_Logger->Error("BioGears needs stabilization criteria, none provided in configuration file");
    return false;
  }

  m_State = EngineState::InitialStabilization;
  if (!m_Config->GetStabilizationCriteria()->StabilizeRestingState(*this)) {
    return false;
  }
  // We need to process conditions here, so systems can prepare for them in their AtSteadyState method
  if (conditions != nullptr && !conditions->empty()) {
    for (const SECondition* c : *conditions) {
      m_ss << "[Condition] " << *c;
      m_Logger->Info(m_ss.str(), "BioGearsEngine");
      if (!m_Conditions->ProcessCondition(*c, *this)) {
        return false;
      }
    }
  }
  AtSteadyState(EngineState::AtInitialStableState);

  m_State = EngineState::SecondaryStabilization;
  // Apply conditions and anything else to the physiology
  // now that it's steady with provided patient, environment, and feedback
  if (conditions != nullptr && !conditions->empty()) { // Now restabilize the patient with any conditions that were applied
    // Push conditions into condition manager
    if (!m_Config->GetStabilizationCriteria()->StabilizeConditions(*this, *conditions)) {
      return false;
    }
  } else {
    if (!m_Config->GetStabilizationCriteria()->StabilizeFeedbackState(*this)) {
      return false;
    }
  }
  AtSteadyState(EngineState::AtSecondaryStableState);
  // Hook up the handlers (Note events will still be in the log)
  m_Patient->ForwardEvents(m_EventHandler);
  m_AnesthesiaMachine->ForwardEvents(m_EventHandler);
  m_Logger->Info("Finalizing homeostasis");

  //! Run this again to clear out any bumps from systems resetting baselines in the last AtSteadyState call
  //! I would rather run Feedback stablization again, but...
  //! This does not work for a few patients, they will not stay stable (???)
  AdvanceModelTime(30, TimeUnit::s);

  m_Logger->Info("Engine is now Active");
  // Don't allow any changes to Quantity/Potential/Flux values directly
  // Use Quantity/Potential/Flux Sources
  m_Circuits->SetReadOnly(true);

  m_State = EngineState::Active;
  if (!IsTrackingStabilization()) {
    m_SimulationTime->SetValue(0, TimeUnit::s);
    m_timeSinceLastDataTrack = 0;
    m_timeStep_remainder = 0;
  }
  return true;
}
//-------------------------------------------------------------------------------
double BioGearsEngine::GetTimeStep(const TimeUnit& unit)
{
  return m_Config->GetTimeStep(unit);
}
//-------------------------------------------------------------------------------
double BioGearsEngine::GetSimulationTime(const TimeUnit& unit) const
{
  return m_SimulationTime->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool BioGearsEngine::AdvanceModelTime(bool appendDataTrack)
{
  // TODO: I am starting to think this should be a protected function
  if (!IsReady()) {
    return false;
  }
  if (m_Patient->IsEventActive(SEPatientEventType::IrreversibleState)) {
    return false;
  }

  PreProcess();
  Process();
  PostProcess();

  m_Patient->UpdateEvents(m_Config->GetTimeStep());
  m_CurrentTime->Increment(m_Config->GetTimeStep());
  m_SimulationTime->Increment(m_Config->GetTimeStep());

  const auto sample_interval_s = 1.0 / GetEngineTrack()->GetDataRequestManager().GetSamplesPerSecond();
  m_timeSinceLastDataTrack += m_Config->GetTimeStep(TimeUnit::s);
  if (m_timeSinceLastDataTrack >= sample_interval_s) {
    m_timeSinceLastDataTrack -= sample_interval_s;
    if ((m_isAutoTracking && EngineState::Active == m_State) || (m_State < EngineState::Active && m_areTrackingStabilization)) {
      auto time = GetSimulationTime(TimeUnit::s);
      GetEngineTrack()->TrackData(time, appendDataTrack);
    }
  }

  return true;
}
//-------------------------------------------------------------------------------
bool BioGearsEngine::AdvanceModelTime(double time, const TimeUnit& unit, bool appendDataTrack)
{
  double time_s = Convert(time, unit, TimeUnit::s);
  double remains = time_s / m_Config->GetTimeStep(TimeUnit::s);
  remains -= static_cast<int>(remains);
  m_timeStep_remainder += remains;

  int count = static_cast<int>(time_s / m_Config->GetTimeStep(TimeUnit::s));
  if (m_timeStep_remainder >= 1.0) {
    ++count;
    m_timeStep_remainder -= 1.0;
  }

  for (int i = 0; i < count; i++) {
    if (!AdvanceModelTime(appendDataTrack)) {
      return false;
    }
  }
  return true;
}
//-------------------------------------------------------------------------------
bool BioGearsEngine::ProcessAction(const SEAction& action)
{
  if (!IsReady()) {
    return false;
  }
  m_ss << "[Action] " << *m_SimulationTime << ", " << action;
  m_Logger->Info(m_ss.str(), "BioGearsEngine");
  m_ss.str("");
  const SESerializeState* serialize = dynamic_cast<const SESerializeState*>(&action);
  if (serialize != nullptr) {
    if (serialize->GetType() == SESerializationType::Save) {
      if (serialize->HasFilename()) {
        if (filesystem::is_directory(serialize->GetFilename()) || (filesystem::is_directory("states/" + serialize->GetFilename()))) {
          SaveStateToFile(asprintf("%s%s@%.0fs.xml", serialize->GetFilename().c_str(), m_Patient->GetName().c_str(), GetSimulationTime(TimeUnit::s)));
        } else {
          SaveStateToFile(serialize->GetFilename());
        }
      } else {
        SaveStateToFile(asprintf("%s@%.0fs.xml", m_Patient->GetName().c_str(), GetSimulationTime(TimeUnit::s)));
      }
    } else {
      return LoadState(serialize->GetFilename());
    }
    return true;
  }

  const SEPatientAssessmentRequest* assessment = dynamic_cast<const SEPatientAssessmentRequest*>(&action);
  auto io = m_Logger->GetIoManager().lock();
  if (assessment != nullptr) {
    m_ss.str("");
    std::string results_filepath = GetEngineTrack()->GetDataRequestManager().GetResultsFilename();
    results_filepath = results_filepath.substr(0, results_filepath.find_last_of("."));

    switch (assessment->GetType()) {
    case SEPatientAssessmentType::ArterialBloodGasAnalysis: {
      SEArterialBloodGasAnalysis abga;
      GetPatientAssessment(abga);
      if (results_filepath.empty()) {
        results_filepath = "ArterialBloodGasAnalysis";
      }
      std::ofstream stream(io->ResolveResultsFileLocation(asprintf("%sABGA@%fs.xml", results_filepath.c_str(), GetSimulationTime(TimeUnit::s))));
      // Write out the xml file
      xml_schema::namespace_infomap map;
      map[""].name = "uri:/mil/tatrc/phsyiology/datamodel";

      auto abgaData = std::unique_ptr<CDM::ArterialBloodGasAnalysisData>();
      io::PatientAssessments::Marshall(abga, *abgaData);

      abgaData->contentVersion(branded_version_string());
      ArterialBloodGasAnalysis(stream, *abgaData, map);
      stream.close();
      break;
    }

    case SEPatientAssessmentType::PulmonaryFunctionTest: {
      SEPulmonaryFunctionTest pft;
      GetPatientAssessment(pft);
      if (results_filepath.empty()) {
        results_filepath = "PulmonaryFunctionTest";
      }
      std::ofstream stream(io->ResolveResultsFileLocation(asprintf("%sPFT@%.0fs.xml", results_filepath.c_str(), GetSimulationTime(TimeUnit::s))));
      // Write out the xml file
      xml_schema::namespace_infomap map;
      map[""].name = "uri:/mil/tatrc/phsyiology/datamodel";
      map[""].schema = "BioGears.xsd";

      auto pftData = std::unique_ptr<CDM::PulmonaryFunctionTestData>();
      io::PatientAssessments::Marshall(pft, *pftData);
      pftData->contentVersion(branded_version_string());
      PulmonaryFunctionTest(stream, *pftData, map);
      stream.close();
      break;
    }

    case SEPatientAssessmentType::ProthrombinTime: {
      SEProthrombinTime ptt;
      GetPatientAssessment(ptt);
      if (results_filepath.empty()) {
        results_filepath = "ProthrombinTime";
      }
      std::ofstream stream(io->ResolveResultsFileLocation(asprintf("%sPTT@%.0fs.xml", results_filepath.c_str(), GetSimulationTime(TimeUnit::s))));
      // Write out the xml file
      xml_schema::namespace_infomap map;
      map[""].name = "uri:/mil/tatrc/phsyiology/datamodel";
      map[""].schema = "BioGears.xsd";

      auto pttData = std::unique_ptr<CDM::ProthrombinTimeData>();
      io::PatientAssessments::Marshall(ptt, *pttData);
      pttData->contentVersion(branded_version_string());
      ProthrombinTime(stream, *pttData, map);
      stream.close();
      break;
    }

    case SEPatientAssessmentType::PsychomotorVigilanceTask: {
      SEPsychomotorVigilanceTask pvt;
      GetPatientAssessment(pvt);
      if (results_filepath.empty()) {
        results_filepath = "PsychomotorVigilanceTask";
      }
      std::ofstream stream(io->ResolveResultsFileLocation(asprintf("%sPVT@%.0fs.xml", results_filepath.c_str(), GetSimulationTime(TimeUnit::s))));
      // Write out the xml file
      xml_schema::namespace_infomap map;
      map[""].name = "uri:/mil/tatrc/phsyiology/datamodel";
      map[""].schema = "BioGears.xsd";
      auto pvtData = std::unique_ptr<CDM::PsychomotorVigilanceTaskData>();
      io::PatientAssessments::Marshall(pvt, *pvtData);

      pvtData->contentVersion(branded_version_string());
      PsychomotorVigilanceTask(stream, *pvtData, map);
      stream.close();
      break;
    }

    case SEPatientAssessmentType::Urinalysis: {
      SEUrinalysis upan;
      GetPatientAssessment(upan);
      if (results_filepath.empty()) {
        results_filepath = "Urinalysis";
      }
      std::ofstream stream(io->ResolveResultsFileLocation(asprintf("%sUrinalysis@%.0fs.xml", results_filepath.c_str(), GetSimulationTime(TimeUnit::s))));
      // Write out the xml file
      xml_schema::namespace_infomap map;
      map[""].name = "uri:/mil/tatrc/phsyiology/datamodel";
      auto upanData = std::unique_ptr<CDM::UrinalysisData>();
      io::PatientAssessments::Marshall(upan, *upanData);
      upanData->contentVersion(branded_version_string());
      Urinalysis(stream, *upanData, map);
      stream.close();
      break;
    }

    case SEPatientAssessmentType::CompleteBloodCount: {
      SECompleteBloodCount cbc;
      GetPatientAssessment(cbc);
      if (results_filepath.empty()) {
        results_filepath = "CompleteBloodCount";
      }
      std::ofstream stream(io->ResolveResultsFileLocation(asprintf("%sCBC@%.0fs.xml", results_filepath.c_str(), GetSimulationTime(TimeUnit::s))));
      // Write out the xml file
      xml_schema::namespace_infomap map;
      map[""].name = "uri:/mil/tatrc/phsyiology/datamodel";
      auto cbcData = std::unique_ptr<CDM::CompleteBloodCountData>();
      io::PatientAssessments::Marshall(cbc, *cbcData);
      cbcData->contentVersion(branded_version_string());
      CompleteBloodCount(stream, *cbcData, map);
      stream.close();
      break;
    }

    case SEPatientAssessmentType::ComprehensiveMetabolicPanel: {
      SEComprehensiveMetabolicPanel cmp;
      GetPatientAssessment(cmp);
      if (results_filepath.empty()) {
        results_filepath = "ComprehensiveMetabolicPanel";
      }
      std::ofstream stream(io->ResolveResultsFileLocation(asprintf("%sCMP@%.0fs.xml", results_filepath.c_str(), GetSimulationTime(TimeUnit::s))));
      // Write out the xml file
      xml_schema::namespace_infomap map;
      map[""].name = "uri:/mil/tatrc/phsyiology/datamodel";

      auto cmpData = std::unique_ptr<CDM::ComprehensiveMetabolicPanelData>();
      io::PatientAssessments::Marshall(cmp, *cmpData);

      cmpData->contentVersion(branded_version_string());
      ComprehensiveMetabolicPanel(stream, *cmpData, map);
      stream.close();
      break;
    }
    case SEPatientAssessmentType::SequentialOrganFailureAssessment: {
      SESequentialOrganFailureAssessment sofa;
      GetPatientAssessment(sofa);
      if (results_filepath.empty()) {
        results_filepath = "SequentialOrganFailureAssessment";
      }
      std::ofstream stream(io->ResolveResultsFileLocation(asprintf("%s_SOF@%.0fs.xml", results_filepath.c_str(), GetSimulationTime(TimeUnit::s))));
      // Write out the xml file
      xml_schema::namespace_infomap map;
      map[""].name = "uri:/mil/tatrc/phsyiology/datamodel";

      auto sofaData = std::unique_ptr<CDM::SequentialOrganFailureAssessmentData>();
      io::PatientAssessments::Marshall(sofa, *sofaData);

      sofaData->contentVersion(branded_version_string());
      SequentialOrganFailureAssessment(stream, *sofaData, map);
      stream.close();
      break;
    }
    default: {
      m_ss << "Unsupported assessment request " << assessment->GetType();
      m_Logger->Error(m_ss.str(), "BioGearsEngine");
      return false;
    }
    }
    m_ss.str("");
    m_ss.clear();
    return true;
  }

  return GetActions().ProcessAction(action, *this);
}
//-------------------------------------------------------------------------------
bool BioGearsEngine::IsReady()
{
  if (m_State == EngineState::NotReady) {
    m_Logger->Error("Engine is not ready to process, Initialize the engine or Load a state.");
    return false;
  }
  return true;
}
//-------------------------------------------------------------------------------
void BioGearsEngine::SetEventHandler(SEEventHandler* handler)
{
  m_EventHandler = handler;
  if (m_Patient != nullptr) {
    m_Patient->ForwardEvents(m_EventHandler);
  }
  m_AnesthesiaMachine->ForwardEvents(m_EventHandler);
}
//-------------------------------------------------------------------------------
const PhysiologyEngineConfiguration* BioGearsEngine::GetConfiguration()
{
  return &BioGears::GetConfiguration();
}
//-------------------------------------------------------------------------------
const SEPatient& BioGearsEngine::GetPatient()
{
  return BioGears::GetPatient();
}
//-------------------------------------------------------------------------------
bool BioGearsEngine::GetPatientAssessment(SEPatientAssessment& assessment)
{
  if (!IsReady()) {
    return false;
  }
  return BioGears::GetPatientAssessment(assessment);
}
//-------------------------------------------------------------------------------
const SEEnvironment* BioGearsEngine::GetEnvironment()
{
  return &BioGears::GetEnvironment();
}
//-------------------------------------------------------------------------------
SESubstanceManager& BioGearsEngine::GetSubstanceManager()
{
  return *m_Substances;
}
//-------------------------------------------------------------------------------
const SEBloodChemistrySystem* BioGearsEngine::GetBloodChemistrySystem()
{
  return &BioGears::GetBloodChemistry();
}
//-------------------------------------------------------------------------------
const SECardiovascularSystem* BioGearsEngine::GetCardiovascularSystem()
{
  return &BioGears::GetCardiovascular();
}
//-------------------------------------------------------------------------------
const SEDrugSystem* BioGearsEngine::GetDrugSystem()
{
  return &BioGears::GetDrugs();
}
//-------------------------------------------------------------------------------
const SEEndocrineSystem* BioGearsEngine::GetEndocrineSystem()
{
  return &BioGears::GetEndocrine();
}
//-------------------------------------------------------------------------------
const SEEnergySystem* BioGearsEngine::GetEnergySystem()
{
  return &BioGears::GetEnergy();
}
//-------------------------------------------------------------------------------
const SEGastrointestinalSystem* BioGearsEngine::GetGastrointestinalSystem()
{
  return &BioGears::GetGastrointestinal();
}
//-------------------------------------------------------------------------------
const SEHepaticSystem* BioGearsEngine::GetHepaticSystem()
{
  return &BioGears::GetHepatic();
}
//-------------------------------------------------------------------------------
const SENervousSystem* BioGearsEngine::GetNervousSystem()
{
  return &BioGears::GetNervous();
}
//-------------------------------------------------------------------------------
const SERenalSystem* BioGearsEngine::GetRenalSystem()
{
  return &BioGears::GetRenal();
}
//-------------------------------------------------------------------------------
const SERespiratorySystem* BioGearsEngine::GetRespiratorySystem()
{
  return &BioGears::GetRespiratory();
}
//-------------------------------------------------------------------------------
const SETissueSystem* BioGearsEngine::GetTissueSystem()
{
  return &BioGears::GetTissue();
}
//-------------------------------------------------------------------------------
const SEAnesthesiaMachine* BioGearsEngine::GetAnesthesiaMachine()
{
  return &BioGears::GetAnesthesiaMachine();
}
//-------------------------------------------------------------------------------
const SEElectroCardioGram* BioGearsEngine::GetElectroCardioGram()
{
  return &BioGears::GetECG();
}
//-------------------------------------------------------------------------------
const SEInhaler* BioGearsEngine::GetInhaler()
{
  return &BioGears::GetInhaler();
}
//-------------------------------------------------------------------------------
const SECompartmentManager& BioGearsEngine::GetCompartments()
{
  return BioGears::GetCompartments();
}
//-------------------------------------------------------------------------------
Tree<const char*> BioGearsEngine::GetDataRequestGraph() const
{
  return Tree<const char*> { "BioGearsEngine" }
    .emplace_back(GetBloodChemistry().GetPhysiologyRequestGraph())
    .emplace_back(GetCardiovascular().GetPhysiologyRequestGraph())
    .emplace_back(GetDrugs().GetPhysiologyRequestGraph())
    .emplace_back(GetEndocrine().GetPhysiologyRequestGraph())
    .emplace_back(GetEnergy().GetPhysiologyRequestGraph())
    .emplace_back(GetGastrointestinal().GetPhysiologyRequestGraph())
    .emplace_back(GetHepatic().GetPhysiologyRequestGraph())
    .emplace_back(GetNervous().GetPhysiologyRequestGraph())
    .emplace_back(GetRenal().GetPhysiologyRequestGraph())
    .emplace_back(GetRespiratory().GetPhysiologyRequestGraph())
    .emplace_back(GetTissue().GetPhysiologyRequestGraph());
}
bool BioGearsEngine::IsAutoTracking() const
{
  return m_isAutoTracking;
}
//-------------------------------------------------------------------------------
void BioGearsEngine::SetAutoTrackFlag(bool flag)
{
  m_isAutoTracking = flag;
}
//-------------------------------------------------------------------------------
bool BioGearsEngine::IsTrackingStabilization() const
{
  return m_areTrackingStabilization;
}
//-------------------------------------------------------------------------------
void BioGearsEngine::SetTrackStabilizationFlag(bool flag)
{
  m_areTrackingStabilization = flag;
}

//-------------------------------------------------------------------------------
biogears::Logger* create_logger(const char* logfile)
{
  return new Logger(logfile);
}
void destroy_logger(Logger** logger)
{
  if (logger) {
    delete (*logger);
    (*logger) = nullptr;
  }
}
biogears::BioGearsEngine* create_biogears_engine(biogears::Logger* logger, const char* working_dir)
{
  return new BioGearsEngine(logger, working_dir);
}
biogears::BioGearsEngine* create_biogears_engine(const char* log_file, const char* working_dir)
{
  return new BioGearsEngine(log_file, working_dir);
}
void destroy_biogears_engine(BioGearsEngine** engine)
{
  if (engine) {
    delete (*engine);
    (*engine) = nullptr;
  }
}
}
