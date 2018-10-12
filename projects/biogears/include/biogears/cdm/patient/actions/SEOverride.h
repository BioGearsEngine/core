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
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/schema/cdm/PatientActions.hxx>

class Serializer;

namespace biogears {
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
  CDM::enumOnOff::value GetOverrideSwitch() const;
  void SetOverrideSwitch(CDM::enumOnOff::value state);
  bool HasOverrideSwitch() const;
  void InvalidateOverrideSwitch();
  CDM::enumOnOff::value GetOverrideValidity() const;
  void SetOverrideValidity(CDM::enumOnOff::value valid);
  bool HasOverrideValidity() const;
  void InvalidateOverrideValidity();
  bool IsOverrideValid();


  bool HasMAPOverride() const;
  SEScalarPressure& GetMAPOverride();
  double GetMAPOverride(const PressureUnit& unit) const;
  bool HasCardiovascularOverride() const;

  bool HasCoreTemperatureOverride() const;
  SEScalarTemperature& GetCoreTemperatureOverride();
  double GetCoreTemperatureOverride(const TemperatureUnit& unit) const;
  bool HasSkinTemperatureOverride() const;
  SEScalarTemperature& GetSkinTemperatureOverride();
  double GetSkinTemperatureOverride(const TemperatureUnit& unit) const;
  bool HasEnergyOverride() const;

  virtual void ToString(std::ostream& str) const;

protected:
  CDM::enumOnOff m_OverrideSwitch;
  CDM::enumOnOff m_OverrideValid;
  SEScalarPressure* m_PressureOR;
  SEScalarTemperature* m_CoreTemperatureOR;
  SEScalarTemperature* m_SkinTemperatureOR;
};
}
