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
#include <biogears/cdm/scenario/requests/SEDataRequest.h>

#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/substance/SESubstanceManager.h>

namespace biogears {
SEDataRequest::SEDataRequest(const SEDecimalFormat* dfault)
  : SEDecimalFormat(dfault)
{
  m_Name = "";
  m_RequestedUnit = "";
  m_Unit = nullptr;
  m_Hash = 0;
}
//-----------------------------------------------------------------------------
SEDataRequest::~SEDataRequest()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEDataRequest::Clear()
{
  m_Name = "";
  m_RequestedUnit = "";
  m_Unit = nullptr;
}
//-----------------------------------------------------------------------------
bool SEDataRequest::Load(const CDM::DataRequestData& in)
{
  
  SEDecimalFormat::Load(in);
  m_Name = in.Name();
  if (in.Unit().present())
    m_RequestedUnit = in.Unit().get();
  return true;
}
//-----------------------------------------------------------------------------
CDM::DataRequestData* SEDataRequest::Unload() const
{
  CDM::DataRequestData* data = new CDM::DataRequestData();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEDataRequest::Unload(CDM::DataRequestData& data) const
{
  SEDecimalFormat::Unload(data);
  data.Name(m_Name);
  if (HasUnit())
    data.Unit(m_Unit->GetString());
  else if (HasRequestedUnit())
    data.Unit(m_RequestedUnit);
}
//-----------------------------------------------------------------------------
std::string SEDataRequest::GetName() const
{
  return m_Name;
}
//-----------------------------------------------------------------------------
const char* SEDataRequest::GetName_cStr() const
{
  return m_Name.c_str();
}
//-----------------------------------------------------------------------------
void SEDataRequest::SetName(const char* name)
{
  m_Name = name;
  m_Hash = 0;
}
//-----------------------------------------------------------------------------
void SEDataRequest::SetName(const std::string& name)
{
  m_Name = name;
  m_Hash = 0;
}
//-----------------------------------------------------------------------------
bool SEDataRequest::HasName() const
{
  return m_Name.empty() ? false : true;
}
//-----------------------------------------------------------------------------
void SEDataRequest::InvalidateName()
{
  m_Name = "";
}
//-----------------------------------------------------------------------------
const char* SEDataRequest::GetRequestedUnit() const
{
  return m_RequestedUnit.c_str();
}
//-----------------------------------------------------------------------------
void SEDataRequest::SetRequestedUnit(const char* unit)
{
  m_RequestedUnit = unit;
  m_Unit = nullptr;
  m_Hash = 0;
}
//-----------------------------------------------------------------------------
void SEDataRequest::SetRequestedUnit(const std::string& unit)
{
  m_RequestedUnit = unit;
  m_Unit = nullptr;
  m_Hash = 0;
}
//-----------------------------------------------------------------------------
bool SEDataRequest::HasRequestedUnit() const
{
  return m_RequestedUnit.empty() ? false : true;
}
//-----------------------------------------------------------------------------
void SEDataRequest::InvalidateRequestedUnit()
{
  m_RequestedUnit = "";
}
//-----------------------------------------------------------------------------
const CCompoundUnit* SEDataRequest::GetUnit() const
{
  return m_Unit;
}
//-----------------------------------------------------------------------------
void SEDataRequest::SetUnit(const CCompoundUnit& unit)
{
  m_Unit = &unit;
  m_RequestedUnit = "";
  m_Hash = 0;
}
//-----------------------------------------------------------------------------
bool SEDataRequest::HasUnit() const
{
  return m_Unit == nullptr ? false : true;
}
//-----------------------------------------------------------------------------
void SEDataRequest::InvalidateUnit()
{
  m_Unit = nullptr;
}
//-----------------------------------------------------------------------------
void SEDataRequest::Set(const char* name, const char* unit)
{
  m_Name = name;
  m_RequestedUnit = unit;
  m_Unit = nullptr;
}
//-----------------------------------------------------------------------------
void SEDataRequest::Set(const std::string& name, const std::string& unit)
{
  m_Name = name;
  m_RequestedUnit = unit;
  m_Unit = nullptr;
  m_Hash = 0;
}
//-----------------------------------------------------------------------------
void SEDataRequest::Set(const char* name, const CCompoundUnit& unit)
{
  m_Name = name;
  m_RequestedUnit = unit.ToString();
  m_Unit = &unit;
  m_Hash = 0;
}
//-----------------------------------------------------------------------------
void SEDataRequest::Set(const std::string& name, const CCompoundUnit& unit)
{
  m_Name = name;
  m_RequestedUnit = unit.ToString();
  m_Unit = &unit;
  m_Hash = 0;
}
//-----------------------------------------------------------------------------
}