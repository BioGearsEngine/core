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

#include <biogears/cdm/patient/actions/SEOverride.h>

#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarOsmolality.h>


namespace biogears {
SEOverride::SEOverride()
  : SEPatientAction()
{
  m_OverrideSwitch = CDM::enumOnOff::Off;
  m_OverrideConformance = CDM::enumOnOff::On;
  m_ArterialPHOR = nullptr;
  m_VenousPHOR = nullptr;
  m_CardiacOutputOR = nullptr;
  m_PressureOR = nullptr;
  m_HeartRateOR = nullptr;
  m_AcheivedExerciseLevelOR = nullptr;
  m_CoreTemperatureOR = nullptr;
  m_CreatinineProductionRateOR = nullptr;
  m_ExerciseMeanArterialPressureDeltaOR = nullptr;
  m_FatigueLevelOR = nullptr;
  m_LactateProductionRateOR = nullptr;
  m_SkinTemperatureOR = nullptr;
  m_SweatRateOR = nullptr;
  m_TotalMetabolicOR = nullptr;
  m_TotalWorkRateLevelOR = nullptr;
  m_SodiumLostToSweatOR = nullptr;
  m_PotassiumLostToSweatOR = nullptr;
  m_ChlorideLostToSweatOR = nullptr;
  m_LAfferentArterioleResistOR = nullptr;
  m__LeftGlomerularFiltrationOR = nullptr;
  m_LReabsorptionRateOR = nullptr;
  m_RenalBloodFlowOR = nullptr;
  m_RenalPlasmaOR = nullptr;
  m_RAfferentArterioleResistOR = nullptr;
  m__RightGlomerularFiltrationOR = nullptr;
  m_RReabsorptionRateOR = nullptr;
  m_UrinationRateOR = nullptr;
  m_UrineProductionRateOR = nullptr;
  m_UrineOsmolalityOR = nullptr;
  m_UrineVolumeOR = nullptr;
  m_RespirationRateOR = nullptr;
  m_TidalVolumeOR = nullptr;
}

SEOverride::~SEOverride()
{
  Clear();
}

void SEOverride::Clear()
{
  SEPatientAction::Clear();
  m_OverrideSwitch = CDM::enumOnOff::Off;
  m_OverrideConformance = CDM::enumOnOff::On;
  SAFE_DELETE(m_ArterialPHOR);
  SAFE_DELETE(m_VenousPHOR);
  SAFE_DELETE(m_CardiacOutputOR);
  SAFE_DELETE(m_PressureOR);
  SAFE_DELETE(m_HeartRateOR);
  SAFE_DELETE(m_AcheivedExerciseLevelOR);
  SAFE_DELETE(m_CoreTemperatureOR);
  SAFE_DELETE(m_CreatinineProductionRateOR);
  SAFE_DELETE(m_ExerciseMeanArterialPressureDeltaOR);
  SAFE_DELETE(m_FatigueLevelOR);
  SAFE_DELETE(m_LactateProductionRateOR);
  SAFE_DELETE(m_SkinTemperatureOR);
  SAFE_DELETE(m_SweatRateOR);
  SAFE_DELETE(m_TotalMetabolicOR);
  SAFE_DELETE(m_TotalWorkRateLevelOR);
  SAFE_DELETE(m_SodiumLostToSweatOR);
  SAFE_DELETE(m_PotassiumLostToSweatOR);
  SAFE_DELETE(m_ChlorideLostToSweatOR);
  SAFE_DELETE(m_LAfferentArterioleResistOR);
  SAFE_DELETE(m__LeftGlomerularFiltrationOR);
  SAFE_DELETE(m_LReabsorptionRateOR);
  SAFE_DELETE(m_RenalBloodFlowOR);
  SAFE_DELETE(m_RenalPlasmaOR);
  SAFE_DELETE(m_RAfferentArterioleResistOR);
  SAFE_DELETE(m__RightGlomerularFiltrationOR);
  SAFE_DELETE(m_RReabsorptionRateOR);
  SAFE_DELETE(m_UrinationRateOR);
  SAFE_DELETE(m_UrineProductionRateOR);
  SAFE_DELETE(m_UrineOsmolalityOR);
  SAFE_DELETE(m_UrineVolumeOR);
  SAFE_DELETE(m_RespirationRateOR);
  SAFE_DELETE(m_TidalVolumeOR);
}

bool SEOverride::IsValid() const
{
  if (!HasOverrideSwitch()) {
    Error("Override must have state.");
    return false;
  }
  if (GetOverrideSwitch() == CDM::enumOnOff::On) {
    return true;
  } else {
    return false;
  }
}

bool SEOverride::IsActive() const
{
  return (GetOverrideSwitch() == CDM::enumOnOff::On);
}

bool SEOverride::Load(const CDM::OverrideData& in)
{
  SEPatientAction::Clear();
  SetOverrideSwitch(in.State());
  SetOverrideConformance(in.Conformant());
  if (in.ArterialBloodPHOverride().present()) {
    GetArterialPHOverride().Load(in.ArterialBloodPHOverride().get());
  } else {
    GetArterialPHOverride().Invalidate();
  }
  if (in.VenousBloodPHOverride().present()) {
    GetVenousPHOverride().Load(in.VenousBloodPHOverride().get());
  } else {
    GetVenousPHOverride().Invalidate();
  }
  if (in.CardiacOutputOverride().present()) {
    GetCardiacOutputOverride().Load(in.CardiacOutputOverride().get());
  } else {
    GetCardiacOutputOverride().Invalidate();
  }
  if (in.MeanArterialPressureOverride().present()) {
    GetMAPOverride().Load(in.MeanArterialPressureOverride().get());
  } else {
    GetMAPOverride().Invalidate();
  }
  if (in.HeartRateOverride().present()) {
    GetHeartRateOverride().Load(in.HeartRateOverride().get());
  } else {
    GetHeartRateOverride().Invalidate();
  }
  if (in.AchievedExerciseLevelOverride().present()) {
    GetAchievedExerciseLevelOverride().Load(in.AchievedExerciseLevelOverride().get());
  } else {
    GetAchievedExerciseLevelOverride().Invalidate();
  }
  if (in.CoreTemperatureOverride().present()) {
    GetCoreTemperatureOverride().Load(in.CoreTemperatureOverride().get());
  } else {
    GetCoreTemperatureOverride().Invalidate();
  }
  if (in.CreatinineProductionRateOverride().present()) {
    GetCreatinineProductionRateOverride().Load(in.CreatinineProductionRateOverride().get());
  } else {
    GetCreatinineProductionRateOverride().Invalidate();
  }
  if (in.ExerciseMeanArterialPressureDeltaOverride().present()) {
    GetExerciseMeanArterialPressureDeltaOverride().Load(in.ExerciseMeanArterialPressureDeltaOverride().get());
  } else {
    GetExerciseMeanArterialPressureDeltaOverride().Invalidate();
  }
  if (in.FatigueLevelOverride().present()) {
    GetFatigueLevelOverride().Load(in.FatigueLevelOverride().get());
  } else {
    GetFatigueLevelOverride().Invalidate();
  }
  if (in.LactateProductionRateOverride().present()) {
    GetLactateProductionRateOverride().Load(in.LactateProductionRateOverride().get());
  } else {
    GetLactateProductionRateOverride().Invalidate();
  }
  if (in.SkinTemperatureOverride().present()) {
    GetSkinTemperatureOverride().Load(in.SkinTemperatureOverride().get());
  } else {
    GetSkinTemperatureOverride().Invalidate();
  }
  if (in.SweatRateOverride().present()) {
    GetSweatRateOverride().Load(in.SweatRateOverride().get());
  } else {
    GetSweatRateOverride().Invalidate();
  }
  if (in.TotalMetabolicRateOverride().present()) {
    GetTotalMetabolicOverride().Load(in.TotalMetabolicRateOverride().get());
  } else {
    GetTotalMetabolicOverride().Invalidate();
  }
  if (in.TotalWorkRateLevelOverride().present()) {
    GetTotalWorkRateLevelOverride().Load(in.TotalWorkRateLevelOverride().get());
  } else {
    GetTotalWorkRateLevelOverride().Invalidate();
  }
  if (in.SodiumLostToSweatOverride().present()) {
    GetSodiumLostToSweatOverride().Load(in.SodiumLostToSweatOverride().get());
  } else {
    GetSodiumLostToSweatOverride().Invalidate();
  }
  if (in.PotassiumLostToSweatOverride().present()) {
    GetPotassiumLostToSweatOverride().Load(in.PotassiumLostToSweatOverride().get());
  } else {
    GetPotassiumLostToSweatOverride().Invalidate();
  }
  if (in.ChlorideLostToSweatOverride().present()) {
    GetChlorideLostToSweatOverride().Load(in.ChlorideLostToSweatOverride().get());
  } else {
    GetChlorideLostToSweatOverride().Invalidate();
  }
  if (in.LeftAfferentArterioleResistanceOverride().present()) {
    GetLeftAfferentArterioleResistanceOverride().Load(in.LeftAfferentArterioleResistanceOverride().get());
  } else {
    GetLeftAfferentArterioleResistanceOverride().Invalidate();
  }
  if (in.LeftGlomerularFiltrationRateOverride().present()) {
    GetLeftGlomerularFiltrationRateOverride().Load(in.LeftGlomerularFiltrationRateOverride().get());
  } else {
    GetLeftGlomerularFiltrationRateOverride().Invalidate();
  }
  if (in.LeftReabsorptionRateOverride().present()) {
    GetLeftReaborptionRateOverride().Load(in.LeftReabsorptionRateOverride().get());
  } else {
    GetLeftReaborptionRateOverride().Invalidate();
  }
  if (in.RenalBloodFlowOverride().present()) {
    GetRenalBloodFlowOverride().Load(in.RenalBloodFlowOverride().get());
  } else {
    GetRenalBloodFlowOverride().Invalidate();
  }
  if (in.RenalPlasmaFlowOverride().present()) {
    GetRenalPlasmaFlowOverride().Load(in.RenalPlasmaFlowOverride().get());
  } else {
    GetRenalPlasmaFlowOverride().Invalidate();
  }
  if (in.RightAfferentArterioleResistanceOverride().present()) {
    GetRightAfferentArterioleResistanceOverride().Load(in.RightAfferentArterioleResistanceOverride().get());
  } else {
    GetRightAfferentArterioleResistanceOverride().Invalidate();
  }
  if (in.RightGlomerularFiltrationRateOverride().present()) {
    GetRightGlomerularFiltrationRateOverride().Load(in.RightGlomerularFiltrationRateOverride().get());
  } else {
    GetRightGlomerularFiltrationRateOverride().Invalidate();
  }
  if (in.RightReabsorptionRateOverride().present()) {
    GetRightReaborptionRateOverride().Load(in.RightReabsorptionRateOverride().get());
  } else {
    GetRightReaborptionRateOverride().Invalidate();
  }
  if (in.UrinationRateOverride().present()) {
    GetUrinationRateOverride().Load(in.UrinationRateOverride().get());
  } else {
    GetUrinationRateOverride().Invalidate();
  }
  if (in.UrineProductionRateOverride().present()) {
    GetUrineProductionRateOverride().Load(in.UrineProductionRateOverride().get());
  } else {
    GetUrineProductionRateOverride().Invalidate();
  }
  if (in.UrineOsmolalityOverride().present()) {
    GetUrineOsmolalityOverride().Load(in.UrineOsmolalityOverride().get());
  } else {
    GetUrineOsmolalityOverride().Invalidate();
  }
  if (in.UrineVolumeOverride().present()) {
    GetUrineVolumeOverride().Load(in.UrineVolumeOverride().get());
  } else {
    GetUrineVolumeOverride().Invalidate();
  }
  if (in.RespirationRateOverride().present()) {
    GetRespirationRateOverride().Load(in.RespirationRateOverride().get());
  } else {
    GetRespirationRateOverride().Invalidate();
  }
  if (in.TidalVolumeOverride().present()) {
    GetTidalVolumeOverride().Load(in.TidalVolumeOverride().get());
  } else {
    GetTidalVolumeOverride().Invalidate();
  }
  //SEPatientAction::Load(in);
  //return true;
  return IsValid();
}

CDM::OverrideData* SEOverride::Unload() const
{
  CDM::OverrideData* data(new CDM::OverrideData());
  Unload(*data);
  return data;
}

void SEOverride::Unload(CDM::OverrideData& data) const
{
  SEPatientAction::Unload(data);
  if (HasOverrideSwitch()) {
    data.State(m_OverrideSwitch);
  }
  if (HasOverrideConformance()) {
    data.Conformant(m_OverrideConformance);
  }
  if (HasArterialPHOverride()) {
    data.ArterialBloodPHOverride(std::unique_ptr<CDM::ScalarData>(m_ArterialPHOR->Unload()));
  }
  if (HasVenousPHOverride()) {
    data.VenousBloodPHOverride(std::unique_ptr<CDM::ScalarData>(m_VenousPHOR->Unload()));
  }
  if (HasCardiacOutputOverride()) {
    data.CardiacOutputOverride(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_CardiacOutputOR->Unload()));
  }
  if (HasMAPOverride()) {
    data.MeanArterialPressureOverride(std::unique_ptr<CDM::ScalarPressureData>(m_PressureOR->Unload()));
  }
  if (HasHeartRateOverride()) {
    data.HeartRateOverride(std::unique_ptr<CDM::ScalarFrequencyData>(m_HeartRateOR->Unload()));
  }
  if (HasAchievedExerciseLevelOverride()) {
    data.AchievedExerciseLevelOverride(std::unique_ptr<CDM::ScalarFractionData>(m_AcheivedExerciseLevelOR->Unload()));
  }
  if (HasCoreTemperatureOverride()) {
    data.CoreTemperatureOverride(std::unique_ptr<CDM::ScalarTemperatureData>(m_CoreTemperatureOR->Unload()));
  }
  if (HasCreatinineProductionRateOverride()) {
    data.CreatinineProductionRateOverride(std::unique_ptr<CDM::ScalarAmountPerTimeData>(m_CreatinineProductionRateOR->Unload()));
  }
  if (HasExerciseMeanArterialPressureDeltaOverride()) {
    data.ExerciseMeanArterialPressureDeltaOverride(std::unique_ptr<CDM::ScalarPressureData>(m_ExerciseMeanArterialPressureDeltaOR->Unload()));
  }
  if (HasFatigueLevelOverride()) {
    data.FatigueLevelOverride(std::unique_ptr<CDM::ScalarFractionData>(m_FatigueLevelOR->Unload()));
  }
  if (HasLactateProductionRateOverride()) {
    data.LactateProductionRateOverride(std::unique_ptr<CDM::ScalarAmountPerTimeData>(m_LactateProductionRateOR->Unload()));
  }
  if (HasSkinTemperatureOverride()) {
    data.SkinTemperatureOverride(std::unique_ptr<CDM::ScalarTemperatureData>(m_SkinTemperatureOR->Unload()));
  }
  if (HasSweatRateOverride()) {
    data.SweatRateOverride(std::unique_ptr<CDM::ScalarMassPerTimeData>(m_SweatRateOR->Unload()));
  }
  if (HasTotalMetabolicOverride()) {
    data.TotalMetabolicRateOverride(std::unique_ptr<CDM::ScalarPowerData>(m_TotalMetabolicOR->Unload()));
  }
  if (HasTotalWorkRateLevelOverride()) {
    data.TotalWorkRateLevelOverride(std::unique_ptr<CDM::ScalarFractionData>(m_TotalWorkRateLevelOR->Unload()));
  }
  if (HasSodiumLostToSweatOverride()) {
    data.SodiumLostToSweatOverride(std::unique_ptr<CDM::ScalarMassData>(m_SodiumLostToSweatOR->Unload()));
  }
  if (HasPotassiumLostToSweatOverride()) {
    data.PotassiumLostToSweatOverride(std::unique_ptr<CDM::ScalarMassData>(m_PotassiumLostToSweatOR->Unload()));
  }
  if (HasChlorideLostToSweatOverride()) {
    data.ChlorideLostToSweatOverride(std::unique_ptr<CDM::ScalarMassData>(m_ChlorideLostToSweatOR->Unload()));
  }
  if (HasLeftAfferentArterioleResistanceOverride()) {
    data.LeftAfferentArterioleResistanceOverride(std::unique_ptr<CDM::ScalarFlowResistanceData>(m_LAfferentArterioleResistOR->Unload()));
  }
  if (HasLeftGlomerularFiltrationRateOverride()) {
    data.LeftGlomerularFiltrationRateOverride(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m__LeftGlomerularFiltrationOR->Unload()));
  }
  if (HasLeftReaborptionRateOverride()) {
    data.LeftReabsorptionRateOverride(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_LReabsorptionRateOR->Unload()));
  }
  if (HasRenalBloodFlowOverride()) {
    data.RenalBloodFlowOverride(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_RenalBloodFlowOR->Unload()));
  }
  if (HasRenalPlasmaFlowOverride()) {
    data.RenalPlasmaFlowOverride(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_RenalPlasmaOR->Unload()));
  }
  if (HasRightAfferentArterioleResistanceOverride()) {
    data.RightAfferentArterioleResistanceOverride(std::unique_ptr<CDM::ScalarFlowResistanceData>(m_RAfferentArterioleResistOR->Unload()));
  }
  if (HasRightGlomerularFiltrationRateOverride()) {
    data.RightGlomerularFiltrationRateOverride(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m__RightGlomerularFiltrationOR->Unload()));
  }
  if (HasRightReaborptionRateOverride()) {
    data.RightReabsorptionRateOverride(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_RReabsorptionRateOR->Unload()));
  }
  if (HasUrineProductionRateOverride()) {
    data.UrineProductionRateOverride(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_UrineProductionRateOR->Unload()));
  }
  if (HasUrineOsmolalityOverride()) {
    data.UrineOsmolalityOverride(std::unique_ptr<CDM::ScalarOsmolalityData>(m_UrineOsmolalityOR->Unload()));
  }
  if (HasUrineVolumeOverride()) {
    data.UrineVolumeOverride(std::unique_ptr<CDM::ScalarVolumeData>(m_UrineVolumeOR->Unload()));
  }
  if (HasRespirationRateOverride()) {
    data.RespirationRateOverride(std::unique_ptr<CDM::ScalarFrequencyData>(m_RespirationRateOR->Unload()));
  }
  if (HasTidalVolumeOverride()) {
    data.TidalVolumeOverride(std::unique_ptr<CDM::ScalarVolumeData>(m_TidalVolumeOR->Unload()));
  }
}

CDM::enumOnOff::value SEOverride::GetOverrideSwitch() const
{
  return m_OverrideSwitch;
}
void SEOverride::SetOverrideSwitch(CDM::enumOnOff::value state)
{
  m_OverrideSwitch = state;
}
bool SEOverride::HasOverrideSwitch() const
{
  return (m_OverrideSwitch == CDM::enumOnOff::Off) ? true : 
  (m_OverrideSwitch == CDM::enumOnOff::On) ? true : false;
}
void SEOverride::InvalidateOverrideSwitch()
{
  m_OverrideSwitch = (CDM::enumOnOff::Off);
}
CDM::enumOnOff::value SEOverride::GetOverrideConformance() const
{
  return m_OverrideConformance;
}
void SEOverride::SetOverrideConformance(CDM::enumOnOff::value valid)
{
  m_OverrideConformance = valid;
}
bool SEOverride::HasOverrideConformance() const
{
  return (m_OverrideConformance == CDM::enumOnOff::Off) ? true : 
  (m_OverrideConformance == CDM::enumOnOff::On) ? true : false;
}
void SEOverride::InvalidateOverrideConformance()
{
  m_OverrideConformance = (CDM::enumOnOff::Off);
}

// Blood Chemistry Overrides //
bool SEOverride::HasArterialPHOverride() const
{
  return m_ArterialPHOR == nullptr ? false : m_ArterialPHOR->IsValid();
}
SEScalar& SEOverride::GetArterialPHOverride()
{
  if (m_ArterialPHOR == nullptr) {
    m_ArterialPHOR = new SEScalar();
  }
  return *m_ArterialPHOR;
}
double SEOverride::GetArterialPHOverride() const
{
  if (m_ArterialPHOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_ArterialPHOR->GetValue();
}
bool SEOverride::HasVenousPHOverride() const
{
  return m_VenousPHOR == nullptr ? false : m_VenousPHOR->IsValid();
}
SEScalar& SEOverride::GetVenousPHOverride()
{
  if (m_VenousPHOR == nullptr) {
    m_VenousPHOR = new SEScalar();
  }
  return *m_VenousPHOR;
}
double SEOverride::GetVenousPHOverride() const
{
  if (m_VenousPHOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_VenousPHOR->GetValue();
}

bool SEOverride::HasBloodChemistryOverride() const
{
  return HasArterialPHOverride() 
  || HasVenousPHOverride();
}

// Cardiovascular Overrides //
bool SEOverride::HasCardiacOutputOverride() const
{
  return m_CardiacOutputOR == nullptr ? false : m_CardiacOutputOR->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetCardiacOutputOverride()
{
  if (m_CardiacOutputOR == nullptr) {
    m_CardiacOutputOR = new SEScalarVolumePerTime();
  }
  return *m_CardiacOutputOR;
}
double SEOverride::GetCardiacOutputOverride(const VolumePerTimeUnit& unit) const
{
  if (m_CardiacOutputOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_CardiacOutputOR->GetValue(unit);
}
bool SEOverride::HasMAPOverride() const
{
  return m_PressureOR == nullptr ? false : m_PressureOR->IsValid();
}
SEScalarPressure& SEOverride::GetMAPOverride()
{
  if (m_PressureOR == nullptr) {
    m_PressureOR = new SEScalarPressure();
  }
  return *m_PressureOR;
}
double SEOverride::GetMAPOverride(const PressureUnit& unit) const
{
  if (m_PressureOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_PressureOR->GetValue(unit);
}
bool SEOverride::HasHeartRateOverride() const
{
  return m_HeartRateOR == nullptr ? false : m_HeartRateOR->IsValid();
}
SEScalarFrequency& SEOverride::GetHeartRateOverride()
{
  if (m_HeartRateOR == nullptr) {
    m_HeartRateOR = new SEScalarFrequency();
  }
  return *m_HeartRateOR;
}
double SEOverride::GetHeartRateOverride(const FrequencyUnit& unit) const
{
  if (m_HeartRateOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_HeartRateOR->GetValue(unit);
}

bool SEOverride::HasCardiovascularOverride() const
{
  return HasCardiacOutputOverride() ||
  HasMAPOverride() ||
  HasHeartRateOverride();
 
}

// Energy Overrides //
bool SEOverride::HasAchievedExerciseLevelOverride() const
{
  return m_AcheivedExerciseLevelOR == nullptr ? false : m_AcheivedExerciseLevelOR->IsValid();
}
SEScalarFraction& SEOverride::GetAchievedExerciseLevelOverride()
{
  if (m_AcheivedExerciseLevelOR == nullptr) {
    m_AcheivedExerciseLevelOR = new SEScalarFraction();
  }
  return *m_AcheivedExerciseLevelOR;
}
double SEOverride::GetAchievedExerciseLevelOverride() const
{
  if (m_AcheivedExerciseLevelOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_AcheivedExerciseLevelOR->GetValue();
}
  bool SEOverride::HasCoreTemperatureOverride() const
{
  return m_CoreTemperatureOR == nullptr ? false : m_CoreTemperatureOR->IsValid();
}
SEScalarTemperature& SEOverride::GetCoreTemperatureOverride()
{
  if (m_CoreTemperatureOR == nullptr) {
    m_CoreTemperatureOR = new SEScalarTemperature();
  }
  return *m_CoreTemperatureOR;
}
double SEOverride::GetCoreTemperatureOverride(const TemperatureUnit& unit) const
{
  if (m_CoreTemperatureOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_CoreTemperatureOR->GetValue(unit);
}
bool SEOverride::HasCreatinineProductionRateOverride() const
{
  return m_CreatinineProductionRateOR == nullptr ? false : m_CreatinineProductionRateOR->IsValid();
}
SEScalarAmountPerTime& SEOverride::GetCreatinineProductionRateOverride()
{
  if (m_CreatinineProductionRateOR == nullptr) {
    m_CreatinineProductionRateOR = new SEScalarAmountPerTime();
  }
  return *m_CreatinineProductionRateOR;
}
double SEOverride::GetCreatinineProductionRateOverride(const AmountPerTimeUnit& unit) const
{
  if (m_CreatinineProductionRateOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_CreatinineProductionRateOR->GetValue(unit);
}
bool SEOverride::HasExerciseMeanArterialPressureDeltaOverride() const
{
  return m_ExerciseMeanArterialPressureDeltaOR == nullptr ? false : m_ExerciseMeanArterialPressureDeltaOR->IsValid();
}
SEScalarPressure& SEOverride::GetExerciseMeanArterialPressureDeltaOverride()
{
  if (m_ExerciseMeanArterialPressureDeltaOR == nullptr) {
    m_ExerciseMeanArterialPressureDeltaOR = new SEScalarPressure();
  }
  return *m_ExerciseMeanArterialPressureDeltaOR;
}
double SEOverride::GetExerciseMeanArterialPressureDeltaOverride(const PressureUnit& unit) const
{
  if (m_ExerciseMeanArterialPressureDeltaOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_ExerciseMeanArterialPressureDeltaOR->GetValue(unit);
}
bool SEOverride::HasFatigueLevelOverride() const
{
  return m_FatigueLevelOR == nullptr ? false : m_FatigueLevelOR->IsValid();
}
SEScalarFraction& SEOverride::GetFatigueLevelOverride()
{
  if (m_FatigueLevelOR == nullptr) {
    m_FatigueLevelOR = new SEScalarFraction();
  }
  return *m_FatigueLevelOR;
}
double SEOverride::GetFatigueLevelOverride() const
{
  if (m_FatigueLevelOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_FatigueLevelOR->GetValue();
}
bool SEOverride::HasLactateProductionRateOverride() const
{
  return m_LactateProductionRateOR == nullptr ? false : m_LactateProductionRateOR->IsValid();
}
SEScalarAmountPerTime& SEOverride::GetLactateProductionRateOverride()
{
  if (m_LactateProductionRateOR == nullptr) {
    m_LactateProductionRateOR = new SEScalarAmountPerTime();
  }
  return *m_LactateProductionRateOR;
}
double SEOverride::GetLactateProductionRateOverride(const AmountPerTimeUnit& unit) const
{
  if (m_LactateProductionRateOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_LactateProductionRateOR->GetValue(unit);
}
bool SEOverride::HasSkinTemperatureOverride() const
{
  return m_SkinTemperatureOR == nullptr ? false : m_SkinTemperatureOR->IsValid();
}
SEScalarTemperature& SEOverride::GetSkinTemperatureOverride()
{
  if (m_SkinTemperatureOR == nullptr) {
    m_SkinTemperatureOR = new SEScalarTemperature();
  }
  return *m_SkinTemperatureOR;
}
double SEOverride::GetSkinTemperatureOverride(const TemperatureUnit& unit) const
{
  if (m_SkinTemperatureOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_SkinTemperatureOR->GetValue(unit);
}
bool SEOverride::HasSweatRateOverride() const
{
  return m_SweatRateOR == nullptr ? false : m_SweatRateOR->IsValid();
}
SEScalarMassPerTime& SEOverride::GetSweatRateOverride()
{
  if (m_SweatRateOR == nullptr) {
    m_SweatRateOR = new SEScalarMassPerTime();
  }
  return *m_SweatRateOR;
}
double SEOverride::GetSweatRateOverride(const MassPerTimeUnit& unit) const
{
  if (m_SweatRateOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_SweatRateOR->GetValue(unit);
}
bool SEOverride::HasTotalMetabolicOverride() const
{
  return m_TotalMetabolicOR == nullptr ? false : m_TotalMetabolicOR->IsValid();
}
SEScalarPower& SEOverride::GetTotalMetabolicOverride()
{
  if (m_TotalMetabolicOR == nullptr) {
    m_TotalMetabolicOR = new SEScalarPower();
  }
  return *m_TotalMetabolicOR;
}
double SEOverride::GetTotalMetabolicOverride(const PowerUnit& unit) const
{
  if (m_TotalMetabolicOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TotalMetabolicOR->GetValue(unit);
}
bool SEOverride::HasTotalWorkRateLevelOverride() const
{
  return m_TotalWorkRateLevelOR == nullptr ? false : m_TotalWorkRateLevelOR->IsValid();
}
SEScalarFraction& SEOverride::GetTotalWorkRateLevelOverride()
{
  if (m_TotalWorkRateLevelOR == nullptr) {
    m_TotalWorkRateLevelOR = new SEScalarFraction();
  }
  return *m_TotalWorkRateLevelOR;
}
double SEOverride::GetTotalWorkRateLevelOverride() const
{
  if (m_TotalWorkRateLevelOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TotalWorkRateLevelOR->GetValue();
}
bool SEOverride::HasSodiumLostToSweatOverride() const
{
  return m_SodiumLostToSweatOR == nullptr ? false : m_SodiumLostToSweatOR->IsValid();
}
SEScalarMass& SEOverride::GetSodiumLostToSweatOverride()
{
  if (m_SodiumLostToSweatOR == nullptr) {
    m_SodiumLostToSweatOR = new SEScalarMass();
  }
  return *m_SodiumLostToSweatOR;
}
double SEOverride::GetSodiumLostToSweatOverride(const MassUnit& unit) const
{
  if (m_SodiumLostToSweatOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_SodiumLostToSweatOR->GetValue(unit);
}
bool SEOverride::HasPotassiumLostToSweatOverride() const
{
  return m_PotassiumLostToSweatOR == nullptr ? false : m_PotassiumLostToSweatOR->IsValid();
}
SEScalarMass& SEOverride::GetPotassiumLostToSweatOverride()
{
  if (m_PotassiumLostToSweatOR == nullptr) {
    m_PotassiumLostToSweatOR = new SEScalarMass();
  }
  return *m_PotassiumLostToSweatOR;
}
double SEOverride::GetPotassiumLostToSweatOverride(const MassUnit& unit) const
{
  if (m_PotassiumLostToSweatOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_PotassiumLostToSweatOR->GetValue(unit);
}
bool SEOverride::HasChlorideLostToSweatOverride() const
{
  return m_ChlorideLostToSweatOR == nullptr ? false : m_ChlorideLostToSweatOR->IsValid();
}
SEScalarMass& SEOverride::GetChlorideLostToSweatOverride()
{
  if (m_ChlorideLostToSweatOR == nullptr) {
    m_ChlorideLostToSweatOR = new SEScalarMass();
  }
  return *m_ChlorideLostToSweatOR;
}
double SEOverride::GetChlorideLostToSweatOverride(const MassUnit& unit) const
{
  if (m_ChlorideLostToSweatOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_ChlorideLostToSweatOR->GetValue(unit);
}

bool SEOverride::HasEnergyOverride() const
{
  return HasAchievedExerciseLevelOverride() 
  || HasCoreTemperatureOverride() 
  || HasCreatinineProductionRateOverride() 
  || HasExerciseMeanArterialPressureDeltaOverride() 
  || HasFatigueLevelOverride() 
  || HasLactateProductionRateOverride() 
  || HasSkinTemperatureOverride() 
  || HasSweatRateOverride() 
  || HasTotalMetabolicOverride() 
  || HasTotalWorkRateLevelOverride() 
  || HasSodiumLostToSweatOverride() 
  || HasPotassiumLostToSweatOverride() 
  || HasChlorideLostToSweatOverride();
}

// Renal Overrides //
bool SEOverride::HasLeftAfferentArterioleResistanceOverride() const
{
  return m_LAfferentArterioleResistOR == nullptr ? false : m_LAfferentArterioleResistOR->IsValid();
}
SEScalarFlowResistance& SEOverride::GetLeftAfferentArterioleResistanceOverride()
{
  if (m_LAfferentArterioleResistOR == nullptr) {
    m_LAfferentArterioleResistOR = new SEScalarFlowResistance();
  }
  return *m_LAfferentArterioleResistOR;
}
double SEOverride::GetLeftAfferentArterioleResistanceOverride(const FlowResistanceUnit& unit) const
{
  if (m_LAfferentArterioleResistOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_LAfferentArterioleResistOR->GetValue(unit);
}
bool SEOverride::HasLeftGlomerularFiltrationRateOverride() const
{
  return m__LeftGlomerularFiltrationOR == nullptr ? false : m__LeftGlomerularFiltrationOR->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetLeftGlomerularFiltrationRateOverride()
{
  if (m__LeftGlomerularFiltrationOR == nullptr) {
    m__LeftGlomerularFiltrationOR = new SEScalarVolumePerTime();
  }
  return *m__LeftGlomerularFiltrationOR;
}
double SEOverride::GetLeftGlomerularFiltrationRateOverride(const VolumePerTimeUnit& unit) const
{
  if (m__LeftGlomerularFiltrationOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m__LeftGlomerularFiltrationOR->GetValue(unit);
}
bool SEOverride::HasLeftReaborptionRateOverride() const
{
  return m_LReabsorptionRateOR == nullptr ? false : m_LReabsorptionRateOR->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetLeftReaborptionRateOverride()
{
  if (m_LReabsorptionRateOR == nullptr) {
    m_LReabsorptionRateOR = new SEScalarVolumePerTime();
  }
  return *m_LReabsorptionRateOR;
}
double SEOverride::GetLeftReaborptionRateOverride(const VolumePerTimeUnit& unit) const
{
  if (m_LReabsorptionRateOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_LReabsorptionRateOR->GetValue(unit);
}
bool SEOverride::HasRenalBloodFlowOverride() const
{
  return m_RenalBloodFlowOR == nullptr ? false : m_RenalBloodFlowOR->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetRenalBloodFlowOverride()
{
  if (m_RenalBloodFlowOR == nullptr) {
    m_RenalBloodFlowOR = new SEScalarVolumePerTime();
  }
  return *m_RenalBloodFlowOR;
}
double SEOverride::GetRenalBloodFlowOverride(const VolumePerTimeUnit& unit) const
{
  if (m_RenalBloodFlowOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_RenalBloodFlowOR->GetValue(unit);
}
bool SEOverride::HasRenalPlasmaFlowOverride() const
{
  return m_RenalPlasmaOR == nullptr ? false : m_RenalPlasmaOR->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetRenalPlasmaFlowOverride()
{
  if (m_RenalPlasmaOR == nullptr) {
    m_RenalPlasmaOR = new SEScalarVolumePerTime();
  }
  return *m_RenalPlasmaOR;
}
double SEOverride::GetRenalPlasmaFlowOverride(const VolumePerTimeUnit& unit) const
{
  if (m_RenalPlasmaOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_RenalPlasmaOR->GetValue(unit);
}
bool SEOverride::HasRightAfferentArterioleResistanceOverride() const
{
  return m_RAfferentArterioleResistOR == nullptr ? false : m_RAfferentArterioleResistOR->IsValid();
}
SEScalarFlowResistance& SEOverride::GetRightAfferentArterioleResistanceOverride()
{
  if (m_RAfferentArterioleResistOR == nullptr) {
    m_RAfferentArterioleResistOR = new SEScalarFlowResistance();
  }
  return *m_RAfferentArterioleResistOR;
}
double SEOverride::GetRightAfferentArterioleResistanceOverride(const FlowResistanceUnit& unit) const
{
  if (m_RAfferentArterioleResistOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_RAfferentArterioleResistOR->GetValue(unit);
}
bool SEOverride::HasRightGlomerularFiltrationRateOverride() const
{
  return m__RightGlomerularFiltrationOR == nullptr ? false : m__RightGlomerularFiltrationOR->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetRightGlomerularFiltrationRateOverride()
{
  if (m__RightGlomerularFiltrationOR == nullptr) {
    m__RightGlomerularFiltrationOR = new SEScalarVolumePerTime();
  }
  return *m__RightGlomerularFiltrationOR;
}
double SEOverride::GetRightGlomerularFiltrationRateOverride(const VolumePerTimeUnit& unit) const
{
  if (m__RightGlomerularFiltrationOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m__RightGlomerularFiltrationOR->GetValue(unit);
}

bool SEOverride::HasRightReaborptionRateOverride() const
{
  return m_RReabsorptionRateOR == nullptr ? false : m_RReabsorptionRateOR->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetRightReaborptionRateOverride()
{
  if (m_RReabsorptionRateOR == nullptr) {
    m_RReabsorptionRateOR = new SEScalarVolumePerTime();
  }
  return *m_RReabsorptionRateOR;
}
double SEOverride::GetRightReaborptionRateOverride(const VolumePerTimeUnit& unit) const
{
  if (m_RReabsorptionRateOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_RReabsorptionRateOR->GetValue(unit);
}
bool SEOverride::HasUrinationRateOverride() const
{
  return m_UrinationRateOR == nullptr ? false : m_UrinationRateOR->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetUrinationRateOverride()
{
  if (m_UrinationRateOR == nullptr) {
    m_UrinationRateOR = new SEScalarVolumePerTime();
  }
  return *m_UrinationRateOR;
}
double SEOverride::GetUrinationRateOverride(const VolumePerTimeUnit& unit) const
{
  if (m_UrinationRateOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_UrinationRateOR->GetValue(unit);
}
bool SEOverride::HasUrineProductionRateOverride() const
{
  return m_UrineProductionRateOR == nullptr ? false : m_UrineProductionRateOR->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetUrineProductionRateOverride()
{
  if (m_UrineProductionRateOR == nullptr) {
    m_UrineProductionRateOR = new SEScalarVolumePerTime();
  }
  return *m_UrineProductionRateOR;
}
double SEOverride::GetUrineProductionRateOverride(const VolumePerTimeUnit& unit) const
{
  if (m_UrineProductionRateOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_UrineProductionRateOR->GetValue(unit);
}
bool SEOverride::HasUrineOsmolalityOverride() const
{
  return m_UrineOsmolalityOR == nullptr ? false : m_UrineOsmolalityOR->IsValid();
}
SEScalarOsmolality& SEOverride::GetUrineOsmolalityOverride()
{
  if (m_UrineOsmolalityOR == nullptr) {
    m_UrineOsmolalityOR = new SEScalarOsmolality();
  }
  return *m_UrineOsmolalityOR;
}
double SEOverride::GetUrineOsmolalityOverride(const OsmolalityUnit& unit) const
{
  if (m_UrineOsmolalityOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_UrineOsmolalityOR->GetValue(unit);
}
bool SEOverride::HasUrineVolumeOverride() const
{
  return m_UrineVolumeOR == nullptr ? false : m_UrineVolumeOR->IsValid();
}
SEScalarVolume& SEOverride::GetUrineVolumeOverride()
{
  if (m_UrineVolumeOR == nullptr) {
    m_UrineVolumeOR = new SEScalarVolume();
  }
  return *m_UrineVolumeOR;
}
double SEOverride::GetUrineVolumeOverride(const VolumeUnit& unit) const
{
  if (m_UrineVolumeOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_UrineVolumeOR->GetValue(unit);
}

bool SEOverride::HasRenalOverride() const
{
  return HasLeftAfferentArterioleResistanceOverride() ||
  HasLeftGlomerularFiltrationRateOverride() ||
  HasLeftReaborptionRateOverride() ||
  HasRenalBloodFlowOverride() ||
  HasRenalPlasmaFlowOverride() ||
  HasRightAfferentArterioleResistanceOverride() ||
  HasRightGlomerularFiltrationRateOverride() ||
  HasRightReaborptionRateOverride() ||
  HasUrinationRateOverride() ||
  HasUrineProductionRateOverride() ||
  HasUrineOsmolalityOverride() ||
  HasUrineVolumeOverride();
}


// Respiratory Overrides //
bool SEOverride::HasRespirationRateOverride() const
{
  return m_RespirationRateOR == nullptr ? false : m_RespirationRateOR->IsValid();
}
SEScalarFrequency& SEOverride::GetRespirationRateOverride()
{
  if (m_RespirationRateOR == nullptr) {
    m_RespirationRateOR = new SEScalarFrequency();
  }
  return *m_RespirationRateOR;
}
double SEOverride::GetRespirationRateOverride(const FrequencyUnit& unit) const
{
  if (m_RespirationRateOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_RespirationRateOR->GetValue(unit);
}
bool SEOverride::HasTidalVolumeOverride() const
{
  return m_TidalVolumeOR == nullptr ? false : m_TidalVolumeOR->IsValid();
}
SEScalarVolume& SEOverride::GetTidalVolumeOverride()
{
  if (m_TidalVolumeOR == nullptr) {
    m_TidalVolumeOR = new SEScalarVolume();
  }
  return *m_TidalVolumeOR;
}
double SEOverride::GetTidalVolumeOverride(const VolumeUnit& unit) const
{
  if (m_TidalVolumeOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TidalVolumeOR->GetValue(unit);
}

bool SEOverride::HasRespiratoryOverride() const
{
  return HasRespirationRateOverride() 
    || HasTidalVolumeOverride();
}


void SEOverride::ToString(std::ostream& str) const
{
  str << "Patient Action : Override Parameter";
  if (HasComment()) {
    str << "\n\tComment: " << m_Comment;
  }

  str << "\n\tState: ";
  HasOverrideSwitch() ? str << GetOverrideSwitch() : str << "Not Set";
  str << "\n\tConformant: ";
  HasOverrideConformance() ? str << GetOverrideConformance() : str << "Not Set";
  if (GetOverrideConformance() == CDM::enumOnOff::Off 
    && GetOverrideSwitch() == CDM::enumOnOff::On) {
    str << ("\n\tOverride has turned conformance off. Outputs no longer resemble validated parameters.");
  }
  if (HasArterialPHOverride()) {
    str << "\n\tArterial PH: ";
    HasArterialPHOverride() ? str << *m_ArterialPHOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tArterial PH has a lower bound of 0 and an upper bound of 14.";
    }
    str << std::flush;
  }
  if (HasVenousPHOverride()) {
    str << "\n\tVenous PH: ";
    HasVenousPHOverride() ? str << *m_VenousPHOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tVenous PH has a lower bound of 0 and an upper bound of 14.";
    }
    str << std::flush;
  }
  if (HasCardiacOutputOverride()) {
    str << "\n\tCardiac Output: ";
    HasCardiacOutputOverride() ? str << *m_CardiacOutputOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tCardiac Output has a lower bound of 0 L/min and an upper bound of 100 L/min.";
    }
    str << std::flush;
  }
  if (HasMAPOverride())
  {
    str << "\n\tMean Arterial Pressure: ";
    HasMAPOverride() ? str << *m_PressureOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tMean Arterial Pressure has a lower bound of 60 mmHg and an upper bound of 105 mmHg.";
    } else {
      str << "\n\tPharmacodynamics have been turned off due to conformance being turned off.";
    }
    str << std::flush;
  }
  if (HasHeartRateOverride()) {
    str << "\n\tHeart Rate: ";
    HasHeartRateOverride() ? str << *m_HeartRateOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tHeart Rate has a lower bound of 30 bpm and an upper bound of 240 bpm.";
    } else {
      str << "\n\tPharmacodynamics have been turned off due to conformance being turned off.";
    }
    str << std::flush;
  }
  if (HasAchievedExerciseLevelOverride()) {
    str << "\n\tAcieved Exercise Level: ";
    HasAchievedExerciseLevelOverride() ? str << *m_AcheivedExerciseLevelOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tAcheived Exercise Level has a lower bound of 0 and an upper bound of 1.";
    }
    str << std::flush;
  }
  if (HasCoreTemperatureOverride())
  {
    str << "\n\tCore Temperature: ";
    HasCoreTemperatureOverride() ? str << *m_CoreTemperatureOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tCore Temperature has a lower bound of 0 degrees Celsius and an upper bound of 200 degrees Celsius.";
    }
    str << std::flush;
  }
  if (HasCreatinineProductionRateOverride()) {
    str << "\n\tCreatinine Production Rate: ";
    HasCreatinineProductionRateOverride() ? str << *m_CreatinineProductionRateOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tCreatinine Production has a lower bound of 0 mol/s and an upper bound of 100 mol/s.";
    }
    str << std::flush;
  }
  if (HasExerciseMeanArterialPressureDeltaOverride()) {
    str << "\n\tExercise MAP Delta: ";
    HasExerciseMeanArterialPressureDeltaOverride() ? str << *m_ExerciseMeanArterialPressureDeltaOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tExercise MAP Delta has a lower bound of 0 mmHg and an upper bound of 200 mmHg.";
    }
    str << std::flush;
  }
  if (HasFatigueLevelOverride()) {
    str << "\n\tFatigue Level: ";
    HasFatigueLevelOverride() ? str << *m_FatigueLevelOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tFatigue Level has a lower bound of 0 and an upper bound of 1.";
    }
    str << std::flush;
  }
  if (HasLactateProductionRateOverride()) {
    str << "\n\tLactate Production: ";
    HasLactateProductionRateOverride() ? str << *m_LactateProductionRateOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tLactate Production has a lower bound of 0 mol/s and an upper bound of 200 mol/s.";
    }
    str << std::flush;
  }
  if (HasSkinTemperatureOverride()) {
    str << "\n\tSkin Temperature: ";
    HasSkinTemperatureOverride() ? str << *m_SkinTemperatureOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tSkin Temperature has a lower bound of 0 degrees Celsius and an upper bound of 200 degrees Celsius.";
    }
    str << std::flush;
  }
  if (HasSweatRateOverride()) {
    str << "\n\tSweat Rate: ";
    HasSweatRateOverride() ? str << *m_SweatRateOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tSweat Rate has a lower bound of 0 g/s and an upper bound of 50 g/s.";
    }
    str << std::flush;
  }
  if (HasTotalMetabolicOverride()) {
    str << "\n\tTotal Metabolic Rate: ";
    HasTotalMetabolicOverride() ? str << *m_TotalMetabolicOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tTotal Metabolic Rate has a lower bound of 1 kcal/day and an upper bound of 5000 kcal/day.";
    }
    str << std::flush;
  }
  if (HasTotalWorkRateLevelOverride()) {
    str << "\n\tTotal Work Rate Level: ";
    HasTotalWorkRateLevelOverride() ? str << *m_TotalWorkRateLevelOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tTotal Work Rate Level has a lower bound of 0 and an upper bound of 1.";
    }
    str << std::flush;
  }
  if (HasSodiumLostToSweatOverride()) {
    str << "\n\tSodium Lost to Sweat: ";
    HasSodiumLostToSweatOverride() ? str << *m_SodiumLostToSweatOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tSodium Lost to Sweat has a lower bound of 0 g and an upper bound of 500 g.";
    }
    str << std::flush;
  }
  if (HasPotassiumLostToSweatOverride()) {
    str << "\n\tPotassium Lost to Sweat: ";
    HasPotassiumLostToSweatOverride() ? str << *m_PotassiumLostToSweatOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tPotassium Lost to Sweat has a lower bound of 0 g and an upper bound of 500 g.";
    }
    str << std::flush;
  }
  if (HasChlorideLostToSweatOverride()) {
    str << "\n\tChloride Lost to Sweat: ";
    HasChlorideLostToSweatOverride() ? str << *m_ChlorideLostToSweatOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tChloride Lost to Sweat has a lower bound of 0 g and an upper bound of 500 g.";
    }
    str << std::flush;
  }
  if (HasLeftAfferentArterioleResistanceOverride()) {
    str << "\n\tLeft Afferent Arteriole Resistance: ";
    HasLeftAfferentArterioleResistanceOverride() ? str << *m_LAfferentArterioleResistOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tLeft Afferent Arteriole Resistance has a lower bound of 0 mmHg min/mL and an upper bound of 1 mmHg min/mL.";
    }
    str << std::flush;
  }
  if (HasLeftGlomerularFiltrationRateOverride()) {
    str << "\n\tLeft Glomerular Filtration Rate: ";
    HasLeftGlomerularFiltrationRateOverride() ? str << *m__LeftGlomerularFiltrationOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tLeft Glomerular Filtration Rate has a lower bound of 0 mL/min and an upper bound of 1000 mL/min.";
    }
    str << std::flush;
  }
  if (HasLeftReaborptionRateOverride()) {
    str << "\n\tLeft Reabsorption Rate: ";
    HasLeftReaborptionRateOverride() ? str << *m_LReabsorptionRateOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tLeft Reabsorption Rate has a lower bound of 0 mL/min and an upper bound of 1000 mL/min.";
    }
    str << std::flush;
  }
  if (HasRenalBloodFlowOverride()) {
    str << "\n\tRenal Blood Flow: ";
    HasRenalBloodFlowOverride() ? str << *m_RenalBloodFlowOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tRenal Blood Flow has a lower bound of 0 mL/min and an upper bound of 3000 mL/min.";
    }
    str << std::flush;
  }
  if (HasRenalPlasmaFlowOverride()) {
    str << "\n\tRenal Plasma Flow: ";
    HasRenalPlasmaFlowOverride() ? str << *m_RenalPlasmaOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tRenal Plasma Flow has a lower bound of 0 mL/min and an upper bound of 3000 mL/min.";
    }
    str << std::flush;
  }
  if (HasRightAfferentArterioleResistanceOverride()) {
    str << "\n\tRight Afferent Arteriole Resistance: ";
    HasRightAfferentArterioleResistanceOverride() ? str << *m_RAfferentArterioleResistOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tRight Afferent Arteriole Resistance has a lower bound of 0 mmHg min/mL and an upper bound of 1 mmHg min/mL.";
    }
    str << std::flush;
  }
  if (HasRightGlomerularFiltrationRateOverride()) {
    str << "\n\tRight Glomerular Filtration Rate: ";
    HasRightGlomerularFiltrationRateOverride() ? str << *m__RightGlomerularFiltrationOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tRight Glomerular Filtration Rate has a lower bound of 0 mL/min and an upper bound of 1000 mL/min.";
    }
    str << std::flush;
  }
  if (HasRightReaborptionRateOverride()) {
    str << "\n\tRight Reabsorption Rate: ";
    HasRightReaborptionRateOverride() ? str << *m_RReabsorptionRateOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tRight Reabsorption Rate has a lower bound of 0 mL/min and an upper bound of 1000 mL/min.";
    }
    str << std::flush;
  }
  if (HasUrinationRateOverride()) {
    str << "\n\tUrination Rate: ";
    HasUrinationRateOverride() ? str << *m_UrinationRateOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tUrination Rate has a lower bound of 0 mL/min and an upper bound of 1000 mL/min.";
    }
    str << std::flush;
  }
  if (HasUrineProductionRateOverride()) {
    str << "\n\tUrine Production Rate: ";
    HasUrineProductionRateOverride() ? str << *m_UrineProductionRateOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tUrine Production Rate has a lower bound of 0 mL/min and an upper bound of 100 mL/min.";
    }
    str << std::flush;
  }
  if (HasUrineOsmolalityOverride()) {
    str << "\n\tUrine Osmolality: ";
    HasUrineOsmolalityOverride() ? str << *m_UrineOsmolalityOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tUrine Osmolality has a lower bound of 0 mOsm/kg and an upper bound of 2000 mOsm/kg.";
    }
    str << std::flush;
  }
  if (HasUrineVolumeOverride()) {
    str << "\n\tUrine Volume: ";
    HasUrineVolumeOverride() ? str << *m_UrineVolumeOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tUrine Volume has a lower bound of 0 mL and an upper bound of 1000 mL.";
    }
    str << std::flush;
  }
  if (HasRespirationRateOverride()) {
    str << "\n\tRespiration Rate: ";
    HasRespirationRateOverride() ? str << *m_RespirationRateOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tRespiration Rate has a lower bound of 0 breaths/min and an upper bound of 60 breaths/min.";
    }
    str << std::flush;
  }
  if (HasTidalVolumeOverride()) {
    str << "\n\tTidal Volume: ";
    HasTidalVolumeOverride() ? str << *m_TidalVolumeOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tTidal Volume has a lower bound of 0 mL and an upper bound of 10000 mL.";
    }
    str << std::flush;
  }
}
}
