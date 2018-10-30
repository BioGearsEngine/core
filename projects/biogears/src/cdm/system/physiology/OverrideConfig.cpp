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

namespace biogears {

// Work in Progress
OverrideConfig::OverrideConfig()
{
  m_overrideMode = CDM::enumOnOff::Off;
  m_MeanArterialPressureOverride = nullptr;
  m_HeartRateOverride = nullptr;
  m_CoreTemperatureOverride = nullptr;
  m_SkinTemperatureOverride = nullptr;
  m_TotalMetabolicOverride = nullptr;
}

OverrideConfig::~OverrideConfig()
{
  Clear();
}

void OverrideConfig::Clear()
{
  /* Check this function */
  SAFE_DELETE(m_MeanArterialPressureOverride);
  SAFE_DELETE(m_HeartRateOverride);
  SAFE_DELETE(m_CoreTemperatureOverride);
  SAFE_DELETE(m_SkinTemperatureOverride);
  SAFE_DELETE(m_TotalMetabolicOverride);
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
  if (!pData->CardiovascularOverride() && !pData->EnergyOverride()) {
    return false;
  }

  m_overrideMode = CDM::enumOnOff::On;
  Load(*pData);
  return true;
}

//Need to finish
bool OverrideConfig::Load(const CDM::OverrideConfigData& in)
{
  if (in.CardiovascularOverride().present()) {
    const CDM::CardiovascularOverrideData& config = in.CardiovascularOverride().get();
    if (config.EnableCardiovascularOverride().present())
      EnableCardiovascularOverride(config.EnableCardiovascularOverride().get());
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

  CDM::CardiovascularOverrideData* cardio(new CDM::CardiovascularOverrideData());
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
/** CardioVascular */
////////////////////

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
}
