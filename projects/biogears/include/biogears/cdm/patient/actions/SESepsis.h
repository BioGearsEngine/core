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

#include <biogears/cdm/patient/actions/SEPatientAction.h>
#include <biogears/schema/cdm/PatientActions.hxx>

namespace biogears {
class SEScalar0To1;
class SEScalar;

class BIOGEARS_API SESepsis : public SEPatientAction {
public:
  SESepsis();
  virtual ~SESepsis();

  static constexpr const char* TypeTag() { return "SESepsis"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual bool Load(const CDM::SepsisData& in);
  virtual CDM::SepsisData* Unload() const;

protected:
  virtual void Unload(CDM::SepsisData& data) const;

public:
  virtual std::string GetCompartment() const;
  virtual bool HasCompartment() const;
  virtual void SetCompartment(const std::string& name);
  virtual void InvalidateCompartment();

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();

  virtual void BuildTissueResistorMap();
  virtual const std::map<std::string, std::string>& GetTissueResistorMap() const;

  virtual void ToString(std::ostream& str) const;

protected:
  std::string m_Compartment;
  SEScalar0To1* m_Severity;
  std::map<std::string, std::string> m_TissueResistorMap;
};
}
