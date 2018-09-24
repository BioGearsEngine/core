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
}

SEOverride::~SEOverride()
{
  Clear();
}

void SEOverride::Clear()
{
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
  SEPatientAction::Load(in);
  return true;
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
}

void SEOverride::ToString(std::ostream& str) const
{
  str << "Action : Parameter Override";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << std::flush;
}
