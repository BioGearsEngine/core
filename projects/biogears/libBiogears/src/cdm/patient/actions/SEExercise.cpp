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
#include <biogears/cdm/patient/actions/SEExercise.h>

#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarLengthPerTime.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarPower.h>

namespace biogears {
SEExercise::SEExercise()
  : m_mode(NONE)
{
  
}
SEExercise::SEExercise(SEGeneric ex)
  : m_mode(GENERIC)
  , m_genericExercise(ex)
{
}
SEExercise::SEExercise(SECycling ex)
  : m_mode(CYCLING)
  , m_cyclingExercise(ex)
{
}
SEExercise::SEExercise(SERunning ex)
  : m_mode(RUNNING)
  , m_runningExercise(ex)
{
}
SEExercise::SEExercise::SEExercise(SEStrengthTraining ex)
  : m_mode(STRENGTH_TRAINING)
  , m_strengthExercise(ex)
{
}

SEExercise::~SEExercise()
{
}

void SEExercise::Clear()
{
  m_mode = NONE;
  SEPatientAction::Clear();

  SEGeneric gen;
  gen.Intensity.Clear();
  gen.DesiredWorkRate.Clear();
  SECycling cyc;
  cyc.CadenceCycle.Clear();
  cyc.PowerCycle.Clear();
  cyc.AddedWeight.Clear();
  SERunning run;
  run.SpeedRun.Clear();
  run.InclineRun.Clear();
  run.AddedWeight.Clear();
  SEStrengthTraining str;
  str.WeightStrength.Clear();
  str.RepsStrength.Clear();
}

bool SEExercise::IsValid() const
{
  return SEPatientAction::IsValid()
    && m_mode != NONE;
}

bool SEExercise::IsActive() const
{
  if (HasGenericExercise()) {
    if (m_genericExercise.Intensity.IsValid()) {
      return !m_genericExercise.Intensity.IsZero();
    } else {
      return !m_genericExercise.DesiredWorkRate.IsZero();
    }
  } else if (HasCyclingExercise()) {
    return (!m_cyclingExercise.CadenceCycle.IsZero() && !m_cyclingExercise.PowerCycle.IsZero());
  } else if (HasRunningExercise()) {
    return (!m_runningExercise.SpeedRun.IsZero());
  } else if (HasStrengthExercise()) {
    return (!m_strengthExercise.WeightStrength.IsZero() && !m_strengthExercise.RepsStrength.IsZero());
  } else {
    return IsValid();
  }
}


bool SEExercise::Load(const CDM::ExerciseData& in)
{
  SEPatientAction::Load(in);
  if (in.GenericExercise().present()) {
    LoadGeneric(in.GenericExercise().get());
  } else if (in.CyclingExercise().present()) {
    LoadCycling(in.CyclingExercise().get());
  } else if (in.RunningExercise().present()) {
    LoadRunning(in.RunningExercise().get());
  } else if (in.StrengthExercise().present()) {
    LoadStrength(in.StrengthExercise().get());
  }
  return true;
}

SEExercise::ExerciseType SEExercise::GetExerciseType() const
{
  return m_mode;
}

bool SEExercise::LoadGeneric(const CDM::GenericExerciseData& in) 
{
  SEPatientAction::Load(in);
  m_mode = GENERIC;
  if (in.Intensity().present()) {
    m_genericExercise.Intensity.Load(in.Intensity().get());
  } else if (in.DesiredWorkRate().present()) {
    m_genericExercise.DesiredWorkRate.Load(in.DesiredWorkRate().get());
  }
  return true;
}

bool SEExercise::LoadCycling(const CDM::CyclingExerciseData& in)
{
  SEPatientAction::Load(in);
  m_mode = CYCLING;
  m_cyclingExercise.CadenceCycle.Load(in.Cadence());
  m_cyclingExercise.PowerCycle.Load(in.Power());
  if (in.AddedWeight().present()) {
    m_cyclingExercise.AddedWeight.Load(in.AddedWeight().get());
  } else {
    m_cyclingExercise.AddedWeight.SetValue(0, MassUnit::kg);
  }
  return true;
}

bool SEExercise::LoadRunning(const CDM::RunningExerciseData& in)
{
  SEPatientAction::Load(in);
  m_mode = RUNNING;
  m_runningExercise.SpeedRun.Load(in.Speed());
  m_runningExercise.InclineRun.Load(in.Incline());
  if (in.AddedWeight().present()) {
    m_runningExercise.AddedWeight.Load(in.AddedWeight().get());
  } else {
    m_runningExercise.AddedWeight.SetValue(0, MassUnit::kg);
  }
  return true;
}

bool SEExercise::LoadStrength(const CDM::StrengthExerciseData& in)
{
  SEPatientAction::Load(in);
  m_mode = STRENGTH_TRAINING;
  m_strengthExercise.WeightStrength.Load(in.Weight());
  m_strengthExercise.RepsStrength.Load(in.Repetitions());
  return true;
}

CDM::ExerciseData* SEExercise::Unload() const
{
  CDM::ExerciseData* data(new CDM::ExerciseData());
  Unload(*data);
  return data;
}

void SEExercise::Unload(CDM::ExerciseData& data) const
{
  SEPatientAction::Unload(data);
  if (HasGenericExercise()) {
    data.GenericExercise(std::make_unique<CDM::ExerciseData::GenericExercise_type>());
    if (m_genericExercise.Intensity.IsValid()) {
      data.GenericExercise()->Intensity(std::unique_ptr<CDM::Scalar0To1Data>(m_genericExercise.Intensity.Unload()));
    } else if (m_genericExercise.DesiredWorkRate.IsValid()) {
      data.GenericExercise()->DesiredWorkRate(std::unique_ptr<CDM::ScalarPowerData>(m_genericExercise.DesiredWorkRate.Unload()));
    }
  } else if (HasCyclingExercise()) {
    data.CyclingExercise(std::make_unique<CDM::ExerciseData::CyclingExercise_type>(
std::unique_ptr<CDM::ExerciseData::CyclingExercise_type::Cadence_type>(),
std::unique_ptr<CDM::ExerciseData::CyclingExercise_type::Power_type>()
));
    data.CyclingExercise()->Cadence(std::unique_ptr<CDM::ScalarFrequencyData>(m_cyclingExercise.CadenceCycle.Unload()));
    data.CyclingExercise()->Power(std::unique_ptr<CDM::ScalarPowerData>(m_cyclingExercise.PowerCycle.Unload()));
    if (m_cyclingExercise.AddedWeight.IsValid()) {
      data.CyclingExercise()->AddedWeight(std::unique_ptr<CDM::ScalarMassData>(m_cyclingExercise.AddedWeight.Unload()));
    }
  } else if (HasRunningExercise()) {
    data.RunningExercise(std::make_unique<CDM::ExerciseData::RunningExercise_type>(
      std::unique_ptr<CDM::ExerciseData::RunningExercise_type::Speed_type>(),
      std::unique_ptr<CDM::ExerciseData::RunningExercise_type::Incline_type>()));
    data.RunningExercise()->Speed(std::unique_ptr<CDM::ScalarLengthPerTimeData>(m_runningExercise.SpeedRun.Unload()));
    data.RunningExercise()->Incline(std::unique_ptr<CDM::Scalar0To1Data>(m_runningExercise.InclineRun.Unload()));
    if (m_runningExercise.AddedWeight.IsValid()) {
      data.RunningExercise()->AddedWeight(std::unique_ptr<CDM::ScalarMassData>(m_runningExercise.AddedWeight.Unload()));
    }
  } else if (HasStrengthExercise()) {
    data.StrengthExercise(std::make_unique<CDM::ExerciseData::StrengthExercise_type>(
      std::unique_ptr<CDM::ExerciseData::StrengthExercise_type::Weight_type>(),
      std::unique_ptr<CDM::ExerciseData::StrengthExercise_type::Repetitions_type>()));
    data.StrengthExercise()->Weight(std::unique_ptr<CDM::ScalarMassData>(m_strengthExercise.WeightStrength.Unload()));
    data.StrengthExercise()->Repetitions(std::unique_ptr<CDM::ScalarData>(m_strengthExercise.RepsStrength.Unload()));
  }
}

bool SEExercise::HasGenericExercise() const { return m_mode == GENERIC; };
bool SEExercise::HasRunningExercise() const { return m_mode == RUNNING; };
bool SEExercise::HasCyclingExercise() const { return m_mode == CYCLING; };
bool SEExercise::HasStrengthExercise() const { return m_mode == STRENGTH_TRAINING; };

SEExercise::SEGeneric& SEExercise::GetGenericExercise() {
  return m_genericExercise;
}
SEExercise::SERunning& SEExercise::GetRunningExercise() {
  return m_runningExercise;
}
SEExercise::SECycling& SEExercise::GetCyclingExercise() {
  return m_cyclingExercise;
}
SEExercise::SEStrengthTraining& SEExercise::GetStrengthExercise() {
  return m_strengthExercise;
}
SEExercise::SEGeneric SEExercise::GetGenericExercise() const
{
  return m_genericExercise;
}
SEExercise::SERunning SEExercise::GetRunningExercise() const {
  return m_runningExercise;
}
SEExercise::SECycling SEExercise::GetCyclingExercise() const {
  return m_cyclingExercise;
}
SEExercise::SEStrengthTraining SEExercise::GetStrengthExercise() const {
  return m_strengthExercise;
}
void SEExercise::SetGenericExercise(SEGeneric exercise)
{
  m_mode = GENERIC;
  m_genericExercise = exercise;
}
void SEExercise::SetRunningExercise(SERunning exercise)
{
  m_mode = RUNNING;
  m_runningExercise = exercise;
}
void SEExercise::SetCyclingExercise(SECycling exercise)
{
  m_mode = CYCLING;
  m_cyclingExercise = exercise;
}
void SEExercise::SetStrengthExercise(SEStrengthTraining exercise)
{
  m_mode = STRENGTH_TRAINING;
  m_strengthExercise = exercise;
}

void SEExercise::ToString(std::ostream& str) const
{
  str << "Patient Action : Exercise";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  if (HasGenericExercise()) {
    str << "\n\tGeneric Exercise";
    str << "\n\tIntensity: ";
    (m_genericExercise.Intensity.IsValid()) ? str << m_genericExercise.Intensity : str << "NaN";
    str << "\n\tDesired Work Rate: ";
    (m_genericExercise.DesiredWorkRate.IsValid()) ? str << m_genericExercise.DesiredWorkRate : str << "NaN";

  } else if (HasCyclingExercise()) {
    str << "\n\tCycling Exercise";
    str << "\n\tCadence: ";
    (m_cyclingExercise.CadenceCycle.IsValid()) ? str << m_cyclingExercise.CadenceCycle : str << "NaN";
    str << "\n\tPower: ";
    (m_cyclingExercise.PowerCycle.IsValid()) ? str << m_cyclingExercise.PowerCycle : str << "NaN";
    if (m_cyclingExercise.AddedWeight.IsValid()) {
      str << "\n\tAdded Weight: ";
     str << m_cyclingExercise.AddedWeight;
    }

  } else if (HasRunningExercise()) {
    str << "\n\tRunning Exercise";
    str << "\n\tSpeed: ";
    (m_runningExercise.SpeedRun.IsValid()) ? str << m_runningExercise.SpeedRun : str << "NaN";
    str << "\n\tIncline: ";
    (m_runningExercise.InclineRun.IsValid()) ? str << m_runningExercise.InclineRun : str << "NaN";
    if (m_runningExercise.AddedWeight.IsValid()) {
      str << "\n\tAdded Weight: ";
      str << m_runningExercise.AddedWeight;
    }

  } else if (HasStrengthExercise()) {
    str << "\n\tStrength Exercise";
    str << "\n\tWeight: ";
    (m_strengthExercise.WeightStrength.IsValid()) ? str << m_strengthExercise.WeightStrength : str << "NaN";
    str << "\n\tRepetitions: ";
    (m_strengthExercise.RepsStrength.IsValid()) ? str << m_strengthExercise.RepsStrength : str << "NaN";
  } else {
    str << "No input for exercise";
  }
  str << std::flush;
}
}