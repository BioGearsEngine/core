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
#include <biogears/cdm/patient/conditions/SEPatientCondition.h>

OPEN_BIOGEARS_NAMESPACE
SEPatientCondition::SEPatientCondition()
  : SECondition()
{
}

SEPatientCondition::~SEPatientCondition()
{
  Clear();
}

void SEPatientCondition::Clear()
{
  SECondition::Clear();
}

bool SEPatientCondition::IsValid() const
{
  return SECondition::IsValid();
}

bool SEPatientCondition::Load(const CDM::PatientConditionData& in)
{
  SECondition::Load(in);
  return true;
}

void SEPatientCondition::Unload(CDM::PatientConditionData& data) const
{
  SECondition::Unload(data);
}
CLOSE_BIOGEARS_NAMESPACE