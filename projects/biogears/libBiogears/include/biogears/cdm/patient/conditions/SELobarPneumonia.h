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
#include <biogears/cdm/patient/conditions/SEPatientCondition.h>
#include <biogears/schema/cdm/PatientConditions.hxx>

namespace biogears {
class SEScalar0To1;

class BIOGEARS_API SELobarPneumonia : public SEPatientCondition {
public:
  SELobarPneumonia();
  virtual ~SELobarPneumonia();

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;

  virtual bool Load(const CDM::LobarPneumoniaData& in);
  virtual CDM::LobarPneumoniaData* Unload() const;

protected:
  virtual void Unload(CDM::LobarPneumoniaData& data) const;

public:
  virtual std::string GetName() const { return "LobarPneumonia"; }
  virtual const char* GetName_cStr() const { return "LobarPneumonia"; }

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();

  virtual bool HasLeftLungAffected() const;
  virtual SEScalar0To1& GetLeftLungAffected();

  virtual bool HasRightLungAffected() const;
  virtual SEScalar0To1& GetRightLungAffected();

  virtual void ToString(std::ostream& str) const;

protected:
  SEScalar0To1* m_Severity;
  SEScalar0To1* m_LeftLungAffected;
  SEScalar0To1* m_RightLungAffected;
};
}