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
#include <biogears/engine/Controller/BioGearsCircuits.h>
#include <biogears/engine/Controller/BioGearsCompartments.h>
#include <biogears/engine/Controller/BioGearsConfiguration.h>
#include <biogears/engine/Controller/BioGearsSubstances.h>

#include <biogears/schema/enumBioGearsAirwayMode.hxx>
// CDM 
#include <biogears/cdm/scenario/SEActionManager.h>
#include <biogears/cdm/scenario/SEConditionManager.h>
#include <biogears/cdm/properties/SEScalarTime.h>

#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>

// BioGears
#include <biogears/engine/Systems/BloodChemistry.h>
#include <biogears/engine/Systems/Cardiovascular.h>
#include <biogears/engine/Systems/Endocrine.h>
#include <biogears/engine/Systems/Energy.h>
#include <biogears/engine/Systems/Gastrointestinal.h>
#include <biogears/engine/Systems/Hepatic.h>
#include <biogears/engine/Systems/Nervous.h>
#include <biogears/engine/Systems/Renal.h>
#include <biogears/engine/Systems/Respiratory.h>
#include <biogears/engine/Systems/Drugs.h>
#include <biogears/engine/Systems/Tissue.h>
#include <biogears/engine/Systems/Environment.h>

#include <biogears/engine/Equipment/ECG.h>
#include <biogears/engine/Equipment/AnesthesiaMachine.h>
#include <biogears/engine/Equipment/Inhaler.h>

#include <biogears/engine/Systems/Saturation.h>

class BioGearsScenarioExec;
enum class EngineState { NotReady=0, Initialization, InitialStabilization, AtInitialStableState, SecondaryStabilization, AtSecondaryStableState, Active };

/**
* @brief Manages and controls execution of all data/systems in %BioGears
*/
class BIOGEARS_API BioGears : public Loggable, protected LoggerForward
{
  friend class BioGearsEngineTest;
  friend class BioGearsScenarioExec;
protected:
  EngineState m_State;
public:
  

  BioGears(Logger* logger);
  BioGears(const std::string& logfileName);
  virtual ~BioGears();

 
  EngineState                             GetState() { return m_State; }

  DataTrack&                              GetDataTrack();
  SaturationCalculator&                   GetSaturationCalculator() { return *m_SaturationCalculator; }

  BioGearsSubstances&                     GetSubstances() { return *m_Substances; }

  SEPatient&                              GetPatient() { return *m_Patient; }
  bool                                    GetPatientAssessment(SEPatientAssessment& assessment);

  SEBloodChemistrySystem&                 GetBloodChemistry() { return *m_BloodChemistrySystem; }
  SECardiovascularSystem&                 GetCardiovascular() { return *m_CardiovascularSystem; }
  SEDrugSystem&                           GetDrugs() { return *m_DrugSystem; }
  SEEndocrineSystem&                      GetEndocrine() { return *m_EndocrineSystem; }
  SEEnergySystem&                         GetEnergy() { return *m_EnergySystem; }
  SEGastrointestinalSystem&               GetGastrointestinal() { return *m_GastrointestinalSystem; }
  SEHepaticSystem&                        GetHepatic() { return *m_HepaticSystem; }
  SENervousSystem&                        GetNervous() { return *m_NervousSystem; }
  SERenalSystem&                          GetRenal() { return *m_RenalSystem; }
  SERespiratorySystem&                    GetRespiratory() { return *m_RespiratorySystem; }
  SETissueSystem&                         GetTissue() { return *m_TissueSystem; }

  SEEnvironment&                          GetEnvironment() { return *m_Environment; }

  SEAnesthesiaMachine&                    GetAnesthesiaMachine() { return *m_AnesthesiaMachine; }

  SEElectroCardioGram&                    GetECG() { return *m_ECG; }

  SEInhaler&                              GetInhaler() { return *m_Inhaler; }

  SEActionManager&                        GetActions() { return *m_Actions; }

  SEConditionManager&                     GetConditions() { return *m_Conditions; }

  BioGearsCircuits&                       GetCircuits() { return *m_Circuits; }


  BioGearsCompartments&                   GetCompartments() { return *m_Compartments; }

  const BioGearsConfiguration&            GetConfiguration() { return *m_Config; }

  const SEScalarTime&                     GetEngineTime() { return *m_CurrentTime; }
  const SEScalarTime&                     GetSimulationTime() { return *m_SimulationTime; }
  const SEScalarTime&                     GetTimeStep() { return m_Config->GetTimeStep(); }

  CDM::enumBioGearsAirwayMode::value      GetAirwayMode() { return m_AirwayMode; }
  void                                    SetAirwayMode(CDM::enumBioGearsAirwayMode::value mode);

  CDM::enumOnOff::value                   GetIntubation() { return m_Intubation; }
  void                                    SetIntubation(CDM::enumOnOff::value s);

  bool CreateCircuitsAndCompartments();
protected:
  void SetupCardiovascular();
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

  // Notify systems that steady state has been achieved
  virtual void AtSteadyState(EngineState state);
  void PreProcess();
  void Process();
  void PostProcess();

  void ForwardFatal(const std::string&  msg, const std::string&  origin);

  DataTrack*                                                    m_DataTrack;

  std::unique_ptr<SEScalarTime>                                 m_CurrentTime;
  std::unique_ptr<SEScalarTime>                                 m_SimulationTime;
  CDM::enumBioGearsAirwayMode::value                            m_AirwayMode;
  CDM::enumOnOff::value                                         m_Intubation;

  std::unique_ptr<BioGearsConfiguration>                        m_Config;
  std::unique_ptr<SaturationCalculator>                         m_SaturationCalculator;

  std::unique_ptr<BioGearsSubstances>                           m_Substances;

  std::unique_ptr<SEActionManager>                              m_Actions;
  std::unique_ptr<SEConditionManager>                           m_Conditions;
  std::unique_ptr<BioGearsCircuits>                             m_Circuits;
  std::unique_ptr<BioGearsCompartments>                         m_Compartments;

  std::unique_ptr<Environment>                                  m_Environment;

  std::unique_ptr<BloodChemistry>                               m_BloodChemistrySystem;
  std::unique_ptr<Cardiovascular>                               m_CardiovascularSystem;
  std::unique_ptr<Endocrine>                                    m_EndocrineSystem;
  std::unique_ptr<Energy>                                       m_EnergySystem;
  std::unique_ptr<Gastrointestinal>                             m_GastrointestinalSystem;
  std::unique_ptr<Hepatic>                                      m_HepaticSystem;
  std::unique_ptr<Nervous>                                      m_NervousSystem;
  std::unique_ptr<Renal>                                        m_RenalSystem;
  std::unique_ptr<Respiratory>                                  m_RespiratorySystem;
  std::unique_ptr<Drugs>                                        m_DrugSystem;
  std::unique_ptr<Tissue>                                       m_TissueSystem;

  std::unique_ptr<ECG>                                          m_ECG;

  std::unique_ptr<AnesthesiaMachine>                            m_AnesthesiaMachine;

  std::unique_ptr<Inhaler>                                      m_Inhaler;

  std::unique_ptr<SEPatient>                                    m_Patient;

  // Flag to destroy the logger or not                          
  bool                                                          myLogger;
};

