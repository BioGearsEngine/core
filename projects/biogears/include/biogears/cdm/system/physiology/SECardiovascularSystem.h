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
#include <biogears/cdm/system/SESystem.h>
#include <biogears/schema/cdm/Physiology.hxx>

class SEScalarPressure;
class PressureUnit;
class SEScalarVolume;
class VolumeUnit;
class SEScalarVolumePerTimeArea;
class VolumePerTimeAreaUnit;
class SEScalarVolumePerTime;
class VolumePerTimeUnit;
class SEScalarFraction;
class FractionUnit;
class SEScalarFrequency;
class FrequencyUnit;
class SEScalarFlowResistance;
class FlowResistanceUnit;
class SEScalarPressureTimePerVolumeArea;
class PressureTimePerVolumeAreaUnit;

class BIOGEARS_API SECardiovascularSystem : public SESystem {
public:
  SECardiovascularSystem(Logger* logger);
  virtual ~SECardiovascularSystem();

  virtual void Clear(); // Deletes all members

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool Load(const CDM::CardiovascularSystemData& in);
  virtual CDM::CardiovascularSystemData* Unload() const;

protected:
  virtual void Unload(CDM::CardiovascularSystemData& data) const;

public:
  virtual bool HasArterialPressure() const;
  virtual SEScalarPressure& GetArterialPressure();
  virtual double GetArterialPressure(const PressureUnit& unit) const;

  virtual bool HasBloodVolume() const;
  virtual SEScalarVolume& GetBloodVolume();
  virtual double GetBloodVolume(const VolumeUnit& unit) const;

  virtual bool HasCardiacIndex() const;
  virtual SEScalarVolumePerTimeArea& GetCardiacIndex();
  virtual double GetCardiacIndex(const VolumePerTimeAreaUnit& unit) const;

  virtual bool HasCardiacOutput() const;
  virtual SEScalarVolumePerTime& GetCardiacOutput();
  virtual double GetCardiacOutput(const VolumePerTimeUnit& unit) const;

  virtual bool HasCentralVenousPressure() const;
  virtual SEScalarPressure& GetCentralVenousPressure();
  virtual double GetCentralVenousPressure(const PressureUnit& unit) const;

  virtual bool HasCerebralBloodFlow() const;
  virtual SEScalarVolumePerTime& GetCerebralBloodFlow();
  virtual double GetCerebralBloodFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasCerebralPerfusionPressure() const;
  virtual SEScalarPressure& GetCerebralPerfusionPressure();
  virtual double GetCerebralPerfusionPressure(const PressureUnit& unit) const;

  virtual bool HasDiastolicArterialPressure() const;
  virtual SEScalarPressure& GetDiastolicArterialPressure();
  virtual double GetDiastolicArterialPressure(const PressureUnit& unit) const;

  virtual bool HasHeartEjectionFraction() const;
  virtual SEScalarFraction& GetHeartEjectionFraction();
  virtual double GetHeartEjectionFraction() const;

  virtual bool HasHeartRate() const;
  virtual SEScalarFrequency& GetHeartRate();
  virtual double GetHeartRate(const FrequencyUnit& unit) const;

  virtual CDM::enumHeartRhythm::value GetHeartRhythm() const;
  virtual void SetHeartRhythm(CDM::enumHeartRhythm::value Rhythm);
  virtual bool HasHeartRhythm() const;
  virtual void InvalidateHeartRhythm();

  virtual bool HasHeartStrokeVolume() const;
  virtual SEScalarVolume& GetHeartStrokeVolume();
  virtual double GetHeartStrokeVolume(const VolumeUnit& unit) const;

  virtual bool HasIntracranialPressure() const;
  virtual SEScalarPressure& GetIntracranialPressure();
  virtual double GetIntracranialPressure(const PressureUnit& unit) const;

  virtual bool HasMeanArterialPressure() const;
  virtual SEScalarPressure& GetMeanArterialPressure();
  virtual double GetMeanArterialPressure(const PressureUnit& unit) const;

  virtual bool HasMeanArterialCarbonDioxidePartialPressure() const;
  virtual SEScalarPressure& GetMeanArterialCarbonDioxidePartialPressure();
  virtual double GetMeanArterialCarbonDioxidePartialPressure(const PressureUnit& unit) const;

  virtual bool HasMeanArterialCarbonDioxidePartialPressureDelta() const;
  virtual SEScalarPressure& GetMeanArterialCarbonDioxidePartialPressureDelta();
  virtual double GetMeanArterialCarbonDioxidePartialPressureDelta(const PressureUnit& unit) const;

  virtual bool HasMeanCentralVenousPressure() const;
  virtual SEScalarPressure& GetMeanCentralVenousPressure();
  virtual double GetMeanCentralVenousPressure(const PressureUnit& unit) const;

  virtual bool HasMeanSkinFlow() const;
  virtual SEScalarVolumePerTime& GetMeanSkinFlow();
  virtual double GetMeanSkinFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasPulmonaryArterialPressure() const;
  virtual SEScalarPressure& GetPulmonaryArterialPressure();
  virtual double GetPulmonaryArterialPressure(const PressureUnit& unit) const;

  virtual bool HasPulmonaryCapillariesWedgePressure() const;
  virtual SEScalarPressure& GetPulmonaryCapillariesWedgePressure();
  virtual double GetPulmonaryCapillariesWedgePressure(const PressureUnit& unit) const;

  virtual bool HasPulmonaryDiastolicArterialPressure() const;
  virtual SEScalarPressure& GetPulmonaryDiastolicArterialPressure();
  virtual double GetPulmonaryDiastolicArterialPressure(const PressureUnit& unit) const;

  virtual bool HasPulmonaryMeanArterialPressure() const;
  virtual SEScalarPressure& GetPulmonaryMeanArterialPressure();
  virtual double GetPulmonaryMeanArterialPressure(const PressureUnit& unit) const;

  virtual bool HasPulmonaryMeanCapillaryFlow() const;
  virtual SEScalarVolumePerTime& GetPulmonaryMeanCapillaryFlow();
  virtual double GetPulmonaryMeanCapillaryFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasPulmonaryMeanShuntFlow() const;
  virtual SEScalarVolumePerTime& GetPulmonaryMeanShuntFlow();
  virtual double GetPulmonaryMeanShuntFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasPulmonarySystolicArterialPressure() const;
  virtual SEScalarPressure& GetPulmonarySystolicArterialPressure();
  virtual double GetPulmonarySystolicArterialPressure(const PressureUnit& unit) const;

  virtual bool HasPulmonaryVascularResistance() const;
  virtual SEScalarFlowResistance& GetPulmonaryVascularResistance();
  virtual double GetPulmonaryVascularResistance(const FlowResistanceUnit& unit) const;

  virtual bool HasPulmonaryVascularResistanceIndex() const;
  virtual SEScalarPressureTimePerVolumeArea& GetPulmonaryVascularResistanceIndex();
  virtual double GetPulmonaryVascularResistanceIndex(const PressureTimePerVolumeAreaUnit& unit) const;

  virtual bool HasPulsePressure() const;
  virtual SEScalarPressure& GetPulsePressure();
  virtual double GetPulsePressure(const PressureUnit& unit) const;

  virtual bool HasSystolicArterialPressure() const;
  virtual SEScalarPressure& GetSystolicArterialPressure();
  virtual double GetSystolicArterialPressure(const PressureUnit& unit) const;

  virtual bool HasSystemicVascularResistance() const;
  virtual SEScalarFlowResistance& GetSystemicVascularResistance();
  virtual double GetSystemicVascularResistance(const FlowResistanceUnit& unit) const;

protected:
  SEScalarPressure* m_ArterialPressure;
  SEScalarVolume* m_BloodVolume;
  SEScalarVolumePerTimeArea* m_CardiacIndex;
  SEScalarVolumePerTime* m_CardiacOutput;
  SEScalarPressure* m_CentralVenousPressure;
  SEScalarVolumePerTime* m_CerebralBloodFlow;
  SEScalarPressure* m_CerebralPerfusionPressure;
  SEScalarPressure* m_DiastolicArterialPressure;
  SEScalarFraction* m_HeartEjectionFraction;
  SEScalarFrequency* m_HeartRate;
  CDM::enumHeartRhythm::value m_HeartRhythm;
  SEScalarVolume* m_HeartStrokeVolume;
  SEScalarPressure* m_IntracranialPressure;
  SEScalarPressure* m_MeanArterialPressure;
  SEScalarPressure* m_MeanArterialCarbonDioxidePartialPressure;
  SEScalarPressure* m_MeanArterialCarbonDioxidePartialPressureDelta;
  SEScalarPressure* m_MeanCentralVenousPressure;
  SEScalarVolumePerTime* m_MeanSkinFlow;
  SEScalarPressure* m_PulmonaryArterialPressure;
  SEScalarPressure* m_PulmonaryCapillariesWedgePressure;
  SEScalarPressure* m_PulmonaryDiastolicArterialPressure;
  SEScalarPressure* m_PulmonaryMeanArterialPressure;
  SEScalarVolumePerTime* m_PulmonaryMeanCapillaryFlow;
  SEScalarVolumePerTime* m_PulmonaryMeanShuntFlow;
  SEScalarPressure* m_PulmonarySystolicArterialPressure;
  SEScalarFlowResistance* m_PulmonaryVascularResistance;
  SEScalarPressureTimePerVolumeArea* m_PulmonaryVascularResistanceIndex;
  SEScalarPressure* m_PulsePressure;
  SEScalarPressure* m_SystolicArterialPressure;
  SEScalarFlowResistance* m_SystemicVascularResistance;
};