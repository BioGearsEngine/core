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

#include <biogears/cdm/stdafx.h>
#include <biogears/cdm/patient/actions/SEExercise.h>
#include <biogears/schema/ExerciseData.hxx>
#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/schema/Scalar0To1Data.hxx>

SEExercise::SEExercise() : SEPatientAction()
{
	m_Intensity = nullptr;
}

SEExercise::~SEExercise()
{
	Clear();
}

void SEExercise::Clear()
{
	SEPatientAction::Clear();
	SAFE_DELETE(m_Intensity);
}

bool SEExercise::IsValid() const
{
	return SEPatientAction::IsValid() && HasIntensity();
}

bool SEExercise::IsActive() const
{
	if (HasIntensity())
		return m_Intensity->IsPositive();
	return false;	
}

bool SEExercise::Load(const CDM::ExerciseData& in)
{
	SEPatientAction::Load(in);
	GetIntensity().Load(in.Intensity());
	return true;
}

CDM::ExerciseData* SEExercise::Unload() const
{
	CDM::ExerciseData*data(new CDM::ExerciseData());
	Unload(*data);
	return data;
}

void SEExercise::Unload(CDM::ExerciseData& data) const
{
	SEPatientAction::Unload(data);
	if (m_Intensity != nullptr)
		data.Intensity(std::unique_ptr<CDM::Scalar0To1Data>(m_Intensity->Unload()));
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

void SEExercise::ToString(std::ostream &str) const
{
	str << "Patient Action : Exercise";
	if (HasComment())
		str << "\n\tComment: " << m_Comment;
	str << "\n\tIntensity: "; HasIntensity() ? str << *m_Intensity : str << "NaN";
	str << std::flush;
}
