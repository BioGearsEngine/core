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

#include <biogears/cdm/circuit/fluid/SEFluidCircuitPath.h>

namespace biogears {
	SEFluidCircuitPath::SEFluidCircuitPath(SEFluidCircuitNode& src, SEFluidCircuitNode& tgt, const char* name)
	  : SECircuitPath<SEScalarVolumePerTime, SEScalarFlowResistance, SEScalarFlowCompliance, SEScalarFlowInertance, SEScalarPressure, SEScalarVolume>(src, tgt, std::string{ name })
	  , m_FluidSourceNode(src)
	  , m_FluidTargetNode(tgt)
	{
	  m_CardiovascularRegion = (CDM::enumResistancePathType::value)-1;
	}
	SEFluidCircuitPath::SEFluidCircuitPath(SEFluidCircuitNode& src, SEFluidCircuitNode& tgt, const std::string& name)
	  : SECircuitPath<SEScalarVolumePerTime, SEScalarFlowResistance, SEScalarFlowCompliance, SEScalarFlowInertance, SEScalarPressure, SEScalarVolume>(src, tgt, name)
	  , m_FluidSourceNode(src)
	  , m_FluidTargetNode(tgt)
	{
	  m_CardiovascularRegion = (CDM::enumResistancePathType::value)-1;
	}

	SEFluidCircuitPath::~SEFluidCircuitPath()
	{
	  Clear();
	}

	void SEFluidCircuitPath::Clear()
	{
	  SECircuitPath::Clear();
	  m_CardiovascularRegion = (CDM::enumResistancePathType::value)-1;
	}

	bool SEFluidCircuitPath::Load(const CDM::FluidCircuitPathData& in)
	{
	  SECircuitPath::Load(in);
	  if (in.Resistance().present())
		GetResistance().Load(in.Resistance().get());
	  if (in.NextResistance().present())
		GetNextResistance().Load(in.NextResistance().get());
	  if (in.ResistanceBaseline().present())
		GetResistanceBaseline().Load(in.ResistanceBaseline().get());
	  if (in.Compliance().present())
		GetCompliance().Load(in.Compliance().get());
	  if (in.NextCompliance().present())
		GetNextCompliance().Load(in.NextCompliance().get());
	  if (in.ComplianceBaseline().present())
		GetComplianceBaseline().Load(in.ComplianceBaseline().get());
	  if (in.Inertance().present())
		GetInertance().Load(in.Inertance().get());
	  if (in.NextInertance().present())
		GetNextInertance().Load(in.NextInertance().get());
	  if (in.InertanceBaseline().present())
		GetInertanceBaseline().Load(in.InertanceBaseline().get());
	  if (in.Flow().present())
		GetFlow().Load(in.Flow().get());
	  if (in.NextFlow().present())
		GetNextFlow().Load(in.NextFlow().get());
	  if (in.FlowSource().present())
		GetFlowSource().Load(in.FlowSource().get());
	  if (in.NextFlowSource().present())
		GetNextFlowSource().Load(in.NextFlowSource().get());
	  if (in.FlowSourceBaseline().present())
		GetFlowSourceBaseline().Load(in.FlowSourceBaseline().get());
	  if (in.PressureSource().present())
		GetPressureSource().Load(in.PressureSource().get());
	  if (in.NextPressureSource().present())
		GetNextPressureSource().Load(in.NextPressureSource().get());
	  if (in.PressureSourceBaseline().present())
		GetPressureSourceBaseline().Load(in.PressureSourceBaseline().get());
	  if (in.ValveBreakdownPressure().present())
		GetValveBreakdownPressure().Load(in.ValveBreakdownPressure().get());
	  if (in.CardiovascularRegion().present())
		SetCardiovascularRegion(in.CardiovascularRegion().get());

	  return HasValidElements();
	}

	CDM::FluidCircuitPathData* SEFluidCircuitPath::Unload() const
	{
	  CDM::FluidCircuitPathData* data = new CDM::FluidCircuitPathData();
	  Unload(*data);
	  return data;
	}

	void SEFluidCircuitPath::Unload(CDM::FluidCircuitPathData& data) const
	{
	  SECircuitPath::Unload(data);
	  if (HasResistance())
		data.Resistance(std::unique_ptr<CDM::ScalarFlowResistanceData>(m_Resistance->Unload()));
	  if (HasNextResistance())
		data.NextResistance(std::unique_ptr<CDM::ScalarFlowResistanceData>(m_NextResistance->Unload()));
	  if (HasResistanceBaseline())
		data.ResistanceBaseline(std::unique_ptr<CDM::ScalarFlowResistanceData>(m_ResistanceBaseline->Unload()));
	  if (HasCompliance())
		data.Compliance(std::unique_ptr<CDM::ScalarFlowComplianceData>(m_Capacitance->Unload()));
	  if (HasNextCompliance())
		data.NextCompliance(std::unique_ptr<CDM::ScalarFlowComplianceData>(m_NextCapacitance->Unload()));
	  if (HasComplianceBaseline())
		data.ComplianceBaseline(std::unique_ptr<CDM::ScalarFlowComplianceData>(m_CapacitanceBaseline->Unload()));
	  if (HasInertance())
		data.Inertance(std::unique_ptr<CDM::ScalarFlowInertanceData>(m_Inductance->Unload()));
	  if (HasNextInertance())
		data.NextInertance(std::unique_ptr<CDM::ScalarFlowInertanceData>(m_NextInductance->Unload()));
	  if (HasInertanceBaseline())
		data.InertanceBaseline(std::unique_ptr<CDM::ScalarFlowInertanceData>(m_InductanceBaseline->Unload()));
	  if (HasFlow())
		data.Flow(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_Flux->Unload()));
	  if (HasNextFlow())
		data.NextFlow(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_NextFlux->Unload()));
	  if (HasFlowSource())
		data.FlowSource(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_FluxSource->Unload()));
	  if (HasNextFlowSource())
		data.NextFlowSource(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_NextFluxSource->Unload()));
	  if (HasFlowSourceBaseline())
		data.FlowSourceBaseline(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_FluxSourceBaseline->Unload()));
	  if (HasPressureSource())
		data.PressureSource(std::unique_ptr<CDM::ScalarPressureData>(m_PotentialSource->Unload()));
	  if (HasNextPressureSource())
		data.NextPressureSource(std::unique_ptr<CDM::ScalarPressureData>(m_NextPotentialSource->Unload()));
	  if (HasPressureSourceBaseline())
		data.PressureSourceBaseline(std::unique_ptr<CDM::ScalarPressureData>(m_PotentialSourceBaseline->Unload()));
	  if (HasValveBreakdownPressure())
		data.ValveBreakdownPressure(std::unique_ptr<CDM::ScalarPressureData>(m_ValveBreakdownPotential->Unload()));
	  if (HasCardiovascularRegion())
		data.CardiovascularRegion(m_CardiovascularRegion);
	}

	////////////////////////////////
	// Fluid Resistance Types//
	////////////////////////////////

	bool SEFluidCircuitPath::HasResistance() const
	{
	  return SECircuitPath::HasResistance();
	}
	SEScalarFlowResistance& SEFluidCircuitPath::GetResistance()
	{
	  return SECircuitPath::GetResistance();
	}
	double SEFluidCircuitPath::GetResistance(const FlowResistanceUnit& unit) const
	{
	  if (m_Resistance == nullptr)
		return SEScalar::dNaN();
	  return m_Resistance->GetValue(unit);
	}
	bool SEFluidCircuitPath::HasNextResistance() const
	{
	  return SECircuitPath::HasNextResistance();
	}
	SEScalarFlowResistance& SEFluidCircuitPath::GetNextResistance()
	{
	  return SECircuitPath::GetNextResistance();
	}
	double SEFluidCircuitPath::GetNextResistance(const FlowResistanceUnit& unit) const
	{
	  if (m_NextResistance == nullptr)
		return SEScalar::dNaN();
	  return m_NextResistance->GetValue(unit);
	}
	bool SEFluidCircuitPath::HasResistanceBaseline() const
	{
	  return SECircuitPath::HasResistanceBaseline();
	}
	SEScalarFlowResistance& SEFluidCircuitPath::GetResistanceBaseline()
	{
	  return SECircuitPath::GetResistanceBaseline();
	}
	double SEFluidCircuitPath::GetResistanceBaseline(const FlowResistanceUnit& unit) const
	{
	  if (m_ResistanceBaseline == nullptr)
		return SEScalar::dNaN();
	  return m_ResistanceBaseline->GetValue(unit);
	}

	//////////////////////////////////
	// Fluid Capacitance Types //
	//////////////////////////////////

	bool SEFluidCircuitPath::HasCompliance() const
	{
	  return HasCapacitance();
	}
	SEScalarFlowCompliance& SEFluidCircuitPath::GetCompliance()
	{
	  return GetCapacitance();
	}
	double SEFluidCircuitPath::GetCompliance(const FlowComplianceUnit& unit) const
	{
	  if (m_Capacitance == nullptr)
		return SEScalar::dNaN();
	  return m_Capacitance->GetValue(unit);
	}
	bool SEFluidCircuitPath::HasNextCompliance() const
	{
	  return HasNextCapacitance();
	}
	SEScalarFlowCompliance& SEFluidCircuitPath::GetNextCompliance()
	{
	  return GetNextCapacitance();
	}
	double SEFluidCircuitPath::GetNextCompliance(const FlowComplianceUnit& unit) const
	{
	  if (m_NextCapacitance == nullptr)
		return SEScalar::dNaN();
	  return m_NextCapacitance->GetValue(unit);
	}
	bool SEFluidCircuitPath::HasComplianceBaseline() const
	{
	  return HasCapacitanceBaseline();
	}
	SEScalarFlowCompliance& SEFluidCircuitPath::GetComplianceBaseline()
	{
	  return GetCapacitanceBaseline();
	}
	double SEFluidCircuitPath::GetComplianceBaseline(const FlowComplianceUnit& unit) const
	{
	  if (m_CapacitanceBaseline == nullptr)
		return SEScalar::dNaN();
	  return m_CapacitanceBaseline->GetValue(unit);
	}

	/////////////////////////////////
	// Fluid Inductance Types //
	/////////////////////////////////

	bool SEFluidCircuitPath::HasInertance() const
	{
	  return HasInductance();
	}
	SEScalarFlowInertance& SEFluidCircuitPath::GetInertance()
	{
	  return GetInductance();
	}
	double SEFluidCircuitPath::GetInertance(const FlowInertanceUnit& unit) const
	{
	  if (m_Inductance == nullptr)
		return SEScalar::dNaN();
	  return m_Inductance->GetValue(unit);
	}
	bool SEFluidCircuitPath::HasNextInertance() const
	{
	  return HasNextInductance();
	}
	SEScalarFlowInertance& SEFluidCircuitPath::GetNextInertance()
	{
	  return GetNextInductance();
	}
	double SEFluidCircuitPath::GetNextInertance(const FlowInertanceUnit& unit) const
	{
	  if (m_NextInductance == nullptr)
		return SEScalar::dNaN();
	  return m_NextInductance->GetValue(unit);
	}
	bool SEFluidCircuitPath::HasInertanceBaseline() const
	{
	  return HasInductanceBaseline();
	}
	SEScalarFlowInertance& SEFluidCircuitPath::GetInertanceBaseline()
	{
	  return GetInductanceBaseline();
	}
	double SEFluidCircuitPath::GetInertanceBaseline(const FlowInertanceUnit& unit) const
	{
	  if (m_InductanceBaseline == nullptr)
		return SEScalar::dNaN();
	  return m_InductanceBaseline->GetValue(unit);
	}

	///////////////////////////
	// Fluid Flux Types //
	///////////////////////////
	bool SEFluidCircuitPath::HasFlow() const
	{
	  return HasFlux();
	}
	SEScalarVolumePerTime& SEFluidCircuitPath::GetFlow()
	{
	  return GetFlux();
	}
	double SEFluidCircuitPath::GetFlow(const VolumePerTimeUnit& unit) const
	{
	  if (m_Flux == nullptr)
		return SEScalar::dNaN();
	  return m_Flux->GetValue(unit);
	}
	bool SEFluidCircuitPath::HasNextFlow() const
	{
	  return HasNextFlux();
	}
	SEScalarVolumePerTime& SEFluidCircuitPath::GetNextFlow()
	{
	  return GetNextFlux();
	}
	double SEFluidCircuitPath::GetNextFlow(const VolumePerTimeUnit& unit) const
	{
	  if (m_NextFlux == nullptr)
		return SEScalar::dNaN();
	  return m_NextFlux->GetValue(unit);
	}
	bool SEFluidCircuitPath::HasFlowSource() const
	{
	  return HasFluxSource();
	}
	SEScalarVolumePerTime& SEFluidCircuitPath::GetFlowSource()
	{
	  return GetFluxSource();
	}
	double SEFluidCircuitPath::GetFlowSource(const VolumePerTimeUnit& unit) const
	{
	  if (m_FluxSource == nullptr)
		return SEScalar::dNaN();
	  return m_FluxSource->GetValue(unit);
	}
	bool SEFluidCircuitPath::HasNextFlowSource() const
	{
	  return HasNextFluxSource();
	}
	SEScalarVolumePerTime& SEFluidCircuitPath::GetNextFlowSource()
	{
	  return GetNextFluxSource();
	}
	double SEFluidCircuitPath::GetNextFlowSource(const VolumePerTimeUnit& unit) const
	{
	  if (m_NextFluxSource == nullptr)
		return SEScalar::dNaN();
	  return m_NextFluxSource->GetValue(unit);
	}
	bool SEFluidCircuitPath::HasFlowSourceBaseline() const
	{
	  return HasFluxSourceBaseline();
	}
	SEScalarVolumePerTime& SEFluidCircuitPath::GetFlowSourceBaseline()
	{
	  return GetFluxSourceBaseline();
	}
	double SEFluidCircuitPath::GetFlowSourceBaseline(const VolumePerTimeUnit& unit) const
	{
	  if (m_FluxSourceBaseline == nullptr)
		return SEScalar::dNaN();
	  return m_FluxSourceBaseline->GetValue(unit);
	}

	////////////////////////////////
	// Fluid Potential Types //
	////////////////////////////////
	bool SEFluidCircuitPath::HasPressureSource() const
	{
	  return HasPotentialSource();
	}
	SEScalarPressure& SEFluidCircuitPath::GetPressureSource()
	{
	  return GetPotentialSource();
	}
	double SEFluidCircuitPath::GetPressureSource(const PressureUnit& unit) const
	{
	  if (m_PotentialSource == nullptr)
		return SEScalar::dNaN();
	  return m_PotentialSource->GetValue(unit);
	}
	bool SEFluidCircuitPath::HasNextPressureSource() const
	{
	  return HasNextPotentialSource();
	}
	SEScalarPressure& SEFluidCircuitPath::GetNextPressureSource()
	{
	  return GetNextPotentialSource();
	}
	double SEFluidCircuitPath::GetNextPressureSource(const PressureUnit& unit) const
	{
	  if (m_NextPotentialSource == nullptr)
		return SEScalar::dNaN();
	  return m_NextPotentialSource->GetValue(unit);
	}
	bool SEFluidCircuitPath::HasPressureSourceBaseline() const
	{
	  return HasPotentialSourceBaseline();
	}
	SEScalarPressure& SEFluidCircuitPath::GetPressureSourceBaseline()
	{
	  return GetPotentialSourceBaseline();
	}
	double SEFluidCircuitPath::GetPressureSourceBaseline(const PressureUnit& unit) const
	{
	  if (m_PotentialSourceBaseline == nullptr)
		return SEScalar::dNaN();
	  return m_PotentialSourceBaseline->GetValue(unit);
	}
	bool SEFluidCircuitPath::HasValveBreakdownPressure() const
	{
	  return HasValveBreakdownPotential();
	}
	SEScalarPressure& SEFluidCircuitPath::GetValveBreakdownPressure()
	{
	  return GetValveBreakdownPotential();
	}
	double SEFluidCircuitPath::GetValveBreakdownPressure(const PressureUnit& unit) const
	{
	  if (m_ValveBreakdownPotential == nullptr)
		return SEScalar::dNaN();
	  return m_ValveBreakdownPotential->GetValue(unit);
	}
	bool SEFluidCircuitPath::HasCardiovascularRegion() const
	{
	  return m_CardiovascularRegion == ((CDM::enumResistancePathType::value)-1) ? false : true;
	}
	CDM::enumResistancePathType::value SEFluidCircuitPath::GetCardiovascularRegion() const
	{
	  return m_CardiovascularRegion;
	}
	void SEFluidCircuitPath::SetCardiovascularRegion(CDM::enumResistancePathType::value pType)
	{
	  m_CardiovascularRegion = pType;
	}
	void SEFluidCircuitPath::InvalidateCardiovascularRegion()
	{
	  m_CardiovascularRegion = (CDM::enumResistancePathType::value)-1;
	}
}