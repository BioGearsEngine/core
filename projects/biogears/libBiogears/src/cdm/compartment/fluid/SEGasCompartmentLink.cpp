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
#include <biogears/cdm/compartment/fluid/SEGasCompartment.h>

#include <biogears/cdm/compartment/fluid/SEFluidCompartmentLink.inl>
#include <biogears/cdm/compartment/fluid/SEGasCompartmentLink.h>

namespace biogears {
//-----------------------------------------------------------------------------
SEGasCompartmentLink::~SEGasCompartmentLink()
{
}
//-----------------------------------------------------------------------------
SEGasCompartmentLink::SEGasCompartmentLink(SEGasCompartment& src, SEGasCompartment& tgt, const char* name)
  : SEFluidCompartmentLink(src, tgt, std::string { name })
{
}
//-----------------------------------------------------------------------------
SEGasCompartmentLink::SEGasCompartmentLink(SEGasCompartment& src, SEGasCompartment& tgt, const std::string& name)
  : SEFluidCompartmentLink(src, tgt, name)
{
}
//-----------------------------------------------------------------------------
CDM::GasCompartmentLinkData* SEGasCompartmentLink::Unload()
{
  CDM::GasCompartmentLinkData* data = new CDM::GasCompartmentLinkData();
  SEFluidCompartmentLink::Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
}