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

#include <biogears/cdm/circuit/thermal/SEThermalCircuitNode.h>

#include "io/cdm/Circuit.h"

namespace biogears {
SEThermalCircuitNode::SEThermalCircuitNode(const char* name, Logger* logger)
  : SEThermalCircuitNode(std::string{ name }, logger)
{
}
//-------------------------------------------------------------------------------
SEThermalCircuitNode::SEThermalCircuitNode(const std::string& name, Logger* logger)
  : SECircuitNode<THERMAL_CIRCUIT_NODE>(name, logger)
{
}
//-------------------------------------------------------------------------------
SEThermalCircuitNode::~SEThermalCircuitNode()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEThermalCircuitNode::Clear()
{
  SECircuitNode::Clear();
}
//-------------------------------------------------------------------------------
bool SEThermalCircuitNode::HasTemperature() const
{
  return HasPotential();
}
//-------------------------------------------------------------------------------
SEScalarTemperature& SEThermalCircuitNode::GetTemperature()
{
  return GetPotential();
}
//-------------------------------------------------------------------------------
double SEThermalCircuitNode::GetTemperature(const TemperatureUnit& unit) const
{
  if (m_Potential == nullptr)
    return SEScalar::dNaN();
  return m_Potential->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEThermalCircuitNode::HasNextTemperature() const
{
  return HasNextPotential();
}
//-------------------------------------------------------------------------------
SEScalarTemperature& SEThermalCircuitNode::GetNextTemperature()
{
  return GetNextPotential();
}
//-------------------------------------------------------------------------------
double SEThermalCircuitNode::GetNextTemperature(const TemperatureUnit& unit) const
{
  if (m_NextPotential == nullptr)
    return SEScalar::dNaN();
  return m_NextPotential->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEThermalCircuitNode::HasHeat() const
{
  return HasQuantity();
}
//-------------------------------------------------------------------------------
SEScalarEnergy& SEThermalCircuitNode::GetHeat()
{
  return GetQuantity();
}
//-------------------------------------------------------------------------------
double SEThermalCircuitNode::GetHeat(const EnergyUnit& unit) const
{
  if (m_Quantity == nullptr)
    return SEScalar::dNaN();
  return m_Quantity->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEThermalCircuitNode::HasNextHeat() const
{
  return HasNextQuantity();
}
//-------------------------------------------------------------------------------
SEScalarEnergy& SEThermalCircuitNode::GetNextHeat()
{
  return GetNextQuantity();
}
//-------------------------------------------------------------------------------
double SEThermalCircuitNode::GetNextHeat(const EnergyUnit& unit) const
{
  if (m_NextQuantity == nullptr)
    return SEScalar::dNaN();
  return m_NextQuantity->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEThermalCircuitNode::HasHeatBaseline() const
{
  return HasQuantityBaseline();
}
//-------------------------------------------------------------------------------
SEScalarEnergy& SEThermalCircuitNode::GetHeatBaseline()
{
  return GetQuantityBaseline();
}
//-------------------------------------------------------------------------------
double SEThermalCircuitNode::GetHeatBaseline(const EnergyUnit& unit) const
{
  if (m_QuantityBaseline == nullptr)
    return SEScalar::dNaN();
  return m_QuantityBaseline->GetValue(unit);
}
//-------------------------------------------------------------------------------
}