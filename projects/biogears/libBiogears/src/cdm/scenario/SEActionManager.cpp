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
#include "io/cdm/PatientActions.h"
#include "io/cdm/EnvironmentActions.h"
#include "io/cdm/AnesthesiaActions.h"
#include "io/cdm/InhalerActions.h"

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
}

//-------------------------------------------------------------------------------
bool SEActionManager::ProcessAction(const SEAction& action, const PhysiologyEngine& engine)
{
  // Store the action data. This is intended to be able to
  // Serialize out all the actions that the engine was asked to perform
 
  if (auto aData = dynamic_cast<const SEPatientAction*>(&action))
    return m_PatientActions.ProcessAction(*aData, engine);

  if (auto aData = dynamic_cast<const SEAnesthesiaMachineAction*>(&action))
    return m_AnesthesiaMachineActions.ProcessAction(*aData, engine);

  if (auto aData = dynamic_cast<const SEEnvironmentAction*>(&action))
    return m_EnvironmentActions.ProcessAction(*aData, engine);

  if (auto aData = dynamic_cast<const SEInhalerAction*>(&action))
    return m_InhalerActions.ProcessAction(*aData, engine);

  /// \error Unsupported Action
  Error("Unsupported Action");
  return false;
}
//-------------------------------------------------------------------------------
}