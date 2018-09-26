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

void SEInhalerActionCollection::Unload(std::vector<CDM::ActionData*>& to)
{
  if (HasConfiguration())
    to.push_back(GetConfiguration()->Unload());
}

bool SEInhalerActionCollection::ProcessAction(const SEInhalerAction& action)
{
  if (!IsValid(action))
    return false;
  CDM::InhalerActionData* bind = action.Unload();
  bool b = ProcessAction(*bind);
  delete bind;
  return b;
}

bool SEInhalerActionCollection::ProcessAction(const CDM::InhalerActionData& action)
{
  const CDM::InhalerConfigurationData* config = dynamic_cast<const CDM::InhalerConfigurationData*>(&action);
  if (config != nullptr) {
    if (m_Configuration == nullptr)
      m_Configuration = new SEInhalerConfiguration(m_Substances);
    m_Configuration->Load(*config);
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