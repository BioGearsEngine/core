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

#include <biogears/cdm/patient/actions/SENeedleDecompression.h>

#include "io/cdm/PatientActions.h"
namespace biogears {
SENeedleDecompression::SENeedleDecompression()
  : SEPatientAction()
{
  m_State = SEOnOff::Off;
  m_Side = SESide::Invalid;
}
//-------------------------------------------------------------------------------
SENeedleDecompression::~SENeedleDecompression()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SENeedleDecompression::Clear()
{
  SEPatientAction::Clear();
  m_State = SEOnOff::Off;
  m_Side = SESide::Invalid;
}
//-------------------------------------------------------------------------------
bool SENeedleDecompression::IsValid() const
{
  return SEPatientAction::IsValid() && HasSide();
}
//-------------------------------------------------------------------------------
bool SENeedleDecompression::IsActive() const
{
  return IsValid() && m_State == SEOnOff::On;
}
//-------------------------------------------------------------------------------
void SENeedleDecompression::SetActive(bool b)
{
  m_State = b ? SEOnOff::On : SEOnOff::Off;
}
//-------------------------------------------------------------------------------
SESide SENeedleDecompression::GetSide() const
{
  return m_Side;
}
//-------------------------------------------------------------------------------
void SENeedleDecompression::SetSide(SESide Side)
{
  m_Side = Side;
}
//-------------------------------------------------------------------------------
bool SENeedleDecompression::HasSide() const
{
  return m_Side == SESide::Invalid ? false : true;
}
//-------------------------------------------------------------------------------
void SENeedleDecompression::InvalidateSide()
{
  m_Side = SESide::Invalid;
}
//-------------------------------------------------------------------------------
void SENeedleDecompression::ToString(std::ostream& str) const
{
  str << "Patient Action : Needle Decompression";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tState: " << IsActive();
  str << "\n\tSide: ";
  HasSide() ? str << GetSide() : str << "No Side Set";
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SENeedleDecompression::operator==( const SENeedleDecompression& rhs) const
{
  bool equivilant = m_Comment == rhs.m_Comment;
  equivilant &= m_Side == rhs.m_Side;
  equivilant &= m_State == rhs.m_State;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SENeedleDecompression::operator!=( const SENeedleDecompression& rhs) const
{
  return !(*this == rhs);
}
}