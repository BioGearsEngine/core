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
    io::BioGears::UnMarshall(*state, *this, simTime);
    return true;
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
    io::BioGears::UnMarshall(*state, *this);
    return true;
  }
  m_Logger->Error("File does not contain a valid PhysiologyEngineState");
  return false;
}

#pragma optimize("", on)//-------------------------------------------------------------------------------
void BioGearsEngine::SaveStateToFile(const char* file)
{
  SaveStateToFile(std::string { file });
}

//-------------------------------------------------------------------------------
#pragma optimize("", off)
void BioGearsEngine::SaveStateToFile(const std::string& file)
{
  auto state = std::make_unique<CDM::BioGearsStateData>();
  io::BioGears::Marshall(*this, *state);

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
#pragma optimize("", on)
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
  if (!m_Configuration->HasStabilizationCriteria()) {
    m_Logger->Error("BioGears needs stabilization criteria, none provided in configuration file");
    return false;
  }

  m_State = EngineState::InitialStabilization;
  if (!m_Configuration->GetStabilizationCriteria()->StabilizeRestingState(*this)) {
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
    if (!m_Configuration->GetStabilizationCriteria()->StabilizeConditions(*this, *conditions)) {
      return false;
    }
  } else {
    if (!m_Configuration->GetStabilizationCriteria()->StabilizeFeedbackState(*this)) {
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
  return m_Configuration->GetTimeStep(unit);
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

  m_Patient->UpdateEvents(m_Configuration->GetTimeStep());
  m_CurrentTime->Increment(m_Configuration->GetTimeStep());
  m_SimulationTime->Increment(m_Configuration->GetTimeStep());

  const auto sample_interval_s = 1.0 / GetEngineTrack()->GetDataRequestManager().GetSamplesPerSecond();
  m_timeSinceLastDataTrack += m_Configuration->GetTimeStep(TimeUnit::s);
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
  double remains = time_s / m_Configuration->GetTimeStep(TimeUnit::s);
  remains -= static_cast<int>(remains);
  m_timeStep_remainder += remains;

  int count = static_cast<int>(time_s / m_Configuration->GetTimeStep(TimeUnit::s));
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

      auto abgaData = std::make_unique<CDM::ArterialBloodGasAnalysisData>();
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

      auto pftData = std::make_unique<CDM::PulmonaryFunctionTestData>();
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

      auto pttData = std::make_unique<CDM::ProthrombinTimeData>();
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
      auto pvtData = std::make_unique<CDM::PsychomotorVigilanceTaskData>();
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
      auto upanData = std::make_unique<CDM::UrinalysisData>();
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
      auto cbcData = std::make_unique<CDM::CompleteBloodCountData>();
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

      auto cmpData = std::make_unique<CDM::ComprehensiveMetabolicPanelData>();
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

      auto sofaData = std::make_unique<CDM::SequentialOrganFailureAssessmentData>();
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
