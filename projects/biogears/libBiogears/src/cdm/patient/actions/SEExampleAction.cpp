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
#include <biogears/cdm/patient/actions/SEExampleAction.h>

#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SEExampleAction::SEExampleAction()
  : SEPatientAction()
{
  m_Severity = nullptr;
}
//-------------------------------------------------------------------------------
SEExampleAction::~SEExampleAction()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEExampleAction::Clear()
{

  SEPatientAction::Clear();
  SAFE_DELETE(m_Severity);
}
//-------------------------------------------------------------------------------
bool SEExampleAction::IsValid() const
{
  return SEPatientAction::IsValid() && HasSeverity();
}
//-------------------------------------------------------------------------------
bool SEExampleAction::IsActive() const
{
  return IsValid() ? !m_Severity->IsZero() : false;
}
//-------------------------------------------------------------------------------
bool SEExampleAction::Load(const CDM::ExampleActionData& in)
{
  SEPatientAction::Load(in);
  GetSeverity().Load(in.Severity());
  return true;
}
//-------------------------------------------------------------------------------
CDM::ExampleActionData* SEExampleAction::Unload() const
{
  CDM::ExampleActionData* data(new CDM::ExampleActionData());
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SEExampleAction::Unload(CDM::ExampleActionData& data) const
{
  SEPatientAction::Unload(data);
  if (m_Severity != nullptr)
    data.Severity(std::unique_ptr<CDM::Scalar0To1Data>(m_Severity->Unload()));
}
//-------------------------------------------------------------------------------
bool SEExampleAction::HasSeverity() const
{
  return m_Severity == nullptr ? false : m_Severity->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar0To1& SEExampleAction::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
//-------------------------------------------------------------------------------
void SEExampleAction::ToString(std::ostream& str) const
{
  str << "Patient Action : Action Example";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity: ";
  HasSeverity() ? str << *m_Severity : str << "Not Set";
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SEExampleAction::operator==(const SEExampleAction& rhs) const
{
  bool equivilant = SEPatientAction::operator==(rhs);
  equivilant &= m_Comment == rhs.m_Comment;
  equivilant &= (m_Severity && rhs.m_Severity) ? m_Severity->operator==(*rhs.m_Severity) : m_Severity == rhs.m_Severity;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SEExampleAction::operator!=(const SEExampleAction& rhs) const
{
  return !(*this == rhs);
}
}