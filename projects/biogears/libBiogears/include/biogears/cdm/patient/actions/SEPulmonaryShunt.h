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
#include <biogears/schema/cdm/Scenario.hxx>

OPEN_BIOGEARS_NAMESPACE
class SEScalar0To1;
namespace io {
  class PatientActions;
}
class BIOGEARS_API SEPulmonaryShunt : public SEPatientAction {
  friend io::PatientActions;

public:
  SEPulmonaryShunt();
  virtual ~SEPulmonaryShunt() override;

  static constexpr const char* TypeTag() { return "SEPulmonaryShunt"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override;

  virtual bool IsValid() const override;
  virtual bool IsActive() const override;

  virtual bool Load(const CDM::PulmonaryShuntData& in);
  virtual CDM::PulmonaryShuntData* Unload() const override;

protected:
  virtual void Unload(CDM::PulmonaryShuntData& data) const;

public:
  virtual bool HasFlowRateScale() const;
  virtual SEScalar0To1& GetFlowRateScale();

  virtual void ToString(std::ostream& str) const override;
  bool operator==(const SEPulmonaryShunt& rhs) const;
  bool operator!=(const SEPulmonaryShunt& rhs) const;

protected:
  SEScalar0To1* m_FlowRateScaling;
};
CLOSE_BIOGEARS_NAMESPACE