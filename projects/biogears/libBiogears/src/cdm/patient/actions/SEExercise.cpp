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
#include <biogears/cdm/properties/SEScalarFraction.h>
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
  // May want to consider clearing each struct here
}

bool SEExercise::IsValid() const
{
  return SEPatientAction::IsValid()
    && m_mode != NONE;
}

bool SEExercise::IsActive() const
{
  if (HasGenericExercise()) {
    if (m_genericExercise.m_Intensity.IsValid()) {
      return !m_genericExercise.m_Intensity.IsZero();
    } else {
      return !m_genericExercise.m_DesiredWorkRate.IsZero();
    }
  } else if (HasCyclingExercise()) {
    return (!m_cyclingExercise.m_CadenceCycle.IsZero() && !m_cyclingExercise.m_PowerCycle.IsZero());
  } else if (HasRunningExercise()) {
    return (!m_runningExercise.m_SpeedRun.IsZero());
  } else if (HasStrengthExercise()) {
    return (!m_strengthExercise.m_WeightStrength.IsZero() && !m_strengthExercise.m_RepsStrength.IsZero());
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
    m_genericExercise.m_Intensity.Load(in.Intensity().get());
  } else if (in.DesiredWorkRate().present()) {
    m_genericExercise.m_Intensity.Load(in.DesiredWorkRate().get());
  }
  return true;
}

bool SEExercise::LoadCycling(const CDM::CyclingExerciseData& in)
{
  SEPatientAction::Load(in);
  m_mode = CYCLING;
  m_cyclingExercise.m_CadenceCycle.Load(in.Cadence());
  m_cyclingExercise.m_PowerCycle.Load(in.Power());
  if (in.AddedWeight().present()) {
    m_cyclingExercise.m_AddedWeight.Load(in.AddedWeight().get());
  } else {
    m_cyclingExercise.m_AddedWeight.SetValue(0, MassUnit::kg);
  }
  return true;
}

bool SEExercise::LoadRunning(const CDM::RunningExerciseData& in)
{
  SEPatientAction::Load(in);
  m_mode = RUNNING;
  m_runningExercise.m_SpeedRun.Load(in.Speed());
  m_runningExercise.m_InclineRun.Load(in.Incline());
  if (in.AddedWeight().present()) {
    m_runningExercise.m_AddedWeight.Load(in.AddedWeight().get());
  } else {
    m_runningExercise.m_AddedWeight.SetValue(0, MassUnit::kg);
  }
  return true;
}

bool SEExercise::LoadStrength(const CDM::StrengthExerciseData& in)
{
  SEPatientAction::Load(in);
  m_mode = STRENGTH_TRAINING;
  m_strengthExercise.m_WeightStrength.Load(in.Weight());
  m_strengthExercise.m_RepsStrength.Load(in.Repetitions());
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
    if (m_genericExercise.m_Intensity.IsValid()) {
      data.GenericExercise()->Intensity(std::unique_ptr<CDM::Scalar0To1Data>(m_genericExercise.m_Intensity.Unload()));
    } else if (m_genericExercise.m_DesiredWorkRate.IsValid()) {
      data.GenericExercise()->DesiredWorkRate(std::unique_ptr<CDM::ScalarPowerData>(m_genericExercise.m_DesiredWorkRate.Unload()));
    }
  } else if (HasCyclingExercise()) {
    data.CyclingExercise(std::make_unique<CDM::ExerciseData::CyclingExercise_type>(
std::unique_ptr<CDM::ExerciseData::CyclingExercise_type::Cadence_type>(),
std::unique_ptr<CDM::ExerciseData::CyclingExercise_type::Power_type>()
));
    data.CyclingExercise()->Cadence(std::unique_ptr<CDM::ScalarFrequencyData>(m_cyclingExercise.m_CadenceCycle.Unload()));
    data.CyclingExercise()->Power(std::unique_ptr<CDM::ScalarPowerData>(m_cyclingExercise.m_PowerCycle.Unload()));
    if (m_cyclingExercise.m_AddedWeight.IsValid()) {
      data.CyclingExercise()->AddedWeight(std::unique_ptr<CDM::ScalarMassData>(m_cyclingExercise.m_AddedWeight.Unload()));
    }
  } else if (HasRunningExercise()) {
    data.RunningExercise(std::make_unique<CDM::ExerciseData::RunningExercise_type>(
      std::unique_ptr<CDM::ExerciseData::RunningExercise_type::Speed_type>(),
      std::unique_ptr<CDM::ExerciseData::RunningExercise_type::Incline_type>()));
    data.RunningExercise()->Speed(std::unique_ptr<CDM::ScalarLengthPerTimeData>(m_runningExercise.m_SpeedRun.Unload()));
    data.RunningExercise()->Incline(std::unique_ptr<CDM::ScalarFractionData>(m_runningExercise.m_InclineRun.Unload()));
    if (m_runningExercise.m_AddedWeight.IsValid()) {
      data.RunningExercise()->AddedWeight(std::unique_ptr<CDM::ScalarMassData>(m_runningExercise.m_AddedWeight.Unload()));
    }
  } else if (HasStrengthExercise()) {
    data.StrengthExercise(std::make_unique<CDM::ExerciseData::StrengthExercise_type>(
      std::unique_ptr<CDM::ExerciseData::StrengthExercise_type::Weight_type>(),
      std::unique_ptr<CDM::ExerciseData::StrengthExercise_type::Repetitions_type>()));
    data.StrengthExercise()->Weight(std::unique_ptr<CDM::ScalarMassData>(m_strengthExercise.m_WeightStrength.Unload()));
    data.StrengthExercise()->Repetitions(std::unique_ptr<CDM::ScalarData>(m_strengthExercise.m_RepsStrength.Unload()));
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

//bool SEExercise::HasIntensity() const
//{
//  return m_genericExercise.m_Intensity == nullptr ? false : m_genericExercise.m_Intensity->IsValid();
//}
//SEScalar0To1& SEExercise::GetIntensity()
//{
//  if (m_genericExercise.m_Intensity == nullptr)
//    m_genericExercise.m_Intensity = new SEScalar0To1();
//  return *m_genericExercise.m_Intensity;
//}
//
//bool SEExercise::HasDesiredWorkRate() const
//{
//  return m_genericExercise.m_DesiredWorkRate == nullptr ? false : m_genericExercise.m_DesiredWorkRate->IsValid();
//}
//SEScalarPower& SEExercise::GetDesiredWorkRate()
//{
//  if (m_genericExercise.m_DesiredWorkRate == nullptr)
//    m_genericExercise.m_DesiredWorkRate = new SEScalarPower();
//  return *m_genericExercise.m_DesiredWorkRate;
//}
//
//bool SEExercise::HasCadence() const
//{
//  return m_cyclingExercise.m_CadenceCycle == nullptr ? false : m_cyclingExercise.m_CadenceCycle->IsValid();
//}
//SEScalarFrequency& SEExercise::GetCadence()
//{
//  if (m_cyclingExercise.m_CadenceCycle == nullptr)
//    m_cyclingExercise.m_CadenceCycle = new SEScalarFrequency();
//  return *m_cyclingExercise.m_CadenceCycle;
//}
//
//bool SEExercise::HasPower() const
//{
//  return m_cyclingExercise.m_PowerCycle == nullptr ? false : m_cyclingExercise.m_PowerCycle->IsValid();
//}
//SEScalarPower& SEExercise::GetPower()
//{
//  if (m_cyclingExercise.m_PowerCycle == nullptr)
//    m_cyclingExercise.m_PowerCycle = new SEScalarPower();
//  return *m_cyclingExercise.m_PowerCycle;
//}
//
//bool SEExercise::HasAddedWeight() const
//{
//  if (HasCyclingExercise()) {
//    return m_cyclingExercise.m_AddedWeight == nullptr ? false : m_cyclingExercise.m_AddedWeight->IsValid();
//  } else if (HasRunningExercise()) {
//    return m_runningExercise.m_AddedWeight == nullptr ? false : m_runningExercise.m_AddedWeight->IsValid();
//  }
//}
//SEScalarMass& SEExercise::GetAddedWeight()
//{
//  if (HasCyclingExercise()) {
//    if (m_cyclingExercise.m_AddedWeight == nullptr)
//      m_cyclingExercise.m_AddedWeight = new SEScalarMass();
//    return *m_cyclingExercise.m_AddedWeight;
//  } else if (HasRunningExercise()) {
//  if (m_runningExercise.m_AddedWeight == nullptr)
//    m_runningExercise.m_AddedWeight = new SEScalarMass();
//  return *m_runningExercise.m_AddedWeight;
//  }
//}
//
//
//bool SEExercise::HasSpeed() const
//{
//  return m_runningExercise.m_SpeedRun == nullptr ? false : m_runningExercise.m_SpeedRun->IsValid();
//}
//SEScalarLengthPerTime& SEExercise::GetSpeed()
//{
//  if (m_runningExercise.m_SpeedRun == nullptr)
//    m_runningExercise.m_SpeedRun = new SEScalarLengthPerTime();
//  return *m_runningExercise.m_SpeedRun;
//}
//
//bool SEExercise::HasIncline() const
//{
//  return m_runningExercise.m_InclineRun == nullptr ? false : m_runningExercise.m_InclineRun->IsValid();
//}
//SEScalarFraction& SEExercise::GetIncline()
//{
//  if (m_runningExercise.m_InclineRun == nullptr)
//    m_runningExercise.m_InclineRun = new SEScalarFraction();
//  return *m_runningExercise.m_InclineRun;
//}
//
//bool SEExercise::HasWeight() const
//{
//  return m_strengthExercise.m_WeightStrength == nullptr ? false : m_strengthExercise.m_WeightStrength->IsValid();
//}
//SEScalarMass& SEExercise::GetWeight()
//{
//  if (m_strengthExercise.m_WeightStrength == nullptr)
//    m_strengthExercise.m_WeightStrength = new SEScalarMass();
//  return *m_strengthExercise.m_WeightStrength;
//}
//
//bool SEExercise::HasRepetitions() const
//{
//  return m_strengthExercise.m_RepsStrength == nullptr ? false : m_strengthExercise.m_RepsStrength->IsValid();
//}
//SEScalar& SEExercise::GetRepetitions()
//{
//  if (m_strengthExercise.m_RepsStrength == nullptr)
//    m_strengthExercise.m_RepsStrength = new SEScalar();
//  return *m_strengthExercise.m_RepsStrength;
//}

void SEExercise::ToString(std::ostream& str) const
{
  str << "Patient Action : Exercise";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  if (HasGenericExercise()) {
    str << "\n\tGeneric Exercise";
    str << "\n\tIntensity: ";
    (m_genericExercise.m_Intensity.IsValid()) ? str << m_genericExercise.m_Intensity : str << "NaN";
    str << "\n\tDesired Work Rate: ";
    (m_genericExercise.m_DesiredWorkRate.IsValid()) ? str << m_genericExercise.m_DesiredWorkRate : str << "NaN";

  } else if (HasCyclingExercise()) {
    str << "\n\tCycling Exercise";
    str << "\n\tCadence: ";
    (m_cyclingExercise.m_CadenceCycle.IsValid()) ? str << m_cyclingExercise.m_CadenceCycle : str << "NaN";
    str << "\n\tPower: ";
    (m_cyclingExercise.m_PowerCycle.IsValid()) ? str << m_cyclingExercise.m_PowerCycle : str << "NaN";
    if (m_cyclingExercise.m_AddedWeight.IsValid()) {
      str << "\n\tAdded Weight: ";
     str << m_cyclingExercise.m_AddedWeight;
    }

  } else if (HasRunningExercise()) {
    str << "\n\tRunning Exercise";
    str << "\n\tSpeed: ";
    (m_runningExercise.m_SpeedRun.IsValid()) ? str << m_runningExercise.m_SpeedRun : str << "NaN";
    str << "\n\tIncline: ";
    (m_runningExercise.m_InclineRun.IsValid()) ? str << m_runningExercise.m_InclineRun : str << "NaN";
    if (m_runningExercise.m_AddedWeight.IsValid()) {
      str << "\n\tAdded Weight: ";
      str << m_runningExercise.m_AddedWeight;
    }

  } else if (HasStrengthExercise()) {
    str << "\n\tStrength Exercise";
    str << "\n\tWeight: ";
    (m_strengthExercise.m_WeightStrength.IsValid()) ? str << m_strengthExercise.m_WeightStrength : str << "NaN";
    str << "\n\tRepetitions: ";
    (m_strengthExercise.m_RepsStrength.IsValid()) ? str << m_strengthExercise.m_RepsStrength : str << "NaN";
  } else {
    str << "No input for exercise";
  }
  str << std::flush;
}
}