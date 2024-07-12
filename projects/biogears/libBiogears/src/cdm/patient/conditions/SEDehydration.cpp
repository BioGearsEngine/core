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
#include <biogears/cdm/patient/conditions/SEDehydration.h>

#include <biogears/cdm/properties/SEScalar0To1.h>
#include "io/cdm/PatientConditions.h"

namespace biogears {
SEDehydration::SEDehydration()
  : SEPatientCondition()
{
  m_DehydrationFraction = nullptr;
}
//-----------------------------------------------------------------------------
SEDehydration::~SEDehydration()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEDehydration::Clear()
{
  SEPatientCondition::Clear();
  SAFE_DELETE(m_DehydrationFraction);
}
//-----------------------------------------------------------------------------
bool SEDehydration::IsValid() const
{
  return SEPatientCondition::IsValid() && HasDehydrationFraction();
}
//-----------------------------------------------------------------------------
bool SEDehydration::Load(const CDM::DehydrationData& in)
{
  io::PatientConditions::UnMarshall(in, *this);
  return true;
}
//-----------------------------------------------------------------------------
CDM::DehydrationData* SEDehydration::Unload() const
{
  CDM::DehydrationData* data(new CDM::DehydrationData());
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEDehydration::Unload(CDM::DehydrationData& data) const
{
  io::PatientConditions::Marshall(*this, data); 
}
//-----------------------------------------------------------------------------
bool SEDehydration::HasDehydrationFraction() const
{
  return m_DehydrationFraction == nullptr ? false : m_DehydrationFraction->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar0To1& SEDehydration::GetDehydrationFraction()
{
  if (m_DehydrationFraction == nullptr)
    m_DehydrationFraction = new SEScalar0To1();
  return *m_DehydrationFraction;
}
//-----------------------------------------------------------------------------
void SEDehydration::ToString(std::ostream& str) const
{
  str << "Patient Condition : Dehydration";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tFractional amount of body weight change due to fluid loss: ";
  HasDehydrationFraction() ? str << m_DehydrationFraction : str << "NaN";
  str << std::flush;
}
//-----------------------------------------------------------------------------
bool SEDehydration::operator==(SEDehydration const& rhs) const
{
  if (this == &rhs)
    return true;

  return m_Comment == rhs.m_Comment
    && ((m_DehydrationFraction && rhs.m_DehydrationFraction) ? m_DehydrationFraction->operator==(*rhs.m_DehydrationFraction) : m_DehydrationFraction == rhs.m_DehydrationFraction);
}
bool SEDehydration::operator!=(SEDehydration const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
bool SEDehydration::operator==(SECondition const& rhs) const
{
  try {
    auto& dehydration = dynamic_cast<SEDehydration const&>(rhs);
    return this->operator==(dehydration);
  } catch (std::exception) {
    return false;
  }
  return false;
}
bool SEDehydration::operator!=(SECondition const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}