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
  BioGearsEngine(Logger* logger);
  BioGearsEngine(const std::string&);
  ~BioGearsEngine() override;

  bool LoadState(const std::string& file, const SEScalarTime* simTime = nullptr) override;
  bool LoadState(const CDM::PhysiologyEngineStateData& state, const SEScalarTime* simTime = nullptr) override;
  std::unique_ptr<CDM::PhysiologyEngineStateData> SaveState(const std::string& file = "") override;

  Logger* GetLogger() override;
  PhysiologyEngineTrack* GetEngineTrack() override;

  bool InitializeEngine(const std::string& patientFile, const std::vector<const SECondition*>* conditions = nullptr, const PhysiologyEngineConfiguration* config = nullptr) override;
  bool InitializeEngine(const SEPatient& patient, const std::vector<const SECondition*>* conditions = nullptr, const PhysiologyEngineConfiguration* config = nullptr) override;

  const PhysiologyEngineConfiguration* GetConfiguration() override;

  double GetTimeStep(const TimeUnit& unit) override;
  double GetSimulationTime(const TimeUnit& unit) override;

  void AdvanceModelTime() override;
  void AdvanceModelTime(double time, const TimeUnit& unit) override;
  bool ProcessAction(const SEAction& action) override;

  SESubstanceManager& GetSubstanceManager() override;
  void SetEventHandler(SEEventHandler* handler) override;
  const SEPatient& GetPatient() override;
  bool GetPatientAssessment(SEPatientAssessment& assessment) override;

  const SEEnvironment* GetEnvironment() override;
  const SEBloodChemistrySystem* GetBloodChemistrySystem() override;
  const SECardiovascularSystem* GetCardiovascularSystem() override;
  const SEDrugSystem* GetDrugSystem() override;
  const SEEndocrineSystem* GetEndocrineSystem() override;
  const SEEnergySystem* GetEnergySystem() override;
  const SEGastrointestinalSystem* GetGastrointestinalSystem() override;
  const SEHepaticSystem* GetHepaticSystem() override;
  const SENervousSystem* GetNervousSystem() override;
  const SERenalSystem* GetRenalSystem() override;
  const SERespiratorySystem* GetRespiratorySystem() override;
  const SETissueSystem* GetTissueSystem() override;
  const SEAnesthesiaMachine* GetAnesthesiaMachine() override;
  const SEElectroCardioGram* GetElectroCardioGram() override;
  const SEInhaler* GetInhaler() override;

  const SECompartmentManager& GetCompartments() override;
  Tree<std::string> GetDataRequestGraph () const override;

protected:
  bool IsReady();
  bool InitializeEngine(const std::vector<const SECondition*>* conditions = nullptr, const PhysiologyEngineConfiguration* config = nullptr);

  SEEventHandler* m_EventHandler;
  PhysiologyEngineTrack m_EngineTrack;
  std::stringstream m_ss;
};
}