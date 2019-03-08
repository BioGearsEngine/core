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
#include <biogears/cdm/circuit/fluid/SEFluidCircuitPath.h>

#include <biogears/cdm/compartment/SECompartmentLink.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/schema/cdm/Compartment.hxx>

#define FLUID_COMPARTMENT_LINK_TEMPLATE typename EdgeType, typename VertexType, typename CompartmentType
#define FLUID_COMPARTMENT_LINK_TYPES EdgeType, VertexType, CompartmentType

namespace biogears {
template <FLUID_COMPARTMENT_LINK_TEMPLATE>
class SEFluidCompartmentLink : public SECompartmentLink, public EdgeType {
protected:
  SEFluidCompartmentLink(CompartmentType& src, CompartmentType& tgt, const char* name);
  SEFluidCompartmentLink(CompartmentType& src, CompartmentType& tgt, const std::string& name);

  public:
    virtual ~SEFluidCompartmentLink();
  
    virtual void Clear() override;
  
    virtual bool Load(const CDM::FluidCompartmentLinkData& in, SECircuitManager* circuits = nullptr);
    virtual CDM::FluidCompartmentLinkData* Unload() override = 0;
  
protected:
  virtual void Unload(CDM::FluidCompartmentLinkData& data);

public:
  virtual const SEScalar* GetScalar(const char* name) override;
  virtual const SEScalar* GetScalar(const std::string& name) override;

  virtual std::string GetName() const override;
  virtual const char* GetName_cStr() const override;

  virtual bool HasFlow() const;
  virtual SEScalarVolumePerTime& GetFlow();
  virtual double GetFlow(const VolumePerTimeUnit& unit) const;

  virtual CompartmentType& GetSourceCompartment() { return m_SourceCmpt; }
  virtual CompartmentType& GetTargetCompartment() { return m_TargetCmpt; }

  virtual bool HasPath() { return m_Path != nullptr; }
  virtual SEFluidCircuitPath* GetPath() { return m_Path; }
  virtual void RemovePath() { m_Path = nullptr; }
  virtual void MapPath(SEFluidCircuitPath& path)
  {
    Clear();
    m_Path = &path;
  }

protected:
  // For Transport
  virtual bool HasFlux() const  override{ return HasFlow(); }
  virtual SEScalarVolumePerTime& GetFlux()  override{ return GetFlow(); }
  virtual double GetFlux(const VolumePerTimeUnit& unit) const { return GetFlow(unit); }

  virtual VertexType& GetSourceVertex()  override{ return m_SourceVertex; }
  virtual VertexType& GetTargetVertex()  override{ return m_TargetVertex; }

  SEScalarVolumePerTime* m_Flow;
  CompartmentType& m_SourceCmpt;
  CompartmentType& m_TargetCmpt;
  VertexType& m_SourceVertex;
  VertexType& m_TargetVertex;
  SEFluidCircuitPath* m_Path;
};
}