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
#include <biogears/cdm/patient/conditions/SEDiabetesType1.h>

#include <biogears/cdm/properties/SEScalar0To1.h>

// Private Include
#include <io/cdm/PatientConditions.h>

namespace biogears {
SEDiabetesType1::SEDiabetesType1()
  : SEPatientCondition()
{
  m_InsulinProductionSeverity = nullptr;
}
//-----------------------------------------------------------------------------
SEDiabetesType1::~SEDiabetesType1()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEDiabetesType1::Clear()
{
  SEPatientCondition::Clear();
  SAFE_DELETE(m_InsulinProductionSeverity);
}
//-----------------------------------------------------------------------------
bool SEDiabetesType1::IsValid() const
{
  return SEPatientCondition::IsValid() && HasInsulinProductionSeverity();
}
//-----------------------------------------------------------------------------
bool SEDiabetesType1::Load(const CDM::DiabetesType1Data& in)
{
  io::PatientConditions::UnMarshall(in, *this);
  return true;
}
//-----------------------------------------------------------------------------
CDM::DiabetesType1Data* SEDiabetesType1::Unload() const
{
  CDM::DiabetesType1Data* data(new CDM::DiabetesType1Data());
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEDiabetesType1::Unload(CDM::DiabetesType1Data& data) const
{
  io::PatientConditions::Marshall(*this, data);
}
//-----------------------------------------------------------------------------
bool SEDiabetesType1::HasInsulinProductionSeverity() const
{
  return m_InsulinProductionSeverity == nullptr ? false : m_InsulinProductionSeverity->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar0To1& SEDiabetesType1::GetInsulinProductionSeverity()
{
  if (m_InsulinProductionSeverity == nullptr)
    m_InsulinProductionSeverity = new SEScalar0To1();
  return *m_InsulinProductionSeverity;
}
//-----------------------------------------------------------------------------
void SEDiabetesType1::ToString(std::ostream& str) const
{
  str << "Patient Condition : DiabetesType1";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tDiabetes Type 1 Insulin Production Severity: ";
  HasInsulinProductionSeverity() ? str << m_InsulinProductionSeverity : str << "NaN";
  str << std::flush;
}

//-----------------------------------------------------------------------------
bool SEDiabetesType1::operator==(SEDiabetesType1 const& rhs) const
{
  if (this == &rhs)
    return true;

  return m_Comment == rhs.m_Comment
    && ((m_InsulinProductionSeverity && rhs.m_InsulinProductionSeverity) ? m_InsulinProductionSeverity->operator==(*rhs.m_InsulinProductionSeverity) : m_InsulinProductionSeverity == rhs.m_InsulinProductionSeverity);
}
bool SEDiabetesType1::operator!=(SEDiabetesType1 const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
bool SEDiabetesType1::operator==(SECondition const& rhs) const
{
  try {
    auto& diabetesType1 = dynamic_cast<SEDiabetesType1 const&>(rhs);
    return this->operator==(diabetesType1);
  } catch (std::exception) {
    return false;
  }
  return false;
}
bool SEDiabetesType1::operator!=(SECondition const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}