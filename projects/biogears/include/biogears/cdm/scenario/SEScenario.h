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
#include <biogears/cdm/scenario/requests/SEDataRequestManager.h>
#include <biogears/exports.h>
#include <biogears/schema/cdm/Scenario.hxx>

namespace biogears {
class SESubstanceManager;
class SEAction;
class SEDataRequest;
class SEScenarioInitialParameters;
class SEScenarioAutoSerialization;
class SEDecimalFormat;

class BIOGEARS_API SEScenario : public Loggable {
public:
  SEScenario(SESubstanceManager& subMgr);

  virtual ~SEScenario();

  virtual void Clear(); //clear memory

  bool Load(const CDM::ScenarioData& in);
  CDM::ScenarioData* Unload() const;

protected:
  void Unload(CDM::ScenarioData& data) const;

public:
  bool Load(const char* scenarioFile);
  bool Load(const std::string& scenarioFile);
  bool IsValid() const;

  virtual std::string GetName() const;
  virtual const char* GetName_cStr() const;
  virtual void SetName(const char* name);
  virtual void SetName(const std::string& name);
  virtual bool HasName() const;
  virtual void InvalidateName();

  virtual const char* GetPatientFile() const;
  virtual void SetPatientFile(const char* PatientFile);
  virtual void SetPatientFile(const std::string& PatientFile);
  virtual bool HasPatientFile() const;
  virtual void InvalidatePatientFile();

  virtual const char* GetDescription() const;
  virtual void SetDescription(const char* desc);
  virtual void SetDescription(const std::string& desc);
  virtual bool HasDescription() const;
  virtual void InvalidateDescription();

  virtual const char* GetEngineStateFile() const;
  virtual void SetEngineStateFile(const char* file);
  virtual void SetEngineStateFile(const std::string& file);
  virtual bool HasEngineStateFile() const;
  virtual void InvalidateEngineStateFile();

  virtual SEScenarioInitialParameters& GetInitialParameters();
  virtual const SEScenarioInitialParameters* GetInitialParameters() const;
  virtual bool HasInitialParameters() const;
  virtual void InvalidateInitialParameters();

  virtual bool HasAutoSerialization() const;
  virtual SEScenarioAutoSerialization& GetAutoSerialization();
  virtual const SEScenarioAutoSerialization* GetAutoSerialization() const;
  virtual void RemoveAutoSerialization();

  virtual void AddAction(const SEAction& action);
  virtual void ClearActions();
  virtual bool AddActionAfter(const SEAction& reference, const SEAction& action);
  virtual const std::vector<SEAction*>& GetActions() const;

  virtual SEDataRequestManager& GetDataRequestManager() { return m_DataRequestMgr; }
  virtual const SEDataRequestManager& GetDataRequestManager() const { return m_DataRequestMgr; }

protected:
  SESubstanceManager& m_SubMgr;
  std::string m_Name;
  std::string m_Description;
  std::string m_EngineStateFile;
  std::string m_PatientFile;
  std::string m_workingDir;
  SEScenarioInitialParameters* m_InitialParameters;
  SEScenarioAutoSerialization* m_AutoSerialization;
  SEDataRequestManager m_DataRequestMgr;
  std::vector<SEAction*> m_Actions;
};
}