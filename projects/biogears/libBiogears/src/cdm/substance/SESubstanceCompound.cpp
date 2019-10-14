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
#include <biogears/cdm/substance/SESubstanceCompound.h>

#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/cdm/substance/SESubstanceManager.h>

namespace biogears {
SESubstanceCompound::SESubstanceCompound(Logger* logger)
  : Loggable(logger)
{
  m_Name = "";
  m_Classification = (CDM::enumSubstanceClass::value)-1;
}
//-----------------------------------------------------------------------------
SESubstanceCompound::~SESubstanceCompound()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SESubstanceCompound::Clear()
{
  m_Name = "";
  m_Classification = (CDM::enumSubstanceClass::value)-1;
  DELETE_VECTOR(m_Components);
  m_cComponents.clear();
}
//-----------------------------------------------------------------------------
bool SESubstanceCompound::Load(const CDM::SubstanceCompoundData& in, const SESubstanceManager& subMgr)
{
  Clear();
  m_Name = in.Name();
  if (in.Classification().present())
    m_Classification = in.Classification().get();
  if (in.BloodRHFactor().present())
    m_RhFactor = in.BloodRHFactor().get();

  std::string err;

  SESubstance* substance = nullptr;
  SESubstanceConcentration* cc;
  CDM::SubstanceConcentrationData* ccData;
  for (unsigned int i = 0; i < in.Component().size(); i++) {
    ccData = (CDM::SubstanceConcentrationData*)&in.Component().at(i);
    substance = subMgr.GetSubstance(ccData->Name());
    if (substance == nullptr) {
      /// \error Could not load find substance compound component for specified substance
      err = "Could not load find substance compound component : ";
      err += ccData->Name();
      Error(err, "SESubstanceCompound::Load");
      return false;
    }
    cc = new SESubstanceConcentration(*substance);
    cc->Load(*ccData);
    m_Components.push_back(cc);
    m_cComponents.push_back(cc);
  }
  return true;
}
//-----------------------------------------------------------------------------
CDM::SubstanceCompoundData* SESubstanceCompound::Unload() const
{
  CDM::SubstanceCompoundData* data = new CDM::SubstanceCompoundData();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SESubstanceCompound::Unload(CDM::SubstanceCompoundData& data) const
{
  if (HasName())
    data.Name(m_Name);
  if (HasClassification())
    data.Classification(m_Classification);
  if (HasRhFactor())
    data.BloodRHFactor(m_RhFactor);

  for (unsigned int i = 0; i < m_Components.size(); i++) {
    data.Component().push_back(*m_Components.at(i)->Unload());
  }
};
//-----------------------------------------------------------------------------
std::string SESubstanceCompound::GetName() const
{
  return m_Name;
}
//-----------------------------------------------------------------------------
const char* SESubstanceCompound::GetName_cStr() const
{
  return m_Name.c_str();
}
//-----------------------------------------------------------------------------
void SESubstanceCompound::SetName(const char* name)
{
  m_Name = name;
}
//-----------------------------------------------------------------------------
void SESubstanceCompound::SetName(const std::string& name)
{
  m_Name = name;
}
//-----------------------------------------------------------------------------
bool SESubstanceCompound::HasName() const
{
  return m_Name.empty() ? false : true;
}
//-----------------------------------------------------------------------------
void SESubstanceCompound::InvalidateName()
{
  m_Name = "";
}
//-----------------------------------------------------------------------------
CDM::enumSubstanceClass::value SESubstanceCompound::GetClassification() const
{
  return m_Classification;
}
//-----------------------------------------------------------------------------
void SESubstanceCompound::SetClassification(CDM::enumSubstanceClass::value subClass)
{
  m_Classification = subClass;
}
//-----------------------------------------------------------------------------
bool SESubstanceCompound::HasClassification() const
{
  return m_Classification == ((CDM::enumSubstanceState::value)-1) ? false : true;
}
//-----------------------------------------------------------------------------
void SESubstanceCompound::InvalidateClassification()
{
  m_Classification = (CDM::enumSubstanceClass::value)-1;
}
//-----------------------------------------------------------------------------
bool SESubstanceCompound::GetRhFactor() const
{
  return m_RhFactor;
}
//-----------------------------------------------------------------------------
void SESubstanceCompound::SetRhFactor(bool subFactor)
{
  m_RhFactor = subFactor;
}
//-----------------------------------------------------------------------------
bool SESubstanceCompound::HasRhFactor() const
{
  return (m_RhFactor == true || m_RhFactor == false) ? true : false;
}
//-----------------------------------------------------------------------------
bool SESubstanceCompound::HasComponent() const
{
  return m_Components.size() == 0 ? false : true;
}
//-----------------------------------------------------------------------------
bool SESubstanceCompound::HasComponent(const SESubstance& substance) const
{
  for (SESubstanceConcentration* q : m_Components) {
    if (&substance == &q->GetSubstance())
      return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
const std::vector<SESubstanceConcentration*>& SESubstanceCompound::GetComponents()
{
  return m_Components;
}
//-----------------------------------------------------------------------------
const std::vector<const SESubstanceConcentration*>& SESubstanceCompound::GetComponents() const
{
  return m_cComponents;
}
//-----------------------------------------------------------------------------
const SESubstanceConcentration& SESubstanceCompound::GetComponent(SESubstance& substance)
{
  for (SESubstanceConcentration* sq : m_Components) {
    if (&substance == &sq->GetSubstance())
      return *sq;
  }
  SESubstanceConcentration* sq = new SESubstanceConcentration(substance);
  sq->GetConcentration().SetValue(0, MassPerVolumeUnit::ug_Per_mL);
  m_Components.push_back(sq);
  m_cComponents.push_back(sq);
  return *sq;
}
//-----------------------------------------------------------------------------
const SESubstanceConcentration* SESubstanceCompound::GetComponent(SESubstance& substance) const
{
  for (SESubstanceConcentration* sq : m_Components) {
    if (&substance == &sq->GetSubstance())
      return sq;
  }
  return nullptr;
}
//-----------------------------------------------------------------------------
void SESubstanceCompound::RemoveComponent(const SESubstance& substance)
{
  unsigned int i = 0;
  for (SESubstanceConcentration* sq : m_Components) {
    if (&substance == &sq->GetSubstance()) {
      m_Components.erase(m_Components.begin() + i);
      m_cComponents.erase(m_cComponents.begin() + i);
      delete sq;
    }
    i++;
  }
}
//-----------------------------------------------------------------------------
}
