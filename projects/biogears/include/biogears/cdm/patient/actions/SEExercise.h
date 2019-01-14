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

namespace biogears {
class SEScalar0To1;
class SEScalar;

class BIOGEARS_API SEExercise : public SEPatientAction {
public:
  SEExercise();
  virtual ~SEExercise() override;

  static constexpr const char* TypeTag() { return "SEExercise"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override; //clear memory

  virtual bool IsValid() const override;
  virtual bool IsActive() const override;

  virtual bool Load(const CDM::ExerciseData& in);
  virtual CDM::ExerciseData* Unload() const override;

protected:
  virtual void Unload(CDM::ExerciseData& data) const;

public:
  virtual bool HasIntensity() const;
  virtual SEScalar0To1& GetIntensity();
  virtual bool HasDesiredWorkRate() const;
  virtual SEScalar& GetDesiredWorkRate();

  virtual void ToString(std::ostream& str) const override;

protected:
  SEScalar0To1* m_Intensity;
  SEScalar* m_DesiredWorkRate;
};
}