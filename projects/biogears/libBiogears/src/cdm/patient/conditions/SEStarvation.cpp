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
  SEPatientCondition::Load(in);
  GetTimeSinceMeal().Load(in.TimeSinceMeal());
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
  SEPatientCondition::Unload(data);
  if (m_TimeSinceMeal != nullptr)
    data.TimeSinceMeal(std::unique_ptr<CDM::ScalarTimeData>(m_TimeSinceMeal->Unload()));
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
}