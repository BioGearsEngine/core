#include "Circuit.h"

#include "Circuit.h"
#include "Property.h"

#include <biogears/cdm/circuit/SECircuit.h>

#include <biogears/cdm/circuit/SECircuit.h>
#include <biogears/cdm/enums/SECircuitEnums.h>
#include <biogears/cdm/circuit/SECircuitManager.h>
#include <biogears/cdm/circuit/SECircuitNode.inl>
#include <biogears/cdm/circuit/SECircuitPath.h> 
#include <biogears/cdm/circuit/electrical/SEElectricalCircuit.h>
#include <biogears/cdm/circuit/electrical/SEElectricalCircuitNode.h>
#include <biogears/cdm/circuit/electrical/SEElectricalCircuitPath.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuit.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuitNode.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuitPath.h>
#include <biogears/cdm/circuit/thermal/SEThermalCircuit.h>
#include <biogears/cdm/circuit/thermal/SEThermalCircuitNode.h>
#include <biogears/cdm/circuit/thermal/SEThermalCircuitPath.h>
namespace biogears {
namespace io {
  // class SEElectricalCircuit
  void Circuit::UnMarshall(const CDM::ElectricalCircuitData& in, SECircuitLedger<ELECTRICAL_LEDGER_TYPES> const& ledger, SEElectricalCircuit& out)
  {
    // note: not clearing here as the derived class needs to clear and call this super class Load last to get the ref node hooked up
    out.Clear();
    out.m_Name = in.Name();
    for (auto name : in.Node()) {
      auto idx = ledger.nodes.find(name);
      if (idx == ledger.nodes.end()) {
        out.Error(out.m_Name + " could not find node " + name.c_str());
      }
      out.AddNode(*idx->second);
    }
    for (auto name : in.Path()) {
      auto idx = ledger.paths.find(name);
      if (idx == ledger.paths.end()) {
        out.Error(out.m_Name + " could not find path " + name.c_str());
      }
      out.AddPath(*idx->second);
    }
    for (auto name : in.ReferenceNode()) {
      auto idx = ledger.nodes.find(name);
      if (idx == ledger.nodes.end()) {
        out.Error(out.m_Name + " could not find reference node " + name.c_str());
      }
      out.AddReferenceNode(*idx->second);
    }
    out.StateChange();
  }
  //----------------------------------------------------------------------------------
  void Circuit::Marshall(const SEElectricalCircuit& in, CDM::ElectricalCircuitData& out)
  {
    out.Name(in.m_Name);
    if (in.HasReferenceNode()) {
      for (auto n : in.m_ReferenceNodes)
        out.ReferenceNode().push_back(n->GetName());
    }
    for (auto n : in.m_Nodes)
      out.Node().push_back(n->GetName());
    for (auto p : in.m_Paths)
      out.Path().push_back(p->GetName());
  }
  //----------------------------------------------------------------------------------
  // class SEElectricalCircuitNode
  void Circuit::UnMarshall(const CDM::ElectricalCircuitNodeData& in, SECircuitLedger<ELECTRICAL_LEDGER_TYPES> const& ledger, SEElectricalCircuitNode& out)
  {
    UnMarshall(static_cast<const CDM::CircuitNodeData>(in), static_cast<SECircuitNode<SEScalarElectricPotential, SEScalarElectricCharge>&>(out));
    io::Property::UnMarshall(in.Voltage(), out.GetVoltage());
    io::Property::UnMarshall(in.NextVoltage(), out.GetNextVoltage());
    io::Property::UnMarshall(in.Charge(), out.GetCharge());
    io::Property::UnMarshall(in.NextCharge(), out.GetNextCharge());
    io::Property::UnMarshall(in.ChargeBaseline(), out.GetChargeBaseline());
  }
  //----------------------------------------------------------------------------------
  void Circuit::Marshall(const SEElectricalCircuitNode& in, CDM::ElectricalCircuitNodeData& out)
  {
    Marshall(static_cast<const SECircuitNode<SEScalarElectricPotential, SEScalarElectricCharge>&>(in), static_cast<CDM::CircuitNodeData&>(out));

    if (in.HasVoltage())
      io::Property::Marshall(*in.m_Potential, out.Voltage());
    if (in.HasNextVoltage())
      io::Property::Marshall(*in.m_NextPotential, out.NextVoltage());
    if (in.HasCharge())
      io::Property::Marshall(*in.m_Quantity, out.Charge());
    if (in.HasNextCharge())
      io::Property::Marshall(*in.m_NextQuantity, out.NextCharge());
    if (in.HasChargeBaseline())
      io::Property::Marshall(*in.m_QuantityBaseline, out.ChargeBaseline());
  }
  //----------------------------------------------------------------------------------
  // class SEElectricalCircuitPath
  void Circuit::UnMarshall(const CDM::ElectricalCircuitPathData& in, SECircuitLedger<ELECTRICAL_LEDGER_TYPES> const& ledger, SEElectricalCircuitPath& out)
  {
    UnMarshall(static_cast<const CDM::CircuitPathData>(in),
               static_cast<SECircuitPath<SEScalarElectricCurrent, SEScalarElectricResistance, SEScalarElectricCapacitance, SEScalarElectricInductance, SEScalarElectricPotential, SEScalarElectricCharge>&>(out));

    io::Property::UnMarshall(in.Resistance(), out.GetResistance());
    io::Property::UnMarshall(in.NextResistance(), out.GetNextResistance());
    io::Property::UnMarshall(in.ResistanceBaseline(), out.GetResistanceBaseline());
    io::Property::UnMarshall(in.Capacitance(), out.GetCapacitance());
    io::Property::UnMarshall(in.NextCapacitance(), out.GetNextCapacitance());
    io::Property::UnMarshall(in.CapacitanceBaseline(), out.GetCapacitanceBaseline());
    io::Property::UnMarshall(in.Inductance(), out.GetInductance());
    io::Property::UnMarshall(in.NextInductance(), out.GetNextInductance());
    io::Property::UnMarshall(in.InductanceBaseline(), out.GetInductanceBaseline());
    io::Property::UnMarshall(in.Current(), out.GetCurrent());
    io::Property::UnMarshall(in.NextCurrent(), out.GetNextCurrent());
    io::Property::UnMarshall(in.CurrentSource(), out.GetCurrentSource());
    io::Property::UnMarshall(in.NextCurrentSource(), out.GetNextCurrentSource());
    io::Property::UnMarshall(in.CurrentSourceBaseline(), out.GetCurrentSourceBaseline());
    io::Property::UnMarshall(in.VoltageSource(), out.GetVoltageSource());
    io::Property::UnMarshall(in.NextVoltageSource(), out.GetNextVoltageSource());
    io::Property::UnMarshall(in.VoltageSourceBaseline(), out.GetVoltageSourceBaseline());
    io::Property::UnMarshall(in.ValveBreakdownVoltage(), out.GetValveBreakdownVoltage());
  }
  //----------------------------------------------------------------------------------
  void Circuit::Marshall(const SEElectricalCircuitPath& in, CDM::ElectricalCircuitPathData& out)
  {

    Marshall(static_cast<const SECircuitPath<SEScalarElectricCurrent, SEScalarElectricResistance, SEScalarElectricCapacitance, SEScalarElectricInductance, SEScalarElectricPotential, SEScalarElectricCharge>&>(in),
             static_cast<CDM::CircuitPathData&>(out));
    if (in.HasResistance()) {
      io::Property::Marshall(*in.m_Resistance, out.Resistance());
    }
    if (in.HasNextResistance()) {
      io::Property::Marshall(*in.m_NextResistance, out.NextResistance());
    }
    if (in.HasResistanceBaseline()) {
      io::Property::Marshall(*in.m_ResistanceBaseline, out.ResistanceBaseline());
    }
    if (in.HasCapacitance()) {
      io::Property::Marshall(*in.m_Capacitance, out.Capacitance());
    }
    if (in.HasNextCapacitance()) {
      io::Property::Marshall(*in.m_NextCapacitance, out.NextCapacitance());
    }
    if (in.HasCapacitanceBaseline()) {
      io::Property::Marshall(*in.m_CapacitanceBaseline, out.CapacitanceBaseline());
    }
    if (in.HasInductance()) {
      io::Property::Marshall(*in.m_Inductance, out.Inductance());
    }
    if (in.HasNextInductance()) {
      io::Property::Marshall(*in.m_NextInductance, out.NextInductance());
    }
    if (in.HasInductanceBaseline()) {
      io::Property::Marshall(*in.m_InductanceBaseline, out.InductanceBaseline());
    }
    if (in.HasCurrent()) {
      io::Property::Marshall(*in.m_Flux, out.Current());
    }
    if (in.HasNextCurrent()) {
      io::Property::Marshall(*in.m_NextFlux, out.NextCurrent());
    }
    if (in.HasCurrentSource()) {
      io::Property::Marshall(*in.m_FluxSource, out.CurrentSource());
    }
    if (in.HasNextCurrentSource()) {
      io::Property::Marshall(*in.m_NextFluxSource, out.NextCurrentSource());
    }
    if (in.HasCurrentSourceBaseline()) {
      io::Property::Marshall(*in.m_FluxSourceBaseline, out.CurrentSourceBaseline());
    }
    if (in.HasVoltageSource()) {
      io::Property::Marshall(*in.m_PotentialSource, out.VoltageSource());
    }
    if (in.HasNextVoltageSource()) {
      io::Property::Marshall(*in.m_NextPotentialSource, out.NextVoltageSource());
    }
    if (in.HasVoltageSourceBaseline()) {
      io::Property::Marshall(*in.m_PotentialSourceBaseline, out.VoltageSourceBaseline());
    }
    if (in.HasValveBreakdownVoltage()) {
      io::Property::Marshall(*in.m_ValveBreakdownPotential, out.ValveBreakdownVoltage());
    }
  }
  //----------------------------------------------------------------------------------
  // class SEFluidCircuit
  void Circuit::UnMarshall(const CDM::FluidCircuitData& in, SECircuitLedger<FLUID_LEDGER_TYPES> const& ledger, SEFluidCircuit& out)
  {
    // note: not clearing here as the derived class needs to clear and call this super class Load last to get the ref node hooked up
    out.Clear();
    out.m_Name = in.Name();
    for (auto name : in.Node()) {
      auto idx = ledger.nodes.find(name);
      if (idx == ledger.nodes.end()) {
        out.Error(out.m_Name + " could not find node " + name.c_str());
      }
      out.AddNode(*idx->second);
    }
    for (auto name : in.Path()) {
      auto idx = ledger.paths.find(name);
      if (idx == ledger.paths.end()) {
        out.Error(out.m_Name + " could not find path " + name.c_str());
      }
      out.AddPath(*idx->second);
    }
    for (auto name : in.ReferenceNode()) {
      auto idx = ledger.nodes.find(name);
      if (idx == ledger.nodes.end()) {
        out.Error(out.m_Name + " could not find reference node " + name.c_str());
      }
      out.AddReferenceNode(*idx->second);
    }
    out.StateChange();
  }
  //----------------------------------------------------------------------------------
  void Circuit::Marshall(const SEFluidCircuit& in, CDM::FluidCircuitData& out)
  {
    out.Name(in.m_Name);
    if (in.HasReferenceNode()) {
      for (auto n : in.m_ReferenceNodes)
        out.ReferenceNode().push_back(n->GetName());
    }
    for (auto n : in.m_Nodes)
      out.Node().push_back(n->GetName());
    for (auto p : in.m_Paths)
      out.Path().push_back(p->GetName());
  }
  //----------------------------------------------------------------------------------
  // class SEFluidCircuitNode
  void Circuit::UnMarshall(const CDM::FluidCircuitNodeData& in, SECircuitLedger<FLUID_LEDGER_TYPES> const& ledger, SEFluidCircuitNode& out)
  {
    UnMarshall(static_cast<const CDM::CircuitNodeData&>(in), static_cast<SECircuitNode<FLUID_CIRCUIT_NODE>&>(out));
    io::Property::UnMarshall(in.Pressure(), out.GetPressure());
    io::Property::UnMarshall(in.NextPressure(), out.GetNextPressure());
    io::Property::UnMarshall(in.Volume(), out.GetVolume());
    ;
    io::Property::UnMarshall(in.NextVolume(), out.GetNextVolume());
    io::Property::UnMarshall(in.VolumeBaseline(), out.GetVolumeBaseline());
  }
  //----------------------------------------------------------------------------------
  void Circuit::Marshall(const SEFluidCircuitNode& in, CDM::FluidCircuitNodeData& out)
  {
    Marshall(static_cast<const SECircuitNode<FLUID_CIRCUIT_NODE>&>(in), static_cast<CDM::CircuitNodeData&>(out));
    if (in.HasPressure())
      io::Property::Marshall(*in.m_Potential, out.Pressure());
    if (in.HasNextPressure())
      io::Property::Marshall(*in.m_NextPotential, out.NextPressure());
    if (in.HasVolume())
      io::Property::Marshall(*in.m_Quantity, out.Volume());
    if (in.HasNextVolume())
      io::Property::Marshall(*in.m_NextQuantity, out.NextVolume());
    if (in.HasVolumeBaseline())
      io::Property::Marshall(*in.m_QuantityBaseline, out.VolumeBaseline());
  }
  //----------------------------------------------------------------------------------
  // class SEFluidCircuitPath
  void Circuit::UnMarshall(const CDM::FluidCircuitPathData& in, SECircuitLedger<FLUID_LEDGER_TYPES> const& ledger, SEFluidCircuitPath& out)
  {

    UnMarshall(static_cast<const CDM::CircuitPathData&>(in), static_cast<SECircuitPath<FLUID_CIRCUIT_PATH>&>(out));
    io::Property::UnMarshall(in.Resistance(), out.GetResistance());
    io::Property::UnMarshall(in.NextResistance(), out.GetNextResistance());
    io::Property::UnMarshall(in.ResistanceBaseline(), out.GetResistanceBaseline());
    io::Property::UnMarshall(in.Compliance(), out.GetCompliance());
    io::Property::UnMarshall(in.NextCompliance(), out.GetNextCompliance());
    io::Property::UnMarshall(in.ComplianceBaseline(), out.GetComplianceBaseline());
    io::Property::UnMarshall(in.Inertance(), out.GetInertance());
    io::Property::UnMarshall(in.NextInertance(), out.GetNextInertance());
    io::Property::UnMarshall(in.InertanceBaseline(), out.GetInertanceBaseline());
    io::Property::UnMarshall(in.Flow(), out.GetFlow());
    io::Property::UnMarshall(in.NextFlow(), out.GetNextFlow());
    io::Property::UnMarshall(in.FlowSource(), out.GetFlowSource());
    io::Property::UnMarshall(in.NextFlowSource(), out.GetNextFlowSource());
    io::Property::UnMarshall(in.FlowSourceBaseline(), out.GetFlowSourceBaseline());
    io::Property::UnMarshall(in.PressureSource(), out.GetPressureSource());
    io::Property::UnMarshall(in.NextPressureSource(), out.GetNextPressureSource());
    io::Property::UnMarshall(in.PressureSourceBaseline(), out.GetPressureSourceBaseline());
    io::Property::UnMarshall(in.ValveBreakdownPressure(), out.GetValveBreakdownPressure());
  }
  //----------------------------------------------------------------------------------
  void Circuit::Marshall(const SEFluidCircuitPath& in, CDM::FluidCircuitPathData& out)
  {
    Marshall(static_cast<const SECircuitPath<FLUID_CIRCUIT_PATH>&>(in), static_cast<CDM::CircuitPathData&>(out));
    if (in.HasResistance()) {
      io::Property::Marshall(*in.m_Resistance, out.Resistance());
    }
    if (in.HasNextResistance()) {
      io::Property::Marshall(*in.m_NextResistance, out.NextResistance());
    }
    if (in.HasResistanceBaseline()) {
      io::Property::Marshall(*in.m_ResistanceBaseline, out.ResistanceBaseline());
    }
    if (in.HasCompliance()) {
      io::Property::Marshall(*in.m_Capacitance, out.Compliance());
    }
    if (in.HasNextCompliance()) {
      io::Property::Marshall(*in.m_NextCapacitance, out.NextCompliance());
    }
    if (in.HasComplianceBaseline()) {
      io::Property::Marshall(*in.m_CapacitanceBaseline, out.ComplianceBaseline());
    }
    if (in.HasInertance()) {
      io::Property::Marshall(*in.m_Inductance, out.Inertance());
    }
    if (in.HasNextInertance()) {
      io::Property::Marshall(*in.m_NextInductance, out.NextInertance());
    }
    if (in.HasInertanceBaseline()) {
      io::Property::Marshall(*in.m_InductanceBaseline, out.InertanceBaseline());
    }
    if (in.HasFlow()) {
      io::Property::Marshall(*in.m_Flux, out.Flow());
    }
    if (in.HasNextFlow()) {
      io::Property::Marshall(*in.m_NextFlux, out.NextFlow());
    }
    if (in.HasFlowSource()) {
      io::Property::Marshall(*in.m_FluxSource, out.FlowSource());
    }
    if (in.HasNextFlowSource()) {
      io::Property::Marshall(*in.m_NextFluxSource, out.NextFlowSource());
    }
    if (in.HasFlowSourceBaseline()) {
      io::Property::Marshall(*in.m_FluxSourceBaseline, out.FlowSourceBaseline());
    }
    if (in.HasPressureSource()) {
      io::Property::Marshall(*in.m_PotentialSource, out.PressureSource());
    }
    if (in.HasNextPressureSource()) {
      io::Property::Marshall(*in.m_NextPotentialSource, out.NextPressureSource());
    }
    if (in.HasPressureSourceBaseline()) {
      io::Property::Marshall(*in.m_PotentialSourceBaseline, out.PressureSourceBaseline());
    }
    if (in.HasValveBreakdownPressure()) {
      io::Property::Marshall(*in.m_ValveBreakdownPotential, out.ValveBreakdownPressure());
    }
  }
  //----------------------------------------------------------------------------------
  // class SEThermalCircuit
  void Circuit::UnMarshall(const CDM::ThermalCircuitData& in, SECircuitLedger<THERMAL_LEDGER_TYPES> const& ledger, SEThermalCircuit& out)
  {
    // note: not clearing here as the derived class needs to clear and call this super class Load last to get the ref node hooked up
    out.Clear();
    out.m_Name = in.Name();
    for (auto name : in.Node()) {
      auto idx = ledger.nodes.find(name);
      if (idx == ledger.nodes.end()) {
        out.Error(out.m_Name + " could not find node " + name.c_str());
      }
      out.AddNode(*idx->second);
    }
    for (auto name : in.Path()) {
      auto idx = ledger.paths.find(name);
      if (idx == ledger.paths.end()) {
        out.Error(out.m_Name + " could not find path " + name.c_str());
      }
      out.AddPath(*idx->second);
    }
    for (auto name : in.ReferenceNode()) {
      auto idx = ledger.nodes.find(name);
      if (idx == ledger.nodes.end()) {
        out.Error(out.m_Name + " could not find reference node " + name.c_str());
      }
      out.AddReferenceNode(*idx->second);
    }
    out.StateChange();
  }
  //----------------------------------------------------------------------------------
  void Circuit::Marshall(const SEThermalCircuit& in, CDM::ThermalCircuitData& out)
  {
    out.Name(in.m_Name);
    if (in.HasReferenceNode()) {
      for (auto n : in.m_ReferenceNodes)
        out.ReferenceNode().push_back(n->GetName());
    }
    for (auto n : in.m_Nodes)
      out.Node().push_back(n->GetName());
    for (auto p : in.m_Paths)
      out.Path().push_back(p->GetName());
  }
  //----------------------------------------------------------------------------------
  // class SEThermalCircuitNode
  void Circuit::UnMarshall(const CDM::ThermalCircuitNodeData& in, SECircuitLedger<THERMAL_LEDGER_TYPES> const& ledger, SEThermalCircuitNode& out)
  {

    UnMarshall(static_cast<const CDM::CircuitNodeData>(in), static_cast<SECircuitNode<THERMAL_CIRCUIT_NODE>&>(out));
    io::Property::UnMarshall(in.Temperature(), out.GetTemperature());
    io::Property::UnMarshall(in.NextTemperature(), out.GetNextTemperature());
    io::Property::UnMarshall(in.Heat(), out.GetHeat());
    io::Property::UnMarshall(in.NextHeat(), out.GetNextHeat());
    io::Property::UnMarshall(in.HeatBaseline(), out.GetHeatBaseline());
  }
  //----------------------------------------------------------------------------------
  void Circuit::Marshall(const SEThermalCircuitNode& in, CDM::ThermalCircuitNodeData& out)
  {

    Marshall(static_cast<const SECircuitNode<THERMAL_CIRCUIT_NODE>&>(in), static_cast<CDM::CircuitNodeData&>(out));
    if (in.HasTemperature())
      io::Property::Marshall(*in.m_Potential, out.Temperature());
    if (in.HasNextTemperature())
      io::Property::Marshall(*in.m_NextPotential, out.NextTemperature());
    if (in.HasHeat())
      io::Property::Marshall(*in.m_Quantity, out.Heat());
    if (in.HasNextHeat())
      io::Property::Marshall(*in.m_NextQuantity, out.NextHeat());
    if (in.HasHeatBaseline())
      io::Property::Marshall(*in.m_QuantityBaseline, out.HeatBaseline());
  }
  //----------------------------------------------------------------------------------
  // class SEThermalCircuitPath
  void Circuit::UnMarshall(const CDM::ThermalCircuitPathData& in, SECircuitLedger<THERMAL_LEDGER_TYPES> const& ledger, SEThermalCircuitPath& out)
  {
    UnMarshall(static_cast<const CDM::CircuitPathData&>(in), static_cast<SECircuitPath<THERMAL_CIRCUIT_PATH>&>(out));
    io::Property::UnMarshall(in.Resistance(), out.GetResistance());
    io::Property::UnMarshall(in.NextResistance(), out.GetNextResistance());
    io::Property::UnMarshall(in.ResistanceBaseline(), out.GetResistanceBaseline());
    io::Property::UnMarshall(in.Capacitance(), out.GetCapacitance());
    io::Property::UnMarshall(in.NextCapacitance(), out.GetNextCapacitance());
    io::Property::UnMarshall(in.CapacitanceBaseline(), out.GetCapacitanceBaseline());
    io::Property::UnMarshall(in.Inductance(), out.GetInductance());
    io::Property::UnMarshall(in.NextInductance(), out.GetNextInductance());
    io::Property::UnMarshall(in.InductanceBaseline(), out.GetInductanceBaseline());
    io::Property::UnMarshall(in.HeatTransferRate(), out.GetHeatTransferRate());
    io::Property::UnMarshall(in.NextHeatTransferRate(), out.GetNextHeatTransferRate());
    io::Property::UnMarshall(in.HeatSource(), out.GetHeatSource());
    io::Property::UnMarshall(in.NextHeatSource(), out.GetNextHeatSource());
    io::Property::UnMarshall(in.HeatSourceBaseline(), out.GetHeatSourceBaseline());
    io::Property::UnMarshall(in.TemperatureSource(), out.GetTemperatureSource());
    io::Property::UnMarshall(in.NextTemperatureSource(), out.GetNextTemperatureSource());
    io::Property::UnMarshall(in.TemperatureSourceBaseline(), out.GetTemperatureSourceBaseline());
    io::Property::UnMarshall(in.ValveBreakdownTemperature(), out.GetValveBreakdownTemperature());
  }
  //----------------------------------------------------------------------------------
  void Circuit::Marshall(const SEThermalCircuitPath& in, CDM::ThermalCircuitPathData& out)
  {
    Marshall(static_cast<const SECircuitPath<THERMAL_CIRCUIT_PATH>&>(in), static_cast<CDM::CircuitPathData&>(out));
    if (in.HasResistance()) {
      io::Property::Marshall(*in.m_Resistance, out.Resistance());
    }
    if (in.HasNextResistance()) {
      io::Property::Marshall(*in.m_NextResistance, out.NextResistance());
    }
    if (in.HasResistanceBaseline()) {
      io::Property::Marshall(*in.m_ResistanceBaseline, out.ResistanceBaseline());
    }
    if (in.HasCapacitance()) {
      io::Property::Marshall(*in.m_Capacitance, out.Capacitance());
    }
    if (in.HasNextCapacitance()) {
      io::Property::Marshall(*in.m_NextCapacitance, out.NextCapacitance());
    }
    if (in.HasCapacitanceBaseline()) {
      io::Property::Marshall(*in.m_CapacitanceBaseline, out.CapacitanceBaseline());
    }
    if (in.HasInductance()) {
      io::Property::Marshall(*in.m_Inductance, out.Inductance());
    }
    if (in.HasNextInductance()) {
      io::Property::Marshall(*in.m_NextInductance, out.NextInductance());
    }
    if (in.HasInductanceBaseline()) {
      io::Property::Marshall(*in.m_InductanceBaseline, out.InductanceBaseline());
    }
    if (in.HasHeatTransferRate()) {
      io::Property::Marshall(*in.m_Flux, out.HeatTransferRate());
    }
    if (in.HasNextHeatTransferRate()) {
      io::Property::Marshall(*in.m_NextFlux, out.NextHeatTransferRate());
    }
    if (in.HasHeatSource()) {
      io::Property::Marshall(*in.m_FluxSource, out.HeatSource());
    }
    if (in.HasNextHeatSource()) {
      io::Property::Marshall(*in.m_NextFluxSource, out.NextHeatSource());
    }
    if (in.HasHeatSourceBaseline()) {
      io::Property::Marshall(*in.m_FluxSourceBaseline, out.HeatSourceBaseline());
    }
    if (in.HasTemperatureSource()) {
      io::Property::Marshall(*in.m_PotentialSource, out.TemperatureSource());
    }
    if (in.HasNextTemperatureSource()) {
      io::Property::Marshall(*in.m_NextPotentialSource, out.NextTemperatureSource());
    }
    if (in.HasTemperatureSourceBaseline()) {
      io::Property::Marshall(*in.m_PotentialSourceBaseline, out.TemperatureSourceBaseline());
    }
    if (in.HasValveBreakdownTemperature()) {
      io::Property::Marshall(*in.m_ValveBreakdownPotential, out.ValveBreakdownTemperature());
    }
  }
  //----------------------------------------------------------------------------------
  // class SECircuitManager
  void Circuit::UnMarshall(const CDM::CircuitManagerData& in, SECircuitManager& out)
  {
    out.Clear();
    for (const CDM::ElectricalCircuitNodeData& n : in.ElectricalNode()) {
      io::Circuit::UnMarshall(n, out.m_ElectricalLedger, out.CreateNode<ELECTRICAL_LEDGER_TYPES>(n.Name(), out.m_ElectricalLedger));
    }
    for (const CDM::ElectricalCircuitPathData& p : in.ElectricalPath()) {
      SEElectricalCircuitNode* src = out.GetNode(p.SourceNode(), out.m_ElectricalLedger);
      if (src == nullptr) {
        throw CommonDataModelException(std::string { "Could not find source node " } + std::string { p.SourceNode() } + " from path " + p.Name().c_str());
      }
      SEElectricalCircuitNode* tgt = out.GetNode(p.TargetNode(), out.m_ElectricalLedger);
      if (tgt == nullptr) {
        throw CommonDataModelException(std::string { "Could not find target node " } + std::string { p.TargetNode() } + " from path " + p.Name().c_str());
      }
      io::Circuit::UnMarshall(p, out.m_ElectricalLedger, out.CreatePath<ELECTRICAL_LEDGER_TYPES>(*src, *tgt, p.Name(), out.m_ElectricalLedger));
    }
    for (const CDM::ElectricalCircuitData& c : in.ElectricalCircuit()) {
      io::Circuit::UnMarshall(c, out.m_ElectricalLedger, out.CreateCircuit<ELECTRICAL_LEDGER_TYPES>(c.Name(), out.m_ElectricalLedger));
    }
    for (const CDM::FluidCircuitNodeData& n : in.FluidNode()) {
      io::Circuit::UnMarshall(n, out.m_FluidLedger, out.CreateNode<FLUID_LEDGER_TYPES>(n.Name(), out.m_FluidLedger));
    }
    for (const CDM::FluidCircuitPathData& p : in.FluidPath()) {
      SEFluidCircuitNode* src = out.GetNode(p.SourceNode(), out.m_FluidLedger);
      if (src == nullptr) {
        throw CommonDataModelException(std::string { "Could not find source node " } + std::string { p.SourceNode() } + " from path " + p.Name().c_str());
      }
      SEFluidCircuitNode* tgt = out.GetNode(p.TargetNode(), out.m_FluidLedger);
      if (tgt == nullptr) {
        throw CommonDataModelException(std::string { "Could not find target node " } + std::string { p.TargetNode() } + " from path " + p.Name().c_str());
      }
      io::Circuit::UnMarshall(p, out.m_FluidLedger, out.CreatePath<FLUID_LEDGER_TYPES>(*src, *tgt, p.Name(), out.m_FluidLedger));
    }
    for (const CDM::FluidCircuitData& c : in.FluidCircuit()) {
      io::Circuit::UnMarshall(c, out.m_FluidLedger, out.CreateCircuit<FLUID_LEDGER_TYPES>(c.Name(), out.m_FluidLedger));
    }
    for (const CDM::ThermalCircuitNodeData& n : in.ThermalNode()) {
      io::Circuit::UnMarshall(n, out.m_ThermalLedger, out.CreateNode<THERMAL_LEDGER_TYPES>(n.Name(), out.m_ThermalLedger));
    }
    for (const CDM::ThermalCircuitPathData& p : in.ThermalPath()) {
      SEThermalCircuitNode* src = out.GetNode(p.SourceNode(), out.m_ThermalLedger);
      if (src == nullptr) {
        throw CommonDataModelException(std::string { "Could not find source node " } + std::string { p.SourceNode() } + " from path " + p.Name().c_str());
      }
      SEThermalCircuitNode* tgt = out.GetNode(p.TargetNode(), out.m_ThermalLedger);
      if (tgt == nullptr) {
        throw CommonDataModelException(std::string { "Could not find target node " } + std::string { p.TargetNode() } + " from path " + p.Name().c_str());
      }
      io::Circuit::UnMarshall(p, out.m_ThermalLedger, out.CreatePath<THERMAL_LEDGER_TYPES>(*src, *tgt, p.Name(), out.m_ThermalLedger));
    }
    for (const CDM::ThermalCircuitData& c : in.ThermalCircuit()) {
      io::Circuit::UnMarshall(c, out.m_ThermalLedger, out.CreateCircuit<THERMAL_LEDGER_TYPES>(c.Name(), out.m_ThermalLedger));
    }
  }

  //----------------------------------------------------------------------------------
  void Circuit::Marshall(const SECircuitManager& in, CDM::CircuitManagerData& out)
  {
     for (auto itr : in.m_ElectricalLedger.nodes) {
      auto ecnData = CDM::ElectricalCircuitNodeData();
      Marshall(*itr.second, ecnData);
      out.ElectricalNode().push_back(ecnData);
    }
    for (auto itr : in.m_ElectricalLedger.paths) {
      auto ecpData = CDM::ElectricalCircuitPathData();
      Marshall(*itr.second, ecpData);
      out.ElectricalPath().push_back(ecpData);
    }
    for (auto itr : in.m_ElectricalLedger.circuits) {
      auto ecData = CDM::ElectricalCircuitData();
      Marshall(*itr.second, ecData);
      out.ElectricalCircuit().push_back(ecData);
    }

    for (auto itr : in.m_FluidLedger.nodes) {
      auto fcnData = CDM::FluidCircuitNodeData();
      Marshall(*itr.second, fcnData);
      out.FluidNode().push_back(fcnData);
    }
    for (auto itr : in.m_FluidLedger.paths) {
      auto fcpData = CDM::FluidCircuitPathData();
      Marshall(*itr.second, fcpData);
      out.FluidPath().push_back(fcpData);
    }
    for (auto itr : in.m_FluidLedger.circuits) {
      auto fcData = CDM::FluidCircuitData();
      Marshall(*itr.second, fcData);
      out.FluidCircuit().push_back(fcData);
    }

    for (auto itr : in.m_ThermalLedger.nodes) {
      auto tcnData = CDM::ThermalCircuitNodeData();
      Marshall(*itr.second, tcnData);
      out.ThermalNode().push_back(tcnData);
    }
    for (auto itr : in.m_ThermalLedger.paths) {
      auto tcpData = CDM::ThermalCircuitPathData();
      Marshall(*itr.second, tcpData);
      out.ThermalPath().push_back(tcpData);
    }
    for (auto itr : in.m_ThermalLedger.circuits) {
      auto tcData = CDM::ThermalCircuitData();
      Marshall(*itr.second, tcData);
      out.ThermalCircuit().push_back(tcData);
    }
  }
  //----------------------------------------------------------------------------------
}
}
