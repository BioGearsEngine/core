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
#include <biogears/cdm/compartment/thermal/SEThermalCompartmentLink.h>

#include <biogears/cdm/compartment/SECompartmentGraph.inl>
#include "io/cdm/Compartment.h"
#include <biogears/cdm/circuit/SECircuitManager.h>
#include <biogears/cdm/properties/SEScalarPower.h>

namespace biogears {
SEThermalCompartmentLink::SEThermalCompartmentLink(SEThermalCompartment& src, SEThermalCompartment& tgt, const char* name)
  : SEThermalCompartmentLink(src, tgt, std::string { name })
{
}
//-------------------------------------------------------------------------------
SEThermalCompartmentLink::SEThermalCompartmentLink(SEThermalCompartment& src, SEThermalCompartment& tgt, const std::string& name)
  : SECompartmentLink(name, src.GetLogger())
  , m_SourceCmpt(src)
  , m_TargetCmpt(tgt)
{
  m_HeatTransferRate = nullptr;
  m_Path = nullptr;
}
//-------------------------------------------------------------------------------
SEThermalCompartmentLink::~SEThermalCompartmentLink()
{
}

//-------------------------------------------------------------------------------
void SEThermalCompartmentLink::Clear()
{
  m_Path = nullptr;
  SAFE_DELETE(m_HeatTransferRate);
}
//-------------------------------------------------------------------------------
const SEScalar* SEThermalCompartmentLink::GetScalar(const char* name)
{
  return GetScalar(std::string { name });
}
//-------------------------------------------------------------------------------
const SEScalar* SEThermalCompartmentLink::GetScalar(const std::string& name)
{
  if (name.compare("HeatTransferRate") == 0)
    return &GetHeatTransferRate();
  return nullptr;
}
//-------------------------------------------------------------------------------
bool SEThermalCompartmentLink::HasHeatTransferRate() const
{
  if (m_Path != nullptr)
    return m_Path->HasNextHeatTransferRate();
  return m_HeatTransferRate == nullptr ? false : m_HeatTransferRate->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPower& SEThermalCompartmentLink::GetHeatTransferRate()
{
  if (m_Path != nullptr)
    return m_Path->GetNextHeatTransferRate();
  if (m_HeatTransferRate == nullptr)
    m_HeatTransferRate = new SEScalarPower();
  return *m_HeatTransferRate;
}
//-------------------------------------------------------------------------------
double SEThermalCompartmentLink::GetHeatTransferRate(const PowerUnit& unit) const
{
  if (m_Path != nullptr)
    return m_Path->GetNextHeatTransferRate(unit);
  if (m_HeatTransferRate == nullptr)
    return SEScalar::dNaN();
  return m_HeatTransferRate->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEThermalCompartmentLink::operator==(const SEThermalCompartmentLink& rhs) const
{
  bool equivilant = ((m_HeatTransferRate && rhs.m_HeatTransferRate)
                       ? m_HeatTransferRate->operator==(*rhs.m_HeatTransferRate)
                       : m_HeatTransferRate == rhs.m_HeatTransferRate)
    && m_SourceCmpt == m_SourceCmpt
    && m_TargetCmpt == m_TargetCmpt
    && ((m_Path && rhs.m_Path) ? m_Path->operator==(*rhs.m_Path) : m_Path == rhs.m_Path);

  return equivilant;
}
//-------------------------------------------------------------------------------

bool SEThermalCompartmentLink::operator!=(const SEThermalCompartmentLink& rhs) const
{
  return !this->operator==(rhs);
}
//-------------------------------------------------------------------------------

bool SEThermalCompartmentLink::operator==(const SECompartmentLink& rhs) const
{
  auto rhs_ptr = dynamic_cast<decltype(this)>(&rhs);
  return (rhs_ptr) ? this->operator==(*rhs_ptr) : false;
}
//-------------------------------------------------------------------------------

bool SEThermalCompartmentLink::operator!=(const SECompartmentLink& rhs) const
{
  return !this->operator==(rhs);
}

} // namespace biogears

namespace biogears {
SEThermalCompartmentGraph::SEThermalCompartmentGraph(const char* name, Logger* logger)
  : SECompartmentGraph(std::string { name }, logger)
{
}
//-----------------------------------------------------------------------------
SEThermalCompartmentGraph::SEThermalCompartmentGraph(const std::string& name, Logger* logger)
  : SECompartmentGraph(name, logger) {};
//-----------------------------------------------------------------------------
SEThermalCompartmentGraph::~SEThermalCompartmentGraph()
{
}

} // namespace biogears