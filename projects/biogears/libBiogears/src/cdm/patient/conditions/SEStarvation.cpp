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
#include <biogears/cdm/patient/conditions/SEStarvation.h>

#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/schema/cdm/PatientConditions.hxx>

#include "io/cdm/PatientConditions.h"

namespace biogears {
SEStarvation::SEStarvation()
  : SEPatientCondition()
{
  m_TimeSinceMeal = nullptr;
}
//-----------------------------------------------------------------------------
SEStarvation::~SEStarvation()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEStarvation::Clear()
{
  SEPatientCondition::Clear();
  SAFE_DELETE(m_TimeSinceMeal);
}
//-----------------------------------------------------------------------------
bool SEStarvation::IsValid() const
{
  return SEPatientCondition::IsValid() && HasTimeSinceMeal();
}
//-----------------------------------------------------------------------------
bool SEStarvation::Load(const CDM::StarvationData& in)
{
  io::PatientConditions::UnMarshall(in, *this);
  return true;
}
//-----------------------------------------------------------------------------
CDM::StarvationData* SEStarvation::Unload() const
{
  CDM::StarvationData* data(new CDM::StarvationData());
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEStarvation::Unload(CDM::StarvationData& data) const
{
  io::PatientConditions::Marshall(*this, data);
}
//-----------------------------------------------------------------------------
bool SEStarvation::HasTimeSinceMeal() const
{
  return m_TimeSinceMeal == nullptr ? false : m_TimeSinceMeal->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarTime& SEStarvation::GetTimeSinceMeal()
{
  if (m_TimeSinceMeal == nullptr)
    m_TimeSinceMeal = new SEScalarTime();
  return *m_TimeSinceMeal;
}
//-----------------------------------------------------------------------------
void SEStarvation::ToString(std::ostream& str) const
{
  str << "Patient Condition : Starvation";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << std::flush;
}
//-----------------------------------------------------------------------------
bool SEStarvation::operator==(SEStarvation const& rhs) const
{
  if (this == &rhs)
    return true;

  return m_Comment == rhs.m_Comment
   && ((m_TimeSinceMeal&& rhs.m_TimeSinceMeal) ? m_TimeSinceMeal->operator==(*rhs.m_TimeSinceMeal) : m_TimeSinceMeal == rhs.m_TimeSinceMeal)
  ;
}
bool SEStarvation::operator!=(SEStarvation const& rhs) const 
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
bool SEStarvation::operator==(SECondition const& rhs) const
{
  if (this == &rhs)
    return true;
  try {
    auto& starvationRhs = dynamic_cast<SEStarvation const&>(rhs);
    return this->operator==(starvationRhs);
  } catch (std::exception) {
    return false;
  }
  return false;
}
bool SEStarvation::operator!=(SECondition const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}