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
#include <biogears/cdm/scenario/requests/SECompartmentDataRequest.h>

#include <biogears/cdm/utils/EnumHashSpecialization.h>

namespace biogears {
SECompartmentDataRequest::SECompartmentDataRequest(const SEDecimalFormat* dfault)
  : SEDataRequest(dfault)
{
  m_Compartment = "";
}
//-------------------------------------------------------------------------------
SECompartmentDataRequest::~SECompartmentDataRequest()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SECompartmentDataRequest::Clear()
{
  SEDataRequest::Clear();
  m_Compartment = "";
}
//-------------------------------------------------------------------------------
size_t SECompartmentDataRequest::HashCode() const
{
  if (m_Hash == 0) {
    m_Hash = std::hash<std::string>()(m_Name + m_Compartment);
  }
  return m_Hash;
}
//-------------------------------------------------------------------------------
bool SECompartmentDataRequest::Load(const CDM::CompartmentDataRequestData& in)
{
  SEDataRequest::Load(in);
  SetCompartment(in.Compartment());
  return true;
}
//-------------------------------------------------------------------------------
void SECompartmentDataRequest::Unload(CDM::CompartmentDataRequestData& data) const
{
  SEDataRequest::Unload(data);
  data.Compartment(m_Compartment);
}
//-------------------------------------------------------------------------------
const char* SECompartmentDataRequest::GetCompartment() const
{
  return m_Compartment.c_str();
}
//-------------------------------------------------------------------------------
void SECompartmentDataRequest::SetCompartment(const char* name)
{
  m_Compartment = name;
  m_Hash = 0;
}
//-------------------------------------------------------------------------------
void SECompartmentDataRequest::SetCompartment(const std::string& name)
{
  m_Compartment = name;
  m_Hash = 0;
}
//-------------------------------------------------------------------------------
bool SECompartmentDataRequest::HasCompartment() const
{
  return !m_Compartment.empty();
}
//-------------------------------------------------------------------------------
void SECompartmentDataRequest::InvalidateCompartment()
{
  m_Compartment = "";
}
//-------------------------------------------------------------------------------
void SECompartmentDataRequest::Set(const char* cmpt, const char* name, const char* unit)
{
  m_Compartment = cmpt;
  m_Name = name;
  m_RequestedUnit = unit;
  m_Unit = nullptr;
  m_Hash = 0;
}
//-------------------------------------------------------------------------------
void SECompartmentDataRequest::Set(const std::string& cmpt, const std::string& name, const std::string& unit)
{
  m_Compartment = cmpt;
  m_Name = name;
  m_RequestedUnit = unit;
  m_Unit = nullptr;
  m_Hash = 0;
}
//-------------------------------------------------------------------------------
void SECompartmentDataRequest::Set(const char* cmpt, const char* name, const CCompoundUnit& unit)
{
  m_Compartment = cmpt;
  m_Name = name;
  m_RequestedUnit = "";
  m_Unit = &unit;
  m_Hash = 0;
}
//-------------------------------------------------------------------------------
void SECompartmentDataRequest::Set(const std::string& cmpt, const std::string& name, const CCompoundUnit& unit)
{
  m_Compartment = cmpt;
  m_Name = name;
  m_RequestedUnit = "";
  m_Unit = &unit;
  m_Hash = 0;
}
//-------------------------------------------------------------------------------
}