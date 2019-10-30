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
class SEScalarVolumePerTime;

class BIOGEARS_API SETourniquet : public SEPatientAction {
public:
  SETourniquet();
  virtual ~SETourniquet() override;

  static constexpr const char* TypeTag() { return "SETourniquet"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override; //clear memory

  virtual bool IsValid() const override;
  virtual bool IsActive() const override;

  virtual bool Load(const CDM::TourniquetData& in);
  virtual CDM::TourniquetData* Unload() const override;

protected:
  virtual void Unload(CDM::TourniquetData& data) const;

public:
  virtual const char* GetCompartment_cStr() const;
  virtual std::string GetCompartment() const;
  virtual bool HasCompartment() const;
  virtual void SetCompartment(const char* name);
  virtual void SetCompartment(const std::string& name);
  virtual void InvalidateCompartment();

  virtual bool HasTourniquetLevel() const;
  virtual CDM::enumTourniquetApplicationLevel GetTourniquetLevel();
  virtual void SetTourniquetLevel(CDM::enumTourniquetApplicationLevel::value level);

  virtual void ToString(std::ostream& str) const override;

protected:
  std::string m_Compartment;
  CDM::enumTourniquetApplicationLevel::value m_TourniquetLevel;
 
};
}