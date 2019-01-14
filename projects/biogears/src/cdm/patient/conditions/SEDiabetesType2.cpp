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
#include <biogears/cdm/patient/conditions/SEDiabetesType2.h>

#include <biogears/cdm/properties/SEScalar0To1.h>

namespace biogears {
SEDiabetesType2::SEDiabetesType2()
  : SEPatientCondition()
{
  m_InsulinProductionSeverity = nullptr;
  m_InsulinResistanceSeverity = nullptr;
}
//-----------------------------------------------------------------------------
SEDiabetesType2::~SEDiabetesType2()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEDiabetesType2::Clear()
{
  SEPatientCondition::Clear();
  SAFE_DELETE(m_InsulinProductionSeverity);
  SAFE_DELETE(m_InsulinResistanceSeverity);
}
//-----------------------------------------------------------------------------
bool SEDiabetesType2::IsValid() const
{
  return SEPatientCondition::IsValid() && HasInsulinProductionSeverity() && HasInsulinResistanceSeverity();
}
//-----------------------------------------------------------------------------
bool SEDiabetesType2::Load(const CDM::DiabetesType2Data& in)
{
  SEPatientCondition::Load(in);
  GetInsulinProductionSeverity().Load(in.InsulinProductionSeverity());
  GetInsulinResistanceSeverity().Load(in.InsulinResistanceSeverity());
  return true;
}
//-----------------------------------------------------------------------------
CDM::DiabetesType2Data* SEDiabetesType2::Unload() const
{
  CDM::DiabetesType2Data* data(new CDM::DiabetesType2Data());
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEDiabetesType2::Unload(CDM::DiabetesType2Data& data) const
{
  SEPatientCondition::Unload(data);
  if (m_InsulinProductionSeverity != nullptr)
    data.InsulinProductionSeverity(std::unique_ptr<CDM::Scalar0To1Data>(m_InsulinProductionSeverity->Unload()));
  if (m_InsulinResistanceSeverity != nullptr)
    data.InsulinResistanceSeverity(std::unique_ptr<CDM::Scalar0To1Data>(m_InsulinResistanceSeverity->Unload()));
}
//-----------------------------------------------------------------------------
bool SEDiabetesType2::HasInsulinProductionSeverity() const
{
  return m_InsulinProductionSeverity == nullptr ? false : m_InsulinProductionSeverity->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar0To1& SEDiabetesType2::GetInsulinProductionSeverity()
{
  if (m_InsulinProductionSeverity == nullptr)
    m_InsulinProductionSeverity = new SEScalar0To1();
  return *m_InsulinProductionSeverity;
}
//-----------------------------------------------------------------------------
bool SEDiabetesType2::HasInsulinResistanceSeverity() const
{
  return m_InsulinResistanceSeverity == nullptr ? false : m_InsulinResistanceSeverity->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar0To1& SEDiabetesType2::GetInsulinResistanceSeverity()
{
  if (m_InsulinResistanceSeverity == nullptr)
    m_InsulinResistanceSeverity = new SEScalar0To1();
  return *m_InsulinResistanceSeverity;
}
//-----------------------------------------------------------------------------
void SEDiabetesType2::ToString(std::ostream& str) const
{
  str << "Patient Condition : DiabetesType2";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tDiabetes Type 2 Insulin Production Severity: ";
  HasInsulinProductionSeverity() ? str << m_InsulinProductionSeverity : str << "NaN";
  str << "\n\tDiabetes Type 2 Insulin Resistance Severity: ";
  HasInsulinResistanceSeverity() ? str << m_InsulinResistanceSeverity : str << "NaN";
  str << std::flush;
}
//-----------------------------------------------------------------------------
}