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
#include <biogears/cdm/patient/actions/SEFracture.h>

#include "io/cdm/PatientActions.h"
#include <biogears/cdm/properties/SEProperties.h>

namespace biogears {

SEFracture::SEFracture()
  : SEPatientAction()
  , m_Severity(new SEScalar0To1())
{

  m_Inflammation = false; // When the fracture is constructed, the corresponding inflammation state has not been established
  m_FracturedBone = SEFracturedBone::Invalid; // User input, bone affected
  m_FractureType = SEFractureType::Invalid; // User input, type of fracture on bone
  m_Side = SESide::Invalid; //Side of Affected Fracture
}
//-----------------------------------------------------------------------------
SEFracture::~SEFracture()
{
  Invalidate();
}
//-----------------------------------------------------------------------------
void SEFracture::Invalidate()
{
  SEPatientAction::Invalidate();
  m_Inflammation = false;
  m_FracturedBone = SEFracturedBone::Invalid;
  m_Side = SESide::Invalid; 
  m_FractureType = SEFractureType::Invalid;
  SAFE_DELETE(m_Severity);
}
//-----------------------------------------------------------------------------
bool SEFracture::IsValid() const
{
  return SEPatientAction::IsValid() && HasFracturedBone() && HasSide() && HasFractureType() && HasSeverity();
}
//-----------------------------------------------------------------------------
bool SEFracture::IsActive() const
{
  return IsValid() ? !m_Severity->IsZero() : false;
}
//-----------------------------------------------------------------------------
bool SEFracture::HasFracturedBone() const
{
  return m_FracturedBone == SEFracturedBone::Invalid ? false : true;
}
//-----------------------------------------------------------------------------
SEFracturedBone SEFracture::GetFracturedBone() const
{
  return m_FracturedBone;
}
//-----------------------------------------------------------------------------
void SEFracture::SetFracturedBone(SEFracturedBone bone)
{
  m_FracturedBone = bone;
}
//-------------------------------------------------------------------------------
bool SEFracture::HasSide() const
{
  return m_Side == SESide::Invalid ? false : true;
}
//-------------------------------------------------------------------------------
SESide SEFracture::GetSide() const
{
  return m_Side;
}
//-------------------------------------------------------------------------------
void SEFracture::SetSide(SESide Side)
{
  m_Side = Side;
}
//-------------------------------------------------------------------------------
void SEFracture::InvalidateSide()
{
  m_Side = SESide::Invalid;
}
//-----------------------------------------------------------------------------
bool SEFracture::HasFractureType() const
{
  return m_FractureType == SEFractureType::Invalid ? false : true;
}
//-----------------------------------------------------------------------------
SEFractureType SEFracture::GetFractureType() const
{
  return m_FractureType;
}
//-----------------------------------------------------------------------------
void SEFracture::SetFractureType(SEFractureType type)
{
  m_FractureType = type;
}
//-----------------------------------------------------------------------------
bool SEFracture::HasSeverity() const
{
  return m_Severity == nullptr ? false : m_Severity->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar0To1& SEFracture::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
//-----------------------------------------------------------------------------
/* void SEFracture::SetSeverity(SEFracturedBone bone, SEFractureType type)
{
  switch (bone) {
  case SEFracturedBone::Radius:
    switch (type) {
    case SEFractureType::Comminuted:
      m_Severity->SetValue(0.3); // If comminuted AIS code 3 (severe, not life threatening)
      break;
    default:
      m_Severity->SetValue(0.2); // Default to AIS code 2 (moderate) for radius fracture
    }
    break;
  case SEFracturedBone::Tibia:
    switch (type) {
    case SEFractureType::Comminuted:
      m_Severity->SetValue(0.3); // If comminuted AIS code 3 (severe, not life threatening)
      break;
    default:
      m_Severity->SetValue(0.2); // Default to AIS code 2 (moderate) for tibia fracture
    }
  default:
    m_Severity->SetValue(0.0);
    break;
  }
} */
//-----------------------------------------------------------------------------
bool SEFracture::HasInflammation() const
{
  return m_Inflammation;
}
//-----------------------------------------------------------------------------
void SEFracture::SetInflammation(bool activate)
{
  m_Inflammation = activate;
}
//-----------------------------------------------------------------------------
void SEFracture::ToString(std::ostream& str) const
{
  // Currently all fields are required, but this should be updated if/when that is changed
  str << "Patient Action : Musculoskeletal Fracture";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tFractured Bone:  ";
  str << m_FracturedBone;
  str << "\n\tFracture Type: ";
  str << m_FractureType;
  str << "\n\tSide: ";
  str << m_Side;
  str << "\n\tSeverity: ";
  str << *m_Severity;
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SEFracture::operator==(const SEFracture& rhs) const
{
  bool equivilant = m_Comment == rhs.m_Comment;
  equivilant &= m_FracturedBone == rhs.m_FracturedBone;
  equivilant &= m_FractureType == rhs.m_FractureType;
  equivilant &= (m_Severity && rhs.m_Severity) ? m_Severity->operator==(*rhs.m_Severity) : m_Severity == rhs.m_Severity;
  equivilant &= m_Side == rhs.m_Side;

  return equivilant;
}
//-------------------------------------------------------------------------------
bool SEFracture::operator!=(const SEFracture& rhs) const
{
  return !(*this == rhs);
}
}
