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
#include <biogears/cdm/scenario/SEScenarioExec.h>

#include <biogears/filesystem/path.h>

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/engine/PhysiologyEngine.h>
#include <biogears/cdm/engine/PhysiologyEngineConfiguration.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/scenario/SEAdvanceTime.h>
#include <biogears/cdm/scenario/SECondition.h>
#include <biogears/cdm/scenario/SEScenario.h>
#include <biogears/cdm/scenario/SEScenarioAutoSerialization.h>
#include <biogears/cdm/scenario/SEScenarioInitialParameters.h>
#include <biogears/cdm/utils/TimingProfile.h>
#include <biogears/io/io-manager.h>
#include <biogears/schema/cdm/Scenario.hxx>

namespace biogears {
SEScenarioExec::SEScenarioExec(PhysiologyEngine& engine)
  : Loggable(engine.GetLogger())
  , m_Engine(engine)
{
  m_Cancel = false;
  m_CustomExec = nullptr;
}
//-----------------------------------------------------------------------------
SEScenarioExec::~SEScenarioExec()
{
}
//-----------------------------------------------------------------------------
void SEScenarioExec::Cancel()
{
  m_Cancel = true;
}
//-----------------------------------------------------------------------------
}
