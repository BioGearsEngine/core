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

#include <biogears/cdm/circuit/thermal/SEThermalCircuitPath.h>

#include "io/cdm/Circuit.h"

namespace biogears {

template class SECircuitPath<FLUID_CIRCUIT_PATH>;

SEThermalCircuitPath::SEThermalCircuitPath(SEThermalCircuitNode& src, SEThermalCircuitNode& tgt, const char* name)
  :SEThermalCircuitPath(src, tgt, std::string{name})
{}
//-------------------------------------------------------------------------------
SEThermalCircuitPath::SEThermalCircuitPath(SEThermalCircuitNode& src, SEThermalCircuitNode& tgt, const std::string& name)
  : SECircuitPath<SEScalarPower, SEScalarHeatResistance, SEScalarHeatCapacitance, SEScalarHeatInductance, SEScalarTemperature, SEScalarEnergy>(src, tgt, name)
  , m_ThermalSourceNode(src)
  , m_ThermalTargetNode(tgt)
{
}
//-------------------------------------------------------------------------------
SEThermalCircuitPath::~SEThermalCircuitPath()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEThermalCircuitPath::Clear()
{
  SECircuitPath::Clear();
}
//-------------------------------------------------------------------------------
bool SEThermalCircuitPath::Load(const CDM::ThermalCircuitPathData& in)
{
  io::Circuit::UnMarshall(in, *this);
  return HasValidElements();
}
//-------------------------------------------------------------------------------
CDM::ThermalCircuitPathData* SEThermalCircuitPath::Unload() const
{
  CDM::ThermalCircuitPathData* data = new CDM::ThermalCircuitPathData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SEThermalCircuitPath::Unload(CDM::ThermalCircuitPathData& data) const
{
  io::Circuit::Marshall(*this, data);
}
//-------------------------------------------------------------------------------

////////////////////////////////
// Thermal Resistance Types//
////////////////////////////////

bool SEThermalCircuitPath::HasResistance() const
{
  return SECircuitPath::HasResistance();
}
//-------------------------------------------------------------------------------
SEScalarHeatResistance& SEThermalCircuitPath::GetResistance()
{
  return SECircuitPath::GetResistance();
}
//-------------------------------------------------------------------------------
double SEThermalCircuitPath::GetResistance(const HeatResistanceUnit& unit) const
{
  if (m_Resistance == nullptr)
    return SEScalar::dNaN();
  return m_Resistance->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEThermalCircuitPath::HasNextResistance() const
{
  return SECircuitPath::HasNextResistance();
}
//-------------------------------------------------------------------------------
SEScalarHeatResistance& SEThermalCircuitPath::GetNextResistance()
{
  return SECircuitPath::GetNextResistance();
}
//-------------------------------------------------------------------------------
double SEThermalCircuitPath::GetNextResistance(const HeatResistanceUnit& unit) const
{
  if (m_NextResistance == nullptr)
    return SEScalar::dNaN();
  return m_NextResistance->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEThermalCircuitPath::HasResistanceBaseline() const
{
  return SECircuitPath::HasResistanceBaseline();
}
//-------------------------------------------------------------------------------
SEScalarHeatResistance& SEThermalCircuitPath::GetResistanceBaseline()
{
  return SECircuitPath::GetResistanceBaseline();
}
//-------------------------------------------------------------------------------
double SEThermalCircuitPath::GetResistanceBaseline(const HeatResistanceUnit& unit) const
{
  if (m_ResistanceBaseline == nullptr)
    return SEScalar::dNaN();
  return m_ResistanceBaseline->GetValue(unit);
}
//-------------------------------------------------------------------------------

//////////////////////////////////
// Thermal Capacitance Types //
//////////////////////////////////

bool SEThermalCircuitPath::HasCapacitance() const
{
  return SECircuitPath::HasCapacitance();
}
//-------------------------------------------------------------------------------
SEScalarHeatCapacitance& SEThermalCircuitPath::GetCapacitance()
{
  return SECircuitPath::GetCapacitance();
}
//-------------------------------------------------------------------------------
double SEThermalCircuitPath::GetCapacitance(const HeatCapacitanceUnit& unit) const
{
  if (m_Capacitance == nullptr)
    return SEScalar::dNaN();
  return m_Capacitance->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEThermalCircuitPath::HasNextCapacitance() const
{
  return SECircuitPath::HasNextCapacitance();
}
//-------------------------------------------------------------------------------
SEScalarHeatCapacitance& SEThermalCircuitPath::GetNextCapacitance()
{
  return SECircuitPath::GetNextCapacitance();
}
//-------------------------------------------------------------------------------
double SEThermalCircuitPath::GetNextCapacitance(const HeatCapacitanceUnit& unit) const
{
  if (m_NextCapacitance == nullptr)
    return SEScalar::dNaN();
  return m_NextCapacitance->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEThermalCircuitPath::HasCapacitanceBaseline() const
{
  return SECircuitPath::HasCapacitanceBaseline();
}
//-------------------------------------------------------------------------------
SEScalarHeatCapacitance& SEThermalCircuitPath::GetCapacitanceBaseline()
{
  return SECircuitPath::GetCapacitanceBaseline();
}
//-------------------------------------------------------------------------------
double SEThermalCircuitPath::GetCapacitanceBaseline(const HeatCapacitanceUnit& unit) const
{
  if (m_CapacitanceBaseline == nullptr)
    return SEScalar::dNaN();
  return m_CapacitanceBaseline->GetValue(unit);
}
//-------------------------------------------------------------------------------

/////////////////////////////////
// Thermal Inductance Types //
/////////////////////////////////

bool SEThermalCircuitPath::HasInductance() const
{
  return SECircuitPath::HasInductance();
}
//-------------------------------------------------------------------------------
SEScalarHeatInductance& SEThermalCircuitPath::GetInductance()
{
  return SECircuitPath::GetInductance();
}
//-------------------------------------------------------------------------------
double SEThermalCircuitPath::GetInductance(const HeatInductanceUnit& unit) const
{
  if (m_Inductance == nullptr)
    return SEScalar::dNaN();
  return m_Inductance->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEThermalCircuitPath::HasNextInductance() const
{
  return SECircuitPath::HasNextInductance();
}
//-------------------------------------------------------------------------------
SEScalarHeatInductance& SEThermalCircuitPath::GetNextInductance()
{
  return SECircuitPath::GetNextInductance();
}
//-------------------------------------------------------------------------------
double SEThermalCircuitPath::GetNextInductance(const HeatInductanceUnit& unit) const
{
  if (m_NextInductance == nullptr)
    return SEScalar::dNaN();
  return m_NextInductance->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEThermalCircuitPath::HasInductanceBaseline() const
{
  return SECircuitPath::HasInductanceBaseline();
}
//-------------------------------------------------------------------------------
SEScalarHeatInductance& SEThermalCircuitPath::GetInductanceBaseline()
{
  return SECircuitPath::GetInductanceBaseline();
}
//-------------------------------------------------------------------------------
double SEThermalCircuitPath::GetInductanceBaseline(const HeatInductanceUnit& unit) const
{
  if (m_InductanceBaseline == nullptr)
    return SEScalar::dNaN();
  return m_InductanceBaseline->GetValue(unit);
}
//-------------------------------------------------------------------------------

///////////////////////////
// Thermal Flux Types //
///////////////////////////
bool SEThermalCircuitPath::HasHeatTransferRate() const
{
  return HasFlux();
}
//-------------------------------------------------------------------------------
SEScalarPower& SEThermalCircuitPath::GetHeatTransferRate()
{
  return GetFlux();
}
//-------------------------------------------------------------------------------
double SEThermalCircuitPath::GetHeatTransferRate(const PowerUnit& unit) const
{
  if (m_Flux == nullptr)
    return SEScalar::dNaN();
  return m_Flux->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEThermalCircuitPath::HasNextHeatTransferRate() const
{
  return HasNextFlux();
}
//-------------------------------------------------------------------------------
SEScalarPower& SEThermalCircuitPath::GetNextHeatTransferRate()
{
  return GetNextFlux();
}
//-------------------------------------------------------------------------------
double SEThermalCircuitPath::GetNextHeatTransferRate(const PowerUnit& unit) const
{
  if (m_NextFlux == nullptr)
    return SEScalar::dNaN();
  return m_NextFlux->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEThermalCircuitPath::HasHeatSource() const
{
  return HasFluxSource();
}
//-------------------------------------------------------------------------------
SEScalarPower& SEThermalCircuitPath::GetHeatSource()
{
  return GetFluxSource();
}
//-------------------------------------------------------------------------------
double SEThermalCircuitPath::GetHeatSource(const PowerUnit& unit) const
{
  if (m_FluxSource == nullptr)
    return SEScalar::dNaN();
  return m_FluxSource->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEThermalCircuitPath::HasNextHeatSource() const
{
  return HasNextFluxSource();
}
//-------------------------------------------------------------------------------
SEScalarPower& SEThermalCircuitPath::GetNextHeatSource()
{
  return GetNextFluxSource();
}
//-------------------------------------------------------------------------------
double SEThermalCircuitPath::GetNextHeatSource(const PowerUnit& unit) const
{
  if (m_NextFluxSource == nullptr)
    return SEScalar::dNaN();
  return m_NextFluxSource->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEThermalCircuitPath::HasHeatSourceBaseline() const
{
  return HasFluxSourceBaseline();
}
//-------------------------------------------------------------------------------
SEScalarPower& SEThermalCircuitPath::GetHeatSourceBaseline()
{
  return GetFluxSourceBaseline();
}
//-------------------------------------------------------------------------------
double SEThermalCircuitPath::GetHeatSourceBaseline(const PowerUnit& unit) const
{
  if (m_FluxSourceBaseline == nullptr)
    return SEScalar::dNaN();
  return m_FluxSourceBaseline->GetValue(unit);
}
//-------------------------------------------------------------------------------

////////////////////////////////
// Thermal Potential Types //
////////////////////////////////
bool SEThermalCircuitPath::HasTemperatureSource() const
{
  return HasPotentialSource();
}
//-------------------------------------------------------------------------------
SEScalarTemperature& SEThermalCircuitPath::GetTemperatureSource()
{
  return GetPotentialSource();
}
//-------------------------------------------------------------------------------
double SEThermalCircuitPath::GetTemperatureSource(const TemperatureUnit& unit) const
{
  if (m_PotentialSource == nullptr)
    return SEScalar::dNaN();
  return m_PotentialSource->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEThermalCircuitPath::HasNextTemperatureSource() const
{
  return HasNextPotentialSource();
}
//-------------------------------------------------------------------------------
SEScalarTemperature& SEThermalCircuitPath::GetNextTemperatureSource()
{
  return GetNextPotentialSource();
}
//-------------------------------------------------------------------------------
double SEThermalCircuitPath::GetNextTemperatureSource(const TemperatureUnit& unit) const
{
  if (m_NextPotentialSource == nullptr)
    return SEScalar::dNaN();
  return m_NextPotentialSource->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEThermalCircuitPath::HasTemperatureSourceBaseline() const
{
  return HasPotentialSourceBaseline();
}
//-------------------------------------------------------------------------------
SEScalarTemperature& SEThermalCircuitPath::GetTemperatureSourceBaseline()
{
  return GetPotentialSourceBaseline();
}
//-------------------------------------------------------------------------------
double SEThermalCircuitPath::GetTemperatureSourceBaseline(const TemperatureUnit& unit) const
{
  if (m_PotentialSourceBaseline == nullptr)
    return SEScalar::dNaN();
  return m_PotentialSourceBaseline->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEThermalCircuitPath::HasValveBreakdownTemperature() const
{
  return HasValveBreakdownPotential();
}
//-------------------------------------------------------------------------------
SEScalarTemperature& SEThermalCircuitPath::GetValveBreakdownTemperature()
{
  return GetValveBreakdownPotential();
}
//-------------------------------------------------------------------------------
double SEThermalCircuitPath::GetValveBreakdownTemperature(const TemperatureUnit& unit) const
{
  if (m_ValveBreakdownPotential == nullptr)
    return SEScalar::dNaN();
  return m_ValveBreakdownPotential->GetValue(unit);
}
//-------------------------------------------------------------------------------
}