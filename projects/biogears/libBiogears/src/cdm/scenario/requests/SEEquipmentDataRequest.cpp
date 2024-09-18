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
#include <biogears/cdm/scenario/requests/SEEquipmentDataRequest.h>

#include "io/cdm/DataRequests.h"

#include <biogears/schema/cdm/Environment.hxx>

namespace biogears {
SEEquipmentDataRequest::SEEquipmentDataRequest(const SEDecimalFormat* dfault)
  : SEDataRequest(dfault)
{
  m_Type = "";
}
//-----------------------------------------------------------------------------
SEEquipmentDataRequest::~SEEquipmentDataRequest()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEEquipmentDataRequest::Clear()
{
  SEDataRequest::Clear();
  m_Type = "";
}

//-----------------------------------------------------------------------------
bool SEEquipmentDataRequest::HasType() const
{
  return !(m_Type.empty());
}
//-----------------------------------------------------------------------------
const char* SEEquipmentDataRequest::GetType() const
{
  return m_Type.c_str();
}
//-----------------------------------------------------------------------------
void SEEquipmentDataRequest::SetType(const char* Type)
{
  m_Type = Type;
}
//-----------------------------------------------------------------------------
void SEEquipmentDataRequest::SetType(const std::string& Type)
{
  m_Type = Type;
}
//-----------------------------------------------------------------------------
void SEEquipmentDataRequest::InvalidateType()
{
  m_Type = "";
}
//-----------------------------------------------------------------------------
size_t SEEquipmentDataRequest::HashCode() const
{
  if (m_Hash == 0) {
    m_Hash = std::hash<std::string>()(m_Name + m_Type);
  }
  return m_Hash;
}
//-------------------------------------------------------------------------------
bool SEEquipmentDataRequest ::operator==(SEEquipmentDataRequest const& rhs) const
{
  if (this == &rhs)
    return true;

  return SEDataRequest::operator==(rhs)
    && m_Type == rhs.m_Type;
}
bool SEEquipmentDataRequest ::operator!=(SEEquipmentDataRequest const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}