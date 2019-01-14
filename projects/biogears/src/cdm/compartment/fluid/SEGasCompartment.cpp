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
#include <biogears/cdm/compartment/fluid/SEGasCompartment.h>

#include <biogears/cdm/compartment/SECompartmentGraph.inl>
#include <biogears/cdm/compartment/fluid/SEFluidCompartment.inl>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/substance/SESubstanceManager.h>

namespace biogears {
SEGasCompartment::SEGasCompartment(const char* name, Logger* logger)
  : SEGasCompartment(std::string{ name }, logger)
{
}
//-------------------------------------------------------------------------------
SEGasCompartment::SEGasCompartment(const std::string& name, Logger* logger)
  : SEFluidCompartment(name, logger)
{
}
//-------------------------------------------------------------------------------
SEGasCompartment::~SEGasCompartment()
{
}
//-------------------------------------------------------------------------------
bool SEGasCompartment::Load(const CDM::GasCompartmentData& in, SESubstanceManager& subMgr, SECircuitManager* circuits)
{
  if (!SEFluidCompartment::Load(in, circuits))
    return false;
  if (in.Child().empty()) {
    for (const CDM::GasSubstanceQuantityData& d : in.SubstanceQuantity()) {
      SESubstance* sub = subMgr.GetSubstance(d.Substance());
      if (sub == nullptr) {
        Error("Could not find a substance for " + std::string{ d.Substance() });
        return false;
      }
      CreateSubstanceQuantity(*sub).Load(d);
      ;
    }
  }
  return true;
}
//-------------------------------------------------------------------------------
CDM::GasCompartmentData* SEGasCompartment::Unload()
{
  CDM::GasCompartmentData* data = new CDM::GasCompartmentData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SEGasCompartment::Unload(CDM::GasCompartmentData& data)
{
  SEFluidCompartment::Unload(data);
  for (SEGasSubstanceQuantity* subQ : m_SubstanceQuantities)
    data.SubstanceQuantity().push_back(std::unique_ptr<CDM::GasSubstanceQuantityData>(subQ->Unload()));
}
//-------------------------------------------------------------------------------
void SEGasCompartment::StateChange()
{
  m_Leaves.clear();
  FindLeaves<SEGasCompartment>(*this, m_Leaves);
  m_Nodes.StateChange();
}
//-------------------------------------------------------------------------------
void SEGasCompartment::Balance(BalanceGasBy by)
{
  if (!m_FluidChildren.empty())
    Fatal("You cannot balance a quantity with children", "SEGasCompartment::Balance");
  switch (by) {
  case BalanceGasBy::Volume: {
    //Note: We won't modify the compartment volume, just the fractions
    double totalVolume_mL = 0;
    for (SEGasSubstanceQuantity* subQ : GetSubstanceQuantities()) {
      if (subQ->HasVolume())
        totalVolume_mL += subQ->GetVolume(VolumeUnit::mL);
    }
    for (SEGasSubstanceQuantity* subQ : GetSubstanceQuantities()) {
      if (!subQ->HasVolume()) {
        subQ->Invalidate();
        continue;
      } else {
        subQ->GetVolumeFraction().SetValue(subQ->GetVolume(VolumeUnit::mL) / totalVolume_mL);
        subQ->GetVolume().SetValue(subQ->GetVolumeFraction().GetValue() * GetVolume(VolumeUnit::mL), VolumeUnit::mL);
        if (HasPressure())
          GeneralMath::CalculatePartialPressureInGas(subQ->GetVolumeFraction(), GetPressure(), subQ->GetPartialPressure(), m_Logger);
      }
    }
    break;
  }
  case BalanceGasBy::VolumeFraction: {
    if (!HasVolume()) {
      for (SEGasSubstanceQuantity* subQ : GetSubstanceQuantities()) {
        subQ->Invalidate();
        if (HasPressure())
          GeneralMath::CalculatePartialPressureInGas(subQ->GetVolumeFraction(), GetPressure(), subQ->GetPartialPressure(), m_Logger);
      }
      return;
    }
    if (GetVolume().IsInfinity()) {
      for (SEGasSubstanceQuantity* subQ : GetSubstanceQuantities()) {
        subQ->GetVolume().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::mL);
        if (HasPressure())
          GeneralMath::CalculatePartialPressureInGas(subQ->GetVolumeFraction(), GetPressure(), subQ->GetPartialPressure(), m_Logger);
      }
    } else {
      double totalFraction = 0;
      double totalVolume_mL = GetVolume(VolumeUnit::mL);
      for (SEGasSubstanceQuantity* subQ : GetSubstanceQuantities()) {
        if (!subQ->HasVolumeFraction())
          subQ->Invalidate();
        else {
          totalFraction += subQ->GetVolumeFraction().GetValue();
          subQ->GetVolume().SetValue(subQ->GetVolumeFraction().GetValue() * totalVolume_mL, VolumeUnit::mL);
        }
        if (HasPressure())
          GeneralMath::CalculatePartialPressureInGas(subQ->GetVolumeFraction(), GetPressure(), subQ->GetPartialPressure(), m_Logger);
      }
      if (!SEScalar::IsZero(1 - totalFraction, ZERO_APPROX))
        Fatal(std::string{ GetName() } +" Compartment's volume fractions do not sum up to 1");
    }
    break;
  }
  }
}
//-------------------------------------------------------------------------------
void SEGasCompartment::AddChild(SEGasCompartment& child)
{
  if (HasNodeMapping()) {
    Fatal("You cannont add a child compartment to a compartment mapped to nodes");
    return;
  }
  if (HasChild(child.GetName()))
    return;
  m_FluidChildren.push_back(&child);
  m_Children.push_back(&child);
  for (SEGasSubstanceQuantity* subQ : m_SubstanceQuantities)
    subQ->AddChild(child.CreateSubstanceQuantity(subQ->GetSubstance()));
}
//-------------------------------------------------------------------------------
SEGasSubstanceQuantity& SEGasCompartment::CreateSubstanceQuantity(SESubstance& substance)
{
  SEGasSubstanceQuantity* subQ = GetSubstanceQuantity(substance);
  if (subQ == nullptr) {
    subQ = new SEGasSubstanceQuantity(substance, *this);
    subQ->SetToZero();
    m_SubstanceQuantities.push_back(subQ);
    m_TransportSubstances.push_back(subQ);
  }
  if (!m_FluidChildren.empty()) {
    for (SEGasCompartment* child : m_Children)
      subQ->AddChild(child->CreateSubstanceQuantity(substance));
  }
  return *subQ;
}
//-------------------------------------------------------------------------------
}