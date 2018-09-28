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

#include <biogears/cdm/patient/actions/SEOverride.h>

SEOverride::SEOverride()
  : SEPatientAction()
{
  m_OverrideSwitch = CDM::enumOnOff::Off;
  }

SEOverride::~SEOverride()
{
  Clear();
}

void SEOverride::Clear()
{
  m_OverrideSwitch = CDM::enumOnOff::Off;
  SEPatientAction::Clear();
}

bool SEOverride::IsValid() const
{
  return SEPatientAction::IsValid();
}

bool SEOverride::IsActive() const
{
  return IsValid();
}

bool SEOverride::Load(const CDM::OverrideData& in)
{
  SEPatientAction::Clear();
  SetOverrideSwitch(in.State());
  //SEPatientAction::Load(in);
  //return true;
  return IsValid();
}

CDM::OverrideData* SEOverride::Unload() const
{
  CDM::OverrideData* data(new CDM::OverrideData());
  Unload(*data);
  return data;
}

void SEOverride::Unload(CDM::OverrideData& data) const
{
  SEPatientAction::Unload(data);
  if (HasOverrideSwitch())
    data.State(m_OverrideSwitch);
}

CDM::enumOnOff::value SEOverride::GetOverrideSwitch() const
{
  return m_OverrideSwitch;
}
void SEOverride::SetOverrideSwitch(CDM::enumOnOff::value state)
{
  m_OverrideSwitch = state;
}
bool SEOverride::HasOverrideSwitch() const
{
  return (m_OverrideSwitch == CDM::enumOnOff::Off) ? true : 
         (m_OverrideSwitch == CDM::enumOnOff::On) ?  true : false;
}
void SEOverride::InvalidateOverrideSwitch()
{
  m_OverrideSwitch = (CDM::enumOnOff::Off);
}


void SEOverride::ToString(std::ostream& str) const
{
  str << "Patient Action : Override Parameter";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;

  str << "\n\tState: ";
  HasOverrideSwitch() ? str << GetOverrideSwitch() : str << "Not Set";
  str << std::flush;
}
