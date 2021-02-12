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
#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarLengthPerTime.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarPower.h>
#include <biogears/schema/cdm/PatientActions.hxx>

namespace biogears {
class SEScalar;
namespace io {
  class PatientActions;
}
class BIOGEARS_API SEExercise : public SEPatientAction {
  friend io::PatientActions;

public:
  enum ExerciseType {
    GENERIC,
    CYCLING,
    RUNNING,
    STRENGTH_TRAINING,
    NONE
  };
  struct SEGeneric {
    SEScalarPower DesiredWorkRate;
    SEScalar0To1 Intensity;
  };
  struct SECycling {
    SEScalarFrequency CadenceCycle;
    SEScalarPower PowerCycle;
    SEScalarMass AddedWeight;
  };
  struct SERunning {
    SEScalarLengthPerTime SpeedRun;
    SEScalar0To1 InclineRun;
    SEScalarMass AddedWeight;
  };
  struct SEStrengthTraining {
    SEScalarMass WeightStrength;
    SEScalar RepsStrength;
  };

  SEExercise();
  SEExercise(SEGeneric);
  SEExercise(SECycling);
  SEExercise(SERunning);
  SEExercise(SEStrengthTraining);

  virtual ~SEExercise() override;

  static constexpr const char* TypeTag() { return "SEExercise"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override; //clear memory

  virtual bool IsValid() const override;
  virtual bool IsActive() const override;

  virtual bool Load(const CDM::ExerciseData& in);

  virtual bool HasGenericExercise() const;
  virtual bool HasCyclingExercise() const;
  virtual bool HasRunningExercise() const;
  virtual bool HasStrengthExercise() const;

  virtual CDM::ExerciseData* Unload() const override;

  virtual ExerciseType GetExerciseType() const;
  virtual SEGeneric GetGenericExercise() const;
  virtual SECycling GetCyclingExercise() const;
  virtual SERunning GetRunningExercise() const;
  virtual SEStrengthTraining GetStrengthExercise() const;

  virtual SEGeneric& GetGenericExercise();
  virtual SECycling& GetCyclingExercise();
  virtual SERunning& GetRunningExercise();
  virtual SEStrengthTraining& GetStrengthExercise();

  virtual void SetGenericExercise(SEGeneric exercise);
  virtual void SetRunningExercise(SERunning exercise);
  virtual void SetCyclingExercise(SECycling exercise);
  virtual void SetStrengthExercise(SEStrengthTraining exercise);

  virtual void ToString(std::ostream& str) const override;

protected:
  virtual void Unload(CDM::ExerciseData& data) const;
  virtual bool LoadGeneric(const CDM::GenericExerciseData& in);
  virtual bool LoadCycling(const CDM::CyclingExerciseData& in);
  virtual bool LoadRunning(const CDM::RunningExerciseData& in);
  virtual bool LoadStrength(const CDM::StrengthExerciseData& in);

private:
  ExerciseType m_mode;
  SEGeneric m_genericExercise;
  SECycling m_cyclingExercise;
  SERunning m_runningExercise;
  SEStrengthTraining m_strengthExercise;
};
}
