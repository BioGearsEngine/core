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
  : SEPatientAction()
{
  m_Intensity = nullptr;
  m_DesiredWorkRate = nullptr;
  m_SpeedRun = nullptr;
  m_InclineRun = nullptr;
  m_AddedWeight = nullptr;
  m_CadenceCycle = nullptr;
  m_PowerCycle = nullptr;
  m_WeightStrength = nullptr;
  m_RepsStrength = nullptr;
}

SEExercise::~SEExercise()
{
  Clear();
}

void SEExercise::Clear()
{
  SEPatientAction::Clear();
  SAFE_DELETE(m_Intensity);
  SAFE_DELETE(m_DesiredWorkRate)
  SAFE_DELETE(m_SpeedRun);
  SAFE_DELETE(m_InclineRun);
  SAFE_DELETE(m_AddedWeight);
  SAFE_DELETE(m_CadenceCycle);
  SAFE_DELETE(m_PowerCycle);
  SAFE_DELETE(m_WeightStrength);
  SAFE_DELETE(m_RepsStrength);
}

bool SEExercise::IsValid() const
{
    return SEPatientAction::IsValid() 
      && (HasGenericExercise() 
      || HasCyclingExercise() 
      || HasRunningExercise()
      || HasStrengthExercise());
}

bool SEExercise::IsActive() const
{
    return IsValid();
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

bool SEExercise::LoadGeneric(const CDM::GenericExerciseData& in) {
  SEPatientAction::Load(in);
  if (in.Intensity().present()) {
    GetIntensity().Load(in.Intensity().get());
  } else if (in.DesiredWorkRate().present()) {
    GetDesiredWorkRate().Load(in.DesiredWorkRate().get());
  }
  return true;
}

bool SEExercise::LoadCycling(const CDM::CyclingExerciseData& in)
{
  SEPatientAction::Load(in);
  GetCadence().Load(in.Cadence());
  GetPower().Load(in.Power());
  if (in.AddedWeight().present()) {
    GetAddedWeight().Load(in.AddedWeight().get());
  }
  return true;
}

bool SEExercise::LoadRunning(const CDM::RunningExerciseData& in)
{
  SEPatientAction::Load(in);
  GetSpeed().Load(in.Speed());
  GetIncline().Load(in.Incline());
  if (in.AddedWeight().present()) {
    GetAddedWeight().Load(in.AddedWeight().get());
  }
  return true;
}

bool SEExercise::LoadStrength(const CDM::StrengthExerciseData& in)
{
  SEPatientAction::Load(in);
  GetWeight().Load(in.Weight());
  GetRepetitions().Load(in.Repetitions());
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
    if (m_Intensity != nullptr) {
      data.GenericExercise()->Intensity(std::unique_ptr<CDM::Scalar0To1Data>(m_Intensity->Unload()));
    } else if (m_DesiredWorkRate != nullptr) {
      data.GenericExercise()->DesiredWorkRate(std::unique_ptr<CDM::ScalarPowerData>(m_DesiredWorkRate->Unload()));
    }
  } else if (HasCyclingExercise()) {
    data.CyclingExercise(std::make_unique<CDM::ExerciseData::CyclingExercise_type>(
std::unique_ptr<CDM::ExerciseData::CyclingExercise_type::Cadence_type>(),
std::unique_ptr<CDM::ExerciseData::CyclingExercise_type::Power_type>()
));
    data.CyclingExercise()->Cadence(std::unique_ptr<CDM::ScalarFrequencyData>(m_CadenceCycle->Unload()));
    data.CyclingExercise()->Power(std::unique_ptr<CDM::ScalarPowerData>(m_PowerCycle->Unload()));
    if (m_AddedWeight != nullptr) {
      data.CyclingExercise()->AddedWeight(std::unique_ptr<CDM::ScalarMassData>(m_AddedWeight->Unload()));
    }
  } else if (HasRunningExercise()) {
    data.RunningExercise(std::make_unique<CDM::ExerciseData::RunningExercise_type>(
      std::unique_ptr<CDM::ExerciseData::RunningExercise_type::Speed_type>(),
      std::unique_ptr<CDM::ExerciseData::RunningExercise_type::Incline_type>()));
    data.RunningExercise()->Speed(std::unique_ptr<CDM::ScalarLengthPerTimeData>(m_SpeedRun->Unload()));
    data.RunningExercise()->Incline(std::unique_ptr<CDM::ScalarFractionData>(m_InclineRun->Unload()));
    if (m_AddedWeight != nullptr) {
      data.RunningExercise()->AddedWeight(std::unique_ptr<CDM::ScalarMassData>(m_AddedWeight->Unload()));
    }
  } else if (HasStrengthExercise()) {
    data.StrengthExercise(std::make_unique<CDM::ExerciseData::StrengthExercise_type>(
      std::unique_ptr<CDM::ExerciseData::StrengthExercise_type::Weight_type>(),
      std::unique_ptr<CDM::ExerciseData::StrengthExercise_type::Repetitions_type>()));
    data.StrengthExercise()->Weight(std::unique_ptr<CDM::ScalarMassData>(m_WeightStrength->Unload()));
    data.StrengthExercise()->Repetitions(std::unique_ptr<CDM::ScalarData>(m_RepsStrength->Unload()));
  }
}

bool SEExercise::HasGenericExercise() const
{
  return (HasIntensity() || HasDesiredWorkRate()) ? true : false;
}

bool SEExercise::HasCyclingExercise() const
{
  return (HasCadence() && HasPower()) ? true : false;
}

bool SEExercise::HasRunningExercise() const
{
  return (HasSpeed() && HasIncline()) ? true : false;
}

bool SEExercise::HasStrengthExercise() const
{
  return (HasWeight() && HasRepetitions()) ? true : false;
}

bool SEExercise::HasIntensity() const
{
  return m_Intensity == nullptr ? false : m_Intensity->IsValid();
}
SEScalar0To1& SEExercise::GetIntensity()
{
  if (m_Intensity == nullptr)
    m_Intensity = new SEScalar0To1();
  return *m_Intensity;
}

bool SEExercise::HasDesiredWorkRate() const
{
  return m_DesiredWorkRate == nullptr ? false : m_DesiredWorkRate->IsValid();
}
SEScalarPower& SEExercise::GetDesiredWorkRate()
{
  if (m_DesiredWorkRate == nullptr)
    m_DesiredWorkRate = new SEScalarPower();
  return *m_DesiredWorkRate;
}

bool SEExercise::HasCadence() const
{
  return m_CadenceCycle == nullptr ? false : m_CadenceCycle->IsValid();
}
SEScalarFrequency& SEExercise::GetCadence()
{
  if (m_CadenceCycle == nullptr)
    m_CadenceCycle = new SEScalarFrequency();
  return *m_CadenceCycle;
}

bool SEExercise::HasPower() const
{
  return m_PowerCycle == nullptr ? false : m_PowerCycle->IsValid();
}
SEScalarPower& SEExercise::GetPower()
{
  if (m_PowerCycle == nullptr)
    m_PowerCycle = new SEScalarPower();
  return *m_PowerCycle;
}

bool SEExercise::HasAddedWeight() const
{
  return m_AddedWeight == nullptr ? false : m_AddedWeight->IsValid();
}
SEScalarMass& SEExercise::GetAddedWeight()
{
  if (m_AddedWeight == nullptr)
    m_AddedWeight = new SEScalarMass();
  return *m_AddedWeight;
}

bool SEExercise::HasSpeed() const
{
  return m_SpeedRun == nullptr ? false : m_SpeedRun->IsValid();
}
SEScalarLengthPerTime& SEExercise::GetSpeed()
{
  if (m_SpeedRun == nullptr)
    m_SpeedRun = new SEScalarLengthPerTime();
  return *m_SpeedRun;
}

bool SEExercise::HasIncline() const
{
  return m_InclineRun == nullptr ? false : m_InclineRun->IsValid();
}
SEScalarFraction& SEExercise::GetIncline()
{
  if (m_InclineRun == nullptr)
    m_InclineRun = new SEScalarFraction();
  return *m_InclineRun;
}

bool SEExercise::HasWeight() const
{
  return m_WeightStrength == nullptr ? false : m_WeightStrength->IsValid();
}
SEScalarMass& SEExercise::GetWeight()
{
  if (m_WeightStrength == nullptr)
    m_WeightStrength = new SEScalarMass();
  return *m_WeightStrength;
}

bool SEExercise::HasRepetitions() const
{
  return m_RepsStrength == nullptr ? false : m_RepsStrength->IsValid();
}
SEScalar& SEExercise::GetRepetitions()
{
  if (m_RepsStrength == nullptr)
    m_RepsStrength = new SEScalar();
  return *m_RepsStrength;
}

void SEExercise::ToString(std::ostream& str) const
{
  str << "Patient Action : Exercise";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  if (HasGenericExercise()) {
    str << "\n\tGeneric Exercise";
    str << "\n\tIntensity: ";
    HasIntensity() ? str << *m_Intensity : str << "NaN";
    str << "\n\tDesired Work Rate: ";
    HasDesiredWorkRate() ? str << *m_DesiredWorkRate : str << "NaN";
  } else if (HasCyclingExercise()) {
    str << "\n\tCycling Exercise";
    str << "\n\tCadence: ";
    HasCadence() ? str << *m_CadenceCycle : str << "NaN";
    str << "\n\tPower: ";
    HasPower() ? str << *m_PowerCycle : str << "NaN";
    if (HasAddedWeight()) {
      str << "\n\tAdded Weight: ";
     str << *m_AddedWeight;
    }
  } else if (HasRunningExercise()) {
    str << "\n\tRunning Exercise";
    str << "\n\tSpeed: ";
    HasSpeed() ? str << *m_SpeedRun : str << "NaN";
    str << "\n\tIncline: ";
    HasIncline() ? str << *m_InclineRun : str << "NaN";
    if (HasAddedWeight()) {
      str << "\n\tAdded Weight: ";
      str << *m_AddedWeight;
    }
  } else if (HasStrengthExercise()) {
    str << "\n\tStrength Exercise";
    str << "\n\tWeight: ";
    HasWeight() ? str << *m_WeightStrength : str << "NaN";
    str << "\n\tRepetitions: ";
    HasRepetitions() ? str << *m_RepsStrength : str << "NaN";
  } else {
    str << "No input for exercise";
  }
  str << std::flush;
}
}