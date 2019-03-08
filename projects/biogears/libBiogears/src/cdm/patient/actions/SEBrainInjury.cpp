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

#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SEBrainInjury::SEBrainInjury()
  : SEPatientAction()
{
  m_Severity = nullptr;
  m_Type = (CDM::enumBrainInjuryType::value)-1;
}

SEBrainInjury::~SEBrainInjury()
{
  Clear();
}

void SEBrainInjury::Clear()
{

  SEPatientAction::Clear();
  SAFE_DELETE(m_Severity);
  m_Type = (CDM::enumBrainInjuryType::value)-1;
}

bool SEBrainInjury::IsValid() const
{
  return SEPatientAction::IsValid() && HasSeverity();
}

bool SEBrainInjury::IsActive() const
{
  return IsValid() ? !m_Severity->IsZero() : false;
}

bool SEBrainInjury::Load(const CDM::BrainInjuryData& in)
{
  SEPatientAction::Load(in);
  GetSeverity().Load(in.Severity());
  m_Type = in.Type();
  return true;
}

CDM::BrainInjuryData* SEBrainInjury::Unload() const
{
  CDM::BrainInjuryData* data(new CDM::BrainInjuryData());
  Unload(*data);
  return data;
}

void SEBrainInjury::Unload(CDM::BrainInjuryData& data) const
{
  SEPatientAction::Unload(data);
  if (m_Severity != nullptr)
    data.Severity(std::unique_ptr<CDM::Scalar0To1Data>(m_Severity->Unload()));
  if (HasType())
    data.Type(m_Type);
}

bool SEBrainInjury::HasSeverity() const
{
  return m_Severity == nullptr ? false : m_Severity->IsValid();
}
SEScalar0To1& SEBrainInjury::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}

CDM::enumBrainInjuryType::value SEBrainInjury::GetType() const
{
  return m_Type;
}
void SEBrainInjury::SetType(CDM::enumBrainInjuryType::value Type)
{
  m_Type = Type;
}
bool SEBrainInjury::HasType() const
{
  return m_Type == ((CDM::enumBrainInjuryType::value)-1) ? false : true;
}
void SEBrainInjury::InvalidateType()
{
  m_Type = (CDM::enumBrainInjuryType::value)-1;
}

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
}