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

  virtual void Clear();

  virtual bool Load(const CDM::EnvironmentalConditionsData& in);
  virtual bool Load(const char* environmentFile);
  virtual bool Load(const std::string& environmentFile);
  virtual CDM::EnvironmentalConditionsData* Unload() const;

  virtual const SEScalar* GetScalar(const char* name);
  virtual const SEScalar* GetScalar(const std::string& name);

  virtual std::string GetName() const;
  virtual const char* GetName_cStr() const;
  virtual void SetName(const char* name);
  virtual void SetName(const std::string& name);
  virtual bool HasName() const;
  virtual void InvalidateName();

  virtual CDM::enumSurroundingType::value GetSurroundingType() const;
  virtual void SetSurroundingType(CDM::enumSurroundingType::value name);
  virtual bool HasSurroundingType() const;
  virtual void InvalidateSurroundingType();

  virtual bool HasAirDensity() const;
  virtual SEScalarMassPerVolume& GetAirDensity();
  virtual double GetAirDensity(const MassPerVolumeUnit& unit) const;

  virtual bool HasAirVelocity() const;
  virtual SEScalarLengthPerTime& GetAirVelocity();
  virtual double GetAirVelocity(const LengthPerTimeUnit& unit) const;

  virtual bool HasAmbientTemperature() const;
  virtual SEScalarTemperature& GetAmbientTemperature();
  virtual double GetAmbientTemperature(const TemperatureUnit& unit) const;

  virtual bool HasAtmosphericPressure() const;
  virtual SEScalarPressure& GetAtmosphericPressure();
  virtual double GetAtmosphericPressure(const PressureUnit& unit) const;

  virtual bool HasClothingResistance() const;
  virtual SEScalarHeatResistanceArea& GetClothingResistance();
  virtual double GetClothingResistance(const HeatResistanceAreaUnit& unit) const;

  virtual bool HasEmissivity() const;
  virtual SEScalarFraction& GetEmissivity();
  virtual double GetEmissivity() const;

  virtual bool HasMeanRadiantTemperature() const;
  virtual SEScalarTemperature& GetMeanRadiantTemperature();
  virtual double GetMeanRadiantTemperature(const TemperatureUnit& unit) const;

  virtual bool HasRelativeHumidity() const;
  virtual SEScalarFraction& GetRelativeHumidity();
  virtual double GetRelativeHumidity() const;

  virtual bool HasRespirationAmbientTemperature() const;
  virtual SEScalarTemperature& GetRespirationAmbientTemperature();
  virtual double GetRespirationAmbientTemperature(const TemperatureUnit& unit) const;

  bool HasAmbientGas() const;
  bool HasAmbientGas(const SESubstance& substance) const;
  const std::vector<SESubstanceFraction*>& GetAmbientGases();
  const std::vector<const SESubstanceFraction*>& GetAmbientGases() const;
  SESubstanceFraction& GetAmbientGas(SESubstance& substance);
  const SESubstanceFraction* GetAmbientGas(const SESubstance& substance) const;
  void RemoveAmbientGas(const SESubstance& substance);
  void RemoveAmbientGases();

  bool HasAmbientAerosol() const;
  bool HasAmbientAerosol(const SESubstance& substance) const;
  const std::vector<SESubstanceConcentration*>& GetAmbientAerosols();
  const std::vector<const SESubstanceConcentration*>& GetAmbientAerosols() const;
  SESubstanceConcentration& GetAmbientAerosol(SESubstance& substance);
  const SESubstanceConcentration* GetAmbientAerosol(const SESubstance& substance) const;
  void RemoveAmbientAerosol(const SESubstance& substance);
  void RemoveAmbientAerosols();

protected:
  virtual void Unload(CDM::EnvironmentalConditionsData& data) const;
  virtual void Merge(const SEEnvironmentalConditions& from);

protected:
  CDM::enumSurroundingType::value m_SurroundingType;

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