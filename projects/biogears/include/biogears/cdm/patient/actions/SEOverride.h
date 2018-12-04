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

#include <biogears/schema/cdm/PatientActions.hxx>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarPower.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarAmountPerTime.h>
#include <biogears/cdm/properties/SEScalarMassPerTime.h>

class Serializer;

namespace biogears {
class OsmolarityUnit;
class MassUnit;

class BIOGEARS_API SEOverride : public SEPatientAction {
public:
  SEOverride();
  virtual ~SEOverride();

  static constexpr const char* TypeTag() { return "SEOverride"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual bool Load(const CDM::OverrideData& in);
  virtual CDM::OverrideData* Unload() const;

protected:
  virtual void Unload(CDM::OverrideData& data) const;

public:
  CDM::enumOnOff::value GetOverrideSwitch() const;
  void SetOverrideSwitch(CDM::enumOnOff::value state);
  bool HasOverrideSwitch() const;
  void InvalidateOverrideSwitch();
  CDM::enumOnOff::value GetOverrideConformance() const;
  void SetOverrideConformance(CDM::enumOnOff::value valid);
  bool HasOverrideConformance() const;
  void InvalidateOverrideConformance();
  bool IsOverrideConformant();

  //Blood Chemistry
  bool HasArterialPHOverride() const;
  SEScalar& GetArterialPHOverride();
  double GetArterialPHOverride() const;
  bool HasVenousPHOverride() const;
  SEScalar& GetVenousPHOverride();
  double GetVenousPHOverride() const;
  bool HasBloodChemistryOverride() const;

  //Cardiovascular
  bool HasCardiacOutputOverride() const;
  SEScalarVolumePerTime& GetCardiacOutputOverride();
  double GetCardiacOutputOverride(const VolumePerTimeUnit& unit) const;
  bool HasMAPOverride() const;
  SEScalarPressure& GetMAPOverride();
  double GetMAPOverride(const PressureUnit& unit) const;
  bool HasHeartRateOverride() const;
  SEScalarFrequency& GetHeartRateOverride();
  double GetHeartRateOverride(const FrequencyUnit& unit) const;
  bool HasCardiovascularOverride() const;

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
  bool HasLeftGlomerularFiltrationRateOverride() const;
  SEScalarVolumePerTime& GetLeftGlomerularFiltrationRateOverride();
  double GetLeftGlomerularFiltrationRateOverride(const VolumePerTimeUnit& unit) const;
  bool HasRenalBloodFlowOverride() const;
  SEScalarVolumePerTime& GetRenalBloodFlowOverride();
  double GetRenalBloodFlowOverride(const VolumePerTimeUnit& unit) const;
  bool HasRightGlomerularFiltrationRateOverride() const;
  SEScalarVolumePerTime& GetRightGlomerularFiltrationRateOverride();
  double GetRightGlomerularFiltrationRateOverride(const VolumePerTimeUnit& unit) const;
  bool HasUrineProductionRateOverride() const;
  SEScalarVolumePerTime& GetUrineProductionRateOverride();
  double GetUrineProductionRateOverride(const VolumePerTimeUnit& unit) const;
  bool HasUrineOsmolarityOverride() const;
  SEScalarOsmolarity& GetUrineOsmolarityOverride();
  double GetUrineOsmolarityOverride(const OsmolarityUnit& unit) const;
  bool HasUrineVolumeOverride() const;
  SEScalarVolume& GetUrineVolumeOverride();
  double GetUrineVolumeOverride(const VolumeUnit& unit) const;

  bool HasRenalOverride() const;

  //Respiratory
  bool HasRespirationRateOverride() const;
  SEScalarFrequency& GetRespirationRateOverride();
  double GetRespirationRateOverride(const FrequencyUnit& unit) const;
  bool HasTidalVolumeOverride() const;
  SEScalarVolume& GetTidalVolumeOverride();
  double GetTidalVolumeOverride(const VolumeUnit& unit) const;
  bool HasRespiratoryOverride() const;

  virtual void ToString(std::ostream& str) const;

protected:
  CDM::enumOnOff m_OverrideSwitch;
  CDM::enumOnOff m_OverrideConformance;
  SEScalar* m_ArterialPHOR;
  SEScalar* m_VenousPHOR;
  SEScalarVolumePerTime* m_CardiacOutputOR;
  SEScalarPressure* m_PressureOR;
  SEScalarFrequency* m_HeartRateOR;
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
  SEScalarVolumePerTime* m__LeftGlomerularFiltrationOR;
  SEScalarVolumePerTime* m_RenalBloodFlowOR;
  SEScalarVolumePerTime* m__RightGlomerularFiltrationOR;
  SEScalarVolumePerTime* m_UrineProductionRateOR;
  SEScalarOsmolarity* m_UrineOsmolarityOR;
  SEScalarVolume* m_UrineVolumeOR;
  SEScalarFrequency* m_RespirationRateOR;
  SEScalarVolume* m_TidalVolumeOR;
};
}
