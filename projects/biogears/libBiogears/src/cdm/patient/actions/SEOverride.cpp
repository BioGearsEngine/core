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

namespace biogears {
SEOverride::SEOverride()
  : SEPatientAction()
{
  m_OverrideState = CDM::enumOnOff::Off;
  m_OverrideConformance = CDM::enumOnOff::On;
  m_ArterialPHOR = nullptr;
  m_VenousPHOR = nullptr;
  m_CO2SaturationOR = nullptr;
  m_COSaturationOR = nullptr;
  m_O2SaturationOR = nullptr;
  m_PhosphateOR = nullptr;
  m_WBCCountOR = nullptr;
  m_TotalBilirubinOR = nullptr;
  m_CalciumConcentrationOR = nullptr;
  m_GlucoseConcentrationOR = nullptr;
  m_LactateConcentrationOR = nullptr;
  m_PotassiumConcentrationOR = nullptr;
  m_SodiumConcentrationOR = nullptr;
  m_BloodVolumeOR = nullptr;
  m_CardiacOutputOR = nullptr;
  m_DiastolicArtPressureOR = nullptr;
  m_MeanArtPressureOR = nullptr;
  m_HeartRateOR = nullptr;
  m_HeartStrokeVolumeOR = nullptr;
  m_SystolicArtPressureOR = nullptr;
  m_InsulinSynthesisRateOR = nullptr;
  m_GlucagonSynthesisRateOR = nullptr;
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
  m_UrineUreaNitrogenConcentrationOverrideOR = nullptr;
  m_ExpiratoryFlowOR = nullptr;
  m_InspiratoryFlowOR = nullptr;
  m_PulmonaryComplianceOR = nullptr;
  m_PulmonaryResistanceOR = nullptr;
  m_RespirationRateOR = nullptr;
  m_TidalVolumeOR = nullptr;
  m_TargetPulmonaryVentilationOR = nullptr;
  m_TotalAlveolarVentilationOR = nullptr;
  m_TotalLungVolumeOR = nullptr;
  m_TotalPulmonaryVentilationOR = nullptr;
  m_ExtravascularFluidVolumeOR = nullptr;
  m_IntracellularFluidVolumeOR = nullptr;
  m_LiverGlycogenOR = nullptr;
  m_MuscleGlycogenOR = nullptr;
  m_StoredProteinOR = nullptr;
  m_StoredFatOR = nullptr;
}

SEOverride::~SEOverride()
{
  Clear();
}

void SEOverride::Clear()
{
  SEPatientAction::Clear();
  m_OverrideState = CDM::enumOnOff::Off;
  m_OverrideConformance = CDM::enumOnOff::On;
  SAFE_DELETE(m_ArterialPHOR);
  SAFE_DELETE(m_VenousPHOR);
  SAFE_DELETE(m_CO2SaturationOR);
  SAFE_DELETE(m_COSaturationOR);
  SAFE_DELETE(m_O2SaturationOR);
  SAFE_DELETE(m_PhosphateOR);
  SAFE_DELETE(m_WBCCountOR);
  SAFE_DELETE(m_TotalBilirubinOR);
  SAFE_DELETE(m_CalciumConcentrationOR);
  SAFE_DELETE(m_GlucoseConcentrationOR);
  SAFE_DELETE(m_LactateConcentrationOR);
  SAFE_DELETE(m_PotassiumConcentrationOR);
  SAFE_DELETE(m_SodiumConcentrationOR);
  SAFE_DELETE(m_BloodVolumeOR);
  SAFE_DELETE(m_CardiacOutputOR);
  SAFE_DELETE(m_DiastolicArtPressureOR);
  SAFE_DELETE(m_MeanArtPressureOR);
  SAFE_DELETE(m_HeartRateOR);
  SAFE_DELETE(m_HeartStrokeVolumeOR);
  SAFE_DELETE(m_SystolicArtPressureOR);
  SAFE_DELETE(m_InsulinSynthesisRateOR);
  SAFE_DELETE(m_GlucagonSynthesisRateOR);
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
  SAFE_DELETE(m_UrineUreaNitrogenConcentrationOverrideOR);
  SAFE_DELETE(m_ExpiratoryFlowOR);
  SAFE_DELETE(m_InspiratoryFlowOR);
  SAFE_DELETE(m_PulmonaryComplianceOR);
  SAFE_DELETE(m_PulmonaryResistanceOR);
  SAFE_DELETE(m_RespirationRateOR);
  SAFE_DELETE(m_TidalVolumeOR);
  SAFE_DELETE(m_TargetPulmonaryVentilationOR);
  SAFE_DELETE(m_TotalAlveolarVentilationOR);
  SAFE_DELETE(m_TotalLungVolumeOR);
  SAFE_DELETE(m_TotalPulmonaryVentilationOR);
  SAFE_DELETE(m_ExtravascularFluidVolumeOR);
  SAFE_DELETE(m_IntracellularFluidVolumeOR);
  SAFE_DELETE(m_LiverGlycogenOR);
  SAFE_DELETE(m_MuscleGlycogenOR);
  SAFE_DELETE(m_StoredProteinOR);
  SAFE_DELETE(m_StoredFatOR);
}

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

bool SEOverride::IsActive() const
{
  return (GetOverrideState() == CDM::enumOnOff::On);
}

bool SEOverride::Load(const CDM::OverrideData& in)
{
  SEPatientAction::Clear();
  SetOverrideState(in.State());
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
  if (in.CarbonDioxideSaturationOverride().present()) {
    GetCO2SaturationOverride().Load(in.CarbonDioxideSaturationOverride().get());
  } else {
    GetCO2SaturationOverride().Invalidate();
  }
  if (in.CarbonMonoxideSaturationOverride().present()) {
    GetCOSaturationOverride().Load(in.CarbonMonoxideSaturationOverride().get());
  } else {
    GetCOSaturationOverride().Invalidate();
  }
  if (in.OxygenSaturationOverride().present()) {
    GetO2SaturationOverride().Load(in.OxygenSaturationOverride().get());
  } else {
    GetO2SaturationOverride().Invalidate();
  }
  if (in.PhosphateOverride().present()) {
    GetPhosphateOverride().Load(in.PhosphateOverride().get());
  } else {
    GetPhosphateOverride().Invalidate();
  }
  if (in.WhiteBloodCellCountOverride().present()) {
    GetWBCCountOverride().Load(in.WhiteBloodCellCountOverride().get());
  } else {
    GetWBCCountOverride().Invalidate();
  }
  if (in.TotalBilirubinOverride().present()) {
    GetTotalBilirubinOverride().Load(in.TotalBilirubinOverride().get());
  } else {
    GetTotalBilirubinOverride().Invalidate();
  }
  if (in.CalciumConcentrationOverride().present()) {
    GetCalciumConcentrationOverride().Load(in.CalciumConcentrationOverride().get());
  } else {
    GetCalciumConcentrationOverride().Invalidate();
  }
  if (in.GlucoseConcentrationOverride().present()) {
    GetGlucoseConcentrationOverride().Load(in.GlucoseConcentrationOverride().get());
  } else {
    GetGlucoseConcentrationOverride().Invalidate();
  }
  if (in.LactateConcentrationOverride().present()) {
    GetLactateConcentrationOverride().Load(in.LactateConcentrationOverride().get());
  } else {
    GetLactateConcentrationOverride().Invalidate();
  }
  if (in.PotassiumConcentrationOverride().present()) {
    GetPotassiumConcentrationOverride().Load(in.PotassiumConcentrationOverride().get());
  } else {
    GetPotassiumConcentrationOverride().Invalidate();
  }
  if (in.SodiumConcentrationOverride().present()) {
    GetSodiumConcentrationOverride().Load(in.SodiumConcentrationOverride().get());
  } else {
    GetSodiumConcentrationOverride().Invalidate();
  }
  if (in.BloodVolumeOverride().present()) {
    GetBloodVolumeOverride().Load(in.BloodVolumeOverride().get());
  } else {
    GetBloodVolumeOverride().Invalidate();
  }
  if (in.CardiacOutputOverride().present()) {
    GetCardiacOutputOverride().Load(in.CardiacOutputOverride().get());
  } else {
    GetCardiacOutputOverride().Invalidate();
  }
  if (in.DiastolicArterialPressureOverride().present()) {
    GetDiastolicArterialPressureOverride().Load(in.DiastolicArterialPressureOverride().get());
  } else {
    GetDiastolicArterialPressureOverride().Invalidate();
  }
  if (in.MeanArterialPressureOverride().present()) {
    if (!in.DiastolicArterialPressureOverride().present() || !in.SystolicArterialPressureOverride().present()) {
      GetMAPOverride().Load(in.MeanArterialPressureOverride().get());
    } else {
      Warning("If overriding systolic or diastolic pressures, the mean arterial pressure will change according to those changes.");
      GetMAPOverride().Invalidate();
    }
  } else {
    GetMAPOverride().Invalidate();
  }
  if (in.HeartRateOverride().present()) {
    GetHeartRateOverride().Load(in.HeartRateOverride().get());
  } else {
    GetHeartRateOverride().Invalidate();
  }
  if (in.HeartStrokeVolumeOverride().present()) {
    GetHeartStrokeVolumeOverride().Load(in.HeartStrokeVolumeOverride().get());
  } else {
    GetHeartStrokeVolumeOverride().Invalidate();
  }
  if (in.SystolicArterialPressureOverride().present()) {
    GetSystolicArterialPressureOverride().Load(in.SystolicArterialPressureOverride().get());
  } else {
    GetSystolicArterialPressureOverride().Invalidate();
  }
  if (in.InsulinSynthesisRateOverride().present()) {
    GetInsulinSynthesisRateOverride().Load(in.InsulinSynthesisRateOverride().get());
  } else {
    GetInsulinSynthesisRateOverride().Invalidate();
  }
  if (in.GlucagonSynthesisRateOverride().present()) {
    GetGlucagonSynthesisRateOverride().Load(in.GlucagonSynthesisRateOverride().get());
  } else {
    GetGlucagonSynthesisRateOverride().Invalidate();
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
  if (in.UrineUreaNitrogenConcentrationOverride().present()) {
    GetUrineUreaNitrogenConcentrationOverride().Load(in.UrineUreaNitrogenConcentrationOverride().get());
  } else {
    GetUrineUreaNitrogenConcentrationOverride().Invalidate();
  }
  if (in.ExpiratoryFlowOverride().present()) {
    GetExpiratoryFlowOverride().Load(in.ExpiratoryFlowOverride().get());
  } else {
    GetExpiratoryFlowOverride().Invalidate();
  }
  if (in.InspiratoryFlowOverride().present()) {
    GetInspiratoryFlowOverride().Load(in.InspiratoryFlowOverride().get());
  } else {
    GetInspiratoryFlowOverride().Invalidate();
  }
  if (in.PulmonaryComplianceOverride().present()) {
    GetPulmonaryComplianceOverride().Load(in.PulmonaryComplianceOverride().get());
  } else {
    GetPulmonaryComplianceOverride().Invalidate();
  }
  if (in.PulmonaryResistanceOverride().present()) {
    GetPulmonaryResistanceOverride().Load(in.PulmonaryResistanceOverride().get());
  } else {
    GetPulmonaryResistanceOverride().Invalidate();
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
  if (in.TargetPulmonaryVentilationOverride().present()) {
    GetTargetPulmonaryVentilationOverride().Load(in.TargetPulmonaryVentilationOverride().get());
  } else {
    GetTargetPulmonaryVentilationOverride().Invalidate();
  }
  if (in.TotalAlveolarVentilationOverride().present()) {
    GetTotalAlveolarVentilationOverride().Load(in.TotalAlveolarVentilationOverride().get());
  } else {
    GetTotalAlveolarVentilationOverride().Invalidate();
  }
  if (in.TotalLungVolumeOverride().present()) {
    GetTotalLungVolumeOverride().Load(in.TotalLungVolumeOverride().get());
  } else {
    GetTotalLungVolumeOverride().Invalidate();
  }
  if (in.TotalPulmonaryVentilationOverride().present()) {
    GetTotalPulmonaryVentilationOverride().Load(in.TotalPulmonaryVentilationOverride().get());
  } else {
    GetTotalPulmonaryVentilationOverride().Invalidate();
  }
  if (in.ExtravascularFluidVolumeOverride().present()) {
    GetExtravascularFluidVolumeOverride().Load(in.ExtravascularFluidVolumeOverride().get());
  } else {
    GetExtravascularFluidVolumeOverride().Invalidate();
  }
  if (in.IntracellularFluidVolumeOverride().present()) {
    GetIntracellularFluidVolumeOverride().Load(in.IntracellularFluidVolumeOverride().get());
  } else {
    GetIntracellularFluidVolumeOverride().Invalidate();
  }
  if (in.LiverGlycogenOverride().present()) {
    GetLiverGlycogenOverride().Load(in.LiverGlycogenOverride().get());
  } else {
    GetLiverGlycogenOverride().Invalidate();
  }
  if (in.MuscleGlycogenOverride().present()) {
    GetMuscleGlycogenOverride().Load(in.MuscleGlycogenOverride().get());
  } else {
    GetMuscleGlycogenOverride().Invalidate();
  }
  if (in.StoredProteinOverride().present()) {
    GetStoredProteinOverride().Load(in.StoredProteinOverride().get());
  } else {
    GetStoredProteinOverride().Invalidate();
  }
  if (in.StoredFatOverride().present()) {
    GetStoredFatOverride().Load(in.StoredFatOverride().get());
  } else {
    GetStoredFatOverride().Invalidate();
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
  if (HasOverrideState()) {
    data.State(m_OverrideState);
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
  if (HasCO2SaturationOverride()) {
    data.CarbonDioxideSaturationOverride(std::unique_ptr<CDM::ScalarFractionData>(m_CO2SaturationOR->Unload()));
  }
  if (HasCOSaturationOverride()) {
    data.CarbonMonoxideSaturationOverride(std::unique_ptr<CDM::ScalarFractionData>(m_COSaturationOR->Unload()));
  }
  if (HasO2SaturationOverride()) {
    data.OxygenSaturationOverride(std::unique_ptr<CDM::ScalarFractionData>(m_O2SaturationOR->Unload()));
  }
  if (HasPhosphateOverride()) {
    data.PhosphateOverride(std::unique_ptr<CDM::ScalarAmountPerVolumeData>(m_PhosphateOR->Unload()));
  }
  if (HasWBCCountOverride()) {
    data.WhiteBloodCellCountOverride(std::unique_ptr<CDM::ScalarAmountPerVolumeData>(m_WBCCountOR->Unload()));
  }
  if (HasTotalBilirubinOverride()) {
    data.TotalBilirubinOverride(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_TotalBilirubinOR->Unload()));
  }
  if (HasCalciumConcentrationOverride()) {
    data.CalciumConcentrationOverride(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_CalciumConcentrationOR->Unload()));
  }
  if (HasGlucoseConcentrationOverride()) {
    data.GlucoseConcentrationOverride(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_GlucoseConcentrationOR->Unload()));
  }
  if (HasLactateConcentrationOverride()) {
    data.LactateConcentrationOverride(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_LactateConcentrationOR->Unload()));
  }
  if (HasPotassiumConcentrationOverride()) {
    data.PotassiumConcentrationOverride(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_PotassiumConcentrationOR->Unload()));
  }
  if (HasSodiumConcentrationOverride()) {
    data.SodiumConcentrationOverride(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_SodiumConcentrationOR->Unload()));
  }
  if (HasBloodVolumeOverride()) {
    data.BloodVolumeOverride(std::unique_ptr<CDM::ScalarVolumeData>(m_BloodVolumeOR->Unload()));
  }
  if (HasCardiacOutputOverride()) {
    data.CardiacOutputOverride(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_CardiacOutputOR->Unload()));
  }
  if (HasDiastolicArterialPressureOverride()) {
    data.DiastolicArterialPressureOverride(std::unique_ptr<CDM::ScalarPressureData>(m_DiastolicArtPressureOR->Unload()));
  }
  if (HasMAPOverride()) {
    data.MeanArterialPressureOverride(std::unique_ptr<CDM::ScalarPressureData>(m_MeanArtPressureOR->Unload()));
  }
  if (HasHeartRateOverride()) {
    data.HeartRateOverride(std::unique_ptr<CDM::ScalarFrequencyData>(m_HeartRateOR->Unload()));
  }
  if (HasHeartStrokeVolumeOverride()) {
    data.HeartStrokeVolumeOverride(std::unique_ptr<CDM::ScalarVolumeData>(m_HeartStrokeVolumeOR->Unload()));
  }
  if (HasSystolicArterialPressureOverride()) {
    data.SystolicArterialPressureOverride(std::unique_ptr<CDM::ScalarPressureData>(m_SystolicArtPressureOR->Unload()));
  }
  if (HasInsulinSynthesisRateOverride())
  {
    data.InsulinSynthesisRateOverride(std::unique_ptr<CDM::ScalarAmountPerTimeData>(m_InsulinSynthesisRateOR->Unload()));
  }
  if (HasGlucagonSynthesisRateOverride()) {
    data.GlucagonSynthesisRateOverride(std::unique_ptr<CDM::ScalarAmountPerTimeData>(m_GlucagonSynthesisRateOR->Unload()));
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
  if (HasUrineUreaNitrogenConcentrationOverride()) {
    data.UrineUreaNitrogenConcentrationOverride(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_UrineUreaNitrogenConcentrationOverrideOR->Unload()));
  }
  if (HasExpiratoryFlowOverride()) {
    data.ExpiratoryFlowOverride(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_ExpiratoryFlowOR->Unload()));
  }
  if (HasInspiratoryFlowOverride()) {
    data.InspiratoryFlowOverride(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_InspiratoryFlowOR->Unload()));
  }
  if (HasPulmonaryComplianceOverride()) {
    data.PulmonaryComplianceOverride(std::unique_ptr<CDM::ScalarFlowComplianceData>(m_PulmonaryComplianceOR->Unload()));
  }
  if (HasPulmonaryResistanceOverride()) {
    data.PulmonaryResistanceOverride(std::unique_ptr<CDM::ScalarFlowResistanceData>(m_PulmonaryResistanceOR->Unload()));
  }
  if (HasRespirationRateOverride()) {
    data.RespirationRateOverride(std::unique_ptr<CDM::ScalarFrequencyData>(m_RespirationRateOR->Unload()));
  }
  if (HasTidalVolumeOverride()) {
    data.TidalVolumeOverride(std::unique_ptr<CDM::ScalarVolumeData>(m_TidalVolumeOR->Unload()));
  }
  if (HasTargetPulmonaryVentilationOverride()) {
    data.TargetPulmonaryVentilationOverride(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_TargetPulmonaryVentilationOR->Unload()));
  }
  if (HasTotalAlveolarVentilationOverride()) {
    data.TotalAlveolarVentilationOverride(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_TotalAlveolarVentilationOR->Unload()));
  }
  if (HasTotalLungVolumeOverride()) {
    data.TotalLungVolumeOverride(std::unique_ptr<CDM::ScalarVolumeData>(m_TotalLungVolumeOR->Unload()));
  }
  if (HasTotalPulmonaryVentilationOverride()) {
    data.TotalPulmonaryVentilationOverride(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_TotalPulmonaryVentilationOR->Unload()));
  }
  if (HasExtravascularFluidVolumeOverride()) {
    data.ExtravascularFluidVolumeOverride(std::unique_ptr<CDM::ScalarVolumeData>(m_ExtravascularFluidVolumeOR->Unload()));
  }
  if (HasIntracellularFluidVolumeOverride()) {
    data.IntracellularFluidVolumeOverride(std::unique_ptr<CDM::ScalarVolumeData>(m_IntracellularFluidVolumeOR->Unload()));
  }
  if (HasLiverGlycogenOverride()) {
    data.LiverGlycogenOverride(std::unique_ptr<CDM::ScalarMassData>(m_LiverGlycogenOR->Unload()));
  }
  if (HasMuscleGlycogenOverride()) {
    data.MuscleGlycogenOverride(std::unique_ptr<CDM::ScalarMassData>(m_MuscleGlycogenOR->Unload()));
  }
  if (HasStoredProteinOverride()) {
    data.StoredProteinOverride(std::unique_ptr<CDM::ScalarMassData>(m_StoredProteinOR->Unload()));
  }
  if (HasStoredFatOverride()) {
    data.StoredFatOverride(std::unique_ptr<CDM::ScalarMassData>(m_StoredFatOR->Unload()));
  }
}

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
  return (m_OverrideState == CDM::enumOnOff::Off) ? true : (m_OverrideState == CDM::enumOnOff::On) ? true : false;
}
void SEOverride::InvalidateOverrideState()
{
  m_OverrideState = (CDM::enumOnOff::Off);
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
  return (m_OverrideConformance == CDM::enumOnOff::Off) ? true : (m_OverrideConformance == CDM::enumOnOff::On) ? true : false;
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

bool SEOverride::HasCO2SaturationOverride() const
{
  return m_CO2SaturationOR == nullptr ? false : m_CO2SaturationOR->IsValid();
}
SEScalarFraction& SEOverride::GetCO2SaturationOverride()
{
  if (m_CO2SaturationOR == nullptr) {
    m_CO2SaturationOR = new SEScalarFraction();
  }
  return *m_CO2SaturationOR;
}
double SEOverride::GetCO2SaturationOverride() const
{
  if (m_CO2SaturationOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_CO2SaturationOR->GetValue();
}
bool SEOverride::HasCOSaturationOverride() const
{
  return m_COSaturationOR == nullptr ? false : m_COSaturationOR->IsValid();
}
SEScalarFraction& SEOverride::GetCOSaturationOverride()
{
  if (m_COSaturationOR == nullptr) {
    m_COSaturationOR = new SEScalarFraction();
  }
  return *m_COSaturationOR;
}
double SEOverride::GetCOSaturationOverride() const
{
  if (m_COSaturationOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_COSaturationOR->GetValue();
}
bool SEOverride::HasO2SaturationOverride() const
{
  return m_O2SaturationOR == nullptr ? false : m_O2SaturationOR->IsValid();
}
SEScalarFraction& SEOverride::GetO2SaturationOverride()
{
  if (m_O2SaturationOR == nullptr) {
    m_O2SaturationOR = new SEScalarFraction();
  }
  return *m_O2SaturationOR;
}
double SEOverride::GetO2SaturationOverride() const
{
  if (m_O2SaturationOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_O2SaturationOR->GetValue();
}
bool SEOverride::HasPhosphateOverride() const
{
  return m_PhosphateOR == nullptr ? false : m_PhosphateOR->IsValid();
}
SEScalarAmountPerVolume& SEOverride::GetPhosphateOverride()
{
  if (m_PhosphateOR == nullptr) {
    m_PhosphateOR = new SEScalarAmountPerVolume();
  }
  return *m_PhosphateOR;
}
double SEOverride::GetPhosphateOverride(const AmountPerVolumeUnit& unit) const
{
  if (m_PhosphateOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_PhosphateOR->GetValue(unit);
}
bool SEOverride::HasWBCCountOverride() const
{
  return m_WBCCountOR == nullptr ? false : m_WBCCountOR->IsValid();
}
SEScalarAmountPerVolume& SEOverride::GetWBCCountOverride()
{
  if (m_WBCCountOR == nullptr) {
    m_WBCCountOR = new SEScalarAmountPerVolume();
  }
  return *m_WBCCountOR;
}
double SEOverride::GetWBCCountOverride(const AmountPerVolumeUnit& unit) const
{
  if (m_WBCCountOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_WBCCountOR->GetValue(unit);
}
bool SEOverride::HasTotalBilirubinOverride() const
{
  return m_TotalBilirubinOR == nullptr ? false : m_TotalBilirubinOR->IsValid();
}
SEScalarMassPerVolume& SEOverride::GetTotalBilirubinOverride()
{
  if (m_TotalBilirubinOR == nullptr) {
    m_TotalBilirubinOR = new SEScalarMassPerVolume();
  }
  return *m_TotalBilirubinOR;
}
double SEOverride::GetTotalBilirubinOverride(const MassPerVolumeUnit& unit) const
{
  if (m_TotalBilirubinOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TotalBilirubinOR->GetValue(unit);
}
bool SEOverride::HasCalciumConcentrationOverride() const
{
  return m_CalciumConcentrationOR == nullptr ? false : m_CalciumConcentrationOR->IsValid();
}
SEScalarMassPerVolume& SEOverride::GetCalciumConcentrationOverride()
{
  if (m_CalciumConcentrationOR == nullptr) {
    m_CalciumConcentrationOR = new SEScalarMassPerVolume();
  }
  return *m_CalciumConcentrationOR;
}
double SEOverride::GetCalciumConcentrationOverride(const MassPerVolumeUnit& unit) const
{
  if (m_CalciumConcentrationOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_CalciumConcentrationOR->GetValue(unit);
}
bool SEOverride::HasGlucoseConcentrationOverride() const
{
  return m_GlucoseConcentrationOR == nullptr ? false : m_GlucoseConcentrationOR->IsValid();
}
SEScalarMassPerVolume& SEOverride::GetGlucoseConcentrationOverride()
{
  if (m_GlucoseConcentrationOR == nullptr) {
    m_GlucoseConcentrationOR = new SEScalarMassPerVolume();
  }
  return *m_GlucoseConcentrationOR;
}
double SEOverride::GetGlucoseConcentrationOverride(const MassPerVolumeUnit& unit) const
{
  if (m_GlucoseConcentrationOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_GlucoseConcentrationOR->GetValue(unit);
}
bool SEOverride::HasLactateConcentrationOverride() const
{
  return m_LactateConcentrationOR == nullptr ? false : m_LactateConcentrationOR->IsValid();
}
SEScalarMassPerVolume& SEOverride::GetLactateConcentrationOverride()
{
  if (m_LactateConcentrationOR == nullptr) {
    m_LactateConcentrationOR = new SEScalarMassPerVolume();
  }
  return *m_LactateConcentrationOR;
}
double SEOverride::GetLactateConcentrationOverride(const MassPerVolumeUnit& unit) const
{
  if (m_LactateConcentrationOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_LactateConcentrationOR->GetValue(unit);
}
bool SEOverride::HasPotassiumConcentrationOverride() const
{
  return m_PotassiumConcentrationOR == nullptr ? false : m_PotassiumConcentrationOR->IsValid();
}
SEScalarMassPerVolume& SEOverride::GetPotassiumConcentrationOverride()
{
  if (m_PotassiumConcentrationOR == nullptr) {
    m_PotassiumConcentrationOR = new SEScalarMassPerVolume();
  }
  return *m_PotassiumConcentrationOR;
}
double SEOverride::GetPotassiumConcentrationOverride(const MassPerVolumeUnit& unit) const
{
  if (m_PotassiumConcentrationOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_PotassiumConcentrationOR->GetValue(unit);
}
bool SEOverride::HasSodiumConcentrationOverride() const
{
  return m_SodiumConcentrationOR == nullptr ? false : m_SodiumConcentrationOR->IsValid();
}
SEScalarMassPerVolume& SEOverride::GetSodiumConcentrationOverride()
{
  if (m_SodiumConcentrationOR == nullptr) {
    m_SodiumConcentrationOR = new SEScalarMassPerVolume();
  }
  return *m_SodiumConcentrationOR;
}
double SEOverride::GetSodiumConcentrationOverride(const MassPerVolumeUnit& unit) const
{
  if (m_SodiumConcentrationOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_SodiumConcentrationOR->GetValue(unit);
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
  return m_BloodVolumeOR == nullptr ? false : m_BloodVolumeOR->IsValid();
}
SEScalarVolume& SEOverride::GetBloodVolumeOverride()
{
  if (m_BloodVolumeOR == nullptr) {
    m_BloodVolumeOR = new SEScalarVolume();
  }
  return *m_BloodVolumeOR;
}
double SEOverride::GetBloodVolumeOverride(const VolumeUnit& unit) const
{
  if (m_BloodVolumeOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_BloodVolumeOR->GetValue(unit);
}
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
bool SEOverride::HasDiastolicArterialPressureOverride() const
{
  return m_DiastolicArtPressureOR == nullptr ? false : m_DiastolicArtPressureOR->IsValid();
}
SEScalarPressure& SEOverride::GetDiastolicArterialPressureOverride()
{
  if (m_DiastolicArtPressureOR == nullptr) {
    m_DiastolicArtPressureOR = new SEScalarPressure();
  }
  return *m_DiastolicArtPressureOR;
}
double SEOverride::GetDiastolicArterialPressureOverride(const PressureUnit& unit) const
{
  if (m_DiastolicArtPressureOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_DiastolicArtPressureOR->GetValue(unit);
}
bool SEOverride::HasMAPOverride() const
{
  return m_MeanArtPressureOR == nullptr ? false : m_MeanArtPressureOR->IsValid();
}
SEScalarPressure& SEOverride::GetMAPOverride()
{
  if (m_MeanArtPressureOR == nullptr) {
    m_MeanArtPressureOR = new SEScalarPressure();
  }
  return *m_MeanArtPressureOR;
}
double SEOverride::GetMAPOverride(const PressureUnit& unit) const
{
  if (m_MeanArtPressureOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_MeanArtPressureOR->GetValue(unit);
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
bool SEOverride::HasHeartStrokeVolumeOverride() const
{
  return m_HeartStrokeVolumeOR == nullptr ? false : m_HeartStrokeVolumeOR->IsValid();
}
SEScalarVolume& SEOverride::GetHeartStrokeVolumeOverride()
{
  if (m_HeartStrokeVolumeOR == nullptr) {
    m_HeartStrokeVolumeOR = new SEScalarVolume();
  }
  return *m_HeartStrokeVolumeOR;
}
double SEOverride::GetHeartStrokeVolumeOverride(const VolumeUnit& unit) const
{
  if (m_HeartStrokeVolumeOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_HeartStrokeVolumeOR->GetValue(unit);
}
bool SEOverride::HasSystolicArterialPressureOverride() const
{
  return m_SystolicArtPressureOR == nullptr ? false : m_SystolicArtPressureOR->IsValid();
}
SEScalarPressure& SEOverride::GetSystolicArterialPressureOverride()
{
  if (m_SystolicArtPressureOR == nullptr) {
    m_SystolicArtPressureOR = new SEScalarPressure();
  }
  return *m_SystolicArtPressureOR;
}
double SEOverride::GetSystolicArterialPressureOverride(const PressureUnit& unit) const
{
  if (m_SystolicArtPressureOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_SystolicArtPressureOR->GetValue(unit);
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
  return m_InsulinSynthesisRateOR == nullptr ? false : m_InsulinSynthesisRateOR->IsValid();
}
SEScalarAmountPerTime& SEOverride::GetInsulinSynthesisRateOverride()
{
  if (m_InsulinSynthesisRateOR == nullptr) {
    m_InsulinSynthesisRateOR = new SEScalarAmountPerTime();
  }
  return *m_InsulinSynthesisRateOR;
}
double SEOverride::GetInsulinSynthesisRateOverride(const AmountPerTimeUnit& unit) const
{
  if (m_InsulinSynthesisRateOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_InsulinSynthesisRateOR->GetValue(unit);
}
bool SEOverride::HasGlucagonSynthesisRateOverride() const
{
  return m_GlucagonSynthesisRateOR == nullptr ? false : m_GlucagonSynthesisRateOR->IsValid();
}
SEScalarAmountPerTime& SEOverride::GetGlucagonSynthesisRateOverride()
{
  if (m_GlucagonSynthesisRateOR == nullptr) {
    m_GlucagonSynthesisRateOR = new SEScalarAmountPerTime();
  }
  return *m_GlucagonSynthesisRateOR;
}
double SEOverride::GetGlucagonSynthesisRateOverride(const AmountPerTimeUnit& unit) const
{
  if (m_GlucagonSynthesisRateOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_GlucagonSynthesisRateOR->GetValue(unit);
}

bool SEOverride::HasEndocrineOverride() const
{
  return HasInsulinSynthesisRateOverride()
    || HasGlucagonSynthesisRateOverride();
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
bool SEOverride::HasUrineUreaNitrogenConcentrationOverride() const
{
  return m_UrineUreaNitrogenConcentrationOverrideOR == nullptr ? false : m_UrineUreaNitrogenConcentrationOverrideOR->IsValid();
}
SEScalarMassPerVolume& SEOverride::GetUrineUreaNitrogenConcentrationOverride()
{
  if (m_UrineUreaNitrogenConcentrationOverrideOR == nullptr) {
    m_UrineUreaNitrogenConcentrationOverrideOR = new SEScalarMassPerVolume();
  }
  return *m_UrineUreaNitrogenConcentrationOverrideOR;
}
double SEOverride::GetUrineUreaNitrogenConcentrationOverride(const MassPerVolumeUnit& unit) const
{
  if (m_UrineUreaNitrogenConcentrationOverrideOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_UrineUreaNitrogenConcentrationOverrideOR->GetValue(unit);
}

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
  return m_ExpiratoryFlowOR == nullptr ? false : m_ExpiratoryFlowOR->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetExpiratoryFlowOverride()
{
  if (m_ExpiratoryFlowOR == nullptr) {
    m_ExpiratoryFlowOR = new SEScalarVolumePerTime();
  }
  return *m_ExpiratoryFlowOR;
}
double SEOverride::GetExpiratoryFlowOverride(const VolumePerTimeUnit& unit) const
{
  if (m_ExpiratoryFlowOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_ExpiratoryFlowOR->GetValue(unit);
}
bool SEOverride::HasInspiratoryFlowOverride() const
{
  return m_InspiratoryFlowOR == nullptr ? false : m_InspiratoryFlowOR->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetInspiratoryFlowOverride()
{
  if (m_InspiratoryFlowOR == nullptr) {
    m_InspiratoryFlowOR = new SEScalarVolumePerTime();
  }
  return *m_InspiratoryFlowOR;
}
double SEOverride::GetInspiratoryFlowOverride(const VolumePerTimeUnit& unit) const
{
  if (m_InspiratoryFlowOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_InspiratoryFlowOR->GetValue(unit);
}
bool SEOverride::HasPulmonaryComplianceOverride() const
{
  return m_PulmonaryComplianceOR == nullptr ? false : m_PulmonaryComplianceOR->IsValid();
}
SEScalarFlowCompliance& SEOverride::GetPulmonaryComplianceOverride()
{
  if (m_PulmonaryComplianceOR == nullptr) {
    m_PulmonaryComplianceOR = new SEScalarFlowCompliance();
  }
  return *m_PulmonaryComplianceOR;
}
double SEOverride::GetPulmonaryComplianceOverride(const FlowComplianceUnit& unit) const
{
  if (m_PulmonaryComplianceOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_PulmonaryComplianceOR->GetValue(unit);
}
bool SEOverride::HasPulmonaryResistanceOverride() const
{
  return m_PulmonaryResistanceOR == nullptr ? false : m_PulmonaryResistanceOR->IsValid();
}
SEScalarFlowResistance& SEOverride::GetPulmonaryResistanceOverride()
{
  if (m_PulmonaryResistanceOR == nullptr) {
    m_PulmonaryResistanceOR = new SEScalarFlowResistance();
  }
  return *m_PulmonaryResistanceOR;
}
double SEOverride::GetPulmonaryResistanceOverride(const FlowResistanceUnit& unit) const
{
  if (m_PulmonaryResistanceOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_PulmonaryResistanceOR->GetValue(unit);
}
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
bool SEOverride::HasTargetPulmonaryVentilationOverride() const
{
  return m_TargetPulmonaryVentilationOR == nullptr ? false : m_TargetPulmonaryVentilationOR->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetTargetPulmonaryVentilationOverride()
{
  if (m_TargetPulmonaryVentilationOR == nullptr) {
    m_TargetPulmonaryVentilationOR = new SEScalarVolumePerTime();
  }
  return *m_TargetPulmonaryVentilationOR;
}
double SEOverride::GetTargetPulmonaryVentilationOverride(const VolumePerTimeUnit& unit) const
{
  if (m_TargetPulmonaryVentilationOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TargetPulmonaryVentilationOR->GetValue(unit);
}
bool SEOverride::HasTotalAlveolarVentilationOverride() const
{
  return m_TotalAlveolarVentilationOR == nullptr ? false : m_TotalAlveolarVentilationOR->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetTotalAlveolarVentilationOverride()
{
  if (m_TotalAlveolarVentilationOR == nullptr) {
    m_TotalAlveolarVentilationOR = new SEScalarVolumePerTime();
  }
  return *m_TotalAlveolarVentilationOR;
}
double SEOverride::GetTotalAlveolarVentilationOverride(const VolumePerTimeUnit& unit) const
{
  if (m_TotalAlveolarVentilationOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TotalAlveolarVentilationOR->GetValue(unit);
}
bool SEOverride::HasTotalLungVolumeOverride() const
{
  return m_TotalLungVolumeOR == nullptr ? false : m_TotalLungVolumeOR->IsValid();
}
SEScalarVolume& SEOverride::GetTotalLungVolumeOverride()
{
  if (m_TotalLungVolumeOR == nullptr) {
    m_TotalLungVolumeOR = new SEScalarVolume();
  }
  return *m_TotalLungVolumeOR;
}
double SEOverride::GetTotalLungVolumeOverride(const VolumeUnit& unit) const
{
  if (m_TotalLungVolumeOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TotalLungVolumeOR->GetValue(unit);
}
bool SEOverride::HasTotalPulmonaryVentilationOverride() const
{
  return m_TotalPulmonaryVentilationOR == nullptr ? false : m_TotalPulmonaryVentilationOR->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetTotalPulmonaryVentilationOverride()
{
  if (m_TotalPulmonaryVentilationOR == nullptr) {
    m_TotalPulmonaryVentilationOR = new SEScalarVolumePerTime();
  }
  return *m_TotalPulmonaryVentilationOR;
}
double SEOverride::GetTotalPulmonaryVentilationOverride(const VolumePerTimeUnit& unit) const
{
  if (m_TotalPulmonaryVentilationOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TotalPulmonaryVentilationOR->GetValue(unit);
}

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
  return m_ExtravascularFluidVolumeOR == nullptr ? false : m_ExtravascularFluidVolumeOR->IsValid();
}
SEScalarVolume& SEOverride::GetExtravascularFluidVolumeOverride()
{
  if (m_ExtravascularFluidVolumeOR == nullptr) {
    m_ExtravascularFluidVolumeOR = new SEScalarVolume();
  }
  return *m_ExtravascularFluidVolumeOR;
}
double SEOverride::GetExtravascularFluidVolumeOverride(const VolumeUnit& unit) const
{
  if (m_ExtravascularFluidVolumeOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_ExtravascularFluidVolumeOR->GetValue(unit);
}
bool SEOverride::HasIntracellularFluidVolumeOverride() const
{
  return m_IntracellularFluidVolumeOR == nullptr ? false : m_IntracellularFluidVolumeOR->IsValid();
}
SEScalarVolume& SEOverride::GetIntracellularFluidVolumeOverride()
{
  if (m_IntracellularFluidVolumeOR == nullptr) {
    m_IntracellularFluidVolumeOR = new SEScalarVolume();
  }
  return *m_IntracellularFluidVolumeOR;
}
double SEOverride::GetIntracellularFluidVolumeOverride(const VolumeUnit& unit) const
{
  if (m_IntracellularFluidVolumeOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_IntracellularFluidVolumeOR->GetValue(unit);
}
bool SEOverride::HasLiverGlycogenOverride() const
{
  return m_LiverGlycogenOR == nullptr ? false : m_LiverGlycogenOR->IsValid();
}
SEScalarMass& SEOverride::GetLiverGlycogenOverride()
{
  if (m_LiverGlycogenOR == nullptr) {
    m_LiverGlycogenOR = new SEScalarMass();
  }
  return *m_LiverGlycogenOR;
}
double SEOverride::GetLiverGlycogenOverride(const MassUnit& unit) const
{
  if (m_LiverGlycogenOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_LiverGlycogenOR->GetValue(unit);
}
bool SEOverride::HasMuscleGlycogenOverride() const
{
  return m_MuscleGlycogenOR == nullptr ? false : m_MuscleGlycogenOR->IsValid();
}
SEScalarMass& SEOverride::GetMuscleGlycogenOverride()
{
  if (m_MuscleGlycogenOR == nullptr) {
    m_MuscleGlycogenOR = new SEScalarMass();
  }
  return *m_MuscleGlycogenOR;
}
double SEOverride::GetMuscleGlycogenOverride(const MassUnit& unit) const
{
  if (m_MuscleGlycogenOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_MuscleGlycogenOR->GetValue(unit);
}
bool SEOverride::HasStoredProteinOverride() const
{
  return m_StoredProteinOR == nullptr ? false : m_StoredProteinOR->IsValid();
}
SEScalarMass& SEOverride::GetStoredProteinOverride()
{
  if (m_StoredProteinOR == nullptr) {
    m_StoredProteinOR = new SEScalarMass();
  }
  return *m_StoredProteinOR;
}
double SEOverride::GetStoredProteinOverride(const MassUnit& unit) const
{
  if (m_StoredProteinOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_StoredProteinOR->GetValue(unit);
}
bool SEOverride::HasStoredFatOverride() const
{
  return m_StoredFatOR == nullptr ? false : m_StoredFatOR->IsValid();
}
SEScalarMass& SEOverride::GetStoredFatOverride()
{
  if (m_StoredFatOR == nullptr) {
    m_StoredFatOR = new SEScalarMass();
  }
  return *m_StoredFatOR;
}
double SEOverride::GetStoredFatOverride(const MassUnit& unit) const
{
  if (m_StoredFatOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_StoredFatOR->GetValue(unit);
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
  if (HasCO2SaturationOverride()) {
    str << "\n\tCarbon Dioxide Saturation: ";
    HasCO2SaturationOverride() ? str << *m_CO2SaturationOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tCO2 Saturation has a lower bound of 0 and an upper bound of 1.";
    }
    str << std::flush;
  }
  if (HasCOSaturationOverride()) {
    str << "\n\tCarbon Monoxide Saturation: ";
    HasCOSaturationOverride() ? str << *m_COSaturationOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tCO Saturation has a lower bound of 0 and an upper bound of 1.";
    }
    str << std::flush;
  }
  if (HasO2SaturationOverride()) {
    str << "\n\tOxygen Saturation: ";
    HasO2SaturationOverride() ? str << *m_O2SaturationOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tO2 Saturation has a lower bound of 0 and an upper bound of 1.";
    }
    str << std::flush;
  }
  if (HasPhosphateOverride()) {
    str << "\n\tPhosphate: ";
    HasPhosphateOverride() ? str << *m_PhosphateOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tPhosphate has a lower bound of 0 mmol/L and an upper bound of 1000 mmol/L.";
    }
    str << std::flush;
  }
  if (HasWBCCountOverride()) {
    str << "\n\tWhite Blood Cell Count: ";
    HasWBCCountOverride() ? str << *m_WBCCountOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tWBC Count has a lower bound of 0 ct/uL and an upper bound of 50000 ct/uL.";
    }
    str << std::flush;
  }
  if (HasTotalBilirubinOverride()) {
    str << "\n\tTotal Bilirubin: ";
    HasTotalBilirubinOverride() ? str << *m_TotalBilirubinOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tTotal Bilirubin has a lower bound of 0 mg/dL and an upper bound of 500 mg/dL.";
    }
    str << std::flush;
  }
  if (HasCalciumConcentrationOverride()) {
    str << "\n\tCalcium Concentration: ";
    HasCalciumConcentrationOverride() ? str << *m_CalciumConcentrationOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tCalcium Concentration has a lower bound of 0 mg/dL and an upper bound of 500 mg/dL.";
    }
    str << std::flush;
  }
  if (HasGlucoseConcentrationOverride()) {
    str << "\n\tGlucose Concentration: ";
    HasGlucoseConcentrationOverride() ? str << *m_GlucoseConcentrationOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tGlucose Concentration has a lower bound of 0 mg/dL and an upper bound of 1000 mg/dL.";
    }
    str << std::flush;
  }
  if (HasLactateConcentrationOverride()) {
    str << "\n\tLactate Concentration: ";
    HasLactateConcentrationOverride() ? str << *m_LactateConcentrationOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tLactate Concentration has a lower bound of 0 mg/dL and an upper bound of 1000 mg/dL.";
    }
    str << std::flush;
  }
  if (HasPotassiumConcentrationOverride()) {
    str << "\n\tPotassium Concentration: ";
    HasPotassiumConcentrationOverride() ? str << *m_PotassiumConcentrationOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tPotassium Concentration has a lower bound of 0 mg/dL and an upper bound of 500 mg/dL.";
    }
    str << std::flush;
  }
  if (HasSodiumConcentrationOverride()) {
    str << "\n\tSodium Concentration: ";
    HasSodiumConcentrationOverride() ? str << *m_SodiumConcentrationOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tSodium Concentration has a lower bound of 0 mg/dL and an upper bound of 500 mg/dL.";
    }
    str << std::flush;
  }
  if (HasBloodVolumeOverride()) {
    str << "\n\tBlood Volume: ";
    HasBloodVolumeOverride() ? str << *m_BloodVolumeOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tBlood Volume has a lower bound of 0 L and an upper bound of 25 L.";
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
  if (HasDiastolicArterialPressureOverride()) {
    str << "\n\tDiastolic Arterial Pressure: ";
    HasDiastolicArterialPressureOverride() ? str << *m_DiastolicArtPressureOR : str << "Not Set";
    str << std::flush;
  }
  if (HasMAPOverride()) {
    str << "\n\tMean Arterial Pressure: ";
    HasMAPOverride() ? str << *m_MeanArtPressureOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::Off) {
      str << "\n\tPharmacodynamics affecting this value have been turned off due to conformance being turned off.";
    }
    str << std::flush;
  }
  if (HasHeartRateOverride()) {
    str << "\n\tHeart Rate: ";
    HasHeartRateOverride() ? str << *m_HeartRateOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tHeart Rate has a lower/upper bound dependent on the patient.";
    } else {
      str << "\n\tPharmacodynamics affecting this value have been turned off due to conformance being turned off.";
    }
    str << std::flush;
  }
  if (HasHeartStrokeVolumeOverride()) {
    str << "\n\tHeart Stroke Volume: ";
    HasHeartStrokeVolumeOverride() ? str << *m_HeartStrokeVolumeOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tHeart Stroke Volume has a lower bound of 0 mL and an upper bound of 5000 mL.";
    }
    str << std::flush;
  }
  if (HasSystolicArterialPressureOverride()) {
    str << "\n\tSystolic Arterial Pressure: ";
    HasSystolicArterialPressureOverride() ? str << *m_SystolicArtPressureOR : str << "Not Set";
    str << std::flush;
  }
  if (HasInsulinSynthesisRateOverride()) {
    str << "\n\tInsulin Synthesis Rate: ";
    HasInsulinSynthesisRateOverride() ? str << *m_InsulinSynthesisRateOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tInsulin Synthesis Rate has a lower bound of 0 pmol/min and an upper bound of 10 pmol/min.";
    }
    str << std::flush;
  }
  if (HasGlucagonSynthesisRateOverride()) {
    str << "\n\tGlucagon Synthesis Rate: ";
    HasGlucagonSynthesisRateOverride() ? str << *m_GlucagonSynthesisRateOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tGlucagon Synthesis Rate has a lower bound of 0 pmol/min and an upper bound of 10 pmol/min.";
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
  if (HasCoreTemperatureOverride()) {
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
  if (HasUrineUreaNitrogenConcentrationOverride()) {
    str << "\n\tUrine Urea Nitrogen Concentration: ";
    HasUrineUreaNitrogenConcentrationOverride() ? str << *m_UrineUreaNitrogenConcentrationOverrideOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tUrine Urea Nitrogen Concentration has a lower bound of 0 g/L and an upper bound of 100 g/L.";
    }
    str << std::flush;
  }
  if (HasExpiratoryFlowOverride()) {
    str << "\n\tExpiratory Flow: ";
    HasExpiratoryFlowOverride() ? str << *m_ExpiratoryFlowOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tExpiratory Flow has a lower bound of 0 L/min and an upper bound of 1000 L/min.";
    }
    str << std::flush;
  }
  if (HasInspiratoryFlowOverride()) {
    str << "\n\tInspiratory Flow: ";
    HasInspiratoryFlowOverride() ? str << *m_InspiratoryFlowOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tInspiratory Flow has a lower bound of 0 L/min and an upper bound of 1000 L/min.";
    }
    str << std::flush;
  }
  if (HasPulmonaryComplianceOverride()) {
    str << "\n\tPulmonary Compliance: ";
    HasPulmonaryComplianceOverride() ? str << *m_PulmonaryComplianceOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tPulmonary Compliance has a lower bound of 0 L/cmH2O and an upper bound of 1000 L/cmH20.";
    }
    str << std::flush;
  }
  if (HasPulmonaryResistanceOverride()) {
    str << "\n\tPulmonary Resistance: ";
    HasPulmonaryResistanceOverride() ? str << *m_PulmonaryResistanceOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tPulmonary Resistance has a lower bound of 0 cmH2O*s/L and an upper bound of 1000 cmH2O*s/L.";
    }
    str << std::flush;
  }
  if (HasRespirationRateOverride()) {
    str << "\n\tRespiration Rate: ";
    HasRespirationRateOverride() ? str << *m_RespirationRateOR : str << "Not Set";
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
  if (HasTargetPulmonaryVentilationOverride()) {
    str << "\n\tTarget Pulmonary Ventilation: ";
    HasTargetPulmonaryVentilationOverride() ? str << *m_TargetPulmonaryVentilationOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tTarget Pulmonary Ventilation has a lower bound of 0 L/min and an upper bound of 1000 L/min.";
    }
    str << std::flush;
  }
  if (HasTotalAlveolarVentilationOverride()) {
    str << "\n\tTotal Alveolar Ventilation: ";
    HasTotalAlveolarVentilationOverride() ? str << *m_TotalAlveolarVentilationOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tTotal Alveolar Ventilation has a lower bound of 0 L/min and an upper bound of 1000 L/min.";
    }
    str << std::flush;
  }
  if (HasTotalLungVolumeOverride()) {
    str << "\n\tTotal Lung Volume: ";
    HasTotalLungVolumeOverride() ? str << *m_TotalLungVolumeOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tTotal Lung Volume has a lower bound of 0 L and an upper bound of 500 L.";
    }
    str << std::flush;
  }
  if (HasTotalPulmonaryVentilationOverride()) {
    str << "\n\tTotal Pulmonary Ventilation: ";
    HasTotalPulmonaryVentilationOverride() ? str << *m_TotalPulmonaryVentilationOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tTotal Pulmonary Ventilation has a lower bound of 0 L/min and an upper bound of 1000 L/min.";
    }
    str << std::flush;
  }
  if (HasExtravascularFluidVolumeOverride()) {
    str << "\n\tExtravascular Fluid Volume: ";
    HasExtravascularFluidVolumeOverride() ? str << *m_ExtravascularFluidVolumeOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tExtravascular Fluid Volume has a lower bound of 0 L and an upper bound of 1000 L.";
    }
    str << std::flush;
  }
  if (HasIntracellularFluidVolumeOverride()) {
    str << "\n\tIntracellular Fluid Volume: ";
    HasIntracellularFluidVolumeOverride() ? str << *m_IntracellularFluidVolumeOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tIntracellular Fluid Volume has a lower bound of 0 L and an upper bound of 1000 L.";
    }
    str << std::flush;
  }
  if (HasLiverGlycogenOverride()) {
    str << "\n\tLiver Glycogen: ";
    HasLiverGlycogenOverride() ? str << *m_LiverGlycogenOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tLiver Glycogen has a lower bound of 0 g and an upper bound of 1000 g.";
    }
    str << std::flush;
  }
  if (HasMuscleGlycogenOverride()) {
    str << "\n\tMuscle Glycogen: ";
    HasMuscleGlycogenOverride() ? str << *m_MuscleGlycogenOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tMuscle Glycogen has a lower bound of 0 g and an upper bound of 2000 g.";
    }
    str << std::flush;
  }
  if (HasStoredProteinOverride()) {
    str << "\n\tStored Protein: ";
    HasStoredProteinOverride() ? str << *m_StoredProteinOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tStored Protein has a lower bound of 0 g and an upper bound of 5000 g.";
    }
    str << std::flush;
  }
  if (HasStoredFatOverride()) {
    str << "\n\tStored Fat: ";
    HasStoredFatOverride() ? str << *m_StoredFatOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tStored Fat has a lower bound of 0 g and an upper bound of 5000 g.";
    }
    str << std::flush;
  }
}
}
