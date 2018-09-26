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

#include <biogears/cdm/patient/actions/SEPatientAssessmentRequest.h>

namespace biogears {
SEPatientAssessmentRequest::SEPatientAssessmentRequest()
  : SEPatientAction()
{
  m_Type = (CDM::enumPatientAssessment::value)-1;
}

SEPatientAssessmentRequest::~SEPatientAssessmentRequest()
{
  Clear();
}

void SEPatientAssessmentRequest::Clear()
{
  SEPatientAction::Clear();
  m_Type = (CDM::enumPatientAssessment::value)-1;
}

bool SEPatientAssessmentRequest::IsValid() const
{
  return SEPatientAction::IsValid() && HasType();
}

bool SEPatientAssessmentRequest::IsActive() const
{
  return IsValid();
}

bool SEPatientAssessmentRequest::Load(const CDM::PatientAssessmentRequestData& in)
{
  SEPatientAction::Load(in);
  m_Type = in.Type();
  return true;
}

CDM::PatientAssessmentRequestData* SEPatientAssessmentRequest::Unload() const
{
  CDM::PatientAssessmentRequestData* data(new CDM::PatientAssessmentRequestData());
  Unload(*data);
  return data;
}

void SEPatientAssessmentRequest::Unload(CDM::PatientAssessmentRequestData& data) const
{
  SEPatientAction::Unload(data);
  if (HasType())
    data.Type(m_Type);
}

CDM::enumPatientAssessment::value SEPatientAssessmentRequest::GetType() const
{
  return m_Type;
}
void SEPatientAssessmentRequest::SetType(CDM::enumPatientAssessment::value Type)
{
  m_Type = Type;
}
bool SEPatientAssessmentRequest::HasType() const
{
  return m_Type == ((CDM::enumPatientAssessment::value)-1) ? false : true;
}
void SEPatientAssessmentRequest::InvalidateType()
{
  m_Type = (CDM::enumPatientAssessment::value)-1;
}

void SEPatientAssessmentRequest::ToString(std::ostream& str) const
{
  str << "Patient Action : Assessment Request";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tType: ";
  HasType() ? str << GetType() : str << "Not Set";
  str << std::flush;
}
}