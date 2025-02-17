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
{
  m_Inflammation = false; // When the burn wound is constructed, the corresponding inflammation state has not been established
  m_FracturedBone = SEFracturedBone::Invalid; // User input, bone affected
  m_Side = SESide::Invalid; //Side of Affected Fracture
  m_FractureType = SEFractureType::Invalid; // User input, type of fracture on bone
  m_Severity = nullptr;
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
bool SEFracture::HasInflammation() const
{
  return m_Inflammation;
}
//-----------------------------------------------------------------------------
void SEFracture::SetInflammation(bool activate)
{
  m_Inflammation = activate;
}
//-------------------------------------------------------------------------------
}
