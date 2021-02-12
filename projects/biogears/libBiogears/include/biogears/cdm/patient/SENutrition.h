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

CDM_BIND_DECL(NutritionData)

namespace biogears {
class SEPatient;
class SEScalar;
class SEScalarMass;
class MassUnit;
class SEScalarMassPerTime;
class MassPerTimeUnit;
class SEScalarVolume;
class VolumeUnit;
namespace io {
  class PatientNutrition;
}
class BIOGEARS_API SENutrition : public Loggable {
  friend io::PatientNutrition;
public:
  SENutrition(Logger* logger);
  virtual ~SENutrition();

  virtual void Clear();

  virtual bool Load(const CDM::NutritionData& in);
  virtual CDM::NutritionData* Unload() const;

protected:
  virtual void Unload(CDM::NutritionData& data) const;

public:
  const SEScalar* GetScalar(const char* name);
  const SEScalar* GetScalar(const std::string& name);

  bool Load(const char* nutritionFile);
  bool Load(const std::string& nutritionFile);

  void Increment(const SENutrition& from);

  virtual std::string GetName() const;
  virtual const char* GetName_cStr() const;
  virtual void SetName(const char* name);
  virtual void SetName(const std::string& name);
  virtual bool HasName() const;
  virtual void InvalidateName();

  virtual bool HasCarbohydrate() const;
  virtual SEScalarMass& GetCarbohydrate();
  virtual double GetCarbohydrate(const MassUnit& unit) const;

  virtual bool HasFat() const;
  virtual SEScalarMass& GetFat();
  virtual double GetFat(const MassUnit& unit) const;

  virtual bool HasProtein() const;
  virtual SEScalarMass& GetProtein();
  virtual double GetProtein(const MassUnit& unit) const;

  virtual bool HasSodium() const;
  virtual SEScalarMass& GetSodium();
  virtual double GetSodium(const MassUnit& unit) const;

  virtual bool HasCalcium() const;
  virtual SEScalarMass& GetCalcium();
  virtual double GetCalcium(const MassUnit& unit) const;

  virtual bool HasWater() const;
  virtual SEScalarVolume& GetWater();
  virtual double GetWater(const VolumeUnit& unit) const;

  virtual double GetWeight(const MassUnit& unit) const;

  virtual void ToString(std::ostream& str) const;

protected:
  std::string m_Name;
  SEScalarMass* m_Carbohydrate;
  SEScalarMass* m_Fat;
  SEScalarMass* m_Protein;
  SEScalarMass* m_Calcium;
  SEScalarMass* m_Sodium;
  SEScalarVolume* m_Water;
};

inline std::ostream& operator<<(std::ostream& out, const SENutrition& n)
{
  n.ToString(out);
  return out;
}
}