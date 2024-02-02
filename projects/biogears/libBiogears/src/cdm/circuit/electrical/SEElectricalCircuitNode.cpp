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

#include <biogears/cdm/circuit/electrical/SEElectricalCircuitNode.h>

//Private Includes
#include <io/cdm/Circuit.h>

namespace biogears {
SEElectricalCircuitNode::SEElectricalCircuitNode(const char* name, Logger* logger)
  : SECircuitNode<SEScalarElectricPotential, SEScalarElectricCharge>(name, logger)
//-------------------------------------------------------------------------------
{
}
SEElectricalCircuitNode::SEElectricalCircuitNode(const std::string& name, Logger* logger)
  : SECircuitNode<SEScalarElectricPotential, SEScalarElectricCharge>(name, logger)
{
}
//-------------------------------------------------------------------------------
SEElectricalCircuitNode::~SEElectricalCircuitNode()
{
}
//-------------------------------------------------------------------------------
void SEElectricalCircuitNode::Clear()
{
  SECircuitNode::Clear();
}
//-------------------------------------------------------------------------------
bool SEElectricalCircuitNode::Load(const CDM::ElectricalCircuitNodeData& in)
{
  io::Circuit::UnMarshall(in, *this);
  return true;
}
//-------------------------------------------------------------------------------
CDM::ElectricalCircuitNodeData* SEElectricalCircuitNode::Unload() const
{
  CDM::ElectricalCircuitNodeData* data = new CDM::ElectricalCircuitNodeData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SEElectricalCircuitNode::Unload(CDM::ElectricalCircuitNodeData& data) const
{
  io::Circuit::Marshall(*this, data);
}

//-------------------------------------------------------------------------------
bool SEElectricalCircuitNode::HasVoltage() const
{
  return HasPotential();
}
//-------------------------------------------------------------------------------
SEScalarElectricPotential& SEElectricalCircuitNode::GetVoltage()
{
  return GetPotential();
}
//-------------------------------------------------------------------------------
double SEElectricalCircuitNode::GetVoltage(const ElectricPotentialUnit& unit) const
{
  if (m_Potential == nullptr)
    return SEScalar::dNaN();
  return m_Potential->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEElectricalCircuitNode::HasNextVoltage() const
{
  return HasNextPotential();
}
//-------------------------------------------------------------------------------
SEScalarElectricPotential& SEElectricalCircuitNode::GetNextVoltage()
{
  return GetNextPotential();
}
//-------------------------------------------------------------------------------
double SEElectricalCircuitNode::GetNextVoltage(const ElectricPotentialUnit& unit) const
{
  if (m_NextPotential == nullptr)
    return SEScalar::dNaN();
  return m_NextPotential->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEElectricalCircuitNode::HasCharge() const
{
  return HasQuantity();
}
//-------------------------------------------------------------------------------
SEScalarElectricCharge& SEElectricalCircuitNode::GetCharge()
{
  return GetQuantity();
}
//-------------------------------------------------------------------------------
double SEElectricalCircuitNode::GetCharge(const ElectricChargeUnit& unit) const
{
  if (m_Quantity == nullptr)
    return SEScalar::dNaN();
  return m_Quantity->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEElectricalCircuitNode::HasNextCharge() const
{
  return HasNextQuantity();
}
//-------------------------------------------------------------------------------
SEScalarElectricCharge& SEElectricalCircuitNode::GetNextCharge()
{
  return GetNextQuantity();
}
//-------------------------------------------------------------------------------
double SEElectricalCircuitNode::GetNextCharge(const ElectricChargeUnit& unit) const
{
  if (m_NextQuantity == nullptr)
    return SEScalar::dNaN();
  return m_NextQuantity->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEElectricalCircuitNode::HasChargeBaseline() const
{
  return HasQuantityBaseline();
}
//-------------------------------------------------------------------------------
SEScalarElectricCharge& SEElectricalCircuitNode::GetChargeBaseline()
{
  return GetQuantityBaseline();
}
//-------------------------------------------------------------------------------
double SEElectricalCircuitNode::GetChargeBaseline(const ElectricChargeUnit& unit) const
{
  if (m_QuantityBaseline == nullptr)
    return SEScalar::dNaN();
  return m_QuantityBaseline->GetValue(unit);
}
//-------------------------------------------------------------------------------
}