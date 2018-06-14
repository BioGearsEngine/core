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
#include <biogears/schema/RespiratorySystemData.hxx>

class BIOGEARS_API SERespiratorySystem : public SESystem {
public:
  SERespiratorySystem(Logger* logger);
  virtual ~SERespiratorySystem();

  virtual void Clear(); // Deletes all members

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool Load(const CDM::RespiratorySystemData& in);
  virtual CDM::RespiratorySystemData* Unload() const;

protected:
  virtual void Unload(CDM::RespiratorySystemData& data) const;

public:
  virtual bool HasAlveolarArterialGradient() const;
  virtual SEScalarPressure& GetAlveolarArterialGradient();
  virtual double GetAlveolarArterialGradient(const PressureUnit& unit) const;

  virtual bool HasCarricoIndex() const;
  virtual SEScalarPressure& GetCarricoIndex();
  virtual double GetCarricoIndex(const PressureUnit& unit) const;

  virtual bool HasEndTidalCarbonDioxideFraction() const;
  virtual SEScalarFraction& GetEndTidalCarbonDioxideFraction();
  virtual double GetEndTidalCarbonDioxideFraction() const;

  virtual bool HasEndTidalCarbonDioxidePressure() const;
  virtual SEScalarPressure& GetEndTidalCarbonDioxidePressure();
  virtual double GetEndTidalCarbonDioxidePressure(const PressureUnit& unit) const;

  virtual bool HasExpiratoryFlow() const;
  virtual SEScalarVolumePerTime& GetExpiratoryFlow();
  virtual double GetExpiratoryFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasInspiratoryExpiratoryRatio() const;
  virtual SEScalar& GetInspiratoryExpiratoryRatio();
  virtual double GetInspiratoryExpiratoryRatio() const;

  virtual bool HasInspiratoryFlow() const;
  virtual SEScalarVolumePerTime& GetInspiratoryFlow();
  virtual double GetInspiratoryFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasPulmonaryCompliance() const;
  virtual SEScalarFlowCompliance& GetPulmonaryCompliance();
  virtual double GetPulmonaryCompliance(const FlowComplianceUnit& unit) const;

  virtual bool HasPulmonaryResistance() const;
  virtual SEScalarFlowResistance& GetPulmonaryResistance();
  virtual double GetPulmonaryResistance(const FlowResistanceUnit& unit) const;

  virtual bool HasRespirationDriverPressure() const;
  virtual SEScalarPressure& GetRespirationDriverPressure();
  virtual double GetRespirationDriverPressure(const PressureUnit& unit) const;

  virtual bool HasRespirationMusclePressure() const;
  virtual SEScalarPressure& GetRespirationMusclePressure();
  virtual double GetRespirationMusclePressure(const PressureUnit& unit) const;

  virtual bool HasRespirationRate() const;
  virtual SEScalarFrequency& GetRespirationRate();
  virtual double GetRespirationRate(const FrequencyUnit& unit) const;

  virtual bool HasSpecificVentilation() const;
  virtual SEScalar& GetSpecificVentilation();
  virtual double GetSpecificVentilation() const;

  virtual bool HasTidalVolume() const;
  virtual SEScalarVolume& GetTidalVolume();
  virtual double GetTidalVolume(const VolumeUnit& unit) const;

  virtual bool HasTotalAlveolarVentilation() const;
  virtual SEScalarVolumePerTime& GetTotalAlveolarVentilation();
  virtual double GetTotalAlveolarVentilation(const VolumePerTimeUnit& unit) const;

  virtual bool HasTotalDeadSpaceVentilation() const;
  virtual SEScalarVolumePerTime& GetTotalDeadSpaceVentilation();
  virtual double GetTotalDeadSpaceVentilation(const VolumePerTimeUnit& unit) const;

  virtual bool HasTotalPulmonaryVentilation() const;
  virtual SEScalarVolumePerTime& GetTotalPulmonaryVentilation();
  virtual double GetTotalPulmonaryVentilation(const VolumePerTimeUnit& unit) const;

  virtual bool HasTotalLungVolume() const;
  virtual SEScalarVolume& GetTotalLungVolume();
  virtual double GetTotalLungVolume(const VolumeUnit& unit) const;

  virtual bool HasTranspulmonaryPressure() const;
  virtual SEScalarPressure& GetTranspulmonaryPressure();
  virtual double GetTranspulmonaryPressure(const PressureUnit& unit) const;

protected:
  SEScalarPressure* m_AlveolarArterialGradient;
  SEScalarPressure* m_CarricoIndex;
  SEScalarFraction* m_EndTidalCarbonDioxideFraction;
  SEScalarPressure* m_EndTidalCarbonDioxidePressure;
  SEScalarVolumePerTime* m_ExpiratoryFlow;
  SEScalar* m_InspiratoryExpiratoryRatio;
  SEScalarVolumePerTime* m_InspiratoryFlow;
  SEScalarFlowCompliance* m_PulmonaryCompliance;
  SEScalarFlowResistance* m_PulmonaryResistance;
  SEScalarPressure* m_RespirationDriverPressure;
  SEScalarPressure* m_RespirationMusclePressure;
  SEScalarFrequency* m_RespirationRate;
  SEScalar* m_SpecificVentilation;
  SEScalarVolume* m_TidalVolume;
  SEScalarVolumePerTime* m_TotalAlveolarVentilation;
  SEScalarVolumePerTime* m_TotalDeadSpaceVentilation;
  SEScalarVolume* m_TotalLungVolume;
  SEScalarVolumePerTime* m_TotalPulmonaryVentilation;
  SEScalarPressure* m_TranspulmonaryPressure;
};