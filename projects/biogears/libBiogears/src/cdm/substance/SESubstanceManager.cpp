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

#include <algorithm>

#include "io/cdm/Substance.h"

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
  DELETE_CONTAINER_OF_POINTERS(m_Substances);
  DELETE_CONTAINER_OF_POINTERS(m_Compounds);
  m_ActiveSubstances.clear();
  m_ActiveCompounds.clear();
  m_ActiveGases.clear();
  m_ActiveLiquids.clear();

  m_OriginalCompoundData.clear();
  m_OriginalSubstanceData.clear();
}
//-----------------------------------------------------------------------------
void SESubstanceManager::Reset()
{
  m_ActiveCompounds.clear();
  m_ActiveSubstances.clear();
  m_ActiveGases.clear();
  m_ActiveLiquids.clear();
  for (auto& [key, definition]: m_OriginalSubstanceData) {
    key->Clear();
    key->m_def = definition;
  }
  for (auto& [key, definition]: m_OriginalCompoundData) {
    *key = definition;
  }
}
//-----------------------------------------------------------------------------

/**
 * TRANSFERS OWNERSHIP TO THE MANAGER
 */
void SESubstanceManager::AddSubstance(SESubstance& substance)
{
  m_Substances.insert(&substance);
}
SESubstance* SESubstanceManager::AddSubstance(SESubstanceDefinition const& definition)
{
  auto existingSubstance = std::find_if(m_Substances.begin(), m_Substances.end(), [&definition](auto substance) { return substance->m_def == definition; });
  if (existingSubstance == m_Substances.end()) {
    auto newSubstance = std::make_unique<SESubstance>(definition).release();
    m_Substances.insert(newSubstance);
    return newSubstance;
  }
  return *existingSubstance;
}
//-----------------------------------------------------------------------------
SESubstance* SESubstanceManager::GetSubstance(const char* name) const
{
  return GetSubstance(std::string { name });
}
//-----------------------------------------------------------------------------
SESubstance* SESubstanceManager::GetSubstance(const std::string& name) const
{
  auto existingSubstance = std::find_if(m_Substances.begin(), m_Substances.end(), [&name](auto substance) { return substance->m_def.Name == name; });
  return (existingSubstance == m_Substances.end()) ? nullptr : *existingSubstance;
}
//-----------------------------------------------------------------------------
SESubstance* SESubstanceManager::GetSubstance(const SESubstanceDefinition& definition) const
{
  auto existingSubstance = std::find_if(m_Substances.begin(), m_Substances.end(), [&definition](auto substance) { return substance->m_def == definition; });
  return (existingSubstance == m_Substances.end()) ? nullptr : *existingSubstance;
}
//-----------------------------------------------------------------------------
const std::set<SESubstance*>& SESubstanceManager::GetSubstances() const
{
  return m_Substances;
}
//-----------------------------------------------------------------------------
bool SESubstanceManager::IsActive(const SESubstance& substance) const
{
  return m_ActiveSubstances.contains(const_cast<SESubstance*>(&substance));
}
//-----------------------------------------------------------------------------
const std::set<SESubstance*>& SESubstanceManager::GetActiveSubstances() const
{
  return m_ActiveSubstances;
}
//-----------------------------------------------------------------------------
void SESubstanceManager::AddActiveSubstance(SESubstance& substance)
{
  if (IsActive(substance))
    return;
  if (substance.GetState() == SESubstanceState::Gas) {
    m_ActiveGases.insert(&substance);
  }
  if (substance.GetState() == SESubstanceState::Liquid) {
    m_ActiveLiquids.insert(&substance);
  }
  if (substance.HasPK()) {
    m_ActiveDrugs.insert(&substance);
  }
  m_ActiveSubstances.insert(&substance);
}
//-----------------------------------------------------------------------------
SESubstance* SESubstanceManager::AddActiveSubstance(SESubstanceDefinition const& definition)
{

  auto existingSubstance = std::find_if(m_Substances.begin(), m_Substances.end(), [&definition](auto substance) { return substance->m_def == definition; });
  SESubstance* activeSubstance = nullptr;
  if (existingSubstance == m_Substances.end()) {
    activeSubstance = std::make_unique<SESubstance>(definition).release();
    m_Substances.insert(activeSubstance);
  } else {
    activeSubstance = *existingSubstance;
  }

  if (IsActive(*activeSubstance))
    return activeSubstance;

  if (activeSubstance->GetState() == SESubstanceState::Gas) {
    m_ActiveGases.insert(activeSubstance);
  }
  if (activeSubstance->GetState() == SESubstanceState::Liquid) {
    m_ActiveLiquids.insert(activeSubstance);
  }
  if (activeSubstance->HasPK()) {
    m_ActiveDrugs.insert(activeSubstance);
  }
  m_ActiveSubstances.insert(activeSubstance);

  return activeSubstance;
}
//-----------------------------------------------------------------------------
void SESubstanceManager::RemoveActiveSubstance(const SESubstance& substance)
{
  std::erase_if(m_ActiveSubstances, [&substance](auto lhs) { return *lhs == substance; });
  std::erase_if(m_ActiveGases, [&substance](auto lhs) { return *lhs == substance; });
  std::erase_if(m_ActiveLiquids, [&substance](auto lhs) { return *lhs == substance; });
  std::erase_if(m_ActiveSubstances, [&substance](auto lhs) { return *lhs == substance; });
}
//-----------------------------------------------------------------------------
void SESubstanceManager::RemoveActiveSubstances(const std::set<SESubstance*>& substances)
{
  for (SESubstance* sub : substances)
    RemoveActiveSubstance(*sub);
}
//-----------------------------------------------------------------------------
void SESubstanceManager::RemoveActiveSubstances()
{
  std::set<SESubstance*> copy(m_ActiveSubstances);
  for (SESubstance* sub : copy)
    RemoveActiveSubstance(*sub);
}
//-----------------------------------------------------------------------------
const std::set<SESubstance*>& SESubstanceManager::GetActiveGases() const
{
  return m_ActiveGases;
}
//-----------------------------------------------------------------------------
const std::set<SESubstance*>& SESubstanceManager::GetActiveLiquids() const
{
  return m_ActiveLiquids;
}
//-----------------------------------------------------------------------------
const std::set<SESubstance*>& SESubstanceManager::GetActiveDrugs() const
{
  return m_ActiveDrugs;
}
//-----------------------------------------------------------------------------
void SESubstanceManager::AddCompound(SESubstanceCompound& compound)
{
  m_Compounds.insert(&compound);
}
//-----------------------------------------------------------------------------
SESubstanceCompound* SESubstanceManager::GetCompound(const char* name) const
{
  return GetCompound(std::string { name });
}
//-----------------------------------------------------------------------------
SESubstanceCompound* SESubstanceManager::GetCompound(const std::string& name) const
{
  for (auto compound : m_Compounds) {
    if (name == compound->GetName())
      return compound;
  }
  return nullptr;
}
//-----------------------------------------------------------------------------
const std::set<SESubstanceCompound*>& SESubstanceManager::GetCompounds() const
{
  return m_Compounds;
}
//-----------------------------------------------------------------------------
bool SESubstanceManager::IsActive(const SESubstanceCompound& compound) const
{
  return m_ActiveCompounds.contains(const_cast<SESubstanceCompound*>(&compound));
}
//-----------------------------------------------------------------------------
const std::set<SESubstanceCompound*>& SESubstanceManager::GetActiveCompounds() const
{
  return m_ActiveCompounds;
}
//-----------------------------------------------------------------------------
void SESubstanceManager::AddActiveCompound(SESubstanceCompound& compound)
{
  m_ActiveCompounds.insert(&compound);
}
//-----------------------------------------------------------------------------
void SESubstanceManager::RemoveActiveCompound(SESubstanceCompound& compound)
{
  std::erase_if(m_ActiveCompounds, [&compound](auto lhs) { return *lhs == compound; });
}
//-----------------------------------------------------------------------------
void SESubstanceManager::RemoveActiveCompounds(const std::set<SESubstanceCompound*>& compounds)
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
    io::Substance::UnMarshall(*subData, *sub);
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
  std::map<std::string, std::unique_ptr<CDM::SubstanceCompoundData>> compoundDefinitions;

  std::string path_string;
  for (auto& filepath : io->FindAllSubstanceFiles()) {
    path_string = filepath;
#ifdef _DEBUG
    // Debugs(asprintf("Reading substance file : %s", path_string.c_str()));
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
  for (auto& [key, objectData] : definitions) {
    if (auto subData = dynamic_cast<CDM::SubstanceData*>(objectData.get())) {
      auto sub = std::make_unique<SESubstance>(GetLogger());
      io::Substance::UnMarshall(*subData, *sub);
      m_OriginalSubstanceData[sub.get()] = sub->GetDefinition();
      AddSubstance(*sub.release());
      continue;
    }
    if (auto compoundData = dynamic_cast<CDM::SubstanceCompoundData*>(objectData.get())) {
      compoundDefinitions[key] = std::unique_ptr<CDM::SubstanceCompoundData>(dynamic_cast<CDM::SubstanceCompoundData*>(objectData.release()));
      continue;
    }
    Error("Unknown Type");
    succeeded = false;
  }

  for (auto& pair : compoundDefinitions) {

    // Differ processing compounds, until after all substances are known
    // To prevent first pass errors.
    auto compound = std::make_unique<SESubstanceCompound>(GetLogger());
    io::Substance::UnMarshall(*pair.second, *this, *compound);
    m_OriginalCompoundData[compound.get()] = *compound;
    AddCompound(*compound.release());
    continue;
  }

  if (!succeeded) {
    ss << "Unable to read some substances definitions" << std::ends;
    Error(ss);
  }
  return succeeded;
}
//-----------------------------------------------------------------------------
bool SESubstanceManager::operator==(SESubstanceManager const& rhs) const
{

  if (this == &rhs)
    return true;

  // NOTE: This equality operator needs improvment
  //       We need to seperate SubstanceData from the SubstanceMGR
  //       We can then hash substance data and compare the hash
  //       Additionally we can likely only support SubstanceMgr comparrision by memory address and Data Definitions

  // TODO: Iterate over substances and call operator== on the data element. This just checks
  //       If the pointers are equal which is hardly true
  return (m_Substances == rhs.m_Substances
          && m_ActiveSubstances == rhs.m_ActiveSubstances
          && m_ActiveGases == rhs.m_ActiveGases
          && m_ActiveLiquids == rhs.m_ActiveLiquids
          && m_ActiveDrugs == rhs.m_ActiveDrugs
          && m_Compounds == rhs.m_Compounds
          && m_ActiveCompounds == rhs.m_ActiveCompounds
          && m_OriginalSubstanceData == rhs.m_OriginalSubstanceData
          && m_OriginalCompoundData == rhs.m_OriginalCompoundData);
}
//-----------------------------------------------------------------------------
bool SESubstanceManager::operator!=(SESubstanceManager const& rhs) const
{
  return !(*this == rhs);
}
//-----------------------------------------------------------------------------
}
