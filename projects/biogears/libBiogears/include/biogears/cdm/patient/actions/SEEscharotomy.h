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

OPEN_BIOGEARS_NAMESPACE
namespace io {
  class PatientActions;
}
class BIOGEARS_API SEEscharotomy : public SEPatientAction {
  friend io::PatientActions;
public:
  SEEscharotomy();
  virtual ~SEEscharotomy() override;

  static constexpr const char* TypeTag() { return "SEEscharotomy"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override; //clear memory

  virtual bool IsValid() const override;
  virtual bool IsActive() const override;

  virtual bool Load(const CDM::EscharotomyData& in);
  virtual CDM::EscharotomyData* Unload() const override;

protected:
  virtual void Unload(CDM::EscharotomyData& data) const;

public:
  virtual std::string GetLocation() const;
  virtual bool HasLocation() const;
  virtual void SetLocation(const char* name);
  virtual void SetLocation(const std::string& name);
  virtual void InvalidateLocation();

  virtual void ToString(std::ostream& str) const override;

  bool operator==(const SEEscharotomy& rhs) const;
  bool operator!=(const SEEscharotomy& rhs) const;

protected:
  std::string m_Location;
};
CLOSE_BIOGEARS_NAMESPACE
