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
#include <biogears/cdm/compartment/fluid/SEFluidCompartment.h>

#include <biogears/cdm/CommonDataModel.h>

#include <biogears/cdm/circuit/SECircuitManager.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>

namespace biogears {
template <FLUID_COMPARTMENT_TEMPLATE>
SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::SEFluidCompartment(const char* name, Logger* logger)
  : SEFluidCompartment(std::string{ name }, logger)
{
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::SEFluidCompartment(const std::string& name, Logger* logger)
  : SECompartment(name, logger)
  , m_Nodes(logger)
{
  m_InFlow = nullptr;
  m_OutFlow = nullptr;
  m_Pressure = nullptr;
  m_Volume = nullptr;
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::~SEFluidCompartment()
{
  Clear();
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
void SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::Clear()
{
  SECompartment::Clear();
  SAFE_DELETE(m_InFlow);
  SAFE_DELETE(m_OutFlow);
  SAFE_DELETE(m_Pressure);
  SAFE_DELETE(m_Volume);
  m_Links.clear();
  m_FluidChildren.clear();
  DELETE_VECTOR(m_SubstanceQuantities);
  m_Nodes.Clear();
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
bool SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::Load(const CDM::FluidCompartmentData& in, SECircuitManager* circuits)
{
  if (!SECompartment::Load(in, circuits))
    return false;
  // Not Loading In/Out Flow, those are calculated on demand
  if (!in.Child().empty())
    return true;
  else if (!in.Node().empty()) {
    if (circuits == nullptr) {
      Error("Compartment is mapped to circuit nodes, but no circuit manager was provided, cannot load");
      return false;
    }
    for (auto name : in.Node()) {
      SEFluidCircuitNode* node = circuits->GetFluidNode(name);
      if (node == nullptr) {
        Error("Compartment is mapped to circuit node, " + std::string{ name } +", but provided circuit manager did not have that node");
        return false;
      }
      MapNode(*node);
    }
  } else { // Only load these if you don't have children or nodes
    if (in.Pressure().present())
      GetPressure().Load(in.Pressure().get());
    if (in.Volume().present())
      GetVolume().Load(in.Volume().get());
  }
  return true;
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
void SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::Unload(CDM::FluidCompartmentData& data)
{
  SECompartment::Unload(data);
  for (SEFluidCompartment* child : m_FluidChildren)
    data.Child().push_back(child->GetName());
  for (SEFluidCircuitNode* nodes : m_Nodes.GetNodes())
    data.Node().push_back(nodes->GetName());
  // Even if you have children or nodes, I am unloading everything, this makes the xml actually usefull...
  if (HasInFlow())
    data.InFlow(std::unique_ptr<CDM::ScalarVolumePerTimeData>(GetInFlow().Unload()));
  if (HasOutFlow())
    data.OutFlow(std::unique_ptr<CDM::ScalarVolumePerTimeData>(GetOutFlow().Unload()));
  if (HasPressure())
    data.Pressure(std::unique_ptr<CDM::ScalarPressureData>(GetPressure().Unload()));
  if (HasVolume())
    data.Volume(std::unique_ptr<CDM::ScalarVolumeData>(GetVolume().Unload()));
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
std::string SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::GetName() const
{
  return m_Name;
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
const char* SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::GetName_cStr() const
{
  return m_Name.c_str();
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
const SEScalar* SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::GetScalar(const char* name)
{
  return GetScalar(std::string{ name });
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
const SEScalar* SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::GetScalar(const std::string& name)
{
  if (name.compare("InFlow") == 0)
    return &GetInFlow();
  if (name.compare("OutFlow") == 0)
    return &GetOutFlow();
  if (name.compare("Pressure") == 0)
    return &GetPressure();
  if (name.compare("Volume") == 0)
    return &GetVolume();
  return nullptr;
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
void SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::MapNode(SEFluidCircuitNode& node)
{
  if (HasChildren())
    Fatal("You cannot map nodes to compartments with children.");
  else
    m_Nodes.MapNode(node);
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
bool SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::HasInFlow() const
{
  if (m_Links.empty())
    return false;
  for (auto* link : m_Links)
    if (link->HasFlow())
      return true;
  return false;
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
const SEScalarVolumePerTime& SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::GetInFlow()
{
  if (m_InFlow == nullptr)
    m_InFlow = new SEScalarVolumePerTime();
  m_InFlow->SetReadOnly(false);
  if (m_Links.empty())
    m_InFlow->Invalidate();
  else
    m_InFlow->SetValue(CalculateInFlow_mL_Per_s(), VolumePerTimeUnit::mL_Per_s);
  m_InFlow->SetReadOnly(true);
  return *m_InFlow;
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
double SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::GetInFlow(const VolumePerTimeUnit& unit) const
{
  if (m_Links.empty())
    return SEScalar::dNaN();
  return Convert(CalculateInFlow_mL_Per_s(), VolumePerTimeUnit::mL_Per_s, unit);
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
bool SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::HasOutFlow() const
{
  if (m_Links.empty())
    return false;
  for (auto* link : m_Links)
    if (link->HasFlow())
      return true;
  return false;
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
const SEScalarVolumePerTime& SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::GetOutFlow()
{
  if (m_OutFlow == nullptr)
    m_OutFlow = new SEScalarVolumePerTime();
  m_OutFlow->SetReadOnly(false);
  if (m_Links.empty())
    m_OutFlow->Invalidate();
  else
    m_OutFlow->SetValue(CalculateOutFlow_mL_Per_s(), VolumePerTimeUnit::mL_Per_s);
  m_OutFlow->SetReadOnly(true);
  return *m_OutFlow;
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
double SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::GetOutFlow(const VolumePerTimeUnit& unit) const
{
  if (m_Links.empty())
    return SEScalar::dNaN();
  return Convert(CalculateOutFlow_mL_Per_s(), VolumePerTimeUnit::mL_Per_s, unit);
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
double SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::CalculateInFlow_mL_Per_s() const
{
  double flow_mL_Per_s = 0;

  SEScalarVolumePerTime* f;
  for (auto* link : m_IncomingLinks) {
    // Positive flow on an incoming path, is flow into the compartment
    f = &link->GetFlow();
    if (f->IsPositive() || f->IsZero())
      flow_mL_Per_s += f->GetValue(VolumePerTimeUnit::mL_Per_s);
  }
  for (auto* link : m_OutgoingLinks) { // Negative flow on an outgoing path, is flow into the compartment
    f = &link->GetFlow();
    if (f->IsNegative() || f->IsZero())
      flow_mL_Per_s += -f->GetValue(VolumePerTimeUnit::mL_Per_s);
  }
  if (flow_mL_Per_s < 0)
    flow_mL_Per_s = 0; // This number is something like x.e-12, which we treat as 0
  return flow_mL_Per_s;
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
double SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::CalculateOutFlow_mL_Per_s() const
{
  double flow_mL_Per_s = 0;

  SEScalarVolumePerTime* f;
  for (auto* link : m_IncomingLinks) {
    // Negative flow on an incoming path, is flow out of the compartment
    f = &link->GetFlow();
    if (f->IsNegative() || f->IsZero())
      flow_mL_Per_s += -f->GetValue(VolumePerTimeUnit::mL_Per_s);
  }
  for (auto* link : m_OutgoingLinks) { // Positive flow on an outgoing path, is flow out of the compartment
    f = &link->GetFlow();
    if (f->IsPositive() || f->IsZero())
      flow_mL_Per_s += f->GetValue(VolumePerTimeUnit::mL_Per_s);
  }
  if (flow_mL_Per_s < 0)
    flow_mL_Per_s = 0; // This number is something like x.e-12, which we treat as 0
  return flow_mL_Per_s;
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
bool SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::HasPressure() const
{
  if (m_Nodes.HasMapping())
    return m_Nodes.HasPotential();
  if (!m_FluidChildren.empty()) {
    for (auto* child : m_FluidChildren)
      if (child->HasPressure())
        return true;
    return false;
  }
  return m_Pressure == nullptr ? false : m_Pressure->IsValid();
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
SEScalarPressure& SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::GetPressure()
{
  if (m_Nodes.HasMapping())
    return m_Nodes.GetPotential();
  if (m_Pressure == nullptr)
    m_Pressure = new SEScalarPressure();
  if (!m_FluidChildren.empty()) {
    m_Pressure->SetReadOnly(false);
    m_Pressure->Invalidate();
    const PressureUnit* pUnit = nullptr;
    for (SEFluidCompartment* child : m_FluidChildren) {
      if (child->HasPressure()) {
        pUnit = child->GetPressure().GetUnit();
        break;
      }
    }
    if (pUnit != nullptr)
      m_Pressure->SetValue(GetPressure(*pUnit), *pUnit);
    m_Pressure->SetReadOnly(true);
  }
  return *m_Pressure;
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
double SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::GetPressure(const PressureUnit& unit) const
{
  if (!HasPressure())
    return SEScalar::dNaN();
  if (m_Nodes.HasMapping())
    return m_Nodes.GetPotential(unit);
  if (!m_FluidChildren.empty()) {
    double Pressure = 0;
    if (HasVolume()) {
      double totalVolume_mL = GetVolume(VolumeUnit::mL);
      for (SEFluidCompartment* child : m_FluidChildren) {
        if (child->HasPressure() && child->HasVolume()) {
          Pressure += child->GetPressure(unit) * (child->GetVolume(VolumeUnit::mL) / totalVolume_mL);
        }
      }
      return Pressure;
    } else {
      int numHas = 0;
      for (SEFluidCompartment* child : m_FluidChildren) {
        if (child->HasPressure()) {
          numHas++;
          Pressure += child->GetPressure(unit);
        }
        Pressure /= numHas;
      }
      return Pressure;
    }
  }
  if (m_Pressure == nullptr)
    return SEScalar::dNaN();
  return m_Pressure->GetValue(unit);
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
bool SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::HasVolume() const
{
  if (m_Nodes.HasMapping())
    return m_Nodes.HasQuantity();
  if (!m_FluidChildren.empty()) {
    for (auto* child : m_FluidChildren)
      if (child->HasVolume())
        return true;
    return false;
  }
  return m_Volume == nullptr ? false : m_Volume->IsValid();
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
SEScalarVolume& SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::GetVolume()
{
  if (m_Nodes.HasMapping())
    return m_Nodes.GetQuantity();
  if (m_Volume == nullptr)
    m_Volume = new SEScalarVolume();
  if (!m_FluidChildren.empty()) {
    m_Volume->SetReadOnly(false);
    m_Volume->Invalidate();
    for (SEFluidCompartment* child : m_FluidChildren)
      if (child->HasVolume())
        m_Volume->Increment(child->GetVolume());
    m_Volume->SetReadOnly(true);
  }
  return *m_Volume;
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
double SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::GetVolume(const VolumeUnit& unit) const
{
  if (m_Nodes.HasMapping())
    return m_Nodes.GetQuantity(unit);
  if (!m_FluidChildren.empty()) {
    double volume = 0;
    for (SEFluidCompartment* child : m_FluidChildren)
      if (child->HasVolume())
        volume += child->GetVolume(unit);
    return volume;
  }
  if (m_Volume == nullptr)
    return SEScalar::dNaN();
  return m_Volume->GetValue(unit);
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
bool SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::HasSubstanceQuantities() const
{
  return !m_SubstanceQuantities.empty();
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
bool SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::HasSubstanceQuantity(const SESubstance& substance) const
{
  for (SubstanceQuantityType* sq : m_SubstanceQuantities) {
    if (&sq->GetSubstance() == &substance)
      return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
SubstanceQuantityType* SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::GetSubstanceQuantity(const SESubstance& substance) const
{
  for (SubstanceQuantityType* sq : m_SubstanceQuantities) {
    if (&sq->GetSubstance() == &substance)
      return sq;
  }
  return nullptr;
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
const std::vector<SubstanceQuantityType*>& SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::GetSubstanceQuantities() const
{
  return m_SubstanceQuantities;
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
void SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::RemoveSubstanceQuantity(const SESubstance& substance)
{
  for (size_t i = 0; i < m_SubstanceQuantities.size(); i++) {
    if (&m_SubstanceQuantities[i]->GetSubstance() == &substance) {
      SAFE_DELETE(m_SubstanceQuantities[i]);
      SAFE_DELETE(m_TransportSubstances[i]); // Assumes these are in sync
      m_SubstanceQuantities.erase(m_SubstanceQuantities.begin() + i);
      m_TransportSubstances.erase(m_TransportSubstances.begin() + i);
    }
  }
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
void SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::ZeroSubstanceQuantities()
{
  for (SubstanceQuantityType* sq : m_SubstanceQuantities) {
    sq->SetToZero();
  }
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
void SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::AddLink(LinkType& link)
{
  if (!Contains(m_Links, link)) {
    m_Links.push_back(&link);
    // Is it incoming or out going?
    if (this == &link.GetSourceCompartment())
      m_OutgoingLinks.push_back(&link);
    else if (this == &link.GetTargetCompartment())
      m_IncomingLinks.push_back(&link);
  }
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
void SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::RemoveLink(LinkType& link)
{
  Remove(m_Links, &link);
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
void SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::RemoveLinks()
{
  m_Links.clear();
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
const std::vector<LinkType*>& SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::GetLinks()
{
  return m_Links;
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
bool SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::HasChild(const char* name)
{
  return HasChild( std::string{ name } );
}
//-----------------------------------------------------------------------------
template <FLUID_COMPARTMENT_TEMPLATE>
bool SEFluidCompartment<FLUID_COMPARTMENT_TYPES>::HasChild(const std::string& name)
{
  for (SEFluidCompartment* cmpt : m_FluidChildren) {
    if (name == cmpt->GetName())
      return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
}