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

class BIOGEARS_API SESleep : public SEPatientAction {
public:
    SESleep();
  virtual ~SESleep() override;

  static constexpr const char* TypeTag() { return "SESleep"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override; //clear memory

  virtual bool IsValid() const override;
  virtual bool IsActive() const override;

  virtual bool Load(const CDM::SleepData& in);
  virtual CDM::SleepData* Unload() const override;

protected:
  virtual void Unload(CDM::SleepData& data) const;

public:


  virtual bool HasSleepState() const;
  virtual CDM::enumOnOff GetSleepState();
  virtual void SetSleepState(CDM::enumOnOff::value t);

  virtual void ToString(std::ostream& str) const override;

protected:
  CDM::enumOnOff::value m_SleepState;


}; 
}