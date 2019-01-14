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
#include <biogears/cdm/compartment/SECompartmentLink.h>

#include <biogears/cdm/circuit/SECircuitManager.h>
#include <biogears/schema/cdm/Compartment.hxx>

namespace biogears {
SECompartmentLink::SECompartmentLink(const char* name, Logger* logger)
  : SECompartmentLink(std::string{ name }, logger)
{
}
//-------------------------------------------------------------------------------
SECompartmentLink::SECompartmentLink(const std::string& name, Logger* logger)
  : Loggable(logger)
  , m_Name(name)
{
}
//-------------------------------------------------------------------------------
SECompartmentLink::~SECompartmentLink()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SECompartmentLink::Clear()
{
}
//-------------------------------------------------------------------------------
bool SECompartmentLink::Load(const CDM::CompartmentLinkData& in, SECircuitManager* circuits)
{
  Clear();
  return true;
}
//-------------------------------------------------------------------------------
void SECompartmentLink::Unload(CDM::CompartmentLinkData& data)
{
  data.Name(m_Name);
}
//-------------------------------------------------------------------------------
std::string SECompartmentLink::GetName() const
{
  return m_Name;
}
//-------------------------------------------------------------------------------
const char* SECompartmentLink::GetName_cStr() const
{
  return m_Name.c_str();
}
}