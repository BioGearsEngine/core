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

#include <biogears/schema/cdm/Environment.hxx>
#include <biogears/cdm/enums/SEEnvironmentEnums.h>


#include <random>

CDM_BIND_DECL(EnvironmentalConditionsData)

namespace biogears {
class SESubstance;
class SESubstanceFraction;
class SESubstanceConcentration;
class SEEnvironment;
class SESubstanceManager;
class SEEnvironmentChange;
class SEInitialEnvironment;

class SEScalar;
class SEScalarMassPerVolume;
class MassPerVolumeUnit;
class SEScalarLengthPerTime;
class LengthPerTimeUnit;
class SEScalarTemperature;
class TemperatureUnit;
class SEScalarHeatResistanceArea;
class HeatResistanceAreaUnit;
class SEScalarPressure;
class PressureUnit;
class SEScalarFraction;

namespace io {
  class Environment;
}

class BIOGEARS_API SEEnvironmentalConditions : public Loggable {
protected:
  friend SEEnvironment;
  friend SEEnvironmentChange;
  friend SEInitialEnvironment;
  friend io::Environment;

public:
  SEEnvironmentalConditions(SESubstanceManager& substances);
  virtual ~SEEnvironmentalConditions();

  void Clear();
  bool IsValid();

  bool operator==(SEEnvironmentalConditions const& rhs) const;
  bool operator!=(SEEnvironmentalConditions const& rhs) const;

  bool Load(const char* environmentFile);
  bool Load(const std::string& environmentFile);

  const SEScalar* GetScalar(const char* name);
  const SEScalar* GetScalar(const std::string& name);

  std::string GetName() const;
  const char* GetName_cStr() const;
  void SetName(const char* name);
  void SetName(const std::string& name);
  bool HasName() const;
  void InvalidateName();

  SESurroundingType GetSurroundingType() const;
  void SetSurroundingType(SESurroundingType name);
  bool HasSurroundingType() const;
  void InvalidateSurroundingType();

  bool HasAirDensity() const;
  SEScalarMassPerVolume& GetAirDensity();
  double GetAirDensity(const MassPerVolumeUnit& unit) const;

  bool HasAirVelocity() const;
  SEScalarLengthPerTime& GetAirVelocity();
  double GetAirVelocity(const LengthPerTimeUnit& unit) const;

  bool HasAmbientTemperature() const;
  SEScalarTemperature& GetAmbientTemperature();
  double GetAmbientTemperature(const TemperatureUnit& unit) const;

  bool HasAtmosphericPressure() const;
  SEScalarPressure& GetAtmosphericPressure();
  double GetAtmosphericPressure(const PressureUnit& unit) const;

  bool HasClothingResistance() const;
  SEScalarHeatResistanceArea& GetClothingResistance();
  double GetClothingResistance(const HeatResistanceAreaUnit& unit) const;

  bool HasEmissivity() const;
  SEScalarFraction& GetEmissivity();
  double GetEmissivity() const;

  bool HasMeanRadiantTemperature() const;
  SEScalarTemperature& GetMeanRadiantTemperature();
  double GetMeanRadiantTemperature(const TemperatureUnit& unit) const;

  bool HasRelativeHumidity() const;
  SEScalarFraction& GetRelativeHumidity();
  double GetRelativeHumidity() const;

  bool HasRespirationAmbientTemperature() const;
  SEScalarTemperature& GetRespirationAmbientTemperature();
  double GetRespirationAmbientTemperature(const TemperatureUnit& unit) const;

  bool HasAmbientGas() const;
  bool HasAmbientGas(const SESubstance& substance) const;
  const std::vector<SESubstanceFraction*>& GetAmbientGases();
  const std::vector<const SESubstanceFraction*>& GetAmbientGases() const;
  SESubstanceFraction& GetAmbientGas(SESubstance& substance);
  const SESubstanceFraction* GetAmbientGas(const SESubstance& substance) const;
  void AddAmbientGas(const SESubstance& substance, SEScalarFraction const& concentration);
  void RemoveAmbientGas(const SESubstance& substance);
  void RemoveAmbientGases();

  bool HasAmbientAerosol() const;
  bool HasAmbientAerosol(const SESubstance& substance) const;
  const std::vector<SESubstanceConcentration*>& GetAmbientAerosols();
  const std::vector<const SESubstanceConcentration*>& GetAmbientAerosols() const;
  SESubstanceConcentration& GetAmbientAerosol(SESubstance& substance);
  const SESubstanceConcentration* GetAmbientAerosol(const SESubstance& substance) const;
  void AddAmbientAerosol(const SESubstance& substance, SEScalarMassPerVolume const& concentration);
  void RemoveAmbientAerosol(const SESubstance& substance);
  void RemoveAmbientAerosols();

protected:
  void Merge(const SEEnvironmentalConditions& from);

protected:
  SESurroundingType m_SurroundingType;

  std::string m_Name;
  SEScalarMassPerVolume* m_AirDensity;
  SEScalarLengthPerTime* m_AirVelocity;
  SEScalarTemperature* m_AmbientTemperature;
  SEScalarPressure* m_AtmosphericPressure;
  SEScalarHeatResistanceArea* m_ClothingResistance;
  SEScalarFraction* m_Emissivity;
  SEScalarTemperature* m_MeanRadiantTemperature;
  SEScalarFraction* m_RelativeHumidity;
  SEScalarTemperature* m_RespirationAmbientTemperature;

  std::vector<SESubstanceFraction*> m_AmbientGases;
  std::vector<const SESubstanceFraction*> m_cAmbientGases;

  std::vector<SESubstanceConcentration*> m_AmbientAerosols;
  std::vector<const SESubstanceConcentration*> m_cAmbientAerosols;

  SESubstanceManager& m_Substances;
};
}