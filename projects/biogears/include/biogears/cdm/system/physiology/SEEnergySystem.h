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
  virtual ~SEEnergySystem();

  virtual void Clear(); // Deletes all members

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool Load(const CDM::EnergySystemData& in);
  virtual CDM::EnergySystemData* Unload() const;

protected:
  virtual void Unload(CDM::EnergySystemData& data) const;

public:
  virtual bool HasAchievedExerciseLevel() const;
  virtual SEScalarFraction& GetAchievedExerciseLevel();
  virtual double GetAchievedExerciseLevel() const;

  virtual bool HasChlorideLostToSweat() const;
  virtual SEScalarMass& GetChlorideLostToSweat();
  virtual double GetChlorideLostToSweat(const MassUnit& unit) const;

  virtual bool HasCoreTemperature() const;
  virtual SEScalarTemperature& GetCoreTemperature();
  virtual double GetCoreTemperature(const TemperatureUnit& unit) const;

  virtual bool HasCreatinineProductionRate() const;
  virtual SEScalarAmountPerTime& GetCreatinineProductionRate();
  virtual double GetCreatinineProductionRate(const AmountPerTimeUnit& unit) const;

  virtual bool HasExerciseMeanArterialPressureDelta() const;
  virtual SEScalarPressure& GetExerciseMeanArterialPressureDelta();
  virtual double GetExerciseMeanArterialPressureDelta(const PressureUnit& unit) const;

  virtual bool HasFatigueLevel() const;
  virtual SEScalarFraction& GetFatigueLevel();
  virtual double GetFatigueLevel() const;

  virtual bool HasLactateProductionRate() const;
  virtual SEScalarAmountPerTime& GetLactateProductionRate();
  virtual double GetLactateProductionRate(const AmountPerTimeUnit& unit) const;

  virtual bool HasPotassiumLostToSweat() const;
  virtual SEScalarMass& GetPotassiumLostToSweat();
  virtual double GetPotassiumLostToSweat(const MassUnit& unit) const;

  virtual bool HasSkinTemperature() const;
  virtual SEScalarTemperature& GetSkinTemperature();
  virtual double GetSkinTemperature(const TemperatureUnit& unit) const;

  virtual bool HasSodiumLostToSweat() const;
  virtual SEScalarMass& GetSodiumLostToSweat();
  virtual double GetSodiumLostToSweat(const MassUnit& unit) const;

  virtual bool HasSweatRate() const;
  virtual SEScalarMassPerTime& GetSweatRate();
  virtual double GetSweatRate(const MassPerTimeUnit& unit) const;

  virtual bool HasTotalMetabolicRate() const;
  virtual SEScalarPower& GetTotalMetabolicRate();
  virtual double GetTotalMetabolicRate(const PowerUnit& unit) const;

  virtual bool HasTotalWorkRateLevel() const;
  virtual SEScalarFraction& GetTotalWorkRateLevel();
  virtual double GetTotalWorkRateLevel() const;

protected:
  SEScalarFraction* m_AchievedExerciseLevel;
  SEScalarMass* m_ChlorideLostToSweat;
  SEScalarTemperature* m_CoreTemperature;
  SEScalarAmountPerTime* m_CreatinineProductionRate;
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
