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

CDM_BIND_DECL(ScenarioInitialParametersData)

namespace biogears {
class SEScenario;
class SEPatient;
class SECondition;
class SESubstanceManager;
class PhysiologyEngineConfiguration;

class BIOGEARS_API SEScenarioInitialParameters : public Loggable {
protected:
  friend SEScenario;
  SEScenarioInitialParameters(SESubstanceManager& subMgr);
  virtual ~SEScenarioInitialParameters();
  

public:
  virtual void Clear(); //clear memory

  bool Load(const CDM::ScenarioInitialParametersData& in);
  CDM::ScenarioInitialParametersData* Unload() const;

protected:
  void Unload(CDM::ScenarioInitialParametersData& data) const;

public:
  virtual bool IsValid() const;

  virtual PhysiologyEngineConfiguration& GetConfiguration();
  virtual const PhysiologyEngineConfiguration* GetConfiguration() const;
  virtual void SetConfiguration(const PhysiologyEngineConfiguration& config);
  virtual bool HasConfiguration() const;
  virtual void InvalidateConfiguration();

  virtual const char* GetPatientFile() const;
  virtual void SetPatientFile(const char* patientFile);
  virtual void SetPatientFile(const std::string& patientFile);
  virtual bool HasPatientFile() const;
  virtual void InvalidatePatientFile();

  virtual SEPatient& GetPatient();
  virtual const SEPatient* GetPatient() const;
  virtual void SetPatient(const SEPatient& patient);
  virtual bool HasPatient() const;
  virtual void InvalidatePatient();

  virtual void AddCondition(const SECondition& c);
  virtual const std::vector<SECondition*>& GetConditions() const;

protected:
  SESubstanceManager& m_SubMgr;
  PhysiologyEngineConfiguration* m_Configuration;
  SEPatient* m_Patient;
  std::string m_PatientFile;
  std::vector<SECondition*> m_Conditions;

};
}