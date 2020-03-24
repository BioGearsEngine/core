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

#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>

namespace biogears {
class BIOGEARS_API SEPainStimulus : public SEPatientAction {
public:
  SEPainStimulus();
  virtual ~SEPainStimulus() override;

  static constexpr const char* TypeTag() { return "SEPainStimulus"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override;

  virtual bool IsValid() const override;
  virtual bool IsActive() const override;

  virtual bool Load(const CDM::PainStimulusData& in);
  virtual CDM::PainStimulusData* Unload() const override;

protected:
  virtual void Unload(CDM::PainStimulusData& data) const;

public:
  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();

  virtual bool HasDecayRate() const;
  virtual SEScalarFrequency& GetDecayRate();

  virtual bool HasLocation() const;
  virtual const char* GetLocation_cStr() const;
  virtual std::string GetLocation() const;
  virtual void SetLocation(const char* name);
  virtual void SetLocation(const std::string& name);

  virtual void ToString(std::ostream& str) const override;

protected:
  SEScalar0To1* m_Severity;
  SEScalarFrequency* m_DecayRate = 0;
  std::string m_Location;
};
}
