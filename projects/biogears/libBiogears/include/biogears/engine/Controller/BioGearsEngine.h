
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
#pragma once
#include <string>
#pragma warning(push)
// Disabling the waring about STL classes used have
// to have a dll interface to be used by clients
// From what I have read STL is compiler dependent
// But since we are releasing source, you can build
// the project neccessary to ensure proper linkage
// If anyone else has opinions on this, let me know
// abray@ara.com
#pragma warning(disable : 4251)
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/engine/PhysiologyEngine.h>
#include <biogears/exports.h>

#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/scenario/SEActionManager.h>
#include <biogears/cdm/scenario/SEConditionManager.h>
#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/engine/Controller/BioGearsCircuits.h>
#include <biogears/engine/Controller/BioGearsCompartments.h>
#include <biogears/engine/Controller/BioGearsConfiguration.h>
#include <biogears/engine/Controller/BioGearsSubstances.h>
#include <biogears/engine/Equipment/AnesthesiaMachine.h>
#include <biogears/engine/Equipment/ECG.h>
#include <biogears/engine/Equipment/Inhaler.h>
#include <biogears/engine/Systems/BloodChemistry.h>
#include <biogears/engine/Systems/Cardiovascular.h>
#include <biogears/engine/Systems/Diffusion.h>
#include <biogears/engine/Systems/Drugs.h>
#include <biogears/engine/Systems/Endocrine.h>
#include <biogears/engine/Systems/Energy.h>
#include <biogears/engine/Systems/Environment.h>
#include <biogears/engine/Systems/Gastrointestinal.h>
#include <biogears/engine/Systems/Hepatic.h>
#include <biogears/engine/Systems/Nervous.h>
#include <biogears/engine/Systems/Renal.h>
#include <biogears/engine/Systems/Respiratory.h>
#include <biogears/engine/Systems/Saturation.h>
#include <biogears/engine/Systems/Tissue.h>
#include <biogears/schema/biogears/BioGears.hxx>

#include <memory>
#pragma warning(pop)

namespace biogears {

enum class SimulationPhase { NotReady = 0,
                             Initialization,
                             InitialStabilization,
                             AtInitialStableState,
                             SecondaryStabilization,
                             AtSecondaryStableState,
                             Active };

//--------------------------------------------------------------------------------------------------
/// @brief
/// This is the implementation of the PhysiologyEngine interface for the biogears engines.
/// @details
/// It contains the necessary execution calls, patient customization calls, insult and intervention
/// calls as well as assessment calls for obtaining the results. During engine execution a log files
/// is generated containing information, warning and error data.
//--------------------------------------------------------------------------------------------------
class BioGearsEngine : public PhysiologyEngine {
  friend class BioGearsEngineTest;
  friend class BioGearsScenarioExec;
  friend class SEScenarioExec;
  friend class AnesthesiaMachine;

public:
  //-------------------------------------------------------------------------------
  BIOGEARS_API BioGearsEngine(Logger const* logger);
  BIOGEARS_API BioGearsEngine(Logger const* logger, char const* working_dir);
  BIOGEARS_API BioGearsEngine(Logger const* logger, std::string const& working_dir);

  BIOGEARS_API BioGearsEngine(char const* logFileName);
  BIOGEARS_API BioGearsEngine(std::string const& logFileName);
  BIOGEARS_API BioGearsEngine(char const*, char const*);
  BIOGEARS_API BioGearsEngine(std::string const&, std::string const& working_dir);

  BIOGEARS_API ~BioGearsEngine() override;

  BIOGEARS_API auto IsValid() -> bool;

  BIOGEARS_API auto LoadState(char const* file, SEScalarTime const* simTime = nullptr) -> bool override;
  BIOGEARS_API auto LoadState(std::string const& file, SEScalarTime const* simTime = nullptr) -> bool override;
  BIOGEARS_API auto LoadState(const CDM::PhysiologyEngineStateData& state, SEScalarTime const* simTime = nullptr) -> bool override;
  BIOGEARS_API auto LoadState(char const* buffer, size_t size) -> bool override;

  BIOGEARS_API auto SaveStateToFile(char const* file) const -> void override;
  BIOGEARS_API auto SaveStateToFile(std::string const& file = "") const -> void override;

  BIOGEARS_API auto GetLogger() -> Logger*;
  BIOGEARS_API auto GetLogger() const -> Logger const* override;
  BIOGEARS_API auto GetEngineTrack() const -> PhysiologyEngineTrack const& override;
  BIOGEARS_API auto GetEngineTrack() -> PhysiologyEngineTrack&;

  BIOGEARS_API auto InitializeEngine(char const* patientFile) -> bool override;
  BIOGEARS_API auto InitializeEngine(char const* patientFile, std::vector<const SECondition*> const* conditions) -> bool override;
  BIOGEARS_API auto InitializeEngine(char const* patientFile, const std::vector<const SECondition*>* conditions, const PhysiologyEngineConfiguration* config) -> bool override;
  BIOGEARS_API auto InitializeEngine(std::string const& patientFile, const std::vector<const SECondition*>* conditions = nullptr, PhysiologyEngineConfiguration const* config = nullptr) -> bool override;
  BIOGEARS_API auto InitializeEngine(SEPatient const& patient, const std::vector<const SECondition*>* conditions = nullptr, PhysiologyEngineConfiguration const* config = nullptr) -> bool override;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Advances the model time by one deltaT.
  //!
  //! BioGears updates at the rate set in BioGearsConfiguration.xml which defaults to 0.02 or 50hz
  //! This is the resolution of the underlying simulation. That is to say you can not record
  //!	physiology metrics at simulation times that would fall between a deltaT. Additionally advancing time by
  //! non multiples of deltaT can result in the true simtime being out of sync with the expected time sync
  //!-------------------------------------------------------------------------------------------------
  BIOGEARS_API auto AdvanceModelTime(bool appendDataTrack = false) -> bool override;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Subdivdes the time provided by deltaT and calls AdvanceModelTime() for each subdivision
  //!
  //! See AdvanceModelTime(bool) for additional details.
  //!
  //!-------------------------------------------------------------------------------------------------
  BIOGEARS_API auto AdvanceModelTime(double time, TimeUnit const& unit = TimeUnit::s, bool appendDataTrack = false) -> bool override; // NOTE: Maynot compile on clang will evaluate
  BIOGEARS_API auto ProcessAction(SEAction const& action) -> bool override;

  BIOGEARS_API auto GetSubstanceManager() -> BioGearsSubstances&;
  BIOGEARS_API auto GetSubstanceManager() const -> BioGearsSubstances const& override;

  BIOGEARS_API auto SetEventHandler(SEEventHandler* handler) -> void override;
  BIOGEARS_API auto GetPatientAssessment(SEPatientAssessment& assessment) const -> bool override;

  BIOGEARS_API Tree<char const*> GetDataRequestGraph() const override;

  BIOGEARS_API bool IsAutoTracking() const override;
  BIOGEARS_API void SetAutoTrackFlag(bool flag) override;
  BIOGEARS_API bool IsTrackingStabilization() const override;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! This value will be overridden if TrackingStabilizationCriteria is loaded by any means
  //! After it is set. But can be used as a shortcut to modify m_Config->GetStabilizationCriteria()->TrackStabilization()
  //! After stabiliztion criteira has been loaded.
  //!-------------------------------------------------------------------------------------------------
  BIOGEARS_API auto SetTrackStabilizationFlag(bool flag) -> void override;

  BIOGEARS_API SimulationPhase GetSimulationPhase() const;

  BIOGEARS_API auto GetDataTrack() -> DataTrack&;
  BIOGEARS_API auto GetDataTrack() const -> DataTrack const&;

  BIOGEARS_API auto GetSaturationCalculator() -> SaturationCalculator&;
  BIOGEARS_API auto GetSaturationCalculator() const -> SaturationCalculator const&;

  BIOGEARS_API DiffusionCalculator& GetDiffusionCalculator();
  BIOGEARS_API DiffusionCalculator const& GetDiffusionCalculator() const;

  BIOGEARS_API SEPatient& GetPatient();
  BIOGEARS_API SEPatient const& GetPatient() const;

  BIOGEARS_API auto GetActions() -> SEActionManager&;
  BIOGEARS_API auto GetActions() const -> SEActionManager const& override;

  BIOGEARS_API auto GetAnesthesiaMachine() -> AnesthesiaMachine&;
  BIOGEARS_API auto GetAnesthesiaMachine() const -> AnesthesiaMachine const& override;

  BIOGEARS_API auto GetBloodChemistrySystem() -> BloodChemistry&;
  BIOGEARS_API auto GetBloodChemistrySystem() const -> BloodChemistry const& override;

  BIOGEARS_API auto GetCardiovascularSystem() -> Cardiovascular&;
  BIOGEARS_API auto GetCardiovascularSystem() const -> Cardiovascular const& override;

  BIOGEARS_API auto GetCircuits() -> BioGearsCircuits&;
  BIOGEARS_API auto GetCircuits() const -> BioGearsCircuits const&;

  BIOGEARS_API auto GetCompartments() -> BioGearsCompartments&;
  BIOGEARS_API auto GetCompartments() const -> BioGearsCompartments const& override;

  BIOGEARS_API auto GetConditions() -> SEConditionManager&;
  BIOGEARS_API auto GetConditions() const -> SEConditionManager const& override;

  BIOGEARS_API auto GetConfiguration() -> BioGearsConfiguration&;
  BIOGEARS_API auto GetConfiguration() const -> BioGearsConfiguration const& override;

  BIOGEARS_API auto GetDrugsSystem() -> Drugs&;
  BIOGEARS_API auto GetDrugsSystem() const -> Drugs const& override;

  BIOGEARS_API auto GetElectroCardioGram() -> ECG&;
  BIOGEARS_API auto GetElectroCardioGram() const -> ECG const& override;

  BIOGEARS_API auto GetEndocrineSystem() -> Endocrine&;
  BIOGEARS_API auto GetEndocrineSystem() const -> Endocrine const& override;

  BIOGEARS_API auto GetEnergySystem() -> Energy&;
  BIOGEARS_API auto GetEnergySystem() const -> Energy const& override;

  BIOGEARS_API auto GetEngineTime() const -> SEScalarTime const&;
  BIOGEARS_API auto GetEngineTime() -> SEScalarTime const&;

  BIOGEARS_API auto GetEnvironment() -> Environment&;
  BIOGEARS_API auto GetEnvironment() const -> Environment const& override;

  BIOGEARS_API auto GetGastrointestinalSystem() -> Gastrointestinal&;
  BIOGEARS_API auto GetGastrointestinalSystem() const -> Gastrointestinal const& override;

  BIOGEARS_API auto GetHepaticSystem() -> Hepatic&;
  BIOGEARS_API auto GetHepaticSystem() const -> Hepatic const& override;

  BIOGEARS_API auto GetInhaler() -> Inhaler&;
  BIOGEARS_API auto GetInhaler() const -> Inhaler const& override;

  BIOGEARS_API auto GetNervousSystem() -> Nervous&;
  BIOGEARS_API auto GetNervousSystem() const -> Nervous const& override;

  BIOGEARS_API auto GetRenalSystem() -> Renal&;
  BIOGEARS_API auto GetRenalSystem() const -> Renal const& override;

  BIOGEARS_API auto GetRespiratorySystem() -> Respiratory&;
  BIOGEARS_API auto GetRespiratorySystem() const -> Respiratory const& override;

  BIOGEARS_API auto GetSimulationTime(TimeUnit const& unit) const -> double override;
  BIOGEARS_API auto GetSimulationTime() -> SEScalarTime&;
  BIOGEARS_API auto GetSimulationTime() const -> SEScalarTime const& override;

  BIOGEARS_API auto GetTimeStep(const TimeUnit& unit) const -> double override;
  BIOGEARS_API auto GetTimeStep() -> SEScalarTime&;
  BIOGEARS_API auto GetTimeStep() const -> SEScalarTime const& override;

  BIOGEARS_API auto GetTissueSystem() -> Tissue&;
  BIOGEARS_API auto GetTissueSystem() const -> Tissue const& override;

  BIOGEARS_API auto GetAirwayMode() const -> CDM::enumBioGearsAirwayMode::value const;
  BIOGEARS_API auto GetIntubation() const -> CDM::enumOnOff::value const;

  BIOGEARS_API void SetAirwayMode(CDM::enumBioGearsAirwayMode::value mode);
  BIOGEARS_API void SetIntubation(CDM::enumOnOff::value s);

protected:
  BIOGEARS_API bool IsReady() const;
  BIOGEARS_API bool InitializeEngine(const std::vector<const SECondition*>* conditions = nullptr, PhysiologyEngineConfiguration const* config = nullptr);

  BIOGEARS_API void SetupCardiovascular();
  BIOGEARS_API void SetupCerebral();
  BIOGEARS_API void SetupRenal();
  BIOGEARS_API void SetupTissue();
  BIOGEARS_API void SetupGastrointestinal();
  BIOGEARS_API void SetupRespiratory();
  BIOGEARS_API void SetupAnesthesiaMachine();
  BIOGEARS_API void SetupInhaler();
  BIOGEARS_API void SetupMechanicalVentilator();
  BIOGEARS_API void SetupExternalTemperature();
  BIOGEARS_API void SetupInternalTemperature();

  BIOGEARS_API bool Initialize(PhysiologyEngineConfiguration const* config);
  BIOGEARS_API bool SetupPatient();

  BIOGEARS_API void SetUp();

  // Notify systems that steady state has been achieved
  BIOGEARS_API void SimulationPhaseChange(SimulationPhase state);
  BIOGEARS_API void PreProcess();
  BIOGEARS_API void Process();
  BIOGEARS_API void PostProcess();

  BIOGEARS_API bool CreateCircuitsAndCompartments();

private:
  auto GetStateData() const -> std::unique_ptr<CDM::PhysiologyEngineStateData>;

  double m_timeStep_remainder = 0.0;
  double m_timeSinceLastDataTrack = 0.0;
  CDM::enumBioGearsAirwayMode::value m_AirwayMode = CDM::enumBioGearsAirwayMode::value::Free;
  CDM::enumOnOff::value m_Intubation = CDM::enumOnOff::value::Off;

  std::unique_ptr<Logger> m_managedLogger;
  std::unique_ptr<SEScalarTime> m_CurrentTime;
  std::unique_ptr<SEScalarTime> m_SimulationTime;

  std::unique_ptr<BioGearsSubstances> m_Substances;
  std::unique_ptr<BioGearsCircuits> m_Circuits;
  std::unique_ptr<BioGearsCompartments> m_Compartments;

  std::unique_ptr<BioGearsConfiguration> m_Config;
  std::unique_ptr<SEActionManager> m_Actions;
  std::unique_ptr<SEConditionManager> m_Conditions;

  std::unique_ptr<SEPatient> m_Patient;
  std::unique_ptr<Environment> m_Environment;
  std::unique_ptr<BloodChemistry> m_BloodChemistrySystem;
  std::unique_ptr<Cardiovascular> m_CardiovascularSystem;
  std::unique_ptr<Endocrine> m_EndocrineSystem;
  std::unique_ptr<Energy> m_EnergySystem;
  std::unique_ptr<Gastrointestinal> m_GastrointestinalSystem;
  std::unique_ptr<Hepatic> m_HepaticSystem;
  std::unique_ptr<Nervous> m_NervousSystem;
  std::unique_ptr<Renal> m_RenalSystem;
  std::unique_ptr<Respiratory> m_RespiratorySystem;
  std::unique_ptr<Drugs> m_DrugSystem;
  std::unique_ptr<Tissue> m_TissueSystem;
  std::unique_ptr<ECG> m_ElectroCardioGram;
  std::unique_ptr<AnesthesiaMachine> m_AnesthesiaMachine;
  std::unique_ptr<Inhaler> m_Inhaler;

  std::unique_ptr<SaturationCalculator> m_SaturationCalculator;
  std::unique_ptr<DiffusionCalculator> m_DiffusionCalculator;

  SEEventHandler* m_EventHandler;
  DataTrack* m_DataTrack;
  PhysiologyEngineTrack m_EngineTrack;

#pragma warning(push, 0)
  std::stringstream m_ss;
#pragma warning(pop)

  bool m_isAutoTracking = true;
  bool m_areTrackingStabilization = false;

  SimulationPhase m_SimulationPhase;

  /// END BIOGEARS
};

BIOGEARS_API Logger* create_logger(char const* logfile);
BIOGEARS_API void destroy_logger(Logger** engine);
BIOGEARS_API BioGearsEngine* create_biogears_engine(biogears::Logger* logger, char const* working_dir);
BIOGEARS_API BioGearsEngine* create_biogears_engine(char const* logger, char const* working_dir);
BIOGEARS_API void destroy_biogears_engine(BioGearsEngine** engine);
} // namespace biogears
