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
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEExpiratoryValveLeak.h>

#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SEExpiratoryValveLeak::SEExpiratoryValveLeak()
  : SEAnesthesiaMachineAction()
{
  m_Severity = nullptr;
}

SEExpiratoryValveLeak::~SEExpiratoryValveLeak()
{
  Clear();
}

void SEExpiratoryValveLeak::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  SAFE_DELETE(m_Severity);
}

bool SEExpiratoryValveLeak::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && HasSeverity();
}

bool SEExpiratoryValveLeak::IsActive() const
{
  return HasSeverity() ? !m_Severity->IsZero() : false;
}

bool SEExpiratoryValveLeak::Load(const CDM::ExpiratoryValveLeakData& in)
{
  SEAnesthesiaMachineAction::Load(in);
  GetSeverity().Load(in.Severity());
  return true;
}

CDM::ExpiratoryValveLeakData* SEExpiratoryValveLeak::Unload() const
{
  CDM::ExpiratoryValveLeakData* data = new CDM::ExpiratoryValveLeakData();
  Unload(*data);
  return data;
}

void SEExpiratoryValveLeak::Unload(CDM::ExpiratoryValveLeakData& data) const
{
  SEAnesthesiaMachineAction::Unload(data);
  if (m_Severity != nullptr)
    data.Severity(std::unique_ptr<CDM::Scalar0To1Data>(m_Severity->Unload()));
}

bool SEExpiratoryValveLeak::HasSeverity() const
{
  return m_Severity != nullptr;
}
SEScalar0To1& SEExpiratoryValveLeak::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}

void SEExpiratoryValveLeak::ToString(std::ostream& str) const
{
  str << "Anesthesia Machine Action : Expiratory Valve Leak";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity :";
  HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}
}