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
class SEScalarMassPerVolume;

class BIOGEARS_API SEInfection : public SEPatientAction {
public:
  SEInfection();
  virtual ~SEInfection() override;

  static constexpr const char* TypeTag() { return "SEInfection"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override; //clear memory

  virtual bool IsValid() const override;
  virtual bool IsActive() const override;

  virtual bool Load(const CDM::InfectionData& in);
  virtual CDM::InfectionData* Unload() const override;

protected:
  virtual void Unload(CDM::InfectionData& data) const;

public:
  virtual const char* GetLocation_cStr() const;
  virtual std::string GetLocation() const;
  virtual bool HasLocation() const;
  virtual void SetLocation(const char* name);
  virtual void SetLocation(const std::string& name);
  virtual void InvalidateLocation();

  virtual bool HasSeverity() const;
  virtual CDM::enumInfectionSeverity::value GetSeverity() const;
  virtual void SetSeverity(CDM::enumInfectionSeverity::value t);
  virtual void InvalidateSeverity();

  virtual bool HasMinimumInhibitoryConcentration() const;
  virtual SEScalarMassPerVolume& GetMinimumInhibitoryConcentration();

  virtual void ToString(std::ostream& str) const override;

protected:
  std::string m_Location;
  CDM::enumInfectionSeverity::value m_Severity;
  SEScalarMassPerVolume* m_MinimumInhibitoryConcentration;


}; 
}