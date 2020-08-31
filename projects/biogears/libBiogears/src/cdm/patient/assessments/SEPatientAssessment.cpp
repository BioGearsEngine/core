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
#include <biogears/cdm/patient/assessments/SEPatientAssessment.h>

#include <biogears/schema/cdm/PatientAssessments.hxx>

namespace biogears {
SEPatientAssessment::SEPatientAssessment()
{
}

SEPatientAssessment::~SEPatientAssessment()
{
  Clear();
}

void SEPatientAssessment::Clear()
{
}

void SEPatientAssessment::Reset()
{
}

bool SEPatientAssessment::Load(const CDM::PatientAssessmentData& in)
{
  return true;
}

CDM::PatientAssessmentData* SEPatientAssessment::Unload()
{
  CDM::PatientAssessmentData* data = new CDM::PatientAssessmentData();
  Unload(*data);
  return data;
}

void SEPatientAssessment::Unload(CDM::PatientAssessmentData& data)
{
}
}