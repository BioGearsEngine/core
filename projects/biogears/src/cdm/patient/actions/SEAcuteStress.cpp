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
#include <biogears/cdm/patient/actions/SEAcuteStress.h>

#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SEAcuteStress::SEAcuteStress()
  : SEPatientAction()
{
  m_Severity = nullptr;
}

SEAcuteStress::~SEAcuteStress()
{
  Clear();
}

void SEAcuteStress::Clear()
{

  SEPatientAction::Clear();
  SAFE_DELETE(m_Severity);
}

bool SEAcuteStress::IsValid() const
{
  return SEPatientAction::IsValid() && HasSeverity();
}

bool SEAcuteStress::IsActive() const
{
  return IsValid() ? !m_Severity->IsZero() : false;
}

bool SEAcuteStress::Load(const CDM::AcuteStressData& in)
{
  SEPatientAction::Load(in);
  GetSeverity().Load(in.Severity());
  return true;
}

CDM::AcuteStressData* SEAcuteStress::Unload() const
{
  CDM::AcuteStressData* data(new CDM::AcuteStressData());
  Unload(*data);
  return data;
}

void SEAcuteStress::Unload(CDM::AcuteStressData& data) const
{
  SEPatientAction::Unload(data);
  if (m_Severity != nullptr)
    data.Severity(std::unique_ptr<CDM::Scalar0To1Data>(m_Severity->Unload()));
}

bool SEAcuteStress::HasSeverity() const
{
  return m_Severity == nullptr ? false : m_Severity->IsValid();
}

SEScalar0To1& SEAcuteStress::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}

void SEAcuteStress::ToString(std::ostream& str) const
{
  str << "Patient Action : Acute Stress";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity: ";
  HasSeverity() ? str << *m_Severity : str << "Not Set";
  str << std::flush;
}
}