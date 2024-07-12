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

#include <biogears/cdm/patient/actions/SEIntubation.h>

#include "io/cdm/PatientActions.h"
namespace biogears {
SEIntubation::SEIntubation()
  : SEPatientAction()
{
  m_Type = SEIntubationType::Invalid;
}
//-------------------------------------------------------------------------------
SEIntubation::~SEIntubation()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEIntubation::Clear()
{
  SEPatientAction::Clear();
  m_Type = SEIntubationType::Invalid;
}
//-------------------------------------------------------------------------------
bool SEIntubation::IsValid() const
{
  return HasType();
}
//-------------------------------------------------------------------------------
bool SEIntubation::IsActive() const
{
  return HasType() && GetType() != SEIntubationType::Off;
}
//-------------------------------------------------------------------------------
bool SEIntubation::Load(const CDM::IntubationData& in, std::default_random_engine *rd)
{
  io::PatientActions::UnMarshall(in, *this, rd);
  return true;
}
//-------------------------------------------------------------------------------
CDM::IntubationData* SEIntubation::Unload() const
{
  CDM::IntubationData* data(new CDM::IntubationData());
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SEIntubation::Unload(CDM::IntubationData& data) const
{
  io::PatientActions::Marshall(*this, data);
}
//-------------------------------------------------------------------------------
SEIntubationType SEIntubation::GetType() const
{
  return m_Type;
}
//-------------------------------------------------------------------------------
void SEIntubation::SetType(SEIntubationType Type)
{
  m_Type = Type;
}
//-------------------------------------------------------------------------------
bool SEIntubation::HasType() const
{
  return m_Type == SEIntubationType::Invalid ? false : true;
}
//-------------------------------------------------------------------------------
void SEIntubation::InvalidateType()
{
  m_Type = SEIntubationType::Invalid;
}
//-------------------------------------------------------------------------------
void SEIntubation::ToString(std::ostream& str) const
{
  str << "Patient Action : Intubation";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tType: ";
  HasType() ? str << GetType() : str << "Not Set";
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SEIntubation::operator==(const SEIntubation& rhs) const
{
  bool equivilant = m_Comment == rhs.m_Comment;
  equivilant &= m_Type == rhs.m_Type;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SEIntubation::operator!=(const SEIntubation& rhs) const
{
  return !(*this == rhs);
}
}