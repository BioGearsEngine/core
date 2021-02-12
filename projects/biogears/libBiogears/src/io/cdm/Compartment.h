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

#include <biogears/cdm/compartment/SECompartmentGraph.h>
#include <biogears/cdm/compartment/fluid/SEFluidCompartment.h>
#include <biogears/cdm/compartment/fluid/SEFluidCompartmentLink.h>

#include <biogears/schema/cdm/Compartment.hxx>

namespace biogears {

class SECompartment;
class SECompartmentLink;

template <FLUID_COMPARTMENT_TEMPLATE>
class SEFluidCompartment;

template <FLUID_COMPARTMENT_LINK_TEMPLATE>
class SEFluidCompartmentLink;

class SEGasCompartment;
class SEGasCompartmentLink;
class SEGasCompartmentGraph;
class SELiquidCompartment;
class SELiquidCompartmentLink;
class SELiquidCompartmentGraph;
class SETissueCompartment;
class SEThermalCompartment;
class SEThermalCompartmentLink;
class SECompartmentManager;

#define CDM_COMPARTMENT_UNMARSHAL_HELPER(xsd, func)                                  \
  if (m_##func) {                                                                    \
    xsd.func(std::make_unique<std::remove_reference<decltype(xsd.func())>::type>()); \
    io::Property::UnMarshall(*m_##func, xsd.func());                                 \
  }
namespace io {
  class BIOGEARS_PRIVATE_API Compartment {
  public:
    //template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    //class SECompartment
    static void Marshall(const CDM::CompartmentData& in, SECompartment& out);
    static void UnMarshall(const SECompartment& in, CDM::CompartmentData& out);
    //class SECompartmentLink
    static void Marshall(const CDM::CompartmentLinkData& in, SECompartmentLink& out);
    static void UnMarshall(const SECompartmentLink& in, CDM::CompartmentLinkData& out);
    //class SEFluidCompartment
    template <FLUID_COMPARTMENT_TEMPLATE>
    static void Marshall(const CDM::FluidCompartmentData& in, SEFluidCompartment<FLUID_COMPARTMENT_TYPES>& out, SECircuitManager* circuits = nullptr);
    template <FLUID_COMPARTMENT_TEMPLATE>
    static void UnMarshall(const SEFluidCompartment<FLUID_COMPARTMENT_TYPES>& in, CDM::FluidCompartmentData& out);
    //class SEFluidCompartmentLink
    template <FLUID_COMPARTMENT_LINK_TEMPLATE>
    static void Marshall(const CDM::FluidCompartmentLinkData& in, SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>& out, SECircuitManager* circuits = nullptr);
    template <FLUID_COMPARTMENT_LINK_TEMPLATE>
    static void UnMarshall(const SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>& in, CDM::FluidCompartmentLinkData& out);
    //class SEGasCompartment
    static void Marshall(const CDM::GasCompartmentData& in, SEGasCompartment& out, SESubstanceManager& subMgr, SECircuitManager* circuits = nullptr);
    static void UnMarshall(const SEGasCompartment& in, CDM::GasCompartmentData& out);
    //class SEGasCompartmentLink
    static void Marshall(const CDM::GasCompartmentLinkData& in, SEGasCompartmentLink& out);
    static void UnMarshall(const SEGasCompartmentLink& in, CDM::GasCompartmentLinkData& out);
    //class SEGasCompartmentGraph
    static void Marshall(const CDM::GasCompartmentGraphData& in, SEGasCompartmentGraph& out, SECompartmentManager& cmptMgr);
    static void UnMarshall(const SEGasCompartmentGraph& in, CDM::GasCompartmentGraphData& out);
    //class SELiquidCompartment
    static void Marshall(const CDM::LiquidCompartmentData& in, SELiquidCompartment& out, SESubstanceManager& subMgr, SECircuitManager* circuits);
    static void UnMarshall(const SELiquidCompartment& in, CDM::LiquidCompartmentData& out);
    //class SELiquidCompartmentLink
    static void Marshall(const CDM::LiquidCompartmentLinkData& in, SELiquidCompartmentLink& out);
    static void UnMarshall(const SELiquidCompartmentLink& in, CDM::LiquidCompartmentLinkData& out);
    //class SELiquidCompartmentGraph
    static void Marshall(const CDM::LiquidCompartmentGraphData& in, SELiquidCompartmentGraph& out, SECompartmentManager& cmptMgr);
    static void UnMarshall(const SELiquidCompartmentGraph& in, CDM::LiquidCompartmentGraphData& out);
    //class SETissueCompartment
    static void Marshall(const CDM::TissueCompartmentData& in, SETissueCompartment& out, SESubstanceManager& subMgr, SECircuitManager* circuits = nullptr);
    static void UnMarshall(const SETissueCompartment& in, CDM::TissueCompartmentData& out);
    //class SEThermalCompartment
    static void Marshall(const CDM::ThermalCompartmentData& in, SEThermalCompartment& out, SECircuitManager* circuits = nullptr);
    static void UnMarshall(SEThermalCompartment& in, CDM::ThermalCompartmentData& out);
    //class SEThermalCompartmentLink
    static void Marshall(const CDM::ThermalCompartmentLinkData& in, SEThermalCompartmentLink& out, SECircuitManager* circuits = nullptr);
    static void UnMarshall(const SEThermalCompartmentLink& in, CDM::ThermalCompartmentLinkData& out);
    //class SECompartmentManager
    static void Marshall(CDM::CompartmentManagerData& in, SECompartmentManager& out, SECircuitManager* circuits = nullptr);
    static void UnMarshall(const SECompartmentManager& in, CDM::CompartmentManagerData& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Compartment::Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Invalidate();
    } else {
      Marshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Compartment::UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    UnMarshall(in, *item);
    option_out.set(*item);
  }

} // Namespace IO
} //Namespace Biogears
