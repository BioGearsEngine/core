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
#include <biogears/exports.h>

#include <biogears/cdm/patient/actions/SEPupillaryResponse.h>
#include <biogears/cdm/system/SESystem.h>
#include <biogears/schema/biogears/BioGearsPhysiology.hxx>

namespace biogears {
class SEScalar;
class SEScalarFraction;
class SEScalarMass;
class MassUnit;
class SEScalarTemperature;
class TemperatureUnit;
class SEScalarPressure;
class PressureUnit;
class SEScalarFrequency;
class FrequencyUnit;
class SEScalarVolume;
class VolumeUnit;
class SEScalarVolumePerTime;

class BIOGEARS_API SEDrugSystem : public SESystem {
public:
  SEDrugSystem(Logger* logger);
  ~SEDrugSystem() override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); }
  static constexpr char const * const  TypeTag() { return "SEDrugSystem"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override; // Deletes all members

  const SEScalar* GetScalar(const char* name) override;
  const SEScalar* GetScalar(const std::string& name) override;

  bool Load(const CDM::DrugSystemData& in);
  CDM::DrugSystemData* Unload() const override;

  Tree<const char*> GetPhysiologyRequestGraph() const override;
protected:
  void Unload(CDM::DrugSystemData& data) const;

public:
  bool HasAntibioticActivity() const;
  SEScalar& GetAntibioticActivity();
  double GetAntibioticActivity() const;

  bool HasBronchodilationLevel() const;
  SEScalarFraction& GetBronchodilationLevel();
  double GetBronchodilationLevel() const;

  bool HasFeverChange() const;
  SEScalarTemperature& GetFeverChange();
  double GetFeverChange(const TemperatureUnit& unit) const;

  bool HasHeartRateChange() const;
  SEScalarFrequency& GetHeartRateChange();
  double GetHeartRateChange(const FrequencyUnit& unit) const;

  bool HasHemorrhageChange() const;
  SEScalarFraction& GetHemorrhageChange();
  double GetHemorrhageChange() const;

  bool HasMeanBloodPressureChange() const;
  SEScalarPressure& GetMeanBloodPressureChange();
  double GetMeanBloodPressureChange(const PressureUnit& unit) const;

  bool HasNeuromuscularBlockLevel() const;
  SEScalarFraction& GetNeuromuscularBlockLevel();
  double GetNeuromuscularBlockLevel() const;

  bool HasPainToleranceChange() const;
  SEScalarFraction& GetPainToleranceChange();
  double GetPainToleranceChange() const;

  bool HasPulsePressureChange() const;
  SEScalarPressure& GetPulsePressureChange();
  double GetPulsePressureChange(const PressureUnit& unit) const;

  bool HasRespirationRateChange() const;
  SEScalarFrequency& GetRespirationRateChange();
  double GetRespirationRateChange(const FrequencyUnit& unit) const;

  bool HasSedationLevel() const;
  SEScalarFraction& GetSedationLevel();
  double GetSedationLevel() const;

  bool HasTidalVolumeChange() const;
  SEScalarVolume& GetTidalVolumeChange();
  double GetTidalVolumeChange(const VolumeUnit& unit) const;

  bool HasTubularPermeabilityChange() const;
  SEScalarFraction& GetTubularPermeabilityChange();
  double GetTubularPermeabilityChange() const;

  bool HasCentralNervousResponse() const;
  SEScalarFraction& GetCentralNervousResponse();
  double GetCentralNervousResponse() const;

protected:
  SEScalar* m_AntibioticActivity;
  SEScalarFraction* m_BronchodilationLevel;
  SEScalarTemperature* m_FeverChange;
  SEScalarFrequency* m_HeartRateChange;
  SEScalarFraction* m_HemorrhageChange;
  SEScalarPressure* m_MeanBloodPressureChange;
  SEScalarFraction* m_NeuromuscularBlockLevel;
  SEScalarFraction* m_PainToleranceChange;
  SEScalarPressure* m_PulsePressureChange;
  SEScalarFrequency* m_RespirationRateChange;
  SEScalarFraction* m_SedationLevel;
  SEScalarVolume* m_TidalVolumeChange;
  SEScalarFraction* m_TubularPermeabilityChange;
  SEScalarFraction* m_CentralNervousResponse;
};
}
