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

#include <biogears/cdm/patient/actions/SEChestTube.h>

#include "io/cdm/PatientActions.h"
namespace biogears {
SEChestTube::SEChestTube()
  : SEPatientAction()
{
  m_State = SEOnOff::Off;
  m_Side = SESide::Invalid;
}
//-------------------------------------------------------------------------------
SEChestTube::~SEChestTube()
{
  Invalidate();
}
//-------------------------------------------------------------------------------
void SEChestTube::Invalidate()
{
  SEPatientAction::Invalidate();
  m_State = SEOnOff::Off;
  m_Side = SESide::Invalid;
}
//-------------------------------------------------------------------------------
bool SEChestTube::IsValid() const
{
  return SEPatientAction::IsValid() && HasSide();
}
//-------------------------------------------------------------------------------
bool SEChestTube::IsActive() const
{
  return IsValid() && m_State == SEOnOff::On;
}
//-------------------------------------------------------------------------------
void SEChestTube::SetActive(bool b)
{
  m_State = b ? SEOnOff::On : SEOnOff::Off;
}
//-------------------------------------------------------------------------------
SESide SEChestTube::GetSide() const
{
  return m_Side;
}
//-------------------------------------------------------------------------------
void SEChestTube::SetSide(SESide Side)
{
  m_Side = Side;
}
//-------------------------------------------------------------------------------
bool SEChestTube::HasSide() const
{
  return m_Side == SESide::Invalid ? false : true;
}
//-------------------------------------------------------------------------------
void SEChestTube::InvalidateSide()
{
  m_Side = SESide::Invalid;
}
//-------------------------------------------------------------------------------
void SEChestTube::ToString(std::ostream& str) const
{
  str << "Patient Action : Chest Tube";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tState: " << IsActive();
  str << "\n\tSide: ";
  HasSide() ? str << GetSide() : str << "No Side Set";
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SEChestTube::operator==(const SEChestTube& rhs) const
{
  bool equivilant = m_Comment == rhs.m_Comment;
  equivilant &= m_Side == rhs.m_Side;
  equivilant &= m_State == rhs.m_State;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SEChestTube::operator!=(const SEChestTube& rhs) const
{
  return !(*this == rhs);
}
}