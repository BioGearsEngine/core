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

#include <biogears/cdm/patient/actions/SEBronchoconstriction.h>
#include <biogears/cdm/properties/SEScalar0To1.h>

namespace biogears {
SEBronchoconstriction::SEBronchoconstriction()
  : SEPatientAction()
{
  m_Severity = nullptr;
}

SEBronchoconstriction::~SEBronchoconstriction()
{
  Clear();
}

void SEBronchoconstriction::Clear()
{
  SEPatientAction::Clear();
  SAFE_DELETE(m_Severity);
}

bool SEBronchoconstriction::IsValid() const
{
  return SEPatientAction::IsValid() && HasSeverity();
}

bool SEBronchoconstriction::IsActive() const
{
  return IsValid() ? !m_Severity->IsZero() : false;
}

bool SEBronchoconstriction::Load(const CDM::BronchoconstrictionData& in)
{
  SEPatientAction::Load(in);
  GetSeverity().Load(in.Severity());
  return true;
}

CDM::BronchoconstrictionData* SEBronchoconstriction::Unload() const
{
  CDM::BronchoconstrictionData* data(new CDM::BronchoconstrictionData());
  Unload(*data);
  return data;
}

void SEBronchoconstriction::Unload(CDM::BronchoconstrictionData& data) const
{
  SEPatientAction::Unload(data);
  if (m_Severity != nullptr)
    data.Severity(std::unique_ptr<CDM::Scalar0To1Data>(m_Severity->Unload()));
}

bool SEBronchoconstriction::HasSeverity() const
{
  return m_Severity == nullptr ? false : m_Severity->IsValid();
}
SEScalar0To1& SEBronchoconstriction::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}

void SEBronchoconstriction::ToString(std::ostream& str) const
{
  str << "Patient Action : Bronchoconstriction";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity: ";
  HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}
}