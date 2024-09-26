/**************************************************************************************
Copyright 2019 Applied Research Associates, Inc.
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
#include <memory>

#include "Property.h"

#include "biogears/cdm/CommonDataModel.h"
#include <biogears/exports.h>

#include <biogears/cdm/circuit/SECircuit.h>
#include <biogears/cdm/enums/SECircuitEnums.h>
#include <biogears/cdm/circuit/SECircuitLedger.h>
#include <biogears/cdm/circuit/SECircuitNode.h>
#include <biogears/cdm/circuit/SECircuitPath.h>
#include <biogears/cdm/circuit/electrical/SEElectricalCircuit.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuit.h>
#include <biogears/cdm/circuit/thermal/SEThermalCircuit.h>
#include <biogears/schema/cdm/Circuit.hxx>
#include <biogears/cdm/circuit/SECircuit.inl>

#define CDM_CIRCUIT_MARSHALL_HELPER(in, out, func)                                   \
  if (in.m_##func) {                                                                 \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::Circuit::Marshall(*in.m_##func, out.func());                                 \
  }

#define CDM_OPTIONAL_CIRCUIT_MARSHALL_HELPER(in, out, func) \
  if (in.m_##func) {                                        \
    io::Circuit::Marshall(*in.m_##func, out.func());        \
  }

namespace biogears {
template <CIRCUIT_NODE_TEMPLATE>
class SECircuitNode;
template <CIRCUIT_PATH_TEMPLATE>
class SECircuitPath;
class SEElectricalCircuit;
class SEElectricalCircuitNode;
class SEElectricalCircuitPath;
class SEFluidCircuit;
class SEFluidCircuitNode;
class SEFluidCircuitPath;
class SEThermalCircuit;
class SEThermalCircuitNode;
class SEThermalCircuitPath;
class SECircuitManager;

namespace io {
  class BIOGEARS_PRIVATE_API Circuit {
  public:
    // template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    // class SECircuit
    template <typename CircuitBindType, typename NodeType, typename PathType>
    static void UnMarshall(const CircuitBindType& in, SECircuit<CIRCUIT_TYPES>& out, const std::map<std::string, NodeType*>& nodes, const std::map<std::string, PathType*>& paths);
    template <typename CircuitBindType, typename NodeType, typename PathType>
    static void Marshall(const SECircuit<CIRCUIT_TYPES>& in, CircuitBindType& out);
    // class SECircuitNode
    template <CIRCUIT_NODE_TEMPLATE>
    static void UnMarshall(const CDM::CircuitNodeData& in, SECircuitNode<CIRCUIT_NODE_TYPES>& out);
    template <CIRCUIT_NODE_TEMPLATE>
    static void Marshall(const SECircuitNode<CIRCUIT_NODE_TYPES>& in, CDM::CircuitNodeData& out);
    // class SECircuitPath
    template <CIRCUIT_PATH_TEMPLATE>
    static void UnMarshall(const CDM::CircuitPathData& in, SECircuitPath<CIRCUIT_PATH_TYPES>& out);
    template <CIRCUIT_PATH_TEMPLATE>
    static void Marshall(const SECircuitPath<CIRCUIT_PATH_TYPES>& in, CDM::CircuitPathData& out);

    // class SEElectricalCircuit
    static void UnMarshall(const CDM::ElectricalCircuitData& in, SECircuitLedger<ELECTRICAL_LEDGER_TYPES> const&, SEElectricalCircuit& out);
    static void Marshall(const SEElectricalCircuit& in, CDM::ElectricalCircuitData& out);
    // class SEElectricalCircuitNode
    static void UnMarshall(const CDM::ElectricalCircuitNodeData& in, SECircuitLedger<ELECTRICAL_LEDGER_TYPES> const&, SEElectricalCircuitNode& out);
    static void Marshall(const SEElectricalCircuitNode& in, CDM::ElectricalCircuitNodeData& out);
    // class SEElectricalCircuitPath
    static void UnMarshall(const CDM::ElectricalCircuitPathData& in, SECircuitLedger<ELECTRICAL_LEDGER_TYPES> const&, SEElectricalCircuitPath& out);
    static void Marshall(const SEElectricalCircuitPath& in, CDM::ElectricalCircuitPathData& out);
    // class SEFluidCircuit
    static void UnMarshall(const CDM::FluidCircuitData& in, SECircuitLedger<FLUID_LEDGER_TYPES> const&, SEFluidCircuit& out);
    static void Marshall(const SEFluidCircuit& in, CDM::FluidCircuitData& out);
    // class SEFluidCircuitNode
    static void UnMarshall(const CDM::FluidCircuitNodeData& in, SECircuitLedger<FLUID_LEDGER_TYPES> const&, SEFluidCircuitNode& out);
    static void Marshall(const SEFluidCircuitNode& in, CDM::FluidCircuitNodeData& out);
    // class SEFluidCircuitPath
    static void UnMarshall(const CDM::FluidCircuitPathData& in, SECircuitLedger<FLUID_LEDGER_TYPES> const&, SEFluidCircuitPath& out);
    static void Marshall(const SEFluidCircuitPath& in, CDM::FluidCircuitPathData& out);
    // class SEThermalCircuit
    static void UnMarshall(const CDM::ThermalCircuitData& in, SECircuitLedger<THERMAL_LEDGER_TYPES> const&, SEThermalCircuit& out);
    static void Marshall(const SEThermalCircuit& in, CDM::ThermalCircuitData& out);
    // class SEThermalCircuitNode
    static void UnMarshall(const CDM::ThermalCircuitNodeData& in, SECircuitLedger<THERMAL_LEDGER_TYPES> const&, SEThermalCircuitNode& out);
    static void Marshall(const SEThermalCircuitNode& in, CDM::ThermalCircuitNodeData& out);
    // class SEThermalCircuitPath
    static void UnMarshall(const CDM::ThermalCircuitPathData& in, SECircuitLedger<THERMAL_LEDGER_TYPES> const&, SEThermalCircuitPath& out);
    static void Marshall(const SEThermalCircuitPath& in, CDM::ThermalCircuitPathData& out);
    // class SECircuitManager
    static void UnMarshall(const CDM::CircuitManagerData& in, SECircuitManager& out);
    static void Marshall(const SECircuitManager& in, CDM::CircuitManagerData& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Circuit::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      UnMarshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Circuit::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    Marshall(in, *item);
    option_out.set(*item);
  }
  // class SECircuit
  template <typename CircuitBindType, typename NodeType, typename PathType>
  void Circuit::UnMarshall(const CircuitBindType& in, SECircuit<CIRCUIT_TYPES>& out, const std::map<std::string, NodeType*>& nodes, const std::map<std::string, PathType*>& paths)
  {
    out.Clear();
    out.m_Name = in.Name();
    for (auto name : in.Node()) {
      auto idx = nodes.find(name);
      if (idx == nodes.end()) {
        throw CommonDataModelException(out.m_Name + " could not find node " + name.c_str());
      }
      out.AddNode(*idx->second);
    }
    for (auto name : in.Path()) {
      auto idx = paths.find(name);
      if (idx == paths.end()) {
        throw CommonDataModelException(out.m_Name + " could not find path " + name.c_str());
      }
      out.AddPath(*idx->second);
    }
    for (auto name : in.ReferenceNode()) {
      auto idx = nodes.find(name);
      if (idx == nodes.end()) {
        throw CommonDataModelException(out.m_Name + " could not find reference node " + name.c_str());
      }
      out.AddReferenceNode(*idx->second);
    }
    out.StateChange();
  }
  //----------------------------------------------------------------------------------
  template <typename CircuitBindType, typename NodeType, typename PathType >
  void Circuit::Marshall(const SECircuit<CIRCUIT_TYPES>& in, CircuitBindType& out)
  {
    out.Name(in.m_Name);
    if (in.HasReferenceNode()) {
      for (NodeType* n : in.m_ReferenceNodes)
        out.ReferenceNode().push_back(n->GetName());
    }
    for (auto* n : in.m_Nodes)
      out.Node().push_back(n->GetName());
    for (auto* p : in.m_Paths)
      out.Path().push_back(p->GetName());
  }
  //----------------------------------------------------------------------------------
  // class SECircuitNode
  template <CIRCUIT_NODE_TEMPLATE>
  void Circuit::UnMarshall(const CDM::CircuitNodeData& in, SECircuitNode<CIRCUIT_NODE_TYPES>& out)
  {
    out.Clear();
  }
  //----------------------------------------------------------------------------------
  template <CIRCUIT_NODE_TEMPLATE>
  void Circuit::Marshall(const SECircuitNode<CIRCUIT_NODE_TYPES>& in, CDM::CircuitNodeData& out)
  {
    out.Name(in.m_Name);
  }
  //----------------------------------------------------------------------------------
  // class SECircuitPath
  template <CIRCUIT_PATH_TEMPLATE>
  void Circuit::UnMarshall(const CDM::CircuitPathData& in, SECircuitPath<CIRCUIT_PATH_TYPES>& out)
  {
    // out.Clear();

    io::Property::UnMarshall(in.Switch(), out.m_Switch);
    io::Property::UnMarshall(in.NextSwitch(), out.m_NextSwitch);
    io::Property::UnMarshall(in.Valve(), out.m_Valve);
    io::Property::UnMarshall(in.NextValve(), out.m_NextValve);
    io::Property::UnMarshall(in.PolarizedState(), out.m_PolarizedState);
    io::Property::UnMarshall(in.NextPolarizedState(), out.m_NextPolarizedState);
  }
  //----------------------------------------------------------------------------------
  template <CIRCUIT_PATH_TEMPLATE>
  void Circuit::Marshall(const SECircuitPath<CIRCUIT_PATH_TYPES>& in, CDM::CircuitPathData& out)
  {
    out.Name(in.m_Name);
    out.SourceNode(in.m_SourceNode.GetName());
    out.TargetNode(in.m_TargetNode.GetName());

    if (in.HasSwitch()) {
      io::Property::Marshall(in.m_Switch, out.Switch());
    }
    if (in.HasNextSwitch()) {
      io::Property::Marshall(in.m_NextSwitch, out.NextSwitch());
    }
    if (in.HasValve()) {
      io::Property::Marshall(in.m_Valve, out.Valve());
    }
    if (in.HasNextValve()) {
      io::Property::Marshall(in.m_NextValve, out.NextValve());
    }
    if (in.HasPolarizedState()) {
      io::Property::Marshall(in.m_PolarizedState, out.PolarizedState());
    }
    if (in.HasNextPolarizedState()) {
      io::Property::Marshall(in.m_NextPolarizedState, out.NextPolarizedState());
    }
  }
  //----------------------------------------------------------------------------------
} // Namespace IO
} // Namespace Biogears
