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

#include <biogears/cdm/circuit/electrical/SEElectricalCircuit.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuit.h>
#include <biogears/cdm/circuit/thermal/SEThermalCircuit.h>
#include <biogears/schema/cdm/Circuit.hxx>

namespace biogears {

template class SECircuitLedger<SEElectricalCircuitNode, SEElectricalCircuitPath, SEElectricalCircuit>;
template class SECircuitLedger<SEFluidCircuitNode, SEFluidCircuitPath, SEFluidCircuit>;
template class SECircuitLedger<SEThermalCircuitNode, SEThermalCircuitPath, SEThermalCircuit>;

SECircuitManager::SECircuitManager(Logger* logger)
  : Loggable(logger)
{
}
//-------------------------------------------------------------------------------
SECircuitManager::~SECircuitManager()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SECircuitManager::Clear()
{
  m_ElectricalLedger.Clear();
  m_FluidLedger.Clear();
  m_ThermalLedger.Clear();
}
//-------------------------------------------------------------------------------
bool SECircuitManager::Load(const CDM::CircuitManagerData& in)
{
  Clear();
  for (const CDM::ElectricalCircuitNodeData& n : in.ElectricalNode()) {
    CreateNode<ELECTRICAL_LEDGER_TYPES>(n.Name(), m_ElectricalLedger).Load(n);
  }
  for (const CDM::ElectricalCircuitPathData& p : in.ElectricalPath()) {
    SEElectricalCircuitNode* src = GetNode(p.SourceNode(), m_ElectricalLedger);
    if (src == nullptr) {
      Error(std::string { "Could not find source node " } + std::string { p.SourceNode() } + " from path " + p.Name().c_str());
      return false;
    }
    SEElectricalCircuitNode* tgt = GetNode(p.TargetNode(), m_ElectricalLedger);
    if (tgt == nullptr) {
      Error(std::string { "Could not find target node " } + std::string { p.TargetNode() } + " from path " + p.Name().c_str());
      return false;
    }
    CreatePath<ELECTRICAL_LEDGER_TYPES>(*src, *tgt, p.Name(), m_ElectricalLedger).Load(p);
  }
  for (const CDM::ElectricalCircuitData& c : in.ElectricalCircuit()) {
    CreateCircuit<ELECTRICAL_LEDGER_TYPES>(c.Name(), m_ElectricalLedger).Load(c, m_ElectricalLedger.nodes, m_ElectricalLedger.paths);
  }
  for (const CDM::FluidCircuitNodeData& n : in.FluidNode()) {
    CreateNode<FLUID_LEDGER_TYPES>(n.Name(), m_FluidLedger).Load(n);
  }
  for (const CDM::FluidCircuitPathData& p : in.FluidPath()) {
    SEFluidCircuitNode* src = GetNode(p.SourceNode(), m_FluidLedger);
    if (src == nullptr) {
      Error(std::string { "Could not find source node " } + std::string { p.SourceNode() } + " from path " + p.Name().c_str());
      return false;
    }
    SEFluidCircuitNode* tgt = GetNode(p.TargetNode(), m_FluidLedger);
    if (tgt == nullptr) {
      Error(std::string { "Could not find target node " } + std::string { p.TargetNode() } + " from path " + p.Name().c_str());
      return false;
    }
    CreatePath<FLUID_LEDGER_TYPES>(*src, *tgt, p.Name(), m_FluidLedger).Load(p);
  }
  for (const CDM::FluidCircuitData& c : in.FluidCircuit()) {
    CreateCircuit<FLUID_LEDGER_TYPES>(c.Name(), m_FluidLedger).Load(c, m_FluidLedger.nodes, m_FluidLedger.paths);
  }
  for (const CDM::ThermalCircuitNodeData& n : in.ThermalNode()) {
    CreateNode<THERMAL_LEDGER_TYPES>(n.Name(), m_ThermalLedger).Load(n);
  }
  for (const CDM::ThermalCircuitPathData& p : in.ThermalPath()) {
    SEThermalCircuitNode* src = GetNode(p.SourceNode(), m_ThermalLedger);
    if (src == nullptr) {
      Error(std::string { "Could not find source node " } + std::string { p.SourceNode() } + " from path " + p.Name().c_str());
      return false;
    }
    SEThermalCircuitNode* tgt = GetNode(p.TargetNode(), m_ThermalLedger);
    if (tgt == nullptr) {
      Error(std::string { "Could not find target node " } + std::string { p.TargetNode() } + " from path " + p.Name().c_str());
      return false;
    }
    CreatePath<THERMAL_LEDGER_TYPES>(*src, *tgt, p.Name(), m_ThermalLedger).Load(p);
  }
  for (const CDM::ThermalCircuitData& c : in.ThermalCircuit()) {
    CreateCircuit<THERMAL_LEDGER_TYPES>(c.Name(), m_ThermalLedger).Load(c, m_ThermalLedger.nodes, m_ThermalLedger.paths);
  }
  return true;
}
//-------------------------------------------------------------------------------
CDM::CircuitManagerData* SECircuitManager::Unload() const
{
  CDM::CircuitManagerData* data = new CDM::CircuitManagerData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SECircuitManager::Unload(CDM::CircuitManagerData& data) const
{
  for (auto itr : m_ElectricalLedger.nodes)
    data.ElectricalNode().push_back(std::unique_ptr<CDM::ElectricalCircuitNodeData>(itr.second->Unload()));
  for (auto itr : m_ElectricalLedger.paths)
    data.ElectricalPath().push_back(std::unique_ptr<CDM::ElectricalCircuitPathData>(itr.second->Unload()));
  for (auto itr : m_ElectricalLedger.circuits)
    data.ElectricalCircuit().push_back(std::unique_ptr<CDM::ElectricalCircuitData>(itr.second->Unload()));

  for (auto itr : m_FluidLedger.nodes)
    data.FluidNode().push_back(std::unique_ptr<CDM::FluidCircuitNodeData>(itr.second->Unload()));
  for (auto itr : m_FluidLedger.paths)
    data.FluidPath().push_back(std::unique_ptr<CDM::FluidCircuitPathData>(itr.second->Unload()));
  for (auto itr : m_FluidLedger.circuits)
    data.FluidCircuit().push_back(std::unique_ptr<CDM::FluidCircuitData>(itr.second->Unload()));

  for (auto itr : m_ThermalLedger.nodes)
    data.ThermalNode().push_back(std::unique_ptr<CDM::ThermalCircuitNodeData>(itr.second->Unload()));
  for (auto itr : m_ThermalLedger.paths)
    data.ThermalPath().push_back(std::unique_ptr<CDM::ThermalCircuitPathData>(itr.second->Unload()));
  for (auto itr : m_ThermalLedger.circuits)
    data.ThermalCircuit().push_back(std::unique_ptr<CDM::ThermalCircuitData>(itr.second->Unload()));
}
//-------------------------------------------------------------------------------
void SECircuitManager::SetReadOnly(bool b)
{
  SetReadOnlyElectrical(b);
  SetReadOnlyFluid(b);
  SetReadOnlyThermal(b);
}
//-------------------------------------------------------------------------------
void SECircuitManager::SetReadOnlyElectrical(bool b)
{
  for (auto nItr : m_ElectricalLedger.nodes) {
    if (nItr.second->HasQuantityBaseline())
      nItr.second->GetQuantityBaseline().SetReadOnly(b);
    if (nItr.second->HasQuantity())
      nItr.second->GetQuantity().SetReadOnly(b);

    if (nItr.second->HasPotential())
      nItr.second->GetPotential().SetReadOnly(b);
    if (nItr.second->HasNextPotential())
      nItr.second->GetNextPotential().SetReadOnly(b);
  }
  for (auto pItr : m_ElectricalLedger.paths) {
    if (pItr.second->HasCapacitanceBaseline()) {
      if (pItr.second->GetSourceNode().HasNextQuantity())
        pItr.second->GetSourceNode().GetNextQuantity().SetReadOnly(b);
      if (pItr.second->GetTargetNode().HasNextQuantity())
        pItr.second->GetTargetNode().GetNextQuantity().SetReadOnly(b);
    }
    if (pItr.second->HasFlux())
      pItr.second->GetFlux().SetReadOnly(b);
    if (pItr.second->HasNextFlux())
      pItr.second->GetNextFlux().SetReadOnly(b);
    // Lock all the current values
    if (pItr.second->HasFluxSource())
      pItr.second->GetFluxSource().SetReadOnly(b);
    if (pItr.second->HasResistance())
      pItr.second->GetResistance().SetReadOnly(b);
    if (pItr.second->HasCapacitance())
      pItr.second->GetCapacitance().SetReadOnly(b);
    if (pItr.second->HasInductance())
      pItr.second->GetInductance().SetReadOnly(b);
    if (pItr.second->HasPotentialSource())
      pItr.second->GetPotentialSource().SetReadOnly(b);
  }
}
//-------------------------------------------------------------------------------
void SECircuitManager::SetReadOnlyFluid(bool b)
{
  for (auto nItr : m_FluidLedger.nodes) {
    if (nItr.second->HasQuantityBaseline())
      nItr.second->GetQuantityBaseline().SetReadOnly(b);
    if (nItr.second->HasQuantity())
      nItr.second->GetQuantity().SetReadOnly(b);

    if (nItr.second->HasPotential())
      nItr.second->GetPotential().SetReadOnly(b);
    if (nItr.second->HasNextPotential())
      nItr.second->GetNextPotential().SetReadOnly(b);
  }
  for (auto pItr : m_FluidLedger.paths) {
    if (pItr.second->HasCapacitanceBaseline()) {
      if (pItr.second->GetSourceNode().HasNextQuantity())
        pItr.second->GetSourceNode().GetNextQuantity().SetReadOnly(b);
      if (pItr.second->GetTargetNode().HasNextQuantity())
        pItr.second->GetTargetNode().GetNextQuantity().SetReadOnly(b);
    }
    if (pItr.second->HasFlux())
      pItr.second->GetFlux().SetReadOnly(b);
    if (pItr.second->HasNextFlux())
      pItr.second->GetNextFlux().SetReadOnly(b);
    // Lock all the current values
    if (pItr.second->HasFluxSource())
      pItr.second->GetFluxSource().SetReadOnly(b);
    if (pItr.second->HasResistance())
      pItr.second->GetResistance().SetReadOnly(b);
    if (pItr.second->HasCapacitance())
      pItr.second->GetCapacitance().SetReadOnly(b);
    if (pItr.second->HasInductance())
      pItr.second->GetInductance().SetReadOnly(b);
    if (pItr.second->HasPotentialSource())
      pItr.second->GetPotentialSource().SetReadOnly(b);
  }
}
//-------------------------------------------------------------------------------
void SECircuitManager::SetReadOnlyThermal(bool b)
{
  for (auto nItr : m_ThermalLedger.nodes) {
    if (nItr.second->HasQuantityBaseline())
      nItr.second->GetQuantityBaseline().SetReadOnly(b);
    if (nItr.second->HasQuantity())
      nItr.second->GetQuantity().SetReadOnly(b);

    if (nItr.second->HasPotential())
      nItr.second->GetPotential().SetReadOnly(b);
    if (nItr.second->HasNextPotential())
      nItr.second->GetNextPotential().SetReadOnly(b);
  }
  for (auto pItr : m_ThermalLedger.paths) {
    if (pItr.second->HasCapacitanceBaseline()) {
      if (pItr.second->GetSourceNode().HasNextQuantity())
        pItr.second->GetSourceNode().GetNextQuantity().SetReadOnly(b);
      if (pItr.second->GetTargetNode().HasNextQuantity())
        pItr.second->GetTargetNode().GetNextQuantity().SetReadOnly(b);
    }
    if (pItr.second->HasFlux())
      pItr.second->GetFlux().SetReadOnly(b);
    if (pItr.second->HasNextFlux())
      pItr.second->GetNextFlux().SetReadOnly(b);
    // Lock all the current values
    if (pItr.second->HasFluxSource())
      pItr.second->GetFluxSource().SetReadOnly(b);
    if (pItr.second->HasResistance())
      pItr.second->GetResistance().SetReadOnly(b);
    if (pItr.second->HasCapacitance())
      pItr.second->GetCapacitance().SetReadOnly(b);
    if (pItr.second->HasInductance())
      pItr.second->GetInductance().SetReadOnly(b);
    if (pItr.second->HasPotentialSource())
      pItr.second->GetPotentialSource().SetReadOnly(b);
  }
}
//-------------------------------------------------------------------------------
}