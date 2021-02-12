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

namespace biogears {
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
namespace io {
  class Physiology;
}
class BIOGEARS_API SECardiovascularSystem : public SESystem {
  friend io::Physiology;
public:
  SECardiovascularSystem(Logger* logger);
  ~SECardiovascularSystem() override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); }
  static constexpr char const* const TypeTag() { return "SECardiovascularSystem"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override; //! Deletes all members

  const SEScalar* GetScalar(const char* name) override;
  const SEScalar* GetScalar(const std::string& name) override;

  bool Load(const CDM::CardiovascularSystemData& in);
  CDM::CardiovascularSystemData* Unload() const override;

  Tree<const char*> GetPhysiologyRequestGraph() const override;

protected:
  void Unload(CDM::CardiovascularSystemData& data) const;

public:
  bool HasArterialPressure() const;
  SEScalarPressure& GetArterialPressure();
  double GetArterialPressure(const PressureUnit& unit) const;

  bool HasBloodVolume() const;
  SEScalarVolume& GetBloodVolume();
  double GetBloodVolume(const VolumeUnit& unit) const;

  bool HasCardiacIndex() const;
  SEScalarVolumePerTimeArea& GetCardiacIndex();
  double GetCardiacIndex(const VolumePerTimeAreaUnit& unit) const;

  bool HasCardiacOutput() const;
  SEScalarVolumePerTime& GetCardiacOutput();
  double GetCardiacOutput(const VolumePerTimeUnit& unit) const;

  bool HasCentralVenousPressure() const;
  SEScalarPressure& GetCentralVenousPressure();
  double GetCentralVenousPressure(const PressureUnit& unit) const;

  bool HasCerebralBloodFlow() const;
  SEScalarVolumePerTime& GetCerebralBloodFlow();
  double GetCerebralBloodFlow(const VolumePerTimeUnit& unit) const;

  bool HasCerebralPerfusionPressure() const;
  SEScalarPressure& GetCerebralPerfusionPressure();
  double GetCerebralPerfusionPressure(const PressureUnit& unit) const;

  bool HasDiastolicArterialPressure() const;
  SEScalarPressure& GetDiastolicArterialPressure();
  double GetDiastolicArterialPressure(const PressureUnit& unit) const;

  bool HasHeartEjectionFraction() const;
  SEScalarFraction& GetHeartEjectionFraction();
  double GetHeartEjectionFraction() const;

  bool HasHeartRate() const;
  SEScalarFrequency& GetHeartRate();
  double GetHeartRate(const FrequencyUnit& unit) const;

  CDM::enumHeartRhythm::value GetHeartRhythm() const;
  void SetHeartRhythm(CDM::enumHeartRhythm::value Rhythm);
  bool HasHeartRhythm() const;
  void InvalidateHeartRhythm();

  bool HasHeartStrokeVolume() const;
  SEScalarVolume& GetHeartStrokeVolume();
  double GetHeartStrokeVolume(const VolumeUnit& unit) const;

  bool HasIntracranialPressure() const;
  SEScalarPressure& GetIntracranialPressure();
  double GetIntracranialPressure(const PressureUnit& unit) const;

  bool HasMeanArterialPressure() const;
  SEScalarPressure& GetMeanArterialPressure();
  double GetMeanArterialPressure(const PressureUnit& unit) const;

  bool HasMeanArterialCarbonDioxidePartialPressure() const;
  SEScalarPressure& GetMeanArterialCarbonDioxidePartialPressure();
  double GetMeanArterialCarbonDioxidePartialPressure(const PressureUnit& unit) const;

  bool HasMeanArterialCarbonDioxidePartialPressureDelta() const;
  SEScalarPressure& GetMeanArterialCarbonDioxidePartialPressureDelta();
  double GetMeanArterialCarbonDioxidePartialPressureDelta(const PressureUnit& unit) const;

  bool HasMeanCentralVenousPressure() const;
  SEScalarPressure& GetMeanCentralVenousPressure();
  double GetMeanCentralVenousPressure(const PressureUnit& unit) const;

  bool HasMeanSkinFlow() const;
  SEScalarVolumePerTime& GetMeanSkinFlow();
  double GetMeanSkinFlow(const VolumePerTimeUnit& unit) const;

  bool HasPulmonaryArterialPressure() const;
  SEScalarPressure& GetPulmonaryArterialPressure();
  double GetPulmonaryArterialPressure(const PressureUnit& unit) const;

  bool HasPulmonaryCapillariesWedgePressure() const;
  SEScalarPressure& GetPulmonaryCapillariesWedgePressure();
  double GetPulmonaryCapillariesWedgePressure(const PressureUnit& unit) const;

  bool HasPulmonaryDiastolicArterialPressure() const;
  SEScalarPressure& GetPulmonaryDiastolicArterialPressure();
  double GetPulmonaryDiastolicArterialPressure(const PressureUnit& unit) const;

  bool HasPulmonaryMeanArterialPressure() const;
  SEScalarPressure& GetPulmonaryMeanArterialPressure();
  double GetPulmonaryMeanArterialPressure(const PressureUnit& unit) const;

  bool HasPulmonaryMeanCapillaryFlow() const;
  SEScalarVolumePerTime& GetPulmonaryMeanCapillaryFlow();
  double GetPulmonaryMeanCapillaryFlow(const VolumePerTimeUnit& unit) const;

  bool HasPulmonaryMeanShuntFlow() const;
  SEScalarVolumePerTime& GetPulmonaryMeanShuntFlow();
  double GetPulmonaryMeanShuntFlow(const VolumePerTimeUnit& unit) const;

  bool HasPulmonarySystolicArterialPressure() const;
  SEScalarPressure& GetPulmonarySystolicArterialPressure();
  double GetPulmonarySystolicArterialPressure(const PressureUnit& unit) const;

  bool HasPulmonaryVascularResistance() const;
  SEScalarFlowResistance& GetPulmonaryVascularResistance();
  double GetPulmonaryVascularResistance(const FlowResistanceUnit& unit) const;

  bool HasPulmonaryVascularResistanceIndex() const;
  SEScalarPressureTimePerVolumeArea& GetPulmonaryVascularResistanceIndex();
  double GetPulmonaryVascularResistanceIndex(const PressureTimePerVolumeAreaUnit& unit) const;

  bool HasPulsePressure() const;
  SEScalarPressure& GetPulsePressure();
  double GetPulsePressure(const PressureUnit& unit) const;

  bool HasSystolicArterialPressure() const;
  SEScalarPressure& GetSystolicArterialPressure();
  double GetSystolicArterialPressure(const PressureUnit& unit) const;

  bool HasSystemicVascularResistance() const;
  SEScalarFlowResistance& GetSystemicVascularResistance();
  double GetSystemicVascularResistance(const FlowResistanceUnit& unit) const;

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
}