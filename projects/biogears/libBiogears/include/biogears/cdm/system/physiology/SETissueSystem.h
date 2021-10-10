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
class SEScalarVolumePerTime;
class VolumePerTimeUnit;
class SEScalar0To1;
class SEScalarVolume;
class VolumeUnit;
class SEScalarAmountPerVolume;
class AmountPerVolumeUnit;
class SEScalarMassPerVolume;
class MassPerVolumeUnit;
class SEScalarMass;
class MassUnit;
namespace io {
  class Physiology;
}
class BIOGEARS_API SETissueSystem : public SESystem {
  friend io::Physiology;

public:
  SETissueSystem(Logger* logger);
  ~SETissueSystem() override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); }
  static constexpr char const* const TypeTag() { return "SETissueSystem"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override; // Deletes all members

  const SEScalar* GetScalar(const char* name) override;
  const SEScalar* GetScalar(const std::string& name) override;

  bool Load(const CDM::TissueSystemData& in);
  CDM::TissueSystemData* Unload() const override;

  Tree<const char*> GetPhysiologyRequestGraph() const override;

protected:
  void Unload(CDM::TissueSystemData& data) const;

public:
  bool HasCarbonDioxideProductionRate() const;
  SEScalarVolumePerTime& GetCarbonDioxideProductionRate();
  double GetCarbonDioxideProductionRate(const VolumePerTimeUnit& unit) const;

  bool HasDehydrationFraction() const;
  SEScalar0To1& GetDehydrationFraction();
  double GetDehydrationFraction() const;

  bool HasExtracellularFluidVolume() const;
  SEScalarVolume& GetExtracellularFluidVolume();
  double GetExtracellularFluidVolume(const VolumeUnit& unit) const;

  bool HasExtravascularFluidVolume() const;
  SEScalarVolume& GetExtravascularFluidVolume();
  double GetExtravascularFluidVolume(const VolumeUnit& unit) const;

  bool HasIntracellularFluidVolume() const;
  SEScalarVolume& GetIntracellularFluidVolume();
  double GetIntracellularFluidVolume(const VolumeUnit& unit) const;

  bool HasTotalBodyFluidVolume() const;
  SEScalarVolume& GetTotalBodyFluidVolume();
  double GetTotalBodyFluidVolume(const VolumeUnit& unit) const;

  bool HasIntracellularFluidPH() const;
  SEScalar& GetIntracellularFluidPH();
  double GetIntracellularFluidPH() const;

  bool HasOxygenConsumptionRate() const;
  SEScalarVolumePerTime& GetOxygenConsumptionRate();
  double GetOxygenConsumptionRate(const VolumePerTimeUnit& unit) const;

  bool HasRespiratoryExchangeRatio() const;
  SEScalar& GetRespiratoryExchangeRatio();
  double GetRespiratoryExchangeRatio() const;

  bool HasLiverInsulinSetPoint() const;
  SEScalarAmountPerVolume& GetLiverInsulinSetPoint();
  double GetLiverInsulinSetPoint(const AmountPerVolumeUnit& unit) const;

  bool HasLiverGlucagonSetPoint() const;
  SEScalarMassPerVolume& GetLiverGlucagonSetPoint();
  double GetLiverGlucagonSetPoint(const MassPerVolumeUnit& unit) const;

  bool HasMuscleInsulinSetPoint() const;
  SEScalarAmountPerVolume& GetMuscleInsulinSetPoint();
  double GetMuscleInsulinSetPoint(const AmountPerVolumeUnit& unit) const;

  bool HasMuscleGlucagonSetPoint() const;
  SEScalarMassPerVolume& GetMuscleGlucagonSetPoint();
  double GetMuscleGlucagonSetPoint(const MassPerVolumeUnit& unit) const;

  bool HasFatInsulinSetPoint() const;
  SEScalarAmountPerVolume& GetFatInsulinSetPoint();
  double GetFatInsulinSetPoint(const AmountPerVolumeUnit& unit) const;

  bool HasFatGlucagonSetPoint() const;
  SEScalarMassPerVolume& GetFatGlucagonSetPoint();
  double GetFatGlucagonSetPoint(const MassPerVolumeUnit& unit) const;

  bool HasLiverGlycogen() const;
  SEScalarMass& GetLiverGlycogen();
  double GetLiverGlycogen(const MassUnit& unit) const;

  bool HasMuscleGlycogen() const;
  SEScalarMass& GetMuscleGlycogen();
  double GetMuscleGlycogen(const MassUnit& unit) const;

  bool HasStoredProtein() const;
  SEScalarMass& GetStoredProtein();
  double GetStoredProtein(const MassUnit& unit) const;

  bool HasStoredFat() const;
  SEScalarMass& GetStoredFat();
  double GetStoredFat(const MassUnit& unit) const;

protected:
  SEScalarVolumePerTime* m_CarbonDioxideProductionRate;
  SEScalar0To1* m_DehydrationFraction;
  SEScalarVolume* m_ExtracellularFluidVolume;
  SEScalarVolume* m_ExtravascularFluidVolume;
  SEScalarVolume* m_IntracellularFluidVolume;
  SEScalarVolume* m_TotalBodyFluidVolume;
  SEScalar* m_IntracellularFluidPH;
  SEScalarVolumePerTime* m_OxygenConsumptionRate;
  SEScalar* m_RespiratoryExchangeRatio;
  SEScalarAmountPerVolume* m_LiverInsulinSetPoint;
  SEScalarMassPerVolume* m_LiverGlucagonSetPoint;
  SEScalarAmountPerVolume* m_MuscleInsulinSetPoint;
  SEScalarMassPerVolume* m_MuscleGlucagonSetPoint;
  SEScalarAmountPerVolume* m_FatInsulinSetPoint;
  SEScalarMassPerVolume* m_FatGlucagonSetPoint;
  SEScalarMass* m_LiverGlycogen;
  SEScalarMass* m_MuscleGlycogen;
  SEScalarMass* m_StoredProtein;
  SEScalarMass* m_StoredFat;
};
}