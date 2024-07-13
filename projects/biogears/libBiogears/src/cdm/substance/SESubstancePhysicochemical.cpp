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
#include <biogears/cdm/substance/SESubstancePhysicochemical.h>

#include "io/cdm/Substance.h"

#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SESubstancePhysicochemical::SESubstancePhysicochemical(Logger* logger)
  : Loggable(logger)
{
  m_AcidDissociationConstants.clear();
  m_BindingProtein = (SESubstanceBindingProtein)-1;
  m_BloodPlasmaRatio = nullptr;
  m_FractionUnboundInPlasma = nullptr;
  m_IonicState = (SESubstanceIonicState)-1;
  m_LogP = nullptr;
  m_HydrogenBondCount = nullptr;
  m_PolarSurfaceArea = nullptr;
}
//-----------------------------------------------------------------------------
SESubstancePhysicochemical::~SESubstancePhysicochemical()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SESubstancePhysicochemical::Clear()
{
  m_AcidDissociationConstants.clear();
  m_BindingProtein = (SESubstanceBindingProtein)-1;
  SAFE_DELETE(m_BloodPlasmaRatio);
  SAFE_DELETE(m_FractionUnboundInPlasma);
  m_IonicState = (SESubstanceIonicState)-1;
  SAFE_DELETE(m_LogP);
  SAFE_DELETE(m_HydrogenBondCount);
  SAFE_DELETE(m_PolarSurfaceArea);
}
//-----------------------------------------------------------------------------
bool SESubstancePhysicochemical::IsValid() const
{
  if (!HasPrimaryPKA())
    return false;
  if (!HasBindingProtein())
    return false;
  if (!HasBloodPlasmaRatio())
    return false;
  if (!HasFractionUnboundInPlasma())
    return false;
  if (!HasIonicState())
    return false;
  if (!HasLogP())
    return false;
  return true;
}
//-----------------------------------------------------------------------------
const SEScalar* SESubstancePhysicochemical::GetScalar(const char* name)
{
  return GetScalar(std::string{ name });
}
//-----------------------------------------------------------------------------
const SEScalar* SESubstancePhysicochemical::GetScalar(const std::string& name)
{
  if (name.compare("PrimaryPKA") == 0)
    return &GetPrimaryPKA();
  if (name.compare("SecondaryPKA") == 0)
    return &GetSecondaryPKA();
  if (name.compare("BloodPlasmaRatio") == 0)
    return &GetBloodPlasmaRatio();
  if (name.compare("FractionUnboundInPlasma") == 0)
    return &GetFractionUnboundInPlasma();
  if (name.compare("LogP") == 0)
    return &GetLogP();
  if (name.compare("HydrogenBondCount") == 0)
    return &GetHydrogenBondCount();
  if (name.compare("PolarSurfaceArea") == 0)
    return &GetPolarSurfaceArea();

  return nullptr;
}
//-----------------------------------------------------------------------------
bool SESubstancePhysicochemical::Load(const CDM::SubstancePhysicochemicalData& in)
{
  io::Substance::UnMarshall(in, *this);
  return true;
}
//-----------------------------------------------------------------------------
CDM::SubstancePhysicochemicalData* SESubstancePhysicochemical::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::SubstancePhysicochemicalData* data = new CDM::SubstancePhysicochemicalData();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SESubstancePhysicochemical::Unload(CDM::SubstancePhysicochemicalData& data) const
{
  io::Substance::Marshall(*this, data);
};
//-----------------------------------------------------------------------------
bool SESubstancePhysicochemical::HasPrimaryPKA() const
{
  return (!m_AcidDissociationConstants.empty());
}
SEScalar& SESubstancePhysicochemical::GetPrimaryPKA()
{
  if (m_AcidDissociationConstants.empty()) {
    SEScalar* pKA1 = new SEScalar();
    m_AcidDissociationConstants.push_back(pKA1);
  }
  return *m_AcidDissociationConstants[0];
}
double SESubstancePhysicochemical::GetPrimaryPKA() const
{
  if (m_AcidDissociationConstants.empty())
    return SEScalar::dNaN();
  return m_AcidDissociationConstants[0]->GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstancePhysicochemical::HasSecondaryPKA() const
{
  return m_AcidDissociationConstants.size() < 2 ? false : true;
}
SEScalar& SESubstancePhysicochemical::GetSecondaryPKA()
{
  if (m_AcidDissociationConstants.size() < 2) {
    SEScalar* pKA2 = new SEScalar();
    m_AcidDissociationConstants.push_back(pKA2);
  }
  return *m_AcidDissociationConstants[1];
}
double SESubstancePhysicochemical::GetSecondaryPKA() const
{
  if (m_AcidDissociationConstants.size()<2)
    return SEScalar::dNaN();
  return m_AcidDissociationConstants[1]->GetValue();
}
////-----------------------------------------------------------------------------
SESubstanceBindingProtein SESubstancePhysicochemical::GetBindingProtein() const
{
  return m_BindingProtein;
}
void SESubstancePhysicochemical::SetBindingProtein(SESubstanceBindingProtein protein)
{
  m_BindingProtein = protein;
}
bool SESubstancePhysicochemical::HasBindingProtein() const
{
  return m_BindingProtein == SESubstanceBindingProtein::Invalid ? false : true;
}
void SESubstancePhysicochemical::InvalidateBindingProtein()
{
  m_BindingProtein = (SESubstanceBindingProtein)-1;
}
//-----------------------------------------------------------------------------
bool SESubstancePhysicochemical::HasBloodPlasmaRatio() const
{
  return (m_BloodPlasmaRatio == nullptr) ? false : m_BloodPlasmaRatio->IsValid();
}
SEScalar& SESubstancePhysicochemical::GetBloodPlasmaRatio()
{
  if (m_BloodPlasmaRatio == nullptr)
    m_BloodPlasmaRatio = new SEScalar();
  return *m_BloodPlasmaRatio;
}
double SESubstancePhysicochemical::GetBloodPlasmaRatio() const
{
  if (m_BloodPlasmaRatio == nullptr)
    return SEScalar::dNaN();
  return m_BloodPlasmaRatio->GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstancePhysicochemical::HasFractionUnboundInPlasma() const
{
  return (m_FractionUnboundInPlasma == nullptr) ? false : m_FractionUnboundInPlasma->IsValid();
}
SEScalarFraction& SESubstancePhysicochemical::GetFractionUnboundInPlasma()
{
  if (m_FractionUnboundInPlasma == nullptr)
    m_FractionUnboundInPlasma = new SEScalarFraction();
  return *m_FractionUnboundInPlasma;
}
double SESubstancePhysicochemical::GetFractionUnboundInPlasma() const
{
  if (m_FractionUnboundInPlasma == nullptr)
    return SEScalar::dNaN();
  return m_FractionUnboundInPlasma->GetValue();
}
//-----------------------------------------------------------------------------
SESubstanceIonicState SESubstancePhysicochemical::GetIonicState() const
{
  return m_IonicState;
}
void SESubstancePhysicochemical::SetIonicState(SESubstanceIonicState state)
{
  m_IonicState = state;
}
bool SESubstancePhysicochemical::HasIonicState() const
{
  return m_IonicState == SESubstanceIonicState::Invalid ? false : true;
}
void SESubstancePhysicochemical::InvalidateIonicState()
{
  m_IonicState = (SESubstanceIonicState)-1;
}
//-----------------------------------------------------------------------------
bool SESubstancePhysicochemical::HasLogP() const
{
  return (m_LogP == nullptr) ? false : m_LogP->IsValid();
}
SEScalar& SESubstancePhysicochemical::GetLogP()
{
  if (m_LogP == nullptr)
    m_LogP = new SEScalar();
  return *m_LogP;
}
double SESubstancePhysicochemical::GetLogP() const
{
  if (m_LogP == nullptr)
    return SEScalar::dNaN();
  return m_LogP->GetValue();
}

//-----------------------------------------------------------------------------
bool SESubstancePhysicochemical::HasHydrogenBondCount() const
{
  return (m_HydrogenBondCount == nullptr) ? false : m_HydrogenBondCount->IsValid();
}
SEScalar& SESubstancePhysicochemical::GetHydrogenBondCount()
{
  if (m_HydrogenBondCount == nullptr)
    m_HydrogenBondCount = new SEScalar();
  return *m_HydrogenBondCount;
}
double SESubstancePhysicochemical::GetHydrogenBondCount() const
{
  if (m_HydrogenBondCount == nullptr)
    return SEScalar::dNaN();
  return m_HydrogenBondCount->GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstancePhysicochemical::HasPolarSurfaceArea() const
{
  return (m_PolarSurfaceArea == nullptr) ? false : m_PolarSurfaceArea->IsValid();
}
SEScalar& SESubstancePhysicochemical::GetPolarSurfaceArea()
{
  if (m_PolarSurfaceArea == nullptr)
    m_PolarSurfaceArea = new SEScalar();
  return *m_PolarSurfaceArea;
}
double SESubstancePhysicochemical::GetPolarSurfaceArea() const
{
  if (m_PolarSurfaceArea == nullptr)
    return SEScalar::dNaN();
  return m_PolarSurfaceArea->GetValue();
}

//-------------------------------------------------------------------------------
bool SESubstancePhysicochemical::operator==( const SESubstancePhysicochemical& rhs) const
{

bool equivilant = m_BindingProtein == rhs.m_BindingProtein;
  equivilant &= m_IonicState == rhs.m_IonicState;
  equivilant &= (m_BloodPlasmaRatio && rhs.m_BloodPlasmaRatio) ? m_BloodPlasmaRatio->operator==(*rhs.m_BloodPlasmaRatio) : m_BloodPlasmaRatio == rhs.m_BloodPlasmaRatio;
  equivilant &= (m_FractionUnboundInPlasma && rhs.m_FractionUnboundInPlasma) ? m_FractionUnboundInPlasma->operator==(*rhs.m_FractionUnboundInPlasma) : m_FractionUnboundInPlasma == rhs.m_FractionUnboundInPlasma;
  equivilant &= (m_LogP && rhs.m_LogP) ? m_LogP->operator==(*rhs.m_LogP) : m_LogP == rhs.m_LogP;
  equivilant &= (m_HydrogenBondCount && rhs.m_HydrogenBondCount) ? m_HydrogenBondCount->operator==(*rhs.m_HydrogenBondCount) : m_HydrogenBondCount == rhs.m_HydrogenBondCount;
  equivilant &= (m_PolarSurfaceArea && rhs.m_PolarSurfaceArea) ? m_PolarSurfaceArea->operator==(*rhs.m_PolarSurfaceArea) : m_PolarSurfaceArea == rhs.m_PolarSurfaceArea;
  equivilant &= m_AcidDissociationConstants.size() == rhs.m_AcidDissociationConstants.size();
  if(equivilant){
    for ( auto i = 0; i< m_AcidDissociationConstants.size(); ++i){
      equivilant &= (m_AcidDissociationConstants[i] && rhs.m_AcidDissociationConstants[i]) 
        ? m_AcidDissociationConstants[i]->operator==(*rhs.m_AcidDissociationConstants[i]) 
        : m_AcidDissociationConstants[i] == rhs.m_AcidDissociationConstants[i];    
    }
  }
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SESubstancePhysicochemical::operator!=( const SESubstancePhysicochemical& rhs) const
{
  return !(*this == rhs);
}
}