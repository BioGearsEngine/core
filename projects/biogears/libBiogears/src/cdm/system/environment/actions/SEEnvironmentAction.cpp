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
#include <biogears/cdm/system/environment/actions/SEEnvironmentAction.h>

namespace biogears {
SEEnvironmentAction::SEEnvironmentAction()
  : SEAction()
{
}

SEEnvironmentAction::~SEEnvironmentAction()
{
  Clear();
}

void SEEnvironmentAction::Clear()
{
  SEAction::Clear();
}

bool SEEnvironmentAction::IsValid() const
{
  return SEAction::IsValid();
}

bool SEEnvironmentAction::Load(const CDM::EnvironmentActionData& in)
{
  SEAction::Load(in);
  return true;
}

CDM::EnvironmentActionData* SEEnvironmentAction::Unload() const
{
  CDM::EnvironmentActionData* data = new CDM::EnvironmentActionData();
  Unload(*data);
  return data;
}

void SEEnvironmentAction::Unload(CDM::EnvironmentActionData& data) const
{
  SEAction::Unload(data);
}
}