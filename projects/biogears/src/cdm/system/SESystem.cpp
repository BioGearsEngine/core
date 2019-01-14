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
#include <biogears/cdm/system/SESystem.h>

namespace biogears {

SESystem::SESystem(Logger* logger)
  : Loggable(logger)
{
}
//-------------------------------------------------------------------------------
SESystem::~SESystem()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SESystem::Clear()
{
}
//-------------------------------------------------------------------------------
bool SESystem::Load(const CDM::SystemData& in)
{
  Clear();
  return true;
}
//-------------------------------------------------------------------------------
void SESystem::Unload(CDM::SystemData& data) const
{
}
//-------------------------------------------------------------------------------
const SEScalar* SESystem::GetScalar(const std::string& name, std::vector<SESystem*>* systems)
{
  const SEScalar* s;
  for (auto itr = systems->begin(); itr != systems->end(); itr++) {
    if (*itr == nullptr)
      continue;
    s = (*itr)->GetScalar(name);
    if (s != nullptr)
      return s;
  }
  return nullptr;
}
//-------------------------------------------------------------------------------
const SEScalar* SESystem::GetScalar(const char* name, std::vector<SESystem*>* systems)
{
  return GetScalar( std::string{ name }, systems);
}
}