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
#include <biogears/cdm/patient/actions/SEPatientAction.h>
#include <biogears/schema/cdm/PatientActions.hxx>

namespace biogears {
class SEScalar0To1;
namespace io {
  class PatientActions;
}
class BIOGEARS_API SEBronchoconstriction : public SEPatientAction {
  friend io::PatientActions;

public:
  SEBronchoconstriction();
  virtual ~SEBronchoconstriction() override;

  static constexpr const char* TypeTag() { return "SEBronchoconstriction"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override; //clear memory

  virtual bool IsValid() const override;
  virtual bool IsActive() const override;

  virtual bool Load(const CDM::BronchoconstrictionData& in);
  virtual CDM::BronchoconstrictionData* Unload() const override;

protected:
  virtual void Unload(CDM::BronchoconstrictionData& data) const;

public:
  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();

  virtual void ToString(std::ostream& str) const override;

protected:
  SEScalar0To1* m_Severity;
};
}
