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
#include <biogears/cdm/patient/conditions/SEChronicHeartFailure.h>

namespace biogears {
SEChronicHeartFailure::SEChronicHeartFailure()
  : SEPatientCondition()
{
}

SEChronicHeartFailure::~SEChronicHeartFailure()
{
  Clear();
}

void SEChronicHeartFailure::Clear()
{
  SEPatientCondition::Clear();
}

bool SEChronicHeartFailure::IsValid() const
{
  return SEPatientCondition::IsValid();
}

bool SEChronicHeartFailure::Load(const CDM::ChronicHeartFailureData& in)
{
  SEPatientCondition::Load(in);
  return true;
}

CDM::ChronicHeartFailureData* SEChronicHeartFailure::Unload() const
{
  CDM::ChronicHeartFailureData* data(new CDM::ChronicHeartFailureData());
  Unload(*data);
  return data;
}

void SEChronicHeartFailure::Unload(CDM::ChronicHeartFailureData& data) const
{
  SEPatientCondition::Unload(data);
}
}