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
#include <biogears/cdm/patient/conditions/SEChronicObstructivePulmonaryDisease.h>

#include <biogears/cdm/properties/SEScalar0To1.h>

// Private Include
#include <io/cdm/PatientConditions.h>

namespace biogears {
SEChronicObstructivePulmonaryDisease::SEChronicObstructivePulmonaryDisease()
  : SEPatientCondition()
{
  m_BronchitisSeverity = nullptr;
  m_EmphysemaSeverity = nullptr;
}
//-----------------------------------------------------------------------------
SEChronicObstructivePulmonaryDisease::~SEChronicObstructivePulmonaryDisease()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEChronicObstructivePulmonaryDisease::Clear()
{
  SEPatientCondition::Clear();
  SAFE_DELETE(m_BronchitisSeverity);
  SAFE_DELETE(m_EmphysemaSeverity);
}
//-----------------------------------------------------------------------------
bool SEChronicObstructivePulmonaryDisease::IsValid() const
{
  return SEPatientCondition::IsValid() && HasBronchitisSeverity() && HasEmphysemaSeverity();
}
//-----------------------------------------------------------------------------
bool SEChronicObstructivePulmonaryDisease::Load(const CDM::ChronicObstructivePulmonaryDiseaseData& in)
{
  io::PatientConditions::UnMarshall(in, *this);
  return true;
}
//-----------------------------------------------------------------------------
CDM::ChronicObstructivePulmonaryDiseaseData* SEChronicObstructivePulmonaryDisease::Unload() const
{
  CDM::ChronicObstructivePulmonaryDiseaseData* data(new CDM::ChronicObstructivePulmonaryDiseaseData());
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEChronicObstructivePulmonaryDisease::Unload(CDM::ChronicObstructivePulmonaryDiseaseData& data) const
{
  io::PatientConditions::Marshall(*this, data);
}
//-----------------------------------------------------------------------------
bool SEChronicObstructivePulmonaryDisease::HasBronchitisSeverity() const
{
  return m_BronchitisSeverity == nullptr ? false : m_BronchitisSeverity->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar0To1& SEChronicObstructivePulmonaryDisease::GetBronchitisSeverity()
{
  if (m_BronchitisSeverity == nullptr)
    m_BronchitisSeverity = new SEScalar0To1();
  return *m_BronchitisSeverity;
}
//-----------------------------------------------------------------------------
bool SEChronicObstructivePulmonaryDisease::HasEmphysemaSeverity() const
{
  return m_EmphysemaSeverity == nullptr ? false : m_EmphysemaSeverity->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar0To1& SEChronicObstructivePulmonaryDisease::GetEmphysemaSeverity()
{
  if (m_EmphysemaSeverity == nullptr)
    m_EmphysemaSeverity = new SEScalar0To1();
  return *m_EmphysemaSeverity;
}
//-----------------------------------------------------------------------------
void SEChronicObstructivePulmonaryDisease::ToString(std::ostream& str) const
{
  str << "Patient Condition : COPD";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tBronchitisSeverity: ";
  HasBronchitisSeverity() ? str << *m_BronchitisSeverity : str << "NaN";
  str << "\n\tEmphysemaSeverity: ";
  HasEmphysemaSeverity() ? str << *m_EmphysemaSeverity : str << "NaN";
  str << std::flush;
}
//-----------------------------------------------------------------------------
bool SEChronicObstructivePulmonaryDisease::operator==(SEChronicObstructivePulmonaryDisease const& rhs) const
{
  if (this == &rhs)
    return true;

  return m_Comment == rhs.m_Comment
    && ((m_BronchitisSeverity && rhs.m_BronchitisSeverity) ? m_BronchitisSeverity->operator==(*rhs.m_BronchitisSeverity) : m_BronchitisSeverity == rhs.m_BronchitisSeverity)
    && ((m_EmphysemaSeverity && rhs.m_EmphysemaSeverity) ? m_EmphysemaSeverity->operator==(*rhs.m_EmphysemaSeverity) : m_EmphysemaSeverity == rhs.m_EmphysemaSeverity);
}
bool SEChronicObstructivePulmonaryDisease::operator!=(SEChronicObstructivePulmonaryDisease const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
bool SEChronicObstructivePulmonaryDisease::operator==(SECondition const& rhs) const
{
  try {
    auto& chronicObstructivePulmonaryDisease = dynamic_cast<SEChronicObstructivePulmonaryDisease const&>(rhs);
    return this->operator==(chronicObstructivePulmonaryDisease);
  } catch (std::exception) {
    return false;
  }
  return false;
}
bool SEChronicObstructivePulmonaryDisease::operator!=(SECondition const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}