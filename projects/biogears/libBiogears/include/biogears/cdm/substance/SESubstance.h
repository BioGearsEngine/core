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

#include <biogears/cdm/enums/SESubstanceEnums.h>
#include <biogears/cdm/substance/SESubstanceAerosolization.h>
#include <biogears/cdm/substance/SESubstanceClearance.h>
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>
#include <biogears/cdm/substance/SESubstancePharmacokinetics.h>

#include <string>
#include <vector>

#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarElectricResistance.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarInversePressure.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>
#include <biogears/cdm/properties/SEScalarMassPerAreaTime.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTimeMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/properties/SEScalarVolumePerTimePressure.h>
namespace biogears {
class SESubstanceManager;
namespace io {
  class Substance;
} // namespace io

class BIOGEARS_API SESubstanceDefinition {

public:
  SESubstanceDefinition(SESubstanceDefinition const& obj);
  SESubstanceDefinition(SESubstanceDefinition&& obj);
 ~SESubstanceDefinition();
  SESubstanceDefinition(Logger* logger = nullptr);

  SESubstanceDefinition(SESubstanceClearanceDefinition definition, Logger* logger = nullptr);

  SESubstanceDefinition& operator=(SESubstanceDefinition const& rhs);
  SESubstanceDefinition& operator=(SESubstanceDefinition&& rhs);
  bool operator==(SESubstanceDefinition const& rhs) const;
  bool operator!=(SESubstanceDefinition const& rhs) const;

  std::string Name;
  SESubstanceClass Classification = SESubstanceClass::Invalid;
  SESubstanceState State = SESubstanceState::Invalid;

  SEScalarMassPerVolume Density;
  SEScalarMassPerAmount MolarMass;
  SEScalarMassPerAreaTime MaximumDiffusionFlux;
  SEScalar MichaelisCoefficient;
  SEScalarElectricResistance MembraneResistance;
  SEScalarTimeMassPerVolume AreaUnderCurve;
  SEScalarVolumePerTime AlveolarTransfer;
  SEScalarVolumePerTimePressure DiffusingCapacity;
  SEScalar RelativeDiffusionCoefficient;
  SEScalarInversePressure SolubilityCoefficient;

  SESubstanceClearanceDefinition ClearanceDefinition;
  SESubstanceAerosolization Aerosolization;
  SESubstancePharmacokinetics Pharmacokinetics;
  SESubstancePharmacodynamics Pharmacodynamics;
};

class BIOGEARS_API SESubstance : public Loggable {
  friend io::Substance;
  friend SESubstanceManager;

public:
  SESubstance(Logger* logger);
  SESubstance(SESubstanceDefinition definition);
  virtual ~SESubstance();

  virtual void Clear();

  virtual const SEScalar* GetScalar(const char* name);
  virtual const SEScalar* GetScalar(const std::string& name);

  virtual std::string GetName() const;
  virtual const char* GetName_cStr() const;
  virtual void SetName(const char* name);
  virtual void SetName(const std::string& name);
  virtual bool HasName() const;
  virtual void InvalidateName();

  virtual SESubstanceState GetState() const;
  virtual void SetState(SESubstanceState state);
  virtual bool HasState() const;
  virtual void InvalidateState();

  virtual SESubstanceClass GetClassification() const;
  virtual void SetClassification(SESubstanceClass subClass);
  virtual bool HasClassification() const;
  virtual void InvalidateClassification();

  virtual bool HasDensity() const;
  virtual SEScalarMassPerVolume& GetDensity();
  virtual double GetDensity(const MassPerVolumeUnit& unit) const;

  virtual bool HasMolarMass() const;
  virtual SEScalarMassPerAmount& GetMolarMass();
  virtual double GetMolarMass(const MassPerAmountUnit& unit) const;

  // Diffusion-ish
  virtual bool HasMaximumDiffusionFlux() const;
  virtual SEScalarMassPerAreaTime& GetMaximumDiffusionFlux();
  virtual double GetMaximumDiffusionFlux(const MassPerAreaTimeUnit& unit) const;

  virtual bool HasMichaelisCoefficient() const;
  virtual SEScalar& GetMichaelisCoefficient();
  virtual double GetMichaelisCoefficient() const;

  virtual bool HasMembraneResistance() const;
  virtual SEScalarElectricResistance& GetMembraneResistance();
  virtual double GetMembraneResistance(const ElectricResistanceUnit& unit) const;

  // Liquid-ish
  virtual bool HasAerosolization() const;
  virtual SESubstanceAerosolization& GetAerosolization();
  virtual const SESubstanceAerosolization* GetAerosolization() const;
  virtual void RemoveAerosolization();

  virtual bool HasAreaUnderCurve() const;
  virtual SEScalarTimeMassPerVolume& GetAreaUnderCurve();
  virtual double GetAreaUnderCurve(const TimeMassPerVolumeUnit& unit) const;

  virtual bool HasBloodConcentration() const;
  virtual SEScalarMassPerVolume& GetBloodConcentration();
  virtual double GetBloodConcentration(const MassPerVolumeUnit& unit) const;

  virtual bool HasEffectSiteConcentration() const;
  virtual SEScalarMassPerVolume& GetEffectSiteConcentration();
  virtual double GetEffectSiteConcentration(const MassPerVolumeUnit& unit) const;

  virtual bool HasMassInBody() const;
  virtual SEScalarMass& GetMassInBody();
  virtual double GetMassInBody(const MassUnit& unit) const;

  virtual bool HasMassInBlood() const;
  virtual SEScalarMass& GetMassInBlood();
  virtual double GetMassInBlood(const MassUnit& unit) const;

  virtual bool HasMassInTissue() const;
  virtual SEScalarMass& GetMassInTissue();
  virtual double GetMassInTissue(const MassUnit& unit) const;

  virtual bool HasPlasmaConcentration() const;
  virtual SEScalarMassPerVolume& GetPlasmaConcentration();
  virtual double GetPlasmaConcentration(const MassPerVolumeUnit& unit) const;

  virtual bool HasSystemicMassCleared() const;
  virtual SEScalarMass& GetSystemicMassCleared();
  virtual double GetSystemicMassCleared(const MassUnit& unit) const;

  virtual bool HasTissueConcentration() const;
  virtual SEScalarMassPerVolume& GetTissueConcentration();
  virtual double GetTissueConcentration(const MassPerVolumeUnit& unit) const;

  // Gas-ish
  virtual bool HasAlveolarTransfer() const;
  virtual SEScalarVolumePerTime& GetAlveolarTransfer();
  virtual double GetAlveolarTransfer(const VolumePerTimeUnit& unit) const;

  virtual bool HasDiffusingCapacity() const;
  virtual SEScalarVolumePerTimePressure& GetDiffusingCapacity();
  virtual double GetDiffusingCapacity(const VolumePerTimePressureUnit& unit) const;

  virtual bool HasEndTidalFraction() const;
  virtual SEScalarFraction& GetEndTidalFraction();
  virtual double GetEndTidalFraction() const;

  virtual bool HasEndTidalPressure() const;
  virtual SEScalarPressure& GetEndTidalPressure();
  virtual double GetEndTidalPressure(const PressureUnit& unit) const;

  virtual bool HasRelativeDiffusionCoefficient() const;
  virtual SEScalar& GetRelativeDiffusionCoefficient();
  virtual double GetRelativeDiffusionCoefficient() const;

  virtual bool HasSolubilityCoefficient() const;
  virtual SEScalarInversePressure& GetSolubilityCoefficient();
  virtual double GetSolubilityCoefficient(const InversePressureUnit& unit) const;

  virtual bool HasClearance() const;
  virtual SESubstanceClearance& GetClearance();
  virtual const SESubstanceClearance* GetClearance() const;
  virtual void RemoveClearance();

  virtual bool HasPK() const;
  virtual SESubstancePharmacokinetics& GetPK();
  virtual const SESubstancePharmacokinetics* GetPK() const;
  virtual void RemovePK();

  virtual bool HasPD() const;
  virtual SESubstancePharmacodynamics& GetPD();
  virtual const SESubstancePharmacodynamics* GetPD() const;
  virtual void RemovePD();

  bool operator==(const SESubstance& rhs) const;
  bool operator!=(const SESubstance& rhs) const;

  bool operator==(const SESubstanceDefinition& rhs) const;
  bool operator!=(const SESubstanceDefinition& rhs) const;

  SESubstanceDefinition GetDefinition() const;

private:
  SESubstanceDefinition m_def;

  SESubstanceClearance m_Clearance;

  SEScalarMassPerVolume m_BloodConcentration;
  SEScalarMassPerVolume m_EffectSiteConcentration;
  SEScalarMass m_MassInBody;
  SEScalarMass m_MassInBlood;
  SEScalarMass m_MassInTissue;
  SEScalarMassPerVolume m_PlasmaConcentration;
  SEScalarMass m_SystemicMassCleared;
  SEScalarMassPerVolume m_TissueConcentration;

  SEScalarFraction m_EndTidalFraction;
  SEScalarPressure m_EndTidalPressure;
};

inline bool operator==(const SESubstanceDefinition& lhs, const SESubstance& rhs)
{
  return rhs == lhs;
}

inline bool operator!=(const SESubstanceDefinition& lhs, const SESubstance& rhs)
{
  return rhs == lhs;
}

} // namespace biogears
#pragma warning(disable : 4661)

namespace std {
extern template class vector<biogears::SESubstance*>;
} // namespace std

#pragma warning(default : 4661)
