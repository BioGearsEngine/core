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

#include <biogears/cdm/Macros.h>
#include <utility>

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

SESubstancePharmacodynamics::SESubstancePharmacodynamics(Logger* logger)
  : Loggable(logger)
  , m_Modifiers()
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
  , m_EffectSiteRateConstant(new SEScalarFrequency())
{
  m_Modifiers.clear();
  m_Modifiers["Bronchodilation"] = m_Bronchodilation;
  m_Modifiers["CentralNervous"] = m_CentralNervousModifier;
  m_Modifiers["DiastolicPressure"] = m_DiastolicPressureModifier;
  m_Modifiers["Fever"] = m_FeverModifier;
  m_Modifiers["HeartRate"] = m_HeartRateModifier;
  m_Modifiers["Hemorrhage"] = m_HemorrhageModifier;
  m_Modifiers["NeuromuscularBlock"] = m_NeuromuscularBlock;
  m_Modifiers["Pain"] = m_PainModifier;
  m_Modifiers["PupilReactivity"] = m_PupilReactivityModifier;
  m_Modifiers["PupilSize"] = m_PupilSizeModifier;
  m_Modifiers["RespirationRate"] = m_RespirationRateModifier;
  m_Modifiers["Sedation"] = m_Sedation;
  m_Modifiers["SystolicPressure"] = m_SystolicPressureModifier;
  m_Modifiers["TidalVolume"] = m_TidalVolumeModifier;
  m_Modifiers["TubularPermeability"] = m_TubularPermeabilityModifier;
}
//-----------------------------------------------------------------------------
SESubstancePharmacodynamics::SESubstancePharmacodynamics(SESubstancePharmacodynamics const& obj)
  : Loggable(obj.GetLogger())
  , m_Modifiers()
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
  m_Modifiers.clear();
  m_Modifiers["Bronchodilation"] = m_Bronchodilation;
  m_Modifiers["CentralNervous"] = m_CentralNervousModifier;
  m_Modifiers["DiastolicPressure"] = m_DiastolicPressureModifier;
  m_Modifiers["Fever"] = m_FeverModifier;
  m_Modifiers["HeartRate"] = m_HeartRateModifier;
  m_Modifiers["Hemorrhage"] = m_HemorrhageModifier;
  m_Modifiers["NeuromuscularBlock"] = m_NeuromuscularBlock;
  m_Modifiers["Pain"] = m_PainModifier;
  m_Modifiers["PupilReactivity"] = m_PupilReactivityModifier;
  m_Modifiers["PupilSize"] = m_PupilSizeModifier;
  m_Modifiers["RespirationRate"] = m_RespirationRateModifier;
  m_Modifiers["Sedation"] = m_Sedation;
  m_Modifiers["SystolicPressure"] = m_SystolicPressureModifier;
  m_Modifiers["TidalVolume"] = m_TidalVolumeModifier;
  m_Modifiers["TubularPermeability"] = m_TubularPermeabilityModifier;
}
//-----------------------------------------------------------------------------
SESubstancePharmacodynamics::SESubstancePharmacodynamics(SESubstancePharmacodynamics&& obj)
  : Loggable(obj.GetLogger())
  , m_Modifiers(std ::exchange(obj.m_Modifiers, decltype(m_Modifiers)()))
  , m_AntibacterialEffect(std::exchange(obj.m_AntibacterialEffect, nullptr))
  , m_Bronchodilation(std::exchange(obj.m_Bronchodilation, nullptr))
  , m_DiastolicPressureModifier(std::exchange(obj.m_DiastolicPressureModifier, nullptr))
  , m_EMaxShapeParameter(std::exchange(obj.m_EMaxShapeParameter, nullptr))
  , m_FeverModifier(std::exchange(obj.m_FeverModifier, nullptr))
  , m_HeartRateModifier(std::exchange(obj.m_HeartRateModifier, nullptr))
  , m_HemorrhageModifier(std::exchange(obj.m_HemorrhageModifier, nullptr))
  , m_NeuromuscularBlock(std::exchange(obj.m_NeuromuscularBlock, nullptr))
  , m_PainModifier(std::exchange(obj.m_PainModifier, nullptr))
  , m_PupilReactivityModifier(std::exchange(obj.m_PupilReactivityModifier, nullptr))
  , m_PupilSizeModifier(std::exchange(obj.m_PupilSizeModifier, nullptr))
  , m_RespirationRateModifier(std::exchange(obj.m_RespirationRateModifier, nullptr))
  , m_Sedation(std::exchange(obj.m_Sedation, nullptr))
  , m_SystolicPressureModifier(std::exchange(obj.m_SystolicPressureModifier, nullptr))
  , m_TidalVolumeModifier(std::exchange(obj.m_TidalVolumeModifier, nullptr))
  , m_TubularPermeabilityModifier(std::exchange(obj.m_TubularPermeabilityModifier, nullptr))
  , m_CentralNervousModifier(std::exchange(obj.m_CentralNervousModifier, nullptr))
  , m_EffectSiteRateConstant(std::exchange(obj.m_EffectSiteRateConstant, nullptr))
{
  m_Modifiers.clear();
  m_Modifiers["Bronchodilation"] = m_Bronchodilation;
  m_Modifiers["CentralNervous"] = m_CentralNervousModifier;
  m_Modifiers["DiastolicPressure"] = m_DiastolicPressureModifier;
  m_Modifiers["Fever"] = m_FeverModifier;
  m_Modifiers["HeartRate"] = m_HeartRateModifier;
  m_Modifiers["Hemorrhage"] = m_HemorrhageModifier;
  m_Modifiers["NeuromuscularBlock"] = m_NeuromuscularBlock;
  m_Modifiers["Pain"] = m_PainModifier;
  m_Modifiers["PupilReactivity"] = m_PupilReactivityModifier;
  m_Modifiers["PupilSize"] = m_PupilSizeModifier;
  m_Modifiers["RespirationRate"] = m_RespirationRateModifier;
  m_Modifiers["Sedation"] = m_Sedation;
  m_Modifiers["SystolicPressure"] = m_SystolicPressureModifier;
  m_Modifiers["TidalVolume"] = m_TidalVolumeModifier;
  m_Modifiers["TubularPermeability"] = m_TubularPermeabilityModifier;
}
//-----------------------------------------------------------------------------
SESubstancePharmacodynamics& SESubstancePharmacodynamics::operator=(SESubstancePharmacodynamics const& rhs)
{
  if (this == &rhs)
    return *this;

  GetAntibacterialEffect() = *rhs.m_AntibacterialEffect;
  GetBronchodilation() = *rhs.m_Bronchodilation;
  GetDiastolicPressureModifier() = *rhs.m_DiastolicPressureModifier;
  GetEMaxShapeParameter() = *rhs.m_EMaxShapeParameter;
  GetFeverModifier() = *rhs.m_FeverModifier;
  GetHeartRateModifier() = *rhs.m_HeartRateModifier;
  GetHemorrhageModifier() = *rhs.m_HemorrhageModifier;
  GetNeuromuscularBlock() = *rhs.m_NeuromuscularBlock;
  GetPainModifier() = *rhs.m_PainModifier;
  GetPupilReactivityModifier() = *rhs.m_PupilReactivityModifier;
  GetPupilSizeModifier() = *rhs.m_PupilSizeModifier;
  GetRespirationRateModifier() = *rhs.m_RespirationRateModifier;
  GetSedation() = *rhs.m_Sedation;
  GetSystolicPressureModifier() = *rhs.m_SystolicPressureModifier;
  GetTidalVolumeModifier() = *rhs.m_TidalVolumeModifier;
  GetTubularPermeabilityModifier() = *rhs.m_TubularPermeabilityModifier;
  GetCentralNervousModifier() = *rhs.m_CentralNervousModifier;

  m_Modifiers.clear();
  m_Modifiers["Bronchodilation"] = m_Bronchodilation;
  m_Modifiers["CentralNervous"] = m_CentralNervousModifier;
  m_Modifiers["DiastolicPressure"] = m_DiastolicPressureModifier;
  m_Modifiers["Fever"] = m_FeverModifier;
  m_Modifiers["HeartRate"] = m_HeartRateModifier;
  m_Modifiers["Hemorrhage"] = m_HemorrhageModifier;
  m_Modifiers["NeuromuscularBlock"] = m_NeuromuscularBlock;
  m_Modifiers["Pain"] = m_PainModifier;
  m_Modifiers["PupilReactivity"] = m_PupilReactivityModifier;
  m_Modifiers["PupilSize"] = m_PupilSizeModifier;
  m_Modifiers["RespirationRate"] = m_RespirationRateModifier;
  m_Modifiers["Sedation"] = m_Sedation;
  m_Modifiers["SystolicPressure"] = m_SystolicPressureModifier;
  m_Modifiers["TidalVolume"] = m_TidalVolumeModifier;
  m_Modifiers["TubularPermeability"] = m_TubularPermeabilityModifier;

  return *this;
}
SESubstancePharmacodynamics& SESubstancePharmacodynamics::operator=(SESubstancePharmacodynamics&& rhs)
{
  if (this == &rhs)
    return *this;

  m_Modifiers = std::move(rhs.m_Modifiers);
  GetAntibacterialEffect() = std::move(*rhs.m_AntibacterialEffect);
  GetBronchodilation() = std::move(*rhs.m_Bronchodilation);
  GetDiastolicPressureModifier() = std::move(*rhs.m_DiastolicPressureModifier);
  GetEMaxShapeParameter() = std::move(*rhs.m_EMaxShapeParameter);
  GetFeverModifier() = std::move(*rhs.m_FeverModifier);
  GetHeartRateModifier() = std::move(*rhs.m_HeartRateModifier);
  GetHemorrhageModifier() = std::move(*rhs.m_HemorrhageModifier);
  GetNeuromuscularBlock() = std::move(*rhs.m_NeuromuscularBlock);
  GetPainModifier() = std::move(*rhs.m_PainModifier);
  GetPupilReactivityModifier() = std::move(*rhs.m_PupilReactivityModifier);
  GetPupilSizeModifier() = std::move(*rhs.m_PupilSizeModifier);
  GetRespirationRateModifier() = std::move(*rhs.m_RespirationRateModifier);
  GetSedation() = std::move(*rhs.m_Sedation);
  GetSystolicPressureModifier() = std::move(*rhs.m_SystolicPressureModifier);
  GetTidalVolumeModifier() = std::move(*rhs.m_TidalVolumeModifier);
  GetTubularPermeabilityModifier() = std::move(*rhs.m_TubularPermeabilityModifier);
  GetCentralNervousModifier() = std::move(*rhs.m_CentralNervousModifier);
  GetEffectSiteRateConstant() = std::move(*rhs.m_EffectSiteRateConstant);

  m_Modifiers.clear();
  m_Modifiers["Bronchodilation"] = m_Bronchodilation;
  m_Modifiers["CentralNervous"] = m_CentralNervousModifier;
  m_Modifiers["DiastolicPressure"] = m_DiastolicPressureModifier;
  m_Modifiers["Fever"] = m_FeverModifier;
  m_Modifiers["HeartRate"] = m_HeartRateModifier;
  m_Modifiers["Hemorrhage"] = m_HemorrhageModifier;
  m_Modifiers["NeuromuscularBlock"] = m_NeuromuscularBlock;
  m_Modifiers["Pain"] = m_PainModifier;
  m_Modifiers["PupilReactivity"] = m_PupilReactivityModifier;
  m_Modifiers["PupilSize"] = m_PupilSizeModifier;
  m_Modifiers["RespirationRate"] = m_RespirationRateModifier;
  m_Modifiers["Sedation"] = m_Sedation;
  m_Modifiers["SystolicPressure"] = m_SystolicPressureModifier;
  m_Modifiers["TidalVolume"] = m_TidalVolumeModifier;
  m_Modifiers["TubularPermeability"] = m_TubularPermeabilityModifier;
  return *this;
}
//-----------------------------------------------------------------------------
SESubstancePharmacodynamics::~SESubstancePharmacodynamics()
{
  m_Modifiers.clear(); // NO LEAK JUST A DIFFERENT WAY TO REFERENCE THE BELOW.
   SEScalarFrequency* m_AntibacterialEffect;
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
  if (m_AntibacterialEffect) {
    m_AntibacterialEffect->Clear();
  }
  if (m_Bronchodilation) {
    m_Bronchodilation->Clear();
  }
  if (m_DiastolicPressureModifier) {
    m_DiastolicPressureModifier->Clear();
  }
  if (m_EMaxShapeParameter) {
    m_EMaxShapeParameter->Clear();
  }
  if (m_FeverModifier) {
    m_FeverModifier->Clear();
  }
  if (m_HeartRateModifier) {
    m_HeartRateModifier->Clear();
  }
  if (m_HemorrhageModifier) {
    m_HemorrhageModifier->Clear();
  }
  if (m_NeuromuscularBlock) {
    m_NeuromuscularBlock->Clear();
  }
  if (m_PainModifier) {
    m_PainModifier->Clear();
  }
  if (m_PupilReactivityModifier) {
    m_PupilReactivityModifier->Clear();
  }
  if (m_PupilSizeModifier) {
    m_PupilSizeModifier->Clear();
  }
  if (m_RespirationRateModifier) {
    m_RespirationRateModifier->Clear();
  }
  if (m_Sedation) {
    m_Sedation->Clear();
  }
  if (m_SystolicPressureModifier) {
    m_SystolicPressureModifier->Clear();
  }
  if (m_TidalVolumeModifier) {
    m_TidalVolumeModifier->Clear();
  }
  if (m_TubularPermeabilityModifier) {
    m_TubularPermeabilityModifier->Clear();
  }
  if (m_CentralNervousModifier) {
    m_CentralNervousModifier->Clear();
  }
  if (m_EffectSiteRateConstant) {
    m_EffectSiteRateConstant->Clear();
  }
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
SEPharmacodynamicModifier const& SESubstancePharmacodynamics::GetBronchodilation() const
{
  return *m_Bronchodilation;
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
SEPharmacodynamicModifier const& SESubstancePharmacodynamics::GetDiastolicPressureModifier() const
{
  return *m_DiastolicPressureModifier;
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
SEPharmacodynamicModifier const& SESubstancePharmacodynamics::GetFeverModifier() const
{
  return *m_FeverModifier;
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
SEPharmacodynamicModifier const& SESubstancePharmacodynamics::GetHeartRateModifier() const
{
  return *m_HeartRateModifier;
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
SEPharmacodynamicModifier const& SESubstancePharmacodynamics::GetHemorrhageModifier() const
{
  return *m_HemorrhageModifier;
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
SEPharmacodynamicModifier const& SESubstancePharmacodynamics::GetNeuromuscularBlock() const
{
  return *m_NeuromuscularBlock;
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
SEPharmacodynamicModifier const& SESubstancePharmacodynamics::GetPainModifier() const
{
  return *m_PainModifier;
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
SEPharmacodynamicModifier const& SESubstancePharmacodynamics::GetPupilReactivityModifier() const
{
  return *m_PupilReactivityModifier;
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
SEPharmacodynamicModifier const& SESubstancePharmacodynamics::GetPupilSizeModifier() const
{
  return *m_PupilSizeModifier;
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
SEPharmacodynamicModifier const& SESubstancePharmacodynamics::GetRespirationRateModifier() const
{
  return *m_RespirationRateModifier;
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
SEPharmacodynamicModifier const& SESubstancePharmacodynamics::GetSedation() const
{
  return *m_Sedation;
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
SEPharmacodynamicModifier const& SESubstancePharmacodynamics::GetSystolicPressureModifier() const
{
  return *m_SystolicPressureModifier;
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
SEPharmacodynamicModifier const& SESubstancePharmacodynamics::GetTidalVolumeModifier() const
{
  return *m_TidalVolumeModifier;
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
SEPharmacodynamicModifier const& SESubstancePharmacodynamics::GetTubularPermeabilityModifier() const
{
  return *m_TubularPermeabilityModifier;
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
SEPharmacodynamicModifier const& SESubstancePharmacodynamics::GetCentralNervousModifier() const
{
  return *m_CentralNervousModifier;
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
//-----------------------------------------------------------------------------
SEPharmacodynamicModifier::SEPharmacodynamicModifier()
  : m_EC50(std::make_unique<SEScalarMassPerVolume>().release())
  , m_EMax(std::make_unique<SEScalarFraction>().release())
{
}
//-----------------------------------------------------------------------------
SEPharmacodynamicModifier::SEPharmacodynamicModifier(SEScalarMassPerVolume const& ec50, SEScalarFraction const& max)
  : m_EC50(std::make_unique<SEScalarMassPerVolume>(ec50).release())
  , m_EMax(std::make_unique<SEScalarFraction>(max).release())
{
}
//-----------------------------------------------------------------------------
SEPharmacodynamicModifier::SEPharmacodynamicModifier(SEPharmacodynamicModifier const& obj)
  : m_EC50(std::make_unique<SEScalarMassPerVolume>(*obj.m_EC50).release())
  , m_EMax(std::make_unique<SEScalarFraction>(*obj.m_EMax).release())
{
}
//-----------------------------------------------------------------------------
SEPharmacodynamicModifier::SEPharmacodynamicModifier(SEPharmacodynamicModifier&& obj)
  : m_EC50(std::exchange(obj.m_EC50, std::make_unique<SEScalarMassPerVolume>().release()))
  , m_EMax(std::exchange(obj.m_EMax, std::make_unique<SEScalarFraction>().release()))
{
}
//-----------------------------------------------------------------------------

SEPharmacodynamicModifier& SEPharmacodynamicModifier::operator=(SEPharmacodynamicModifier const& rhs)
{
  if (this != &rhs) {
    auto temp = SEPharmacodynamicModifier(std::move(rhs));
    GetEC50() = std::move(temp.GetEC50());
    GetEMax() = std::move(temp.GetEMax());
  }
  return *this;
}
//-----------------------------------------------------------------------------
SEPharmacodynamicModifier& SEPharmacodynamicModifier::operator=(SEPharmacodynamicModifier&& rhs)
{
  if (this != &rhs) {
    auto temp = SEPharmacodynamicModifier(std::move(rhs));
    GetEC50() = std::move(rhs.GetEC50());
    GetEMax() = std::move(rhs.GetEMax());
  }
  return *this;
}

//-----------------------------------------------------------------------------
SEPharmacodynamicModifier::~SEPharmacodynamicModifier()
{
  SAFE_DELETE(m_EMax);
  SAFE_DELETE(m_EC50);
}
//-----------------------------------------------------------------------------
void SEPharmacodynamicModifier::Clear()
{
  if (m_EMax) {
    m_EMax->Clear();
  }
  if (m_EC50) {
    m_EC50->Clear();
  }
}
#pragma optimize("", off)
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
#pragma optimize("", off)
//-----------------------------------------------------------------------------
bool SEPharmacodynamicModifier::HasEMax() const
{
  return m_EMax ? m_EMax->IsValid() : false;
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
#pragma optimize("", off)
//-----------------------------------------------------------------------------
bool SEPharmacodynamicModifier::HasEC50() const
{
  return m_EC50 ? m_EC50->IsValid() : false;
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