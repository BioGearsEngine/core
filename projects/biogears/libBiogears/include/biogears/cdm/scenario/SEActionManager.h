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
#include <biogears/cdm/scenario/SEAnesthesiaMachineActionCollection.h>
#include <biogears/cdm/scenario/SEEnvironmentActionCollection.h>
#include <biogears/cdm/scenario/SEInhalerActionCollection.h>
#include <biogears/cdm/scenario/SEPatientActionCollection.h>

namespace biogears {
class SESubstanceManager;
class SEAction;

class BIOGEARS_API SEActionManager : public Loggable {
public:
  SEActionManager(SESubstanceManager&);
  ~SEActionManager();

  void Clear();

  void Unload(std::vector<CDM::ActionData*>& to);

  bool ProcessAction(const SEAction& action);
  bool ProcessAction(const CDM::ActionData& in);

  SEEnvironmentActionCollection& GetEnvironmentActions() { return m_EnvironmentActions; }
  SEPatientActionCollection& GetPatientActions() { return m_PatientActions; }
  SEAnesthesiaMachineActionCollection& GetAnesthesiaMachineActions() { return m_AnesthesiaMachineActions; }
  SEInhalerActionCollection& GetInhalerActions() { return m_InhalerActions; }

protected:
  SESubstanceManager& m_Substances;
  SEEnvironmentActionCollection m_EnvironmentActions;
  SEPatientActionCollection m_PatientActions;
  SEAnesthesiaMachineActionCollection m_AnesthesiaMachineActions;
  SEInhalerActionCollection m_InhalerActions;

  std::vector<CDM::ActionData*> m_ProcessedActions;

  std::stringstream m_ss;
};
}