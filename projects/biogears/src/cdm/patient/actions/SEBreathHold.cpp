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

#include <biogears/cdm/patient/actions/SEBreathHold.h>

#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarTime.h>

namespace biogears {
SEBreathHold::SEBreathHold()
  : SEConsciousRespirationCommand()
{
  m_Period = nullptr;
}

SEBreathHold::~SEBreathHold()
{
  Clear();
}

void SEBreathHold::Clear()
{
  SEConsciousRespirationCommand::Clear();
  SAFE_DELETE(m_Period);
}

bool SEBreathHold::IsValid() const
{
  return SEConsciousRespirationCommand::IsValid() && HasPeriod();
}

bool SEBreathHold::IsActive() const
{
  return SEConsciousRespirationCommand::IsActive();
}

bool SEBreathHold::Load(const CDM::BreathHoldData& in)
{
  SEConsciousRespirationCommand::Load(in);
  GetPeriod().Load(in.Period());
  return true;
}

CDM::BreathHoldData* SEBreathHold::Unload() const
{
  CDM::BreathHoldData* data(new CDM::BreathHoldData());
  Unload(*data);
  return data;
}

void SEBreathHold::Unload(CDM::BreathHoldData& data) const
{
  SEConsciousRespirationCommand::Unload(data);
  if (m_Period != nullptr)
    data.Period(std::unique_ptr<CDM::ScalarTimeData>(m_Period->Unload()));
}

bool SEBreathHold::HasPeriod() const
{
  return m_Period == nullptr ? false : m_Period->IsValid();
}
SEScalarTime& SEBreathHold::GetPeriod()
{
  if (m_Period == nullptr)
    m_Period = new SEScalarTime();
  return *m_Period;
}

void SEBreathHold::ToString(std::ostream& str) const
{
  str << "Breath Hold";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tPeriod: ";
  HasPeriod() ? str << *m_Period : str << "NaN";
  str << std::flush;
}
}