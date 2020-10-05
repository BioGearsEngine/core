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
#include <biogears/cdm/scenario/requests/SECompartmentSubstanceDataRequest.h>

#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/utils/EnumHashSpecialization.h>

namespace biogears {
SECompartmentSubstanceDataRequest::SECompartmentSubstanceDataRequest(const SEDecimalFormat* dfault)
  : SECompartmentDataRequest(dfault)
{
  m_Substance = nullptr;
}
//-----------------------------------------------------------------------------
SECompartmentSubstanceDataRequest::~SECompartmentSubstanceDataRequest()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SECompartmentSubstanceDataRequest::Clear()
{
  SECompartmentDataRequest::Clear();
  m_Substance = nullptr;
}
//-----------------------------------------------------------------------------
size_t SECompartmentSubstanceDataRequest::HashCode() const
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
bool SECompartmentSubstanceDataRequest::Load(const CDM::CompartmentSubstanceDataRequestData& in, const SESubstanceManager& substances)
{
  SECompartmentDataRequest::Load(in);
  if (in.Substance().present())
    SetSubstance(substances.GetSubstance(in.Substance().get()));
  return true;
}
//-----------------------------------------------------------------------------
void SECompartmentSubstanceDataRequest::Unload(CDM::CompartmentSubstanceDataRequestData& data) const
{
  SECompartmentDataRequest::Unload(data);
  if (HasSubstance())
    data.Substance(m_Substance->GetName());
}
//-----------------------------------------------------------------------------
SESubstance* SECompartmentSubstanceDataRequest::GetSubstance() const
{
  return m_Substance;
}
//-----------------------------------------------------------------------------
void SECompartmentSubstanceDataRequest::SetSubstance(SESubstance* substance)
{
  m_Substance = substance;
  m_Hash = 0;
}
//-----------------------------------------------------------------------------
bool SECompartmentSubstanceDataRequest::HasSubstance() const
{
  return m_Substance == nullptr ? false : true;
}
//-----------------------------------------------------------------------------
void SECompartmentSubstanceDataRequest::InvalidateSubstance()
{
  m_Substance = nullptr;
}
//-----------------------------------------------------------------------------
void SECompartmentSubstanceDataRequest::Set(const char* cmpt, SESubstance& substance, const char* name, const char* unit)
{
  Set(std::string{ cmpt }, substance, std::string{ name }, std::string{ unit });
}
  //-----------------------------------------------------------------------------
void SECompartmentSubstanceDataRequest::Set(const std::string& cmpt, SESubstance& substance, const std::string& name, const std::string& unit)
{
  m_Compartment = cmpt;
  m_Substance = &substance;
  m_Name = name;
  m_RequestedUnit = unit;
  m_Unit = nullptr;
  m_Hash = 0;
}
//-----------------------------------------------------------------------------
void SECompartmentSubstanceDataRequest::Set(const char* cmpt, SESubstance& substance, const char* name, const CCompoundUnit& unit)
{
  Set(std::string{ cmpt }, substance, std::string{ name }, unit);
}
  //-----------------------------------------------------------------------------
void SECompartmentSubstanceDataRequest::Set(const std::string& cmpt, SESubstance& substance, const std::string& name, const CCompoundUnit& unit)
{
  m_Compartment = cmpt;
  m_Substance = &substance;
  m_Name = name;
  m_RequestedUnit = "";
  m_Unit = &unit;
  m_Hash = 0;
}
//-----------------------------------------------------------------------------
}