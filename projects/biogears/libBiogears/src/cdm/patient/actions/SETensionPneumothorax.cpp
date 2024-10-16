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
#include <biogears/cdm/patient/actions/SETensionPneumothorax.h>

#include "io/cdm/PatientActions.h"
#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SETensionPneumothorax::SETensionPneumothorax()
  : SEPatientAction()
{
  m_Type = (SEPneumothoraxType)-1;
  m_Side = SESide::Invalid;
  m_Severity = nullptr;
}
//-------------------------------------------------------------------------------
SETensionPneumothorax::~SETensionPneumothorax()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SETensionPneumothorax::Clear()
{
  SEPatientAction::Clear();
  m_Type = (SEPneumothoraxType)-1;
  m_Side = SESide::Invalid;
  SAFE_DELETE(m_Severity);
}
//-------------------------------------------------------------------------------
bool SETensionPneumothorax::Load(const CDM::TensionPneumothoraxData& in, std::default_random_engine *rd)
{
  io::PatientActions::UnMarshall(in, *this, rd);
  return true;
}
//-------------------------------------------------------------------------------
CDM::TensionPneumothoraxData* SETensionPneumothorax::Unload() const
{
  CDM::TensionPneumothoraxData* data(new CDM::TensionPneumothoraxData());
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SETensionPneumothorax::Unload(CDM::TensionPneumothoraxData& data) const
{
  io::PatientActions::Marshall(*this, data);
}
//-------------------------------------------------------------------------------
bool SETensionPneumothorax::IsValid() const
{
  return SEPatientAction::IsValid() && HasType() && HasSide() && HasSeverity();
}
//-------------------------------------------------------------------------------
bool SETensionPneumothorax::IsActive() const
{
  return IsValid();
}
//-------------------------------------------------------------------------------
SEPneumothoraxType SETensionPneumothorax::GetType() const
{
  return m_Type;
}
//-------------------------------------------------------------------------------
void SETensionPneumothorax::SetType(SEPneumothoraxType Type)
{
  m_Type = Type;
}
//-------------------------------------------------------------------------------
bool SETensionPneumothorax::HasType() const
{
  return m_Type == SEPneumothoraxType::Invalid ? false : true;
}
//-------------------------------------------------------------------------------
void SETensionPneumothorax::InvalidateType()
{
  m_Type = (SEPneumothoraxType)-1;
}
//-------------------------------------------------------------------------------
SESide SETensionPneumothorax::GetSide() const
{
  return m_Side;
}
//-------------------------------------------------------------------------------
void SETensionPneumothorax::SetSide(SESide Side)
{
  m_Side = Side;
}
bool SETensionPneumothorax::HasSide() const
{
  return m_Side == SESide::Invalid ? false : true;
}
//-------------------------------------------------------------------------------
void SETensionPneumothorax::InvalidateSide()
{
  m_Side = SESide::Invalid;
}
//-------------------------------------------------------------------------------
bool SETensionPneumothorax::HasSeverity() const
{
  return m_Severity == nullptr ? false : m_Severity->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar0To1& SETensionPneumothorax::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
//-------------------------------------------------------------------------------
void SETensionPneumothorax::ToString(std::ostream& str) const
{
  str << "Patient Action : Tension Pneumothorax";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity: ";
  HasSeverity() ? str << *m_Severity : str << "NaN";
  str << "\n\tType: ";
  HasType() ? str << GetType() : str << "Not Set";
  str << "\n\tSide: ";
  HasSide() ? str << GetSide() : str << "Not Set";
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SETensionPneumothorax::operator==( const SETensionPneumothorax& rhs) const
{
  bool equivilant = m_Comment == rhs.m_Comment;
  equivilant &= m_Side == rhs.m_Side;
  equivilant &= m_Type == rhs.m_Type;
  equivilant &= (m_Severity && rhs.m_Severity) ? m_Severity->operator==(*rhs.m_Severity) : m_Severity == rhs.m_Severity;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SETensionPneumothorax::operator!=( const SETensionPneumothorax& rhs) const
{
  return !(*this == rhs);
}
}