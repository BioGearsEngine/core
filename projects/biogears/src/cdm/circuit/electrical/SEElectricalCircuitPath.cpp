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

#include <biogears/cdm/circuit/electrical/SEElectricalCircuitPath.h>

namespace biogears {
SEElectricalCircuitPath::SEElectricalCircuitPath(SEElectricalCircuitNode& src, SEElectricalCircuitNode& tgt, const char* name)
  : SECircuitPath<SEScalarElectricCurrent, SEScalarElectricResistance, SEScalarElectricCapacitance, SEScalarElectricInductance, SEScalarElectricPotential, SEScalarElectricCharge>(src, tgt, name)
  , m_ElectricalSourceNode(src)
  , m_ElectricalTargetNode(tgt)
{
}
SEElectricalCircuitPath::SEElectricalCircuitPath(SEElectricalCircuitNode& src, SEElectricalCircuitNode& tgt, const std::string& name)
  : SECircuitPath<SEScalarElectricCurrent, SEScalarElectricResistance, SEScalarElectricCapacitance, SEScalarElectricInductance, SEScalarElectricPotential, SEScalarElectricCharge>(src, tgt, name)
  , m_ElectricalSourceNode(src)
  , m_ElectricalTargetNode(tgt)
{
}
SEElectricalCircuitPath::~SEElectricalCircuitPath()
{
}

void SEElectricalCircuitPath::Clear()
{
  SECircuitPath::Clear();
}

bool SEElectricalCircuitPath::Load(const CDM::ElectricalCircuitPathData& in)
{
  SECircuitPath::Load(in);
  if (in.Resistance().present())
    GetResistance().Load(in.Resistance().get());
  if (in.NextResistance().present())
    GetNextResistance().Load(in.NextResistance().get());
  if (in.ResistanceBaseline().present())
    GetResistanceBaseline().Load(in.ResistanceBaseline().get());
  if (in.Capacitance().present())
    GetCapacitance().Load(in.Capacitance().get());
  if (in.NextCapacitance().present())
    GetNextCapacitance().Load(in.NextCapacitance().get());
  if (in.CapacitanceBaseline().present())
    GetCapacitanceBaseline().Load(in.CapacitanceBaseline().get());
  if (in.Inductance().present())
    GetInductance().Load(in.Inductance().get());
  if (in.NextInductance().present())
    GetNextInductance().Load(in.NextInductance().get());
  if (in.InductanceBaseline().present())
    GetInductanceBaseline().Load(in.InductanceBaseline().get());
  if (in.Current().present())
    GetCurrent().Load(in.Current().get());
  if (in.NextCurrent().present())
    GetNextCurrent().Load(in.NextCurrent().get());
  if (in.CurrentSource().present())
    GetCurrentSource().Load(in.CurrentSource().get());
  if (in.NextCurrentSource().present())
    GetNextCurrentSource().Load(in.NextCurrentSource().get());
  if (in.CurrentSourceBaseline().present())
    GetCurrentSourceBaseline().Load(in.CurrentSourceBaseline().get());
  if (in.VoltageSource().present())
    GetVoltageSource().Load(in.VoltageSource().get());
  if (in.NextVoltageSource().present())
    GetNextVoltageSource().Load(in.NextVoltageSource().get());
  if (in.VoltageSourceBaseline().present())
    GetVoltageSourceBaseline().Load(in.VoltageSourceBaseline().get());
  if (in.ValveBreakdownVoltage().present())
    GetValveBreakdownVoltage().Load(in.ValveBreakdownVoltage().get());

  return HasValidElements();
}
CDM::ElectricalCircuitPathData* SEElectricalCircuitPath::Unload() const
{
  CDM::ElectricalCircuitPathData* data = new CDM::ElectricalCircuitPathData();
  Unload(*data);
  return data;
}
void SEElectricalCircuitPath::Unload(CDM::ElectricalCircuitPathData& data) const
{
  SECircuitPath::Unload(data);
  if (HasResistance())
    data.Resistance(std::unique_ptr<CDM::ScalarElectricResistanceData>(m_Resistance->Unload()));
  if (HasNextResistance())
    data.NextResistance(std::unique_ptr<CDM::ScalarElectricResistanceData>(m_NextResistance->Unload()));
  if (HasResistanceBaseline())
    data.ResistanceBaseline(std::unique_ptr<CDM::ScalarElectricResistanceData>(m_ResistanceBaseline->Unload()));
  if (HasCapacitance())
    data.Capacitance(std::unique_ptr<CDM::ScalarElectricCapacitanceData>(m_Capacitance->Unload()));
  if (HasNextCapacitance())
    data.NextCapacitance(std::unique_ptr<CDM::ScalarElectricCapacitanceData>(m_NextCapacitance->Unload()));
  if (HasCapacitanceBaseline())
    data.CapacitanceBaseline(std::unique_ptr<CDM::ScalarElectricCapacitanceData>(m_CapacitanceBaseline->Unload()));
  if (HasInductance())
    data.Inductance(std::unique_ptr<CDM::ScalarElectricInductanceData>(m_Inductance->Unload()));
  if (HasNextInductance())
    data.NextInductance(std::unique_ptr<CDM::ScalarElectricInductanceData>(m_NextInductance->Unload()));
  if (HasInductanceBaseline())
    data.InductanceBaseline(std::unique_ptr<CDM::ScalarElectricInductanceData>(m_InductanceBaseline->Unload()));
  if (HasCurrent())
    data.Current(std::unique_ptr<CDM::ScalarElectricCurrentData>(m_Flux->Unload()));
  if (HasNextCurrent())
    data.NextCurrent(std::unique_ptr<CDM::ScalarElectricCurrentData>(m_NextFlux->Unload()));
  if (HasCurrentSource())
    data.CurrentSource(std::unique_ptr<CDM::ScalarElectricCurrentData>(m_FluxSource->Unload()));
  if (HasNextCurrentSource())
    data.NextCurrentSource(std::unique_ptr<CDM::ScalarElectricCurrentData>(m_NextFluxSource->Unload()));
  if (HasCurrentSourceBaseline())
    data.CurrentSourceBaseline(std::unique_ptr<CDM::ScalarElectricCurrentData>(m_FluxSourceBaseline->Unload()));
  if (HasVoltageSource())
    data.VoltageSource(std::unique_ptr<CDM::ScalarElectricPotentialData>(m_PotentialSource->Unload()));
  if (HasNextVoltageSource())
    data.NextVoltageSource(std::unique_ptr<CDM::ScalarElectricPotentialData>(m_NextPotentialSource->Unload()));
  if (HasVoltageSourceBaseline())
    data.VoltageSourceBaseline(std::unique_ptr<CDM::ScalarElectricPotentialData>(m_PotentialSourceBaseline->Unload()));
  if (HasValveBreakdownVoltage())
    data.ValveBreakdownVoltage(std::unique_ptr<CDM::ScalarElectricPotentialData>(m_ValveBreakdownPotential->Unload()));
}

////////////////////////////////
// Electrical Resistance Types//
////////////////////////////////

bool SEElectricalCircuitPath::HasResistance() const
{
  return SECircuitPath::HasResistance();
}
SEScalarElectricResistance& SEElectricalCircuitPath::GetResistance()
{
  return SECircuitPath::GetResistance();
}
double SEElectricalCircuitPath::GetResistance(const ElectricResistanceUnit& unit) const
{
  if (m_Resistance == nullptr)
    return SEScalar::dNaN();
  return m_Resistance->GetValue(unit);
}
bool SEElectricalCircuitPath::HasNextResistance() const
{
  return SECircuitPath::HasNextResistance();
}
SEScalarElectricResistance& SEElectricalCircuitPath::GetNextResistance()
{
  return SECircuitPath::GetNextResistance();
}
double SEElectricalCircuitPath::GetNextResistance(const ElectricResistanceUnit& unit) const
{
  if (m_NextResistance == nullptr)
    return SEScalar::dNaN();
  return m_NextResistance->GetValue(unit);
}
bool SEElectricalCircuitPath::HasResistanceBaseline() const
{
  return SECircuitPath::HasResistanceBaseline();
}
SEScalarElectricResistance& SEElectricalCircuitPath::GetResistanceBaseline()
{
  return SECircuitPath::GetResistanceBaseline();
}
double SEElectricalCircuitPath::GetResistanceBaseline(const ElectricResistanceUnit& unit) const
{
  if (m_ResistanceBaseline == nullptr)
    return SEScalar::dNaN();
  return m_ResistanceBaseline->GetValue(unit);
}

//////////////////////////////////
// Electrical Capacitance Types //
//////////////////////////////////

bool SEElectricalCircuitPath::HasCapacitance() const
{
  return SECircuitPath::HasCapacitance();
}
SEScalarElectricCapacitance& SEElectricalCircuitPath::GetCapacitance()
{
  return SECircuitPath::GetCapacitance();
}
double SEElectricalCircuitPath::GetCapacitance(const ElectricCapacitanceUnit& unit) const
{
  if (m_Capacitance == nullptr)
    return SEScalar::dNaN();
  return m_Capacitance->GetValue(unit);
}
bool SEElectricalCircuitPath::HasNextCapacitance() const
{
  return SECircuitPath::HasNextCapacitance();
}
SEScalarElectricCapacitance& SEElectricalCircuitPath::GetNextCapacitance()
{
  return SECircuitPath::GetNextCapacitance();
}
double SEElectricalCircuitPath::GetNextCapacitance(const ElectricCapacitanceUnit& unit) const
{
  if (m_NextCapacitance == nullptr)
    return SEScalar::dNaN();
  return m_NextCapacitance->GetValue(unit);
}
bool SEElectricalCircuitPath::HasCapacitanceBaseline() const
{
  return SECircuitPath::HasCapacitance();
}
SEScalarElectricCapacitance& SEElectricalCircuitPath::GetCapacitanceBaseline()
{
  return SECircuitPath::GetCapacitance();
}
double SEElectricalCircuitPath::GetCapacitanceBaseline(const ElectricCapacitanceUnit& unit) const
{
  if (m_CapacitanceBaseline == nullptr)
    return SEScalar::dNaN();
  return m_CapacitanceBaseline->GetValue(unit);
}

/////////////////////////////////
// Electrical Inductance Types //
/////////////////////////////////

bool SEElectricalCircuitPath::HasInductance() const
{
  return SECircuitPath::HasInductance();
}
SEScalarElectricInductance& SEElectricalCircuitPath::GetInductance()
{
  return SECircuitPath::GetInductance();
}
double SEElectricalCircuitPath::GetInductance(const ElectricInductanceUnit& unit) const
{
  if (m_Inductance == nullptr)
    return SEScalar::dNaN();
  return m_Inductance->GetValue(unit);
}
bool SEElectricalCircuitPath::HasNextInductance() const
{
  return SECircuitPath::HasNextInductance();
}
SEScalarElectricInductance& SEElectricalCircuitPath::GetNextInductance()
{
  return SECircuitPath::GetNextInductance();
}
double SEElectricalCircuitPath::GetNextInductance(const ElectricInductanceUnit& unit) const
{
  if (m_NextInductance == nullptr)
    return SEScalar::dNaN();
  return m_NextInductance->GetValue(unit);
}
bool SEElectricalCircuitPath::HasInductanceBaseline() const
{
  return SECircuitPath::HasInductanceBaseline();
}
SEScalarElectricInductance& SEElectricalCircuitPath::GetInductanceBaseline()
{
  return SECircuitPath::GetInductanceBaseline();
}
double SEElectricalCircuitPath::GetInductanceBaseline(const ElectricInductanceUnit& unit) const
{
  if (m_InductanceBaseline == nullptr)
    return SEScalar::dNaN();
  return m_InductanceBaseline->GetValue(unit);
}

///////////////////////////
// Electrical Flux Types //
///////////////////////////
bool SEElectricalCircuitPath::HasCurrent() const
{
  return HasFlux();
}
SEScalarElectricCurrent& SEElectricalCircuitPath::GetCurrent()
{
  return GetFlux();
}
double SEElectricalCircuitPath::GetCurrent(const ElectricCurrentUnit& unit) const
{
  if (m_Flux == nullptr)
    return SEScalar::dNaN();
  return m_Flux->GetValue(unit);
}
bool SEElectricalCircuitPath::HasNextCurrent() const
{
  return HasNextFlux();
}
SEScalarElectricCurrent& SEElectricalCircuitPath::GetNextCurrent()
{
  return GetNextFlux();
}
double SEElectricalCircuitPath::GetNextCurrent(const ElectricCurrentUnit& unit) const
{
  if (m_NextFlux == nullptr)
    return SEScalar::dNaN();
  return m_NextFlux->GetValue(unit);
}
bool SEElectricalCircuitPath::HasCurrentSource() const
{
  return HasFluxSource();
}
SEScalarElectricCurrent& SEElectricalCircuitPath::GetCurrentSource()
{
  return GetFluxSource();
}
double SEElectricalCircuitPath::GetCurrentSource(const ElectricCurrentUnit& unit) const
{
  if (m_FluxSource == nullptr)
    return SEScalar::dNaN();
  return m_FluxSource->GetValue(unit);
}
bool SEElectricalCircuitPath::HasNextCurrentSource() const
{
  return HasNextFluxSource();
}
SEScalarElectricCurrent& SEElectricalCircuitPath::GetNextCurrentSource()
{
  return GetNextFluxSource();
}
double SEElectricalCircuitPath::GetNextCurrentSource(const ElectricCurrentUnit& unit) const
{
  if (m_NextFluxSource == nullptr)
    return SEScalar::dNaN();
  return m_NextFluxSource->GetValue(unit);
}
bool SEElectricalCircuitPath::HasCurrentSourceBaseline() const
{
  return HasFluxSourceBaseline();
}
SEScalarElectricCurrent& SEElectricalCircuitPath::GetCurrentSourceBaseline()
{
  return GetFluxSourceBaseline();
}
double SEElectricalCircuitPath::GetCurrentSourceBaseline(const ElectricCurrentUnit& unit) const
{
  if (m_FluxSourceBaseline == nullptr)
    return SEScalar::dNaN();
  return m_FluxSourceBaseline->GetValue(unit);
}

////////////////////////////////
// Electrical Potential Types //
////////////////////////////////
bool SEElectricalCircuitPath::HasVoltageSource() const
{
  return HasPotentialSource();
}
SEScalarElectricPotential& SEElectricalCircuitPath::GetVoltageSource()
{
  return GetPotentialSource();
}
double SEElectricalCircuitPath::GetVoltageSource(const ElectricPotentialUnit& unit) const
{
  if (m_PotentialSource == nullptr)
    return SEScalar::dNaN();
  return m_PotentialSource->GetValue(unit);
}
bool SEElectricalCircuitPath::HasNextVoltageSource() const
{
  return HasNextPotentialSource();
}
SEScalarElectricPotential& SEElectricalCircuitPath::GetNextVoltageSource()
{
  return GetNextPotentialSource();
}
double SEElectricalCircuitPath::GetNextVoltageSource(const ElectricPotentialUnit& unit) const
{
  if (m_NextPotentialSource == nullptr)
    return SEScalar::dNaN();
  return m_NextPotentialSource->GetValue(unit);
}
bool SEElectricalCircuitPath::HasVoltageSourceBaseline() const
{
  return HasPotentialSourceBaseline();
}
SEScalarElectricPotential& SEElectricalCircuitPath::GetVoltageSourceBaseline()
{
  return GetPotentialSourceBaseline();
}
double SEElectricalCircuitPath::GetVoltageSourceBaseline(const ElectricPotentialUnit& unit) const
{
  if (m_PotentialSourceBaseline == nullptr)
    return SEScalar::dNaN();
  return m_PotentialSourceBaseline->GetValue(unit);
}
bool SEElectricalCircuitPath::HasValveBreakdownVoltage() const
{
  return HasValveBreakdownPotential();
}
SEScalarElectricPotential& SEElectricalCircuitPath::GetValveBreakdownVoltage()
{
  return GetValveBreakdownPotential();
}
double SEElectricalCircuitPath::GetValveBreakdownVoltage(const ElectricPotentialUnit& unit) const
{
  if (m_ValveBreakdownPotential == nullptr)
    return SEScalar::dNaN();
  return m_ValveBreakdownPotential->GetValue(unit);
}
}