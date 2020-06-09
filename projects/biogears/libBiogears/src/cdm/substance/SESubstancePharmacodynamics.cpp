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
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>
#include <biogears/schema/cdm/Properties.hxx>
#include <biogears/schema/cdm/Substance.hxx>

namespace biogears {
SESubstancePharmacodynamics::SESubstancePharmacodynamics(Logger* logger)
  : Loggable(logger)
{
  m_AntibacterialEffect = nullptr;
  m_Bronchodilation = nullptr;
  m_DiastolicPressureModifier = nullptr;
  m_EMaxShapeParameter = nullptr;
  m_FeverModifier = nullptr;
  m_HeartRateModifier = nullptr;
  m_HemorrhageModifier = nullptr;
  m_NeuromuscularBlock = nullptr;
  m_PainModifier = nullptr;
  m_PupilReactivityModifier = nullptr;
  m_PupilSizeModifier = nullptr;
  m_RespirationRateModifier = nullptr;
  m_Sedation = nullptr;
  m_SystolicPressureModifier = nullptr;
  m_TidalVolumeModifier = nullptr;
  m_TubularPermeabilityModifier = nullptr;
  m_CentralNervousModifier = nullptr;
  m_EffectSiteRateConstant = nullptr;
}
//-----------------------------------------------------------------------------
SESubstancePharmacodynamics::~SESubstancePharmacodynamics()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SESubstancePharmacodynamics::Clear()
{
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
  //For modifiers that use SEPharmacodynamicModifier type, assuming request will be passed as "HeartRateModifer-EMax"
  //Search to see if modifier name is a substring of request, then pass name to SEPharmacodynamicModifier::GetScalar,
  //which will search for the subrequest (EMax, EC50)
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
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacodynamics::Load(const CDM::SubstancePharmacodynamicsData& in)
{
  GetBronchodilation().Load(in.Bronchodilation());
  GetDiastolicPressureModifier().Load(in.DiastolicPressureModifier());
  GetEMaxShapeParameter().Load(in.EMaxShapeParameter());
  GetFeverModifier().Load(in.FeverModifier());
  GetHeartRateModifier().Load(in.HeartRateModifier());
  GetHemorrhageModifier().Load(in.HemorrhageModifier());
  GetNeuromuscularBlock().Load(in.NeuromuscularBlock());
  GetPainModifier().Load(in.PainModifier());
  GetPupilReactivityModifier().Load(in.PupilReactivityModifier());
  GetPupilSizeModifier().Load(in.PupilSizeModifier());
  GetRespirationRateModifier().Load(in.RespirationRateModifier());
  GetSedation().Load(in.Sedation());
  GetSystolicPressureModifier().Load(in.SystolicPressureModifier());
  GetTidalVolumeModifier().Load(in.TidalVolumeModifier());
  GetTubularPermeabilityModifier().Load(in.TubularPermeabilityModifier());
  GetCentralNervousModifier().Load(in.CentralNervousModifier());
  GetAntibacterialEffect().Load(in.AntibacterialEffect());
  GetEffectSiteRateConstant().Load(in.EffectSiteRateConstant());
  return true;
}
//-----------------------------------------------------------------------------
CDM::SubstancePharmacodynamicsData* SESubstancePharmacodynamics::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::SubstancePharmacodynamicsData* data = new CDM::SubstancePharmacodynamicsData();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SESubstancePharmacodynamics::Unload(CDM::SubstancePharmacodynamicsData& data) const
{
  if (HasBronchodilation())
    data.Bronchodilation(std::unique_ptr<CDM::PharmacodynamicModifierData>(m_Bronchodilation->Unload()));
  if (HasDiastolicPressureModifier())
    data.DiastolicPressureModifier(std::unique_ptr<CDM::PharmacodynamicModifierData>(m_DiastolicPressureModifier->Unload()));
  if (HasEMaxShapeParameter())
    data.EMaxShapeParameter(std::unique_ptr<CDM::ScalarData>(m_EMaxShapeParameter->Unload()));
  if (HasFeverModifier())
    data.FeverModifier(std::unique_ptr<CDM::PharmacodynamicModifierData>(m_FeverModifier->Unload()));
  if (HasHeartRateModifier())
    data.HeartRateModifier(std::unique_ptr<CDM::PharmacodynamicModifierData>(m_HeartRateModifier->Unload()));
  if (HasHemorrhageModifier())
    data.HemorrhageModifier(std::unique_ptr<CDM::PharmacodynamicModifierData>(m_HemorrhageModifier->Unload()));
  if (HasNeuromuscularBlock())
    data.NeuromuscularBlock(std::unique_ptr<CDM::PharmacodynamicModifierData>(m_NeuromuscularBlock->Unload()));
  if (HasPainModifier())
    data.PainModifier(std::unique_ptr<CDM::PharmacodynamicModifierData>(m_PainModifier->Unload()));
  if (HasPupilReactivityModifier())
    data.PupilReactivityModifier(std::unique_ptr<CDM::PharmacodynamicModifierData>(m_PupilReactivityModifier->Unload()));
  if (HasPupilSizeModifier())
    data.PupilSizeModifier(std::unique_ptr<CDM::PharmacodynamicModifierData>(m_PupilReactivityModifier->Unload()));
  if (HasRespirationRateModifier())
    data.RespirationRateModifier(std::unique_ptr<CDM::PharmacodynamicModifierData>(m_RespirationRateModifier->Unload()));
  if (HasSedation())
    data.Sedation(std::unique_ptr<CDM::PharmacodynamicModifierData>(m_Sedation->Unload()));
  if (HasSystolicPressureModifier())
    data.SystolicPressureModifier(std::unique_ptr<CDM::PharmacodynamicModifierData>(m_SystolicPressureModifier->Unload()));
  if (HasTidalVolumeModifier())
    data.TidalVolumeModifier(std::unique_ptr<CDM::PharmacodynamicModifierData>(m_TidalVolumeModifier->Unload()));
  if (HasTubularPermeabilityModifier())
    data.TubularPermeabilityModifier(std::unique_ptr<CDM::PharmacodynamicModifierData>(m_TubularPermeabilityModifier->Unload()));
  if (HasCentralNervousModifier())
    data.CentralNervousModifier(std::unique_ptr<CDM::PharmacodynamicModifierData>(m_CentralNervousModifier->Unload()));
  if (HasAntibacterialEffect())
    data.AntibacterialEffect(std::unique_ptr<CDM::ScalarFrequencyData>(m_AntibacterialEffect->Unload()));
  if (HasEffectSiteRateConstant())
    data.EffectSiteRateConstant(std::unique_ptr<CDM::ScalarFrequencyData>(m_EffectSiteRateConstant->Unload()));
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

SEPharmacodynamicModifier::SEPharmacodynamicModifier()
{
  m_EMax = nullptr;
  m_EC50 = nullptr;
}
//-----------------------------------------------------------------------------
SEPharmacodynamicModifier::~SEPharmacodynamicModifier()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEPharmacodynamicModifier::Clear()
{
  SAFE_DELETE(m_EMax);
  SAFE_DELETE(m_EC50);
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
  if (name.find("EMax") != std::string::npos)
    return &GetEMax();
  if (name.find("EC50") != std::string::npos)
    return &GetEC50();
}
//-----------------------------------------------------------------------------
bool SEPharmacodynamicModifier::Load(const CDM::PharmacodynamicModifierData& in)
{
  GetEMax().Load(in.EMax());
  GetEC50().Load(in.EC50());

  return true;
}
//-----------------------------------------------------------------------------
CDM::PharmacodynamicModifierData* SEPharmacodynamicModifier::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::PharmacodynamicModifierData* data = new CDM::PharmacodynamicModifierData();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEPharmacodynamicModifier::Unload(CDM::PharmacodynamicModifierData& data) const
{
  if (HasEMax())
    data.EMax(std::unique_ptr<CDM::ScalarFractionData>(m_EMax->Unload()));
  if (HasEC50())
    data.EC50(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_EC50->Unload()));
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

}