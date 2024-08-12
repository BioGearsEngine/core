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
#include <string>

#include <biogears/cdm/patient/conditions/SEPatientCondition.h>
#include <biogears/exports.h>
#include <biogears/schema/cdm/PatientConditions.hxx>

namespace biogears {
class SEScalar0To1;
namespace io {
  class PatientConditions;
}
class BIOGEARS_API SEDiabetesType1 : public SEPatientCondition {
  friend io::PatientConditions;

public:
  SEDiabetesType1();
  virtual ~SEDiabetesType1();

  virtual void Clear() override; //clear memory

  virtual bool IsValid() const override;

  bool operator==(SEDiabetesType1 const&) const;
  bool operator!=(SEDiabetesType1 const&) const;
  bool operator==(SECondition const& rhs) const override;
  bool operator!=(SECondition const& rhs) const override;

  virtual std::string GetName() const  override { return "DiabetesType1"; }
  virtual const char* GetName_cStr() const  override { return "DiabetesType1"; }

  virtual bool HasInsulinProductionSeverity() const;
  virtual SEScalar0To1& GetInsulinProductionSeverity();

  virtual void ToString(std::ostream& str) const override ;

protected:
  SEScalar0To1* m_InsulinProductionSeverity;
};
}
