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
#include <biogears/cdm/compartment/fluid/SELiquidCompartmentLink.h>

#include <biogears/cdm/compartment/SECompartmentGraph.inl>
#include <biogears/cdm/compartment/SECompartmentNodes.inl>
#include <biogears/cdm/compartment/SECompartmentTransportGraph.inl>
#include <biogears/cdm/compartment/fluid/SEFluidCompartment.inl>
#include <biogears/cdm/compartment/fluid/SEFluidCompartmentLink.inl>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>

namespace biogears {
SELiquidCompartmentLink::SELiquidCompartmentLink(SELiquidCompartment& src, SELiquidCompartment& tgt, const char* name)
  : SELiquidCompartmentLink(src, tgt, std::string { name })
{
}
//-------------------------------------------------------------------------------
SELiquidCompartmentLink::SELiquidCompartmentLink(SELiquidCompartment& src, SELiquidCompartment& tgt, const std::string& name)
  : SEFluidCompartmentLink(src, tgt, name)
{
}
//-------------------------------------------------------------------------------
SELiquidCompartmentLink::~SELiquidCompartmentLink()
{
}
//-------------------------------------------------------------------------------
CDM::LiquidCompartmentLinkData* SELiquidCompartmentLink::Unload()
{
  CDM::LiquidCompartmentLinkData* data = new CDM::LiquidCompartmentLinkData();
  SEFluidCompartmentLink::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
}