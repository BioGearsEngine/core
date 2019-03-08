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
#include <biogears/cdm/patient/conditions/SEChronicRenalStenosis.h>

#include <biogears/cdm/properties/SEScalar0To1.h>

namespace biogears {
SEChronicRenalStenosis::SEChronicRenalStenosis()
  : SEPatientCondition()
{
  m_LeftKidneySeverity = nullptr;
  m_RightKidneySeverity = nullptr;
}

SEChronicRenalStenosis::~SEChronicRenalStenosis()
{
  Clear();
}

void SEChronicRenalStenosis::Clear()
{
  SEPatientCondition::Clear();
  SAFE_DELETE(m_LeftKidneySeverity);
  SAFE_DELETE(m_RightKidneySeverity);
}

bool SEChronicRenalStenosis::IsValid() const
{
  return SEPatientCondition::IsValid() && (HasLeftKidneySeverity() || HasRightKidneySeverity());
}

bool SEChronicRenalStenosis::Load(const CDM::ChronicRenalStenosisData& in)
{
  SEPatientCondition::Load(in);
  if (in.LeftKidneySeverity().present())
    GetLeftKidneySeverity().Load(in.LeftKidneySeverity().get());
  if (in.RightKidneySeverity().present())
    GetRightKidneySeverity().Load(in.RightKidneySeverity().get());
  return true;
}

CDM::ChronicRenalStenosisData* SEChronicRenalStenosis::Unload() const
{
  CDM::ChronicRenalStenosisData* data(new CDM::ChronicRenalStenosisData());
  Unload(*data);
  return data;
}

void SEChronicRenalStenosis::Unload(CDM::ChronicRenalStenosisData& data) const
{
  SEPatientCondition::Unload(data);
  if (HasLeftKidneySeverity())
    data.LeftKidneySeverity(std::unique_ptr<CDM::Scalar0To1Data>(m_LeftKidneySeverity->Unload()));
  if (HasRightKidneySeverity())
    data.RightKidneySeverity(std::unique_ptr<CDM::Scalar0To1Data>(m_RightKidneySeverity->Unload()));
}

bool SEChronicRenalStenosis::HasLeftKidneySeverity() const
{
  return m_LeftKidneySeverity == nullptr ? false : m_LeftKidneySeverity->IsValid();
}
SEScalar0To1& SEChronicRenalStenosis::GetLeftKidneySeverity()
{
  if (m_LeftKidneySeverity == nullptr)
    m_LeftKidneySeverity = new SEScalar0To1();
  return *m_LeftKidneySeverity;
}

bool SEChronicRenalStenosis::HasRightKidneySeverity() const
{
  return m_RightKidneySeverity == nullptr ? false : m_RightKidneySeverity->IsValid();
}
SEScalar0To1& SEChronicRenalStenosis::GetRightKidneySeverity()
{
  if (m_RightKidneySeverity == nullptr)
    m_RightKidneySeverity = new SEScalar0To1();
  return *m_RightKidneySeverity;
}

void SEChronicRenalStenosis::ToString(std::ostream& str) const
{
  str << "Patient Condition : Chronic Renal Stenosis";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tLeft Kidney Occlusion 0To1: ";
  HasLeftKidneySeverity() ? str << *m_LeftKidneySeverity : str << "NaN";
  str << "\n\tRight Kidney Occlusion 0To1: ";
  HasRightKidneySeverity() ? str << *m_RightKidneySeverity : str << "NaN";
  str << std::flush;
}
}