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

#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/schema/cdm/PatientActions.hxx>

class Serializer;

class BIOGEARS_API SEOverride : public SEPatientAction {
public:
  SEOverride();
  virtual ~SEOverride();

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual bool Load(const CDM::OverrideData& in);
  virtual CDM::OverrideData* Unload() const;

protected:
  virtual void Unload(CDM::OverrideData& data) const;

public:
  CDM::enumOnOff::value SEOverride::GetOverrideSwitch() const;
  void SEOverride::SetOverrideSwitch(CDM::enumOnOff::value state);
  bool SEOverride::HasOverrideSwitch() const;
  void SEOverride::InvalidateOverrideSwitch();
  bool SEOverride::HasMAPOverride() const;
  SEScalarPressure& SEOverride::GetMAPOverride();
  double SEOverride::GetMAPOverride(const PressureUnit& unit) const;

  virtual void ToString(std::ostream& str) const;

protected:
  CDM::enumOnOff m_OverrideSwitch;
  SEScalarPressure* m_PressureOR;
};
