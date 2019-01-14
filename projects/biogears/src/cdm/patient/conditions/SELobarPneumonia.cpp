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
*************************************************************************************/
#include <biogears/cdm/patient/conditions/SELobarPneumonia.h>

#include <biogears/cdm/properties/SEScalar0To1.h>

namespace biogears {
SELobarPneumonia::SELobarPneumonia()
  : SEPatientCondition()
{
  m_Severity = nullptr;
  m_LeftLungAffected = nullptr;
  m_RightLungAffected = nullptr;
}
//-----------------------------------------------------------------------------
SELobarPneumonia::~SELobarPneumonia()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SELobarPneumonia::Clear()
{
  SEPatientCondition::Clear();
  SAFE_DELETE(m_Severity);
  SAFE_DELETE(m_LeftLungAffected);
  SAFE_DELETE(m_RightLungAffected);
}
//-----------------------------------------------------------------------------
bool SELobarPneumonia::IsValid() const
{
  return SEPatientCondition::IsValid() && HasSeverity() && HasLeftLungAffected() && HasRightLungAffected();
}
//-----------------------------------------------------------------------------
bool SELobarPneumonia::Load(const CDM::LobarPneumoniaData& in)
{
  SEPatientCondition::Load(in);
  GetSeverity().Load(in.Severity());
  GetLeftLungAffected().Load(in.LeftLungAffected());
  GetRightLungAffected().Load(in.RightLungAffected());
  return true;
}
//-----------------------------------------------------------------------------
CDM::LobarPneumoniaData* SELobarPneumonia::Unload() const
{
  CDM::LobarPneumoniaData* data(new CDM::LobarPneumoniaData());
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SELobarPneumonia::Unload(CDM::LobarPneumoniaData& data) const
{
  SEPatientCondition::Unload(data);
  if (m_Severity != nullptr)
    data.Severity(std::unique_ptr<CDM::Scalar0To1Data>(m_Severity->Unload()));
  if (m_LeftLungAffected != nullptr)
    data.LeftLungAffected(std::unique_ptr<CDM::Scalar0To1Data>(m_LeftLungAffected->Unload()));
  if (m_RightLungAffected != nullptr)
    data.RightLungAffected(std::unique_ptr<CDM::Scalar0To1Data>(m_RightLungAffected->Unload()));
}
//-----------------------------------------------------------------------------
bool SELobarPneumonia::HasSeverity() const
{
  return m_Severity == nullptr ? false : m_Severity->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar0To1& SELobarPneumonia::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
//-----------------------------------------------------------------------------
bool SELobarPneumonia::HasLeftLungAffected() const
{
  return m_LeftLungAffected == nullptr ? false : m_LeftLungAffected->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar0To1& SELobarPneumonia::GetLeftLungAffected()
{
  if (m_LeftLungAffected == nullptr)
    m_LeftLungAffected = new SEScalar0To1();
  return *m_LeftLungAffected;
}
//-----------------------------------------------------------------------------
bool SELobarPneumonia::HasRightLungAffected() const
{
  return m_RightLungAffected == nullptr ? false : m_RightLungAffected->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar0To1& SELobarPneumonia::GetRightLungAffected()
{
  if (m_RightLungAffected == nullptr)
    m_RightLungAffected = new SEScalar0To1();
  return *m_RightLungAffected;
}
//-----------------------------------------------------------------------------
void SELobarPneumonia::ToString(std::ostream& str) const
{
  str << "Patient Condition : Lobar Pneumonia";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity: ";
  HasSeverity() ? str << *m_Severity : str << "NaN";
  str << "\n\tLeftLungAffected: ";
  HasLeftLungAffected() ? str << *m_LeftLungAffected : str << "NaN";
  str << "\n\tRightLungAffected: ";
  HasRightLungAffected() ? str << *m_RightLungAffected : str << "NaN";
  str << std::flush;
}
//-----------------------------------------------------------------------------
}