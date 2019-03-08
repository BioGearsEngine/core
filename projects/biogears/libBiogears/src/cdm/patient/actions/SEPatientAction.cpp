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

#include <biogears/cdm/patient/actions/SEPatientAction.h>

namespace biogears {
SEPatientAction::SEPatientAction()
  : SEAction()
{
}

SEPatientAction::~SEPatientAction()
{
  Clear();
}

void SEPatientAction::Clear()
{
  SEAction::Clear();
}

bool SEPatientAction::IsValid() const
{
  return SEAction::IsValid();
}

bool SEPatientAction::Load(const CDM::PatientActionData& in)
{
  SEAction::Load(in);
  return true;
}

void SEPatientAction::Unload(CDM::PatientActionData& data) const
{
  SEAction::Unload(data);
}
}