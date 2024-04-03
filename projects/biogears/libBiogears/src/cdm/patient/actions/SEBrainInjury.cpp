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
#include <biogears/cdm/patient/actions/SEBrainInjury.h>

#include "io/cdm/PatientActions.h"
#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SEBrainInjury::SEBrainInjury()
  : SEPatientAction()
{
  m_Severity = nullptr;
  m_Type = SEBrainInjuryType::Invalid;
}
//-------------------------------------------------------------------------------
SEBrainInjury::~SEBrainInjury()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEBrainInjury::Clear()
{

  SEPatientAction::Clear();
  SAFE_DELETE(m_Severity);
  m_Type = SEBrainInjuryType::Invalid;
}
//-------------------------------------------------------------------------------
bool SEBrainInjury::IsValid() const
{
  return SEPatientAction::IsValid() && HasSeverity();
}
//-------------------------------------------------------------------------------
bool SEBrainInjury::IsActive() const
{
  return IsValid() ? !m_Severity->IsZero() : false;
}
//-------------------------------------------------------------------------------
bool SEBrainInjury::Load(const CDM::BrainInjuryData& in, std::default_random_engine* rd)
{
  io::PatientActions::UnMarshall(in, *this, rd);
  return true;
}
//-------------------------------------------------------------------------------
CDM::BrainInjuryData* SEBrainInjury::Unload() const
{
  CDM::BrainInjuryData* data(new CDM::BrainInjuryData());
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SEBrainInjury::Unload(CDM::BrainInjuryData& data) const
{
  io::PatientActions::Marshall(*this, data);
}
//-------------------------------------------------------------------------------
bool SEBrainInjury::HasSeverity() const
{
  return m_Severity == nullptr ? false : m_Severity->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar0To1& SEBrainInjury::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
//-------------------------------------------------------------------------------
SEBrainInjuryType SEBrainInjury::GetType() const
{
  return m_Type;
}
//-------------------------------------------------------------------------------
void SEBrainInjury::SetType(SEBrainInjuryType Type)
{
  m_Type = Type;
}
//-------------------------------------------------------------------------------
bool SEBrainInjury::HasType() const
{
  return m_Type == (SEBrainInjuryType::Invalid) ? false : true;
}
//-------------------------------------------------------------------------------
void SEBrainInjury::InvalidateType()
{
  m_Type = SEBrainInjuryType::Invalid;
}
//-------------------------------------------------------------------------------
void SEBrainInjury::ToString(std::ostream& str) const
{
  str << "Patient Action : Brain Injury";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity: ";
  HasSeverity() ? str << *m_Severity : str << "Not Set";
  str << "\n\tType: ";
  HasType() ? str << GetType() : str << "Not Set";
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SEBrainInjury::operator==(const SEBrainInjury& rhs) const
{
  bool equivilant = m_Comment == rhs.m_Comment;
  equivilant &= (m_Severity && rhs.m_Severity) ? m_Severity->operator==(*rhs.m_Severity) : m_Severity == rhs.m_Severity;
  equivilant &= m_Type == rhs.m_Type;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SEBrainInjury::operator!=(const SEBrainInjury& rhs) const
{
  return !(*this == rhs);
}
}