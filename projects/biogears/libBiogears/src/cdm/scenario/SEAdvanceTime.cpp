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
#include <biogears/cdm/scenario/SEAdvanceTime.h>

#include "io/cdm/Actions.h"

#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/schema/cdm/Actions.hxx>

namespace biogears {
SEAdvanceTime::SEAdvanceTime()
  : SEAction()
{
  m_Time = nullptr;
}
//-----------------------------------------------------------------------------
SEAdvanceTime::~SEAdvanceTime()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEAdvanceTime::Clear()
{
  SEAction::Clear();
  SAFE_DELETE(m_Time);
}
//-----------------------------------------------------------------------------
bool SEAdvanceTime::IsValid() const
{
  return HasTime() && m_Time->IsValid();
}
//-----------------------------------------------------------------------------
void SEAdvanceTime::ToString(std::ostream& str) const
{
  if (HasComment())
    str << "\n\tComment : " << m_Comment;
  str << "Advance Time : " << m_Time;
}
//-----------------------------------------------------------------------------
bool SEAdvanceTime::HasTime() const
{
  return m_Time == nullptr ? false : m_Time->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarTime& SEAdvanceTime::GetTime()
{
  if (m_Time == nullptr)
    m_Time = new SEScalarTime();
  return *m_Time;
}
//-----------------------------------------------------------------------------
double SEAdvanceTime::GetTime(const TimeUnit& unit) const
{
  if (m_Time == nullptr)
    return SEScalar::dNaN();
  return m_Time->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEAdvanceTime::operator==(SEAdvanceTime const& rhs) const
{
  if (this == &rhs)
    return true;

  return ((m_Time && rhs.m_Time) ? m_Time->operator==(*rhs.m_Time) : m_Time == rhs.m_Time);
}
bool SEAdvanceTime::operator!=(SEAdvanceTime const& rhs) const
{
  return !(*this == rhs);
}
//-----------------------------------------------------------------------------
}