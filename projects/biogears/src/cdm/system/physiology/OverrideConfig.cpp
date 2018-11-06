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

#include <biogears/cdm/system/physiology/OverrideConfig.h>

#include <biogears/schema/cdm/Properties.hxx>
#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/engine/PhysiologyEngineConfiguration.h>
//#include <biogears/engine/Controller/BioGearsConfiguration.h>

#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarVolume.h>

namespace biogears {

// Work in Progress
OverrideConfig::OverrideConfig()
{
  m_overrideMode = CDM::enumOnOff::Off;
  m_ArterialPHOverride = nullptr;
  m_VenousPHOverride = nullptr;
  m_CardiacOutputOverride = nullptr;
  m_MeanArterialPressureOverride = nullptr;
  m_HeartRateOverride = nullptr;
  m_CoreTemperatureOverride = nullptr;
  m_SkinTemperatureOverride = nullptr;
  m_TotalMetabolicOverride = nullptr;
  m_UrineProductionOverride = nullptr;
  m_RespirationRateOverride = nullptr;
  m_TidalVolumeOverride = nullptr;
}

OverrideConfig::~OverrideConfig()
{
  Clear();
}

void OverrideConfig::Clear()
{
  /* Check this function */
  SAFE_DELETE(m_ArterialPHOverride);
  SAFE_DELETE(m_VenousPHOverride);
  SAFE_DELETE(m_CardiacOutputOverride);
  SAFE_DELETE(m_MeanArterialPressureOverride);
  SAFE_DELETE(m_HeartRateOverride);
  SAFE_DELETE(m_CoreTemperatureOverride);
  SAFE_DELETE(m_SkinTemperatureOverride);
  SAFE_DELETE(m_TotalMetabolicOverride);
  SAFE_DELETE(m_UrineProductionOverride);
  SAFE_DELETE(m_RespirationRateOverride);
  SAFE_DELETE(m_TidalVolumeOverride);
  m_overrideMode = CDM::enumOnOff::Off;
}

bool OverrideConfig::LoadOverride(const std::string& file)
{
  Clear(); // Reset only if we are not merging

  std::stringstream sst;
  if (file.empty()) {
    sst << "Override file not provided: " << file << std::endl;
    Error(sst);
    return false;
  }

  std::unique_ptr<CDM::ObjectData> data = Serializer::ReadFile(file, GetLogger());
  CDM::OverrideConfigData* pData = dynamic_cast<CDM::OverrideConfigData*>(data.get());
  if (pData == nullptr) {
    sst << "Override data file could not be read : " << file << std::endl;
    Error(sst);
    return false;
  }
  if (!Load(*pData)) {
    sst << "Unable to load override file: " << file << std::endl;
    Error(sst);
    return false;
  }
  if (!pData->BloodChemistryOverride() && !pData->CardiovascularOverride() && !pData->EnergyOverride() && !pData->RenalOverride() && !pData->RespiratoryOverride()) {
    return false;
  }

  m_overrideMode = CDM::enumOnOff::On;
  Load(*pData);
  return true;
}

//Need to finish
bool OverrideConfig::Load(const CDM::OverrideConfigData& in)
{
  if (in.BloodChemistryOverride().present()) {
    const CDM::BloodChemistryOverrideData& config = in.BloodChemistryOverride().get();
    if (config.EnableBloodChemistryOverride().present())
      EnableBloodChemistryOverride(config.EnableBloodChemistryOverride().get());
    if (config.ArterialBloodPHOverride().present())
      GetArterialPHOverride().Load(config.ArterialBloodPHOverride().get());
    if (config.VenousBloodPHOverride().present())
      GetVenousPHOverride().Load(config.VenousBloodPHOverride().get());
  }
  if (in.CardiovascularOverride().present()) {
    const CDM::CardiovascularOverrideData& config = in.CardiovascularOverride().get();
    if (config.EnableCardiovascularOverride().present())
      EnableCardiovascularOverride(config.EnableCardiovascularOverride().get());
    if (config.CardiacOutputOverride().present())
      GetCardiacOutputOverride().Load(config.CardiacOutputOverride().get());
    if (config.MeanArterialPressureOverride().present())
      GetMeanArterialPressureOverride().Load(config.MeanArterialPressureOverride().get());
    if (config.HeartRateOverride().present())
      GetHeartRateOverride().Load(config.HeartRateOverride().get());
  }
  if (in.EnergyOverride().present()) {
    const CDM::EnergyOverrideData& config = in.EnergyOverride().get();
    if (config.EnableEnergyOverride().present())
      EnableEnergyOverride(config.EnableEnergyOverride().get());
    if (config.CoreTemperatureOverride().present())
      GetCoreTemperatureOverride().Load(config.CoreTemperatureOverride().get());
    if (config.SkinTemperatureOverride().present())
      GetSkinTemperatureOverride().Load(config.SkinTemperatureOverride().get());
    if (config.TotalMetabolicRateOverride().present())
      GetTotalMetabolicOverride().Load(config.TotalMetabolicRateOverride().get());
  }
  if (in.RenalOverride().present()) {
    const CDM::RenalOverrideData& config = in.RenalOverride().get();
    if (config.EnableRenalOverride().present())
      EnableRenalOverride(config.EnableRenalOverride().get());
    if (config.UrineProductionRateOverride().present())
      GetUrineProductionOverride().Load(config.UrineProductionRateOverride().get());
  }
  if (in.RespiratoryOverride().present()) {
    const CDM::RespiratoryOverrideData& config = in.RespiratoryOverride().get();
    if (config.EnableRespiratoryOverride().present())
      EnableRespiratoryOverride(config.EnableRespiratoryOverride().get());
    if (config.RespirationRateOverride().present())
      GetRespirationRateOverride().Load(config.RespirationRateOverride().get());
    if (config.TidalVolumeOverride().present())
      GetTidalVolumeOverride().Load(config.TidalVolumeOverride().get());
    }

  return true;
}

CDM::OverrideConfigData* OverrideConfig::Unload() const
{
  CDM::OverrideConfigData* data(new CDM::OverrideConfigData());
  Unload(*data);
  return data;
}

void OverrideConfig::Unload(CDM::OverrideConfigData& data) const
{
  CDM::BloodChemistryOverrideData* bloodchem(new CDM::BloodChemistryOverrideData());
  if (HasArterialPHOverride())
    bloodchem->ArterialBloodPHOverride(std::unique_ptr<CDM::ScalarData>(m_ArterialPHOverride->Unload()));
  if (HasVenousPHOverride())
    bloodchem->VenousBloodPHOverride(std::unique_ptr<CDM::ScalarData>(m_VenousPHOverride->Unload()));

  CDM::BloodChemistryOverrideData* bloodchemoverride(new CDM::BloodChemistryOverrideData());
  if (HasEnableBloodChemistryOverride())
    bloodchemoverride->EnableBloodChemistryOverride(m_overrideMode);
  data.BloodChemistryOverride(std::unique_ptr<CDM::BloodChemistryOverrideData>(bloodchemoverride));

  CDM::CardiovascularOverrideData* cardio(new CDM::CardiovascularOverrideData());
  if (HasCardiacOutputOverride())
    cardio->CardiacOutputOverride(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_CardiacOutputOverride->Unload()));
  if (HasMeanArterialPressureOverride())
    cardio->MeanArterialPressureOverride(std::unique_ptr<CDM::ScalarPressureData>(m_MeanArterialPressureOverride->Unload()));
  if (HasHeartRateOverride())
    cardio->HeartRateOverride(std::unique_ptr<CDM::ScalarFrequencyData>(m_HeartRateOverride->Unload()));

  CDM::CardiovascularOverrideData* cardiovascularoverride(new CDM::CardiovascularOverrideData());
  if (HasEnableCardiovascularOverride())
    cardiovascularoverride->EnableCardiovascularOverride(m_overrideMode);
  data.CardiovascularOverride(std::unique_ptr<CDM::CardiovascularOverrideData>(cardiovascularoverride));

  CDM::EnergyOverrideData* energy(new CDM::EnergyOverrideData());
  if (HasCoreTemperatureOverride())
    energy->CoreTemperatureOverride(std::unique_ptr<CDM::ScalarTemperatureData>(m_CoreTemperatureOverride->Unload()));
  if (HasSkinTemperatureOverride())
    energy->SkinTemperatureOverride(std::unique_ptr<CDM::ScalarTemperatureData>(m_SkinTemperatureOverride->Unload()));
  if (HasTotalMetabolicOverride())
    energy->TotalMetabolicRateOverride(std::unique_ptr<CDM::ScalarPowerData>(m_TotalMetabolicOverride->Unload()));

  CDM::EnergyOverrideData* energyoverride(new CDM::EnergyOverrideData());
  if (HasEnableEnergyOverride())
    energyoverride->EnableEnergyOverride(m_overrideMode);
  data.EnergyOverride(std::unique_ptr<CDM::EnergyOverrideData>(energyoverride));

  CDM::RenalOverrideData* renal(new CDM::RenalOverrideData());
  if (HasUrineProductionOverride())
    renal->UrineProductionRateOverride(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_UrineProductionOverride->Unload()));

  CDM::RenalOverrideData* renaloverride(new CDM::RenalOverrideData());
  if (HasEnableRenalOverride())
    renaloverride->EnableRenalOverride(m_overrideMode);
  data.RenalOverride(std::unique_ptr<CDM::RenalOverrideData>(renaloverride));

  CDM::RespiratoryOverrideData* respiratory(new CDM::RespiratoryOverrideData());
  if (HasRespirationRateOverride())
    respiratory->RespirationRateOverride(std::unique_ptr<CDM::ScalarFrequencyData>(m_RespirationRateOverride->Unload()));
  if (HasTidalVolumeOverride())
    respiratory->TidalVolumeOverride(std::unique_ptr<CDM::ScalarVolumeData>(m_TidalVolumeOverride->Unload()));

  CDM::RespiratoryOverrideData* respiratoryoverride(new CDM::RespiratoryOverrideData());
  if (HasEnableRespiratoryOverride())
    respiratoryoverride->EnableRespiratoryOverride(m_overrideMode);
  data.RespiratoryOverride(std::unique_ptr<CDM::RespiratoryOverrideData>(respiratoryoverride));
}

bool OverrideConfig::ReadOverrideParameters(const std::string& overrideParameterFile)
{
  CDM::OverrideConfigData* pData;
  std::unique_ptr<CDM::ObjectData> data;

  std::string opFile = overrideParameterFile;
  if (opFile.find("/override") == std::string::npos) {
    opFile = "./override/";
    opFile += overrideParameterFile;
  }

  data = Serializer::ReadFile(opFile, GetLogger());
  pData = dynamic_cast<CDM::OverrideConfigData*>(data.get());
  if (pData == nullptr) {
    std::stringstream ss;
    ss << "Override file could not be read : " << overrideParameterFile << std::endl;
    Error(ss);
    return false;
  }
  return Load(*pData);
}

//commenting for now
//bool OverrideConfig::HasOverride() const
//{
//  return m_MeanArterialPressureOverride == nullptr ? false : m_MeanArterialPressureOverride->IsValid();
//}
//
//SEScalarPressure& OverrideConfig::GetOverride()
//{
//  if (m_MeanArterialPressureOverride == nullptr)
//    m_MeanArterialPressureOverride = new SEScalarPressure();
//  return *m_MeanArterialPressureOverride;
//}
//
//double OverrideConfig::GetOverride(const PressureUnit& unit) const
//{
//  if (m_MeanArterialPressureOverride == nullptr)
//  {
//    return SEScalar::dNaN();
//  }
//  return m_MeanArterialPressureOverride->GetValue(unit);
//}

////////////////////
/** Blood Chemistry */
////////////////////

bool OverrideConfig::HasArterialPHOverride() const
{
  return m_ArterialPHOverride == nullptr ? false : m_ArterialPHOverride->IsValid();
}
SEScalar& OverrideConfig::GetArterialPHOverride()
{
  if (m_ArterialPHOverride == nullptr)
    m_ArterialPHOverride = new SEScalar();
  return *m_ArterialPHOverride;
}
double OverrideConfig::GetArterialPHOverride() const
{
  if (m_ArterialPHOverride == nullptr)
    return SEScalar::dNaN();
  return m_ArterialPHOverride->GetValue();
}
bool OverrideConfig::HasVenousPHOverride() const
{
  return m_VenousPHOverride == nullptr ? false : m_VenousPHOverride->IsValid();
}
SEScalar& OverrideConfig::GetVenousPHOverride()
{
  if (m_VenousPHOverride == nullptr)
    m_VenousPHOverride = new SEScalar();
  return *m_VenousPHOverride;
}
double OverrideConfig::GetVenousPHOverride() const
{
  if (m_VenousPHOverride == nullptr)
    return SEScalar::dNaN();
  return m_VenousPHOverride->GetValue();
}

////////////////////
/** CardioVascular */
////////////////////

  bool OverrideConfig::HasCardiacOutputOverride() const
{
    return m_CardiacOutputOverride == nullptr ? false : m_CardiacOutputOverride->IsValid();
  }
SEScalarVolumePerTime& OverrideConfig::GetCardiacOutputOverride()
{
  if (m_CardiacOutputOverride == nullptr)
    m_CardiacOutputOverride = new SEScalarVolumePerTime();
  return *m_CardiacOutputOverride;
}
double OverrideConfig::GetCardiacOutputOverride(const VolumePerTimeUnit& unit) const
{
  if (m_CardiacOutputOverride == nullptr)
    return SEScalar::dNaN();
  return m_CardiacOutputOverride->GetValue(unit);
}
bool OverrideConfig::HasMeanArterialPressureOverride() const
{
  return m_MeanArterialPressureOverride == nullptr ? false : m_MeanArterialPressureOverride->IsValid();
}
SEScalarPressure& OverrideConfig::GetMeanArterialPressureOverride()
{
  if (m_MeanArterialPressureOverride == nullptr)
    m_MeanArterialPressureOverride = new SEScalarPressure();
  return *m_MeanArterialPressureOverride;
}
double OverrideConfig::GetMeanArterialPressureOverride(const PressureUnit& unit) const
{
  if (m_MeanArterialPressureOverride == nullptr)
    return SEScalar::dNaN();
  return m_MeanArterialPressureOverride->GetValue(unit);
}
bool OverrideConfig::HasHeartRateOverride() const
{
  return m_HeartRateOverride == nullptr ? false : m_HeartRateOverride->IsValid();
}
SEScalarFrequency& OverrideConfig::GetHeartRateOverride()
{
  if (m_HeartRateOverride == nullptr)
    m_HeartRateOverride = new SEScalarFrequency();
  return *m_HeartRateOverride;
}
double OverrideConfig::GetHeartRateOverride(const FrequencyUnit& unit) const
{
  if (m_HeartRateOverride == nullptr)
    return SEScalar::dNaN();
  return m_HeartRateOverride->GetValue(unit);
}


////////////////////
/** Energy */
////////////////////

bool OverrideConfig::HasCoreTemperatureOverride() const
{
  return m_CoreTemperatureOverride == nullptr ? false : m_CoreTemperatureOverride->IsValid();
}
SEScalarTemperature& OverrideConfig::GetCoreTemperatureOverride()
{
  if (m_CoreTemperatureOverride == nullptr)
    m_CoreTemperatureOverride = new SEScalarTemperature();
  return *m_CoreTemperatureOverride;
}
double OverrideConfig::GetCoreTemperatureOverride(const TemperatureUnit& unit) const
{
  if (m_CoreTemperatureOverride == nullptr)
    return SEScalar::dNaN();
  return m_CoreTemperatureOverride->GetValue(unit);
}
bool OverrideConfig::HasSkinTemperatureOverride() const
{
  return m_SkinTemperatureOverride == nullptr ? false : m_SkinTemperatureOverride->IsValid();
}
SEScalarTemperature& OverrideConfig::GetSkinTemperatureOverride()
{
  if (m_SkinTemperatureOverride == nullptr)
    m_SkinTemperatureOverride = new SEScalarTemperature();
  return *m_SkinTemperatureOverride;
}
double OverrideConfig::GetSkinTemperatureOverride(const TemperatureUnit& unit) const
{
  if (m_SkinTemperatureOverride == nullptr)
    return SEScalar::dNaN();
  return m_SkinTemperatureOverride->GetValue(unit);
}
  bool OverrideConfig::HasTotalMetabolicOverride() const
{
    return m_TotalMetabolicOverride == nullptr ? false : m_TotalMetabolicOverride->IsValid();
  }
SEScalarPower& OverrideConfig::GetTotalMetabolicOverride()
{
  if (m_TotalMetabolicOverride == nullptr)
    m_TotalMetabolicOverride = new SEScalarPower();
  return *m_TotalMetabolicOverride;
}
double OverrideConfig::GetTotalMetabolicOverride(const PowerUnit& unit) const
{
  if (m_TotalMetabolicOverride == nullptr)
    return SEScalar::dNaN();
  return m_TotalMetabolicOverride->GetValue(unit);
}

////////////////////
/** Renal */
////////////////////
bool OverrideConfig::HasUrineProductionOverride() const
{
  return m_UrineProductionOverride == nullptr ? false : m_UrineProductionOverride->IsValid();
}
SEScalarVolumePerTime& OverrideConfig::GetUrineProductionOverride()
{
  if (m_UrineProductionOverride == nullptr)
    m_UrineProductionOverride = new SEScalarVolumePerTime();
  return *m_UrineProductionOverride;
}
double OverrideConfig::GetUrineProductionOverride(const VolumePerTimeUnit& unit) const
{
  if (m_UrineProductionOverride == nullptr)
    return SEScalar::dNaN();
  return m_UrineProductionOverride->GetValue(unit);
}

////////////////////
/** Respiratory */
////////////////////

bool OverrideConfig::HasRespirationRateOverride() const
{
  return m_RespirationRateOverride == nullptr ? false : m_RespirationRateOverride->IsValid();
}
SEScalarFrequency& OverrideConfig::GetRespirationRateOverride()
{
  if (m_RespirationRateOverride == nullptr)
    m_RespirationRateOverride = new SEScalarFrequency();
  return *m_RespirationRateOverride;
}
double OverrideConfig::GetRespirationRateOverride(const FrequencyUnit& unit) const
{
  if (m_RespirationRateOverride == nullptr)
    return SEScalar::dNaN();
  return m_RespirationRateOverride->GetValue(unit);
}
bool OverrideConfig::HasTidalVolumeOverride() const
{
  return m_TidalVolumeOverride == nullptr ? false : m_TidalVolumeOverride->IsValid();
}
SEScalarVolume& OverrideConfig::GetTidalVolumeOverride()
{
  if (m_TidalVolumeOverride == nullptr)
    m_TidalVolumeOverride = new SEScalarVolume();
  return *m_TidalVolumeOverride;
}
double OverrideConfig::GetTidalVolumeOverride(const VolumeUnit& unit) const
{
  if (m_TidalVolumeOverride == nullptr)
    return SEScalar::dNaN();
  return m_TidalVolumeOverride->GetValue(unit);
}
}
