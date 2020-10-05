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
#include <biogears/cdm/scenario/requests/SESubstanceDataRequest.h>

#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/utils/EnumHashSpecialization.h>

namespace biogears {
SESubstanceDataRequest::SESubstanceDataRequest(const SEDecimalFormat* dfault)
  : SEDataRequest(dfault)
{
  m_Compartment = "";
  m_Substance = nullptr;
}
//-----------------------------------------------------------------------------
SESubstanceDataRequest::~SESubstanceDataRequest()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SESubstanceDataRequest::Clear()
{
  SEDataRequest::Clear();
  m_Compartment = "";
  m_Substance = nullptr;
}
//-----------------------------------------------------------------------------
size_t SESubstanceDataRequest::HashCode() const
{
  if (m_Hash == 0) {
    std::string hashString = m_Name + m_Compartment;
    if (HasSubstance()) {
      hashString += m_Substance->GetName();
    }
    m_Hash = std::hash<std::string>()(hashString);
  }
  return m_Hash;
}
//-----------------------------------------------------------------------------
bool SESubstanceDataRequest::Load(const CDM::SubstanceDataRequestData& in, const SESubstanceManager& substances)
{
  SEDataRequest::Load(in);
  if (in.Compartment().present())
    SetCompartment(in.Compartment().get());
  SetSubstance(substances.GetSubstance(in.Substance()));
  return true;
}
//-----------------------------------------------------------------------------
CDM::SubstanceDataRequestData* SESubstanceDataRequest::Unload() const
{
  CDM::SubstanceDataRequestData* data = new CDM::SubstanceDataRequestData();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SESubstanceDataRequest::Unload(CDM::SubstanceDataRequestData& data) const
{
  SEDataRequest::Unload(data);
  if (HasCompartment())
    data.Compartment(m_Compartment);
  if (HasSubstance())
    data.Substance(m_Substance->GetName());
}
//-----------------------------------------------------------------------------
const char* SESubstanceDataRequest::GetCompartment() const
{
  return m_Compartment.c_str();
}
//-----------------------------------------------------------------------------
void SESubstanceDataRequest::SetCompartment(const char* name)
{
  m_Compartment = name;
  m_Hash = 0;
}
//-----------------------------------------------------------------------------
void SESubstanceDataRequest::SetCompartment(const std::string& name)
{
  m_Compartment = name;
  m_Hash = 0;
}
//-----------------------------------------------------------------------------
bool SESubstanceDataRequest::HasCompartment() const
{
  return !m_Compartment.empty();
}
//-----------------------------------------------------------------------------
void SESubstanceDataRequest::InvalidateCompartment()
{
  m_Compartment = "";
}
//-----------------------------------------------------------------------------
const SESubstance* SESubstanceDataRequest::GetSubstance() const
{
  return m_Substance;
}
//-----------------------------------------------------------------------------
void SESubstanceDataRequest::SetSubstance(SESubstance* substance)
{
  m_Substance = substance;
  m_Hash = 0;
}
//-----------------------------------------------------------------------------
bool SESubstanceDataRequest::HasSubstance() const
{
  return m_Substance == nullptr ? false : true;
}
//-----------------------------------------------------------------------------
void SESubstanceDataRequest::InvalidateSubstance()
{
  m_Substance = nullptr;
}
//-----------------------------------------------------------------------------
void SESubstanceDataRequest::Set(const SESubstance& substance, const char* name, const CCompoundUnit& unit)
{
  assert(&substance);
  assert(const_cast<SESubstance&>(substance).GetScalar(name));
  m_Substance = &substance;
  m_Compartment = "";
  m_Name = name;
  m_RequestedUnit = "";
  m_Unit = &unit;
  m_Hash = 0;
}
//-----------------------------------------------------------------------------
void SESubstanceDataRequest::Set(const SESubstance& substance, const std::string& name, const CCompoundUnit& unit)
{
  assert(&substance);
  assert(const_cast<SESubstance&>(substance).GetScalar(name));
  m_Substance = &substance;
  m_Compartment = "";
  m_Name = name;
  m_RequestedUnit = "";
  m_Unit = &unit;
  m_Hash = 0;
}
//-----------------------------------------------------------------------------
void SESubstanceDataRequest::Set(const SESubstance& substance, const char* name, const char* unit)
{
  assert(&substance);
  assert(const_cast<SESubstance&>(substance).GetScalar(name));
  m_Substance = &substance;
  m_Compartment = "";
  m_Name = name;
  m_RequestedUnit = unit;
  m_Unit = nullptr;
  m_Hash = 0;
}
//-----------------------------------------------------------------------------
void SESubstanceDataRequest::Set(const SESubstance& substance, const std::string& name, const std::string& unit)
{
  assert(&substance);
  assert(const_cast<SESubstance&>(substance).GetScalar(name));
  m_Substance = &substance;
  m_Compartment = "";
  m_Name = name;
  m_RequestedUnit = unit;
  m_Unit = nullptr;
  m_Hash = 0;
}
//-----------------------------------------------------------------------------
void SESubstanceDataRequest::Set(const SESubstance& substance, const char* cmpt, const char* name, const CCompoundUnit& unit)
{
  assert(&substance);
  assert(const_cast<SESubstance&>(substance).GetScalar(name));
  m_Substance = &substance;
  m_Compartment = cmpt;
  m_Name = name;
  m_RequestedUnit = "";
  m_Unit = &unit;
  m_Hash = 0;
}
//-----------------------------------------------------------------------------
void SESubstanceDataRequest::Set(const SESubstance& substance, const std::string& cmpt, const std::string& name, const CCompoundUnit& unit)
{
  assert(&substance);
  assert(const_cast<SESubstance&>(substance).GetScalar(name));
  m_Substance = &substance;
  m_Compartment = cmpt;
  m_Name = name;
  m_RequestedUnit = "";
  m_Unit = &unit;
  m_Hash = 0;
}
//-----------------------------------------------------------------------------
void SESubstanceDataRequest::Set(const SESubstance& substance, const char* cmpt, const char* name, const char* unit)
{
  assert(&substance);
  assert(const_cast<SESubstance&>(substance).GetScalar(name));
  m_Substance = &substance;
  m_Compartment = cmpt;
  m_Name = name;
  m_RequestedUnit = unit;
  m_Unit = nullptr;
  m_Hash = 0;
}
//-----------------------------------------------------------------------------
void SESubstanceDataRequest::Set(const SESubstance& substance, const std::string& cmpt, const std::string& name, const std::string& unit)
{
  assert(&substance);
  assert(const_cast<SESubstance&>(substance).GetScalar(name));
  m_Substance = &substance;
  m_Compartment = cmpt;
  m_Name = name;
  m_RequestedUnit = unit;
  m_Unit = nullptr;
  m_Hash = 0;
}
//-----------------------------------------------------------------------------
}