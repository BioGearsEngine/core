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
#pragma once
#include <biogears/cdm/compartment/fluid/SEFluidCompartmentLink.h>

#include <biogears/cdm/circuit/SECircuitManager.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>

namespace biogears {
template <FLUID_COMPARTMENT_LINK_TEMPLATE>
SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>::SEFluidCompartmentLink(CompartmentType& src, CompartmentType& tgt, const char* name)
  : SEFluidCompartmentLink(src, tgt, std::string{ name })
{  }
//-------------------------------------------------------------------------------
template <FLUID_COMPARTMENT_LINK_TEMPLATE>
SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>::SEFluidCompartmentLink(CompartmentType& src, CompartmentType& tgt, const std::string& name)
  : SECompartmentLink(name, src.GetLogger())
  , m_SourceCmpt(src)
  , m_TargetCmpt(tgt)
  , m_SourceVertex(src)
  , m_TargetVertex(tgt)
{
  m_Flow = nullptr;
  m_Path = nullptr;
}
//-------------------------------------------------------------------------------
template <FLUID_COMPARTMENT_LINK_TEMPLATE>
SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>::~SEFluidCompartmentLink()
{
}
//-------------------------------------------------------------------------------
template <FLUID_COMPARTMENT_LINK_TEMPLATE>
bool SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>::Load(const CDM::FluidCompartmentLinkData& in, SECircuitManager* circuits)
{
  if (!SECompartmentLink::Load(in, circuits))
    return false;
  if (in.Path().present()) {
    if (circuits == nullptr) {
      Error("Link is mapped to circuit path, " + std::string{ in.Path().get() } +", but no circuit manager was provided, cannot load");
      return false;
    }
    SEFluidCircuitPath* path = circuits->GetFluidPath(in.Path().get());
    if (path == nullptr) {
      Error("Link is mapped to circuit path, " + std::string{ in.Path().get() } +", but provided circuit manager did not have that path");
      return false;
    }
    MapPath(*path);
  } else {
    if (in.Flow().present())
      const_cast<SEScalarVolumePerTime&>(GetFlow()).Load(in.Flow().get());
  }
  return true;
}
//-------------------------------------------------------------------------------
template <FLUID_COMPARTMENT_LINK_TEMPLATE>
void SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>::Unload(CDM::FluidCompartmentLinkData& data)
{
  SECompartmentLink::Unload(data);
  data.SourceCompartment(m_SourceCmpt.GetName());
  data.TargetCompartment(m_TargetCmpt.GetName());
  if (m_Path != nullptr)
    data.Path(m_Path->GetName());
  // Even if you have a path, I am unloading everything, this makes the xml actually usefull...
  if (HasFlow())
    data.Flow(std::unique_ptr<CDM::ScalarVolumePerTimeData>(GetFlow().Unload()));
}
//-------------------------------------------------------------------------------
template <FLUID_COMPARTMENT_LINK_TEMPLATE>
void SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>::Clear()
{
  m_Path = nullptr;
  SAFE_DELETE(m_Flow);
}
//-------------------------------------------------------------------------------
template <FLUID_COMPARTMENT_LINK_TEMPLATE>
const SEScalar* SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>::GetScalar(const char* name)
{
  return GetScalar(std::string{ name });
}
//-------------------------------------------------------------------------------
template <FLUID_COMPARTMENT_LINK_TEMPLATE>
const SEScalar* SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>::GetScalar(const std::string& name)
{
  if (name.compare("Flow") == 0)
    return &GetFlow();
  return nullptr;
}
//-------------------------------------------------------------------------------
template <FLUID_COMPARTMENT_LINK_TEMPLATE>
std::string SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>::GetName() const
{
  return m_Name;
}
//-------------------------------------------------------------------------------
template <FLUID_COMPARTMENT_LINK_TEMPLATE>
const char* SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>::GetName_cStr() const
{
  return m_Name.c_str();
}
//-------------------------------------------------------------------------------
template <FLUID_COMPARTMENT_LINK_TEMPLATE>
bool SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>::HasFlow() const
{
  if (m_Path != nullptr)
    return m_Path->HasNextFlow();
  return m_Flow == nullptr ? false : m_Flow->IsValid();
}
//-------------------------------------------------------------------------------
template <FLUID_COMPARTMENT_LINK_TEMPLATE>
SEScalarVolumePerTime& SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>::GetFlow()
{
  if (m_Path != nullptr)
    return m_Path->GetNextFlow();
  if (m_Flow == nullptr)
    m_Flow = new SEScalarVolumePerTime();
  return *m_Flow;
}
//-------------------------------------------------------------------------------
template <FLUID_COMPARTMENT_LINK_TEMPLATE>
double SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>::GetFlow(const VolumePerTimeUnit& unit) const
{
  if (m_Path != nullptr)
    return m_Path->GetNextFlow(unit);
  if (m_Flow == nullptr)
    return SEScalar::dNaN();
  return m_Flow->GetValue(unit);
}
}