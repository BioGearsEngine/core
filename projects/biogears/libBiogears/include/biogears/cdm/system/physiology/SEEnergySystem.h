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
class SEScalarFraction;
class SEScalarMass;
class MassUnit;
class SEScalarTemperature;
class TemperatureUnit;
class SEScalarAmountPerTime;
class AmountPerTimeUnit;
class SEScalarPressure;
class PressureUnit;
class SEScalarMassPerTime;
class MassPerTimeUnit;
class SEScalarPower;
class PowerUnit;

class BIOGEARS_API SEEnergySystem : public SESystem {
public:
  SEEnergySystem(Logger* logger);
  ~SEEnergySystem() override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); }
  static constexpr char const * const  TypeTag() { return "SEEnergySystem"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override; // Deletes all members

  const SEScalar* GetScalar(const char* name) override;
  const SEScalar* GetScalar(const std::string& name) override;

  bool Load(const CDM::EnergySystemData& in);
  CDM::EnergySystemData* Unload() const override;

  Tree<const char*> GetPhysiologyRequestGraph() const override;
protected:
  void Unload(CDM::EnergySystemData& data) const;

public:
  bool HasAchievedExerciseLevel() const;
  SEScalarFraction& GetAchievedExerciseLevel();
  double GetAchievedExerciseLevel() const;

  bool HasChlorideLostToSweat() const;
  SEScalarMass& GetChlorideLostToSweat();
  double GetChlorideLostToSweat(const MassUnit& unit) const;

  bool HasCoreTemperature() const;
  SEScalarTemperature& GetCoreTemperature();
  double GetCoreTemperature(const TemperatureUnit& unit) const;

  bool HasCreatinineProductionRate() const;
  SEScalarAmountPerTime& GetCreatinineProductionRate();
  double GetCreatinineProductionRate(const AmountPerTimeUnit& unit) const;

  bool HasEnergyDeficit() const;
  SEScalarPower& GetEnergyDeficit();
  double GetEnergyDeficit(const PowerUnit& unit) const;

  bool HasExerciseEnergyDemand() const;
  SEScalarPower& GetExerciseEnergyDemand();
  double GetExerciseEnergyDemand(const PowerUnit& unit) const;

  bool HasExerciseMeanArterialPressureDelta() const;
  SEScalarPressure& GetExerciseMeanArterialPressureDelta();
  double GetExerciseMeanArterialPressureDelta(const PressureUnit& unit) const;

  bool HasFatigueLevel() const;
  SEScalarFraction& GetFatigueLevel();
  double GetFatigueLevel() const;

  bool HasLactateProductionRate() const;
  SEScalarAmountPerTime& GetLactateProductionRate();
  double GetLactateProductionRate(const AmountPerTimeUnit& unit) const;

  bool HasPotassiumLostToSweat() const;
  SEScalarMass& GetPotassiumLostToSweat();
  double GetPotassiumLostToSweat(const MassUnit& unit) const;

  bool HasSkinTemperature() const;
  SEScalarTemperature& GetSkinTemperature();
  double GetSkinTemperature(const TemperatureUnit& unit) const;

  bool HasSodiumLostToSweat() const;
  SEScalarMass& GetSodiumLostToSweat();
  double GetSodiumLostToSweat(const MassUnit& unit) const;

  bool HasSweatRate() const;
  SEScalarMassPerTime& GetSweatRate();
  double GetSweatRate(const MassPerTimeUnit& unit) const;

  bool HasTotalMetabolicRate() const;
  SEScalarPower& GetTotalMetabolicRate();
  double GetTotalMetabolicRate(const PowerUnit& unit) const;

  bool HasTotalWorkRateLevel() const;
  SEScalarFraction& GetTotalWorkRateLevel();
  double GetTotalWorkRateLevel() const;

protected:
  SEScalarFraction* m_AchievedExerciseLevel;
  SEScalarMass* m_ChlorideLostToSweat;
  SEScalarTemperature* m_CoreTemperature;
  SEScalarAmountPerTime* m_CreatinineProductionRate;
  SEScalarPower* m_EnergyDeficit;
  SEScalarPower* m_ExerciseEnergyDemand;
  SEScalarPressure* m_ExerciseMeanArterialPressureDelta;
  SEScalarFraction* m_FatigueLevel;
  SEScalarAmountPerTime* m_LactateProductionRate;
  SEScalarMass* m_PotassiumLostToSweat;
  SEScalarTemperature* m_SkinTemperature;
  SEScalarMass* m_SodiumLostToSweat;
  SEScalarMassPerTime* m_SweatRate;
  SEScalarPower* m_TotalMetabolicRate;
  SEScalarFraction* m_TotalWorkRateLevel;
};
}