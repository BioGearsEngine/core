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
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>

#include <biogears/cdm/compartment/fluid/SEFluidCompartment.inl>
#include <biogears/cdm/compartment/substances/SELiquidSubstanceQuantity.h>
#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/substance/SESubstanceManager.h>

namespace biogears {

SELiquidCompartment::SELiquidCompartment(const char* name, Logger* logger)
  : SELiquidCompartment(std::string{ name }, logger)
{
}
//-----------------------------------------------------------------------------
SELiquidCompartment::SELiquidCompartment(const std::string& name, Logger* logger)
  : SEFluidCompartment(name, logger)
{
  m_pH = nullptr;
  m_WaterVolumeFraction = nullptr;
}
//-----------------------------------------------------------------------------
SELiquidCompartment::~SELiquidCompartment()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SELiquidCompartment::Clear()
{
  SEFluidCompartment::Clear();
  SAFE_DELETE(m_pH);
  SAFE_DELETE(m_WaterVolumeFraction);
  m_Children.clear();
}
//-----------------------------------------------------------------------------
bool SELiquidCompartment::Load(const CDM::LiquidCompartmentData& in, SESubstanceManager& subMgr, SECircuitManager* circuits)
{
  if (!SEFluidCompartment::Load(in, circuits))
    return false;
  if (in.Child().empty()) {
    for (const CDM::LiquidSubstanceQuantityData& d : in.SubstanceQuantity()) {
      SESubstance* sub = subMgr.GetSubstance(d.Substance());
      if (sub == nullptr) {
        Error("Could not find a substance for " + std::string{ d.Substance() });
        return false;
      }
      CreateSubstanceQuantity(*sub).Load(d);
      ;
    }
    if (in.pH().present())
      GetPH().Load(in.pH().get());
    if (in.WaterVolumeFraction().present())
      GetWaterVolumeFraction().Load(in.WaterVolumeFraction().get());
  }
  return true;
}
//-----------------------------------------------------------------------------
CDM::LiquidCompartmentData* SELiquidCompartment::Unload()
{
  CDM::LiquidCompartmentData* data = new CDM::LiquidCompartmentData();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SELiquidCompartment::Unload(CDM::LiquidCompartmentData& data)
{
  SEFluidCompartment::Unload(data);
  for (SELiquidSubstanceQuantity* subQ : m_SubstanceQuantities)
    data.SubstanceQuantity().push_back(std::unique_ptr<CDM::LiquidSubstanceQuantityData>(subQ->Unload()));
  if (HasPH())
    data.pH(std::unique_ptr<CDM::ScalarData>(GetPH().Unload()));
  if (HasWaterVolumeFraction())
    data.WaterVolumeFraction(std::unique_ptr<CDM::ScalarFractionData>(GetWaterVolumeFraction().Unload()));
}
//-----------------------------------------------------------------------------
const SEScalar* SELiquidCompartment::GetScalar(const char* name)
{
  return GetScalar(std::string{ name });
}
//-----------------------------------------------------------------------------
const SEScalar* SELiquidCompartment::GetScalar(const std::string& name)
{
  const SEScalar* s = SEFluidCompartment::GetScalar(name);
  if (s != nullptr)
    return s;
  if (name.compare("PH") == 0)
    return &GetPH();
  if (name.compare("WaterVolumeFraction") == 0)
    return &GetWaterVolumeFraction();
  return nullptr;
}
//-----------------------------------------------------------------------------
void SELiquidCompartment::StateChange()
{
  m_Leaves.clear();
  FindLeaves<SELiquidCompartment>(*this, m_Leaves);
  m_Nodes.StateChange();
}
//-----------------------------------------------------------------------------
void SELiquidCompartment::Balance(BalanceLiquidBy by)
{
  for (SELiquidSubstanceQuantity* subQ : m_SubstanceQuantities) {
    if (by == BalanceLiquidBy::PartialPressure && subQ->GetSubstance().GetState() != CDM::enumSubstanceState::Gas)
      continue;

    //Partial pressures only make sense for gases in liquids
    if (HasVolume())
      subQ->Balance(by);
  }
}
//-----------------------------------------------------------------------------
bool SELiquidCompartment::HasPH() const
{
  return m_pH == nullptr ? false : m_pH->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar& SELiquidCompartment::GetPH()
{
  if (m_pH == nullptr)
    m_pH = new SEScalar();
  if (!m_FluidChildren.empty()) {
    m_pH->SetReadOnly(false);
    m_pH->SetValue(const_cast<const SELiquidCompartment*>(this)->GetPH());
    m_pH->SetReadOnly(true);
  }
  return *m_pH;
}
//-----------------------------------------------------------------------------
double SELiquidCompartment::GetPH() const
{
  if (!m_Children.empty()) {
    double pH = 0;
    for (SELiquidCompartment* child : m_Children)
      pH += std::pow(10, -child->GetPH().GetValue()) * child->GetVolume(VolumeUnit::mL);
    pH = -log10(pH / GetVolume(VolumeUnit::mL));
    return pH;
  }
  if (m_pH == nullptr)
    return SEScalar::dNaN();
  return m_pH->GetValue();
}
//-----------------------------------------------------------------------------
bool SELiquidCompartment::HasWaterVolumeFraction() const
{
  return m_WaterVolumeFraction == nullptr ? false : m_WaterVolumeFraction->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFraction& SELiquidCompartment::GetWaterVolumeFraction()
{
  if (m_WaterVolumeFraction == nullptr)
    m_WaterVolumeFraction = new SEScalarFraction();
  if (!m_FluidChildren.empty()) {
    m_WaterVolumeFraction->SetReadOnly(false);
    m_WaterVolumeFraction->SetValue(const_cast<const SELiquidCompartment*>(this)->GetWaterVolumeFraction());
    m_WaterVolumeFraction->SetReadOnly(true);
  }
  return *m_WaterVolumeFraction;
}
//-----------------------------------------------------------------------------
double SELiquidCompartment::GetWaterVolumeFraction() const
{
  if (!m_Children.empty()) {
    double waterVolume_mL = 0;
    for (SELiquidCompartment* child : m_Children)
      waterVolume_mL += child->GetWaterVolumeFraction().GetValue() * child->GetVolume(VolumeUnit::mL);
    return waterVolume_mL / GetVolume(VolumeUnit::mL);
  }
  if (m_WaterVolumeFraction == nullptr)
    return SEScalar::dNaN();
  return m_WaterVolumeFraction->GetValue();
}
//-----------------------------------------------------------------------------
void SELiquidCompartment::AddChild(SELiquidCompartment& child)
{
  if (HasNodeMapping()) {
    Fatal("You cannont add a child compartment to a compartment mapped to nodes");
    return;
  }
  if (HasChild(child.GetName()))
    return;
  m_FluidChildren.push_back(&child);
  m_Children.push_back(&child);
  for (SELiquidSubstanceQuantity* subQ : m_SubstanceQuantities)
    subQ->AddChild(child.CreateSubstanceQuantity(subQ->GetSubstance()));
}
//-----------------------------------------------------------------------------
SELiquidSubstanceQuantity& SELiquidCompartment::CreateSubstanceQuantity(SESubstance& substance)
{
  SELiquidSubstanceQuantity* subQ = GetSubstanceQuantity(substance);
  if (subQ == nullptr) {
    subQ = new SELiquidSubstanceQuantity(substance, *this);
    subQ->SetToZero();
    m_SubstanceQuantities.push_back(subQ);
    m_TransportSubstances.push_back(subQ);
  }
  if (!m_FluidChildren.empty()) {
    for (SELiquidCompartment* child : m_Children)
      subQ->AddChild(child->CreateSubstanceQuantity(substance));
  }
  return *subQ;
}
//-----------------------------------------------------------------------------
}