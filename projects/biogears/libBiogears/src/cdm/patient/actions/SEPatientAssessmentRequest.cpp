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

#include "io/cdm/PatientActions.h"
namespace biogears {
SEPatientAssessmentRequest::SEPatientAssessmentRequest()
  : SEPatientAction()
{
  m_Type = SEPatientAssessmentType::Invalid;
}
//-------------------------------------------------------------------------------
SEPatientAssessmentRequest::~SEPatientAssessmentRequest()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEPatientAssessmentRequest::Clear()
{
  SEPatientAction::Clear();
  m_Type = SEPatientAssessmentType::Invalid;
}
//-------------------------------------------------------------------------------
bool SEPatientAssessmentRequest::IsValid() const
{
  return SEPatientAction::IsValid() && HasType();
}
//-------------------------------------------------------------------------------
bool SEPatientAssessmentRequest::IsActive() const
{
  return IsValid();
}
//-------------------------------------------------------------------------------
SEPatientAssessmentType SEPatientAssessmentRequest::GetType() const
{
  return m_Type;
}
//-------------------------------------------------------------------------------
void SEPatientAssessmentRequest::SetType(SEPatientAssessmentType Type)
{
  m_Type = Type;
}
//-------------------------------------------------------------------------------
bool SEPatientAssessmentRequest::HasType() const
{
  return m_Type == SEPatientAssessmentType::Invalid ? false : true;
}
//-------------------------------------------------------------------------------
void SEPatientAssessmentRequest::InvalidateType()
{
  m_Type = SEPatientAssessmentType::Invalid;
}
//-------------------------------------------------------------------------------
void SEPatientAssessmentRequest::ToString(std::ostream& str) const
{
  str << "Patient Action : Assessment Request";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tType: ";
  HasType() ? str << GetType() : str << "Not Set";
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SEPatientAssessmentRequest::operator==( const SEPatientAssessmentRequest& rhs) const
{
  bool equivilant = m_Comment == rhs.m_Comment;
  equivilant &= m_Type == rhs.m_Type;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SEPatientAssessmentRequest::operator!=( const SEPatientAssessmentRequest& rhs) const
{
  return !(*this == rhs);
}
}