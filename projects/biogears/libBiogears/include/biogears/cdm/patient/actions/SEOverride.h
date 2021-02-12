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

  virtual bool Load(const CDM::OverrideData& in);
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

protected:
  CDM::enumOnOff m_OverrideState;
  CDM::enumOnOff m_OverrideConformance;
  SEScalar* m_ArterialPHOR;
  SEScalar* m_VenousPHOR;
  SEScalarFraction* m_CO2SaturationOR;
  SEScalarFraction* m_COSaturationOR;
  SEScalarFraction* m_O2SaturationOR;
  SEScalarAmountPerVolume* m_PhosphateOR;
  SEScalarAmountPerVolume* m_WBCCountOR;
  SEScalarMassPerVolume* m_TotalBilirubinOR;
  SEScalarMassPerVolume* m_CalciumConcentrationOR;
  SEScalarMassPerVolume* m_GlucoseConcentrationOR;
  SEScalarMassPerVolume* m_LactateConcentrationOR;
  SEScalarMassPerVolume* m_PotassiumConcentrationOR;
  SEScalarMassPerVolume* m_SodiumConcentrationOR;
  SEScalarVolume* m_BloodVolumeOR;
  SEScalarVolumePerTime* m_CardiacOutputOR;
  SEScalarPressure* m_DiastolicArtPressureOR;
  SEScalarPressure* m_MeanArtPressureOR;
  SEScalarFrequency* m_HeartRateOR;
  SEScalarVolume* m_HeartStrokeVolumeOR;
  SEScalarPressure* m_SystolicArtPressureOR;
  SEScalarAmountPerTime* m_InsulinSynthesisRateOR;
  SEScalarAmountPerTime* m_GlucagonSynthesisRateOR;
  SEScalarFraction* m_AcheivedExerciseLevelOR;
  SEScalarTemperature* m_CoreTemperatureOR;
  SEScalarAmountPerTime* m_CreatinineProductionRateOR;
  SEScalarPressure* m_ExerciseMeanArterialPressureDeltaOR;
  SEScalarFraction* m_FatigueLevelOR;
  SEScalarAmountPerTime* m_LactateProductionRateOR;
  SEScalarTemperature* m_SkinTemperatureOR;
  SEScalarMassPerTime* m_SweatRateOR;
  SEScalarPower* m_TotalMetabolicOR;
  SEScalarFraction* m_TotalWorkRateLevelOR;
  SEScalarMass* m_SodiumLostToSweatOR;
  SEScalarMass* m_PotassiumLostToSweatOR;
  SEScalarMass* m_ChlorideLostToSweatOR;
  SEScalarFlowResistance* m_LAfferentArterioleResistOR;
  SEScalarVolumePerTime* m__LeftGlomerularFiltrationOR;
  SEScalarVolumePerTime* m_LReabsorptionRateOR;
  SEScalarVolumePerTime* m_RenalBloodFlowOR;
  SEScalarVolumePerTime* m_RenalPlasmaOR;
  SEScalarFlowResistance* m_RAfferentArterioleResistOR;
  SEScalarVolumePerTime* m__RightGlomerularFiltrationOR;
  SEScalarVolumePerTime* m_RReabsorptionRateOR;
  SEScalarVolumePerTime* m_UrinationRateOR;
  SEScalarVolumePerTime* m_UrineProductionRateOR;
  SEScalarOsmolality* m_UrineOsmolalityOR;
  SEScalarVolume* m_UrineVolumeOR;
  SEScalarMassPerVolume* m_UrineUreaNitrogenConcentrationOverrideOR;
  SEScalarVolumePerTime* m_ExpiratoryFlowOR;
  SEScalarVolumePerTime* m_InspiratoryFlowOR;
  SEScalarFlowCompliance* m_PulmonaryComplianceOR;
  SEScalarFlowResistance* m_PulmonaryResistanceOR;
  SEScalarFrequency* m_RespirationRateOR;
  SEScalarVolume* m_TidalVolumeOR;
  SEScalarVolumePerTime* m_TargetPulmonaryVentilationOR;
  SEScalarVolumePerTime* m_TotalAlveolarVentilationOR;
  SEScalarVolume* m_TotalLungVolumeOR;
  SEScalarVolumePerTime* m_TotalPulmonaryVentilationOR;
  SEScalarVolume* m_ExtravascularFluidVolumeOR;
  SEScalarVolume* m_IntracellularFluidVolumeOR;
  SEScalarMass* m_LiverGlycogenOR;
  SEScalarMass* m_MuscleGlycogenOR;
  SEScalarMass* m_StoredProteinOR;
  SEScalarMass* m_StoredFatOR;
};
}
