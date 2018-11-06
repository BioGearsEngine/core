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

class Serializer;

namespace biogears {
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
  bool HasCoreTemperatureOverride() const;
  SEScalarTemperature& GetCoreTemperatureOverride();
  double GetCoreTemperatureOverride(const TemperatureUnit& unit) const;
  bool HasSkinTemperatureOverride() const;
  SEScalarTemperature& GetSkinTemperatureOverride();
  double GetSkinTemperatureOverride(const TemperatureUnit& unit) const;
  bool HasTotalMetabolicOverride() const;
  SEScalarPower& GetTotalMetabolicOverride();
  double GetTotalMetabolicOverride(const PowerUnit& unit) const;
  bool HasEnergyOverride() const;

  //Renal
  bool HasUrineProductionRateOverride() const;
  SEScalarVolumePerTime& GetUrineProductionRateOverride();
  double GetUrineProductionRateOverride(const VolumePerTimeUnit& unit) const;
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
  SEScalarTemperature* m_CoreTemperatureOR;
  SEScalarTemperature* m_SkinTemperatureOR;
  SEScalarPower* m_TotalMetabolicOR;
  SEScalarVolumePerTime* m_UrineProductionRateOR;
  SEScalarFrequency* m_RespirationRateOR;
  SEScalarVolume* m_TidalVolumeOR;
};
}
