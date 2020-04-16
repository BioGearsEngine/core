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
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/exports.h>

#include <biogears/schema/cdm/Patient.hxx>

CDM_BIND_DECL(PatientData)

namespace biogears {
  
class SEEventHandler;
class SENutrition;
class SEScalar;
class TimeUnit;
class SEScalarMass;
class MassUnit;
class SEScalarLength;
class LengthUnit;
class SEScalarArea;
class AreaUnit;
class SEScalarPower;
class PowerUnit;
class SEScalarVolume;
class VolumeUnit;
class SEScalarMassPerVolume;
class MassPerVolumeUnit;
class SEScalarFraction;
class SEScalarPressure;
class PressureUnit;
class SEScalarFrequency;
class FrequencyUnit;
class SEScalarNeg1To1;
class SEScalarVolumePerTime;
class VolumePerTimeUnit;

class BIOGEARS_API SEPatient : public Loggable {
public:
  SEPatient(Logger* logger);
  virtual ~SEPatient();

  virtual void Clear();

  virtual bool Load(const CDM::PatientData& in);

  bool Load(const char* patientFile);
  bool Load(const std::string& patientFile);

  virtual CDM::PatientData* Unload() const;

  /** @name GetScalar
  *   @brief - A reflextion type call that will return the Scalar associated
  *            with the string. ex. GetScalar("Hematocrit") will return the
  *            SEScalarPercent object associated with Hematocrit
  *   @details - Note this is an expensive call as it will string compare many times
  *              This is best used, and intended for, you to dynamically prepopulate
  *              a mapping data structure that will help access what you need
  */
  virtual const SEScalar* GetScalar(const char* name);
  virtual const SEScalar* GetScalar(const std::string& name);

  virtual const std::map<CDM::enumPatientEvent::value, bool>& GetEventStates() const { return m_EventState; }
  virtual void SetEvent(CDM::enumPatientEvent::value type, bool active, const SEScalarTime& time);
  virtual bool IsEventActive(CDM::enumPatientEvent::value state) const;
  virtual double GetEventDuration(CDM::enumPatientEvent::value type, const TimeUnit& unit) const;
  virtual void UpdateEvents(const SEScalarTime& timeStep);
  /** @name ForwardEvents
   *  @brief - Set a callback class to invoke when any event changes
   *  @details - Note that the handler callback can and will be called in the middle of a time step
   *             So system and compartment objects may not be completely up to date when called.
   *             Use the PhysiologyEngineInterface::SetEventHandler to ensure that all engine 
   *             data is up to date at the time the callback is invoked
   */
  virtual void ForwardEvents(SEEventHandler* handler) const;

  virtual std::string GetName() const;
  virtual const char* GetName_cStr() const;
  virtual void SetName(const char* name);
  virtual void SetName(const std::string& name);
  virtual bool HasName() const;
  virtual void InvalidateName();

  virtual std::string GetAnnotation() const;
  virtual const char* GetAnnotation_cStr() const;
  virtual void SetAnnotation(const char* Annotation);
  virtual void SetAnnotation(const std::string& Annotation);
  virtual bool HasAnnotation() const;
  virtual void InvalidateAnnotation();

  virtual CDM::enumSex::value GetGender() const;
  virtual void SetGender(CDM::enumSex::value sex);
  virtual bool HasGender() const;
  virtual void InvalidateGender();

  virtual CDM::enumSex::value GetSex() const;
  virtual void SetSex(CDM::enumSex::value sex);
  virtual bool HasSex() const;
  virtual void InvalidateSex();

  virtual bool HasAge() const;
  virtual SEScalarTime& GetAge();
  virtual double GetAge(const TimeUnit& unit) const;

  virtual bool HasWeight() const;
  virtual SEScalarMass& GetWeight();
  virtual double GetWeight(const MassUnit& unit) const;

  virtual bool HasHeight() const;
  virtual SEScalarLength& GetHeight();
  virtual double GetHeight(const LengthUnit& unit) const;

  virtual bool HasAlveoliSurfaceArea() const;
  virtual SEScalarArea& GetAlveoliSurfaceArea();
  virtual double GetAlveoliSurfaceArea(const AreaUnit& unit) const;

  virtual bool HasBasalMetabolicRate() const;
  virtual SEScalarPower& GetBasalMetabolicRate();
  virtual double GetBasalMetabolicRate(const PowerUnit& unit) const;

  virtual CDM::enumBloodType::value GetBloodType() const;
  virtual void SetBloodType(CDM::enumBloodType::value bloodAntigen);
  virtual bool HasBloodType() const;
  virtual void InvalidateBloodType();

  virtual bool GetBloodRh() const;
  virtual void SetBloodRh(bool bloodRh);
  virtual bool HasBloodRh() const;
  virtual void InvalidateBloodRh();

  virtual bool HasBloodVolumeBaseline() const;
  virtual SEScalarVolume& GetBloodVolumeBaseline();
  virtual double GetBloodVolumeBaseline(const VolumeUnit& unit) const;

  virtual bool HasBodyDensity() const;
  virtual SEScalarMassPerVolume& GetBodyDensity();
  virtual double GetBodyDensity(const MassPerVolumeUnit& unit) const;

  virtual bool HasBodyFatFraction() const;
  virtual SEScalarFraction& GetBodyFatFraction();
  virtual double GetBodyFatFraction() const;

  virtual bool HasDiastolicArterialPressureBaseline() const;
  virtual SEScalarPressure& GetDiastolicArterialPressureBaseline();
  virtual double GetDiastolicArterialPressureBaseline(const PressureUnit& unit) const;

  virtual bool HasExpiratoryReserveVolume() const;
  virtual SEScalarVolume& GetExpiratoryReserveVolume();
  virtual double GetExpiratoryReserveVolume(const VolumeUnit& unit) const;

  virtual bool HasFunctionalResidualCapacity() const;
  virtual SEScalarVolume& GetFunctionalResidualCapacity();
  virtual double GetFunctionalResidualCapacity(const VolumeUnit& unit) const;

  virtual bool HasHeartRateBaseline() const;
  virtual SEScalarFrequency& GetHeartRateBaseline();
  virtual double GetHeartRateBaseline(const FrequencyUnit& unit) const;

  virtual bool HasHeartRateMaximum() const;
  virtual SEScalarFrequency& GetHeartRateMaximum();
  virtual double GetHeartRateMaximum(const FrequencyUnit& unit) const;

  virtual bool HasHeartRateMinimum() const;
  virtual SEScalarFrequency& GetHeartRateMinimum();
  virtual double GetHeartRateMinimum(const FrequencyUnit& unit) const;

  virtual bool HasHyperhidrosis() const;
  virtual SEScalarNeg1To1& GetHyperhidrosis();
  virtual double GetHyperhidrosis() const;

  virtual bool HasInspiratoryCapacity() const;
  virtual SEScalarVolume& GetInspiratoryCapacity();
  virtual double GetInspiratoryCapacity(const VolumeUnit& unit) const;

  virtual bool HasInspiratoryReserveVolume() const;
  virtual SEScalarVolume& GetInspiratoryReserveVolume();
  virtual double GetInspiratoryReserveVolume(const VolumeUnit& unit) const;

  virtual bool HasLeanBodyMass() const;
  virtual SEScalarMass& GetLeanBodyMass();
  virtual double GetLeanBodyMass(const MassUnit& unit) const;

  virtual bool HasMaxWorkRate() const;
  virtual SEScalarPower& GetMaxWorkRate();
  virtual double GetMaxWorkRate(const PowerUnit& unit) const;

  virtual bool HasMuscleMass() const;
  virtual SEScalarMass& GetMuscleMass();
  virtual double GetMuscleMass(const MassUnit& unit) const;

  virtual bool HasMeanArterialPressureBaseline() const;
  virtual SEScalarPressure& GetMeanArterialPressureBaseline();
  virtual double GetMeanArterialPressureBaseline(const PressureUnit& unit) const;

  virtual bool HasPainSusceptibility() const;
  virtual SEScalarNeg1To1& GetPainSusceptibility();
  virtual double GetPainSusceptibility() const;

  virtual bool HasResidualVolume() const;
  virtual SEScalarVolume& GetResidualVolume();
  virtual double GetResidualVolume(const VolumeUnit& unit) const;

  virtual bool HasRespirationRateBaseline() const;
  virtual SEScalarFrequency& GetRespirationRateBaseline();
  virtual double GetRespirationRateBaseline(const FrequencyUnit& unit) const;

  virtual bool HasRespiratoryDriverAmplitudeBaseline() const;
  virtual SEScalarPressure& GetRespiratoryDriverAmplitudeBaseline();
  virtual double GetRespiratoryDriverAmplitudeBaseline(const PressureUnit& unit) const;

  virtual bool HasRightLungRatio() const;
  virtual SEScalarFraction& GetRightLungRatio();
  virtual double GetRightLungRatio() const;

  virtual bool HasSkinSurfaceArea() const;
  virtual SEScalarArea& GetSkinSurfaceArea();
  virtual double GetSkinSurfaceArea(const AreaUnit& unit) const;

  virtual bool HasSleepAmount() const;
  virtual SEScalarTime& GetSleepAmount();
  virtual double GetSleepAmount(const TimeUnit& unit) const;

  virtual bool HasSystolicArterialPressureBaseline() const;
  virtual SEScalarPressure& GetSystolicArterialPressureBaseline();
  virtual double GetSystolicArterialPressureBaseline(const PressureUnit& unit) const;

  virtual bool HasTotalVentilationBaseline() const;
  virtual SEScalarVolumePerTime& GetTotalVentilationBaseline();
  virtual double GetTotalVentilationBaseline(const VolumePerTimeUnit& unit) const;

  virtual bool HasTidalVolumeBaseline() const;
  virtual SEScalarVolume& GetTidalVolumeBaseline();
  virtual double GetTidalVolumeBaseline(const VolumeUnit& unit) const;

  virtual bool HasTotalLungCapacity() const;
  virtual SEScalarVolume& GetTotalLungCapacity();
  virtual double GetTotalLungCapacity(const VolumeUnit& unit) const;

  virtual bool HasVitalCapacity() const;
  virtual SEScalarVolume& GetVitalCapacity();
  virtual double GetVitalCapacity(const VolumeUnit& unit) const;

protected:
  virtual void Unload(CDM::PatientData& data) const;

protected:
  std::stringstream m_ss;
  mutable SEEventHandler* m_EventHandler;
  std::map<CDM::enumPatientEvent::value, bool> m_EventState;
  std::map<CDM::enumPatientEvent::value, double> m_EventDuration_s;

  std::string m_Name;
  std::string m_Annotation;
  CDM::enumSex::value m_Gender;
  SEScalarTime* m_Age;
  SEScalarMass* m_Weight;
  SEScalarLength* m_Height;
  SEScalarMassPerVolume* m_BodyDensity;
  SEScalarFraction* m_BodyFatFraction;
  SEScalarMass* m_LeanBodyMass;
  SEScalarPower* m_MaxWorkRate;
  SEScalarMass* m_MuscleMass;
  CDM::enumBloodType::value m_BloodType;
  bool m_BloodRh; // true meaning rh positive and false meaning rh negative

  SEScalarArea* m_AlveoliSurfaceArea;
  SEScalarFraction* m_RightLungRatio;
  SEScalarArea* m_SkinSurfaceArea;

  SEScalarPower* m_BasalMetabolicRate;
  SEScalarVolume* m_BloodVolumeBaseline;
  SEScalarPressure* m_DiastolicArterialPressureBaseline;
  SEScalarFrequency* m_HeartRateBaseline;
  SEScalarNeg1To1* m_Hyperhidrosis;
  SEScalarPressure* m_MeanArterialPressureBaseline;
  SEScalarNeg1To1* m_PainSusceptibility;
  SEScalarFrequency* m_RespirationRateBaseline;
  SEScalarPressure* m_RespiratoryDriverAmplitudeBaseline;
  SEScalarTime* m_SleepAmount;
  SEScalarPressure* m_SystolicArterialPressureBaseline;
  SEScalarVolumePerTime* m_TotalVentilationBaseline;
  SEScalarVolume* m_TidalVolumeBaseline;

  SEScalarFrequency* m_HeartRateMaximum;
  SEScalarFrequency* m_HeartRateMinimum;
  SEScalarVolume* m_ExpiratoryReserveVolume;
  SEScalarVolume* m_FunctionalResidualCapacity;
  SEScalarVolume* m_InspiratoryCapacity;
  SEScalarVolume* m_InspiratoryReserveVolume;
  SEScalarVolume* m_ResidualVolume;
  SEScalarVolume* m_TotalLungCapacity;
  SEScalarVolume* m_VitalCapacity;
};
}