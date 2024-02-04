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
#include <biogears/cdm/system/equipment/Anesthesia/actions/SETubeCuffLeak.h>

#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/schema/cdm/Properties.hxx>

// Private Include
#include <io/cdm/AnesthesiaActions.h>

namespace biogears {
SETubeCuffLeak::SETubeCuffLeak()
  : SEAnesthesiaMachineAction()
{
  m_Severity = nullptr;
}
//-------------------------------------------------------------------------------
SETubeCuffLeak::~SETubeCuffLeak()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SETubeCuffLeak::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  SAFE_DELETE(m_Severity);
}
//-------------------------------------------------------------------------------
bool SETubeCuffLeak::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && HasSeverity();
}
//-------------------------------------------------------------------------------
bool SETubeCuffLeak::IsActive() const
{
  return HasSeverity() ? !m_Severity->IsZero() : false;
}
//-------------------------------------------------------------------------------
bool SETubeCuffLeak::Load(const CDM::TubeCuffLeakData& in)
{
  io::AnesthesiaActions::UnMarshall(in, *this);
  return true;
}
//-------------------------------------------------------------------------------
CDM::TubeCuffLeakData* SETubeCuffLeak::Unload() const
{
  CDM::TubeCuffLeakData* data = new CDM::TubeCuffLeakData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SETubeCuffLeak::Unload(CDM::TubeCuffLeakData& data) const
{
  io::AnesthesiaActions::Marshall(*this, data);
}
//-------------------------------------------------------------------------------
bool SETubeCuffLeak::HasSeverity() const
{
  return m_Severity != nullptr;
}
//-------------------------------------------------------------------------------
SEScalar0To1& SETubeCuffLeak::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
//-------------------------------------------------------------------------------
void SETubeCuffLeak::ToString(std::ostream& str) const
{
  str << "Anesthesia Machine Action : Tube Cuff Leak";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity :";
  HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SETubeCuffLeak::operator==(SETubeCuffLeak const& rhs) const
{
  if (this == &rhs)
    return true;

  return ((m_Severity && rhs.m_Severity) ? m_Severity->operator==(*rhs.m_Severity)
                                         : m_Severity == rhs.m_Severity)
    && m_Comment == rhs.m_Comment;
}
//-------------------------------------------------------------------------------
bool SETubeCuffLeak::operator!=(SETubeCuffLeak const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}