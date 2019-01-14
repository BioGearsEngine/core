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
#include <biogears/cdm/compartment/thermal/SEThermalCompartment.h>

#include <biogears/cdm/circuit/SECircuitManager.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>
#include <biogears/cdm/compartment/thermal/SEThermalCompartmentLink.h>
#include <biogears/cdm/properties/SEScalarEnergy.h>
#include <biogears/cdm/properties/SEScalarPower.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>

namespace biogears {
SEThermalCompartment::SEThermalCompartment(const char* name, Logger* logger)
  : SEThermalCompartment(std::string{ name }, logger)
{
}
//-----------------------------------------------------------------------------
SEThermalCompartment::SEThermalCompartment(const std::string& name, Logger* logger)
  : SECompartment(name, logger)
  , m_Nodes(logger)
{
  m_HeatTransferRateIn = nullptr;
  m_HeatTransferRateOut = nullptr;
  m_Heat = nullptr;
  m_Temperature = nullptr;
}
//-----------------------------------------------------------------------------
SEThermalCompartment::~SEThermalCompartment()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEThermalCompartment::Clear()
{
  SECompartment::Clear();
  SAFE_DELETE(m_HeatTransferRateIn);
  SAFE_DELETE(m_HeatTransferRateOut);
  SAFE_DELETE(m_Heat);
  SAFE_DELETE(m_Temperature);
  m_Links.clear();
  m_Children.clear();
  m_Nodes.Clear();
}
//-----------------------------------------------------------------------------
bool SEThermalCompartment::Load(const CDM::ThermalCompartmentData& in, SECircuitManager* circuits)
{
  if (!SECompartment::Load(in, circuits))
    return false;
  // Not Loading In/Out HeatTransferRate, those are calculated on demand
  if (!in.Child().empty())
    return true;
  else if (!in.Node().empty()) {
    if (circuits == nullptr) {
      Error("Compartment is mapped to circuit nodes, but no circuit manager was provided, cannot load");
      return false;
    }
    for (auto name : in.Node()) {
      SEThermalCircuitNode* node = circuits->GetThermalNode(name);
      if (node == nullptr) {
        Error("Compartment is mapped to circuit node, " + std::string{ name } +", but provided circuit manager did not have that node");
        return false;
      }
      MapNode(*node);
    }
  } else { // Only load these if you don't have children or nodes
    if (in.Heat().present())
      GetHeat().Load(in.Heat().get());
    if (in.Temperature().present())
      GetTemperature().Load(in.Temperature().get());
  }
  return true;
}
//-----------------------------------------------------------------------------
CDM::ThermalCompartmentData* SEThermalCompartment::Unload()
{
  CDM::ThermalCompartmentData* data = new CDM::ThermalCompartmentData();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEThermalCompartment::Unload(CDM::ThermalCompartmentData& data)
{
  SECompartment::Unload(data);
  for (SEThermalCompartment* child : m_Children)
    data.Child().push_back(child->GetName());
  for (SEThermalCircuitNode* nodes : m_Nodes.GetNodes())
    data.Node().push_back(nodes->GetName());
  // Even if you have children or nodes, I am unloading everything, this makes the xml actually usefull...
  if (HasHeatTransferRateIn())
    data.HeatTransferRateIn(std::unique_ptr<CDM::ScalarPowerData>(GetHeatTransferRateIn().Unload()));
  if (HasHeatTransferRateOut())
    data.HeatTransferRateOut(std::unique_ptr<CDM::ScalarPowerData>(GetHeatTransferRateOut().Unload()));
  if (HasHeat())
    data.Heat(std::unique_ptr<CDM::ScalarEnergyData>(GetHeat().Unload()));
  if (HasTemperature())
    data.Temperature(std::unique_ptr<CDM::ScalarTemperatureData>(GetTemperature().Unload()));
}
//-----------------------------------------------------------------------------
const SEScalar* SEThermalCompartment::GetScalar(const char* name)
{
  return GetScalar(std::string{ name });
}
//-----------------------------------------------------------------------------
const SEScalar* SEThermalCompartment::GetScalar(const std::string& name)
{
  if (name.compare("HeatTransferRateIn") == 0)
    return &GetHeatTransferRateIn();
  if (name.compare("HeatTransferRateOut") == 0)
    return &GetHeatTransferRateOut();
  if (name.compare("Heat") == 0)
    return &GetHeat();
  if (name.compare("Temperature") == 0)
    return &GetTemperature();
  return nullptr;
}
//-----------------------------------------------------------------------------
void SEThermalCompartment::StateChange()
{
  m_Leaves.clear();
  FindLeaves<SEThermalCompartment>(*this, m_Leaves);
  m_Nodes.StateChange();
}
//-----------------------------------------------------------------------------
void SEThermalCompartment::MapNode(SEThermalCircuitNode& node)
{
  if (HasChildren())
    Fatal("You cannot map nodes to compartments with children.");
  else
    m_Nodes.MapNode(node);
}
//-----------------------------------------------------------------------------
bool SEThermalCompartment::HasHeat() const
{
  if (m_Nodes.HasMapping())
    return m_Nodes.HasQuantity();
  if (!m_Children.empty()) {
    for (auto* child : m_Children)
      if (child->HasHeat())
        return true;
    return false;
  }
  return m_Heat == nullptr ? false : m_Heat->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarEnergy& SEThermalCompartment::GetHeat()
{
  if (m_Nodes.HasMapping())
    return m_Nodes.GetQuantity();
  if (m_Heat == nullptr)
    m_Heat = new SEScalarEnergy();
  if (!m_Children.empty()) {
    m_Heat->SetReadOnly(false);
    m_Heat->Invalidate();
    for (SEThermalCompartment* child : m_Children)
      if (child->HasHeat())
        m_Heat->Increment(child->GetHeat());
    m_Heat->SetReadOnly(true);
  }
  return *m_Heat;
}
//-----------------------------------------------------------------------------
double SEThermalCompartment::GetHeat(const EnergyUnit& unit) const
{
  if (m_Nodes.HasMapping())
    return m_Nodes.GetQuantity(unit);
  if (!m_Children.empty()) {
    double heat = 0;
    for (SEThermalCompartment* child : m_Children)
      if (child->HasHeat())
        heat += child->GetHeat(unit);
    return heat;
  }
  if (m_Heat == nullptr)
    return SEScalar::dNaN();
  return m_Heat->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEThermalCompartment::HasTemperature() const
{
  if (m_Nodes.HasMapping())
    return m_Nodes.HasPotential();
  if (!m_Children.empty()) {
    for (auto* child : m_Children)
      if (child->HasTemperature())
        return true;
    return false;
  }
  return m_Temperature == nullptr ? false : m_Temperature->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarTemperature& SEThermalCompartment::GetTemperature()
{
  if (m_Nodes.HasMapping())
    return m_Nodes.GetPotential();
  if (m_Temperature == nullptr)
    m_Temperature = new SEScalarTemperature();
  if (!m_Children.empty()) {
    m_Temperature->SetReadOnly(false);
    const TemperatureUnit* tUnit = nullptr;
    for (SEThermalCompartment* child : m_Children) {
      if (child->HasTemperature()) {
        tUnit = child->GetTemperature().GetUnit();
        break;
      }
    }
    if (tUnit != nullptr)
      m_Temperature->SetValue(GetTemperature(*tUnit), *tUnit);
    m_Temperature->SetReadOnly(true);
  }
  return *m_Temperature;
}
//-----------------------------------------------------------------------------
double SEThermalCompartment::GetTemperature(const TemperatureUnit& unit) const
{
  if (!HasTemperature())
    return SEScalar::dNaN();
  if (m_Nodes.HasMapping())
    return m_Nodes.GetPotential(unit);
  if (!m_Children.empty()) {
    double temperature = 0;
    if (HasHeat()) {
      double totalHeat_J = GetHeat(EnergyUnit::J);
      for (SEThermalCompartment* child : m_Children) {
        if (child->HasTemperature() && child->HasHeat()) {
          temperature += child->GetTemperature(unit) * (child->GetHeat(EnergyUnit::J) / totalHeat_J);
        }
      }
      return temperature;
    } else {
      int numHas = 0;
      for (SEThermalCompartment* child : m_Children) {
        if (child->HasTemperature()) {
          numHas++;
          temperature += child->GetTemperature(unit);
        }
        temperature /= numHas;
      }
      return temperature;
    }
  }
  if (m_Temperature == nullptr)
    return SEScalar::dNaN();
  return m_Temperature->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEThermalCompartment::HasHeatTransferRateIn() const
{
  if (m_Links.empty())
    return false;
  for (auto* link : m_Links)
    if (link->HasHeatTransferRate())
      return true;
  return false;
}
//-----------------------------------------------------------------------------
const SEScalarPower& SEThermalCompartment::GetHeatTransferRateIn()
{

  if (m_HeatTransferRateIn == nullptr)
    m_HeatTransferRateIn = new SEScalarPower();
  m_HeatTransferRateIn->SetReadOnly(false);
  if (m_Links.empty())
    m_HeatTransferRateIn->Invalidate();
  else
    m_HeatTransferRateIn->SetValue(CalculateInFlow_W(), PowerUnit::W);
  m_HeatTransferRateIn->SetReadOnly(true);
  return *m_HeatTransferRateIn;
}
//-----------------------------------------------------------------------------
double SEThermalCompartment::GetHeatTransferRateIn(const PowerUnit& unit) const
{
  if (m_Links.empty())
    return SEScalar::dNaN();
  return Convert(CalculateInFlow_W(), PowerUnit::W, unit);
}
//-----------------------------------------------------------------------------
bool SEThermalCompartment::HasHeatTransferRateOut() const
{
  if (m_Links.empty())
    return false;
  for (auto* link : m_Links)
    if (link->HasHeatTransferRate())
      return true;
  return false;
}
//-----------------------------------------------------------------------------
const SEScalarPower& SEThermalCompartment::GetHeatTransferRateOut()
{
  if (m_HeatTransferRateOut == nullptr)
    m_HeatTransferRateOut = new SEScalarPower();
  m_HeatTransferRateOut->SetReadOnly(false);
  if (m_Links.empty())
    m_HeatTransferRateOut->Invalidate();
  else
    m_HeatTransferRateOut->SetValue(CalculateOutFlow_W(), PowerUnit::W);
  m_HeatTransferRateOut->SetReadOnly(true);
  return *m_HeatTransferRateOut;
}
//-----------------------------------------------------------------------------
double SEThermalCompartment::GetHeatTransferRateOut(const PowerUnit& unit) const
{
  if (m_Links.empty())
    return SEScalar::dNaN();
  return Convert(CalculateOutFlow_W(), PowerUnit::W, unit);
}
//-----------------------------------------------------------------------------
double SEThermalCompartment::CalculateInFlow_W() const
{
  double flow_W = 0;

  SEScalarPower* f;
  for (SEThermalCompartmentLink* link : m_IncomingLinks) {
    // Positive flow on an incoming path, is flow into the compartment
    f = &link->GetHeatTransferRate();
    if (f->IsPositive() || f->IsZero())
      flow_W += f->GetValue(PowerUnit::W);
  }
  for (SEThermalCompartmentLink* link : m_OutgoingLinks) {
    // Negative flow on an outgoing path, is flow into the compartment
    f = &link->GetHeatTransferRate();
    if (f->IsNegative() || f->IsZero())
      flow_W += -f->GetValue(PowerUnit::W);
  }
  if (flow_W < 0)
    flow_W = 0; // This number is something like x.e-12, which we treat as 0
  return flow_W;
}
//-----------------------------------------------------------------------------
double SEThermalCompartment::CalculateOutFlow_W() const
{
  double flow_W = 0;

  SEScalarPower* f;
  for (SEThermalCompartmentLink* link : m_IncomingLinks) {
    // Negative flow on an incoming path, is flow out of the compartment
    f = &link->GetHeatTransferRate();
    if (f->IsNegative() || f->IsZero())
      flow_W += -f->GetValue(PowerUnit::W);
  }
  for (SEThermalCompartmentLink* link : m_OutgoingLinks) {
    // Positive flow on an outgoing path, is flow out of the compartment
    f = &link->GetHeatTransferRate();
    if (f->IsPositive() || f->IsZero())
      flow_W += f->GetValue(PowerUnit::W);
  }
  if (flow_W < 0)
    flow_W = 0; // This number is something like x.e-12, which we treat as 0
  return flow_W;
}
//-----------------------------------------------------------------------------
void SEThermalCompartment::AddLink(SEThermalCompartmentLink& link)
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
void SEThermalCompartment::RemoveLink(SEThermalCompartmentLink& link)
{
  Remove(m_Links, &link);
}
//-----------------------------------------------------------------------------
void SEThermalCompartment::RemoveLinks()
{
  m_Links.clear();
}
//-----------------------------------------------------------------------------
const std::vector<SEThermalCompartmentLink*>& SEThermalCompartment::GetLinks()
{
  return m_Links;
}
//-----------------------------------------------------------------------------
bool SEThermalCompartment::HasChild(const char* name)
{
  return HasChild(std::string{ name });
}
//-----------------------------------------------------------------------------
bool SEThermalCompartment::HasChild(const std::string& name)
{
  for (SEThermalCompartment* cmpt : m_Children) {
    if (name == cmpt->GetName())
      return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
void SEThermalCompartment::AddChild(SEThermalCompartment& child)
{
  if (HasNodeMapping()) {
    Fatal("You cannont add a child compartment to a compartment mapped to nodes");
    return;
  }
  if (HasChild(child.GetName()))
    return;
  m_Children.push_back(&child);
}
//-----------------------------------------------------------------------------
}