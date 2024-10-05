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
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>

#include "io/cdm/Substance.h"

#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/schema/cdm/Properties.hxx>
#include <biogears/schema/cdm/Substance.hxx>

namespace std {
template class map<string, biogears::SEPharmacodynamicModifier*>;
}
namespace biogears {

SESubstancePharmacodynamics::SESubstancePharmacodynamics(SESubstancePharmacodynamics const& obj)
  : Loggable(obj.GetLogger())
  , m_AntibacterialEffect(new SEScalarFrequency(*obj.m_AntibacterialEffect))
  , m_Bronchodilation(new SEPharmacodynamicModifier(*obj.m_Bronchodilation))
  , m_DiastolicPressureModifier(new SEPharmacodynamicModifier(*obj.m_DiastolicPressureModifier))
  , m_EMaxShapeParameter(new SEScalar(*obj.m_EMaxShapeParameter))
  , m_FeverModifier(new SEPharmacodynamicModifier(*obj.m_FeverModifier))
  , m_HeartRateModifier(new SEPharmacodynamicModifier(*obj.m_HeartRateModifier))
  , m_HemorrhageModifier(new SEPharmacodynamicModifier(*obj.m_HemorrhageModifier))
  , m_NeuromuscularBlock(new SEPharmacodynamicModifier(*obj.m_NeuromuscularBlock))
  , m_PainModifier(new SEPharmacodynamicModifier(*obj.m_PainModifier))
  , m_PupilReactivityModifier(new SEPharmacodynamicModifier(*obj.m_PupilReactivityModifier))
  , m_PupilSizeModifier(new SEPharmacodynamicModifier(*obj.m_PupilSizeModifier))
  , m_RespirationRateModifier(new SEPharmacodynamicModifier(*obj.m_RespirationRateModifier))
  , m_Sedation(new SEPharmacodynamicModifier(*obj.m_Sedation))
  , m_SystolicPressureModifier(new SEPharmacodynamicModifier(*obj.m_SystolicPressureModifier))
  , m_TidalVolumeModifier(new SEPharmacodynamicModifier(*obj.m_TidalVolumeModifier))
  , m_TubularPermeabilityModifier(new SEPharmacodynamicModifier(*obj.m_TubularPermeabilityModifier))
  , m_CentralNervousModifier(new SEPharmacodynamicModifier(*obj.m_CentralNervousModifier))
  , m_EffectSiteRateConstant(new SEScalarFrequency(*obj.m_EffectSiteRateConstant))
{

}

SESubstancePharmacodynamics::SESubstancePharmacodynamics(Logger* logger)
  : Loggable(logger)
  , m_AntibacterialEffect(new SEScalarFrequency())
  , m_Bronchodilation(new SEPharmacodynamicModifier())
  , m_DiastolicPressureModifier(new SEPharmacodynamicModifier())
  , m_EMaxShapeParameter(new SEScalar())
  , m_FeverModifier(new SEPharmacodynamicModifier())
  , m_HeartRateModifier(new SEPharmacodynamicModifier())
  , m_HemorrhageModifier(new SEPharmacodynamicModifier())
  , m_NeuromuscularBlock(new SEPharmacodynamicModifier())
  , m_PainModifier(new SEPharmacodynamicModifier())
  , m_PupilReactivityModifier(new SEPharmacodynamicModifier())
  , m_PupilSizeModifier(new SEPharmacodynamicModifier())
  , m_RespirationRateModifier(new SEPharmacodynamicModifier())
  , m_Sedation(new SEPharmacodynamicModifier())
  , m_SystolicPressureModifier(new SEPharmacodynamicModifier())
  , m_TidalVolumeModifier(new SEPharmacodynamicModifier())
  , m_TubularPermeabilityModifier(new SEPharmacodynamicModifier())
  , m_CentralNervousModifier(new SEPharmacodynamicModifier())
  , m_EffectSiteRateConstant(new SEScalarFrequency()){
}
//-----------------------------------------------------------------------------
SESubstancePharmacodynamics::~SESubstancePharmacodynamics()
{
  Clear();
  SAFE_DELETE(m_AntibacterialEffect);
  SAFE_DELETE(m_Bronchodilation);
  SAFE_DELETE(m_DiastolicPressureModifier);
  SAFE_DELETE(m_EMaxShapeParameter);
  SAFE_DELETE(m_FeverModifier);
  SAFE_DELETE(m_HeartRateModifier);
  SAFE_DELETE(m_HemorrhageModifier);
  SAFE_DELETE(m_NeuromuscularBlock);
  SAFE_DELETE(m_PainModifier);
  SAFE_DELETE(m_PupilReactivityModifier);
  SAFE_DELETE(m_PupilSizeModifier);
  SAFE_DELETE(m_RespirationRateModifier);
  SAFE_DELETE(m_Sedation);
  SAFE_DELETE(m_SystolicPressureModifier);
  SAFE_DELETE(m_TidalVolumeModifier);
  SAFE_DELETE(m_TubularPermeabilityModifier);
  SAFE_DELETE(m_CentralNervousModifier);
  SAFE_DELETE(m_EffectSiteRateConstant);
}
//-----------------------------------------------------------------------------
void SESubstancePharmacodynamics::Clear()
{
  m_AntibacterialEffect->Clear();
  m_Bronchodilation->Clear();
  m_DiastolicPressureModifier->Clear();
  m_EMaxShapeParameter->Clear();
  m_FeverModifier->Clear();
  m_HeartRateModifier->Clear();
  m_HemorrhageModifier->Clear();
  m_NeuromuscularBlock->Clear();
  m_PainModifier->Clear();
  m_PupilReactivityModifier->Clear();
  m_PupilSizeModifier->Clear();
  m_RespirationRateModifier->Clear();
  m_Sedation->Clear();
  m_SystolicPressureModifier->Clear();
  m_TidalVolumeModifier->Clear();
  m_TubularPermeabilityModifier->Clear();
  m_CentralNervousModifier->Clear();
  m_EffectSiteRateConstant->Clear();
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacodynamics::IsValid() const
{
  if (!HasAntibacterialEffect())
    return false;
  if (!HasBronchodilation())
    return false;
  if (!HasDiastolicPressureModifier())
    return false;
  if (!HasEMaxShapeParameter())
    return false;
  if (!HasFeverModifier())
    return false;
  if (!HasHeartRateModifier())
    return false;
  if (!HasHemorrhageModifier())
    return false;
  if (!HasNeuromuscularBlock())
    return false;
  if (!HasPainModifier())
    return false;
  if (!HasPupilReactivityModifier())
    return false;
  if (!HasPupilSizeModifier())
    return false;
  if (!HasRespirationRateModifier())
    return false;
  if (!HasSedation())
    return false;
  if (!HasSystolicPressureModifier())
    return false;
  if (!HasTidalVolumeModifier())
    return false;
  if (!HasTubularPermeabilityModifier())
    return false;
  if (!HasCentralNervousModifier())
    return false;
  if (!HasEffectSiteRateConstant())
    return false;
  return true;
}
//-----------------------------------------------------------------------------
const SEScalar* SESubstancePharmacodynamics::GetScalar(const char* name)
{
  return GetScalar(std::string { name });
}
//-----------------------------------------------------------------------------
const SEScalar* SESubstancePharmacodynamics::GetScalar(const std::string& name)
{
  // For modifiers that use SEPharmacodynamicModifier type, assuming request will be passed as "HeartRateModifer-EMax"
  // Search to see if modifier name is a substring of request, then pass name to SEPharmacodynamicModifier::GetScalar,
  // which will search for the subrequest (EMax, EC50)
  if (name.compare("EMaxShapeParameter") == 0)
    return &GetEMaxShapeParameter();
  if (name.compare("AntibacterialEffect") == 0)
    return &GetAntibacterialEffect();
  if (name.find("Bronchodilation") != std::string::npos)
    return GetBronchodilation().GetScalar(name);
  if (name.find("DiastolicPressureModifier") != std::string::npos)
    return GetDiastolicPressureModifier().GetScalar(name);
  if (name.find("FeverModifier") != std::string::npos)
    return GetFeverModifier().GetScalar(name);
  if (name.find("HeartRateModifier") != std::string::npos)
    return GetHeartRateModifier().GetScalar(name);
  if (name.find("HemorrhageModifier") != std::string::npos)
    return GetHemorrhageModifier().GetScalar(name);
  if (name.find("NeuromuscularBlock") != std::string::npos)
    return GetNeuromuscularBlock().GetScalar(name);
  if (name.find("PainModifier") != std::string::npos)
    return GetPainModifier().GetScalar(name);
  if (name.find("RespirationRateModifier") != std::string::npos)
    return GetRespirationRateModifier().GetScalar(name);
  if (name.find("Sedation") != std::string::npos)
    return GetSedation().GetScalar(name);
  if (name.find("SystolicPressureModifier") != std::string::npos)
    return GetSystolicPressureModifier().GetScalar(name);
  if (name.find("TidalVolumeModifier") != std::string::npos)
    return GetTidalVolumeModifier().GetScalar(name);
  if (name.find("TubularPermeabilityModifier") != std::string::npos)
    return GetTubularPermeabilityModifier().GetScalar(name);
  if (name.find("CentralNervousModifier") != std::string::npos)
    return GetCentralNervousModifier().GetScalar(name);
  if (name.compare("EffectSiteRateConstant") == 0)
    return &GetEffectSiteRateConstant();
  if (name.find("PupilReactivityModifier") != std::string::npos)
    return GetPupilReactivityModifier().GetScalar(name);
  if (name.find("PupilSizeModifier") != std::string::npos)
    return GetPupilSizeModifier().GetScalar(name);
  return nullptr;
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacodynamics::HasAntibacterialEffect() const
{
  return (m_AntibacterialEffect == nullptr) ? false : m_AntibacterialEffect->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFrequency& SESubstancePharmacodynamics::GetAntibacterialEffect()
{
  if (m_AntibacterialEffect == nullptr) {
    m_AntibacterialEffect = new SEScalarFrequency();
  }

  return *m_AntibacterialEffect;
}
//-----------------------------------------------------------------------------
double SESubstancePharmacodynamics::GetAntibacterialEffect(const FrequencyUnit& unit) const
{
  if (m_AntibacterialEffect == nullptr) {
    return SEScalar::dNaN();
  }

  return m_AntibacterialEffect->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacodynamics::HasBronchodilation() const
{
  return (m_Bronchodilation == nullptr) ? false : m_Bronchodilation->IsValid();
}
//-----------------------------------------------------------------------------
SEPharmacodynamicModifier& SESubstancePharmacodynamics::GetBronchodilation()
{
  if (m_Bronchodilation == nullptr) {
    m_Bronchodilation = new SEPharmacodynamicModifier();
  }

  return *m_Bronchodilation;
}
//-----------------------------------------------------------------------------
const SEPharmacodynamicModifier* SESubstancePharmacodynamics::GetBronchodilation() const
{
  return m_Bronchodilation;
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacodynamics::HasDiastolicPressureModifier() const
{
  return (m_DiastolicPressureModifier == nullptr) ? false : m_DiastolicPressureModifier->IsValid();
}
//-----------------------------------------------------------------------------
SEPharmacodynamicModifier& SESubstancePharmacodynamics::GetDiastolicPressureModifier()
{
  if (m_DiastolicPressureModifier == nullptr) {
    m_DiastolicPressureModifier = new SEPharmacodynamicModifier();
  }

  return *m_DiastolicPressureModifier;
}
//-----------------------------------------------------------------------------
const SEPharmacodynamicModifier* SESubstancePharmacodynamics::GetDiastolicPressureModifier() const
{
  return m_DiastolicPressureModifier;
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacodynamics::HasEMaxShapeParameter() const
{
  return (m_EMaxShapeParameter == nullptr) ? false : m_EMaxShapeParameter->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar& SESubstancePharmacodynamics::GetEMaxShapeParameter()
{
  if (m_EMaxShapeParameter == nullptr) {
    m_EMaxShapeParameter = new SEScalar();
  }

  return *m_EMaxShapeParameter;
}
//-----------------------------------------------------------------------------
double SESubstancePharmacodynamics::GetEMaxShapeParameter() const
{
  if (m_EMaxShapeParameter == nullptr) {
    return SEScalar::dNaN();
  }

  return m_EMaxShapeParameter->GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacodynamics::HasFeverModifier() const
{
  return (m_FeverModifier == nullptr) ? false : m_FeverModifier->IsValid();
}
//-----------------------------------------------------------------------------
SEPharmacodynamicModifier& SESubstancePharmacodynamics::GetFeverModifier()
{
  if (m_FeverModifier == nullptr) {
    m_FeverModifier = new SEPharmacodynamicModifier();
  }

  return *m_FeverModifier;
}
//-----------------------------------------------------------------------------
const SEPharmacodynamicModifier* SESubstancePharmacodynamics::GetFeverModifier() const
{
  return m_FeverModifier;
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacodynamics::HasHeartRateModifier() const
{
  return (m_HeartRateModifier == nullptr) ? false : m_HeartRateModifier->IsValid();
}
//-----------------------------------------------------------------------------
SEPharmacodynamicModifier& SESubstancePharmacodynamics::GetHeartRateModifier()
{
  if (m_HeartRateModifier == nullptr) {
    m_HeartRateModifier = new SEPharmacodynamicModifier();
  }

  return *m_HeartRateModifier;
}
//-----------------------------------------------------------------------------
const SEPharmacodynamicModifier* SESubstancePharmacodynamics::GetHeartRateModifier() const
{
  return m_HeartRateModifier;
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacodynamics::HasHemorrhageModifier() const
{
  return (m_HemorrhageModifier == nullptr) ? false : m_HemorrhageModifier->IsValid();
}
//-----------------------------------------------------------------------------
SEPharmacodynamicModifier& SESubstancePharmacodynamics::GetHemorrhageModifier()
{
  if (m_HemorrhageModifier == nullptr) {
    m_HemorrhageModifier = new SEPharmacodynamicModifier();
  }

  return *m_HemorrhageModifier;
}
//-----------------------------------------------------------------------------
const SEPharmacodynamicModifier* SESubstancePharmacodynamics::GetHemorrhageModifier() const
{
  return m_HemorrhageModifier;
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacodynamics::HasNeuromuscularBlock() const
{
  return (m_NeuromuscularBlock == nullptr) ? false : m_NeuromuscularBlock->IsValid();
}
//-----------------------------------------------------------------------------
SEPharmacodynamicModifier& SESubstancePharmacodynamics::GetNeuromuscularBlock()
{
  if (m_NeuromuscularBlock == nullptr) {
    m_NeuromuscularBlock = new SEPharmacodynamicModifier();
  }

  return *m_NeuromuscularBlock;
}
//-----------------------------------------------------------------------------
const SEPharmacodynamicModifier* SESubstancePharmacodynamics::GetNeuromuscularBlock() const
{
  return m_NeuromuscularBlock;
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacodynamics::HasPainModifier() const
{
  return (m_PainModifier == nullptr) ? false : m_PainModifier->IsValid();
}
//-----------------------------------------------------------------------------
SEPharmacodynamicModifier& SESubstancePharmacodynamics::GetPainModifier()
{
  if (m_PainModifier == nullptr) {
    m_PainModifier = new SEPharmacodynamicModifier();
  }

  return *m_PainModifier;
}
//-----------------------------------------------------------------------------
const SEPharmacodynamicModifier* SESubstancePharmacodynamics::GetPainModifier() const
{
  return m_PainModifier;
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacodynamics::HasPupilReactivityModifier() const
{
  return (m_PupilReactivityModifier == nullptr) ? false : m_PupilReactivityModifier->IsValid();
}
//-----------------------------------------------------------------------------
SEPharmacodynamicModifier& SESubstancePharmacodynamics::GetPupilReactivityModifier()
{
  if (m_PupilReactivityModifier == nullptr) {
    m_PupilReactivityModifier = new SEPharmacodynamicModifier();
  }

  return *m_PupilReactivityModifier;
}
//-----------------------------------------------------------------------------
const SEPharmacodynamicModifier* SESubstancePharmacodynamics::GetPupilReactivityModifier() const
{
  return m_PupilReactivityModifier;
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacodynamics::HasPupilSizeModifier() const
{
  return (m_PupilSizeModifier == nullptr) ? false : m_PupilSizeModifier->IsValid();
}
//-----------------------------------------------------------------------------
SEPharmacodynamicModifier& SESubstancePharmacodynamics::GetPupilSizeModifier()
{
  if (m_PupilSizeModifier == nullptr) {
    m_PupilSizeModifier = new SEPharmacodynamicModifier();
  }

  return *m_PupilSizeModifier;
}
//-----------------------------------------------------------------------------
const SEPharmacodynamicModifier* SESubstancePharmacodynamics::GetPupilSizeModifier() const
{
  return m_PupilSizeModifier;
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacodynamics::HasRespirationRateModifier() const
{
  return (m_RespirationRateModifier == nullptr) ? false : m_RespirationRateModifier->IsValid();
}
//-----------------------------------------------------------------------------
SEPharmacodynamicModifier& SESubstancePharmacodynamics::GetRespirationRateModifier()
{
  if (m_RespirationRateModifier == nullptr) {
    m_RespirationRateModifier = new SEPharmacodynamicModifier();
  }

  return *m_RespirationRateModifier;
}
//-----------------------------------------------------------------------------
const SEPharmacodynamicModifier* SESubstancePharmacodynamics::GetRespirationRateModifier() const
{
  return m_RespirationRateModifier;
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacodynamics::HasSedation() const
{
  return (m_Sedation == nullptr) ? false : m_Sedation->IsValid();
}
//-----------------------------------------------------------------------------
SEPharmacodynamicModifier& SESubstancePharmacodynamics::GetSedation()
{
  if (m_Sedation == nullptr) {
    m_Sedation = new SEPharmacodynamicModifier();
  }

  return *m_Sedation;
}
//-----------------------------------------------------------------------------
const SEPharmacodynamicModifier* SESubstancePharmacodynamics::GetSedation() const
{
  return m_Sedation;
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacodynamics::HasSystolicPressureModifier() const
{
  return (m_SystolicPressureModifier == nullptr) ? false : m_SystolicPressureModifier->IsValid();
}
//-----------------------------------------------------------------------------
SEPharmacodynamicModifier& SESubstancePharmacodynamics::GetSystolicPressureModifier()
{
  if (m_SystolicPressureModifier == nullptr) {
    m_SystolicPressureModifier = new SEPharmacodynamicModifier();
  }

  return *m_SystolicPressureModifier;
}
//-----------------------------------------------------------------------------
const SEPharmacodynamicModifier* SESubstancePharmacodynamics::GetSystolicPressureModifier() const
{
  return m_SystolicPressureModifier;
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacodynamics::HasTidalVolumeModifier() const
{
  return (m_TidalVolumeModifier == nullptr) ? false : m_TidalVolumeModifier->IsValid();
}
//-----------------------------------------------------------------------------
SEPharmacodynamicModifier& SESubstancePharmacodynamics::GetTidalVolumeModifier()
{
  if (m_TidalVolumeModifier == nullptr) {
    m_TidalVolumeModifier = new SEPharmacodynamicModifier();
  }

  return *m_TidalVolumeModifier;
}
//-----------------------------------------------------------------------------
const SEPharmacodynamicModifier* SESubstancePharmacodynamics::GetTidalVolumeModifier() const
{
  return m_TidalVolumeModifier;
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacodynamics::HasTubularPermeabilityModifier() const
{
  return (m_TubularPermeabilityModifier == nullptr) ? false : m_TubularPermeabilityModifier->IsValid();
}
//-----------------------------------------------------------------------------
SEPharmacodynamicModifier& SESubstancePharmacodynamics::GetTubularPermeabilityModifier()
{
  if (m_TubularPermeabilityModifier == nullptr)
    m_TubularPermeabilityModifier = new SEPharmacodynamicModifier();
  return *m_TubularPermeabilityModifier;
}
//-----------------------------------------------------------------------------
const SEPharmacodynamicModifier* SESubstancePharmacodynamics::GetTubularPermeabilityModifier() const
{
  return m_TubularPermeabilityModifier;
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacodynamics::HasCentralNervousModifier() const
{
  return (m_CentralNervousModifier == nullptr) ? false : m_CentralNervousModifier->IsValid();
}
//-----------------------------------------------------------------------------
SEPharmacodynamicModifier& SESubstancePharmacodynamics::GetCentralNervousModifier()
{
  if (m_CentralNervousModifier == nullptr) {
    m_CentralNervousModifier = new SEPharmacodynamicModifier();
  }

  return *m_CentralNervousModifier;
}
//-----------------------------------------------------------------------------
const SEPharmacodynamicModifier* SESubstancePharmacodynamics::GetCentralNervousModifier() const
{
  return m_CentralNervousModifier;
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacodynamics::HasEffectSiteRateConstant() const
{
  return (m_EffectSiteRateConstant == nullptr) ? false : m_EffectSiteRateConstant->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFrequency& SESubstancePharmacodynamics::GetEffectSiteRateConstant()
{
  if (m_EffectSiteRateConstant == nullptr) {
    m_EffectSiteRateConstant = new SEScalarFrequency();
  }

  return *m_EffectSiteRateConstant;
}
//-----------------------------------------------------------------------------
double SESubstancePharmacodynamics::GetEffectSiteRateConstant(const FrequencyUnit& unit) const
{
  if (m_EffectSiteRateConstant == nullptr)
    return SEScalar::dNaN();
  return m_EffectSiteRateConstant->GetValue(unit);
}
//-----------------------------------------------------------------------------
std::map<std::string, SEPharmacodynamicModifier*> SESubstancePharmacodynamics::GetPharmacodynamicModifiers() const
{
  return m_Modifiers;
}
SEPharmacodynamicModifier::SEPharmacodynamicModifier(SEPharmacodynamicModifier const& obj)
  : m_EC50(new SEScalarMassPerVolume(*obj.m_EC50))
  , m_EMax(new SEScalarFraction(*obj.m_EMax))
{

}
SEPharmacodynamicModifier::SEPharmacodynamicModifier(SEScalarMassPerVolume const& ec50, SEScalarFraction const& max)
  : m_EC50(std::make_unique<SEScalarMassPerVolume>(ec50).release())
  , m_EMax(std::make_unique<SEScalarFraction>(max).release())
{
}
SEPharmacodynamicModifier::SEPharmacodynamicModifier()
  : m_EC50(std::make_unique<SEScalarMassPerVolume>().release())
  , m_EMax(std::make_unique<SEScalarFraction>().release())
{
}
//-----------------------------------------------------------------------------
SEPharmacodynamicModifier::~SEPharmacodynamicModifier()
{
  Clear();
  SAFE_DELETE(m_EMax);
  SAFE_DELETE(m_EC50);
}
//-----------------------------------------------------------------------------
void SEPharmacodynamicModifier::Clear()
{
  m_EMax->Clear();
  m_EC50->Clear();
}
//-----------------------------------------------------------------------------
bool SEPharmacodynamicModifier::IsValid() const
{
  return (HasEMax() && HasEC50());
}
//-----------------------------------------------------------------------------
const SEScalar* SEPharmacodynamicModifier::GetScalar(const char* name)
{
  return GetScalar(std::string { name });
}
//-----------------------------------------------------------------------------
const SEScalar* SEPharmacodynamicModifier::GetScalar(const std::string& name)
{
  if (name.find("EMax") != std::string::npos) {
    return &GetEMax();
  }
  if (name.find("EC50") != std::string::npos) {
    return &GetEC50();
  }
  return nullptr;
}
//-----------------------------------------------------------------------------
bool SEPharmacodynamicModifier::HasEMax() const
{
  return (m_EMax == nullptr) ? false : m_EMax->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFraction& SEPharmacodynamicModifier::GetEMax()
{
  if (m_EMax == nullptr) {
    m_EMax = new SEScalarFraction();
  }
  return *m_EMax;
}
//-----------------------------------------------------------------------------
double SEPharmacodynamicModifier::GetEMax() const
{
  if (m_EMax == nullptr) {
    return SEScalar::dNaN();
  }
  return m_EMax->GetValue();
}
//-----------------------------------------------------------------------------
bool SEPharmacodynamicModifier::HasEC50() const
{
  return (m_EC50 == nullptr) ? false : m_EC50->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerVolume& SEPharmacodynamicModifier::GetEC50()
{
  if (m_EC50 == nullptr) {
    m_EC50 = new SEScalarMassPerVolume();
  }

  return *m_EC50;
}
//-----------------------------------------------------------------------------
double SEPharmacodynamicModifier::GetEC50(const MassPerVolumeUnit& unit) const
{
  if (m_EC50 == nullptr) {
    return SEScalar::dNaN();
  }

  return m_EC50->GetValue(unit);
}
//-----------------------------------------------------------------------------
void SEPharmacodynamicModifier::Set(SEScalarFraction const& eMax, SEScalarMassPerVolume const& ec50)
{
  this->GetEMax() = eMax;
  this->GetEC50() = ec50;
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacodynamics::operator==(const SESubstancePharmacodynamics& rhs) const
{
  bool equivilant = (m_AntibacterialEffect && rhs.m_AntibacterialEffect) ? m_AntibacterialEffect->operator==(*rhs.m_AntibacterialEffect) : m_AntibacterialEffect == rhs.m_AntibacterialEffect;
  equivilant &= (m_Bronchodilation && rhs.m_Bronchodilation) ? m_Bronchodilation->operator==(*rhs.m_Bronchodilation) : m_Bronchodilation == rhs.m_Bronchodilation;
  equivilant &= (m_DiastolicPressureModifier && rhs.m_DiastolicPressureModifier) ? m_DiastolicPressureModifier->operator==(*rhs.m_DiastolicPressureModifier) : m_DiastolicPressureModifier == rhs.m_DiastolicPressureModifier;
  equivilant &= (m_EMaxShapeParameter && rhs.m_EMaxShapeParameter) ? m_EMaxShapeParameter->operator==(*rhs.m_EMaxShapeParameter) : m_EMaxShapeParameter == rhs.m_EMaxShapeParameter;
  equivilant &= (m_FeverModifier && rhs.m_FeverModifier) ? m_FeverModifier->operator==(*rhs.m_FeverModifier) : m_FeverModifier == rhs.m_FeverModifier;
  equivilant &= (m_HeartRateModifier && rhs.m_HeartRateModifier) ? m_HeartRateModifier->operator==(*rhs.m_HeartRateModifier) : m_HeartRateModifier == rhs.m_HeartRateModifier;
  equivilant &= (m_HemorrhageModifier && rhs.m_HemorrhageModifier) ? m_HemorrhageModifier->operator==(*rhs.m_HemorrhageModifier) : m_HemorrhageModifier == rhs.m_HemorrhageModifier;
  equivilant &= (m_NeuromuscularBlock && rhs.m_NeuromuscularBlock) ? m_NeuromuscularBlock->operator==(*rhs.m_NeuromuscularBlock) : m_NeuromuscularBlock == rhs.m_NeuromuscularBlock;
  equivilant &= (m_PainModifier && rhs.m_PainModifier) ? m_PainModifier->operator==(*rhs.m_PainModifier) : m_PainModifier == rhs.m_PainModifier;
  equivilant &= (m_PupilReactivityModifier && rhs.m_PupilReactivityModifier) ? m_PupilReactivityModifier->operator==(*rhs.m_PupilReactivityModifier) : m_PupilReactivityModifier == rhs.m_PupilReactivityModifier;
  equivilant &= (m_PupilSizeModifier && rhs.m_PupilSizeModifier) ? m_PupilSizeModifier->operator==(*rhs.m_PupilSizeModifier) : m_PupilSizeModifier == rhs.m_PupilSizeModifier;
  equivilant &= (m_RespirationRateModifier && rhs.m_RespirationRateModifier) ? m_RespirationRateModifier->operator==(*rhs.m_RespirationRateModifier) : m_RespirationRateModifier == rhs.m_RespirationRateModifier;
  equivilant &= (m_Sedation && rhs.m_Sedation) ? m_Sedation->operator==(*rhs.m_Sedation) : m_Sedation == rhs.m_Sedation;
  equivilant &= (m_SystolicPressureModifier && rhs.m_SystolicPressureModifier) ? m_SystolicPressureModifier->operator==(*rhs.m_SystolicPressureModifier) : m_SystolicPressureModifier == rhs.m_SystolicPressureModifier;
  equivilant &= (m_TidalVolumeModifier && rhs.m_TidalVolumeModifier) ? m_TidalVolumeModifier->operator==(*rhs.m_TidalVolumeModifier) : m_TidalVolumeModifier == rhs.m_TidalVolumeModifier;
  equivilant &= (m_TubularPermeabilityModifier && rhs.m_TubularPermeabilityModifier) ? m_TubularPermeabilityModifier->operator==(*rhs.m_TubularPermeabilityModifier) : m_TubularPermeabilityModifier == rhs.m_TubularPermeabilityModifier;
  equivilant &= (m_CentralNervousModifier && rhs.m_CentralNervousModifier) ? m_CentralNervousModifier->operator==(*rhs.m_CentralNervousModifier) : m_CentralNervousModifier == rhs.m_CentralNervousModifier;
  equivilant &= (m_EffectSiteRateConstant && rhs.m_EffectSiteRateConstant) ? m_EffectSiteRateConstant->operator==(*rhs.m_EffectSiteRateConstant) : m_EffectSiteRateConstant == rhs.m_EffectSiteRateConstant;
  if (equivilant) {
    for (auto& pair : m_Modifiers) {
      // auto lh = m_TissueKinetics.find(pair.first);
      auto rh = rhs.m_Modifiers.find(pair.first);
      if (rh != rhs.m_Modifiers.end()) {
        equivilant &= (pair.second && rh->second) ? *pair.second == *rh->second : pair.second == rh->second;
      }
    }
  }
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SESubstancePharmacodynamics::operator!=(const SESubstancePharmacodynamics& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
bool SEPharmacodynamicModifier::operator==(const SEPharmacodynamicModifier& rhs) const
{
  bool equivilant = (m_EMax && rhs.m_EMax) ? m_EMax->operator==(*rhs.m_EMax) : m_EMax == rhs.m_EMax;
  equivilant &= (m_EC50 && rhs.m_EC50) ? m_EC50->operator==(*rhs.m_EC50) : m_EC50 == rhs.m_EC50;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SEPharmacodynamicModifier::operator!=(const SEPharmacodynamicModifier& rhs) const
{
  return !(*this == rhs);
}
}