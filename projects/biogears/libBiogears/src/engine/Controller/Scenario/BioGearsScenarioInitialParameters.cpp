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

#include <biogears/engine/Controller/Scenario/BioGearsScenarioInitialParameters.h>

#include <biogears/engine/Controller/BioGears.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>

namespace BGE = mil::tatrc::physiology::biogears;

namespace biogears {
BioGearsScenarioInitialParameters::BioGearsScenarioInitialParameters(SESubstanceManager& subMgr)
  : SEScenarioInitialParameters(subMgr)
{
}

BioGearsScenarioInitialParameters::~BioGearsScenarioInitialParameters()
{
}

BioGearsConfiguration& BioGearsScenarioInitialParameters::GetConfiguration()
{
  if (m_Configuration == nullptr)
    m_Configuration = new BioGearsConfiguration(m_SubMgr);
  return *((BioGearsConfiguration*)(m_Configuration));
}
const BioGearsConfiguration* BioGearsScenarioInitialParameters::GetConfiguration() const
{
  return (BioGearsConfiguration*)m_Configuration;
}
void BioGearsScenarioInitialParameters::SetConfiguration(const BioGearsConfiguration& config)
{
  CDM_COPY((&config), (&GetConfiguration()));
}
}