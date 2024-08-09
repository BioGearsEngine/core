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

#include <biogears/cdm/engine/PhysiologyEngine.h>
#include <biogears/cdm/scenario/SEActionManager.h>

#include "io/cdm/Scenario.h"
#include "io/cdm/Actions.h"

namespace biogears {
SEActionManager::SEActionManager(SESubstanceManager& substances)
  : Loggable(substances.GetLogger())
  , m_PatientActions(substances)
  , m_AnesthesiaMachineActions(substances)
  , m_EnvironmentActions(substances)
  , m_InhalerActions(substances)
  , m_Substances(substances)
{
}
//-------------------------------------------------------------------------------
SEActionManager::~SEActionManager()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEActionManager::Clear()
{
  m_PatientActions.Clear();
  m_AnesthesiaMachineActions.Clear();
  m_EnvironmentActions.Clear();
  m_InhalerActions.Clear();
  m_ProcessedActions.clear();
}
//-------------------------------------------------------------------------------
bool SEActionManager::ProcessAction(const CDM::ActionData& in, const PhysiologyEngine& engine)
{
  const CDM::PatientActionData* pAction = dynamic_cast<const CDM::PatientActionData*>(&in);
  if (pAction != nullptr)
    return m_PatientActions.ProcessAction(*pAction, engine);
  const CDM::EnvironmentActionData* eAction = dynamic_cast<const CDM::EnvironmentActionData*>(&in);
  if (eAction != nullptr)
    return m_EnvironmentActions.ProcessAction(*eAction, engine);
  const CDM::AnesthesiaMachineActionData* aAction = dynamic_cast<const CDM::AnesthesiaMachineActionData*>(&in);
  if (aAction != nullptr)
    return m_AnesthesiaMachineActions.ProcessAction(*aAction, engine);
  const CDM::InhalerActionData* iAction = dynamic_cast<const CDM::InhalerActionData*>(&in);
  if (iAction != nullptr)
    return m_InhalerActions.ProcessAction(*iAction, engine);
  Error("Unknown Action Type");
  return false;
}
//-------------------------------------------------------------------------------
void SEActionManager::Unload(std::vector<CDM::ActionData*>& to)
{
  m_PatientActions.Unload(to);
  m_AnesthesiaMachineActions.Unload(to);
  m_EnvironmentActions.Unload(to);
  m_InhalerActions.Unload(to);
}
//-------------------------------------------------------------------------------
bool SEActionManager::ProcessAction(const SEAction& action, const PhysiologyEngine& engine)
{
  // Store the action data. This is intended to be able to
  // Serialize out all the actions that the engine was asked to perform
  auto* aData = io::Actions::factory(&action).release();
  m_ProcessedActions.push_back(io::Actions::factory(&action));
 
  if (dynamic_cast<const SEPatientAction*>(&action) != nullptr)
    return m_PatientActions.ProcessAction(dynamic_cast<const CDM::PatientActionData&>(*aData), engine);

  if (dynamic_cast<const SEAnesthesiaMachineAction*>(&action) != nullptr)
    return m_AnesthesiaMachineActions.ProcessAction(dynamic_cast<const CDM::AnesthesiaMachineActionData&>(*aData), engine);

  if (dynamic_cast<const SEEnvironmentAction*>(&action) != nullptr)
    return m_EnvironmentActions.ProcessAction(dynamic_cast<const CDM::EnvironmentActionData&>(*aData), engine);

  if (dynamic_cast<const SEInhalerAction*>(&action) != nullptr)
    return m_InhalerActions.ProcessAction(dynamic_cast<const CDM::InhalerActionData&>(*aData), engine);

  /// \error Unsupported Action
  Error("Unsupported Action");
  return false;
}
//-------------------------------------------------------------------------------
}