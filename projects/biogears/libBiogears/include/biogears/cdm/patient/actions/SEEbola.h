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
#include <biogears/cdm/patient/actions/SEPatientAction.h>
#include <biogears/exports.h>
#include <biogears/schema/cdm/PatientActions.hxx>

namespace biogears {
  class SEScalarMassPerVolume;
  namespace io {
    class PatientActions;
  }

class BIOGEARS_API SEEbola : public SEPatientAction {
  friend io::PatientActions;

public:
  SEEbola();
  virtual ~SEEbola() override;

  static constexpr const char* TypeTag() { return "SEEbola"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override; //clear memory

  virtual bool IsValid() const override;
  virtual bool IsActive() const override;

  virtual bool Load(const CDM::EbolaData& in);
  virtual CDM::EbolaData* Unload() const override;

  virtual bool HasSeverity() const;
  virtual CDM::enumInfectionSeverity::value GetSeverity() const;
  virtual void SetSeverity(CDM::enumInfectionSeverity::value t);
  virtual void InvalidateSeverity();

  virtual void ToString(std::ostream& str) const override;

  bool operator==(const SEEbola& rhs) const;
  bool operator!=(const SEEbola& rhs) const;

protected:
  virtual void Unload(CDM::EbolaData& data) const;

protected:
  CDM::enumInfectionSeverity::value m_Severity;
};
}