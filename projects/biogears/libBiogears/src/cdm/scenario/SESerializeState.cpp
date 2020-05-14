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

namespace biogears {
SESerializeState::SESerializeState()
  : SEAction()
{
  m_Filename = "";
  m_Type = (CDM::enumSerializationType::value)-1;
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
  m_Type = (CDM::enumSerializationType::value)-1;
}
//-----------------------------------------------------------------------------
bool SESerializeState::IsValid() const
{
  return HasFilename() && HasType();
}
//-----------------------------------------------------------------------------
bool SESerializeState::Load(const CDM::SerializeStateData& in)
{

  SEAction::Load(in);
  SetType(in.Type());
  SetFilename(in.Filename());
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
  SEAction::Unload(data);
  data.Filename(m_Filename);
  if (HasType())
    data.Type(m_Type);
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
CDM::enumSerializationType::value SESerializeState::GetType() const
{
  return m_Type;
}
//-----------------------------------------------------------------------------
void SESerializeState::SetType(CDM::enumSerializationType::value Type)
{
  m_Type = Type;
}
//-----------------------------------------------------------------------------
bool SESerializeState::HasType() const
{
  return m_Type == ((CDM::enumSerializationType::value)-1) ? false : true;
}
//-----------------------------------------------------------------------------
void SESerializeState::InvalidateType()
{
  m_Type = (CDM::enumSerializationType::value)-1;
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
}