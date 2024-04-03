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
#include <biogears/cdm/scenario/SEScenarioAutoSerialization.h>

#include "io/cdm/Scenario.h"

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/engine/PhysiologyEngineConfiguration.h>
#include <biogears/cdm/scenario/SEPatientActionCollection.h>
#include <biogears/cdm/scenario/SEScenario.h>
#include <biogears/cdm/scenario/SEScenarioInitialParameters.h>
#include <biogears/io/io-manager.h>
#include <biogears/string/manipulation.h>

#include <algorithm>
#include <random>

namespace biogears {
SEScenario::SEScenario(SESubstanceManager& subMgr)
  : Loggable(subMgr.GetLogger())
  , m_SubMgr(subMgr)
  , m_DataRequestMgr(subMgr.GetLogger())
{
  m_InitialParameters = nullptr;
  m_AutoSerialization = nullptr;
  SEScenario::Clear();
}
//-----------------------------------------------------------------------------
SEScenario::~SEScenario()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEScenario::Clear()
{
  m_Name = "";
  m_Description = "";
  m_EngineStateFile = "";
  SAFE_DELETE(m_InitialParameters);
  SAFE_DELETE(m_AutoSerialization);
  DELETE_VECTOR(m_Actions);
  m_DataRequestMgr.Clear();
}
//-----------------------------------------------------------------------------
bool SEScenario::Load(const CDM::ScenarioData& in)
{
  io::Scenario::UnMarshall(in, *this);

  return IsValid();
}
//-----------------------------------------------------------------------------
bool SEScenario::Load(const CDM::ActionListData& in)
{
  m_Actions.clear();
  bool loadSuccess = true;
  for (unsigned int i = 0; i < in.Action().size(); i++) {
    SEAction* a = SEAction::newFromBind(in.Action()[i], m_SubMgr);
    if (a != nullptr) {
      m_Actions.push_back(a);
      loadSuccess &= a->IsValid();
    }
  }
  return IsValid();
}
//-----------------------------------------------------------------------------
CDM::ScenarioData* SEScenario::Unload() const
{
  CDM::ScenarioData* data = new CDM::ScenarioData();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEScenario::Unload(CDM::ScenarioData& data) const
{
  io::Scenario::Marshall(*this, data);
}
//-----------------------------------------------------------------------------
void SEScenario::Unload(CDM::ActionListData& data) const
{
  io::Scenario::Marshall(*this, data);
}
//-----------------------------------------------------------------------------
bool SEScenario::Load(const char* scenarioFile)
{
  return Load(std::string { scenarioFile });
}
//-----------------------------------------------------------------------------
bool SEScenario::Load(const std::string& scenarioFile)
{
  CDM::ScenarioData* pData;
  std::unique_ptr<CDM::ObjectData> data;

  auto io = m_Logger->GetIoManager().lock();
  auto possible_path = io->FindScenarioFile(scenarioFile.c_str());
  if (possible_path.empty()) {
#ifdef BIOGEARS_IO_PRESENT
    size_t content_size;
    auto content = io->get_embedded_resource_file(scenarioFile.c_str(), content_size);
    data = Serializer::ReadBuffer((XMLByte*)content, content_size, m_Logger);
#endif
  } else {
    data = Serializer::ReadFile(possible_path, m_Logger);
  }

  pData = dynamic_cast<CDM::ScenarioData*>(data.get());
  if (pData == nullptr) {
    std::stringstream ss;
    ss << "Scenario file could not be read : " << scenarioFile << std::endl;
    Error(ss);
    return false;
  }
  return Load(*pData);
}
//-----------------------------------------------------------------------------
bool SEScenario::IsValid() const
{
  if (HasInitialParameters()) {
    if (!m_InitialParameters->IsValid())
      return false;
  }
  if (m_Actions.size() == 0)
    return false;
  return true;
}
//-----------------------------------------------------------------------------
std::string SEScenario::GetName() const
{
  return m_Name;
}
//-----------------------------------------------------------------------------
const char* SEScenario::GetName_cStr() const
{
  return m_Name.c_str();
}
//-----------------------------------------------------------------------------
void SEScenario::SetName(const char* name)
{
  m_Name = name;
}
//-----------------------------------------------------------------------------
void SEScenario::SetName(const std::string& name)
{
  m_Name = name;
}
//-----------------------------------------------------------------------------
bool SEScenario::HasName() const
{
  return m_Name.empty() ? false : true;
}
//-----------------------------------------------------------------------------
void SEScenario::InvalidateName()
{
  m_Name = "";
}
//-----------------------------------------------------------------------------
const char* SEScenario::GetPatientFile() const
{
  return m_PatientFile.c_str();
}
//-----------------------------------------------------------------------------
void SEScenario::SetPatientFile(const char* PatientFile)
{
  m_PatientFile = PatientFile;
}
//-----------------------------------------------------------------------------
void SEScenario::SetPatientFile(const std::string& PatientFile)
{
  m_PatientFile = PatientFile;
}
//-----------------------------------------------------------------------------
bool SEScenario::HasPatientFile() const
{
  return m_PatientFile.empty() ? false : true;
}
//-----------------------------------------------------------------------------
void SEScenario::InvalidatePatientFile()
{
  m_PatientFile = "";
}
//-----------------------------------------------------------------------------

const char* SEScenario::GetActionFile() const
{
  return m_ActionFile.c_str();
}
//-----------------------------------------------------------------------------
void SEScenario::SetActionFile(const char* ActionFile)
{
  m_ActionFile = ActionFile;
}
//-----------------------------------------------------------------------------
void SEScenario::SetActionFile(const std::string& ActionFile)
{
  m_ActionFile = ActionFile;
}
//-----------------------------------------------------------------------------
bool SEScenario::HasActionFile() const
{
  return m_ActionFile.empty() ? false : true;
}
//-----------------------------------------------------------------------------
void SEScenario::InvalidateActionFile()
{
  m_ActionFile = "";
}
//-----------------------------------------------------------------------------
const char* SEScenario::GetDescription() const
{
  return m_Description.c_str();
}
//-----------------------------------------------------------------------------
void SEScenario::SetDescription(const char* desc)
{
  m_Description = desc;
}
//-----------------------------------------------------------------------------
void SEScenario::SetDescription(const std::string& desc)
{
  m_Description = desc;
}
//-----------------------------------------------------------------------------
bool SEScenario::HasDescription() const
{
  return m_Description.empty() ? false : true;
}
//-----------------------------------------------------------------------------
void SEScenario::InvalidateDescription()
{
  m_Description = "";
}

//-----------------------------------------------------------------------------
const char* SEScenario::GetEngineStateFile() const
{
  return m_EngineStateFile.c_str();
}
//-----------------------------------------------------------------------------
void SEScenario::SetEngineStateFile(const char* file)
{
  InvalidateInitialParameters();
  m_EngineStateFile = file;
}
//-----------------------------------------------------------------------------
void SEScenario::SetEngineStateFile(const std::string& file)
{
  InvalidateInitialParameters();
  m_EngineStateFile = file;
}
//-----------------------------------------------------------------------------
bool SEScenario::HasEngineStateFile() const
{
  return m_EngineStateFile.empty() ? false : true;
}
//-----------------------------------------------------------------------------
void SEScenario::InvalidateEngineStateFile()
{
  m_EngineStateFile = "";
}
//-----------------------------------------------------------------------------
SEScenarioInitialParameters& SEScenario::GetInitialParameters()
{
  InvalidateEngineStateFile();
  if (m_InitialParameters == nullptr)
    m_InitialParameters = new SEScenarioInitialParameters(m_SubMgr);
  return *m_InitialParameters;
}
//-----------------------------------------------------------------------------
const SEScenarioInitialParameters* SEScenario::GetInitialParameters() const
{
  return m_InitialParameters;
}
//-----------------------------------------------------------------------------
bool SEScenario::HasInitialParameters() const
{
  return m_InitialParameters != nullptr;
}
//-----------------------------------------------------------------------------
void SEScenario::InvalidateInitialParameters()
{
  SAFE_DELETE(m_InitialParameters);
}
//-----------------------------------------------------------------------------
bool SEScenario::HasAutoSerialization() const
{
  return m_AutoSerialization == nullptr ? false : m_AutoSerialization->IsValid();
}
//-----------------------------------------------------------------------------
SEScenarioAutoSerialization& SEScenario::GetAutoSerialization()
{
  if (m_AutoSerialization == nullptr)
    m_AutoSerialization = new SEScenarioAutoSerialization(GetLogger());
  return *m_AutoSerialization;
}
//-----------------------------------------------------------------------------
const SEScenarioAutoSerialization* SEScenario::GetAutoSerialization() const
{
  return m_AutoSerialization;
}
//-----------------------------------------------------------------------------
void SEScenario::RemoveAutoSerialization()
{
  SAFE_DELETE(m_AutoSerialization);
}
//-----------------------------------------------------------------------------
void SEScenario::AddAction(const SEAction& a)
{
  CDM::ActionData* bind = a.Unload();
  m_Actions.push_back(SEAction::newFromBind(*bind, m_SubMgr));
  delete bind;
}
//-----------------------------------------------------------------------------
bool SEScenario::AddActionAfter(const SEAction& ref, const SEAction& after)
{
  bool success = false;
  auto refItr = std::find(m_Actions.begin(), m_Actions.end(), &ref);
  if (refItr != m_Actions.end()) {
    CDM::ActionData* bind = after.Unload();
    m_Actions.insert(refItr, SEAction::newFromBind(*bind, m_SubMgr));
    delete bind;
    success = true;
  }
  return success;
}
//-----------------------------------------------------------------------------
void SEScenario::ClearActions()
{
  DELETE_VECTOR(m_Actions);
}
//-----------------------------------------------------------------------------
const std::vector<SEAction*>& SEScenario::GetActions() const
{
  return m_Actions;
}
//-----------------------------------------------------------------------------
#pragma optimize("", off)
bool SEScenario::operator==(SEScenario const& rhs) const
{
  if (this == &rhs)
    return true;

  bool equivilant = &m_SubMgr == &rhs.m_SubMgr;
  equivilant &= m_Name == rhs.m_Name;
  equivilant &= m_Description == rhs.m_Description;
  equivilant &= m_EngineStateFile == rhs.m_EngineStateFile;
  equivilant &= ((m_InitialParameters && rhs.m_InitialParameters) ? m_InitialParameters->operator==(*rhs.m_InitialParameters) : m_InitialParameters == rhs.m_InitialParameters);
  equivilant &= ((m_AutoSerialization && rhs.m_AutoSerialization) ? m_AutoSerialization->operator==(*rhs.m_AutoSerialization) : m_AutoSerialization == rhs.m_AutoSerialization);
  equivilant &= m_DataRequestMgr.operator==(rhs.m_DataRequestMgr);

  // NOTE: SLOW_COMPARISON
  //  Ok, This is going to be really ugly if we are actually comparing polymorphic downclasses
  //       Our ToString implementation caches, but its still uglier then other things.
  //       We can likely vTable lookup operator== against SEAction const& and then return false
  //       if dynamic cast fails else proceed for a faster runtimes. This pattern
  //       is all over Biogears so we need to investigate teh cost. That said we only
  //       do equality comparisons in unittest really
  if (equivilant && m_Actions.size() == rhs.m_Actions.size()) {
    for (auto idx = 0; idx < m_Actions.size(); ++idx) {
      if (0 == strcmp(m_Actions[idx]->classname(), rhs.m_Actions[idx]->classname())) {
        equivilant &= 0 == strcmp(m_Actions[idx]->ToString(), rhs.m_Actions[idx]->ToString());
      }
    }
  } else {
    equivilant = false;
  }
  return equivilant;
}
#pragma optimize("", on)
bool SEScenario::operator!=(SEScenario const& rhs) const
{
  return !(*this == rhs);
}
//-----------------------------------------------------------------------------
}