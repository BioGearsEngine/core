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

#pragma once
#include <biogears/cdm/patient/actions/SEPatientAction.h>

#include <biogears/cdm/properties/SEScalarAmountPerTime.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarFlowCompliance.h>
#include <biogears/cdm/properties/SEScalarFlowResistance.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerTime.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarOsmolality.h>
#include <biogears/cdm/properties/SEScalarPower.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/schema/cdm/PatientActions.hxx>

#include <random>

class Serializer;

namespace biogears {
namespace io {
  class PatientActions;
}
class BIOGEARS_API SEOverride : public SEPatientAction {
  friend io::PatientActions;

public:
  SEOverride();
  virtual ~SEOverride();

  static constexpr const char* TypeTag() { return "SEOverride"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override; //clear memory

  virtual bool IsValid() const override;
  virtual bool IsActive() const override;

  virtual bool Load(const CDM::OverrideData& in, std::default_random_engine *rd = nullptr);
  virtual CDM::OverrideData* Unload() const override;

protected:
  virtual void Unload(CDM::OverrideData& data) const;

public:
  CDM::enumOnOff::value GetOverrideState() const;
  void SetOverrideState(CDM::enumOnOff::value state);
  bool HasOverrideState() const;
  void InvalidateOverrideState();
  CDM::enumOnOff::value GetOverrideConformance() const;
  void SetOverrideConformance(CDM::enumOnOff::value valid);
  bool HasOverrideConformance() const;
  void InvalidateOverrideConformance();

  //Blood Chemistry
  bool HasArterialPHOverride() const;
  SEScalar& GetArterialPHOverride();
  double GetArterialPHOverride() const;
  bool HasVenousPHOverride() const;
  SEScalar& GetVenousPHOverride();
  double GetVenousPHOverride() const;
  bool HasCO2SaturationOverride() const;
  SEScalarFraction& GetCO2SaturationOverride();
  double GetCO2SaturationOverride() const;
  bool HasCOSaturationOverride() const;
  SEScalarFraction& GetCOSaturationOverride();
  double GetCOSaturationOverride() const;
  bool HasO2SaturationOverride() const;
  SEScalarFraction& GetO2SaturationOverride();
  double GetO2SaturationOverride() const;
  bool HasPhosphateOverride() const;
  SEScalarAmountPerVolume& GetPhosphateOverride();
  double GetPhosphateOverride(const AmountPerVolumeUnit& unit) const;
  bool HasWBCCountOverride() const;
  SEScalarAmountPerVolume& GetWBCCountOverride();
  double GetWBCCountOverride(const AmountPerVolumeUnit& unit) const;
  bool HasTotalBilirubinOverride() const;
  SEScalarMassPerVolume& GetTotalBilirubinOverride();
  double GetTotalBilirubinOverride(const MassPerVolumeUnit& unit) const;
  bool HasCalciumConcentrationOverride() const;
  SEScalarMassPerVolume& GetCalciumConcentrationOverride();
  double GetCalciumConcentrationOverride(const MassPerVolumeUnit& unit) const;
  bool HasGlucoseConcentrationOverride() const;
  SEScalarMassPerVolume& GetGlucoseConcentrationOverride();
  double GetGlucoseConcentrationOverride(const MassPerVolumeUnit& unit) const;
  bool HasLactateConcentrationOverride() const;
  SEScalarMassPerVolume& GetLactateConcentrationOverride();
  double GetLactateConcentrationOverride(const MassPerVolumeUnit& unit) const;
  bool HasPotassiumConcentrationOverride() const;
  SEScalarMassPerVolume& GetPotassiumConcentrationOverride();
  double GetPotassiumConcentrationOverride(const MassPerVolumeUnit& unit) const;
  bool HasSodiumConcentrationOverride() const;
  SEScalarMassPerVolume& GetSodiumConcentrationOverride();
  double GetSodiumConcentrationOverride(const MassPerVolumeUnit& unit) const;

  bool HasBloodChemistryOverride() const;

  //Cardiovascular
  bool HasBloodVolumeOverride() const;
  SEScalarVolume& GetBloodVolumeOverride();
  double GetBloodVolumeOverride(const VolumeUnit& unit) const;
  bool HasCardiacOutputOverride() const;
  SEScalarVolumePerTime& GetCardiacOutputOverride();
  double GetCardiacOutputOverride(const VolumePerTimeUnit& unit) const;
  bool HasDiastolicArterialPressureOverride() const;
  SEScalarPressure& GetDiastolicArterialPressureOverride();
  double GetDiastolicArterialPressureOverride(const PressureUnit& unit) const;
  bool HasMAPOverride() const;
  SEScalarPressure& GetMAPOverride();
  double GetMAPOverride(const PressureUnit& unit) const;
  bool HasHeartRateOverride() const;
  SEScalarFrequency& GetHeartRateOverride();
  double GetHeartRateOverride(const FrequencyUnit& unit) const;
  bool HasHeartStrokeVolumeOverride() const;
  SEScalarVolume& GetHeartStrokeVolumeOverride();
  double GetHeartStrokeVolumeOverride(const VolumeUnit& unit) const;
  bool HasSystolicArterialPressureOverride() const;
  SEScalarPressure& GetSystolicArterialPressureOverride();
  double GetSystolicArterialPressureOverride(const PressureUnit& unit) const;
  bool HasCardiovascularOverride() const;

  //Endocrine
  bool HasInsulinSynthesisRateOverride() const;
  SEScalarAmountPerTime& GetInsulinSynthesisRateOverride();
  double GetInsulinSynthesisRateOverride(const AmountPerTimeUnit& unit) const;
  bool HasGlucagonSynthesisRateOverride() const;
  SEScalarAmountPerTime& GetGlucagonSynthesisRateOverride();
  double GetGlucagonSynthesisRateOverride(const AmountPerTimeUnit& unit) const;
  bool HasEndocrineOverride() const;

  //Energy
  bool HasAchievedExerciseLevelOverride() const;
  SEScalarFraction& GetAchievedExerciseLevelOverride();
  double GetAchievedExerciseLevelOverride() const;
  bool HasCoreTemperatureOverride() const;
  SEScalarTemperature& GetCoreTemperatureOverride();
  double GetCoreTemperatureOverride(const TemperatureUnit& unit) const;
  bool HasCreatinineProductionRateOverride() const;
  SEScalarAmountPerTime& GetCreatinineProductionRateOverride();
  double GetCreatinineProductionRateOverride(const AmountPerTimeUnit& unit) const;
  bool HasExerciseMeanArterialPressureDeltaOverride() const;
  SEScalarPressure& GetExerciseMeanArterialPressureDeltaOverride();
  double GetExerciseMeanArterialPressureDeltaOverride(const PressureUnit& unit) const;
  bool HasFatigueLevelOverride() const;
  SEScalarFraction& GetFatigueLevelOverride();
  double GetFatigueLevelOverride() const;
  bool HasLactateProductionRateOverride() const;
  SEScalarAmountPerTime& GetLactateProductionRateOverride();
  double GetLactateProductionRateOverride(const AmountPerTimeUnit& unit) const;
  bool HasSkinTemperatureOverride() const;
  SEScalarTemperature& GetSkinTemperatureOverride();
  double GetSkinTemperatureOverride(const TemperatureUnit& unit) const;
  bool HasSweatRateOverride() const;
  SEScalarMassPerTime& GetSweatRateOverride();
  double GetSweatRateOverride(const MassPerTimeUnit& unit) const;
  bool HasTotalMetabolicOverride() const;
  SEScalarPower& GetTotalMetabolicOverride();
  double GetTotalMetabolicOverride(const PowerUnit& unit) const;
  bool HasTotalWorkRateLevelOverride() const;
  SEScalarFraction& GetTotalWorkRateLevelOverride();
  double GetTotalWorkRateLevelOverride() const;
  bool HasSodiumLostToSweatOverride() const;
  SEScalarMass& GetSodiumLostToSweatOverride();
  double GetSodiumLostToSweatOverride(const MassUnit& unit) const;
  bool HasPotassiumLostToSweatOverride() const;
  SEScalarMass& GetPotassiumLostToSweatOverride();
  double GetPotassiumLostToSweatOverride(const MassUnit& unit) const;
  bool HasChlorideLostToSweatOverride() const;
  SEScalarMass& GetChlorideLostToSweatOverride();
  double GetChlorideLostToSweatOverride(const MassUnit& unit) const;

  bool HasEnergyOverride() const;

  //Renal
  bool HasLeftAfferentArterioleResistanceOverride() const;
  SEScalarFlowResistance& GetLeftAfferentArterioleResistanceOverride();
  double GetLeftAfferentArterioleResistanceOverride(const FlowResistanceUnit& unit) const;
  bool HasLeftGlomerularFiltrationRateOverride() const;
  SEScalarVolumePerTime& GetLeftGlomerularFiltrationRateOverride();
  double GetLeftGlomerularFiltrationRateOverride(const VolumePerTimeUnit& unit) const;
  bool HasLeftReaborptionRateOverride() const;
  SEScalarVolumePerTime& GetLeftReaborptionRateOverride();
  double GetLeftReaborptionRateOverride(const VolumePerTimeUnit& unit) const;
  bool HasRenalBloodFlowOverride() const;
  SEScalarVolumePerTime& GetRenalBloodFlowOverride();
  double GetRenalBloodFlowOverride(const VolumePerTimeUnit& unit) const;
  bool HasRenalPlasmaFlowOverride() const;
  SEScalarVolumePerTime& GetRenalPlasmaFlowOverride();
  double GetRenalPlasmaFlowOverride(const VolumePerTimeUnit& unit) const;
  bool HasRightAfferentArterioleResistanceOverride() const;
  SEScalarFlowResistance& GetRightAfferentArterioleResistanceOverride();
  double GetRightAfferentArterioleResistanceOverride(const FlowResistanceUnit& unit) const;
  bool HasRightGlomerularFiltrationRateOverride() const;
  SEScalarVolumePerTime& GetRightGlomerularFiltrationRateOverride();
  double GetRightGlomerularFiltrationRateOverride(const VolumePerTimeUnit& unit) const;
  bool HasRightReaborptionRateOverride() const;
  SEScalarVolumePerTime& GetRightReaborptionRateOverride();
  double GetRightReaborptionRateOverride(const VolumePerTimeUnit& unit) const;
  bool HasUrinationRateOverride() const;
  SEScalarVolumePerTime& GetUrinationRateOverride();
  double GetUrinationRateOverride(const VolumePerTimeUnit& unit) const;
  bool HasUrineProductionRateOverride() const;
  SEScalarVolumePerTime& GetUrineProductionRateOverride();
  double GetUrineProductionRateOverride(const VolumePerTimeUnit& unit) const;
  bool HasUrineOsmolalityOverride() const;
  SEScalarOsmolality& GetUrineOsmolalityOverride();
  double GetUrineOsmolalityOverride(const OsmolalityUnit& unit) const;
  bool HasUrineVolumeOverride() const;
  SEScalarVolume& GetUrineVolumeOverride();
  double GetUrineVolumeOverride(const VolumeUnit& unit) const;
  bool HasUrineUreaNitrogenConcentrationOverride() const;
  SEScalarMassPerVolume& GetUrineUreaNitrogenConcentrationOverride();
  double GetUrineUreaNitrogenConcentrationOverride(const MassPerVolumeUnit& unit) const;

  bool HasRenalOverride() const;

  //Respiratory
  bool HasExpiratoryFlowOverride() const;
  SEScalarVolumePerTime& GetExpiratoryFlowOverride();
  double GetExpiratoryFlowOverride(const VolumePerTimeUnit& unit) const;
  bool HasInspiratoryFlowOverride() const;
  SEScalarVolumePerTime& GetInspiratoryFlowOverride();
  double GetInspiratoryFlowOverride(const VolumePerTimeUnit& unit) const;
  bool HasPulmonaryComplianceOverride() const;
  SEScalarFlowCompliance& GetPulmonaryComplianceOverride();
  double GetPulmonaryComplianceOverride(const FlowComplianceUnit& unit) const;
  bool HasPulmonaryResistanceOverride() const;
  SEScalarFlowResistance& GetPulmonaryResistanceOverride();
  double GetPulmonaryResistanceOverride(const FlowResistanceUnit& unit) const;
  bool HasRespirationRateOverride() const;
  SEScalarFrequency& GetRespirationRateOverride();
  double GetRespirationRateOverride(const FrequencyUnit& unit) const;
  bool HasTidalVolumeOverride() const;
  SEScalarVolume& GetTidalVolumeOverride();
  double GetTidalVolumeOverride(const VolumeUnit& unit) const;
  bool HasTargetPulmonaryVentilationOverride() const;
  SEScalarVolumePerTime& GetTargetPulmonaryVentilationOverride();
  double GetTargetPulmonaryVentilationOverride(const VolumePerTimeUnit& unit) const;
  bool HasTotalAlveolarVentilationOverride() const;
  SEScalarVolumePerTime& GetTotalAlveolarVentilationOverride();
  double GetTotalAlveolarVentilationOverride(const VolumePerTimeUnit& unit) const;
  bool HasTotalLungVolumeOverride() const;
  SEScalarVolume& GetTotalLungVolumeOverride();
  double GetTotalLungVolumeOverride(const VolumeUnit& unit) const;
  bool HasTotalPulmonaryVentilationOverride() const;
  SEScalarVolumePerTime& GetTotalPulmonaryVentilationOverride();
  double GetTotalPulmonaryVentilationOverride(const VolumePerTimeUnit& unit) const;

  bool HasRespiratoryOverride() const;

  //Tissue
  bool HasExtravascularFluidVolumeOverride() const;
  SEScalarVolume& GetExtravascularFluidVolumeOverride();
  double GetExtravascularFluidVolumeOverride(const VolumeUnit& unit) const;
  bool HasIntracellularFluidVolumeOverride() const;
  SEScalarVolume& GetIntracellularFluidVolumeOverride();
  double GetIntracellularFluidVolumeOverride(const VolumeUnit& unit) const;
  bool HasLiverGlycogenOverride() const;
  SEScalarMass& GetLiverGlycogenOverride();
  double GetLiverGlycogenOverride(const MassUnit& unit) const;
  bool HasMuscleGlycogenOverride() const;
  SEScalarMass& GetMuscleGlycogenOverride();
  double GetMuscleGlycogenOverride(const MassUnit& unit) const;
  bool HasStoredProteinOverride() const;
  SEScalarMass& GetStoredProteinOverride();
  double GetStoredProteinOverride(const MassUnit& unit) const;
  bool HasStoredFatOverride() const;
  SEScalarMass& GetStoredFatOverride();
  double GetStoredFatOverride(const MassUnit& unit) const;

  bool HasTissueOverride() const;

  virtual void ToString(std::ostream& str) const override;
  
  bool operator==( const SEOverride& rhs) const;
  bool operator!=( const SEOverride& rhs) const;

protected:
  CDM::enumOnOff m_OverrideState;
  CDM::enumOnOff m_OverrideConformance;
  SEScalar* m_ArterialBloodPHOverride;
  SEScalar* m_VenousBloodPHOverride;
  SEScalarFraction* m_CarbonDioxideSaturationOverride;
  SEScalarFraction* m_CarbonMonoxideSaturationOverride;
  SEScalarFraction* m_OxygenSaturationOverride;
  SEScalarAmountPerVolume* m_PhosphateOverride;
  SEScalarAmountPerVolume* m_WhiteBloodCellCountOverride;
  SEScalarMassPerVolume* m_TotalBilirubinOverride;
  SEScalarMassPerVolume* m_CalciumConcentrationOverride;
  SEScalarMassPerVolume* m_GlucoseConcentrationOverride;
  SEScalarMassPerVolume* m_LactateConcentrationOverride;
  SEScalarMassPerVolume* m_PotassiumConcentrationOverride;
  SEScalarMassPerVolume* m_SodiumConcentrationOverride;
  SEScalarVolume* m_BloodVolumeOverride;
  SEScalarVolumePerTime* m_CardiacOutputOverride;
  SEScalarPressure* m_DiastolicArterialPressureOverride;
  SEScalarPressure* m_MeanArterialPressureOverride;
  SEScalarFrequency* m_HeartRateOverride;
  SEScalarVolume* m_HeartStrokeVolumeOverride;
  SEScalarPressure* m_SystolicArterialPressureOverride;
  SEScalarAmountPerTime* m_InsulinSynthesisRateOverride;
  SEScalarAmountPerTime* m_GlucagonSynthesisRateOverride;
  SEScalarFraction* m_AchievedExerciseLevelOverride;
  SEScalarTemperature* m_CoreTemperatureOverride;
  SEScalarAmountPerTime* m_CreatinineProductionRateOverride;
  SEScalarPressure* m_ExerciseMeanArterialPressureDeltaOverride;
  SEScalarFraction* m_FatigueLevelOverride;
  SEScalarAmountPerTime* m_LactateProductionRateOverride;
  SEScalarTemperature* m_SkinTemperatureOverride;
  SEScalarMassPerTime* m_SweatRateOverride;
  SEScalarPower* m_TotalMetabolicRateOverride;
  SEScalarFraction* m_TotalWorkRateLevelOverride;
  SEScalarMass* m_SodiumLostToSweatOverride;
  SEScalarMass* m_PotassiumLostToSweatOverride;
  SEScalarMass* m_ChlorideLostToSweatOverride;
  SEScalarFlowResistance* m_LeftAfferentArterioleResistanceOverride;
  SEScalarVolumePerTime* m_LeftGlomerularFiltrationRateOverride;
  SEScalarVolumePerTime* m_LeftReabsorptionRateOverride;
  SEScalarVolumePerTime* m_RenalBloodFlowOverride;
  SEScalarVolumePerTime* m_RenalPlasmaFlowOverride;
  SEScalarFlowResistance* m_RightAfferentArterioleResistanceOverride;
  SEScalarVolumePerTime* m_RightGlomerularFiltrationRateOverride;
  SEScalarVolumePerTime* m_RightReabsorptionRateOverride;
  SEScalarVolumePerTime* m_UrinationRateOverride;
  SEScalarVolumePerTime* m_UrineProductionRateOverride;
  SEScalarOsmolality* m_UrineOsmolalityOverride;
  SEScalarVolume* m_UrineVolumeOverride;
  SEScalarMassPerVolume* m_UrineUreaNitrogenConcentrationOverride;
  SEScalarVolumePerTime* m_ExpiratoryFlowOverride;
  SEScalarVolumePerTime* m_InspiratoryFlowOverride;
  SEScalarFlowCompliance* m_PulmonaryComplianceOverride;
  SEScalarFlowResistance* m_PulmonaryResistanceOverride;
  SEScalarFrequency* m_RespirationRateOverride;
  SEScalarVolume* m_TidalVolumeOverride;
  SEScalarVolumePerTime* m_TargetPulmonaryVentilationOverride;
  SEScalarVolumePerTime* m_TotalAlveolarVentilationOverride;
  SEScalarVolume* m_TotalLungVolumeOverride;
  SEScalarVolumePerTime* m_TotalPulmonaryVentilationOverride;
  SEScalarVolume* m_ExtravascularFluidVolumeOverride;
  SEScalarVolume* m_IntracellularFluidVolumeOverride;
  SEScalarMass* m_LiverGlycogenOverride;
  SEScalarMass* m_MuscleGlycogenOverride;
  SEScalarMass* m_StoredProteinOverride;
  SEScalarMass* m_StoredFatOverride;
};
}
