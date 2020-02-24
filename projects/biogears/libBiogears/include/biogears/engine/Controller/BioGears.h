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
#include <biogears/cdm/CommonDataModel.h>
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

namespace biogears {
class SEPatientAssessment;
class BioGearsScenarioExec;

enum class EngineState { NotReady = 0,
                         Initialization,
                         InitialStabilization,
                         AtInitialStableState,
                         SecondaryStabilization,
                         AtSecondaryStableState,
                         Active };

class ECG;
class DataTrack;
class SaturationCalculator;
class BioGearsSubstances;
class SEPatient;
class SEBloodChemistrySystem;
class SECardiovascularSystem;
class SEDrugSystem;
class SEEndocrineSystem;
class SEEnergySystem;
class SEGastrointestinalSystem;
class SEHepaticSystem;
class SENervousSystem;
class SERenalSystem;
class SERespiratorySystem;
class SETissueSystem;
class SEEnvironment;
class SEAnesthesiaMachine;
class SEElectroCardioGram;
class SEInhaler;
class SEActionManager;
class SEConditionManager;
class BioGearsCircuits;
class BioGearsCompartments;
class BioGearsConfiguration;
class SEScalarTime;

/**
* @brief Manages and controls execution of all data/systems in %BioGears
*/
class BIOGEARS_API BioGears {
  friend class BioGearsEngineTest;
  friend class BioGearsScenarioExec;

protected:
  EngineState m_State;

public:
  BioGears(Logger* logger);
  BioGears(const std::string& logfileName);
  BioGears(Logger* logger, const std::string& working_dir);
  BioGears(const std::string& logfileName, const std::string& working_dir);
  virtual ~BioGears();

  EngineState GetState();
  const EngineState GetState() const;

  DataTrack& GetDataTrack();
  SaturationCalculator& GetSaturationCalculator();
  const SaturationCalculator& GetSaturationCalculator() const;

  DiffusionCalculator& GetDiffusionCalculator();
  const DiffusionCalculator& GetDiffusionCalculator() const;

  BioGearsSubstances& GetSubstances();
  const BioGearsSubstances& GetSubstances() const;

  SEPatient& GetPatient();
  const SEPatient& GetPatient() const;

  bool GetPatientAssessment(SEPatientAssessment& assessment);

  SEBloodChemistrySystem& GetBloodChemistry();
  SECardiovascularSystem& GetCardiovascular();
  SEDrugSystem& GetDrugs();
  SEEndocrineSystem& GetEndocrine();
  SEEnergySystem& GetEnergy();
  SEGastrointestinalSystem& GetGastrointestinal();
  SEHepaticSystem& GetHepatic();
  SENervousSystem& GetNervous();
  SERenalSystem& GetRenal();
  SERespiratorySystem& GetRespiratory();
  SETissueSystem& GetTissue();
  SEEnvironment& GetEnvironment();
  SEAnesthesiaMachine& GetAnesthesiaMachine();
  SEElectroCardioGram& GetECG();
  SEInhaler& GetInhaler();
  SEActionManager& GetActions();
  SEConditionManager& GetConditions();
  BioGearsCircuits& GetCircuits();
  BioGearsCompartments& GetCompartments();
  const BioGearsConfiguration& GetConfiguration();
  const SEScalarTime& GetEngineTime();
  const SEScalarTime& GetSimulationTime();
  const SEScalarTime& GetTimeStep();
  CDM::enumBioGearsAirwayMode::value GetAirwayMode();
  CDM::enumOnOff::value GetIntubation();

  const SEBloodChemistrySystem& GetBloodChemistry() const;
  const SECardiovascularSystem& GetCardiovascular() const;
  const SEDrugSystem& GetDrugs() const;
  const SEEndocrineSystem& GetEndocrine() const;
  const SEEnergySystem& GetEnergy() const;
  const SEGastrointestinalSystem& GetGastrointestinal() const;
  const SEHepaticSystem& GetHepatic() const;
  const SENervousSystem& GetNervous() const;
  const SERenalSystem& GetRenal() const;
  const SERespiratorySystem& GetRespiratory() const;
  const SETissueSystem& GetTissue() const;
  const SEEnvironment& GetEnvironment() const;
  const SEAnesthesiaMachine& GetAnesthesiaMachine() const;
  const SEElectroCardioGram& GetECG() const;
  const SEInhaler& GetInhaler() const;
  const SEActionManager& GetActions() const;
  const SEConditionManager& GetConditions() const;
  const BioGearsCircuits& GetCircuits() const;
  const BioGearsCompartments& GetCompartments() const;
  const BioGearsConfiguration& GetConfiguration() const;
  const SEScalarTime& GetEngineTime() const;
  const SEScalarTime& GetSimulationTime() const;
  const SEScalarTime& GetTimeStep() const;
  const CDM::enumBioGearsAirwayMode::value GetAirwayMode() const;
  const CDM::enumOnOff::value GetIntubation() const;

  bool CreateCircuitsAndCompartments();
  void SetIntubation(CDM::enumOnOff::value s);
  void SetAirwayMode(CDM::enumBioGearsAirwayMode::value mode);

  Logger* GetLogger() const;
protected:
  void SetupCardiovascular();
  void SetupCerebral();
  void SetupRenal();
  void SetupTissue();
  void SetupGastrointestinal();
  void SetupRespiratory();
  void SetupAnesthesiaMachine();
  void SetupInhaler();
  void SetupMechanicalVentilator();
  void SetupExternalTemperature();
  void SetupInternalTemperature();


  bool Initialize(const PhysiologyEngineConfiguration* config);
  bool SetupPatient();

  void SetUp();

  // Notify systems that steady state has been achieved
  virtual void AtSteadyState(EngineState state);
  void PreProcess();
  void Process();
  void PostProcess();

  void ForwardFatal(const std::string& msg, const std::string& origin);

  DataTrack* m_DataTrack;

  std::unique_ptr<SEScalarTime> m_CurrentTime;
  std::unique_ptr<SEScalarTime> m_SimulationTime;
  CDM::enumBioGearsAirwayMode::value m_AirwayMode;
  CDM::enumOnOff::value m_Intubation;

  std::unique_ptr<BioGearsConfiguration> m_Config;
  std::unique_ptr<SaturationCalculator> m_SaturationCalculator;
  std::unique_ptr<DiffusionCalculator> m_DiffusionCalculator;

  std::unique_ptr<BioGearsSubstances> m_Substances;

  std::unique_ptr<SEActionManager> m_Actions;
  std::unique_ptr<SEConditionManager> m_Conditions;
  std::unique_ptr<BioGearsCircuits> m_Circuits;
  std::unique_ptr<BioGearsCompartments> m_Compartments;

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

  std::unique_ptr<ECG> m_ECG;

  std::unique_ptr<AnesthesiaMachine> m_AnesthesiaMachine;

  std::unique_ptr<Inhaler> m_Inhaler;

  std::unique_ptr<SEPatient> m_Patient;

  // Flag to destroy the logger or not
  bool m_logger_self_managed;
  Logger* m_Logger;

};
}