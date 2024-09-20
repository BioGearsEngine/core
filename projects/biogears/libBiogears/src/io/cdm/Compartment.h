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

#include <biogears/exports.h>

#include <biogears/cdm/CommonDataModel.h>

#include <biogears/cdm/circuit/SECircuit.inl>
#include <biogears/cdm/circuit/SECircuitNode.inl>
#include <biogears/cdm/compartment/SECompartmentGraph.h>
#include <biogears/cdm/compartment/SECompartmentNodes.inl>
#include <biogears/cdm/compartment/fluid/SEFluidCompartment.h>
#include <biogears/cdm/compartment/fluid/SEFluidCompartment.inl>
#include <biogears/cdm/compartment/fluid/SEFluidCompartmentLink.h>
#include <biogears/cdm/enums/SECompartmentEnums.h>
#include <biogears/schema/cdm/Compartment.hxx>

#define CDM_COMPARTMENT_MARSHALL_HELPER(in, out, func)                               \
  if (in.m_##func) {                                                                 \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::Compartment::Marshall(*in.m_##func, out.func());                             \
  }

#define CDM_OPTIONAL_COMPARTMENT_MARSHALL_HELPER(in, out, func) \
  if (in.m_##func) {                                            \
    io::Compartment::Marshall(*in.m_##func, out.func());        \
  }

namespace biogears {

class SECompartment;
class SECompartmentLink;

template <FLUID_COMPARTMENT_TEMPLATE>
class SEFluidCompartment;

template <FLUID_COMPARTMENT_LINK_TEMPLATE>
class SEFluidCompartmentLink;

class SECompartmentManager;
class SEGasCompartment;
class SEGasCompartmentGraph;
class SEGasCompartmentLink;
class SELiquidCompartment;
class SELiquidCompartmentGraph;
class SELiquidCompartmentLink;
class SESubstanceManager;
class SEThermalCompartment;
class SEThermalCompartmentLink;
class SETissueCompartment;

namespace io {
  class BIOGEARS_PRIVATE_API Compartment {
  public:
    // template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    // class SECompartment
    static void UnMarshall(const CDM::CompartmentData& in, SECompartment& out);
    static void Marshall(const SECompartment& in, CDM::CompartmentData& out);
    // class SECompartmentLink
    static void UnMarshall(const CDM::CompartmentLinkData& in, SECompartmentLink& out);
    static void Marshall(const SECompartmentLink& in, CDM::CompartmentLinkData& out);
    // class SEFluidCompartment
    template <FLUID_COMPARTMENT_TEMPLATE>
    static void UnMarshall(const CDM::FluidCompartmentData& in, SEFluidCompartment<FLUID_COMPARTMENT_TYPES>& out, SECircuitManager* circuits = nullptr);
    template <FLUID_COMPARTMENT_TEMPLATE>
    static void Marshall(const SEFluidCompartment<FLUID_COMPARTMENT_TYPES>& in, CDM::FluidCompartmentData& out);
    // class SEFluidCompartmentLink
    template <FLUID_COMPARTMENT_LINK_TEMPLATE>
    static void UnMarshall(const CDM::FluidCompartmentLinkData& in, SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>& out, SECircuitManager* circuits = nullptr);
    template <FLUID_COMPARTMENT_LINK_TEMPLATE>
    static void Marshall(const SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>& in, CDM::FluidCompartmentLinkData& out);
    // class SEGasCompartment
    static void UnMarshall(const CDM::GasCompartmentData& in, SEGasCompartment& out, SESubstanceManager& subMgr, SECircuitManager* circuits = nullptr);
    static void Marshall(const SEGasCompartment& in, CDM::GasCompartmentData& out);
    // class SEGasCompartmentLink
    static void UnMarshall(const CDM::GasCompartmentLinkData& in, SEGasCompartmentLink& out);
    static void Marshall(const SEGasCompartmentLink& in, CDM::GasCompartmentLinkData& out);
    // class SEGasCompartmentGraph
    static void UnMarshall(const CDM::GasCompartmentGraphData& in, SEGasCompartmentGraph& out, SECompartmentManager& cmptMgr);
    static void Marshall(const SEGasCompartmentGraph& in, CDM::GasCompartmentGraphData& out);
    // class SELiquidCompartment
    static void UnMarshall(const CDM::LiquidCompartmentData& in, SELiquidCompartment& out, SESubstanceManager& subMgr, SECircuitManager* circuits);
    static void Marshall(const SELiquidCompartment& in, CDM::LiquidCompartmentData& out);
    // class SELiquidCompartmentLink
    static void UnMarshall(const CDM::LiquidCompartmentLinkData& in, SELiquidCompartmentLink& out);
    static void Marshall(const SELiquidCompartmentLink& in, CDM::LiquidCompartmentLinkData& out);
    // class SELiquidCompartmentGraph
    static void UnMarshall(const CDM::LiquidCompartmentGraphData& in, SELiquidCompartmentGraph& out, SECompartmentManager& cmptMgr);
    static void Marshall(const SELiquidCompartmentGraph& in, CDM::LiquidCompartmentGraphData& out);
    // class SETissueCompartment
    static void UnMarshall(const CDM::TissueCompartmentData& in, SETissueCompartment& out, SESubstanceManager& subMgr, SECircuitManager* circuits = nullptr);
    static void Marshall(const SETissueCompartment& in, CDM::TissueCompartmentData& out);
    // class SEThermalCompartment
    static void UnMarshall(const CDM::ThermalCompartmentData& in, SEThermalCompartment& out, SECircuitManager* circuits = nullptr);
    static void Marshall(SEThermalCompartment& in, CDM::ThermalCompartmentData& out);
    // class SEThermalCompartmentLink
    static void UnMarshall(const CDM::ThermalCompartmentLinkData& in, SEThermalCompartmentLink& out, SECircuitManager* circuits = nullptr);
    static void Marshall(const SEThermalCompartmentLink& in, CDM::ThermalCompartmentLinkData& out);
    // class SECompartmentManager
    static void UnMarshall(CDM::CompartmentManagerData const& in, SECompartmentManager& out, SECircuitManager* circuits = nullptr);
    static void Marshall(const SECompartmentManager& in, CDM::CompartmentManagerData& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Compartment::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Invalidate();
    } else {
      UnMarshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Compartment::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    Marshall(in, *item);
    option_out.set(*item);
  }

} // Namespace IO
} // Namespace Biogears
