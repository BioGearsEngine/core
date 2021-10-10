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
#include <biogears/cdm/substance/SESubstanceManager.h>

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/utils/unitconversion/UnitConversionEngine.h>

#include <biogears/filesystem/path.h>
#include <biogears/string/manipulation.h>

#include <biogears/io/io-manager.h>

#ifdef BIOGEARS_IO_PRESENT
#include <biogears/io/directories/substances.h>
#endif

namespace std {
template class map<biogears::SESubstance*, const CDM::SubstanceData*>;
template class map<biogears::SESubstanceCompound*, const CDM::SubstanceCompoundData*>;
}

namespace biogears {
SESubstanceManager::SESubstanceManager(Logger* logger)
  : Loggable(logger)
{
  Clear();
}
//-----------------------------------------------------------------------------
SESubstanceManager::~SESubstanceManager()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SESubstanceManager::Clear()
{
  DELETE_VECTOR(m_Substances);
  DELETE_VECTOR(m_Compounds);
  m_ActiveSubstances.clear();
  m_ActiveCompounds.clear();
  m_ActiveGases.clear();
  m_ActiveLiquids.clear();

  DELETE_MAP_SECOND(m_OriginalCompoundData);
  DELETE_MAP_SECOND(m_OriginalSubstanceData);
}
//-----------------------------------------------------------------------------
void SESubstanceManager::Reset()
{
  m_ActiveCompounds.clear();
  m_ActiveSubstances.clear();
  m_ActiveGases.clear();
  m_ActiveLiquids.clear();
  for (auto itr : m_OriginalSubstanceData)
    itr.first->Load(*itr.second);
  for (auto itr : m_OriginalCompoundData)
    itr.first->Load(*itr.second, *this);
}
//-----------------------------------------------------------------------------
/**
 * TRANSFERS OWNERSHIP TO THE MANAGER
 */
void SESubstanceManager::AddSubstance(SESubstance& substance)
{
  SESubstance* s;
  for (unsigned int i = 0; i < m_Substances.size(); i++) {
    s = m_Substances.at(i);
    if (s == &substance)
      return;
  }
  m_Substances.push_back(&substance);
}
//-----------------------------------------------------------------------------
SESubstance* SESubstanceManager::GetSubstance(const char* name) const
{
  return GetSubstance(std::string { name });
}
//-----------------------------------------------------------------------------
SESubstance* SESubstanceManager::GetSubstance(const std::string& name) const
{
  SESubstance* s;
  for (unsigned int i = 0; i < m_Substances.size(); i++) {
    s = m_Substances.at(i);
    if (name.compare(s->GetName()) == 0)
      return s;
  }
  return nullptr;
}
//-----------------------------------------------------------------------------
const std::vector<SESubstance*>& SESubstanceManager::GetSubstances() const
{
  return m_Substances;
}
//-----------------------------------------------------------------------------
bool SESubstanceManager::IsActive(const SESubstance& substance) const
{
  SESubstance* sub;
  for (unsigned int iSubstance = 0; iSubstance < m_ActiveSubstances.size(); iSubstance++) {
    sub = m_ActiveSubstances.at(iSubstance);
    if (sub == &substance)
      return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
const std::vector<SESubstance*>& SESubstanceManager::GetActiveSubstances() const
{
  return m_ActiveSubstances;
}
//-----------------------------------------------------------------------------
void SESubstanceManager::AddActiveSubstance(SESubstance& substance)
{
  if (IsActive(substance))
    return;
  if (substance.GetState() == CDM::enumSubstanceState::Gas) {
    m_ActiveGases.push_back(&substance);
  }
  if (substance.GetState() == CDM::enumSubstanceState::Liquid) {
    m_ActiveLiquids.push_back(&substance);
  }
  if (substance.HasPK()) {
    m_ActiveDrugs.push_back(&substance);
  }
  m_ActiveSubstances.push_back(&substance);
}
//-----------------------------------------------------------------------------
void SESubstanceManager::RemoveActiveSubstance(const SESubstance& substance)
{
  SESubstance* sub;
  for (unsigned int iSubstance = 0; iSubstance < m_ActiveSubstances.size(); iSubstance++) {
    sub = m_ActiveSubstances.at(iSubstance);
    if (sub == &substance) {
      m_ActiveSubstances.erase(m_ActiveSubstances.begin() + iSubstance);
      break;
    }
  }
  for (unsigned int iSubstance = 0; iSubstance < m_ActiveGases.size(); iSubstance++) {
    sub = m_ActiveGases.at(iSubstance);
    if (sub == &substance) {
      m_ActiveGases.erase(m_ActiveGases.begin() + iSubstance);
      break;
    }
  }
  for (unsigned int iSubstance = 0; iSubstance < m_ActiveLiquids.size(); iSubstance++) {
    sub = m_ActiveLiquids.at(iSubstance);
    if (sub == &substance) {
      m_ActiveLiquids.erase(m_ActiveLiquids.begin() + iSubstance);
      break;
    }
  }

  for (unsigned int iSubstance = 0; iSubstance < m_ActiveDrugs.size(); iSubstance++) {
    sub = m_ActiveDrugs.at(iSubstance);
    if (sub == &substance) {
      m_ActiveDrugs.erase(m_ActiveDrugs.begin() + iSubstance);
      break;
    }
  }
}
//-----------------------------------------------------------------------------
void SESubstanceManager::RemoveActiveSubstances(const std::vector<SESubstance*>& substances)
{
  for (SESubstance* sub : substances)
    RemoveActiveSubstance(*sub);
}
//-----------------------------------------------------------------------------
void SESubstanceManager::RemoveActiveSubstances()
{
  std::vector<SESubstance*> copy(m_ActiveSubstances);
  for (SESubstance* sub : copy)
    RemoveActiveSubstance(*sub);
}
//-----------------------------------------------------------------------------
const std::vector<SESubstance*>& SESubstanceManager::GetActiveGases() const
{
  return m_ActiveGases;
}
//-----------------------------------------------------------------------------
const std::vector<SESubstance*>& SESubstanceManager::GetActiveLiquids() const
{
  return m_ActiveLiquids;
}
//-----------------------------------------------------------------------------
const std::vector<SESubstance*>& SESubstanceManager::GetActiveDrugs() const
{
  return m_ActiveDrugs;
}
//-----------------------------------------------------------------------------
void SESubstanceManager::AddCompound(SESubstanceCompound& compound)
{
  SESubstanceCompound* c;
  for (unsigned int i = 0; i < m_Compounds.size(); i++) {
    c = m_Compounds.at(i);
    if (c == &compound)
      return;
  }
  m_Compounds.push_back(&compound);
}
//-----------------------------------------------------------------------------
SESubstanceCompound* SESubstanceManager::GetCompound(const char* name) const
{
  return GetCompound(std::string { name });
}
//-----------------------------------------------------------------------------
SESubstanceCompound* SESubstanceManager::GetCompound(const std::string& name) const
{
  SESubstanceCompound* c;
  for (unsigned int i = 0; i < m_Compounds.size(); i++) {
    c = m_Compounds.at(i);
    if (name.compare(c->GetName()) == 0)
      return c;
  }
  return nullptr;
}
//-----------------------------------------------------------------------------
const std::vector<SESubstanceCompound*>& SESubstanceManager::GetCompounds() const
{
  return m_Compounds;
}
//-----------------------------------------------------------------------------
bool SESubstanceManager::IsActive(const SESubstanceCompound& compound) const
{
  SESubstanceCompound* c;
  for (unsigned int i = 0; i < m_ActiveCompounds.size(); i++) {
    c = m_ActiveCompounds.at(i);
    if (c == &compound)
      return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
const std::vector<SESubstanceCompound*>& SESubstanceManager::GetActiveCompounds() const
{
  return m_ActiveCompounds;
}
//-----------------------------------------------------------------------------
void SESubstanceManager::AddActiveCompound(SESubstanceCompound& compound)
{
  SESubstanceCompound* c;
  for (unsigned int i = 0; i < m_ActiveCompounds.size(); i++) {
    c = m_ActiveCompounds.at(i);
    if (c == &compound)
      return;
  }
  m_ActiveCompounds.push_back(&compound);
}
//-----------------------------------------------------------------------------
void SESubstanceManager::RemoveActiveCompound(SESubstanceCompound& compound)
{
  unsigned int i = 0;
  for (SESubstanceCompound* c : m_ActiveCompounds) {
    if (c == &compound) {
      m_ActiveCompounds.erase(m_ActiveCompounds.begin() + i);
      break;
    }
    i++;
  }
}
//-----------------------------------------------------------------------------
void SESubstanceManager::RemoveActiveCompounds(const std::vector<SESubstanceCompound*>& compounds)
{
  for (SESubstanceCompound* c : compounds)
    RemoveActiveCompound(*c);
}
//-----------------------------------------------------------------------------
SESubstance* SESubstanceManager::ReadSubstanceFile(const char* xmlFile)
{
  return ReadSubstanceFile(std::string { xmlFile });
}
//-----------------------------------------------------------------------------
SESubstance* SESubstanceManager::ReadSubstanceFile(const std::string& xmlFile)
{
  std::stringstream ss;
  CDM::ObjectData* obj;

  SESubstance* sub;
  CDM::SubstanceData* subData;

  std::unique_ptr<CDM::ObjectData> data;

  obj = nullptr;
  sub = nullptr;
  subData = nullptr;

  data = Serializer::ReadFile(xmlFile, GetLogger());
  obj = data.get();

  subData = dynamic_cast<CDM::SubstanceData*>(obj);
  if (subData != nullptr) {
    sub = new SESubstance(GetLogger());
    sub->Load(*subData);
    return sub;
  }
  ss.str("");
  /// \error Substance file could not be read in
  ss << "Substance file could not be read in : ./substances/" << xmlFile;
  Error(ss.str(), "SESubstanceManager::ReadSubstanceFile");
  return nullptr;
}
//-----------------------------------------------------------------------------
bool SESubstanceManager::LoadSubstanceDirectory()
{
  bool succeeded = true;
  Clear();
  std::stringstream ss;

  auto io = m_Logger->GetIoManager().lock();

  std::map<std::string, std::unique_ptr<CDM::ObjectData>> definitions;

  std::string path_string;
  for (auto& filepath : io->FindAllSubstanceFiles()) {
    path_string = filepath;
#ifdef _DEBUG
    Debug(asprintf("Reading substance file : %s", path_string.c_str()));
#endif
    definitions[filepath.basename()] = Serializer::ReadFile(path_string, GetLogger());
  }
#ifdef BIOGEARS_IO_PRESENT
  auto substances = io::list_substances_files();
  auto substances_count = io::substances_file_count();

  std::vector<char> buffer;
  char const* substance = "";

  size_t content_size = 0;
  for (auto ii = 0; ii != substances_count; ++ii) {
    if (definitions.find(substances[ii]) == definitions.end()) {
      substance = substances[ii];
      content_size = io::get_embedded_substances_file_size(substance);
      buffer.resize(content_size);
      substance = io::get_embedded_substances_file(substance, content_size);
      definitions[substances[ii]] = Serializer::ReadBuffer((XMLByte*)substance, content_size, GetLogger());
    }
  }
#endif
  for (auto& pair : definitions) {

    auto subData = dynamic_cast<CDM::SubstanceData*>(pair.second.get());
    if (subData != nullptr) {
      auto sub = new SESubstance(GetLogger());
      sub->Load(*subData);
      AddSubstance(*sub);
      m_OriginalSubstanceData[sub] = subData;
      pair.second.release();
      continue;
    }
    auto compoundData = dynamic_cast<CDM::SubstanceCompoundData*>(pair.second.get());
    if (compoundData != nullptr) {
      // Differ processing compounds, until after all substances are known
      // To prevent first pass errors.
      auto compound = new SESubstanceCompound(GetLogger());
      m_OriginalCompoundData[compound] = compoundData;
      AddCompound(*compound);
      pair.second.release();
      continue;
    }
    Error("Unknown Type");
    succeeded = false;
  }
  for (auto itr : m_OriginalCompoundData)
    itr.first->Load((const CDM::SubstanceCompoundData&)*itr.second, *this);

  if (!succeeded) {
    ss << "Unable to read some substances definitions" << std::ends;
    Error(ss);
  }
  return succeeded;
}
//-----------------------------------------------------------------------------
}
