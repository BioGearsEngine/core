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

#include "io/cdm/Circuit.h"

namespace biogears {

    template class  SECircuitPath<SEScalarElectricCurrent, SEScalarElectricResistance, SEScalarElectricCapacitance, SEScalarElectricInductance, SEScalarElectricPotential, SEScalarElectricCharge>;

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
  io::Circuit::UnMarshall(in, *this);
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
  io::Circuit::Marshall(*this, data);
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