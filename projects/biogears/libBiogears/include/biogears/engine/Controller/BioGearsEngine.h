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
#include <biogears/cdm/engine/PhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
#pragma warning(pop)

namespace biogears {
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
  //-------------------------------------------------------------------------------
  BioGearsEngine(Logger* logger);
  
  BioGearsEngine(const std::string& logFileName);
  BioGearsEngine(const char* logFileName);
  BioGearsEngine(Logger* logger, const std::string& working_dir);
  BioGearsEngine(Logger* logger, const char* working_dir);
  BioGearsEngine(const std::string&, const std::string& working_dir);
  BioGearsEngine(const char*, const char*);
  
  virtual ~BioGearsEngine() override;

  virtual bool LoadState(const char* file, const SEScalarTime* simTime = nullptr) override;
  virtual bool LoadState(const std::string& file, const SEScalarTime* simTime = nullptr) override;
  virtual bool LoadState(const CDM::PhysiologyEngineStateData& state, const SEScalarTime* simTime = nullptr) override;
  virtual std::unique_ptr<CDM::PhysiologyEngineStateData> SaveState(const char* file) override;
  virtual std::unique_ptr<CDM::PhysiologyEngineStateData> SaveState(const std::string& file = "") override;

  virtual Logger* GetLogger() override;
  virtual PhysiologyEngineTrack* GetEngineTrack() override;

  virtual bool InitializeEngine(const char* patientFile, const std::vector<const SECondition*>* conditions = nullptr, const PhysiologyEngineConfiguration* config = nullptr) override;
  virtual bool InitializeEngine(const std::string& patientFile, const std::vector<const SECondition*>* conditions = nullptr, const PhysiologyEngineConfiguration* config = nullptr) override;
  virtual bool InitializeEngine(const SEPatient& patient, const std::vector<const SECondition*>* conditions = nullptr, const PhysiologyEngineConfiguration* config = nullptr) override;

  virtual const PhysiologyEngineConfiguration* GetConfiguration() override;

  virtual double GetTimeStep(const TimeUnit& unit) override;
  virtual double GetSimulationTime(const TimeUnit& unit) override;

  virtual void AdvanceModelTime() override;
  virtual void AdvanceModelTime(double time, const TimeUnit& unit) override;
  virtual bool ProcessAction(const SEAction& action) override;

  virtual SESubstanceManager& GetSubstanceManager() override;
  virtual void SetEventHandler(SEEventHandler* handler) override;
  virtual const SEPatient& GetPatient() override;
  virtual bool GetPatientAssessment(SEPatientAssessment& assessment) override;

  virtual const SEEnvironment* GetEnvironment() override;
  virtual const SEBloodChemistrySystem* GetBloodChemistrySystem() override;
  virtual const SECardiovascularSystem* GetCardiovascularSystem() override;
  virtual const SEDrugSystem* GetDrugSystem() override;
  virtual const SEEndocrineSystem* GetEndocrineSystem() override;
  virtual const SEEnergySystem* GetEnergySystem() override;
  virtual const SEGastrointestinalSystem* GetGastrointestinalSystem() override;
  virtual const SEHepaticSystem* GetHepaticSystem() override;
  virtual const SENervousSystem* GetNervousSystem() override;
  virtual const SERenalSystem* GetRenalSystem() override;
  virtual const SERespiratorySystem* GetRespiratorySystem() override;
  virtual const SETissueSystem* GetTissueSystem() override;
  virtual const SEAnesthesiaMachine* GetAnesthesiaMachine() override;
  virtual const SEElectroCardioGram* GetElectroCardioGram() override;
  virtual const SEInhaler* GetInhaler() override;

  virtual const SECompartmentManager& GetCompartments() override;
  virtual Tree<const char*> GetDataRequestGraph() const override;


protected:
  virtual bool IsReady();
  virtual bool InitializeEngine(const std::vector<const SECondition*>* conditions = nullptr, const PhysiologyEngineConfiguration* config = nullptr);

  SEEventHandler* m_EventHandler;
  PhysiologyEngineTrack m_EngineTrack;
#pragma warning(push,0)
  std::stringstream m_ss;
#pragma warning(pop)

};
}