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

#include <biogears/cdm/patient/actions/SEUseInhaler.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/substance/SESubstance.h>

namespace biogears {
SEUseInhaler::SEUseInhaler()
  : SEConsciousRespirationCommand()
{
}

SEUseInhaler::~SEUseInhaler()
{
  Clear();
}

void SEUseInhaler::Clear()
{
  SEConsciousRespirationCommand::Clear();
}

bool SEUseInhaler::IsValid() const
{
  return SEConsciousRespirationCommand::IsValid();
}

bool SEUseInhaler::IsActive() const
{
  return SEConsciousRespirationCommand::IsActive();
}

bool SEUseInhaler::Load(const CDM::UseInhalerData& in)
{
  SEConsciousRespirationCommand::Load(in);
  return true;
}

CDM::UseInhalerData* SEUseInhaler::Unload() const
{
  CDM::UseInhalerData* data(new CDM::UseInhalerData());
  Unload(*data);
  return data;
}

void SEUseInhaler::Unload(CDM::UseInhalerData& data) const
{
  SEConsciousRespirationCommand::Unload(data);
}

void SEUseInhaler::ToString(std::ostream& str) const
{
  str << "Use Inhaler";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << std::flush;
}
}