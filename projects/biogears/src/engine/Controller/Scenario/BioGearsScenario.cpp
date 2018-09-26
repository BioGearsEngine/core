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
#include <biogears/engine/Controller/Scenario/BioGearsScenario.h>

#include <biogears/engine/Controller/BioGears.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
namespace BGE = mil::tatrc::physiology::biogears;

namespace biogears {
BioGearsScenario::BioGearsScenario(SESubstanceManager& subMgr)
  : SEScenario(subMgr)
{
}

BioGearsScenario::~BioGearsScenario()
{
}

BioGearsScenarioInitialParameters& BioGearsScenario::GetInitialParameters()
{
  InvalidateEngineStateFile();
  if (m_InitialParameters == nullptr)
    m_InitialParameters = new BioGearsScenarioInitialParameters(m_SubMgr);
  return *((BioGearsScenarioInitialParameters*)m_InitialParameters);
}
const BioGearsScenarioInitialParameters* BioGearsScenario::GetInitialParameters() const
{
  return (BioGearsScenarioInitialParameters*)m_InitialParameters;
}
}