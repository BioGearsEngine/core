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

#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarLengthPerTime.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarPower.h>

// Private Includes
#include <io/cdm/PatientActions.h>

namespace biogears {
SEExercise::SEExercise()
  : m_mode(NONE)
{
}
//-------------------------------------------------------------------------------
SEExercise::SEExercise(SEGeneric ex)
  : m_mode(GENERIC)
  , m_genericExercise(ex)
{
}
//-------------------------------------------------------------------------------
SEExercise::SEExercise(SECycling ex)
  : m_mode(CYCLING)
  , m_cyclingExercise(ex)
{
}
//-------------------------------------------------------------------------------
SEExercise::SEExercise(SERunning ex)
  : m_mode(RUNNING)
  , m_runningExercise(ex)
{
}
//-------------------------------------------------------------------------------
SEExercise::SEExercise::SEExercise(SEStrengthTraining ex)
  : m_mode(STRENGTH_TRAINING)
  , m_strengthExercise(ex)
{
}
//-------------------------------------------------------------------------------
SEExercise::~SEExercise()
{
}
//-------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------
bool SEExercise::IsValid() const
{
  return SEPatientAction::IsValid()
    && m_mode != NONE;
}
//-------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------
bool SEExercise::Load(const CDM::ExerciseData& in)
{
  io::PatientActions::UnMarshall(in, *this);
  return true;
}
//-------------------------------------------------------------------------------
SEExercise::ExerciseType SEExercise::GetExerciseType() const
{
  return m_mode;
}
//-------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------
bool SEExercise::LoadStrength(const CDM::StrengthExerciseData& in)
{
  SEPatientAction::Load(in);
  m_mode = STRENGTH_TRAINING;
  m_strengthExercise.WeightStrength.Load(in.Weight());
  m_strengthExercise.RepsStrength.Load(in.Repetitions());
  return true;
}
//-------------------------------------------------------------------------------
CDM::ExerciseData* SEExercise::Unload() const
{
  CDM::ExerciseData* data(new CDM::ExerciseData());
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SEExercise::Unload(CDM::ExerciseData& data) const
{
  io::PatientActions::Marshall(*this, data);
}
//-------------------------------------------------------------------------------
bool SEExercise::HasGenericExercise() const { return m_mode == GENERIC; };
//-------------------------------------------------------------------------------
bool SEExercise::HasRunningExercise() const { return m_mode == RUNNING; };
//-------------------------------------------------------------------------------
bool SEExercise::HasCyclingExercise() const { return m_mode == CYCLING; };
//-------------------------------------------------------------------------------
bool SEExercise::HasStrengthExercise() const { return m_mode == STRENGTH_TRAINING; };
//-------------------------------------------------------------------------------
SEExercise::SEGeneric& SEExercise::GetGenericExercise()
{
  return m_genericExercise;
}
//-------------------------------------------------------------------------------
SEExercise::SERunning& SEExercise::GetRunningExercise()
{
  return m_runningExercise;
}
//-------------------------------------------------------------------------------
SEExercise::SECycling& SEExercise::GetCyclingExercise()
{
  return m_cyclingExercise;
}
//-------------------------------------------------------------------------------
SEExercise::SEStrengthTraining& SEExercise::GetStrengthExercise()
{
  return m_strengthExercise;
}
//-------------------------------------------------------------------------------
SEExercise::SEGeneric SEExercise::GetGenericExercise() const
{
  return m_genericExercise;
}
//-------------------------------------------------------------------------------
SEExercise::SERunning SEExercise::GetRunningExercise() const
{
  return m_runningExercise;
}
//-------------------------------------------------------------------------------
SEExercise::SECycling SEExercise::GetCyclingExercise() const
{
  return m_cyclingExercise;
}
//-------------------------------------------------------------------------------
SEExercise::SEStrengthTraining SEExercise::GetStrengthExercise() const
{
  return m_strengthExercise;
}
//-------------------------------------------------------------------------------
void SEExercise::SetGenericExercise(SEGeneric exercise)
{
  m_mode = GENERIC;
  m_genericExercise = exercise;
}
//-------------------------------------------------------------------------------
void SEExercise::SetRunningExercise(SERunning exercise)
{
  m_mode = RUNNING;
  m_runningExercise = exercise;
}
//-------------------------------------------------------------------------------
void SEExercise::SetCyclingExercise(SECycling exercise)
{
  m_mode = CYCLING;
  m_cyclingExercise = exercise;
}
//-------------------------------------------------------------------------------
void SEExercise::SetStrengthExercise(SEStrengthTraining exercise)
{
  m_mode = STRENGTH_TRAINING;
  m_strengthExercise = exercise;
}
//-------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------
bool SEExercise::SEGeneric::operator==(const SEGeneric& rhs) const
{
  return DesiredWorkRate == rhs.DesiredWorkRate
    && Intensity == rhs.Intensity;
}
//-------------------------------------------------------------------------------
bool SEExercise::SEGeneric::operator!=(const SEGeneric& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
bool SEExercise::SECycling::operator==(const SECycling& rhs) const
{

  return CadenceCycle == rhs.CadenceCycle
    && PowerCycle == rhs.PowerCycle
    && AddedWeight == rhs.AddedWeight;
}
//-------------------------------------------------------------------------------
bool SEExercise::SECycling::operator!=(const SECycling& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
bool SEExercise::SERunning::operator==(const SERunning& rhs) const
{

  return SpeedRun == rhs.SpeedRun
    && InclineRun == rhs.InclineRun
    && AddedWeight == rhs.AddedWeight;
}
//-------------------------------------------------------------------------------
bool SEExercise::SERunning::operator!=(const SERunning& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
bool SEExercise::SEStrengthTraining::operator==(const SEStrengthTraining& rhs) const
{
  return WeightStrength == rhs.WeightStrength
    && RepsStrength == rhs.RepsStrength;
}
//-------------------------------------------------------------------------------
bool SEExercise::SEStrengthTraining::operator!=(const SEStrengthTraining& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
bool SEExercise::operator==(const SEExercise& rhs) const
{
  bool equivilant = m_Comment == rhs.m_Comment;
  equivilant &= m_mode == rhs.m_mode;
  if (equivilant) {
    switch (m_mode) {
    case SEExercise::GENERIC:
      equivilant &= m_genericExercise == rhs.m_genericExercise;
      break;
    case SEExercise::CYCLING:
      equivilant &= m_cyclingExercise == rhs.m_cyclingExercise;
      break;
    case SEExercise::RUNNING:
      equivilant &= m_runningExercise == rhs.m_runningExercise;
      break;
    case SEExercise::STRENGTH_TRAINING:
      equivilant &= m_strengthExercise == rhs.m_strengthExercise;
      break;
    default:
      equivilant = false;
    }
  }
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SEExercise::operator!=(const SEExercise& rhs) const
{
  return !(*this == rhs);
}
}