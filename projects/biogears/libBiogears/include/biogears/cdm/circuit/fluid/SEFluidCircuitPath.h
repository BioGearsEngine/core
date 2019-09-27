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
#include <biogears/cdm/circuit/SECircuitPath.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuitNode.h>
#include <biogears/cdm/properties/SEScalarFlowCompliance.h>
#include <biogears/cdm/properties/SEScalarFlowInertance.h>
#include <biogears/cdm/properties/SEScalarFlowResistance.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>

namespace biogears {
class SECircuitManager;
class BIOGEARS_API SEFluidCircuitPath : public SECircuitPath<FLUID_CIRCUIT_PATH> {
  friend class SECircuitManager;

protected:
  SEFluidCircuitPath(SEFluidCircuitNode& src, SEFluidCircuitNode& tgt, const char* name);
  SEFluidCircuitPath(SEFluidCircuitNode& src, SEFluidCircuitNode& tgt, const std::string& name);

public:
  virtual ~SEFluidCircuitPath();

  virtual void Clear(); //clear memory

  bool Load(const CDM::FluidCircuitPathData& in);
  CDM::FluidCircuitPathData* Unload() const;

protected:
  void Unload(CDM::FluidCircuitPathData& data) const;

public:
  virtual SEFluidCircuitNode& GetSourceNode() const { return m_FluidSourceNode; }
  virtual SEFluidCircuitNode& GetTargetNode() const { return m_FluidTargetNode; }

  virtual bool HasResistance() const;
  virtual SEScalarFlowResistance& GetResistance();
  virtual double GetResistance(const FlowResistanceUnit& unit) const;
  virtual bool HasNextResistance() const;
  virtual SEScalarFlowResistance& GetNextResistance();
  virtual double GetNextResistance(const FlowResistanceUnit& unit) const;
  virtual bool HasResistanceBaseline() const;
  virtual SEScalarFlowResistance& GetResistanceBaseline();
  virtual double GetResistanceBaseline(const FlowResistanceUnit& unit) const;

  virtual bool HasCompliance() const;
  virtual SEScalarFlowCompliance& GetCompliance();
  virtual double GetCompliance(const FlowComplianceUnit& unit) const;
  virtual bool HasNextCompliance() const;
  virtual SEScalarFlowCompliance& GetNextCompliance();
  virtual double GetNextCompliance(const FlowComplianceUnit& unit) const;
  virtual bool HasComplianceBaseline() const;
  virtual SEScalarFlowCompliance& GetComplianceBaseline();
  virtual double GetComplianceBaseline(const FlowComplianceUnit& unit) const;

  virtual bool HasInertance() const;
  virtual SEScalarFlowInertance& GetInertance();
  virtual double GetInertance(const FlowInertanceUnit& unit) const;
  virtual bool HasNextInertance() const;
  virtual SEScalarFlowInertance& GetNextInertance();
  virtual double GetNextInertance(const FlowInertanceUnit& unit) const;
  virtual bool HasInertanceBaseline() const;
  virtual SEScalarFlowInertance& GetInertanceBaseline();
  virtual double GetInertanceBaseline(const FlowInertanceUnit& unit) const;

  virtual bool HasFlow() const;
  virtual SEScalarVolumePerTime& GetFlow();
  virtual double GetFlow(const VolumePerTimeUnit& unit) const;
  virtual bool HasNextFlow() const;
  virtual SEScalarVolumePerTime& GetNextFlow();
  virtual double GetNextFlow(const VolumePerTimeUnit& unit) const;
  virtual bool HasFlowSource() const;
  virtual SEScalarVolumePerTime& GetFlowSource();
  virtual double GetFlowSource(const VolumePerTimeUnit& unit) const;
  virtual bool HasNextFlowSource() const;
  virtual SEScalarVolumePerTime& GetNextFlowSource();
  virtual double GetNextFlowSource(const VolumePerTimeUnit& unit) const;
  virtual bool HasFlowSourceBaseline() const;
  virtual SEScalarVolumePerTime& GetFlowSourceBaseline();
  virtual double GetFlowSourceBaseline(const VolumePerTimeUnit& unit) const;

  virtual bool HasPressureSource() const;
  virtual SEScalarPressure& GetPressureSource();
  virtual double GetPressureSource(const PressureUnit& unit) const;
  virtual bool HasNextPressureSource() const;
  virtual SEScalarPressure& GetNextPressureSource();
  virtual double GetNextPressureSource(const PressureUnit& unit) const;
  virtual bool HasPressureSourceBaseline() const;
  virtual SEScalarPressure& GetPressureSourceBaseline();
  virtual double GetPressureSourceBaseline(const PressureUnit& unit) const;
  virtual bool HasValveBreakdownPressure() const;
  virtual SEScalarPressure& GetValveBreakdownPressure();
  virtual double GetValveBreakdownPressure(const PressureUnit& unit) const;

  virtual bool HasCardiovascularRegion() const;
  CDM::enumResistancePathType::value GetCardiovascularRegion() const;
  void SetCardiovascularRegion(CDM::enumResistancePathType::value pType);
  void InvalidateCardiovascularRegion();

protected:
  SEFluidCircuitNode& m_FluidSourceNode;
  SEFluidCircuitNode& m_FluidTargetNode;
  CDM::enumResistancePathType::value m_CardiovascularRegion;
};
}