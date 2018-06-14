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

#pragma warning(push)
// Disabling the waring about STL classes used have
// to have a dll interface to be used by clients
// From what I have read STL is compiler dependent
// But since we are releasing source, you can build
// the project neccessary to ensure proper linkage
// If anyone else has opinions on this, let me know
// abray@ara.com
#pragma warning(disable : 4251)
#include <biogears/engine/Controller/BioGears.h>
#pragma warning(pop)

//--------------------------------------------------------------------------------------------------
/// @brief
/// This is the implementation of the PhysiologyEngine interface for the biogears engines.
/// @details
/// It contains the necessary execution calls, patient customization calls, insult and intervention
/// calls as well as assessment calls for obtaining the results. During engine execution a log files
/// is generated containing information, warning and error data.
//--------------------------------------------------------------------------------------------------
class BIOGEARS_API BioGearsEngine : public PhysiologyEngine, public BioGears {
public:
  BioGearsEngine(Logger* logger);
  BioGearsEngine(const std::string&);
  virtual ~BioGearsEngine();

  virtual bool LoadState(const std::string& file, const SEScalarTime* simTime = nullptr);
  virtual bool LoadState(const CDM::PhysiologyEngineStateData& state, const SEScalarTime* simTime = nullptr);
  virtual std::unique_ptr<CDM::PhysiologyEngineStateData> SaveState(const std::string& file = "");

  virtual Logger* GetLogger();
  virtual PhysiologyEngineTrack* GetEngineTrack();

  virtual bool InitializeEngine(const std::string& patientFile, const std::vector<const SECondition*>* conditions = nullptr, const PhysiologyEngineConfiguration* config = nullptr);
  virtual bool InitializeEngine(const SEPatient& patient, const std::vector<const SECondition*>* conditions = nullptr, const PhysiologyEngineConfiguration* config = nullptr);

  virtual const PhysiologyEngineConfiguration* GetConfiguration();

  virtual double GetTimeStep(const TimeUnit& unit);
  virtual double GetSimulationTime(const TimeUnit& unit);

  virtual void AdvanceModelTime();
  virtual void AdvanceModelTime(double time, const TimeUnit& unit);
  virtual bool ProcessAction(const SEAction& action);

  virtual SESubstanceManager& GetSubstanceManager();
  virtual void SetEventHandler(SEEventHandler* handler);
  virtual const SEPatient& GetPatient();
  virtual bool GetPatientAssessment(SEPatientAssessment& assessment);

  virtual const SEEnvironment* GetEnvironment();
  virtual const SEBloodChemistrySystem* GetBloodChemistrySystem();
  virtual const SECardiovascularSystem* GetCardiovascularSystem();
  virtual const SEDrugSystem* GetDrugSystem();
  virtual const SEEndocrineSystem* GetEndocrineSystem();
  virtual const SEEnergySystem* GetEnergySystem();
  virtual const SEGastrointestinalSystem* GetGastrointestinalSystem();
  virtual const SEHepaticSystem* GetHepaticSystem();
  virtual const SENervousSystem* GetNervousSystem();
  virtual const SERenalSystem* GetRenalSystem();
  virtual const SERespiratorySystem* GetRespiratorySystem();
  virtual const SETissueSystem* GetTissueSystem();
  virtual const SEAnesthesiaMachine* GetAnesthesiaMachine();
  virtual const SEElectroCardioGram* GetElectroCardioGram();
  virtual const SEInhaler* GetInhaler();

  virtual const SECompartmentManager& GetCompartments();

protected:
  virtual bool IsReady();
  virtual bool InitializeEngine(const std::vector<const SECondition*>* conditions = nullptr, const PhysiologyEngineConfiguration* config = nullptr);

  SEEventHandler* m_EventHandler;
  PhysiologyEngineTrack m_EngineTrack;
  std::stringstream m_ss;
};