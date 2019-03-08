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
#include <biogears/cdm/system/equipment/Inhaler/actions/SEInhalerAction.h>

namespace biogears {
SEInhalerAction::SEInhalerAction()
  : SEAction()
{
}

SEInhalerAction::~SEInhalerAction()
{
  Clear();
}

void SEInhalerAction::Clear()
{
  SEAction::Clear();
}

bool SEInhalerAction::IsValid() const
{
  return SEAction::IsValid();
}

bool SEInhalerAction::Load(const CDM::InhalerActionData& in)
{
  SEAction::Load(in);
  return true;
}

CDM::InhalerActionData* SEInhalerAction::Unload() const
{
  CDM::InhalerActionData* data = new CDM::InhalerActionData();
  Unload(*data);
  return data;
}

void SEInhalerAction::Unload(CDM::InhalerActionData& data) const
{
  SEAction::Unload(data);
}
}