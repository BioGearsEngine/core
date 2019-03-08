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

#include <biogears/cdm/circuit/SECircuitManager.h>
#include <biogears/cdm/compartment/SECompartment.h>
#include <biogears/schema/cdm/Compartment.hxx>

namespace biogears {
SECompartment::SECompartment(const char* name, Logger* logger)
  : SECompartment(std::string{ name }, logger)
{
}
//-----------------------------------------------------------------------------
SECompartment::SECompartment(const std::string& name, Logger* logger)
  : Loggable(logger)
  , m_Name(name)
{
}
//-----------------------------------------------------------------------------
SECompartment::~SECompartment()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SECompartment::Clear()
{
}
//-----------------------------------------------------------------------------
bool SECompartment::Load(const CDM::CompartmentData& in, SECircuitManager* circuits)
{
  Clear();
  return true;
}
//-----------------------------------------------------------------------------
void SECompartment::Unload(CDM::CompartmentData& data)
{
  data.Name(m_Name);
}
//-----------------------------------------------------------------------------
std::string SECompartment::GetName() const
{
  return m_Name;
}
//-----------------------------------------------------------------------------
const char* SECompartment::GetName_cStr() const
{
  return m_Name.c_str();
}
//-----------------------------------------------------------------------------
}