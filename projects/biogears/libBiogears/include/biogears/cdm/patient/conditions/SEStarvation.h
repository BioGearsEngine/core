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
#include <biogears/cdm/patient/conditions/SEPatientCondition.h>
#include <biogears/schema/cdm/PatientActions.hxx>

namespace biogears {
namespace io {
  class PatientConditions;
}
class BIOGEARS_API SEStarvation : public SEPatientCondition {
  friend io::PatientConditions;

public:
  SEStarvation();
  virtual ~SEStarvation();

  virtual void Clear() override ;

  virtual bool IsValid() const override ;

  virtual bool Load(const CDM::StarvationData& in);
  virtual CDM::StarvationData* Unload() const override ;

  bool operator==(SEStarvation const&) const;
  bool operator!=(SEStarvation const&) const;
  bool operator==(SECondition const& rhs) const override;
  bool operator!=(SECondition const& rhs) const override;


protected:
  virtual void Unload(CDM::StarvationData& data) const;

public:
  virtual std::string GetName() const  override { return "Starvation"; }
  virtual const char* GetName_cStr() const  override { return "Starvation"; }

  virtual bool HasTimeSinceMeal() const;
  virtual SEScalarTime& GetTimeSinceMeal();

  virtual void ToString(std::ostream& str) const override ;

protected:
  SEScalarTime* m_TimeSinceMeal;
};
}
