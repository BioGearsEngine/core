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
#include <biogears/schema/biogears/BioGearsPhysiology.hxx>

namespace biogears {
class SEScalarPressure;
class PressureUnit;
class SEScalarFraction;
class SEScalarVolumePerTime;
class VolumePerTimeUnit;
class SEScalarFlowCompliance;
class FlowComplianceUnit;
class SEScalarFlowResistance;
class FlowResistanceUnit;
class SEScalarFrequency;
class FrequencyUnit;
class SEScalarVolume;
class VolumeUnit;
namespace io {
  class Physiology;
}
class BIOGEARS_API SERespiratorySystem : public SESystem {
  friend io::Physiology;

public:
  SERespiratorySystem(Logger* logger);
  ~SERespiratorySystem() override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); }
  static constexpr char const* const TypeTag() { return "SERespiratorySystem"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override; // Deletes all members

  const SEScalar* GetScalar(const char* name) override;
  const SEScalar* GetScalar(const std::string& name) override;

  bool Load(const CDM::RespiratorySystemData& in);
  CDM::RespiratorySystemData* Unload() const override;

  Tree<const char*> GetPhysiologyRequestGraph() const override;

protected:
  void Unload(CDM::RespiratorySystemData& data) const;

public:
  bool HasAlveolarArterialGradient() const;
  SEScalarPressure& GetAlveolarArterialGradient();
  double GetAlveolarArterialGradient(const PressureUnit& unit) const;

  bool HasCarricoIndex() const;
  SEScalarPressure& GetCarricoIndex();
  double GetCarricoIndex(const PressureUnit& unit) const;

  bool HasEndTidalCarbonDioxideFraction() const;
  SEScalarFraction& GetEndTidalCarbonDioxideFraction();
  double GetEndTidalCarbonDioxideFraction() const;

  bool HasEndTidalCarbonDioxidePressure() const;
  SEScalarPressure& GetEndTidalCarbonDioxidePressure();
  double GetEndTidalCarbonDioxidePressure(const PressureUnit& unit) const;

  bool HasExpiratoryFlow() const;
  SEScalarVolumePerTime& GetExpiratoryFlow();
  double GetExpiratoryFlow(const VolumePerTimeUnit& unit) const;

  bool HasInspiratoryExpiratoryRatio() const;
  SEScalar& GetInspiratoryExpiratoryRatio();
  double GetInspiratoryExpiratoryRatio() const;

  bool HasInspiratoryFlow() const;
  SEScalarVolumePerTime& GetInspiratoryFlow();
  double GetInspiratoryFlow(const VolumePerTimeUnit& unit) const;

  bool HasMeanPleuralPressure() const;
  SEScalarPressure& GetMeanPleuralPressure();
  double GetMeanPleuralPressure(const PressureUnit& unit) const;

  bool HasPulmonaryCompliance() const;
  SEScalarFlowCompliance& GetPulmonaryCompliance();
  double GetPulmonaryCompliance(const FlowComplianceUnit& unit) const;

  bool HasPulmonaryResistance() const;
  SEScalarFlowResistance& GetPulmonaryResistance();
  double GetPulmonaryResistance(const FlowResistanceUnit& unit) const;

  bool HasRespirationDriverFrequency() const;
  SEScalarFrequency& GetRespirationDriverFrequency();
  double GetRespirationDriverFrequency(const FrequencyUnit& unit) const;

  bool HasRespirationDriverPressure() const;
  SEScalarPressure& GetRespirationDriverPressure();
  double GetRespirationDriverPressure(const PressureUnit& unit) const;

  bool HasRespirationMusclePressure() const;
  SEScalarPressure& GetRespirationMusclePressure();
  double GetRespirationMusclePressure(const PressureUnit& unit) const;

  bool HasRespirationRate() const;
  SEScalarFrequency& GetRespirationRate();
  double GetRespirationRate(const FrequencyUnit& unit) const;

  bool HasSpecificVentilation() const;
  SEScalar& GetSpecificVentilation();
  double GetSpecificVentilation() const;

  bool HasTargetPulmonaryVentilation() const;
  SEScalarVolumePerTime& GetTargetPulmonaryVentilation();
  double GetTargetPulmonaryVentilation(const VolumePerTimeUnit& unit) const;

  bool HasTidalVolume() const;
  SEScalarVolume& GetTidalVolume();
  double GetTidalVolume(const VolumeUnit& unit) const;

  bool HasTotalAlveolarVentilation() const;
  SEScalarVolumePerTime& GetTotalAlveolarVentilation();
  double GetTotalAlveolarVentilation(const VolumePerTimeUnit& unit) const;

  bool HasTotalDeadSpaceVentilation() const;
  SEScalarVolumePerTime& GetTotalDeadSpaceVentilation();
  double GetTotalDeadSpaceVentilation(const VolumePerTimeUnit& unit) const;

  bool HasTotalPulmonaryVentilation() const;
  SEScalarVolumePerTime& GetTotalPulmonaryVentilation();
  double GetTotalPulmonaryVentilation(const VolumePerTimeUnit& unit) const;

  bool HasTotalLungVolume() const;
  SEScalarVolume& GetTotalLungVolume();
  double GetTotalLungVolume(const VolumeUnit& unit) const;

  bool HasTranspulmonaryPressure() const;
  SEScalarPressure& GetTranspulmonaryPressure();
  double GetTranspulmonaryPressure(const PressureUnit& unit) const;

protected:
  SEScalarPressure* m_AlveolarArterialGradient;
  SEScalarPressure* m_CarricoIndex;
  SEScalarFraction* m_EndTidalCarbonDioxideFraction;
  SEScalarPressure* m_EndTidalCarbonDioxidePressure;
  SEScalarVolumePerTime* m_ExpiratoryFlow;
  SEScalar* m_InspiratoryExpiratoryRatio;
  SEScalarVolumePerTime* m_InspiratoryFlow;
  SEScalarPressure* m_MeanPleuralPressure;
  SEScalarFlowCompliance* m_PulmonaryCompliance;
  SEScalarFlowResistance* m_PulmonaryResistance;
  SEScalarFrequency* m_RespirationDriverFrequency;
  SEScalarPressure* m_RespirationDriverPressure;
  SEScalarPressure* m_RespirationMusclePressure;
  SEScalarFrequency* m_RespirationRate;
  SEScalar* m_SpecificVentilation;
  SEScalarVolumePerTime* m_TargetPulmonaryVentilation;
  SEScalarVolume* m_TidalVolume;
  SEScalarVolumePerTime* m_TotalAlveolarVentilation;
  SEScalarVolumePerTime* m_TotalDeadSpaceVentilation;
  SEScalarVolume* m_TotalLungVolume;
  SEScalarVolumePerTime* m_TotalPulmonaryVentilation;
  SEScalarPressure* m_TranspulmonaryPressure;
};
}