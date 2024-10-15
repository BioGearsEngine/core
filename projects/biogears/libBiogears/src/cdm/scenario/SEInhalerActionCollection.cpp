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

#include <biogears/cdm/scenario/SEInhalerActionCollection.h>

#include "io/cdm/Scenario.h"
#include "io/cdm/InhalerActions.h"

namespace biogears {
SEInhalerActionCollection::SEInhalerActionCollection(SESubstanceManager& substances)
  : Loggable(substances.GetLogger())
  , m_Substances(substances)
{
  m_Configuration = nullptr;
}

SEInhalerActionCollection::~SEInhalerActionCollection()
{
  Clear();
}

void SEInhalerActionCollection::Clear()
{
  // State
  RemoveConfiguration();
}

bool SEInhalerActionCollection::ProcessAction(const SEInhalerAction& action, const PhysiologyEngine& engine)
{

  if (auto config = dynamic_cast<SEInhalerConfiguration const*>(&action)) {
    if (m_Configuration == nullptr)
      m_Configuration = new SEInhalerConfiguration(m_Substances);
    io::InhalerActions::UnMarshall(*(CDM::InhalerConfigurationData*)(io::InhalerActions::factory(config).get()), *m_Configuration);
    return IsValid(*m_Configuration);
  }

  /// \error Unsupported Action
  Error("Unsupported Inhaler Action");
  return false;
}

bool SEInhalerActionCollection::IsValid(const SEInhalerAction& action)
{
  if (!action.IsValid()) {
    Error("Invalid Inhaler Action");
    return false;
  }
  return true;
}

bool SEInhalerActionCollection::HasConfiguration() const
{
  return m_Configuration == nullptr ? false : true;
}
SEInhalerConfiguration* SEInhalerActionCollection::GetConfiguration() const
{
  return m_Configuration;
}
void SEInhalerActionCollection::RemoveConfiguration()
{
  SAFE_DELETE(m_Configuration);
}
}