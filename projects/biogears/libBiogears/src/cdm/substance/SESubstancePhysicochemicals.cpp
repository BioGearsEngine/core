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

#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/substance/SESubstancePhysicochemicals.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SESubstancePhysicochemicals::SESubstancePhysicochemicals(Logger* logger)
  : Loggable(logger)
{
  m_AcidDissociationConstant = nullptr;
  m_BindingProtein = (CDM::enumSubstanceBindingProtein::value)-1;
  m_BloodPlasmaRatio = nullptr;
  m_FractionUnboundInPlasma = nullptr;
  m_IonicState = (CDM::enumSubstanceIonicState::value)-1;
  m_LogP = nullptr;
  m_OralAbsorptionRateConstant = nullptr;
}
//-----------------------------------------------------------------------------
SESubstancePhysicochemicals::~SESubstancePhysicochemicals()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SESubstancePhysicochemicals::Clear()
{
  SAFE_DELETE(m_AcidDissociationConstant);
  m_BindingProtein = (CDM::enumSubstanceBindingProtein::value)-1;
  SAFE_DELETE(m_BloodPlasmaRatio);
  SAFE_DELETE(m_FractionUnboundInPlasma);
  m_IonicState = (CDM::enumSubstanceIonicState::value)-1;
  SAFE_DELETE(m_LogP);
  SAFE_DELETE(m_OralAbsorptionRateConstant);
}
//-----------------------------------------------------------------------------
bool SESubstancePhysicochemicals::IsValid() const
{
  if (!HasAcidDissociationConstant())
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
const SEScalar* SESubstancePhysicochemicals::GetScalar(const char* name)
{
  return GetScalar(std::string{ name });
}
//-----------------------------------------------------------------------------
const SEScalar* SESubstancePhysicochemicals::GetScalar(const std::string& name)
{
  if (name.compare("AcidDissociationConstant") == 0)
    return &GetAcidDissociationConstant();
  if (name.compare("BloodPlasmaRatio") == 0)
    return &GetBloodPlasmaRatio();
  if (name.compare("FractionUnboundInPlasma") == 0)
    return &GetFractionUnboundInPlasma();
  if (name.compare("LogP") == 0)
    return &GetLogP();
  if (name.compare("OralAbsorptionRateConstant") == 0)
    return &GetOralAbsorptionRateConstant();

  return nullptr;
}
//-----------------------------------------------------------------------------
bool SESubstancePhysicochemicals::Load(const CDM::SubstancePhysicochemicalData& in)
{
  Clear();

  GetAcidDissociationConstant().Load(in.AcidDissociationConstant());
  m_BindingProtein = in.BindingProtein();
  GetBloodPlasmaRatio().Load(in.BloodPlasmaRatio());
  GetFractionUnboundInPlasma().Load(in.FractionUnboundInPlasma());
  SetIonicState(in.IonicState());
  GetLogP().Load(in.LogP());
  if (in.OralAbsorptionRateConstant().present())
    GetOralAbsorptionRateConstant().Load(in.OralAbsorptionRateConstant().get());

  return true;
}
//-----------------------------------------------------------------------------
CDM::SubstancePhysicochemicalData* SESubstancePhysicochemicals::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::SubstancePhysicochemicalData* data = new CDM::SubstancePhysicochemicalData();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SESubstancePhysicochemicals::Unload(CDM::SubstancePhysicochemicalData& data) const
{
  if (HasAcidDissociationConstant())
    data.AcidDissociationConstant(std::unique_ptr<CDM::ScalarData>(m_AcidDissociationConstant->Unload()));
  if (HasBindingProtein())
    data.BindingProtein(m_BindingProtein);
  if (HasBloodPlasmaRatio())
    data.BloodPlasmaRatio(std::unique_ptr<CDM::ScalarData>(m_BloodPlasmaRatio->Unload()));
  if (HasFractionUnboundInPlasma())
    data.FractionUnboundInPlasma(std::unique_ptr<CDM::ScalarFractionData>(m_FractionUnboundInPlasma->Unload()));
  if (HasIonicState())
    data.IonicState(m_IonicState);
  if (HasLogP())
    data.LogP(std::unique_ptr<CDM::ScalarData>(m_LogP->Unload()));
  if (HasOralAbsorptionRateConstant())
    data.OralAbsorptionRateConstant(std::unique_ptr<CDM::ScalarData>(m_OralAbsorptionRateConstant->Unload()));
};
//-----------------------------------------------------------------------------
bool SESubstancePhysicochemicals::HasAcidDissociationConstant() const
{
  return (m_AcidDissociationConstant == nullptr) ? false : m_AcidDissociationConstant->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar& SESubstancePhysicochemicals::GetAcidDissociationConstant()
{
  if (m_AcidDissociationConstant == nullptr)
    m_AcidDissociationConstant = new SEScalar();
  return *m_AcidDissociationConstant;
}
//-----------------------------------------------------------------------------
double SESubstancePhysicochemicals::GetAcidDissociationConstant() const
{
  if (m_AcidDissociationConstant == nullptr)
    return SEScalar::dNaN();
  return m_AcidDissociationConstant->GetValue();
}
//-----------------------------------------------------------------------------
CDM::enumSubstanceBindingProtein::value SESubstancePhysicochemicals::GetBindingProtein() const
{
  return m_BindingProtein;
}
//-----------------------------------------------------------------------------
void SESubstancePhysicochemicals::SetBindingProtein(CDM::enumSubstanceBindingProtein::value protein)
{
  m_BindingProtein = protein;
}
//-----------------------------------------------------------------------------
bool SESubstancePhysicochemicals::HasBindingProtein() const
{
  return m_BindingProtein == ((CDM::enumSubstanceBindingProtein::value)-1) ? false : true;
}
//-----------------------------------------------------------------------------
void SESubstancePhysicochemicals::InvalidateBindingProtein()
{
  m_BindingProtein = (CDM::enumSubstanceBindingProtein::value)-1;
}
//-----------------------------------------------------------------------------
bool SESubstancePhysicochemicals::HasBloodPlasmaRatio() const
{
  return (m_BloodPlasmaRatio == nullptr) ? false : m_BloodPlasmaRatio->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar& SESubstancePhysicochemicals::GetBloodPlasmaRatio()
{
  if (m_BloodPlasmaRatio == nullptr)
    m_BloodPlasmaRatio = new SEScalar();
  return *m_BloodPlasmaRatio;
}
//-----------------------------------------------------------------------------
double SESubstancePhysicochemicals::GetBloodPlasmaRatio() const
{
  if (m_BloodPlasmaRatio == nullptr)
    return SEScalar::dNaN();
  return m_BloodPlasmaRatio->GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstancePhysicochemicals::HasFractionUnboundInPlasma() const
{
  return (m_FractionUnboundInPlasma == nullptr) ? false : m_FractionUnboundInPlasma->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFraction& SESubstancePhysicochemicals::GetFractionUnboundInPlasma()
{
  if (m_FractionUnboundInPlasma == nullptr)
    m_FractionUnboundInPlasma = new SEScalarFraction();
  return *m_FractionUnboundInPlasma;
}
//-----------------------------------------------------------------------------
double SESubstancePhysicochemicals::GetFractionUnboundInPlasma() const
{
  if (m_FractionUnboundInPlasma == nullptr)
    return SEScalar::dNaN();
  return m_FractionUnboundInPlasma->GetValue();
}
//-----------------------------------------------------------------------------
CDM::enumSubstanceIonicState::value SESubstancePhysicochemicals::GetIonicState() const
{
  return m_IonicState;
}
//-----------------------------------------------------------------------------
void SESubstancePhysicochemicals::SetIonicState(CDM::enumSubstanceIonicState::value state)
{
  m_IonicState = state;
}
bool SESubstancePhysicochemicals::HasIonicState() const
{
  return m_IonicState == ((CDM::enumSubstanceIonicState::value)-1) ? false : true;
}
//-----------------------------------------------------------------------------
void SESubstancePhysicochemicals::InvalidateIonicState()
{
  m_IonicState = (CDM::enumSubstanceIonicState::value)-1;
}
//-----------------------------------------------------------------------------
bool SESubstancePhysicochemicals::HasLogP() const
{
  return (m_LogP == nullptr) ? false : m_LogP->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar& SESubstancePhysicochemicals::GetLogP()
{
  if (m_LogP == nullptr)
    m_LogP = new SEScalar();
  return *m_LogP;
}
//-----------------------------------------------------------------------------
double SESubstancePhysicochemicals::GetLogP() const
{
  if (m_LogP == nullptr)
    return SEScalar::dNaN();
  return m_LogP->GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstancePhysicochemicals::HasOralAbsorptionRateConstant() const
{
  return (m_OralAbsorptionRateConstant == nullptr) ? false : m_OralAbsorptionRateConstant->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar& SESubstancePhysicochemicals::GetOralAbsorptionRateConstant()
//-----------------------------------------------------------------------------
{
  if (m_OralAbsorptionRateConstant == nullptr)
    m_OralAbsorptionRateConstant = new SEScalar();
  return *m_OralAbsorptionRateConstant;
}
double SESubstancePhysicochemicals::GetOralAbsorptionRateConstant() const
{
  if (m_OralAbsorptionRateConstant == nullptr)
    return SEScalar::dNaN();
  return m_OralAbsorptionRateConstant->GetValue();
}
//-----------------------------------------------------------------------------
}