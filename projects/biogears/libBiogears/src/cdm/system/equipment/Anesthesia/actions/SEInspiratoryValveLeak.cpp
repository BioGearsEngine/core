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
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEInspiratoryValveLeak.h>

#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SEInspiratoryValveLeak::SEInspiratoryValveLeak()
  : SEAnesthesiaMachineAction()
{
  m_Severity = nullptr;
}
//-------------------------------------------------------------------------------
SEInspiratoryValveLeak::~SEInspiratoryValveLeak()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEInspiratoryValveLeak::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  SAFE_DELETE(m_Severity);
}
//-------------------------------------------------------------------------------
bool SEInspiratoryValveLeak::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && HasSeverity();
}
//-------------------------------------------------------------------------------
bool SEInspiratoryValveLeak::IsActive() const
{
  return HasSeverity() ? !m_Severity->IsZero() : false;
}
//-------------------------------------------------------------------------------
bool SEInspiratoryValveLeak::Load(const CDM::InspiratoryValveLeakData& in, std::default_random_engine *rd)
{
  SEAnesthesiaMachineAction::Load(in);
  GetSeverity().Load(in.Severity(), rd);
  return true;
}
//-------------------------------------------------------------------------------
CDM::InspiratoryValveLeakData* SEInspiratoryValveLeak::Unload() const
{
  CDM::InspiratoryValveLeakData* data = new CDM::InspiratoryValveLeakData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SEInspiratoryValveLeak::Unload(CDM::InspiratoryValveLeakData& data) const
{
  SEAnesthesiaMachineAction::Unload(data);
  if (m_Severity != nullptr)
    data.Severity(std::unique_ptr<CDM::Scalar0To1Data>(m_Severity->Unload()));
}
//-------------------------------------------------------------------------------
bool SEInspiratoryValveLeak::HasSeverity() const
{
  return m_Severity != nullptr;
}
//-------------------------------------------------------------------------------
SEScalar0To1& SEInspiratoryValveLeak::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
//-------------------------------------------------------------------------------
void SEInspiratoryValveLeak::ToString(std::ostream& str) const
{
  str << "Anesthesia Machine Action : Inspiratory Valve Leak";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity :";
  HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SEInspiratoryValveLeak::operator==(SEInspiratoryValveLeak const& rhs) const
{
  if (this == &rhs)
    return true;

  return ((m_Severity && rhs.m_Severity) ? m_Severity->operator==(*rhs.m_Severity)
                                         : m_Severity == rhs.m_Severity)
    && m_Comment == rhs.m_Comment;
}
//-------------------------------------------------------------------------------
bool SEInspiratoryValveLeak::operator!=(SEInspiratoryValveLeak const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}