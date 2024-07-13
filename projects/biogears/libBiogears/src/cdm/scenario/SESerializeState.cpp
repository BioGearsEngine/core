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

#include <biogears/cdm/scenario/SESerializeState.h>
#include <biogears/schema/cdm/Scenario.hxx>

#include "io/cdm/Actions.h"
#include "io/cdm/Scenario.h"

namespace biogears {
SESerializeState::SESerializeState()
  : SEAction()
{
  m_Filename = "";
  m_Type = SESerializationType::Invalid;
}
//-----------------------------------------------------------------------------
SESerializeState::~SESerializeState()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SESerializeState::Clear()
{
  SEAction::Clear();
  m_Filename = "";
  m_Type = SESerializationType::Invalid;
}
//-----------------------------------------------------------------------------
bool SESerializeState::IsValid() const
{
  return HasFilename() && HasType();
}
//-----------------------------------------------------------------------------
bool SESerializeState::Load(const CDM::SerializeStateData& in, std::default_random_engine* rd)
{

  io::Actions::UnMarshall(in, *this);
  return true;
}
//-----------------------------------------------------------------------------
CDM::SerializeStateData* SESerializeState::Unload() const
{
  CDM::SerializeStateData* data = new CDM::SerializeStateData();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SESerializeState::Unload(CDM::SerializeStateData& data) const
{
  io::Actions::Marshall(*this, data);
}
//-----------------------------------------------------------------------------
void SESerializeState::ToString(std::ostream& str) const
{
  if (HasComment())
    str << "\n\tComment : " << m_Comment;
  str << "Type : " << m_Type;
  str << "Filename : " << m_Filename;
}
//-----------------------------------------------------------------------------
SESerializationType SESerializeState::GetType() const
{
  return m_Type;
}
//-----------------------------------------------------------------------------
void SESerializeState::SetType(SESerializationType Type)
{
  m_Type = Type;
}
//-----------------------------------------------------------------------------
bool SESerializeState::HasType() const
{
  return m_Type == SESerializationType::Invalid ? false : true;
}
//-----------------------------------------------------------------------------
void SESerializeState::InvalidateType()
{
  m_Type = SESerializationType::Invalid;
}
//-----------------------------------------------------------------------------
bool SESerializeState::HasFilename() const
{
  return !(m_Filename.empty());
}
//-----------------------------------------------------------------------------
std::string SESerializeState::GetFilename() const
{
  return m_Filename;
}
//-----------------------------------------------------------------------------
const char* SESerializeState::GetFilename_cStr() const
{
  return m_Filename.c_str();
}
//-----------------------------------------------------------------------------
void SESerializeState::SetFilename(const char* filename)
{
  m_Filename = filename;
}
//-----------------------------------------------------------------------------
void SESerializeState::SetFilename(const std::string& filename)
{
  m_Filename = filename;
}
//-----------------------------------------------------------------------------
void SESerializeState::InvalidateFilename()
{
  m_Filename = "";
}
//-----------------------------------------------------------------------------
bool SESerializeState::operator==(SESerializeState const& rhs) const
{
  if (this == &rhs)
    return true;

  return m_Filename == rhs.m_Filename
    && m_Type == m_Type;
}
bool SESerializeState::operator!=(SESerializeState const& rhs) const
{
  return !(*this == rhs);
}
//-----------------------------------------------------------------------------
}