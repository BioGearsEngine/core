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
#include <biogears/cdm/patient/actions/SEForcedInhale.h>

#include "io/cdm/PatientActions.h"
#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarTime.h>

namespace biogears {
SEForcedInhale::SEForcedInhale()
  : SEConsciousRespirationCommand()
{
  m_InspiratoryCapacityFraction = nullptr;
  m_Period = nullptr;
}
//-------------------------------------------------------------------------------
SEForcedInhale::~SEForcedInhale()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEForcedInhale::Clear()
{
  SEConsciousRespirationCommand::Clear();
  SAFE_DELETE(m_InspiratoryCapacityFraction);
  SAFE_DELETE(m_Period);
}
//-------------------------------------------------------------------------------
bool SEForcedInhale::IsValid() const
{
  return SEConsciousRespirationCommand::IsValid() && HasInspiratoryCapacityFraction() && HasPeriod();
}
//-------------------------------------------------------------------------------
bool SEForcedInhale::IsActive() const
{
  return SEConsciousRespirationCommand::IsActive();
}
//-------------------------------------------------------------------------------
bool SEForcedInhale::Load(const CDM::ForcedInhaleData& in, std::default_random_engine *rd)
{
  io::PatientActions::UnMarshall(in, *this, rd);
  return true;
}
//-------------------------------------------------------------------------------
CDM::ForcedInhaleData* SEForcedInhale::Unload() const
{
  CDM::ForcedInhaleData* data(new CDM::ForcedInhaleData());
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SEForcedInhale::Unload(CDM::ForcedInhaleData& data) const
{
  io::PatientActions::Marshall(*this, data);
}
//-------------------------------------------------------------------------------
bool SEForcedInhale::HasInspiratoryCapacityFraction() const
{
  return m_InspiratoryCapacityFraction == nullptr ? false : m_InspiratoryCapacityFraction->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar0To1& SEForcedInhale::GetInspiratoryCapacityFraction()
{
  if (m_InspiratoryCapacityFraction == nullptr)
    m_InspiratoryCapacityFraction = new SEScalar0To1();
  return *m_InspiratoryCapacityFraction;
}
//-------------------------------------------------------------------------------
bool SEForcedInhale::HasPeriod() const
{
  return m_Period == nullptr ? false : m_Period->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarTime& SEForcedInhale::GetPeriod()
{
  if (m_Period == nullptr)
    m_Period = new SEScalarTime();
  return *m_Period;
}
//-------------------------------------------------------------------------------
void SEForcedInhale::ToString(std::ostream& str) const
{
  str << "Forced Inhale";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tInspiratoryCapacityFraction: ";
  HasInspiratoryCapacityFraction() ? str << *m_InspiratoryCapacityFraction : str << "NaN";
  str << "\n\tPeriod: ";
  HasPeriod() ? str << *m_Period : str << "NaN";
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SEForcedInhale::operator==( const SEForcedInhale& rhs) const
{
  bool equivilant = m_Comment == rhs.m_Comment;
  equivilant &= (m_InspiratoryCapacityFraction && rhs.m_InspiratoryCapacityFraction) ? m_InspiratoryCapacityFraction->operator==(*rhs.m_InspiratoryCapacityFraction) : m_InspiratoryCapacityFraction == rhs.m_InspiratoryCapacityFraction;
  equivilant &= (m_Period && rhs.m_Period) ? m_Period->operator==(*rhs.m_Period) : m_Period == rhs.m_Period;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SEForcedInhale::operator!=( const SEForcedInhale& rhs) const
{
  return !(*this == rhs);
}
}