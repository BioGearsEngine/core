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

#include <biogears/cdm/circuit/SECircuitManager.h>
#include <biogears/cdm/properties/SEScalarPower.h>

namespace biogears {
SEThermalCompartmentLink::SEThermalCompartmentLink(SEThermalCompartment& src, SEThermalCompartment& tgt, const char* name)
  : SEThermalCompartmentLink(src, tgt, std::string{ name })
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
bool SEThermalCompartmentLink::Load(const CDM::ThermalCompartmentLinkData& in, SECircuitManager* circuits)
{
  if (!SECompartmentLink::Load(in, circuits))
    return false;
  if (in.Path().present()) {
    if (circuits == nullptr) {
      Error("Link is mapped to circuit path, " + std::string{ in.Path().get() } +", but no circuit manager was provided, cannot load");
      return false;
    }
    SEThermalCircuitPath* path = circuits->GetThermalPath(in.Path().get());
    if (path == nullptr) {
      Error("Link is mapped to circuit path, " + std::string{ in.Path().get() }+", but provided circuit manager did not have that path");
      return false;
    }
    MapPath(*path);
  } else {
    if (in.HeatTransferRate().present())
      const_cast<SEScalarPower&>(GetHeatTransferRate()).Load(in.HeatTransferRate().get());
  }
  return true;
}
//-------------------------------------------------------------------------------
CDM::ThermalCompartmentLinkData* SEThermalCompartmentLink::Unload()
{
  CDM::ThermalCompartmentLinkData* data = new CDM::ThermalCompartmentLinkData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SEThermalCompartmentLink::Unload(CDM::ThermalCompartmentLinkData& data)
{
  SECompartmentLink::Unload(data);
  data.SourceCompartment(m_SourceCmpt.GetName());
  data.TargetCompartment(m_TargetCmpt.GetName());
  if (m_Path != nullptr)
    data.Path(m_Path->GetName());
  // Even if you have a path, I am unloading everything, this makes the xml actually usefull...
  if (HasHeatTransferRate())
    data.HeatTransferRate(std::unique_ptr<CDM::ScalarPowerData>(GetHeatTransferRate().Unload()));
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
  return GetScalar(std::string{ name });
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
}//namespace biogears

namespace biogears {
SEThermalCompartmentGraph::SEThermalCompartmentGraph(const char* name, Logger* logger)
  : SECompartmentGraph(std::string{ name }, logger)
{
}
//-----------------------------------------------------------------------------
SEThermalCompartmentGraph::SEThermalCompartmentGraph(const std::string& name, Logger* logger)
  : SECompartmentGraph(name, logger){};
//-----------------------------------------------------------------------------
SEThermalCompartmentGraph::~SEThermalCompartmentGraph()
{
}
//-----------------------------------------------------------------------------
} //namespace biogears