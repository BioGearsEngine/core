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

#include <biogears/cdm/patient/actions/SEUrinate.h>

namespace biogears {
SEUrinate::SEUrinate()
  : SEPatientAction()
{
}

SEUrinate::~SEUrinate()
{
  Clear();
}

void SEUrinate::Clear()
{
  SEPatientAction::Clear();
}

bool SEUrinate::IsValid() const
{
  return SEPatientAction::IsValid();
}

bool SEUrinate::IsActive() const
{
  return IsValid();
}

bool SEUrinate::Load(const CDM::UrinateData& in)
{
  SEPatientAction::Load(in);
  return true;
}

CDM::UrinateData* SEUrinate::Unload() const
{
  CDM::UrinateData* data(new CDM::UrinateData());
  Unload(*data);
  return data;
}

void SEUrinate::Unload(CDM::UrinateData& data) const
{
  SEPatientAction::Unload(data);
}

void SEUrinate::ToString(std::ostream& str) const
{
  str << "Patient Action : Urinate";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << std::flush;
}
}