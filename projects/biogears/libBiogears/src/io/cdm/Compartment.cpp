#include "Compartment.h"

#include "Property.h"
#include "Substance.h"
#include "SubstanceQuantity.h"

#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarMassPerMass.h>
#include <biogears/cdm/properties/SEScalarPower.h>

#include <biogears/cdm/circuit/fluid/SEFluidCircuitNode.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceManager.h>

#include <biogears/cdm/compartment/SECompartment.h>
#include <biogears/cdm/compartment/SECompartmentLink.h>
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/compartment/fluid/SEFluidCompartment.h>
#include <biogears/cdm/compartment/fluid/SEFluidCompartmentLink.h>
#include <biogears/cdm/compartment/fluid/SEGasCompartment.h>
#include <biogears/cdm/compartment/fluid/SEGasCompartmentGraph.h>
#include <biogears/cdm/compartment/fluid/SEGasCompartmentLink.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartmentGraph.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartmentLink.h>
#include <biogears/cdm/compartment/thermal/SEThermalCompartment.h>
#include <biogears/cdm/compartment/thermal/SEThermalCompartmentLink.h>
#include <biogears/cdm/compartment/tissue/SETissueCompartment.h>

namespace biogears {
namespace io {
  //class SECompartment
  void Compartment::Marshall(const CDM::CompartmentData& in, SECompartment& out)
  {
    out.Clear();
  }
  //----------------------------------------------------------------------------------
  void Compartment::UnMarshall(const SECompartment& in, CDM::CompartmentData& out)
  {
    out.Name(in.m_Name);
  }
  //----------------------------------------------------------------------------------
  //class SECompartmentLink
  void Compartment::Marshall(const CDM::CompartmentLinkData& in, SECompartmentLink& out)
  {
    out.Clear();
  }
  //----------------------------------------------------------------------------------
  void Compartment::UnMarshall(const SECompartmentLink& in, CDM::CompartmentLinkData& out)
  {
    out.Name(in.m_Name);
  }
  //----------------------------------------------------------------------------------
  //class SEFluidCompartment
  template <FLUID_COMPARTMENT_TEMPLATE>
  void Compartment::Marshall(const CDM::FluidCompartmentData& in, SEFluidCompartment<FLUID_COMPARTMENT_TYPES>& out, SECircuitManager* circuits)
  {
    Marshall(static_cast<const CDM::CompartmentData&>(in), static_cast<SECompartment&>(out));
    if (!in.Child().empty())
      return;
    else if (!in.Node().empty()) {
      if (circuits == nullptr) {
        throw CommonDataModelException("Compartment is mapped to circuit nodes, but no circuit manager was provided, cannot load");
      }
      for (const auto name : in.Node()) {
        SEFluidCircuitNode* node = circuits->GetFluidNode(name);
        if (node == nullptr) {
          throw CommonDataModelException("Compartment is mapped to circuit node, " + std::string { name } + ", but provided circuit manager did not have that node");
        }
        out.MapNode(*node);
      }
    } else { // Only load these if you don't have children or nodes
      if (in.Pressure().present()) {
        io::Property::Marshall(in.Pressure(), out.GetPressure());
      }
      if (in.Volume().present()) {
        io::Property::Marshall(in.Volume(), out.GetVolume());
      }
    }
  }
  //----------------------------------------------------------------------------------
  template <FLUID_COMPARTMENT_TEMPLATE>
  void Compartment::UnMarshall(const SEFluidCompartment<FLUID_COMPARTMENT_TYPES>& in, CDM::FluidCompartmentData& out)
  {

   UnMarshall(static_cast<const SECompartment&>(in), static_cast<CDM::CompartmentData&>(out));
    for (auto child : in.m_FluidChildren) {
      out.Child().push_back(child->GetName());
    }
    for (auto nodes : in.m_Nodes.m_AllNodes) {
      out.Node().push_back(nodes->GetName());
    }
    // Even if you have children or nodes, I am unloading everything, this makes the xml actually usefull...
    if (in.HasInFlow()) {
      io::Property::UnMarshall(const_cast<SEFluidCompartment<FLUID_COMPARTMENT_TYPES>&>(in).GetInFlow(), out.InFlow());
    }
    if (in.HasOutFlow()) {
      io::Property::UnMarshall(const_cast<SEFluidCompartment<FLUID_COMPARTMENT_TYPES>&>(in).GetOutFlow(), out.OutFlow());
    }
    if (in.HasPressure()) {
      io::Property::UnMarshall(const_cast<SEFluidCompartment<FLUID_COMPARTMENT_TYPES>&>(in).GetPressure(), out.Pressure());
    }
    if (in.HasVolume()) {
      io::Property::UnMarshall(const_cast<SEFluidCompartment<FLUID_COMPARTMENT_TYPES>&>(in).GetVolume(), out.Volume());
    }
  }
  //----------------------------------------------------------------------------------
  //class SEFluidCompartmentLink

  template <FLUID_COMPARTMENT_LINK_TEMPLATE>
  void Compartment::Marshall(const CDM::FluidCompartmentLinkData& in, SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>& out, SECircuitManager* circuits)
  {
    Marshall(static_cast<const CDM::CompartmentLinkData&>(in), static_cast<SECompartmentLink&>(out));
    if (in.Path().present()) {
      if (circuits == nullptr) {
        throw CommonDataModelException("Link is mapped to circuit path, " + std::string { in.Path().get() } + ", but no circuit manager was provided, cannot load");
      }
      SEFluidCircuitPath* path = circuits->GetFluidPath(in.Path().get());
      if (path == nullptr) {
        throw CommonDataModelException("Link is mapped to circuit path, " + std::string { in.Path().get() } + ", but provided circuit manager did not have that path");
      }
      out.MapPath(*path);
    } else {
      if (in.Flow().present())
        io::Property::Marshall(in.Flow(), out.GetFlow());
    }
  }
  //----------------------------------------------------------------------------------
  template <FLUID_COMPARTMENT_LINK_TEMPLATE>
  void Compartment::UnMarshall(const SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>& in, CDM::FluidCompartmentLinkData& out)
  {
    UnMarshall(static_cast<const SECompartmentLink&>(in), static_cast<CDM::CompartmentLinkData&>(out));
    out.SourceCompartment(in.m_SourceCmpt.GetName());
    out.TargetCompartment(in.m_TargetCmpt.GetName());
    if (in.m_Path != nullptr)
      out.Path(in.m_Path->GetName());
    // Even if you have a path, I am unloading everything, this makes the xml actually usefull...
    if (in.HasFlow())
      io::Property::UnMarshall(const_cast<SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>&>(in).GetFlow(), out.Flow());
  }
  //----------------------------------------------------------------------------------
  //class SEGasCompartment
  void Compartment::Marshall(const CDM::GasCompartmentData& in, SEGasCompartment& out, SESubstanceManager& subMgr, SECircuitManager* circuits)
  {
    Marshall(static_cast<const CDM::FluidCompartmentData>(in), static_cast<SEFluidCompartment<SEGasCompartmentLink, SEGasTransportVertex, SEGasTransportSubstance, SEGasSubstanceQuantity>&>(out), circuits);
    if (in.Child().empty()) {
      for (const CDM::GasSubstanceQuantityData& d : in.SubstanceQuantity()) {
        SESubstance* sub = subMgr.GetSubstance(d.Substance());
        if (sub == nullptr) {
          throw CommonDataModelException("Could not find a substance for " + std::string { d.Substance() });
        }
        io::SubstanceQuantity::Marshall(d, out.CreateSubstanceQuantity(*sub));
        ;
      }
    }
  }
  //----------------------------------------------------------------------------------
  void Compartment::UnMarshall(const SEGasCompartment& in, CDM::GasCompartmentData& out)
  {
    UnMarshall(static_cast<const SEFluidCompartment<SEGasCompartmentLink, SEGasTransportVertex, SEGasTransportSubstance, SEGasSubstanceQuantity>&>(in), static_cast<CDM::FluidCompartmentData&>(out));
    for (SEGasSubstanceQuantity* subQ : in.m_SubstanceQuantities) {
      auto gsqData = CDM::GasSubstanceQuantityData();
      io::SubstanceQuantity::UnMarshall(*subQ, gsqData);
      out.SubstanceQuantity().push_back(gsqData);
    }
  }
  //----------------------------------------------------------------------------------
  //class SEGasCompartmentLink
  void Compartment::Marshall(const CDM::GasCompartmentLinkData& in, SEGasCompartmentLink& out)
  {
    Marshall(static_cast<const CDM::CompartmentLinkData&>(in), static_cast<SECompartmentLink&>(out));
  }
  //----------------------------------------------------------------------------------
  void Compartment::UnMarshall(const SEGasCompartmentLink& in, CDM::GasCompartmentLinkData& out)
  {
    UnMarshall(static_cast<const SECompartmentLink&>(in), static_cast<CDM::GasCompartmentLinkData&>(out));
  }
  //----------------------------------------------------------------------------------
  //class SEGasCompartmentGraph
  void Compartment::Marshall(const CDM::GasCompartmentGraphData& in, SEGasCompartmentGraph& out, SECompartmentManager& cmptMgr)
  {
    out.m_Name = in.Name();
    for (auto name : in.Compartment()) {
      SEGasCompartment* cmpt = cmptMgr.GetGasCompartment(name);
      if (cmpt == nullptr) {
        throw CommonDataModelException("Could not find compartment " + std::string { name } + " for graph " + out.m_Name);
      }
      out.AddCompartment(*cmpt);
    }
    for (auto name : in.Link()) {
      SEGasCompartmentLink* link = cmptMgr.GetGasLink(name);
      if (link == nullptr) {
        throw CommonDataModelException("Could not find link " + std::string { name } + " for graph " + out.m_Name);
      }
      out.AddLink(*link);
    }
  }
  //----------------------------------------------------------------------------------
  void Compartment::UnMarshall(const SEGasCompartmentGraph& in, CDM::GasCompartmentGraphData& out)
  {
    out.Name(in.m_Name);
    for (SEGasCompartment* cmpt : in.m_Compartments)
      out.Compartment().push_back(cmpt->GetName());
    for (SEGasCompartmentLink* link : in.m_CompartmentLinks)
      out.Link().push_back(link->GetName());
  }
  //----------------------------------------------------------------------------------
  //class SELiquidCompartment
  void Compartment::Marshall(const CDM::LiquidCompartmentData& in, SELiquidCompartment& out, SESubstanceManager& subMgr, SECircuitManager* circuits)
  {

    Marshall(static_cast<const CDM::FluidCompartmentData&>(in), static_cast<SEFluidCompartment<SELiquidCompartmentLink, SELiquidTransportVertex, SELiquidTransportSubstance, SELiquidSubstanceQuantity>&>(out), circuits);
    if (in.Child().empty()) {
      for (const CDM::LiquidSubstanceQuantityData& d : in.SubstanceQuantity()) {
        SESubstance* sub = subMgr.GetSubstance(d.Substance());
        if (sub == nullptr) {
          throw CommonDataModelException("Could not find a substance for " + std::string { d.Substance() });
        }
        io::SubstanceQuantity::Marshall(d, out.CreateSubstanceQuantity(*sub));
      }
      if (in.pH().present()) {
        io::Property::Marshall(in.pH(), out.GetPH());
      }
      if (in.WaterVolumeFraction().present()) {
        io::Property::Marshall(in.WaterVolumeFraction(), out.GetWaterVolumeFraction());
      }
    }
  }
  //----------------------------------------------------------------------------------
  void Compartment::UnMarshall(const SELiquidCompartment& in, CDM::LiquidCompartmentData& out)
  {
    UnMarshall(static_cast<const SEFluidCompartment<SELiquidCompartmentLink, SELiquidTransportVertex, SELiquidTransportSubstance, SELiquidSubstanceQuantity>&>(in), static_cast<CDM::FluidCompartmentData&>(out));
    for (SELiquidSubstanceQuantity* subQ : in.m_SubstanceQuantities) {
      auto lqsData = CDM::LiquidSubstanceQuantityData();
      io::SubstanceQuantity::UnMarshall(*subQ, lqsData);
      out.SubstanceQuantity().push_back(lqsData);
    }
    if (in.HasPH()) {
      io::Property::UnMarshall(in.GetPH(), out.pH());
    }
    if (in.HasWaterVolumeFraction()) {
      io::Property::UnMarshall(in.GetWaterVolumeFraction(), out.WaterVolumeFraction());
    }
  }
  //----------------------------------------------------------------------------------
  //class SELiquidCompartmentLink
  void Compartment::Marshall(const CDM::LiquidCompartmentLinkData& in, SELiquidCompartmentLink& out)
  {
    Marshall(static_cast<const CDM::FluidCompartmentLinkData&>(in), static_cast<SEFluidCompartmentLink<SELiquidTransportEdge, SELiquidTransportVertex, SELiquidCompartment>&>(out));
  }
  //----------------------------------------------------------------------------------
  void Compartment::UnMarshall(const SELiquidCompartmentLink& in, CDM::LiquidCompartmentLinkData& out)
  {
    UnMarshall(static_cast<const SEFluidCompartmentLink<SELiquidTransportEdge, SELiquidTransportVertex, SELiquidCompartment>&>(in), static_cast<CDM::FluidCompartmentLinkData&>(out));
  }
  //----------------------------------------------------------------------------------
  //class SELiquidCompartmentGraph
  void Compartment::Marshall(const CDM::LiquidCompartmentGraphData& in, SELiquidCompartmentGraph& out, SECompartmentManager& cmptMgr)
  {
    out.m_Name = in.Name();
    for (auto name : in.Compartment()) {
      SELiquidCompartment* cmpt = cmptMgr.GetLiquidCompartment(name);
      if (cmpt == nullptr) {
        throw CommonDataModelException("Could not find compartment " + std::string { name } + " for graph " + out.m_Name);
      }
      out.AddCompartment(*cmpt);
    }
    for (auto name : in.Link()) {
      SELiquidCompartmentLink* link = cmptMgr.GetLiquidLink(name);
      if (link == nullptr) {
        throw CommonDataModelException("Could not find link " + std::string { name } + " for graph " + out.m_Name);
      }
      out.AddLink(*link);
    }
  }
  //----------------------------------------------------------------------------------
  void Compartment::UnMarshall(const SELiquidCompartmentGraph& in, CDM::LiquidCompartmentGraphData& out)
  {
    out.Name(in.m_Name);
    for (SELiquidCompartment* cmpt : in.m_Compartments)
      out.Compartment().push_back(cmpt->GetName());
    for (SELiquidCompartmentLink* link : in.m_CompartmentLinks)
      out.Link().push_back(link->GetName());
  }
  //----------------------------------------------------------------------------------
  //class SETissueCompartment
  void Compartment::Marshall(const CDM::TissueCompartmentData& in, SETissueCompartment& out, SESubstanceManager& subMgr, SECircuitManager* circuits)
  {
    Marshall(static_cast<const CDM::CompartmentData&>(in), static_cast<SECompartment&>(out));

    io::Property::Marshall(in.AcidicPhospohlipidConcentration(), out.GetAcidicPhospohlipidConcentration());
    io::Property::Marshall(in.MatrixVolume(), out.GetMatrixVolume());
    io::Property::Marshall(in.MembranePotential(), out.GetMembranePotential());
    io::Property::Marshall(in.NeutralLipidsVolumeFraction(), out.GetNeutralLipidsVolumeFraction());
    io::Property::Marshall(in.NeutralPhospholipidsVolumeFraction(), out.GetNeutralPhospholipidsVolumeFraction());
    io::Property::Marshall(in.ReflectionCoefficient(), out.GetReflectionCoefficient());
    io::Property::Marshall(in.TissueToPlasmaAlbuminRatio(), out.GetTissueToPlasmaAlbuminRatio());
    io::Property::Marshall(in.TissueToPlasmaAlphaAcidGlycoproteinRatio(), out.GetTissueToPlasmaAlphaAcidGlycoproteinRatio());
    io::Property::Marshall(in.TissueToPlasmaLipoproteinRatio(), out.GetTissueToPlasmaLipoproteinRatio());
    io::Property::Marshall(in.TotalMass(), out.GetTotalMass());
  }
  //----------------------------------------------------------------------------------
  void Compartment::UnMarshall(const SETissueCompartment& in, CDM::TissueCompartmentData& out)
  {

    UnMarshall(static_cast<const SECompartment&>(in), static_cast<CDM::CompartmentData&>(out));
    if (in.HasAcidicPhospohlipidConcentration())
      io::Property::UnMarshall(*in.m_AcidicPhospohlipidConcentration, out.AcidicPhospohlipidConcentration());
    if (in.HasMatrixVolume())
      io::Property::UnMarshall(*in.m_MatrixVolume, out.MatrixVolume());
    if (in.HasMembranePotential())
      io::Property::UnMarshall(*in.m_MembranePotential, out.MembranePotential());
    if (in.HasNeutralLipidsVolumeFraction())
      io::Property::UnMarshall(*in.m_NeutralLipidsVolumeFraction, out.NeutralLipidsVolumeFraction());
    if (in.HasNeutralPhospholipidsVolumeFraction())
      io::Property::UnMarshall(*in.m_NeutralPhospholipidsVolumeFraction, out.NeutralPhospholipidsVolumeFraction());
    if (in.HasReflectionCoefficient())
      io::Property::UnMarshall(*in.m_ReflectionCoefficient, out.ReflectionCoefficient());
    if (in.HasTissueToPlasmaAlbuminRatio())
      io::Property::UnMarshall(*in.m_TissueToPlasmaAlbuminRatio, out.TissueToPlasmaAlbuminRatio());
    if (in.HasTissueToPlasmaAlbuminRatio())
      io::Property::UnMarshall(*in.m_TissueToPlasmaAlbuminRatio, out.TissueToPlasmaAlbuminRatio());
    if (in.HasTissueToPlasmaAlphaAcidGlycoproteinRatio())
      io::Property::UnMarshall(*in.m_TissueToPlasmaAlphaAcidGlycoproteinRatio, out.TissueToPlasmaAlphaAcidGlycoproteinRatio());
    if (in.HasTissueToPlasmaLipoproteinRatio())
      io::Property::UnMarshall(*in.m_TissueToPlasmaLipoproteinRatio, out.TissueToPlasmaLipoproteinRatio());
    if (in.HasTotalMass())
      io::Property::UnMarshall(*in.m_TotalMass, out.TotalMass());
  }
  //----------------------------------------------------------------------------------
  //class SEThermalCompartment
  void Compartment::Marshall(const CDM::ThermalCompartmentData& in, SEThermalCompartment& out, SECircuitManager* circuits)
  {
    Marshall(static_cast<const CDM::CompartmentData&>(in), static_cast<SECompartment&>(out));
    // Not Loading In/Out HeatTransferRate, those are calculated on demand
    if (!in.Child().empty())
      return;
    else if (!in.Node().empty()) {
      if (circuits == nullptr) {
        throw CommonDataModelException("Compartment is mapped to circuit nodes, but no circuit manager was provided, cannot load");
      }
      for (auto name : in.Node()) {
        SEThermalCircuitNode* node = circuits->GetThermalNode(name);
        if (node == nullptr) {
          throw CommonDataModelException("Compartment is mapped to circuit node, " + std::string { name } + ", but provided circuit manager did not have that node");
        }
        out.MapNode(*node);
      }
    } else { // Only load these if you don't have children or nodes
      io::Property::Marshall(in.Heat(), out.GetHeat());
      io::Property::Marshall(in.Temperature(), out.GetTemperature());
    }
  }
  //----------------------------------------------------------------------------------
  void Compartment::UnMarshall(SEThermalCompartment& in, CDM::ThermalCompartmentData& out)
  {
    UnMarshall(static_cast<const SECompartment&>(in), static_cast<CDM::CompartmentData&>(out));
    for (SEThermalCompartment* child : in.m_Children)
      out.Child().push_back(child->GetName());
    for (SEThermalCircuitNode* nodes : in.m_Nodes.GetNodes())
      out.Node().push_back(nodes->GetName());
    // Even if you have children or nodes, I am unloading everything, this makes the xml actually usefull...
    if (in.HasHeatTransferRateIn())
      io::Property::UnMarshall(in.GetHeatTransferRateIn(), out.HeatTransferRateIn());
    if (in.HasHeatTransferRateOut())
      io::Property::UnMarshall(in.GetHeatTransferRateOut(), out.HeatTransferRateOut());
    if (in.HasHeat())
      io::Property::UnMarshall(in.GetHeat(), out.Heat());
    if (in.HasTemperature())
      io::Property::UnMarshall(in.GetTemperature(), out.Temperature());
  }
  //----------------------------------------------------------------------------------
  //class SEThermalCompartmentLink
  void Compartment::Marshall(const CDM::ThermalCompartmentLinkData& in, SEThermalCompartmentLink& out, SECircuitManager* circuits)
  {

    Marshall(static_cast<const CDM::CompartmentLinkData&>(in), static_cast<SECompartmentLink&>(out));
    if (in.Path().present()) {
      if (circuits == nullptr) {
        throw CommonDataModelException("Link is mapped to circuit path, " + std::string { in.Path().get() } + ", but no circuit manager was provided, cannot load");
      }
      SEThermalCircuitPath* path = circuits->GetThermalPath(in.Path().get());
      if (path == nullptr) {
        throw CommonDataModelException("Link is mapped to circuit path, " + std::string { in.Path().get() } + ", but provided circuit manager did not have that path");
      }
      out.MapPath(*path);
    } else {
      if (in.HeatTransferRate().present()) {
        io::Property::Marshall(in.HeatTransferRate(), out.GetHeatTransferRate());
      }
    }
  }
  //----------------------------------------------------------------------------------
  void Compartment::UnMarshall(const SEThermalCompartmentLink& in, CDM::ThermalCompartmentLinkData& out)
  {
    UnMarshall(static_cast<const SECompartmentLink&>(in), static_cast<CDM::CompartmentLinkData&>(out));
    out.SourceCompartment(in.m_SourceCmpt.GetName());
    out.TargetCompartment(in.m_TargetCmpt.GetName());
    if (in.m_Path != nullptr)
      out.Path(in.m_Path->GetName());
    // Even if you have a path, I am unloading everything, this makes the xml actually usefull...
    if (in.HasHeatTransferRate()) {
      io::Property::UnMarshall(*in.m_HeatTransferRate, out.HeatTransferRate());
    }
  }

#define LOAD_COMPARTMENT(type)                                                            \
  for (auto& cData : in.type##Compartment()) {                                            \
    Marshall(cData, out.Create##type##Compartment(cData.Name()), out.m_subMgr, circuits); \
  }

#define LOAD_THERMAL_COMPARTMENT(type)                                      \
  for (auto& cData : in.type##Compartment()) {                              \
    Marshall(cData, out.Create##type##Compartment(cData.Name()), circuits); \
  }

#define LOAD_LINK(type)                                                                                                                                       \
  for (auto& cData : in.type##Link()) {                                                                                                                       \
    auto* src = out.Get##type##Compartment(cData.SourceCompartment());                                                                                        \
    if (src == nullptr) {                                                                                                                                     \
      throw CommonDataModelException("Unable to find source compartment " + std::string { cData.SourceCompartment() } + " for link " + cData.Name().c_str()); \
    }                                                                                                                                                         \
    auto* tgt = out.Get##type##Compartment(cData.TargetCompartment());                                                                                        \
    if (src == nullptr) {                                                                                                                                     \
      throw CommonDataModelException("Unable to find target compartment " + std::string { cData.TargetCompartment() } + " for link " + cData.Name().c_str()); \
    }                                                                                                                                                         \
    Marshall(cData, out.Create##type##Link(*src, *tgt, cData.Name()), circuits);                                                                              \
  }

#define LOAD_HIERARCHY(type)                                                                                             \
  for (auto& cData : in.type##Compartment()) {                                                                           \
    auto* cmpt = out.Get##type##Compartment(cData.Name());                                                               \
    for (auto name : cData.Child()) {                                                                                    \
      auto* child = out.Get##type##Compartment(name);                                                                    \
      if (child == nullptr) {                                                                                            \
        throw CommonDataModelException("Could not find child " + std::string { name } + " for node " + cmpt->GetName()); \
      }                                                                                                                  \
      cmpt->AddChild(*child);                                                                                            \
    }                                                                                                                    \
  }

#define LOAD_GRAPH(type)                                         \
  for (auto& cData : in.type##Graph()) {                         \
    Marshall(cData, out.Create##type##Graph(cData.Name()), out); \
  }

#define LOAD_SUBSTANCE(type)                                                                 \
  for (auto subName : in.type##Substance()) {                                                \
    SESubstance* sub = out.m_subMgr.GetSubstance(subName);                                   \
    if (sub == nullptr) {                                                                    \
      throw CommonDataModelException("Could not find substance " + std::string { subName }); \
    }                                                                                        \
    out.Add##type##CompartmentSubstance(*sub);                                               \
  }

  //----------------------------------------------------------------------------------
  //class SECompartmentManager
  void Compartment::Marshall(CDM::CompartmentManagerData& in, SECompartmentManager& out, SECircuitManager* circuits)
  {
    out.Clear();

    LOAD_COMPARTMENT(Gas);
    LOAD_LINK(Gas);
    LOAD_HIERARCHY(Gas);
    LOAD_SUBSTANCE(Gas);
    LOAD_GRAPH(Gas);

    LOAD_COMPARTMENT(Liquid);
    LOAD_LINK(Liquid);
    LOAD_HIERARCHY(Liquid);
    LOAD_SUBSTANCE(Liquid);
    LOAD_GRAPH(Liquid);

    LOAD_THERMAL_COMPARTMENT(Thermal);
    LOAD_LINK(Thermal);
    LOAD_HIERARCHY(Thermal);

    LOAD_COMPARTMENT(Tissue);

    out.StateChange();
  }
  //----------------------------------------------------------------------------------
  void Compartment::UnMarshall(const SECompartmentManager& in, CDM::CompartmentManagerData& out)
  {
    for (SELiquidCompartment* cmpt : in.m_LiquidCompartments) {
      auto lcData = CDM::LiquidCompartmentData();
      UnMarshall(*cmpt, lcData);
      out.LiquidCompartment().push_back(lcData);
    }
    for (SELiquidCompartmentLink* link : in.m_LiquidLinks) {
      auto lclData = CDM::LiquidCompartmentLinkData();
      UnMarshall(*link, lclData);
      out.LiquidLink().push_back(lclData);
    }
    for (SELiquidCompartmentGraph* graph : in.m_LiquidGraphs) {
      auto lcGData = CDM::LiquidCompartmentGraphData();
      UnMarshall(*graph, lcGData);
      out.LiquidGraph().push_back(lcGData);
    }
    for (SESubstance* sub : in.m_LiquidSubstances) {
      out.LiquidSubstance().push_back(sub->GetName());
    }

    for (SEGasCompartment* cmpt : in.m_GasCompartments) {
      auto gcData = CDM::GasCompartmentData();
      UnMarshall(*cmpt, gcData);
      out.GasCompartment().push_back(gcData);
    }
    for (SEGasCompartmentLink* link : in.m_GasLinks) {
      auto gclData = CDM::GasCompartmentLinkData();
      UnMarshall(*link, gclData);
      out.GasLink().push_back(gclData);
    }
    for (SEGasCompartmentGraph* graph : in.m_GasGraphs) {
      auto gcgData = CDM::GasCompartmentGraphData();
      UnMarshall(*graph, gcgData);
      out.GasGraph().push_back(gcgData);
    }
    for (SESubstance* sub : in.m_GasSubstances)
      out.GasSubstance().push_back(sub->GetName());

    for (SEThermalCompartment* cmpt : in.m_ThermalCompartments) {
      auto tcData = CDM::ThermalCompartmentData();
      UnMarshall(*cmpt, tcData);
      out.ThermalCompartment().push_back(tcData);
    }
    for (SEThermalCompartmentLink* link : in.m_ThermalLinks) {
      auto tclData = CDM::ThermalCompartmentLinkData();
      UnMarshall(*link, tclData);
      out.ThermalLink().push_back(tclData);
    }

    for (SETissueCompartment* cmpt : in.m_TissueCompartments) {
      auto tcData = CDM::TissueCompartmentData();
      UnMarshall(*cmpt, tcData);
      out.TissueCompartment().push_back(tcData);
    }
  }
  //----------------------------------------------------------------------------------
}
}