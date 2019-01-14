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

#include <biogears/cdm/circuit/fluid/SEFluidCircuitNode.h>

namespace biogears {
SEFluidCircuitNode::SEFluidCircuitNode(const char* name, Logger* logger)
  : SECircuitNode<SEScalarPressure, SEScalarVolume>(std::string{ name }, logger)
{
}
//-----------------------------------------------------------------------------
SEFluidCircuitNode::SEFluidCircuitNode(const std::string& name, Logger* logger)
  : SECircuitNode<SEScalarPressure, SEScalarVolume>(name, logger)
{
}
//-----------------------------------------------------------------------------
SEFluidCircuitNode::~SEFluidCircuitNode()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEFluidCircuitNode::Clear()
{
  SECircuitNode::Clear();
}
//-----------------------------------------------------------------------------
bool SEFluidCircuitNode::Load(const CDM::FluidCircuitNodeData& in)
{
  SECircuitNode::Load(in);
  if (in.Pressure().present())
    GetPressure().Load(in.Pressure().get());
  if (in.NextPressure().present())
    GetNextPressure().Load(in.NextPressure().get());
  if (in.Volume().present())
    GetVolume().Load(in.Volume().get());
  if (in.NextVolume().present())
    GetNextVolume().Load(in.NextVolume().get());
  if (in.VolumeBaseline().present())
    GetVolumeBaseline().Load(in.VolumeBaseline().get());
  return true;
}
//-----------------------------------------------------------------------------
CDM::FluidCircuitNodeData* SEFluidCircuitNode::Unload() const
{
  CDM::FluidCircuitNodeData* data = new CDM::FluidCircuitNodeData();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEFluidCircuitNode::Unload(CDM::FluidCircuitNodeData& data) const
{
  SECircuitNode::Unload(data);
  if (HasPressure())
    data.Pressure(std::unique_ptr<CDM::ScalarPressureData>(m_Potential->Unload()));
  if (HasNextPressure())
    data.NextPressure(std::unique_ptr<CDM::ScalarPressureData>(m_NextPotential->Unload()));
  if (HasVolume())
    data.Volume(std::unique_ptr<CDM::ScalarVolumeData>(m_Quantity->Unload()));
  if (HasNextVolume())
    data.NextVolume(std::unique_ptr<CDM::ScalarVolumeData>(m_NextQuantity->Unload()));
  if (HasVolumeBaseline())
    data.VolumeBaseline(std::unique_ptr<CDM::ScalarVolumeData>(m_QuantityBaseline->Unload()));
}
//-----------------------------------------------------------------------------
bool SEFluidCircuitNode::HasPressure() const
{
  return HasPotential();
}
//-----------------------------------------------------------------------------
SEScalarPressure& SEFluidCircuitNode::GetPressure()
{
  return GetPotential();
}
//-----------------------------------------------------------------------------
double SEFluidCircuitNode::GetPressure(const PressureUnit& unit) const
{
  if (m_Potential == nullptr)
    return SEScalar::dNaN();
  return m_Potential->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEFluidCircuitNode::HasNextPressure() const
{
  return HasNextPotential();
}
//-----------------------------------------------------------------------------
SEScalarPressure& SEFluidCircuitNode::GetNextPressure()
{
  return GetNextPotential();
}
//-----------------------------------------------------------------------------
double SEFluidCircuitNode::GetNextPressure(const PressureUnit& unit) const
{
  if (m_NextPotential == nullptr)
    return SEScalar::dNaN();
  return m_NextPotential->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEFluidCircuitNode::HasVolume() const
{
  return HasQuantity();
}
//-----------------------------------------------------------------------------
SEScalarVolume& SEFluidCircuitNode::GetVolume()
{
  return GetQuantity();
}
//-----------------------------------------------------------------------------
double SEFluidCircuitNode::GetVolume(const VolumeUnit& unit) const
{
  if (m_Quantity == nullptr)
    return SEScalar::dNaN();
  return m_Quantity->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEFluidCircuitNode::HasNextVolume() const
{
  return HasNextQuantity();
}
//-----------------------------------------------------------------------------
SEScalarVolume& SEFluidCircuitNode::GetNextVolume()
{
  return GetNextQuantity();
}
//-----------------------------------------------------------------------------
double SEFluidCircuitNode::GetNextVolume(const VolumeUnit& unit) const
{
  if (m_NextQuantity == nullptr)
    return SEScalar::dNaN();
  return m_NextQuantity->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEFluidCircuitNode::HasVolumeBaseline() const
{
  return HasQuantityBaseline();
}
//-----------------------------------------------------------------------------
SEScalarVolume& SEFluidCircuitNode::GetVolumeBaseline()
{
  return GetQuantityBaseline();
}
//-----------------------------------------------------------------------------
double SEFluidCircuitNode::GetVolumeBaseline(const VolumeUnit& unit) const
{
  if (m_QuantityBaseline == nullptr)
    return SEScalar::dNaN();
  return m_QuantityBaseline->GetValue(unit);
}
//-----------------------------------------------------------------------------
}