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

#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarOsmolality.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>

// Private Includes
#include <io/cdm/PatientActions.h>

namespace biogears {
SEOverride::SEOverride()
  : SEPatientAction()
{
  m_OverrideState = CDM::enumOnOff::Off;
  m_OverrideConformance = CDM::enumOnOff::On;
  m_ArterialBloodPHOverride = nullptr;
  m_VenousBloodPHOverride = nullptr;
  m_CarbonDioxideSaturationOverride = nullptr;
  m_CarbonMonoxideSaturationOverride = nullptr;
  m_OxygenSaturationOverride = nullptr;
  m_PhosphateOverride = nullptr;
  m_WhiteBloodCellCountOverride = nullptr;
  m_TotalBilirubinOverride = nullptr;
  m_CalciumConcentrationOverride = nullptr;
  m_GlucoseConcentrationOverride = nullptr;
  m_LactateConcentrationOverride = nullptr;
  m_PotassiumConcentrationOverride = nullptr;
  m_SodiumConcentrationOverride = nullptr;
  m_BloodVolumeOverride = nullptr;
  m_CardiacOutputOverride = nullptr;
  m_DiastolicArterialPressureOverride = nullptr;
  m_MeanArterialPressureOverride = nullptr;
  m_HeartRateOverride = nullptr;
  m_HeartStrokeVolumeOverride = nullptr;
  m_SystolicArterialPressureOverride = nullptr;
  m_InsulinSynthesisRateOverride = nullptr;
  m_GlucagonSynthesisRateOverride = nullptr;
  m_AchievedExerciseLevelOverride = nullptr;
  m_CoreTemperatureOverride = nullptr;
  m_CreatinineProductionRateOverride = nullptr;
  m_ExerciseMeanArterialPressureDeltaOverride = nullptr;
  m_FatigueLevelOverride = nullptr;
  m_LactateProductionRateOverride = nullptr;
  m_SkinTemperatureOverride = nullptr;
  m_SweatRateOverride = nullptr;
  m_TotalMetabolicRateOverride = nullptr;
  m_TotalWorkRateLevelOverride = nullptr;
  m_SodiumLostToSweatOverride = nullptr;
  m_PotassiumLostToSweatOverride = nullptr;
  m_ChlorideLostToSweatOverride = nullptr;
  m_LeftAfferentArterioleResistanceOverride = nullptr;
  m_LeftGlomerularFiltrationRateOverride = nullptr;
  m_LeftReabsorptionRateOverride = nullptr;
  m_RenalBloodFlowOverride = nullptr;
  m_RenalPlasmaFlowOverride = nullptr;
  m_RightAfferentArterioleResistanceOverride = nullptr;
  m_RightGlomerularFiltrationRateOverride = nullptr;
  m_RightReabsorptionRateOverride = nullptr;
  m_UrinationRateOverride = nullptr;
  m_UrineProductionRateOverride = nullptr;
  m_UrineOsmolalityOverride = nullptr;
  m_UrineVolumeOverride = nullptr;
  m_UrineUreaNitrogenConcentrationOverride = nullptr;
  m_ExpiratoryFlowOverride = nullptr;
  m_InspiratoryFlowOverride = nullptr;
  m_PulmonaryComplianceOverride = nullptr;
  m_PulmonaryResistanceOverride = nullptr;
  m_RespirationRateOverride = nullptr;
  m_TidalVolumeOverride = nullptr;
  m_TargetPulmonaryVentilationOverride = nullptr;
  m_TotalAlveolarVentilationOverride = nullptr;
  m_TotalLungVolumeOverride = nullptr;
  m_TotalPulmonaryVentilationOverride = nullptr;
  m_ExtravascularFluidVolumeOverride = nullptr;
  m_IntracellularFluidVolumeOverride = nullptr;
  m_LiverGlycogenOverride = nullptr;
  m_MuscleGlycogenOverride = nullptr;
  m_StoredProteinOverride = nullptr;
  m_StoredFatOverride = nullptr;
}
//-------------------------------------------------------------------------------
SEOverride::~SEOverride()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEOverride::Clear()
{
  SEPatientAction::Clear();
  m_OverrideState = CDM::enumOnOff::Off;
  m_OverrideConformance = CDM::enumOnOff::On;
  SAFE_DELETE(m_ArterialBloodPHOverride);
  SAFE_DELETE(m_VenousBloodPHOverride);
  SAFE_DELETE(m_CarbonDioxideSaturationOverride);
  SAFE_DELETE(m_CarbonMonoxideSaturationOverride);
  SAFE_DELETE(m_OxygenSaturationOverride);
  SAFE_DELETE(m_PhosphateOverride);
  SAFE_DELETE(m_WhiteBloodCellCountOverride);
  SAFE_DELETE(m_TotalBilirubinOverride);
  SAFE_DELETE(m_CalciumConcentrationOverride);
  SAFE_DELETE(m_GlucoseConcentrationOverride);
  SAFE_DELETE(m_LactateConcentrationOverride);
  SAFE_DELETE(m_PotassiumConcentrationOverride);
  SAFE_DELETE(m_SodiumConcentrationOverride);
  SAFE_DELETE(m_BloodVolumeOverride);
  SAFE_DELETE(m_CardiacOutputOverride);
  SAFE_DELETE(m_DiastolicArterialPressureOverride);
  SAFE_DELETE(m_MeanArterialPressureOverride);
  SAFE_DELETE(m_HeartRateOverride);
  SAFE_DELETE(m_HeartStrokeVolumeOverride);
  SAFE_DELETE(m_SystolicArterialPressureOverride);
  SAFE_DELETE(m_InsulinSynthesisRateOverride);
  SAFE_DELETE(m_GlucagonSynthesisRateOverride);
  SAFE_DELETE(m_AchievedExerciseLevelOverride);
  SAFE_DELETE(m_CoreTemperatureOverride);
  SAFE_DELETE(m_CreatinineProductionRateOverride);
  SAFE_DELETE(m_ExerciseMeanArterialPressureDeltaOverride);
  SAFE_DELETE(m_FatigueLevelOverride);
  SAFE_DELETE(m_LactateProductionRateOverride);
  SAFE_DELETE(m_SkinTemperatureOverride);
  SAFE_DELETE(m_SweatRateOverride);
  SAFE_DELETE(m_TotalMetabolicRateOverride);
  SAFE_DELETE(m_TotalWorkRateLevelOverride);
  SAFE_DELETE(m_SodiumLostToSweatOverride);
  SAFE_DELETE(m_PotassiumLostToSweatOverride);
  SAFE_DELETE(m_ChlorideLostToSweatOverride);
  SAFE_DELETE(m_LeftAfferentArterioleResistanceOverride);
  SAFE_DELETE(m_LeftGlomerularFiltrationRateOverride);
  SAFE_DELETE(m_LeftReabsorptionRateOverride);
  SAFE_DELETE(m_RenalBloodFlowOverride);
  SAFE_DELETE(m_RenalPlasmaFlowOverride);
  SAFE_DELETE(m_RightAfferentArterioleResistanceOverride);
  SAFE_DELETE(m_RightGlomerularFiltrationRateOverride);
  SAFE_DELETE(m_RightReabsorptionRateOverride);
  SAFE_DELETE(m_UrinationRateOverride);
  SAFE_DELETE(m_UrineProductionRateOverride);
  SAFE_DELETE(m_UrineOsmolalityOverride);
  SAFE_DELETE(m_UrineVolumeOverride);
  SAFE_DELETE(m_UrineUreaNitrogenConcentrationOverride);
  SAFE_DELETE(m_ExpiratoryFlowOverride);
  SAFE_DELETE(m_InspiratoryFlowOverride);
  SAFE_DELETE(m_PulmonaryComplianceOverride);
  SAFE_DELETE(m_PulmonaryResistanceOverride);
  SAFE_DELETE(m_RespirationRateOverride);
  SAFE_DELETE(m_TidalVolumeOverride);
  SAFE_DELETE(m_TargetPulmonaryVentilationOverride);
  SAFE_DELETE(m_TotalAlveolarVentilationOverride);
  SAFE_DELETE(m_TotalLungVolumeOverride);
  SAFE_DELETE(m_TotalPulmonaryVentilationOverride);
  SAFE_DELETE(m_ExtravascularFluidVolumeOverride);
  SAFE_DELETE(m_IntracellularFluidVolumeOverride);
  SAFE_DELETE(m_LiverGlycogenOverride);
  SAFE_DELETE(m_MuscleGlycogenOverride);
  SAFE_DELETE(m_StoredProteinOverride);
  SAFE_DELETE(m_StoredFatOverride);
}
//-------------------------------------------------------------------------------
bool SEOverride::IsValid() const
{
  if ((GetOverrideState() == CDM::enumOnOff::On)
      && (HasOverrideConformance())) {
    if (HasBloodChemistryOverride()
        || HasCardiovascularOverride()
        || HasEndocrineOverride()
        || HasEnergyOverride()
        || HasRenalOverride()
        || HasRespiratoryOverride()
        || HasTissueOverride()) {
      return true;
    }
  } else if ((GetOverrideState() == CDM::enumOnOff::Off)
             && (HasOverrideConformance())) {
    return true;
  } else {
    return false;
  }
  return false;
}
//-------------------------------------------------------------------------------
bool SEOverride::IsActive() const
{
  return (GetOverrideState() == CDM::enumOnOff::On);
}
//-------------------------------------------------------------------------------
bool SEOverride::Load(const CDM::OverrideData& in)
{
  io::PatientActions::UnMarshall(in, *this);
  return IsValid();
}
//-------------------------------------------------------------------------------
CDM::OverrideData* SEOverride::Unload() const
{
  CDM::OverrideData* data(new CDM::OverrideData());
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SEOverride::Unload(CDM::OverrideData& data) const
{
  io::PatientActions::Marshall(*this, data);
}
//-------------------------------------------------------------------------------
CDM::enumOnOff::value SEOverride::GetOverrideState() const
{
  return m_OverrideState;
}
void SEOverride::SetOverrideState(CDM::enumOnOff::value state)
{
  m_OverrideState = state;
}
bool SEOverride::HasOverrideState() const
{
  return (m_OverrideState == CDM::enumOnOff::Off) ? true : (m_OverrideState == CDM::enumOnOff::On) ? true
                                                                                                   : false;
}
void SEOverride::InvalidateOverrideState()
{
  m_OverrideState = (CDM::enumOnOff::Off);
}
//-------------------------------------------------------------------------------
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
  return (m_OverrideConformance == CDM::enumOnOff::Off) ? true : (m_OverrideConformance == CDM::enumOnOff::On) ? true
                                                                                                               : false;
}
void SEOverride::InvalidateOverrideConformance()
{
  m_OverrideConformance = (CDM::enumOnOff::Off);
}

// Blood Chemistry Overrides //
bool SEOverride::HasArterialPHOverride() const
{
  return m_ArterialBloodPHOverride == nullptr ? false : m_ArterialBloodPHOverride->IsValid();
}
SEScalar& SEOverride::GetArterialPHOverride()
{
  if (m_ArterialBloodPHOverride == nullptr) {
    m_ArterialBloodPHOverride = new SEScalar();
  }
  return *m_ArterialBloodPHOverride;
}
double SEOverride::GetArterialPHOverride() const
{
  if (m_ArterialBloodPHOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_ArterialBloodPHOverride->GetValue();
}
//-------------------------------------------------------------------------------
bool SEOverride::HasVenousPHOverride() const
{
  return m_VenousBloodPHOverride == nullptr ? false : m_VenousBloodPHOverride->IsValid();
}
SEScalar& SEOverride::GetVenousPHOverride()
{
  if (m_VenousBloodPHOverride == nullptr) {
    m_VenousBloodPHOverride = new SEScalar();
  }
  return *m_VenousBloodPHOverride;
}
double SEOverride::GetVenousPHOverride() const
{
  if (m_VenousBloodPHOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_VenousBloodPHOverride->GetValue();
}
//-------------------------------------------------------------------------------
bool SEOverride::HasCO2SaturationOverride() const
{
  return m_CarbonDioxideSaturationOverride == nullptr ? false : m_CarbonDioxideSaturationOverride->IsValid();
}
SEScalarFraction& SEOverride::GetCO2SaturationOverride()
{
  if (m_CarbonDioxideSaturationOverride == nullptr) {
    m_CarbonDioxideSaturationOverride = new SEScalarFraction();
  }
  return *m_CarbonDioxideSaturationOverride;
}
double SEOverride::GetCO2SaturationOverride() const
{
  if (m_CarbonDioxideSaturationOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_CarbonDioxideSaturationOverride->GetValue();
}
//-------------------------------------------------------------------------------
bool SEOverride::HasCOSaturationOverride() const
{
  return m_CarbonMonoxideSaturationOverride == nullptr ? false : m_CarbonMonoxideSaturationOverride->IsValid();
}
SEScalarFraction& SEOverride::GetCOSaturationOverride()
{
  if (m_CarbonMonoxideSaturationOverride == nullptr) {
    m_CarbonMonoxideSaturationOverride = new SEScalarFraction();
  }
  return *m_CarbonMonoxideSaturationOverride;
}
double SEOverride::GetCOSaturationOverride() const
{
  if (m_CarbonMonoxideSaturationOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_CarbonMonoxideSaturationOverride->GetValue();
}
//-------------------------------------------------------------------------------
bool SEOverride::HasO2SaturationOverride() const
{
  return m_OxygenSaturationOverride == nullptr ? false : m_OxygenSaturationOverride->IsValid();
}
SEScalarFraction& SEOverride::GetO2SaturationOverride()
{
  if (m_OxygenSaturationOverride == nullptr) {
    m_OxygenSaturationOverride = new SEScalarFraction();
  }
  return *m_OxygenSaturationOverride;
}
double SEOverride::GetO2SaturationOverride() const
{
  if (m_OxygenSaturationOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_OxygenSaturationOverride->GetValue();
}
//-------------------------------------------------------------------------------
bool SEOverride::HasPhosphateOverride() const
{
  return m_PhosphateOverride == nullptr ? false : m_PhosphateOverride->IsValid();
}
SEScalarAmountPerVolume& SEOverride::GetPhosphateOverride()
{
  if (m_PhosphateOverride == nullptr) {
    m_PhosphateOverride = new SEScalarAmountPerVolume();
  }
  return *m_PhosphateOverride;
}
double SEOverride::GetPhosphateOverride(const AmountPerVolumeUnit& unit) const
{
  if (m_PhosphateOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_PhosphateOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasWBCCountOverride() const
{
  return m_WhiteBloodCellCountOverride == nullptr ? false : m_WhiteBloodCellCountOverride->IsValid();
}
SEScalarAmountPerVolume& SEOverride::GetWBCCountOverride()
{
  if (m_WhiteBloodCellCountOverride == nullptr) {
    m_WhiteBloodCellCountOverride = new SEScalarAmountPerVolume();
  }
  return *m_WhiteBloodCellCountOverride;
}
double SEOverride::GetWBCCountOverride(const AmountPerVolumeUnit& unit) const
{
  if (m_WhiteBloodCellCountOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_WhiteBloodCellCountOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasTotalBilirubinOverride() const
{
  return m_TotalBilirubinOverride == nullptr ? false : m_TotalBilirubinOverride->IsValid();
}
SEScalarMassPerVolume& SEOverride::GetTotalBilirubinOverride()
{
  if (m_TotalBilirubinOverride == nullptr) {
    m_TotalBilirubinOverride = new SEScalarMassPerVolume();
  }
  return *m_TotalBilirubinOverride;
}
double SEOverride::GetTotalBilirubinOverride(const MassPerVolumeUnit& unit) const
{
  if (m_TotalBilirubinOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TotalBilirubinOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasCalciumConcentrationOverride() const
{
  return m_CalciumConcentrationOverride == nullptr ? false : m_CalciumConcentrationOverride->IsValid();
}
SEScalarMassPerVolume& SEOverride::GetCalciumConcentrationOverride()
{
  if (m_CalciumConcentrationOverride == nullptr) {
    m_CalciumConcentrationOverride = new SEScalarMassPerVolume();
  }
  return *m_CalciumConcentrationOverride;
}
double SEOverride::GetCalciumConcentrationOverride(const MassPerVolumeUnit& unit) const
{
  if (m_CalciumConcentrationOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_CalciumConcentrationOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasGlucoseConcentrationOverride() const
{
  return m_GlucoseConcentrationOverride == nullptr ? false : m_GlucoseConcentrationOverride->IsValid();
}
SEScalarMassPerVolume& SEOverride::GetGlucoseConcentrationOverride()
{
  if (m_GlucoseConcentrationOverride == nullptr) {
    m_GlucoseConcentrationOverride = new SEScalarMassPerVolume();
  }
  return *m_GlucoseConcentrationOverride;
}
double SEOverride::GetGlucoseConcentrationOverride(const MassPerVolumeUnit& unit) const
{
  if (m_GlucoseConcentrationOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_GlucoseConcentrationOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasLactateConcentrationOverride() const
{
  return m_LactateConcentrationOverride == nullptr ? false : m_LactateConcentrationOverride->IsValid();
}
SEScalarMassPerVolume& SEOverride::GetLactateConcentrationOverride()
{
  if (m_LactateConcentrationOverride == nullptr) {
    m_LactateConcentrationOverride = new SEScalarMassPerVolume();
  }
  return *m_LactateConcentrationOverride;
}
double SEOverride::GetLactateConcentrationOverride(const MassPerVolumeUnit& unit) const
{
  if (m_LactateConcentrationOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_LactateConcentrationOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasPotassiumConcentrationOverride() const
{
  return m_PotassiumConcentrationOverride == nullptr ? false : m_PotassiumConcentrationOverride->IsValid();
}
SEScalarMassPerVolume& SEOverride::GetPotassiumConcentrationOverride()
{
  if (m_PotassiumConcentrationOverride == nullptr) {
    m_PotassiumConcentrationOverride = new SEScalarMassPerVolume();
  }
  return *m_PotassiumConcentrationOverride;
}
double SEOverride::GetPotassiumConcentrationOverride(const MassPerVolumeUnit& unit) const
{
  if (m_PotassiumConcentrationOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_PotassiumConcentrationOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasSodiumConcentrationOverride() const
{
  return m_SodiumConcentrationOverride == nullptr ? false : m_SodiumConcentrationOverride->IsValid();
}
SEScalarMassPerVolume& SEOverride::GetSodiumConcentrationOverride()
{
  if (m_SodiumConcentrationOverride == nullptr) {
    m_SodiumConcentrationOverride = new SEScalarMassPerVolume();
  }
  return *m_SodiumConcentrationOverride;
}
double SEOverride::GetSodiumConcentrationOverride(const MassPerVolumeUnit& unit) const
{
  if (m_SodiumConcentrationOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_SodiumConcentrationOverride->GetValue(unit);
}

bool SEOverride::HasBloodChemistryOverride() const
{
  return HasArterialPHOverride()
    || HasVenousPHOverride()
    || HasCO2SaturationOverride()
    || HasCOSaturationOverride()
    || HasO2SaturationOverride()
    || HasPhosphateOverride()
    || HasWBCCountOverride()
    || HasTotalBilirubinOverride()
    || HasCalciumConcentrationOverride()
    || HasGlucoseConcentrationOverride()
    || HasLactateConcentrationOverride()
    || HasPotassiumConcentrationOverride()
    || HasSodiumConcentrationOverride();
}

// Cardiovascular Overrides //
bool SEOverride::HasBloodVolumeOverride() const
{
  return m_BloodVolumeOverride == nullptr ? false : m_BloodVolumeOverride->IsValid();
}
SEScalarVolume& SEOverride::GetBloodVolumeOverride()
{
  if (m_BloodVolumeOverride == nullptr) {
    m_BloodVolumeOverride = new SEScalarVolume();
  }
  return *m_BloodVolumeOverride;
}
double SEOverride::GetBloodVolumeOverride(const VolumeUnit& unit) const
{
  if (m_BloodVolumeOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_BloodVolumeOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasCardiacOutputOverride() const
{
  return m_CardiacOutputOverride == nullptr ? false : m_CardiacOutputOverride->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetCardiacOutputOverride()
{
  if (m_CardiacOutputOverride == nullptr) {
    m_CardiacOutputOverride = new SEScalarVolumePerTime();
  }
  return *m_CardiacOutputOverride;
}
double SEOverride::GetCardiacOutputOverride(const VolumePerTimeUnit& unit) const
{
  if (m_CardiacOutputOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_CardiacOutputOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasDiastolicArterialPressureOverride() const
{
  return m_DiastolicArterialPressureOverride == nullptr ? false : m_DiastolicArterialPressureOverride->IsValid();
}
SEScalarPressure& SEOverride::GetDiastolicArterialPressureOverride()
{
  if (m_DiastolicArterialPressureOverride == nullptr) {
    m_DiastolicArterialPressureOverride = new SEScalarPressure();
  }
  return *m_DiastolicArterialPressureOverride;
}
double SEOverride::GetDiastolicArterialPressureOverride(const PressureUnit& unit) const
{
  if (m_DiastolicArterialPressureOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_DiastolicArterialPressureOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasMAPOverride() const
{
  return m_MeanArterialPressureOverride == nullptr ? false : m_MeanArterialPressureOverride->IsValid();
}
SEScalarPressure& SEOverride::GetMAPOverride()
{
  if (m_MeanArterialPressureOverride == nullptr) {
    m_MeanArterialPressureOverride = new SEScalarPressure();
  }
  return *m_MeanArterialPressureOverride;
}
double SEOverride::GetMAPOverride(const PressureUnit& unit) const
{
  if (m_MeanArterialPressureOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_MeanArterialPressureOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasHeartRateOverride() const
{
  return m_HeartRateOverride == nullptr ? false : m_HeartRateOverride->IsValid();
}
SEScalarFrequency& SEOverride::GetHeartRateOverride()
{
  if (m_HeartRateOverride == nullptr) {
    m_HeartRateOverride = new SEScalarFrequency();
  }
  return *m_HeartRateOverride;
}
double SEOverride::GetHeartRateOverride(const FrequencyUnit& unit) const
{
  if (m_HeartRateOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_HeartRateOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasHeartStrokeVolumeOverride() const
{
  return m_HeartStrokeVolumeOverride == nullptr ? false : m_HeartStrokeVolumeOverride->IsValid();
}
SEScalarVolume& SEOverride::GetHeartStrokeVolumeOverride()
{
  if (m_HeartStrokeVolumeOverride == nullptr) {
    m_HeartStrokeVolumeOverride = new SEScalarVolume();
  }
  return *m_HeartStrokeVolumeOverride;
}
double SEOverride::GetHeartStrokeVolumeOverride(const VolumeUnit& unit) const
{
  if (m_HeartStrokeVolumeOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_HeartStrokeVolumeOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasSystolicArterialPressureOverride() const
{
  return m_SystolicArterialPressureOverride == nullptr ? false : m_SystolicArterialPressureOverride->IsValid();
}
SEScalarPressure& SEOverride::GetSystolicArterialPressureOverride()
{
  if (m_SystolicArterialPressureOverride == nullptr) {
    m_SystolicArterialPressureOverride = new SEScalarPressure();
  }
  return *m_SystolicArterialPressureOverride;
}
double SEOverride::GetSystolicArterialPressureOverride(const PressureUnit& unit) const
{
  if (m_SystolicArterialPressureOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_SystolicArterialPressureOverride->GetValue(unit);
}

bool SEOverride::HasCardiovascularOverride() const
{
  return HasBloodVolumeOverride()
    || HasCardiacOutputOverride()
    || HasDiastolicArterialPressureOverride()
    || HasMAPOverride()
    || HasHeartRateOverride()
    || HasHeartStrokeVolumeOverride()
    || HasSystolicArterialPressureOverride();
}

// Endocrine Overrides //
bool SEOverride::HasInsulinSynthesisRateOverride() const
{
  return m_InsulinSynthesisRateOverride == nullptr ? false : m_InsulinSynthesisRateOverride->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarAmountPerTime& SEOverride::GetInsulinSynthesisRateOverride()
{
  if (m_InsulinSynthesisRateOverride == nullptr) {
    m_InsulinSynthesisRateOverride = new SEScalarAmountPerTime();
  }
  return *m_InsulinSynthesisRateOverride;
}
double SEOverride::GetInsulinSynthesisRateOverride(const AmountPerTimeUnit& unit) const
{
  if (m_InsulinSynthesisRateOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_InsulinSynthesisRateOverride->GetValue(unit);
}
bool SEOverride::HasGlucagonSynthesisRateOverride() const
{
  return m_GlucagonSynthesisRateOverride == nullptr ? false : m_GlucagonSynthesisRateOverride->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarAmountPerTime& SEOverride::GetGlucagonSynthesisRateOverride()
{
  if (m_GlucagonSynthesisRateOverride == nullptr) {
    m_GlucagonSynthesisRateOverride = new SEScalarAmountPerTime();
  }
  return *m_GlucagonSynthesisRateOverride;
}
double SEOverride::GetGlucagonSynthesisRateOverride(const AmountPerTimeUnit& unit) const
{
  if (m_GlucagonSynthesisRateOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_GlucagonSynthesisRateOverride->GetValue(unit);
}

bool SEOverride::HasEndocrineOverride() const
{
  return HasInsulinSynthesisRateOverride()
    || HasGlucagonSynthesisRateOverride();
}

// Energy Overrides //
bool SEOverride::HasAchievedExerciseLevelOverride() const
{
  return m_AchievedExerciseLevelOverride == nullptr ? false : m_AchievedExerciseLevelOverride->IsValid();
}
SEScalarFraction& SEOverride::GetAchievedExerciseLevelOverride()
{
  if (m_AchievedExerciseLevelOverride == nullptr) {
    m_AchievedExerciseLevelOverride = new SEScalarFraction();
  }
  return *m_AchievedExerciseLevelOverride;
}
double SEOverride::GetAchievedExerciseLevelOverride() const
{
  if (m_AchievedExerciseLevelOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_AchievedExerciseLevelOverride->GetValue();
}
//-------------------------------------------------------------------------------
bool SEOverride::HasCoreTemperatureOverride() const
{
  return m_CoreTemperatureOverride == nullptr ? false : m_CoreTemperatureOverride->IsValid();
}
SEScalarTemperature& SEOverride::GetCoreTemperatureOverride()
{
  if (m_CoreTemperatureOverride == nullptr) {
    m_CoreTemperatureOverride = new SEScalarTemperature();
  }
  return *m_CoreTemperatureOverride;
}
double SEOverride::GetCoreTemperatureOverride(const TemperatureUnit& unit) const
{
  if (m_CoreTemperatureOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_CoreTemperatureOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasCreatinineProductionRateOverride() const
{
  return m_CreatinineProductionRateOverride == nullptr ? false : m_CreatinineProductionRateOverride->IsValid();
}
SEScalarAmountPerTime& SEOverride::GetCreatinineProductionRateOverride()
{
  if (m_CreatinineProductionRateOverride == nullptr) {
    m_CreatinineProductionRateOverride = new SEScalarAmountPerTime();
  }
  return *m_CreatinineProductionRateOverride;
}
double SEOverride::GetCreatinineProductionRateOverride(const AmountPerTimeUnit& unit) const
{
  if (m_CreatinineProductionRateOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_CreatinineProductionRateOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasExerciseMeanArterialPressureDeltaOverride() const
{
  return m_ExerciseMeanArterialPressureDeltaOverride == nullptr ? false : m_ExerciseMeanArterialPressureDeltaOverride->IsValid();
}
SEScalarPressure& SEOverride::GetExerciseMeanArterialPressureDeltaOverride()
{
  if (m_ExerciseMeanArterialPressureDeltaOverride == nullptr) {
    m_ExerciseMeanArterialPressureDeltaOverride = new SEScalarPressure();
  }
  return *m_ExerciseMeanArterialPressureDeltaOverride;
}
double SEOverride::GetExerciseMeanArterialPressureDeltaOverride(const PressureUnit& unit) const
{
  if (m_ExerciseMeanArterialPressureDeltaOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_ExerciseMeanArterialPressureDeltaOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasFatigueLevelOverride() const
{
  return m_FatigueLevelOverride == nullptr ? false : m_FatigueLevelOverride->IsValid();
}
SEScalarFraction& SEOverride::GetFatigueLevelOverride()
{
  if (m_FatigueLevelOverride == nullptr) {
    m_FatigueLevelOverride = new SEScalarFraction();
  }
  return *m_FatigueLevelOverride;
}
double SEOverride::GetFatigueLevelOverride() const
{
  if (m_FatigueLevelOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_FatigueLevelOverride->GetValue();
}
//-------------------------------------------------------------------------------
bool SEOverride::HasLactateProductionRateOverride() const
{
  return m_LactateProductionRateOverride == nullptr ? false : m_LactateProductionRateOverride->IsValid();
}
SEScalarAmountPerTime& SEOverride::GetLactateProductionRateOverride()
{
  if (m_LactateProductionRateOverride == nullptr) {
    m_LactateProductionRateOverride = new SEScalarAmountPerTime();
  }
  return *m_LactateProductionRateOverride;
}
double SEOverride::GetLactateProductionRateOverride(const AmountPerTimeUnit& unit) const
{
  if (m_LactateProductionRateOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_LactateProductionRateOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasSkinTemperatureOverride() const
{
  return m_SkinTemperatureOverride == nullptr ? false : m_SkinTemperatureOverride->IsValid();
}
SEScalarTemperature& SEOverride::GetSkinTemperatureOverride()
{
  if (m_SkinTemperatureOverride == nullptr) {
    m_SkinTemperatureOverride = new SEScalarTemperature();
  }
  return *m_SkinTemperatureOverride;
}
double SEOverride::GetSkinTemperatureOverride(const TemperatureUnit& unit) const
{
  if (m_SkinTemperatureOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_SkinTemperatureOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasSweatRateOverride() const
{
  return m_SweatRateOverride == nullptr ? false : m_SweatRateOverride->IsValid();
}
SEScalarMassPerTime& SEOverride::GetSweatRateOverride()
{
  if (m_SweatRateOverride == nullptr) {
    m_SweatRateOverride = new SEScalarMassPerTime();
  }
  return *m_SweatRateOverride;
}
double SEOverride::GetSweatRateOverride(const MassPerTimeUnit& unit) const
{
  if (m_SweatRateOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_SweatRateOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasTotalMetabolicOverride() const
{
  return m_TotalMetabolicRateOverride == nullptr ? false : m_TotalMetabolicRateOverride->IsValid();
}
SEScalarPower& SEOverride::GetTotalMetabolicOverride()
{
  if (m_TotalMetabolicRateOverride == nullptr) {
    m_TotalMetabolicRateOverride = new SEScalarPower();
  }
  return *m_TotalMetabolicRateOverride;
}
double SEOverride::GetTotalMetabolicOverride(const PowerUnit& unit) const
{
  if (m_TotalMetabolicRateOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TotalMetabolicRateOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasTotalWorkRateLevelOverride() const
{
  return m_TotalWorkRateLevelOverride == nullptr ? false : m_TotalWorkRateLevelOverride->IsValid();
}
SEScalarFraction& SEOverride::GetTotalWorkRateLevelOverride()
{
  if (m_TotalWorkRateLevelOverride == nullptr) {
    m_TotalWorkRateLevelOverride = new SEScalarFraction();
  }
  return *m_TotalWorkRateLevelOverride;
}
double SEOverride::GetTotalWorkRateLevelOverride() const
{
  if (m_TotalWorkRateLevelOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TotalWorkRateLevelOverride->GetValue();
}
//-------------------------------------------------------------------------------
bool SEOverride::HasSodiumLostToSweatOverride() const
{
  return m_SodiumLostToSweatOverride == nullptr ? false : m_SodiumLostToSweatOverride->IsValid();
}
SEScalarMass& SEOverride::GetSodiumLostToSweatOverride()
{
  if (m_SodiumLostToSweatOverride == nullptr) {
    m_SodiumLostToSweatOverride = new SEScalarMass();
  }
  return *m_SodiumLostToSweatOverride;
}
double SEOverride::GetSodiumLostToSweatOverride(const MassUnit& unit) const
{
  if (m_SodiumLostToSweatOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_SodiumLostToSweatOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasPotassiumLostToSweatOverride() const
{
  return m_PotassiumLostToSweatOverride == nullptr ? false : m_PotassiumLostToSweatOverride->IsValid();
}
SEScalarMass& SEOverride::GetPotassiumLostToSweatOverride()
{
  if (m_PotassiumLostToSweatOverride == nullptr) {
    m_PotassiumLostToSweatOverride = new SEScalarMass();
  }
  return *m_PotassiumLostToSweatOverride;
}
double SEOverride::GetPotassiumLostToSweatOverride(const MassUnit& unit) const
{
  if (m_PotassiumLostToSweatOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_PotassiumLostToSweatOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasChlorideLostToSweatOverride() const
{
  return m_ChlorideLostToSweatOverride == nullptr ? false : m_ChlorideLostToSweatOverride->IsValid();
}
SEScalarMass& SEOverride::GetChlorideLostToSweatOverride()
{
  if (m_ChlorideLostToSweatOverride == nullptr) {
    m_ChlorideLostToSweatOverride = new SEScalarMass();
  }
  return *m_ChlorideLostToSweatOverride;
}
double SEOverride::GetChlorideLostToSweatOverride(const MassUnit& unit) const
{
  if (m_ChlorideLostToSweatOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_ChlorideLostToSweatOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
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
  return m_LeftAfferentArterioleResistanceOverride == nullptr ? false : m_LeftAfferentArterioleResistanceOverride->IsValid();
}
SEScalarFlowResistance& SEOverride::GetLeftAfferentArterioleResistanceOverride()
{
  if (m_LeftAfferentArterioleResistanceOverride == nullptr) {
    m_LeftAfferentArterioleResistanceOverride = new SEScalarFlowResistance();
  }
  return *m_LeftAfferentArterioleResistanceOverride;
}
double SEOverride::GetLeftAfferentArterioleResistanceOverride(const FlowResistanceUnit& unit) const
{
  if (m_LeftAfferentArterioleResistanceOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_LeftAfferentArterioleResistanceOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasLeftGlomerularFiltrationRateOverride() const
{
  return m_LeftGlomerularFiltrationRateOverride == nullptr ? false : m_LeftGlomerularFiltrationRateOverride->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetLeftGlomerularFiltrationRateOverride()
{
  if (m_LeftGlomerularFiltrationRateOverride == nullptr) {
    m_LeftGlomerularFiltrationRateOverride = new SEScalarVolumePerTime();
  }
  return *m_LeftGlomerularFiltrationRateOverride;
}
double SEOverride::GetLeftGlomerularFiltrationRateOverride(const VolumePerTimeUnit& unit) const
{
  if (m_LeftGlomerularFiltrationRateOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_LeftGlomerularFiltrationRateOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasLeftReaborptionRateOverride() const
{
  return m_LeftReabsorptionRateOverride == nullptr ? false : m_LeftReabsorptionRateOverride->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetLeftReaborptionRateOverride()
{
  if (m_LeftReabsorptionRateOverride == nullptr) {
    m_LeftReabsorptionRateOverride = new SEScalarVolumePerTime();
  }
  return *m_LeftReabsorptionRateOverride;
}
double SEOverride::GetLeftReaborptionRateOverride(const VolumePerTimeUnit& unit) const
{
  if (m_LeftReabsorptionRateOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_LeftReabsorptionRateOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasRenalBloodFlowOverride() const
{
  return m_RenalBloodFlowOverride == nullptr ? false : m_RenalBloodFlowOverride->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetRenalBloodFlowOverride()
{
  if (m_RenalBloodFlowOverride == nullptr) {
    m_RenalBloodFlowOverride = new SEScalarVolumePerTime();
  }
  return *m_RenalBloodFlowOverride;
}
double SEOverride::GetRenalBloodFlowOverride(const VolumePerTimeUnit& unit) const
{
  if (m_RenalBloodFlowOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_RenalBloodFlowOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasRenalPlasmaFlowOverride() const
{
  return m_RenalPlasmaFlowOverride == nullptr ? false : m_RenalPlasmaFlowOverride->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetRenalPlasmaFlowOverride()
{
  if (m_RenalPlasmaFlowOverride == nullptr) {
    m_RenalPlasmaFlowOverride = new SEScalarVolumePerTime();
  }
  return *m_RenalPlasmaFlowOverride;
}
double SEOverride::GetRenalPlasmaFlowOverride(const VolumePerTimeUnit& unit) const
{
  if (m_RenalPlasmaFlowOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_RenalPlasmaFlowOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasRightAfferentArterioleResistanceOverride() const
{
  return m_RightAfferentArterioleResistanceOverride == nullptr ? false : m_RightAfferentArterioleResistanceOverride->IsValid();
}
SEScalarFlowResistance& SEOverride::GetRightAfferentArterioleResistanceOverride()
{
  if (m_RightAfferentArterioleResistanceOverride == nullptr) {
    m_RightAfferentArterioleResistanceOverride = new SEScalarFlowResistance();
  }
  return *m_RightAfferentArterioleResistanceOverride;
}
double SEOverride::GetRightAfferentArterioleResistanceOverride(const FlowResistanceUnit& unit) const
{
  if (m_RightAfferentArterioleResistanceOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_RightAfferentArterioleResistanceOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasRightGlomerularFiltrationRateOverride() const
{
  return m_RightGlomerularFiltrationRateOverride == nullptr ? false : m_RightGlomerularFiltrationRateOverride->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetRightGlomerularFiltrationRateOverride()
{
  if (m_RightGlomerularFiltrationRateOverride == nullptr) {
    m_RightGlomerularFiltrationRateOverride = new SEScalarVolumePerTime();
  }
  return *m_RightGlomerularFiltrationRateOverride;
}
double SEOverride::GetRightGlomerularFiltrationRateOverride(const VolumePerTimeUnit& unit) const
{
  if (m_RightGlomerularFiltrationRateOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_RightGlomerularFiltrationRateOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasRightReaborptionRateOverride() const
{
  return m_RightReabsorptionRateOverride == nullptr ? false : m_RightReabsorptionRateOverride->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetRightReaborptionRateOverride()
{
  if (m_RightReabsorptionRateOverride == nullptr) {
    m_RightReabsorptionRateOverride = new SEScalarVolumePerTime();
  }
  return *m_RightReabsorptionRateOverride;
}
double SEOverride::GetRightReaborptionRateOverride(const VolumePerTimeUnit& unit) const
{
  if (m_RightReabsorptionRateOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_RightReabsorptionRateOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasUrinationRateOverride() const
{
  return m_UrinationRateOverride == nullptr ? false : m_UrinationRateOverride->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetUrinationRateOverride()
{
  if (m_UrinationRateOverride == nullptr) {
    m_UrinationRateOverride = new SEScalarVolumePerTime();
  }
  return *m_UrinationRateOverride;
}
double SEOverride::GetUrinationRateOverride(const VolumePerTimeUnit& unit) const
{
  if (m_UrinationRateOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_UrinationRateOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasUrineProductionRateOverride() const
{
  return m_UrineProductionRateOverride == nullptr ? false : m_UrineProductionRateOverride->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetUrineProductionRateOverride()
{
  if (m_UrineProductionRateOverride == nullptr) {
    m_UrineProductionRateOverride = new SEScalarVolumePerTime();
  }
  return *m_UrineProductionRateOverride;
}
double SEOverride::GetUrineProductionRateOverride(const VolumePerTimeUnit& unit) const
{
  if (m_UrineProductionRateOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_UrineProductionRateOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasUrineOsmolalityOverride() const
{
  return m_UrineOsmolalityOverride == nullptr ? false : m_UrineOsmolalityOverride->IsValid();
}
SEScalarOsmolality& SEOverride::GetUrineOsmolalityOverride()
{
  if (m_UrineOsmolalityOverride == nullptr) {
    m_UrineOsmolalityOverride = new SEScalarOsmolality();
  }
  return *m_UrineOsmolalityOverride;
}
double SEOverride::GetUrineOsmolalityOverride(const OsmolalityUnit& unit) const
{
  if (m_UrineOsmolalityOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_UrineOsmolalityOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasUrineVolumeOverride() const
{
  return m_UrineVolumeOverride == nullptr ? false : m_UrineVolumeOverride->IsValid();
}
SEScalarVolume& SEOverride::GetUrineVolumeOverride()
{
  if (m_UrineVolumeOverride == nullptr) {
    m_UrineVolumeOverride = new SEScalarVolume();
  }
  return *m_UrineVolumeOverride;
}
double SEOverride::GetUrineVolumeOverride(const VolumeUnit& unit) const
{
  if (m_UrineVolumeOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_UrineVolumeOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasUrineUreaNitrogenConcentrationOverride() const
{
  return m_UrineUreaNitrogenConcentrationOverride == nullptr ? false : m_UrineUreaNitrogenConcentrationOverride->IsValid();
}
SEScalarMassPerVolume& SEOverride::GetUrineUreaNitrogenConcentrationOverride()
{
  if (m_UrineUreaNitrogenConcentrationOverride == nullptr) {
    m_UrineUreaNitrogenConcentrationOverride = new SEScalarMassPerVolume();
  }
  return *m_UrineUreaNitrogenConcentrationOverride;
}
double SEOverride::GetUrineUreaNitrogenConcentrationOverride(const MassPerVolumeUnit& unit) const
{
  if (m_UrineUreaNitrogenConcentrationOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_UrineUreaNitrogenConcentrationOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasRenalOverride() const
{
  return HasLeftAfferentArterioleResistanceOverride()
    || HasLeftGlomerularFiltrationRateOverride()
    || HasLeftReaborptionRateOverride()
    || HasRenalBloodFlowOverride()
    || HasRenalPlasmaFlowOverride()
    || HasRightAfferentArterioleResistanceOverride()
    || HasRightGlomerularFiltrationRateOverride()
    || HasRightReaborptionRateOverride()
    || HasUrinationRateOverride()
    || HasUrineProductionRateOverride()
    || HasUrineOsmolalityOverride()
    || HasUrineVolumeOverride()
    || HasUrineUreaNitrogenConcentrationOverride();
}

// Respiratory Overrides //
bool SEOverride::HasExpiratoryFlowOverride() const
{
  return m_ExpiratoryFlowOverride == nullptr ? false : m_ExpiratoryFlowOverride->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetExpiratoryFlowOverride()
{
  if (m_ExpiratoryFlowOverride == nullptr) {
    m_ExpiratoryFlowOverride = new SEScalarVolumePerTime();
  }
  return *m_ExpiratoryFlowOverride;
}
double SEOverride::GetExpiratoryFlowOverride(const VolumePerTimeUnit& unit) const
{
  if (m_ExpiratoryFlowOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_ExpiratoryFlowOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasInspiratoryFlowOverride() const
{
  return m_InspiratoryFlowOverride == nullptr ? false : m_InspiratoryFlowOverride->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetInspiratoryFlowOverride()
{
  if (m_InspiratoryFlowOverride == nullptr) {
    m_InspiratoryFlowOverride = new SEScalarVolumePerTime();
  }
  return *m_InspiratoryFlowOverride;
}
double SEOverride::GetInspiratoryFlowOverride(const VolumePerTimeUnit& unit) const
{
  if (m_InspiratoryFlowOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_InspiratoryFlowOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasPulmonaryComplianceOverride() const
{
  return m_PulmonaryComplianceOverride == nullptr ? false : m_PulmonaryComplianceOverride->IsValid();
}
SEScalarFlowCompliance& SEOverride::GetPulmonaryComplianceOverride()
{
  if (m_PulmonaryComplianceOverride == nullptr) {
    m_PulmonaryComplianceOverride = new SEScalarFlowCompliance();
  }
  return *m_PulmonaryComplianceOverride;
}
double SEOverride::GetPulmonaryComplianceOverride(const FlowComplianceUnit& unit) const
{
  if (m_PulmonaryComplianceOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_PulmonaryComplianceOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasPulmonaryResistanceOverride() const
{
  return m_PulmonaryResistanceOverride == nullptr ? false : m_PulmonaryResistanceOverride->IsValid();
}
SEScalarFlowResistance& SEOverride::GetPulmonaryResistanceOverride()
{
  if (m_PulmonaryResistanceOverride == nullptr) {
    m_PulmonaryResistanceOverride = new SEScalarFlowResistance();
  }
  return *m_PulmonaryResistanceOverride;
}
double SEOverride::GetPulmonaryResistanceOverride(const FlowResistanceUnit& unit) const
{
  if (m_PulmonaryResistanceOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_PulmonaryResistanceOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasRespirationRateOverride() const
{
  return m_RespirationRateOverride == nullptr ? false : m_RespirationRateOverride->IsValid();
}
SEScalarFrequency& SEOverride::GetRespirationRateOverride()
{
  if (m_RespirationRateOverride == nullptr) {
    m_RespirationRateOverride = new SEScalarFrequency();
  }
  return *m_RespirationRateOverride;
}
double SEOverride::GetRespirationRateOverride(const FrequencyUnit& unit) const
{
  if (m_RespirationRateOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_RespirationRateOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasTidalVolumeOverride() const
{
  return m_TidalVolumeOverride == nullptr ? false : m_TidalVolumeOverride->IsValid();
}
SEScalarVolume& SEOverride::GetTidalVolumeOverride()
{
  if (m_TidalVolumeOverride == nullptr) {
    m_TidalVolumeOverride = new SEScalarVolume();
  }
  return *m_TidalVolumeOverride;
}
double SEOverride::GetTidalVolumeOverride(const VolumeUnit& unit) const
{
  if (m_TidalVolumeOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TidalVolumeOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasTargetPulmonaryVentilationOverride() const
{
  return m_TargetPulmonaryVentilationOverride == nullptr ? false : m_TargetPulmonaryVentilationOverride->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetTargetPulmonaryVentilationOverride()
{
  if (m_TargetPulmonaryVentilationOverride == nullptr) {
    m_TargetPulmonaryVentilationOverride = new SEScalarVolumePerTime();
  }
  return *m_TargetPulmonaryVentilationOverride;
}
double SEOverride::GetTargetPulmonaryVentilationOverride(const VolumePerTimeUnit& unit) const
{
  if (m_TargetPulmonaryVentilationOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TargetPulmonaryVentilationOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasTotalAlveolarVentilationOverride() const
{
  return m_TotalAlveolarVentilationOverride == nullptr ? false : m_TotalAlveolarVentilationOverride->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetTotalAlveolarVentilationOverride()
{
  if (m_TotalAlveolarVentilationOverride == nullptr) {
    m_TotalAlveolarVentilationOverride = new SEScalarVolumePerTime();
  }
  return *m_TotalAlveolarVentilationOverride;
}
double SEOverride::GetTotalAlveolarVentilationOverride(const VolumePerTimeUnit& unit) const
{
  if (m_TotalAlveolarVentilationOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TotalAlveolarVentilationOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasTotalLungVolumeOverride() const
{
  return m_TotalLungVolumeOverride == nullptr ? false : m_TotalLungVolumeOverride->IsValid();
}
SEScalarVolume& SEOverride::GetTotalLungVolumeOverride()
{
  if (m_TotalLungVolumeOverride == nullptr) {
    m_TotalLungVolumeOverride = new SEScalarVolume();
  }
  return *m_TotalLungVolumeOverride;
}
double SEOverride::GetTotalLungVolumeOverride(const VolumeUnit& unit) const
{
  if (m_TotalLungVolumeOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TotalLungVolumeOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasTotalPulmonaryVentilationOverride() const
{
  return m_TotalPulmonaryVentilationOverride == nullptr ? false : m_TotalPulmonaryVentilationOverride->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetTotalPulmonaryVentilationOverride()
{
  if (m_TotalPulmonaryVentilationOverride == nullptr) {
    m_TotalPulmonaryVentilationOverride = new SEScalarVolumePerTime();
  }
  return *m_TotalPulmonaryVentilationOverride;
}
double SEOverride::GetTotalPulmonaryVentilationOverride(const VolumePerTimeUnit& unit) const
{
  if (m_TotalPulmonaryVentilationOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TotalPulmonaryVentilationOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasRespiratoryOverride() const
{
  return HasExpiratoryFlowOverride()
    || HasInspiratoryFlowOverride()
    || HasPulmonaryComplianceOverride()
    || HasPulmonaryResistanceOverride()
    || HasRespirationRateOverride()
    || HasTidalVolumeOverride()
    || HasTargetPulmonaryVentilationOverride()
    || HasTotalAlveolarVentilationOverride()
    || HasTotalLungVolumeOverride()
    || HasTotalPulmonaryVentilationOverride();
}

// Tissue Overrides //
bool SEOverride::HasExtravascularFluidVolumeOverride() const
{
  return m_ExtravascularFluidVolumeOverride == nullptr ? false : m_ExtravascularFluidVolumeOverride->IsValid();
}
SEScalarVolume& SEOverride::GetExtravascularFluidVolumeOverride()
{
  if (m_ExtravascularFluidVolumeOverride == nullptr) {
    m_ExtravascularFluidVolumeOverride = new SEScalarVolume();
  }
  return *m_ExtravascularFluidVolumeOverride;
}
double SEOverride::GetExtravascularFluidVolumeOverride(const VolumeUnit& unit) const
{
  if (m_ExtravascularFluidVolumeOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_ExtravascularFluidVolumeOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasIntracellularFluidVolumeOverride() const
{
  return m_IntracellularFluidVolumeOverride == nullptr ? false : m_IntracellularFluidVolumeOverride->IsValid();
}
SEScalarVolume& SEOverride::GetIntracellularFluidVolumeOverride()
{
  if (m_IntracellularFluidVolumeOverride == nullptr) {
    m_IntracellularFluidVolumeOverride = new SEScalarVolume();
  }
  return *m_IntracellularFluidVolumeOverride;
}
double SEOverride::GetIntracellularFluidVolumeOverride(const VolumeUnit& unit) const
{
  if (m_IntracellularFluidVolumeOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_IntracellularFluidVolumeOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasLiverGlycogenOverride() const
{
  return m_LiverGlycogenOverride == nullptr ? false : m_LiverGlycogenOverride->IsValid();
}
SEScalarMass& SEOverride::GetLiverGlycogenOverride()
{
  if (m_LiverGlycogenOverride == nullptr) {
    m_LiverGlycogenOverride = new SEScalarMass();
  }
  return *m_LiverGlycogenOverride;
}
double SEOverride::GetLiverGlycogenOverride(const MassUnit& unit) const
{
  if (m_LiverGlycogenOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_LiverGlycogenOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasMuscleGlycogenOverride() const
{
  return m_MuscleGlycogenOverride == nullptr ? false : m_MuscleGlycogenOverride->IsValid();
}
SEScalarMass& SEOverride::GetMuscleGlycogenOverride()
{
  if (m_MuscleGlycogenOverride == nullptr) {
    m_MuscleGlycogenOverride = new SEScalarMass();
  }
  return *m_MuscleGlycogenOverride;
}
double SEOverride::GetMuscleGlycogenOverride(const MassUnit& unit) const
{
  if (m_MuscleGlycogenOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_MuscleGlycogenOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasStoredProteinOverride() const
{
  return m_StoredProteinOverride == nullptr ? false : m_StoredProteinOverride->IsValid();
}
SEScalarMass& SEOverride::GetStoredProteinOverride()
{
  if (m_StoredProteinOverride == nullptr) {
    m_StoredProteinOverride = new SEScalarMass();
  }
  return *m_StoredProteinOverride;
}
double SEOverride::GetStoredProteinOverride(const MassUnit& unit) const
{
  if (m_StoredProteinOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_StoredProteinOverride->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEOverride::HasStoredFatOverride() const
{
  return m_StoredFatOverride == nullptr ? false : m_StoredFatOverride->IsValid();
}
SEScalarMass& SEOverride::GetStoredFatOverride()
{
  if (m_StoredFatOverride == nullptr) {
    m_StoredFatOverride = new SEScalarMass();
  }
  return *m_StoredFatOverride;
}
double SEOverride::GetStoredFatOverride(const MassUnit& unit) const
{
  if (m_StoredFatOverride == nullptr) {
    return SEScalar::dNaN();
  }
  return m_StoredFatOverride->GetValue(unit);
}

bool SEOverride::HasTissueOverride() const
{
  return HasExtravascularFluidVolumeOverride()
    || HasIntracellularFluidVolumeOverride()
    || HasLiverGlycogenOverride()
    || HasMuscleGlycogenOverride()
    || HasStoredProteinOverride()
    || HasStoredFatOverride();
}
//-------------------------------------------------------------------------------
// String //
void SEOverride::ToString(std::ostream& str) const
{
  str << "Patient Action : Override Parameter";
  if (HasComment()) {
    str << "\n\tComment: " << m_Comment;
  }

  str << "\n\tState: ";
  HasOverrideState() ? str << GetOverrideState() : str << "Not Set";
  str << "\n\tConformant: ";
  HasOverrideConformance() ? str << GetOverrideConformance() : str << "Not Set";
  if (GetOverrideConformance() == CDM::enumOnOff::Off
      && GetOverrideState() == CDM::enumOnOff::On) {
    str << ("\n\tOverride has turned conformance off. Outputs no longer resemble validated parameters.");
  }
  if (HasArterialPHOverride()) {
    str << "\n\tArterial PH: ";
    HasArterialPHOverride() ? str << *m_ArterialBloodPHOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tArterial PH has a lower bound of 0 and an upper bound of 14.";
    }
    str << std::flush;
  }
  if (HasVenousPHOverride()) {
    str << "\n\tVenous PH: ";
    HasVenousPHOverride() ? str << *m_VenousBloodPHOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tVenous PH has a lower bound of 0 and an upper bound of 14.";
    }
    str << std::flush;
  }
  if (HasCO2SaturationOverride()) {
    str << "\n\tCarbon Dioxide Saturation: ";
    HasCO2SaturationOverride() ? str << *m_CarbonDioxideSaturationOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tCO2 Saturation has a lower bound of 0 and an upper bound of 1.";
    }
    str << std::flush;
  }
  if (HasCOSaturationOverride()) {
    str << "\n\tCarbon Monoxide Saturation: ";
    HasCOSaturationOverride() ? str << *m_CarbonMonoxideSaturationOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tCO Saturation has a lower bound of 0 and an upper bound of 1.";
    }
    str << std::flush;
  }
  if (HasO2SaturationOverride()) {
    str << "\n\tOxygen Saturation: ";
    HasO2SaturationOverride() ? str << *m_OxygenSaturationOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tO2 Saturation has a lower bound of 0 and an upper bound of 1.";
    }
    str << std::flush;
  }
  if (HasPhosphateOverride()) {
    str << "\n\tPhosphate: ";
    HasPhosphateOverride() ? str << *m_PhosphateOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tPhosphate has a lower bound of 0 mmol/L and an upper bound of 1000 mmol/L.";
    }
    str << std::flush;
  }
  if (HasWBCCountOverride()) {
    str << "\n\tWhite Blood Cell Count: ";
    HasWBCCountOverride() ? str << *m_WhiteBloodCellCountOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tWBC Count has a lower bound of 0 ct/uL and an upper bound of 50000 ct/uL.";
    }
    str << std::flush;
  }
  if (HasTotalBilirubinOverride()) {
    str << "\n\tTotal Bilirubin: ";
    HasTotalBilirubinOverride() ? str << *m_TotalBilirubinOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tTotal Bilirubin has a lower bound of 0 mg/dL and an upper bound of 500 mg/dL.";
    }
    str << std::flush;
  }
  if (HasCalciumConcentrationOverride()) {
    str << "\n\tCalcium Concentration: ";
    HasCalciumConcentrationOverride() ? str << *m_CalciumConcentrationOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tCalcium Concentration has a lower bound of 0 mg/dL and an upper bound of 500 mg/dL.";
    }
    str << std::flush;
  }
  if (HasGlucoseConcentrationOverride()) {
    str << "\n\tGlucose Concentration: ";
    HasGlucoseConcentrationOverride() ? str << *m_GlucoseConcentrationOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tGlucose Concentration has a lower bound of 0 mg/dL and an upper bound of 1000 mg/dL.";
    }
    str << std::flush;
  }
  if (HasLactateConcentrationOverride()) {
    str << "\n\tLactate Concentration: ";
    HasLactateConcentrationOverride() ? str << *m_LactateConcentrationOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tLactate Concentration has a lower bound of 0 mg/dL and an upper bound of 1000 mg/dL.";
    }
    str << std::flush;
  }
  if (HasPotassiumConcentrationOverride()) {
    str << "\n\tPotassium Concentration: ";
    HasPotassiumConcentrationOverride() ? str << *m_PotassiumConcentrationOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tPotassium Concentration has a lower bound of 0 mg/dL and an upper bound of 500 mg/dL.";
    }
    str << std::flush;
  }
  if (HasSodiumConcentrationOverride()) {
    str << "\n\tSodium Concentration: ";
    HasSodiumConcentrationOverride() ? str << *m_SodiumConcentrationOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tSodium Concentration has a lower bound of 0 mg/dL and an upper bound of 500 mg/dL.";
    }
    str << std::flush;
  }
  if (HasBloodVolumeOverride()) {
    str << "\n\tBlood Volume: ";
    HasBloodVolumeOverride() ? str << *m_BloodVolumeOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tBlood Volume has a lower bound of 0 L and an upper bound of 25 L.";
    }
    str << std::flush;
  }
  if (HasCardiacOutputOverride()) {
    str << "\n\tCardiac Output: ";
    HasCardiacOutputOverride() ? str << *m_CardiacOutputOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tCardiac Output has a lower bound of 0 L/min and an upper bound of 100 L/min.";
    }
    str << std::flush;
  }
  if (HasDiastolicArterialPressureOverride()) {
    str << "\n\tDiastolic Arterial Pressure: ";
    HasDiastolicArterialPressureOverride() ? str << *m_DiastolicArterialPressureOverride : str << "Not Set";
    str << std::flush;
  }
  if (HasMAPOverride()) {
    str << "\n\tMean Arterial Pressure: ";
    HasMAPOverride() ? str << *m_MeanArterialPressureOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::Off) {
      str << "\n\tPharmacodynamics affecting this value have been turned off due to conformance being turned off.";
    }
    str << std::flush;
  }
  if (HasHeartRateOverride()) {
    str << "\n\tHeart Rate: ";
    HasHeartRateOverride() ? str << *m_HeartRateOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tHeart Rate has a lower/upper bound dependent on the patient.";
    } else {
      str << "\n\tPharmacodynamics affecting this value have been turned off due to conformance being turned off.";
    }
    str << std::flush;
  }
  if (HasHeartStrokeVolumeOverride()) {
    str << "\n\tHeart Stroke Volume: ";
    HasHeartStrokeVolumeOverride() ? str << *m_HeartStrokeVolumeOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tHeart Stroke Volume has a lower bound of 0 mL and an upper bound of 5000 mL.";
    }
    str << std::flush;
  }
  if (HasSystolicArterialPressureOverride()) {
    str << "\n\tSystolic Arterial Pressure: ";
    HasSystolicArterialPressureOverride() ? str << *m_SystolicArterialPressureOverride : str << "Not Set";
    str << std::flush;
  }
  if (HasInsulinSynthesisRateOverride()) {
    str << "\n\tInsulin Synthesis Rate: ";
    HasInsulinSynthesisRateOverride() ? str << *m_InsulinSynthesisRateOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tInsulin Synthesis Rate has a lower bound of 0 pmol/min and an upper bound of 10 pmol/min.";
    }
    str << std::flush;
  }
  if (HasGlucagonSynthesisRateOverride()) {
    str << "\n\tGlucagon Synthesis Rate: ";
    HasGlucagonSynthesisRateOverride() ? str << *m_GlucagonSynthesisRateOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tGlucagon Synthesis Rate has a lower bound of 0 pmol/min and an upper bound of 10 pmol/min.";
    }
    str << std::flush;
  }
  if (HasAchievedExerciseLevelOverride()) {
    str << "\n\tAcieved Exercise Level: ";
    HasAchievedExerciseLevelOverride() ? str << *m_AchievedExerciseLevelOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tAcheived Exercise Level has a lower bound of 0 and an upper bound of 1.";
    }
    str << std::flush;
  }
  if (HasCoreTemperatureOverride()) {
    str << "\n\tCore Temperature: ";
    HasCoreTemperatureOverride() ? str << *m_CoreTemperatureOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tCore Temperature has a lower bound of 0 degrees Celsius and an upper bound of 200 degrees Celsius.";
    }
    str << std::flush;
  }
  if (HasCreatinineProductionRateOverride()) {
    str << "\n\tCreatinine Production Rate: ";
    HasCreatinineProductionRateOverride() ? str << *m_CreatinineProductionRateOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tCreatinine Production has a lower bound of 0 mol/s and an upper bound of 100 mol/s.";
    }
    str << std::flush;
  }
  if (HasExerciseMeanArterialPressureDeltaOverride()) {
    str << "\n\tExercise MAP Delta: ";
    HasExerciseMeanArterialPressureDeltaOverride() ? str << *m_ExerciseMeanArterialPressureDeltaOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tExercise MAP Delta has a lower bound of 0 mmHg and an upper bound of 200 mmHg.";
    }
    str << std::flush;
  }
  if (HasFatigueLevelOverride()) {
    str << "\n\tFatigue Level: ";
    HasFatigueLevelOverride() ? str << *m_FatigueLevelOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tFatigue Level has a lower bound of 0 and an upper bound of 1.";
    }
    str << std::flush;
  }
  if (HasLactateProductionRateOverride()) {
    str << "\n\tLactate Production: ";
    HasLactateProductionRateOverride() ? str << *m_LactateProductionRateOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tLactate Production has a lower bound of 0 mol/s and an upper bound of 200 mol/s.";
    }
    str << std::flush;
  }
  if (HasSkinTemperatureOverride()) {
    str << "\n\tSkin Temperature: ";
    HasSkinTemperatureOverride() ? str << *m_SkinTemperatureOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tSkin Temperature has a lower bound of 0 degrees Celsius and an upper bound of 200 degrees Celsius.";
    }
    str << std::flush;
  }
  if (HasSweatRateOverride()) {
    str << "\n\tSweat Rate: ";
    HasSweatRateOverride() ? str << *m_SweatRateOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tSweat Rate has a lower bound of 0 g/s and an upper bound of 50 g/s.";
    }
    str << std::flush;
  }
  if (HasTotalMetabolicOverride()) {
    str << "\n\tTotal Metabolic Rate: ";
    HasTotalMetabolicOverride() ? str << *m_TotalMetabolicRateOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tTotal Metabolic Rate has a lower bound of 1 kcal/day and an upper bound of 5000 kcal/day.";
    }
    str << std::flush;
  }
  if (HasTotalWorkRateLevelOverride()) {
    str << "\n\tTotal Work Rate Level: ";
    HasTotalWorkRateLevelOverride() ? str << *m_TotalWorkRateLevelOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tTotal Work Rate Level has a lower bound of 0 and an upper bound of 1.";
    }
    str << std::flush;
  }
  if (HasSodiumLostToSweatOverride()) {
    str << "\n\tSodium Lost to Sweat: ";
    HasSodiumLostToSweatOverride() ? str << *m_SodiumLostToSweatOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tSodium Lost to Sweat has a lower bound of 0 g and an upper bound of 500 g.";
    }
    str << std::flush;
  }
  if (HasPotassiumLostToSweatOverride()) {
    str << "\n\tPotassium Lost to Sweat: ";
    HasPotassiumLostToSweatOverride() ? str << *m_PotassiumLostToSweatOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tPotassium Lost to Sweat has a lower bound of 0 g and an upper bound of 500 g.";
    }
    str << std::flush;
  }
  if (HasChlorideLostToSweatOverride()) {
    str << "\n\tChloride Lost to Sweat: ";
    HasChlorideLostToSweatOverride() ? str << *m_ChlorideLostToSweatOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tChloride Lost to Sweat has a lower bound of 0 g and an upper bound of 500 g.";
    }
    str << std::flush;
  }
  if (HasLeftAfferentArterioleResistanceOverride()) {
    str << "\n\tLeft Afferent Arteriole Resistance: ";
    HasLeftAfferentArterioleResistanceOverride() ? str << *m_LeftAfferentArterioleResistanceOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tLeft Afferent Arteriole Resistance has a lower bound of 0 mmHg min/mL and an upper bound of 1 mmHg min/mL.";
    }
    str << std::flush;
  }
  if (HasLeftGlomerularFiltrationRateOverride()) {
    str << "\n\tLeft Glomerular Filtration Rate: ";
    HasLeftGlomerularFiltrationRateOverride() ? str << *m_LeftGlomerularFiltrationRateOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tLeft Glomerular Filtration Rate has a lower bound of 0 mL/min and an upper bound of 1000 mL/min.";
    }
    str << std::flush;
  }
  if (HasLeftReaborptionRateOverride()) {
    str << "\n\tLeft Reabsorption Rate: ";
    HasLeftReaborptionRateOverride() ? str << *m_LeftReabsorptionRateOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tLeft Reabsorption Rate has a lower bound of 0 mL/min and an upper bound of 1000 mL/min.";
    }
    str << std::flush;
  }
  if (HasRenalBloodFlowOverride()) {
    str << "\n\tRenal Blood Flow: ";
    HasRenalBloodFlowOverride() ? str << *m_RenalBloodFlowOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tRenal Blood Flow has a lower bound of 0 mL/min and an upper bound of 3000 mL/min.";
    }
    str << std::flush;
  }
  if (HasRenalPlasmaFlowOverride()) {
    str << "\n\tRenal Plasma Flow: ";
    HasRenalPlasmaFlowOverride() ? str << *m_RenalPlasmaFlowOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tRenal Plasma Flow has a lower bound of 0 mL/min and an upper bound of 3000 mL/min.";
    }
    str << std::flush;
  }
  if (HasRightAfferentArterioleResistanceOverride()) {
    str << "\n\tRight Afferent Arteriole Resistance: ";
    HasRightAfferentArterioleResistanceOverride() ? str << *m_RightAfferentArterioleResistanceOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tRight Afferent Arteriole Resistance has a lower bound of 0 mmHg min/mL and an upper bound of 1 mmHg min/mL.";
    }
    str << std::flush;
  }
  if (HasRightGlomerularFiltrationRateOverride()) {
    str << "\n\tRight Glomerular Filtration Rate: ";
    HasRightGlomerularFiltrationRateOverride() ? str << *m_RightGlomerularFiltrationRateOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tRight Glomerular Filtration Rate has a lower bound of 0 mL/min and an upper bound of 1000 mL/min.";
    }
    str << std::flush;
  }
  if (HasRightReaborptionRateOverride()) {
    str << "\n\tRight Reabsorption Rate: ";
    HasRightReaborptionRateOverride() ? str << *m_RightReabsorptionRateOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tRight Reabsorption Rate has a lower bound of 0 mL/min and an upper bound of 1000 mL/min.";
    }
    str << std::flush;
  }
  if (HasUrinationRateOverride()) {
    str << "\n\tUrination Rate: ";
    HasUrinationRateOverride() ? str << *m_UrinationRateOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tUrination Rate has a lower bound of 0 mL/min and an upper bound of 1000 mL/min.";
    }
    str << std::flush;
  }
  if (HasUrineProductionRateOverride()) {
    str << "\n\tUrine Production Rate: ";
    HasUrineProductionRateOverride() ? str << *m_UrineProductionRateOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tUrine Production Rate has a lower bound of 0 mL/min and an upper bound of 100 mL/min.";
    }
    str << std::flush;
  }
  if (HasUrineOsmolalityOverride()) {
    str << "\n\tUrine Osmolality: ";
    HasUrineOsmolalityOverride() ? str << *m_UrineOsmolalityOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tUrine Osmolality has a lower bound of 0 mOsm/kg and an upper bound of 2000 mOsm/kg.";
    }
    str << std::flush;
  }
  if (HasUrineVolumeOverride()) {
    str << "\n\tUrine Volume: ";
    HasUrineVolumeOverride() ? str << *m_UrineVolumeOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tUrine Volume has a lower bound of 0 mL and an upper bound of 1000 mL.";
    }
    str << std::flush;
  }
  if (HasUrineUreaNitrogenConcentrationOverride()) {
    str << "\n\tUrine Urea Nitrogen Concentration: ";
    HasUrineUreaNitrogenConcentrationOverride() ? str << *m_UrineUreaNitrogenConcentrationOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tUrine Urea Nitrogen Concentration has a lower bound of 0 g/L and an upper bound of 100 g/L.";
    }
    str << std::flush;
  }
  if (HasExpiratoryFlowOverride()) {
    str << "\n\tExpiratory Flow: ";
    HasExpiratoryFlowOverride() ? str << *m_ExpiratoryFlowOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tExpiratory Flow has a lower bound of 0 L/min and an upper bound of 1000 L/min.";
    }
    str << std::flush;
  }
  if (HasInspiratoryFlowOverride()) {
    str << "\n\tInspiratory Flow: ";
    HasInspiratoryFlowOverride() ? str << *m_InspiratoryFlowOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tInspiratory Flow has a lower bound of 0 L/min and an upper bound of 1000 L/min.";
    }
    str << std::flush;
  }
  if (HasPulmonaryComplianceOverride()) {
    str << "\n\tPulmonary Compliance: ";
    HasPulmonaryComplianceOverride() ? str << *m_PulmonaryComplianceOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tPulmonary Compliance has a lower bound of 0 L/cmH2O and an upper bound of 1000 L/cmH20.";
    }
    str << std::flush;
  }
  if (HasPulmonaryResistanceOverride()) {
    str << "\n\tPulmonary Resistance: ";
    HasPulmonaryResistanceOverride() ? str << *m_PulmonaryResistanceOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tPulmonary Resistance has a lower bound of 0 cmH2O*s/L and an upper bound of 1000 cmH2O*s/L.";
    }
    str << std::flush;
  }
  if (HasRespirationRateOverride()) {
    str << "\n\tRespiration Rate: ";
    HasRespirationRateOverride() ? str << *m_RespirationRateOverride : str << "Not Set";
    str << std::flush;
  }
  if (HasTidalVolumeOverride()) {
    str << "\n\tTidal Volume: ";
    HasTidalVolumeOverride() ? str << *m_TidalVolumeOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tTidal Volume has a lower bound of 0 mL and an upper bound of 10000 mL.";
    }
    str << std::flush;
  }
  if (HasTargetPulmonaryVentilationOverride()) {
    str << "\n\tTarget Pulmonary Ventilation: ";
    HasTargetPulmonaryVentilationOverride() ? str << *m_TargetPulmonaryVentilationOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tTarget Pulmonary Ventilation has a lower bound of 0 L/min and an upper bound of 1000 L/min.";
    }
    str << std::flush;
  }
  if (HasTotalAlveolarVentilationOverride()) {
    str << "\n\tTotal Alveolar Ventilation: ";
    HasTotalAlveolarVentilationOverride() ? str << *m_TotalAlveolarVentilationOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tTotal Alveolar Ventilation has a lower bound of 0 L/min and an upper bound of 1000 L/min.";
    }
    str << std::flush;
  }
  if (HasTotalLungVolumeOverride()) {
    str << "\n\tTotal Lung Volume: ";
    HasTotalLungVolumeOverride() ? str << *m_TotalLungVolumeOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tTotal Lung Volume has a lower bound of 0 L and an upper bound of 500 L.";
    }
    str << std::flush;
  }
  if (HasTotalPulmonaryVentilationOverride()) {
    str << "\n\tTotal Pulmonary Ventilation: ";
    HasTotalPulmonaryVentilationOverride() ? str << *m_TotalPulmonaryVentilationOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tTotal Pulmonary Ventilation has a lower bound of 0 L/min and an upper bound of 1000 L/min.";
    }
    str << std::flush;
  }
  if (HasExtravascularFluidVolumeOverride()) {
    str << "\n\tExtravascular Fluid Volume: ";
    HasExtravascularFluidVolumeOverride() ? str << *m_ExtravascularFluidVolumeOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tExtravascular Fluid Volume has a lower bound of 0 L and an upper bound of 1000 L.";
    }
    str << std::flush;
  }
  if (HasIntracellularFluidVolumeOverride()) {
    str << "\n\tIntracellular Fluid Volume: ";
    HasIntracellularFluidVolumeOverride() ? str << *m_IntracellularFluidVolumeOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tIntracellular Fluid Volume has a lower bound of 0 L and an upper bound of 1000 L.";
    }
    str << std::flush;
  }
  if (HasLiverGlycogenOverride()) {
    str << "\n\tLiver Glycogen: ";
    HasLiverGlycogenOverride() ? str << *m_LiverGlycogenOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tLiver Glycogen has a lower bound of 0 g and an upper bound of 1000 g.";
    }
    str << std::flush;
  }
  if (HasMuscleGlycogenOverride()) {
    str << "\n\tMuscle Glycogen: ";
    HasMuscleGlycogenOverride() ? str << *m_MuscleGlycogenOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tMuscle Glycogen has a lower bound of 0 g and an upper bound of 2000 g.";
    }
    str << std::flush;
  }
  if (HasStoredProteinOverride()) {
    str << "\n\tStored Protein: ";
    HasStoredProteinOverride() ? str << *m_StoredProteinOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tStored Protein has a lower bound of 0 g and an upper bound of 5000 g.";
    }
    str << std::flush;
  }
  if (HasStoredFatOverride()) {
    str << "\n\tStored Fat: ";
    HasStoredFatOverride() ? str << *m_StoredFatOverride : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tStored Fat has a lower bound of 0 g and an upper bound of 5000 g.";
    }
    str << std::flush;
  }
}
//-------------------------------------------------------------------------------
bool SEOverride::operator==(const SEOverride& rhs) const
{
  bool equivilant = m_Comment == rhs.m_Comment;
  equivilant &= m_OverrideState == rhs.m_OverrideState;
  equivilant &= m_OverrideConformance == rhs.m_OverrideConformance;
  equivilant &= (m_ArterialBloodPHOverride && rhs.m_ArterialBloodPHOverride) ? m_ArterialBloodPHOverride->operator==(*rhs.m_ArterialBloodPHOverride) : m_ArterialBloodPHOverride == rhs.m_ArterialBloodPHOverride;
  equivilant &= (m_VenousBloodPHOverride && rhs.m_VenousBloodPHOverride) ? m_VenousBloodPHOverride->operator==(*rhs.m_VenousBloodPHOverride) : m_VenousBloodPHOverride == rhs.m_VenousBloodPHOverride;
  equivilant &= (m_CarbonDioxideSaturationOverride && rhs.m_CarbonDioxideSaturationOverride) ? m_CarbonDioxideSaturationOverride->operator==(*rhs.m_CarbonDioxideSaturationOverride) : m_CarbonDioxideSaturationOverride == rhs.m_CarbonDioxideSaturationOverride;
  equivilant &= (m_CarbonMonoxideSaturationOverride && rhs.m_CarbonMonoxideSaturationOverride) ? m_CarbonMonoxideSaturationOverride->operator==(*rhs.m_CarbonMonoxideSaturationOverride) : m_CarbonMonoxideSaturationOverride == rhs.m_CarbonMonoxideSaturationOverride;
  equivilant &= (m_OxygenSaturationOverride && rhs.m_OxygenSaturationOverride) ? m_OxygenSaturationOverride->operator==(*rhs.m_OxygenSaturationOverride) : m_OxygenSaturationOverride == rhs.m_OxygenSaturationOverride;

  equivilant &= (m_PhosphateOverride && rhs.m_PhosphateOverride) ? m_PhosphateOverride->operator==(*rhs.m_PhosphateOverride) : m_PhosphateOverride == rhs.m_PhosphateOverride;
  equivilant &= (m_WhiteBloodCellCountOverride && rhs.m_WhiteBloodCellCountOverride) ? m_WhiteBloodCellCountOverride->operator==(*rhs.m_WhiteBloodCellCountOverride) : m_WhiteBloodCellCountOverride == rhs.m_WhiteBloodCellCountOverride;
  equivilant &= (m_TotalBilirubinOverride && rhs.m_TotalBilirubinOverride) ? m_TotalBilirubinOverride->operator==(*rhs.m_TotalBilirubinOverride) : m_TotalBilirubinOverride == rhs.m_TotalBilirubinOverride;
  equivilant &= (m_CalciumConcentrationOverride && rhs.m_CalciumConcentrationOverride) ? m_CalciumConcentrationOverride->operator==(*rhs.m_CalciumConcentrationOverride) : m_CalciumConcentrationOverride == rhs.m_CalciumConcentrationOverride;
  equivilant &= (m_GlucoseConcentrationOverride && rhs.m_GlucoseConcentrationOverride) ? m_GlucoseConcentrationOverride->operator==(*rhs.m_GlucoseConcentrationOverride) : m_GlucoseConcentrationOverride == rhs.m_GlucoseConcentrationOverride;

  equivilant &= (m_LactateConcentrationOverride && rhs.m_LactateConcentrationOverride) ? m_LactateConcentrationOverride->operator==(*rhs.m_LactateConcentrationOverride) : m_LactateConcentrationOverride == rhs.m_LactateConcentrationOverride;
  equivilant &= (m_PotassiumConcentrationOverride && rhs.m_PotassiumConcentrationOverride) ? m_PotassiumConcentrationOverride->operator==(*rhs.m_PotassiumConcentrationOverride) : m_PotassiumConcentrationOverride == rhs.m_PotassiumConcentrationOverride;
  equivilant &= (m_SodiumConcentrationOverride && rhs.m_SodiumConcentrationOverride) ? m_SodiumConcentrationOverride->operator==(*rhs.m_SodiumConcentrationOverride) : m_SodiumConcentrationOverride == rhs.m_SodiumConcentrationOverride;
  equivilant &= (m_BloodVolumeOverride && rhs.m_BloodVolumeOverride) ? m_BloodVolumeOverride->operator==(*rhs.m_BloodVolumeOverride) : m_BloodVolumeOverride == rhs.m_BloodVolumeOverride;
  equivilant &= (m_CardiacOutputOverride && rhs.m_CardiacOutputOverride) ? m_CardiacOutputOverride->operator==(*rhs.m_CardiacOutputOverride) : m_CardiacOutputOverride == rhs.m_CardiacOutputOverride;

  equivilant &= (m_DiastolicArterialPressureOverride && rhs.m_DiastolicArterialPressureOverride) ? m_DiastolicArterialPressureOverride->operator==(*rhs.m_DiastolicArterialPressureOverride) : m_DiastolicArterialPressureOverride == rhs.m_DiastolicArterialPressureOverride;
  equivilant &= (m_MeanArterialPressureOverride && rhs.m_MeanArterialPressureOverride) ? m_MeanArterialPressureOverride->operator==(*rhs.m_MeanArterialPressureOverride) : m_MeanArterialPressureOverride == rhs.m_MeanArterialPressureOverride;
  equivilant &= (m_HeartRateOverride && rhs.m_HeartRateOverride) ? m_HeartRateOverride->operator==(*rhs.m_HeartRateOverride) : m_HeartRateOverride == rhs.m_HeartRateOverride;
  equivilant &= (m_HeartStrokeVolumeOverride && rhs.m_HeartStrokeVolumeOverride) ? m_HeartStrokeVolumeOverride->operator==(*rhs.m_HeartStrokeVolumeOverride) : m_HeartStrokeVolumeOverride == rhs.m_HeartStrokeVolumeOverride;
  equivilant &= (m_SystolicArterialPressureOverride && rhs.m_SystolicArterialPressureOverride) ? m_SystolicArterialPressureOverride->operator==(*rhs.m_SystolicArterialPressureOverride) : m_SystolicArterialPressureOverride == rhs.m_SystolicArterialPressureOverride;

  equivilant &= (m_InsulinSynthesisRateOverride && rhs.m_InsulinSynthesisRateOverride) ? m_InsulinSynthesisRateOverride->operator==(*rhs.m_InsulinSynthesisRateOverride) : m_InsulinSynthesisRateOverride == rhs.m_InsulinSynthesisRateOverride;
  equivilant &= (m_GlucagonSynthesisRateOverride && rhs.m_GlucagonSynthesisRateOverride) ? m_GlucagonSynthesisRateOverride->operator==(*rhs.m_GlucagonSynthesisRateOverride) : m_GlucagonSynthesisRateOverride == rhs.m_GlucagonSynthesisRateOverride;
  equivilant &= (m_AchievedExerciseLevelOverride && rhs.m_AchievedExerciseLevelOverride) ? m_AchievedExerciseLevelOverride->operator==(*rhs.m_AchievedExerciseLevelOverride) : m_AchievedExerciseLevelOverride == rhs.m_AchievedExerciseLevelOverride;
  equivilant &= (m_CoreTemperatureOverride && rhs.m_CoreTemperatureOverride) ? m_CoreTemperatureOverride->operator==(*rhs.m_CoreTemperatureOverride) : m_CoreTemperatureOverride == rhs.m_CoreTemperatureOverride;
  equivilant &= (m_CreatinineProductionRateOverride && rhs.m_CreatinineProductionRateOverride) ? m_CreatinineProductionRateOverride->operator==(*rhs.m_CreatinineProductionRateOverride) : m_CreatinineProductionRateOverride == rhs.m_CreatinineProductionRateOverride;

  equivilant &= (m_ExerciseMeanArterialPressureDeltaOverride && rhs.m_ExerciseMeanArterialPressureDeltaOverride) ? m_ExerciseMeanArterialPressureDeltaOverride->operator==(*rhs.m_ExerciseMeanArterialPressureDeltaOverride) : m_ExerciseMeanArterialPressureDeltaOverride == rhs.m_ExerciseMeanArterialPressureDeltaOverride;
  equivilant &= (m_FatigueLevelOverride && rhs.m_FatigueLevelOverride) ? m_FatigueLevelOverride->operator==(*rhs.m_FatigueLevelOverride) : m_FatigueLevelOverride == rhs.m_FatigueLevelOverride;
  equivilant &= (m_LactateProductionRateOverride && rhs.m_LactateProductionRateOverride) ? m_LactateProductionRateOverride->operator==(*rhs.m_LactateProductionRateOverride) : m_LactateProductionRateOverride == rhs.m_LactateProductionRateOverride;
  equivilant &= (m_SkinTemperatureOverride && rhs.m_SkinTemperatureOverride) ? m_SkinTemperatureOverride->operator==(*rhs.m_SkinTemperatureOverride) : m_SkinTemperatureOverride == rhs.m_SkinTemperatureOverride;
  equivilant &= (m_SweatRateOverride && rhs.m_SweatRateOverride) ? m_SweatRateOverride->operator==(*rhs.m_SweatRateOverride) : m_SweatRateOverride == rhs.m_SweatRateOverride;

  equivilant &= (m_TotalMetabolicRateOverride && rhs.m_TotalMetabolicRateOverride) ? m_TotalMetabolicRateOverride->operator==(*rhs.m_TotalMetabolicRateOverride) : m_TotalMetabolicRateOverride == rhs.m_TotalMetabolicRateOverride;
  equivilant &= (m_TotalWorkRateLevelOverride && rhs.m_TotalWorkRateLevelOverride) ? m_TotalWorkRateLevelOverride->operator==(*rhs.m_TotalWorkRateLevelOverride) : m_TotalWorkRateLevelOverride == rhs.m_TotalWorkRateLevelOverride;
  equivilant &= (m_SodiumLostToSweatOverride && rhs.m_SodiumLostToSweatOverride) ? m_SodiumLostToSweatOverride->operator==(*rhs.m_SodiumLostToSweatOverride) : m_SodiumLostToSweatOverride == rhs.m_SodiumLostToSweatOverride;
  equivilant &= (m_PotassiumLostToSweatOverride && rhs.m_PotassiumLostToSweatOverride) ? m_PotassiumLostToSweatOverride->operator==(*rhs.m_PotassiumLostToSweatOverride) : m_PotassiumLostToSweatOverride == rhs.m_PotassiumLostToSweatOverride;
  equivilant &= (m_ChlorideLostToSweatOverride && rhs.m_ChlorideLostToSweatOverride) ? m_ChlorideLostToSweatOverride->operator==(*rhs.m_ChlorideLostToSweatOverride) : m_ChlorideLostToSweatOverride == rhs.m_ChlorideLostToSweatOverride;

  equivilant &= (m_LeftAfferentArterioleResistanceOverride && rhs.m_LeftAfferentArterioleResistanceOverride) ? m_LeftAfferentArterioleResistanceOverride->operator==(*rhs.m_LeftAfferentArterioleResistanceOverride) : m_LeftAfferentArterioleResistanceOverride == rhs.m_LeftAfferentArterioleResistanceOverride;
  equivilant &= (m_LeftGlomerularFiltrationRateOverride && rhs.m_LeftGlomerularFiltrationRateOverride) ? m_LeftGlomerularFiltrationRateOverride->operator==(*rhs.m_LeftGlomerularFiltrationRateOverride) : m_LeftGlomerularFiltrationRateOverride == rhs.m_LeftGlomerularFiltrationRateOverride;
  equivilant &= (m_LeftReabsorptionRateOverride && rhs.m_LeftReabsorptionRateOverride) ? m_LeftReabsorptionRateOverride->operator==(*rhs.m_LeftReabsorptionRateOverride) : m_LeftReabsorptionRateOverride == rhs.m_LeftReabsorptionRateOverride;
  equivilant &= (m_RenalBloodFlowOverride && rhs.m_RenalBloodFlowOverride) ? m_RenalBloodFlowOverride->operator==(*rhs.m_RenalBloodFlowOverride) : m_RenalBloodFlowOverride == rhs.m_RenalBloodFlowOverride;
  equivilant &= (m_RenalPlasmaFlowOverride && rhs.m_RenalPlasmaFlowOverride) ? m_RenalPlasmaFlowOverride->operator==(*rhs.m_RenalPlasmaFlowOverride) : m_RenalPlasmaFlowOverride == rhs.m_RenalPlasmaFlowOverride;

  equivilant &= (m_RightAfferentArterioleResistanceOverride && rhs.m_RightAfferentArterioleResistanceOverride) ? m_RightAfferentArterioleResistanceOverride->operator==(*rhs.m_RightAfferentArterioleResistanceOverride) : m_RightAfferentArterioleResistanceOverride == rhs.m_RightAfferentArterioleResistanceOverride;
  equivilant &= (m_RightGlomerularFiltrationRateOverride && rhs.m_RightGlomerularFiltrationRateOverride) ? m_RightGlomerularFiltrationRateOverride->operator==(*rhs.m_RightGlomerularFiltrationRateOverride) : m_RightGlomerularFiltrationRateOverride == rhs.m_RightGlomerularFiltrationRateOverride;
  equivilant &= (m_RightReabsorptionRateOverride && rhs.m_RightReabsorptionRateOverride) ? m_RightReabsorptionRateOverride->operator==(*rhs.m_RightReabsorptionRateOverride) : m_RightReabsorptionRateOverride == rhs.m_RightReabsorptionRateOverride;
  equivilant &= (m_UrinationRateOverride && rhs.m_UrinationRateOverride) ? m_UrinationRateOverride->operator==(*rhs.m_UrinationRateOverride) : m_UrinationRateOverride == rhs.m_UrinationRateOverride;
  equivilant &= (m_UrineProductionRateOverride && rhs.m_UrineProductionRateOverride) ? m_UrineProductionRateOverride->operator==(*rhs.m_UrineProductionRateOverride) : m_UrineProductionRateOverride == rhs.m_UrineProductionRateOverride;

  equivilant &= (m_UrineOsmolalityOverride && rhs.m_UrineOsmolalityOverride) ? m_UrineOsmolalityOverride->operator==(*rhs.m_UrineOsmolalityOverride) : m_UrineOsmolalityOverride == rhs.m_UrineOsmolalityOverride;
  equivilant &= (m_UrineVolumeOverride && rhs.m_UrineVolumeOverride) ? m_UrineVolumeOverride->operator==(*rhs.m_UrineVolumeOverride) : m_UrineVolumeOverride == rhs.m_UrineVolumeOverride;
  equivilant &= (m_UrineUreaNitrogenConcentrationOverride && rhs.m_UrineUreaNitrogenConcentrationOverride) ? m_UrineUreaNitrogenConcentrationOverride->operator==(*rhs.m_UrineUreaNitrogenConcentrationOverride) : m_UrineUreaNitrogenConcentrationOverride == rhs.m_UrineUreaNitrogenConcentrationOverride;
  equivilant &= (m_ExpiratoryFlowOverride && rhs.m_ExpiratoryFlowOverride) ? m_ExpiratoryFlowOverride->operator==(*rhs.m_ExpiratoryFlowOverride) : m_ExpiratoryFlowOverride == rhs.m_ExpiratoryFlowOverride;
  equivilant &= (m_InspiratoryFlowOverride && rhs.m_InspiratoryFlowOverride) ? m_InspiratoryFlowOverride->operator==(*rhs.m_InspiratoryFlowOverride) : m_InspiratoryFlowOverride == rhs.m_InspiratoryFlowOverride;

  equivilant &= (m_PulmonaryComplianceOverride && rhs.m_PulmonaryComplianceOverride) ? m_PulmonaryComplianceOverride->operator==(*rhs.m_PulmonaryComplianceOverride) : m_PulmonaryComplianceOverride == rhs.m_PulmonaryComplianceOverride;
  equivilant &= (m_PulmonaryResistanceOverride && rhs.m_PulmonaryResistanceOverride) ? m_PulmonaryResistanceOverride->operator==(*rhs.m_PulmonaryResistanceOverride) : m_PulmonaryResistanceOverride == rhs.m_PulmonaryResistanceOverride;
  equivilant &= (m_RespirationRateOverride && rhs.m_RespirationRateOverride) ? m_RespirationRateOverride->operator==(*rhs.m_RespirationRateOverride) : m_RespirationRateOverride == rhs.m_RespirationRateOverride;
  equivilant &= (m_TidalVolumeOverride && rhs.m_TidalVolumeOverride) ? m_TidalVolumeOverride->operator==(*rhs.m_TidalVolumeOverride) : m_TidalVolumeOverride == rhs.m_TidalVolumeOverride;
  equivilant &= (m_TargetPulmonaryVentilationOverride && rhs.m_TargetPulmonaryVentilationOverride) ? m_TargetPulmonaryVentilationOverride->operator==(*rhs.m_TargetPulmonaryVentilationOverride) : m_TargetPulmonaryVentilationOverride == rhs.m_TargetPulmonaryVentilationOverride;

  equivilant &= (m_TotalAlveolarVentilationOverride && rhs.m_TotalAlveolarVentilationOverride) ? m_TotalAlveolarVentilationOverride->operator==(*rhs.m_TotalAlveolarVentilationOverride) : m_TotalAlveolarVentilationOverride == rhs.m_TotalAlveolarVentilationOverride;
  equivilant &= (m_TotalLungVolumeOverride && rhs.m_TotalLungVolumeOverride) ? m_TotalLungVolumeOverride->operator==(*rhs.m_TotalLungVolumeOverride) : m_TotalLungVolumeOverride == rhs.m_TotalLungVolumeOverride;
  equivilant &= (m_TotalPulmonaryVentilationOverride && rhs.m_TotalPulmonaryVentilationOverride) ? m_TotalPulmonaryVentilationOverride->operator==(*rhs.m_TotalPulmonaryVentilationOverride) : m_TotalPulmonaryVentilationOverride == rhs.m_TotalPulmonaryVentilationOverride;
  equivilant &= (m_ExtravascularFluidVolumeOverride && rhs.m_ExtravascularFluidVolumeOverride) ? m_ExtravascularFluidVolumeOverride->operator==(*rhs.m_ExtravascularFluidVolumeOverride) : m_ExtravascularFluidVolumeOverride == rhs.m_ExtravascularFluidVolumeOverride;
  equivilant &= (m_IntracellularFluidVolumeOverride && rhs.m_IntracellularFluidVolumeOverride) ? m_IntracellularFluidVolumeOverride->operator==(*rhs.m_IntracellularFluidVolumeOverride) : m_IntracellularFluidVolumeOverride == rhs.m_IntracellularFluidVolumeOverride;

  equivilant &= (m_LiverGlycogenOverride && rhs.m_LiverGlycogenOverride) ? m_LiverGlycogenOverride->operator==(*rhs.m_LiverGlycogenOverride) : m_LiverGlycogenOverride == rhs.m_LiverGlycogenOverride;
  equivilant &= (m_MuscleGlycogenOverride && rhs.m_MuscleGlycogenOverride) ? m_MuscleGlycogenOverride->operator==(*rhs.m_MuscleGlycogenOverride) : m_MuscleGlycogenOverride == rhs.m_MuscleGlycogenOverride;
  equivilant &= (m_StoredProteinOverride && rhs.m_StoredProteinOverride) ? m_StoredProteinOverride->operator==(*rhs.m_StoredProteinOverride) : m_StoredProteinOverride == rhs.m_StoredProteinOverride;
  equivilant &= (m_StoredFatOverride && rhs.m_StoredFatOverride) ? m_StoredFatOverride->operator==(*rhs.m_StoredFatOverride) : m_StoredFatOverride == rhs.m_StoredFatOverride;

  return equivilant;
}
//-------------------------------------------------------------------------------
bool SEOverride::operator!=(const SEOverride& rhs) const
{
  return !(*this == rhs);
}
}
