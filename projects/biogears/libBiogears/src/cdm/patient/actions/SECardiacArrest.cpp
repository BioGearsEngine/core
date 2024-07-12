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

#include <biogears/cdm/patient/actions/SECardiacArrest.h>

#include "io/cdm/PatientActions.h"
namespace biogears {
SECardiacArrest::SECardiacArrest()
  : SEPatientAction()
{
  m_State = SEOnOff::Off;
}
//-------------------------------------------------------------------------------
SECardiacArrest::~SECardiacArrest()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SECardiacArrest::Clear()
{
  SEPatientAction::Clear();
  m_State = SEOnOff::Off;
}
//-------------------------------------------------------------------------------
bool SECardiacArrest::IsValid() const
{
  return SEPatientAction::IsValid();
}
//-------------------------------------------------------------------------------
bool SECardiacArrest::IsActive() const
{
  return IsValid() && m_State == SEOnOff::On;
}
//-------------------------------------------------------------------------------
void SECardiacArrest::SetActive(bool b)
{
  m_State = b ? SEOnOff::On : SEOnOff::Off;
}
//-------------------------------------------------------------------------------
bool SECardiacArrest::Load(const CDM::CardiacArrestData& in, std::default_random_engine *rd)
{
  io::PatientActions::UnMarshall(in, *this, rd);
  return true;
}
//-------------------------------------------------------------------------------
CDM::CardiacArrestData* SECardiacArrest::Unload() const
{
  CDM::CardiacArrestData* data(new CDM::CardiacArrestData());
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SECardiacArrest::Unload(CDM::CardiacArrestData& data) const
{
  io::PatientActions::Marshall(*this, data);
}
//-------------------------------------------------------------------------------
void SECardiacArrest::ToString(std::ostream& str) const
{
  str << "Patient Action : Cardiac Arrest";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tState: " << IsActive();
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SECardiacArrest::operator==( const SECardiacArrest& rhs) const
{
  return m_Comment == rhs.m_Comment;
}
//-------------------------------------------------------------------------------
bool SECardiacArrest::operator!=( const SECardiacArrest& rhs) const
{
  return !(*this == rhs);
}
}