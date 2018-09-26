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
#include <biogears/cdm/patient/actions/SEAsthmaAttack.h>

#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SEAsthmaAttack::SEAsthmaAttack()
  : SEPatientAction()
{
  m_Severity = nullptr;
}

SEAsthmaAttack::~SEAsthmaAttack()
{
  Clear();
}

void SEAsthmaAttack::Clear()
{
  SEPatientAction::Clear();
  SAFE_DELETE(m_Severity);
}

bool SEAsthmaAttack::IsValid() const
{
  return SEPatientAction::IsValid() && HasSeverity();
}

bool SEAsthmaAttack::IsActive() const
{
  return IsValid() ? !m_Severity->IsZero() : false;
}

bool SEAsthmaAttack::Load(const CDM::AsthmaAttackData& in)
{
  SEPatientAction::Load(in);
  GetSeverity().Load(in.Severity());
  return true;
}

CDM::AsthmaAttackData* SEAsthmaAttack::Unload() const
{
  CDM::AsthmaAttackData* data(new CDM::AsthmaAttackData());
  Unload(*data);
  return data;
}

void SEAsthmaAttack::Unload(CDM::AsthmaAttackData& data) const
{
  SEPatientAction::Unload(data);
  if (m_Severity != nullptr)
    data.Severity(std::unique_ptr<CDM::Scalar0To1Data>(m_Severity->Unload()));
}

bool SEAsthmaAttack::HasSeverity() const
{
  return m_Severity == nullptr ? false : m_Severity->IsValid();
}
SEScalar0To1& SEAsthmaAttack::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}

void SEAsthmaAttack::ToString(std::ostream& str) const
{
  str << "Patient Action : AsthmaAttack";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity: ";
  HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}
}