#include "Circuit.h"

#include "Circuit.h"
#include "Property.h"

#include <biogears/cdm/circuit/SECircuit.h>

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
  //class SEElectricalCircuit
  void Circuit::Marshall(const CDM::ElectricalCircuitData& in, SEElectricalCircuit& out)
  {
    //Nothing to Do
  }
  //----------------------------------------------------------------------------------
  void Circuit::UnMarshall(const SEElectricalCircuit& in, CDM::ElectricalCircuitData& out)
  {
    //Nothing to Do
  }
  //----------------------------------------------------------------------------------
  //class SEElectricalCircuitNode
  void Circuit::Marshall(const CDM::ElectricalCircuitNodeData& in, SEElectricalCircuitNode& out)
  {
    Marshall(static_cast<const CDM::CircuitNodeData>(in), static_cast<SECircuitNode<SEScalarElectricPotential, SEScalarElectricCharge>&>(out));
    io::Property::Marshall(in.Voltage(), out.GetVoltage());
    io::Property::Marshall(in.NextVoltage(), out.GetNextVoltage());
    io::Property::Marshall(in.Charge(), out.GetCharge());
    io::Property::Marshall(in.NextCharge(), out.GetNextCharge());
    io::Property::Marshall(in.ChargeBaseline(), out.GetChargeBaseline());
  }
  //----------------------------------------------------------------------------------
  void Circuit::UnMarshall(const SEElectricalCircuitNode& in, CDM::ElectricalCircuitNodeData& out)
  {
    UnMarshall(static_cast<const SECircuitNode<SEScalarElectricPotential, SEScalarElectricCharge>&>(in), static_cast<CDM::CircuitNodeData&>(out));

    if (in.HasVoltage())
      io::Property::UnMarshall(*in.m_Potential, out.Voltage());
    if (in.HasNextVoltage())
      io::Property::UnMarshall(*in.m_NextPotential, out.NextVoltage());
    if (in.HasCharge())
      io::Property::UnMarshall(*in.m_Quantity, out.Charge());
    if (in.HasNextCharge())
      io::Property::UnMarshall(*in.m_NextQuantity, out.NextCharge());
    if (in.HasChargeBaseline())
      io::Property::UnMarshall(*in.m_QuantityBaseline, out.ChargeBaseline());
  }
  //----------------------------------------------------------------------------------
  //class SEElectricalCircuitPath
  void Circuit::Marshall(const CDM::ElectricalCircuitPathData& in, SEElectricalCircuitPath& out)
  {
    Marshall(static_cast<const CDM::CircuitPathData>(in),
             static_cast<SECircuitPath<SEScalarElectricCurrent, SEScalarElectricResistance, SEScalarElectricCapacitance, SEScalarElectricInductance, SEScalarElectricPotential, SEScalarElectricCharge>&>(out));

    io::Property::Marshall(in.Resistance(), out.GetResistance());
    io::Property::Marshall(in.NextResistance(), out.GetNextResistance());
    io::Property::Marshall(in.ResistanceBaseline(), out.GetResistanceBaseline());
    io::Property::Marshall(in.Capacitance(), out.GetCapacitance());
    io::Property::Marshall(in.NextCapacitance(), out.GetNextCapacitance());
    io::Property::Marshall(in.CapacitanceBaseline(), out.GetCapacitanceBaseline());
    io::Property::Marshall(in.Inductance(), out.GetInductance());
    io::Property::Marshall(in.NextInductance(), out.GetNextInductance());
    io::Property::Marshall(in.InductanceBaseline(), out.GetInductanceBaseline());
    io::Property::Marshall(in.Current(), out.GetCurrent());
    io::Property::Marshall(in.NextCurrent(), out.GetNextCurrent());
    io::Property::Marshall(in.CurrentSource(), out.GetCurrentSource());
    io::Property::Marshall(in.NextCurrentSource(), out.GetNextCurrentSource());
    io::Property::Marshall(in.CurrentSourceBaseline(), out.GetCurrentSourceBaseline());
    io::Property::Marshall(in.VoltageSource(), out.GetVoltageSource());
    io::Property::Marshall(in.NextVoltageSource(), out.GetNextVoltageSource());
    io::Property::Marshall(in.VoltageSourceBaseline(), out.GetVoltageSourceBaseline());
    io::Property::Marshall(in.ValveBreakdownVoltage(), out.GetValveBreakdownVoltage());
  }
  //----------------------------------------------------------------------------------
  void Circuit::UnMarshall(const SEElectricalCircuitPath& in, CDM::ElectricalCircuitPathData& out)
  {

    UnMarshall(static_cast<const SECircuitPath<SEScalarElectricCurrent, SEScalarElectricResistance, SEScalarElectricCapacitance, SEScalarElectricInductance, SEScalarElectricPotential, SEScalarElectricCharge>&>(in),
               static_cast<CDM::CircuitPathData&>(out));
    if (in.HasResistance()) {
      io::Property::UnMarshall(*in.m_Resistance, out.Resistance());
    }
    if (in.HasNextResistance()) {
      io::Property::UnMarshall(*in.m_NextResistance, out.NextResistance());
    }
    if (in.HasResistanceBaseline()) {
      io::Property::UnMarshall(*in.m_ResistanceBaseline, out.ResistanceBaseline());
    }
    if (in.HasCapacitance()) {
      io::Property::UnMarshall(*in.m_Capacitance, out.Capacitance());
    }
    if (in.HasNextCapacitance()) {
      io::Property::UnMarshall(*in.m_NextCapacitance, out.NextCapacitance());
    }
    if (in.HasCapacitanceBaseline()) {
      io::Property::UnMarshall(*in.m_CapacitanceBaseline, out.CapacitanceBaseline());
    }
    if (in.HasInductance()) {
      io::Property::UnMarshall(*in.m_Inductance, out.Inductance());
    }
    if (in.HasNextInductance()) {
      io::Property::UnMarshall(*in.m_NextInductance, out.NextInductance());
    }
    if (in.HasInductanceBaseline()) {
      io::Property::UnMarshall(*in.m_InductanceBaseline, out.InductanceBaseline());
    }
    if (in.HasCurrent()) {
      io::Property::UnMarshall(*in.m_Flux, out.Current());
    }
    if (in.HasNextCurrent()) {
      io::Property::UnMarshall(*in.m_NextFlux, out.NextCurrent());
    }
    if (in.HasCurrentSource()) {
      io::Property::UnMarshall(*in.m_FluxSource, out.CurrentSource());
    }
    if (in.HasNextCurrentSource()) {
      io::Property::UnMarshall(*in.m_NextFluxSource, out.NextCurrentSource());
    }
    if (in.HasCurrentSourceBaseline()) {
      io::Property::UnMarshall(*in.m_FluxSourceBaseline, out.CurrentSourceBaseline());
    }
    if (in.HasVoltageSource()) {
      io::Property::UnMarshall(*in.m_PotentialSource, out.VoltageSource());
    }
    if (in.HasNextVoltageSource()) {
      io::Property::UnMarshall(*in.m_NextPotentialSource, out.NextVoltageSource());
    }
    if (in.HasVoltageSourceBaseline()) {
      io::Property::UnMarshall(*in.m_PotentialSourceBaseline, out.VoltageSourceBaseline());
    }
    if (in.HasValveBreakdownVoltage()) {
      io::Property::UnMarshall(*in.m_ValveBreakdownPotential, out.ValveBreakdownVoltage());
    }
  }
  //----------------------------------------------------------------------------------
  //class SEFluidCircuit
  void Circuit::Marshall(const CDM::FluidCircuitData& in, SEFluidCircuit& out)
  {
    //Nothing to Do
  }
  //----------------------------------------------------------------------------------
  void Circuit::UnMarshall(const SEFluidCircuit& in, CDM::FluidCircuitData& out)
  {
    //Nothing To Do
  }
  //----------------------------------------------------------------------------------
  //class SEFluidCircuitNode
  void Circuit::Marshall(const CDM::FluidCircuitNodeData& in, SEFluidCircuitNode& out)
  {
    Marshall(static_cast<const CDM::CircuitNodeData&>(in), static_cast<SECircuitNode<FLUID_CIRCUIT_NODE>&>(out));
    io::Property::Marshall(in.Pressure(), out.GetPressure());
    io::Property::Marshall(in.NextPressure(), out.GetNextPressure());
    io::Property::Marshall(in.Volume(), out.GetVolume());
    ;
    io::Property::Marshall(in.NextVolume(), out.GetNextVolume());
    io::Property::Marshall(in.VolumeBaseline(), out.GetVolumeBaseline());
  }
  //----------------------------------------------------------------------------------
  void Circuit::UnMarshall(const SEFluidCircuitNode& in, CDM::FluidCircuitNodeData& out)
  {
    UnMarshall(static_cast<const SECircuitNode<FLUID_CIRCUIT_NODE>&>(in), static_cast<CDM::CircuitNodeData&>(out));
    if (in.HasPressure())
      io::Property::UnMarshall(*in.m_Potential, out.Pressure());
    if (in.HasNextPressure())
      io::Property::UnMarshall(*in.m_NextPotential, out.NextPressure());
    if (in.HasVolume())
      io::Property::UnMarshall(*in.m_Quantity, out.Volume());
    if (in.HasNextVolume())
      io::Property::UnMarshall(*in.m_NextQuantity, out.NextVolume());
    if (in.HasVolumeBaseline())
      io::Property::UnMarshall(*in.m_QuantityBaseline, out.VolumeBaseline());
  }
  //----------------------------------------------------------------------------------
  //class SEFluidCircuitPath
  void Circuit::Marshall(const CDM::FluidCircuitPathData& in, SEFluidCircuitPath& out)
  {

    Marshall(static_cast<const CDM::CircuitPathData&>(in), static_cast<SECircuitPath<FLUID_CIRCUIT_PATH>&>(out));
    io::Property::Marshall(in.Resistance(), out.GetResistance());
    io::Property::Marshall(in.NextResistance(), out.GetNextResistance());
    io::Property::Marshall(in.ResistanceBaseline(), out.GetResistanceBaseline());
    io::Property::Marshall(in.Compliance(), out.GetCompliance());
    io::Property::Marshall(in.NextCompliance(), out.GetNextCompliance());
    io::Property::Marshall(in.ComplianceBaseline(), out.GetComplianceBaseline());
    io::Property::Marshall(in.Inertance(), out.GetInertance());
    io::Property::Marshall(in.NextInertance(), out.GetNextInertance());
    io::Property::Marshall(in.InertanceBaseline(), out.GetInertanceBaseline());
    io::Property::Marshall(in.Flow(), out.GetFlow());
    io::Property::Marshall(in.NextFlow(), out.GetNextFlow());
    io::Property::Marshall(in.FlowSource(), out.GetFlowSource());
    io::Property::Marshall(in.NextFlowSource(), out.GetNextFlowSource());
    io::Property::Marshall(in.FlowSourceBaseline(), out.GetFlowSourceBaseline());
    io::Property::Marshall(in.PressureSource(), out.GetPressureSource());
    io::Property::Marshall(in.NextPressureSource(), out.GetNextPressureSource());
    io::Property::Marshall(in.PressureSourceBaseline(), out.GetPressureSourceBaseline());
    io::Property::Marshall(in.ValveBreakdownPressure(), out.GetValveBreakdownPressure());
  }
  //----------------------------------------------------------------------------------
  void Circuit::UnMarshall(const SEFluidCircuitPath& in, CDM::FluidCircuitPathData& out)
  {
    UnMarshall(static_cast<const SECircuitPath<FLUID_CIRCUIT_PATH>&>(in), static_cast<CDM::CircuitPathData&>(out));
    if (in.HasResistance()) {
      io::Property::UnMarshall(*in.m_Resistance, out.Resistance());
    }
    if (in.HasNextResistance()) {
      io::Property::UnMarshall(*in.m_NextResistance, out.NextResistance());
    }
    if (in.HasResistanceBaseline()) {
      io::Property::UnMarshall(*in.m_ResistanceBaseline, out.ResistanceBaseline());
    }
    if (in.HasCompliance()) {
      io::Property::UnMarshall(*in.m_Capacitance, out.Compliance());
    }
    if (in.HasNextCompliance()) {
      io::Property::UnMarshall(*in.m_NextCapacitance, out.NextCompliance());
    }
    if (in.HasComplianceBaseline()) {
      io::Property::UnMarshall(*in.m_CapacitanceBaseline, out.ComplianceBaseline());
    }
    if (in.HasInertance()) {
      io::Property::UnMarshall(*in.m_Inductance, out.Inertance());
    }
    if (in.HasNextInertance()) {
      io::Property::UnMarshall(*in.m_NextInductance, out.NextInertance());
    }
    if (in.HasInertanceBaseline()) {
      io::Property::UnMarshall(*in.m_InductanceBaseline, out.InertanceBaseline());
    }
    if (in.HasFlow()) {
      io::Property::UnMarshall(*in.m_Flux, out.Flow());
    }
    if (in.HasNextFlow()) {
      io::Property::UnMarshall(*in.m_NextFlux, out.NextFlow());
    }
    if (in.HasFlowSource()) {
      io::Property::UnMarshall(*in.m_FluxSource, out.FlowSource());
    }
    if (in.HasNextFlowSource()) {
      io::Property::UnMarshall(*in.m_NextFluxSource, out.NextFlowSource());
    }
    if (in.HasFlowSourceBaseline()) {
      io::Property::UnMarshall(*in.m_FluxSourceBaseline, out.FlowSourceBaseline());
    }
    if (in.HasPressureSource()) {
      io::Property::UnMarshall(*in.m_PotentialSource, out.PressureSource());
    }
    if (in.HasNextPressureSource()) {
      io::Property::UnMarshall(*in.m_NextPotentialSource, out.NextPressureSource());
    }
    if (in.HasPressureSourceBaseline()) {
      io::Property::UnMarshall(*in.m_PotentialSourceBaseline, out.PressureSourceBaseline());
    }
    if (in.HasValveBreakdownPressure()) {
      io::Property::UnMarshall(*in.m_ValveBreakdownPotential, out.ValveBreakdownPressure());
    }
  }
  //----------------------------------------------------------------------------------
  //class SEThermalCircuit
  void Circuit::Marshall(const CDM::ThermalCircuitData& in, SEThermalCircuit& out)
  {
    //Nothing to do
  }
  //----------------------------------------------------------------------------------
  void Circuit::UnMarshall(const SEThermalCircuit& in, CDM::ThermalCircuitData& out)
  {
    //Nothing to do
  }
  //----------------------------------------------------------------------------------
  //class SEThermalCircuitNode
  void Circuit::Marshall(const CDM::ThermalCircuitNodeData& in, SEThermalCircuitNode& out)
  {

    Marshall(static_cast<const CDM::CircuitNodeData>(in), static_cast<SECircuitNode<THERMAL_CIRCUIT_NODE>&>(out));
    io::Property::Marshall(in.Temperature(), out.GetTemperature());
    io::Property::Marshall(in.NextTemperature(), out.GetNextTemperature());
    io::Property::Marshall(in.Heat(), out.GetHeat());
    io::Property::Marshall(in.NextHeat(), out.GetNextHeat());
    io::Property::Marshall(in.HeatBaseline(), out.GetHeatBaseline());
  }
  //----------------------------------------------------------------------------------
  void Circuit::UnMarshall(const SEThermalCircuitNode& in, CDM::ThermalCircuitNodeData& out)
  {

    UnMarshall(static_cast<const SECircuitNode<THERMAL_CIRCUIT_NODE>&>(in), static_cast<CDM::CircuitNodeData>(out));
    if (in.HasTemperature())
      io::Property::UnMarshall(*in.m_Potential, out.Temperature());
    if (in.HasNextTemperature())
      io::Property::UnMarshall(*in.m_NextPotential, out.NextTemperature());
    if (in.HasHeat())
      io::Property::UnMarshall(*in.m_Quantity, out.Heat());
    if (in.HasNextHeat())
      io::Property::UnMarshall(*in.m_NextQuantity, out.NextHeat());
    if (in.HasHeatBaseline())
      io::Property::UnMarshall(*in.m_QuantityBaseline, out.HeatBaseline());
  }
  //----------------------------------------------------------------------------------
  //class SEThermalCircuitPath
  void Circuit::Marshall(const CDM::ThermalCircuitPathData& in, SEThermalCircuitPath& out)
  {
    Marshall(static_cast<const CDM::CircuitPathData&>(in), static_cast<SECircuitPath<THERMAL_CIRCUIT_PATH>&>(out));
    io::Property::Marshall(in.Resistance(), out.GetResistance());
    io::Property::Marshall(in.NextResistance(), out.GetNextResistance());
    io::Property::Marshall(in.ResistanceBaseline(), out.GetResistanceBaseline());
    io::Property::Marshall(in.Capacitance(), out.GetCapacitance());
    io::Property::Marshall(in.NextCapacitance(), out.GetNextCapacitance());
    io::Property::Marshall(in.CapacitanceBaseline(), out.GetCapacitanceBaseline());
    io::Property::Marshall(in.Inductance(), out.GetInductance());
    io::Property::Marshall(in.NextInductance(), out.GetNextInductance());
    io::Property::Marshall(in.InductanceBaseline(), out.GetInductanceBaseline());
    io::Property::Marshall(in.HeatTransferRate(), out.GetHeatTransferRate());
    io::Property::Marshall(in.NextHeatTransferRate(), out.GetNextHeatTransferRate());
    io::Property::Marshall(in.HeatSource(), out.GetHeatSource());
    io::Property::Marshall(in.NextHeatSource(), out.GetNextHeatSource());
    io::Property::Marshall(in.HeatSourceBaseline(), out.GetHeatSourceBaseline());
    io::Property::Marshall(in.TemperatureSource(), out.GetTemperatureSource());
    io::Property::Marshall(in.NextTemperatureSource(), out.GetNextTemperatureSource());
    io::Property::Marshall(in.TemperatureSourceBaseline(), out.GetTemperatureSourceBaseline());
    io::Property::Marshall(in.ValveBreakdownTemperature(), out.GetValveBreakdownTemperature());
  }
  //----------------------------------------------------------------------------------
  void Circuit::UnMarshall(const SEThermalCircuitPath& in, CDM::ThermalCircuitPathData& out)
  {
    UnMarshall(static_cast<const SECircuitPath<THERMAL_CIRCUIT_PATH>&>(in), static_cast<CDM::CircuitPathData&>(out));
    if (in.HasResistance()) {
      io::Property::UnMarshall(*in.m_Resistance, out.Resistance());
    }
    if (in.HasNextResistance()) {
      io::Property::UnMarshall(*in.m_NextResistance, out.NextResistance());
    }
    if (in.HasResistanceBaseline()) {
      io::Property::UnMarshall(*in.m_ResistanceBaseline, out.ResistanceBaseline());
    }
    if (in.HasCapacitance()) {
      io::Property::UnMarshall(*in.m_Capacitance, out.Capacitance());
    }
    if (in.HasNextCapacitance()) {
      io::Property::UnMarshall(*in.m_NextCapacitance, out.NextCapacitance());
    }
    if (in.HasCapacitanceBaseline()) {
      io::Property::UnMarshall(*in.m_CapacitanceBaseline, out.CapacitanceBaseline());
    }
    if (in.HasInductance()) {
      io::Property::UnMarshall(*in.m_Inductance, out.Inductance());
    }
    if (in.HasNextInductance()) {
      io::Property::UnMarshall(*in.m_NextInductance, out.NextInductance());
    }
    if (in.HasInductanceBaseline()) {
      io::Property::UnMarshall(*in.m_InductanceBaseline, out.InductanceBaseline());
    }
    if (in.HasHeatTransferRate()) {
      io::Property::UnMarshall(*in.m_Flux, out.HeatTransferRate());
    }
    if (in.HasNextHeatTransferRate()) {
      io::Property::UnMarshall(*in.m_NextFlux, out.NextHeatTransferRate());
    }
    if (in.HasHeatSource()) {
      io::Property::UnMarshall(*in.m_FluxSource, out.HeatSource());
    }
    if (in.HasNextHeatSource()) {
      io::Property::UnMarshall(*in.m_NextFluxSource, out.NextHeatSource());
    }
    if (in.HasHeatSourceBaseline()) {
      io::Property::UnMarshall(*in.m_FluxSourceBaseline, out.HeatSourceBaseline());
    }
    if (in.HasTemperatureSource()) {
      io::Property::UnMarshall(*in.m_PotentialSource, out.TemperatureSource());
    }
    if (in.HasNextTemperatureSource()) {
      io::Property::UnMarshall(*in.m_NextPotentialSource, out.NextTemperatureSource());
    }
    if (in.HasTemperatureSourceBaseline()) {
      io::Property::UnMarshall(*in.m_PotentialSourceBaseline, out.TemperatureSourceBaseline());
    }
    if (in.HasValveBreakdownTemperature()) {
      io::Property::UnMarshall(*in.m_ValveBreakdownPotential, out.ValveBreakdownTemperature());
    }
  }
  //----------------------------------------------------------------------------------
  //class SECircuitManager
  void Circuit::Marshall(const CDM::CircuitManagerData& in, SECircuitManager& out)
  {
    out.Clear();
    for (const CDM::ElectricalCircuitNodeData& n : in.ElectricalNode()) {
      io::Circuit::Marshall(n, out.CreateNode<ELECTRICAL_LEDGER_TYPES>(n.Name(), out.m_ElectricalLedger));
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
      io::Circuit::Marshall(p, out.CreatePath<ELECTRICAL_LEDGER_TYPES>(*src, *tgt, p.Name(), out.m_ElectricalLedger));
    }
    for (const CDM::ElectricalCircuitData& c : in.ElectricalCircuit()) {
      io::Circuit::Marshall(c,out.CreateCircuit<ELECTRICAL_LEDGER_TYPES>(c.Name(), out.m_ElectricalLedger), out.m_ElectricalLedger.nodes, out.m_ElectricalLedger.paths);
    }
    for (const CDM::FluidCircuitNodeData& n : in.FluidNode()) {
      io::Circuit::Marshall(n, out.CreateNode<FLUID_LEDGER_TYPES>(n.Name(), out.m_FluidLedger));
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
      io::Circuit::Marshall(p, out.CreatePath<FLUID_LEDGER_TYPES>(*src, *tgt, p.Name(), out.m_FluidLedger));
    }
    for (const CDM::FluidCircuitData& c : in.FluidCircuit())
      io::Circuit::Marshall(c, out.CreateCircuit<FLUID_LEDGER_TYPES>(c.Name(), out.m_FluidLedger));

    for (const CDM::ThermalCircuitNodeData& n : in.ThermalNode())
      io::Circuit::Marshall(n, out.CreateNode<THERMAL_LEDGER_TYPES>(n.Name(), out.m_ThermalLedger));
    for (const CDM::ThermalCircuitPathData& p : in.ThermalPath()) {
      SEThermalCircuitNode* src = out.GetNode(p.SourceNode(), out.m_ThermalLedger);
      if (src == nullptr) {
        throw CommonDataModelException(std::string { "Could not find source node " } + std::string { p.SourceNode() } + " from path " + p.Name().c_str());
      }
      SEThermalCircuitNode* tgt = out.GetNode(p.TargetNode(), out.m_ThermalLedger);
      if (tgt == nullptr) {
        throw CommonDataModelException(std::string { "Could not find target node " } + std::string { p.TargetNode() } + " from path " + p.Name().c_str());
      }
      io::Circuit::Marshall(p, out.CreatePath<THERMAL_LEDGER_TYPES>(*src, *tgt, p.Name(), out.m_ThermalLedger));
    }
    for (const CDM::ThermalCircuitData& c : in.ThermalCircuit())
      io::Circuit::Marshall(c, out.CreateCircuit<THERMAL_LEDGER_TYPES>(c.Name(), out.m_ThermalLedger));
  }
  //----------------------------------------------------------------------------------
  void Circuit::UnMarshall(const SECircuitManager& in, CDM::CircuitManagerData& out)
  {
    for (auto itr : in.m_ElectricalLedger.nodes) {
      auto ecnData = CDM::ElectricalCircuitNodeData();
      UnMarshall(*itr.second, ecnData);
      out.ElectricalNode().push_back(ecnData);
    }
    for (auto itr : in.m_ElectricalLedger.paths) {
      auto ecpData = CDM::ElectricalCircuitPathData();
      UnMarshall(*itr.second, ecpData);
      out.ElectricalPath().push_back(ecpData);
    }
    for (auto itr : in.m_ElectricalLedger.circuits) {
      auto ecData = CDM::ElectricalCircuitData();
      UnMarshall(*itr.second, ecData);
      out.ElectricalCircuit().push_back(ecData);
    }

    for (auto itr : in.m_FluidLedger.nodes) {
      auto fcnData = CDM::FluidCircuitNodeData();
      UnMarshall(*itr.second, fcnData);
      out.FluidNode().push_back(fcnData);
    }
    for (auto itr : in.m_FluidLedger.paths) {
      auto fcpData = CDM::FluidCircuitPathData();
      UnMarshall(*itr.second, fcpData);
      out.FluidPath().push_back(fcpData);
    }
    for (auto itr : in.m_FluidLedger.circuits) {
      auto fcData = CDM::FluidCircuitData();
      UnMarshall(*itr.second, fcData);
      out.FluidCircuit().push_back(fcData);
    }

    for (auto itr : in.m_ThermalLedger.nodes) {
      auto tcnData = CDM::ThermalCircuitNodeData();
      UnMarshall(*itr.second, tcnData);
      out.ThermalNode().push_back(tcnData);
    }
    for (auto itr : in.m_ThermalLedger.paths) {
      auto tcpData = CDM::ThermalCircuitPathData();
      UnMarshall(*itr.second, tcpData);
      out.ThermalPath().push_back(tcpData);
    }
    for (auto itr : in.m_ThermalLedger.circuits) {
      auto tcData = CDM::ThermalCircuitData();
      UnMarshall(*itr.second, tcData);
      out.ThermalCircuit().push_back(tcData);
    }
  }
  //----------------------------------------------------------------------------------
}
}