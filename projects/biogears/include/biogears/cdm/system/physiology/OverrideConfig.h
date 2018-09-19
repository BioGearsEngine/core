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
CDM_BIND_DECL(OverrideConfigData)
#include <biogears/exports.h>
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/schema/cdm/Properties.hxx>
#include <biogears/schema/biogears/OverrideConfig.hxx>
//#include <biogears/schema/CardiovascularOverrideData.hxx>



class BIOGEARS_API OverrideConfig : public Loggable {
public:
  OverrideConfig();
  virtual ~OverrideConfig();

  virtual void Clear(); // Deletes all members

  virtual bool LoadOverride(const std::string& file);
  virtual bool Load(const CDM::OverrideConfigData& in);
  virtual CDM::OverrideConfigData* Unload() const;
  // bool ReadOverrideParmeters(const std::string& overrideParameterFile);
  //virtual bool HasOverride() const;
  //virtual SEScalarPressure& GetOverride();
  //virtual double GetOverride(const PressureUnit& unit) const;

  virtual bool HasMeanArterialPressureOverride() const;
  virtual SEScalarPressure& GetMeanArterialPressureOverride();
  virtual double GetMeanArterialPressureOverride(const PressureUnit& unit) const;

  virtual bool HasEnableCardiovascularOverride() const { return m_overrideMode != (CDM::enumOnOff::value)-1; }
  virtual bool IsCardiovascularOverrideEnabled() const { return m_overrideMode == CDM::enumOnOff::On; }
  virtual void EnableCardiovascularOverride(CDM::enumOnOff::value s) { m_overrideMode = s; }

protected:
  virtual void Unload(CDM::OverrideConfigData& data) const;
  bool ReadOverrideParameters(const std::string& overrideParameterFile);


  CDM::enumOnOff::value m_overrideMode;
  SEScalarPressure* m_MeanArterialPressureOverride;
};