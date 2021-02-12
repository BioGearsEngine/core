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

#include <biogears/cdm/patient/SENutrition.h>
#include <biogears/cdm/patient/actions/SEPatientAction.h>
#include <biogears/schema/cdm/PatientActions.hxx>

namespace biogears {
namespace io {
  class PatientActions;
}
class BIOGEARS_API SEConsumeNutrients : public SEPatientAction {
  friend io::PatientActions;

public:
  SEConsumeNutrients();
  virtual ~SEConsumeNutrients() override;

  static constexpr const char* TypeTag() { return "SEconsumeNutrients"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override; //clear memory

  virtual bool IsValid() const override;
  virtual bool IsActive() const override;

  virtual bool Load(const CDM::ConsumeNutrientsData& in);
  virtual CDM::ConsumeNutrientsData* Unload() const override;

protected:
  virtual void Unload(CDM::ConsumeNutrientsData& data) const;

public:
  bool HasNutrition() const;
  SENutrition& GetNutrition();
  const SENutrition* GetNutrition() const;

  virtual std::string GetNutritionFile() const;
  virtual const char* GetNutritionFile_cStr() const;
  virtual void SetNutritionFile(const char* fileName);
  virtual void SetNutritionFile(const std::string& fileName);
  virtual bool HasNutritionFile() const;
  virtual void InvalidateNutritionFile();

  virtual void ToString(std::ostream& str) const override;

protected:
  SENutrition* m_Nutrition;
  std::string m_NutritionFile;
};
}