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
#include <biogears/cdm/system/physiology/SEDrugSystem.h>

#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarLength.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/container/Tree.tci.h>

namespace biogears {
constexpr char idAntibioticActivity[] = "AntibioticActivity";
constexpr char idBronchodilationLevel[] = "BronchodilationLevel";
constexpr char idFeverChange[] = "FeverChange";
constexpr char idHeartRateChange[] = "HeartRateChange";
constexpr char idHemorrhageChange[] = "HemorrhageChange";
constexpr char idMeanBloodPressureChange[] = "MeanBloodPressureChange";
constexpr char idNeuromuscularBlockLevel[] = "NeuromuscularBlockLevel";
constexpr char idPainToleranceChange[] = "PainToleranceChange";
constexpr char idPulsePressureChange[] = "PulsePressureChange";
constexpr char idRespirationRateChange[] = "RespirationRateChange";
constexpr char idSedationLevel[] = "SedationLevel";
constexpr char idTidalVolumeChange[] = "TidalVolumeChange";
constexpr char idTubularPermeabilityChange[] = "TubularPermeabilityChange";
constexpr char idCentralNervousResponse[] = "CentralNervousResponse";
constexpr char idPupillaryResponse[] = "PupillaryResponse";

SEDrugSystem::SEDrugSystem(Logger* logger)
  : SESystem(logger)
{
  m_AntibioticActivity = nullptr;
  m_BronchodilationLevel = nullptr;
  m_FeverChange = nullptr;
  m_HeartRateChange = nullptr;
  m_HemorrhageChange = nullptr;
  m_MeanBloodPressureChange = nullptr;
  m_NeuromuscularBlockLevel = nullptr;
  m_PainToleranceChange = nullptr;
  m_PulsePressureChange = nullptr;
  m_RespirationRateChange = nullptr;
  m_SedationLevel = nullptr;
  m_TidalVolumeChange = nullptr;
  m_TubularPermeabilityChange = nullptr;
  m_CentralNervousResponse = nullptr;
}
//-------------------------------------------------------------------------------

SEDrugSystem::~SEDrugSystem()
{
  Clear();
}
//-------------------------------------------------------------------------------

void SEDrugSystem::Clear()
{
  SESystem::Clear();

  SAFE_DELETE(m_AntibioticActivity);
  SAFE_DELETE(m_BronchodilationLevel);
  SAFE_DELETE(m_FeverChange);
  SAFE_DELETE(m_HeartRateChange);
  SAFE_DELETE(m_HemorrhageChange);
  SAFE_DELETE(m_MeanBloodPressureChange);
  SAFE_DELETE(m_NeuromuscularBlockLevel);
  SAFE_DELETE(m_PainToleranceChange);
  SAFE_DELETE(m_PulsePressureChange);
  SAFE_DELETE(m_RespirationRateChange);
  SAFE_DELETE(m_SedationLevel);
  SAFE_DELETE(m_TidalVolumeChange);
  SAFE_DELETE(m_TubularPermeabilityChange);
  SAFE_DELETE(m_CentralNervousResponse);
}
//-------------------------------------------------------------------------------

bool SEDrugSystem::Load(const CDM::DrugSystemData& in)
{
  SESystem::Load(in);

  if (in.AntibioticActivity().present())
    GetAntibioticActivity().Load(in.AntibioticActivity().get());
  if (in.BronchodilationLevel().present())
    GetBronchodilationLevel().Load(in.BronchodilationLevel().get());
  if (in.FeverChange().present())
    GetFeverChange().Load(in.FeverChange().get());
  if (in.HeartRateChange().present())
    GetHeartRateChange().Load(in.HeartRateChange().get());
  if (in.HemorrhageChange().present())
    GetHemorrhageChange().Load(in.HemorrhageChange().get());
  if (in.MeanBloodPressureChange().present())
      GetMeanBloodPressureChange().Load(in.MeanBloodPressureChange().get());
  if (in.NeuromuscularBlockLevel().present())
    GetNeuromuscularBlockLevel().Load(in.NeuromuscularBlockLevel().get());
  if (in.PainToleranceChange().present())
    GetPainToleranceChange().Load(in.PainToleranceChange().get());
  if (in.PulsePressureChange().present())
    GetPulsePressureChange().Load(in.PulsePressureChange().get());
  if (in.RespirationRateChange().present())
    GetRespirationRateChange().Load(in.RespirationRateChange().get());
  if (in.SedationLevel().present())
    GetSedationLevel().Load(in.SedationLevel().get());
  if (in.TidalVolumeChange().present())
    GetTidalVolumeChange().Load(in.TidalVolumeChange().get());
  if (in.TubularPermeabilityChange().present())
    GetTubularPermeabilityChange().Load(in.TubularPermeabilityChange().get());
  if (in.CentralNervousResponse().present())
    GetCentralNervousResponse().Load(in.CentralNervousResponse().get());

  return true;
}
//-------------------------------------------------------------------------------
const SEScalar* SEDrugSystem::GetScalar(const char* name)
{
  return GetScalar(std::string{ name });
}
//-------------------------------------------------------------------------------
const SEScalar* SEDrugSystem::GetScalar(const std::string& name)
{
  if (name == idAntibioticActivity)
    return &GetAntibioticActivity();
  if (name == idBronchodilationLevel)
    return &GetBronchodilationLevel();
  if (name == idFeverChange)
    return &GetFeverChange();
  if (name == idHeartRateChange)
    return &GetHeartRateChange();
  if (name == idHemorrhageChange)
    return &GetHemorrhageChange();
  if (name == idMeanBloodPressureChange)
    return &GetMeanBloodPressureChange();
  if (name == idNeuromuscularBlockLevel)
    return &GetNeuromuscularBlockLevel();
  if (name == idPainToleranceChange)
    return &GetPainToleranceChange();
  if (name == idPulsePressureChange)
    return &GetPulsePressureChange();
  if (name == idRespirationRateChange)
    return &GetRespirationRateChange();
  if (name == idSedationLevel)
    return &GetSedationLevel();
  if (name == idTidalVolumeChange)
    return &GetTidalVolumeChange();
  if (name == idTubularPermeabilityChange)
    return &GetTubularPermeabilityChange();
  if (name == idCentralNervousResponse)
    return &GetCentralNervousResponse();

  return nullptr;
}
//-------------------------------------------------------------------------------

CDM::DrugSystemData* SEDrugSystem::Unload() const
{
  CDM::DrugSystemData* data = new CDM::DrugSystemData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------

void SEDrugSystem::Unload(CDM::DrugSystemData& data) const
{
  SESystem::Unload(data);

  if (m_AntibioticActivity != nullptr)
    data.AntibioticActivity(std::unique_ptr<CDM::ScalarData>(m_AntibioticActivity->Unload()));
  if (m_BronchodilationLevel != nullptr)
    data.BronchodilationLevel(std::unique_ptr<CDM::ScalarFractionData>(m_BronchodilationLevel->Unload()));
  if (m_FeverChange != nullptr)
    data.FeverChange(std::unique_ptr<CDM::ScalarTemperatureData>(m_FeverChange->Unload()));
  if (m_HeartRateChange != nullptr)
    data.HeartRateChange(std::unique_ptr<CDM::ScalarFrequencyData>(m_HeartRateChange->Unload()));
  if (m_HemorrhageChange != nullptr)
    data.HemorrhageChange(std::unique_ptr<CDM::ScalarFractionData>(m_HemorrhageChange->Unload()));
  if (m_MeanBloodPressureChange != nullptr)
    data.MeanBloodPressureChange(std::unique_ptr<CDM::ScalarPressureData>(m_MeanBloodPressureChange->Unload()));
  if (m_NeuromuscularBlockLevel != nullptr)
    data.NeuromuscularBlockLevel(std::unique_ptr<CDM::ScalarFractionData>(m_NeuromuscularBlockLevel->Unload()));
  if (m_PainToleranceChange != nullptr)
    data.PainToleranceChange(std::unique_ptr<CDM::ScalarFractionData>(m_PainToleranceChange->Unload()));
  if (m_PulsePressureChange != nullptr)
    data.PulsePressureChange(std::unique_ptr<CDM::ScalarPressureData>(m_PulsePressureChange->Unload()));
  if (m_RespirationRateChange != nullptr)
    data.RespirationRateChange(std::unique_ptr<CDM::ScalarFrequencyData>(m_RespirationRateChange->Unload()));
  if (m_SedationLevel != nullptr)
    data.SedationLevel(std::unique_ptr<CDM::ScalarFractionData>(m_SedationLevel->Unload()));
  if (m_TidalVolumeChange != nullptr)
    data.TidalVolumeChange(std::unique_ptr<CDM::ScalarVolumeData>(m_TidalVolumeChange->Unload()));
  if (m_TubularPermeabilityChange != nullptr)
    data.TubularPermeabilityChange(std::unique_ptr<CDM::ScalarFractionData>(m_TubularPermeabilityChange->Unload()));
  if (m_CentralNervousResponse != nullptr)
    data.CentralNervousResponse(std::unique_ptr<CDM::ScalarFractionData>(m_CentralNervousResponse->Unload()));
}
//-------------------------------------------------------------------------------

bool SEDrugSystem::HasAntibioticActivity() const
{
  return m_AntibioticActivity == nullptr ? false : m_AntibioticActivity->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEDrugSystem::GetAntibioticActivity()
{
  if (m_AntibioticActivity == nullptr)
    m_AntibioticActivity = new SEScalar();
  return *m_AntibioticActivity;
}
//-------------------------------------------------------------------------------
double SEDrugSystem::GetAntibioticActivity() const
{
  if (m_AntibioticActivity == nullptr)
    return SEScalar::dNaN();
  return m_AntibioticActivity->GetValue();
}
//-------------------------------------------------------------------------------

bool SEDrugSystem::HasBronchodilationLevel() const
{
  return m_BronchodilationLevel == nullptr ? false : m_BronchodilationLevel->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFraction& SEDrugSystem::GetBronchodilationLevel()
{
  if (m_BronchodilationLevel == nullptr)
    m_BronchodilationLevel = new SEScalarFraction();
  return *m_BronchodilationLevel;
}
//-------------------------------------------------------------------------------
double SEDrugSystem::GetBronchodilationLevel() const
{
  if (m_BronchodilationLevel == nullptr)
    return SEScalar::dNaN();
  return m_BronchodilationLevel->GetValue();
}
//-------------------------------------------------------------------------------
bool SEDrugSystem::HasFeverChange() const
{
  return m_FeverChange == nullptr ? false : m_FeverChange->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarTemperature& SEDrugSystem::GetFeverChange()
{
  if (m_FeverChange == nullptr)
    m_FeverChange = new SEScalarTemperature();
  return *m_FeverChange;
}
//-------------------------------------------------------------------------------
double SEDrugSystem::GetFeverChange(const TemperatureUnit& unit) const
{
  if (m_FeverChange == nullptr)
    return SEScalar::dNaN();
  return m_FeverChange->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEDrugSystem::HasHeartRateChange() const
{
  return m_HeartRateChange == nullptr ? false : m_HeartRateChange->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFrequency& SEDrugSystem::GetHeartRateChange()
{
  if (m_HeartRateChange == nullptr)
    m_HeartRateChange = new SEScalarFrequency();
  return *m_HeartRateChange;
}
//-------------------------------------------------------------------------------
double SEDrugSystem::GetHeartRateChange(const FrequencyUnit& unit) const
{
  if (m_HeartRateChange == nullptr)
    return SEScalar::dNaN();
  return m_HeartRateChange->GetValue(unit);
}

bool SEDrugSystem::HasHemorrhageChange() const
{
  return m_HemorrhageChange == nullptr ? false : m_HemorrhageChange->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFraction& SEDrugSystem::GetHemorrhageChange()
{
  if (m_HemorrhageChange == nullptr)
    m_HemorrhageChange = new SEScalarFraction();
  return *m_HemorrhageChange;
}
//-------------------------------------------------------------------------------
double SEDrugSystem::GetHemorrhageChange() const
{
  if (m_HemorrhageChange == nullptr)
    return SEScalar::dNaN();
  return m_HemorrhageChange->GetValue();
}

bool SEDrugSystem::HasMeanBloodPressureChange() const
{
  return m_MeanBloodPressureChange == nullptr ? false : m_MeanBloodPressureChange->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SEDrugSystem::GetMeanBloodPressureChange()
{
  if (m_MeanBloodPressureChange == nullptr)
    m_MeanBloodPressureChange = new SEScalarPressure();
  return *m_MeanBloodPressureChange;
}
//-------------------------------------------------------------------------------
double SEDrugSystem::GetMeanBloodPressureChange(const PressureUnit& unit) const
{
  if (m_MeanBloodPressureChange == nullptr)
    return SEScalar::dNaN();
  return m_MeanBloodPressureChange->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEDrugSystem::HasNeuromuscularBlockLevel() const
{
  return m_NeuromuscularBlockLevel == nullptr ? false : m_NeuromuscularBlockLevel->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFraction& SEDrugSystem::GetNeuromuscularBlockLevel()
{
  if (m_NeuromuscularBlockLevel == nullptr)
    m_NeuromuscularBlockLevel = new SEScalarFraction();
  return *m_NeuromuscularBlockLevel;
}
//-------------------------------------------------------------------------------
double SEDrugSystem::GetNeuromuscularBlockLevel() const
{
  if (m_NeuromuscularBlockLevel == nullptr)
    return SEScalar::dNaN();
  return m_NeuromuscularBlockLevel->GetValue();
}
//-------------------------------------------------------------------------------

bool SEDrugSystem::HasPainToleranceChange() const
{
  return m_PainToleranceChange == nullptr ? false : m_PainToleranceChange->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFraction& SEDrugSystem::GetPainToleranceChange()
{
  if (m_PainToleranceChange == nullptr)
    m_PainToleranceChange = new SEScalarFraction();
  return *m_PainToleranceChange;
}
//-------------------------------------------------------------------------------
double SEDrugSystem::GetPainToleranceChange() const
{
  if (m_PainToleranceChange == nullptr)
    return SEScalar::dNaN();
  return m_PainToleranceChange->GetValue();
}
//-------------------------------------------------------------------------------

bool SEDrugSystem::HasPulsePressureChange() const
{
  return m_PulsePressureChange == nullptr ? false : m_PulsePressureChange->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SEDrugSystem::GetPulsePressureChange()
{
  if (m_PulsePressureChange == nullptr)
    m_PulsePressureChange = new SEScalarPressure();
  return *m_PulsePressureChange;
}
//-------------------------------------------------------------------------------
double SEDrugSystem::GetPulsePressureChange(const PressureUnit& unit) const
{
  if (m_PulsePressureChange == nullptr)
    return SEScalar::dNaN();
  return m_PulsePressureChange->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEDrugSystem::HasRespirationRateChange() const
{
  return m_RespirationRateChange == nullptr ? false : m_RespirationRateChange->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFrequency& SEDrugSystem::GetRespirationRateChange()
{
  if (m_RespirationRateChange == nullptr)
    m_RespirationRateChange = new SEScalarFrequency();
  return *m_RespirationRateChange;
}
//-------------------------------------------------------------------------------
double SEDrugSystem::GetRespirationRateChange(const FrequencyUnit& unit) const
{
  if (m_RespirationRateChange == nullptr)
    return SEScalar::dNaN();
  return m_RespirationRateChange->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEDrugSystem::HasSedationLevel() const
{
  return m_SedationLevel == nullptr ? false : m_SedationLevel->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFraction& SEDrugSystem::GetSedationLevel()
{
  if (m_SedationLevel == nullptr)
    m_SedationLevel = new SEScalarFraction();
  return *m_SedationLevel;
}
//-------------------------------------------------------------------------------
double SEDrugSystem::GetSedationLevel() const
{
  if (m_SedationLevel == nullptr)
    return SEScalar::dNaN();
  return m_SedationLevel->GetValue();
}
//-------------------------------------------------------------------------------

bool SEDrugSystem::HasTidalVolumeChange() const
{
  return m_TidalVolumeChange == nullptr ? false : m_TidalVolumeChange->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolume& SEDrugSystem::GetTidalVolumeChange()
{
  if (m_TidalVolumeChange == nullptr)
    m_TidalVolumeChange = new SEScalarVolume();
  return *m_TidalVolumeChange;
}
//-------------------------------------------------------------------------------
double SEDrugSystem::GetTidalVolumeChange(const VolumeUnit& unit) const
{
  if (m_TidalVolumeChange == nullptr)
    return SEScalar::dNaN();
  return m_TidalVolumeChange->GetValue(unit);
} 
//-------------------------------------------------------------------------------
bool SEDrugSystem::HasTubularPermeabilityChange() const
{
  return m_TubularPermeabilityChange == nullptr ? false : m_TubularPermeabilityChange->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFraction& SEDrugSystem::GetTubularPermeabilityChange()
{
  if (m_TubularPermeabilityChange == nullptr)
    m_TubularPermeabilityChange = new SEScalarFraction();
  return *m_TubularPermeabilityChange;
}
//-------------------------------------------------------------------------------
double SEDrugSystem::GetTubularPermeabilityChange() const
{
  if (m_TubularPermeabilityChange == nullptr)
    return SEScalar::dNaN();
  return m_TubularPermeabilityChange->GetValue();
}
//-------------------------------------------------------------------------------

bool SEDrugSystem::HasCentralNervousResponse() const
{
  return m_CentralNervousResponse == nullptr ? false : m_CentralNervousResponse->IsValid();
}
//-------------------------------------------------------------------------------

SEScalarFraction& SEDrugSystem::GetCentralNervousResponse()
{
  if (m_CentralNervousResponse == nullptr)
    m_CentralNervousResponse = new SEScalarFraction();
  return *m_CentralNervousResponse;
}
//-------------------------------------------------------------------------------
double SEDrugSystem::GetCentralNervousResponse() const
{
  if (m_CentralNervousResponse == nullptr)
    return SEScalar::dNaN();
  return m_CentralNervousResponse->GetValue();
}
//-------------------------------------------------------------------------------
Tree<const char*> SEDrugSystem::GetPhysiologyRequestGraph() const
{
  return Tree<const char*>{ classname() }
    .emplace_back(idBronchodilationLevel)
    .emplace_back(idFeverChange)
    .emplace_back(idHeartRateChange)
    .emplace_back(idHemorrhageChange)
    .emplace_back(idMeanBloodPressureChange)
    .emplace_back(idNeuromuscularBlockLevel)
    .emplace_back(idPainToleranceChange)
    .emplace_back(idPulsePressureChange)
    .emplace_back(idRespirationRateChange)
    .emplace_back(idSedationLevel)
    .emplace_back(idTidalVolumeChange)
    .emplace_back(idTubularPermeabilityChange)
    .emplace_back(idCentralNervousResponse);
}
}
