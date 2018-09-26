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
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/environment/actions/SEEnvironmentChange.h>
#include <biogears/cdm/system/environment/actions/SEThermalApplication.h>
#include <biogears/schema/cdm/EnvironmentActions.hxx>

namespace biogears {
class BIOGEARS_API SEEnvironmentActionCollection : public Loggable {
public:
  SEEnvironmentActionCollection(SESubstanceManager&);
  ~SEEnvironmentActionCollection();

  void Clear();

  void Unload(std::vector<CDM::ActionData*>& to);

  bool ProcessAction(const SEEnvironmentAction& action);
  bool ProcessAction(const CDM::EnvironmentActionData& action);

  // STATE ACTION
  bool HasChange() const;
  SEEnvironmentChange* GetChange() const;
  void RemoveChange();

  bool HasThermalApplication() const;
  SEThermalApplication* GetThermalApplication() const;
  void RemoveThermalApplication();

protected:
  bool IsValid(const SEEnvironmentAction& action);

  SEEnvironmentChange* m_Change;
  SEThermalApplication* m_ThermalApplication;
  // General
  SESubstanceManager& m_Substances;
  std::stringstream m_ss;
};
}