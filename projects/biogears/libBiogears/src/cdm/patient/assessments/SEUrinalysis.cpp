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
#include <biogears/cdm/patient/assessments/SEUrinalysis.h>

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/properties/SEScalarMassPerTime.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/utils/GeneralMath.h>
#include "io/cdm/PatientAssessments.h"

namespace biogears {
SEUrinalysis::SEUrinalysis()
{
  m_Color = SEUrineColor::Invalid;
  m_Appearance = SEClarityIndicator::Invalid;
  m_Glucose = SEPresenceIndicator::Invalid;
  m_Ketone = SEPresenceIndicator::Invalid;
  m_Bilirubin = nullptr;
  m_SpecificGravity = nullptr;
  m_Blood = SEPresenceIndicator::Invalid;
  m_pH = nullptr;
  m_Protein = SEPresenceIndicator::Invalid;
  m_Urobilinogen = nullptr;
  m_Nitrite = SEPresenceIndicator::Invalid;
  m_LeukocyteEsterase = SEPresenceIndicator::Invalid;

  m_Microscopic = nullptr;
}
//-------------------------------------------------------------------------------
SEUrinalysis::~SEUrinalysis()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEUrinalysis::Clear()
{
  SEPatientAssessment::Clear();
  m_Color = SEUrineColor::Invalid;
  m_Appearance = SEClarityIndicator::Invalid;
  m_Glucose = SEPresenceIndicator::Invalid;
  m_Ketone = SEPresenceIndicator::Invalid;
  SAFE_DELETE(m_Bilirubin);
  SAFE_DELETE(m_SpecificGravity);
  m_Blood = SEPresenceIndicator::Invalid;
  SAFE_DELETE(m_pH);
  m_Protein = SEPresenceIndicator::Invalid;
  SAFE_DELETE(m_Urobilinogen);
  m_Nitrite = SEPresenceIndicator::Invalid;
  m_LeukocyteEsterase = SEPresenceIndicator::Invalid;

  SAFE_DELETE(m_Microscopic);
}
//-------------------------------------------------------------------------------
void SEUrinalysis::Reset()
{
  SEPatientAssessment::Reset();
  m_Color = SEUrineColor::Invalid;
  m_Appearance = SEClarityIndicator::Invalid;
  m_Glucose = SEPresenceIndicator::Invalid;
  m_Ketone = SEPresenceIndicator::Invalid;
  INVALIDATE_PROPERTY(m_Bilirubin);
  INVALIDATE_PROPERTY(m_SpecificGravity);
  m_Blood = SEPresenceIndicator::Invalid;
  INVALIDATE_PROPERTY(m_pH);
  m_Protein = SEPresenceIndicator::Invalid;
  INVALIDATE_PROPERTY(m_Urobilinogen);
  m_Nitrite = SEPresenceIndicator::Invalid;
  m_LeukocyteEsterase = SEPresenceIndicator::Invalid;

  SAFE_DELETE(m_Microscopic);
}

//-------------------------------------------------------------------------------
bool SEUrinalysis::HasColorResult() const
{
  return m_Color != SEUrineColor::Invalid;
}
SEUrineColor SEUrinalysis::GetColorResult() const
{
  return m_Color;
}
void SEUrinalysis::SetColorResult(SEUrineColor color)
{
  m_Color = color;
}
void SEUrinalysis::InvalidateColorResult()
{
  m_Color = SEUrineColor::Invalid;
}
//-------------------------------------------------------------------------------
bool SEUrinalysis::HasAppearanceResult() const
{
  return m_Appearance != SEClarityIndicator::Invalid;
}
SEClarityIndicator SEUrinalysis::GetAppearanceResult() const
{
  return m_Appearance;
}
void SEUrinalysis::SetAppearanceResult(SEClarityIndicator c)
{
  m_Appearance = c;
}
void SEUrinalysis::InvalidateAppearanceResult()
{
  m_Appearance = SEClarityIndicator::Invalid;
}
//-------------------------------------------------------------------------------
bool SEUrinalysis::HasGlucoseResult() const
{
  return m_Glucose != SEPresenceIndicator::Invalid;
}
SEPresenceIndicator SEUrinalysis::GetGlucoseResult() const
{
  return m_Glucose;
}
void SEUrinalysis::SetGlucoseResult(SEPresenceIndicator p)
{
  m_Glucose = p;
}
void SEUrinalysis::InvalidateGlucoseResult()
{
  m_Glucose = SEPresenceIndicator::Invalid;
}
//-------------------------------------------------------------------------------
bool SEUrinalysis::HasKetoneResult() const
{
  return m_Ketone != SEPresenceIndicator::Invalid;
}
SEPresenceIndicator SEUrinalysis::GetKetoneResult() const
{
  return m_Ketone;
}
void SEUrinalysis::SetKetoneResult(SEPresenceIndicator p)
{
  m_Ketone = p;
}
void SEUrinalysis::InvalidateKetoneResult()
{
  m_Ketone = SEPresenceIndicator::Invalid;
}
//-------------------------------------------------------------------------------
bool SEUrinalysis::HasBilirubinResult() const
{
  return m_Bilirubin == nullptr ? false : m_Bilirubin->IsValid();
}
SEScalar& SEUrinalysis::GetBilirubinResult()
{
  if (m_Bilirubin == nullptr)
    m_Bilirubin = new SEScalar();
  return *m_Bilirubin;
}
//-------------------------------------------------------------------------------
bool SEUrinalysis::HasSpecificGravityResult() const
{
  return m_SpecificGravity == nullptr ? false : m_SpecificGravity->IsValid();
}
SEScalar& SEUrinalysis::GetSpecificGravityResult()
{
  if (m_SpecificGravity == nullptr)
    m_SpecificGravity = new SEScalar();
  return *m_SpecificGravity;
}
//-------------------------------------------------------------------------------
bool SEUrinalysis::HasBloodResult() const
{
  return m_Blood != SEPresenceIndicator::Invalid;
}
SEPresenceIndicator SEUrinalysis::GetBloodResult() const
{
  return m_Blood;
}
void SEUrinalysis::SetBloodResult(SEPresenceIndicator p)
{
  m_Blood = p;
}
void SEUrinalysis::InvalidateBloodResult()
{
  m_Blood = SEPresenceIndicator::Invalid;
}
//-------------------------------------------------------------------------------
bool SEUrinalysis::HasPHResult() const
{
  return m_pH == nullptr ? false : m_pH->IsValid();
}
SEScalar& SEUrinalysis::GetPHResult()
{
  if (m_pH == nullptr)
    m_pH = new SEScalar();
  return *m_pH;
}
//-------------------------------------------------------------------------------
bool SEUrinalysis::HasProteinResult() const
{
  return m_Protein != SEPresenceIndicator::Invalid;
}
SEPresenceIndicator SEUrinalysis::GetProteinResult() const
{
  return m_Protein;
}
void SEUrinalysis::SetProteinResult(SEPresenceIndicator p)
{
  m_Protein = p;
}
void SEUrinalysis::InvalidateProteinResult()
{
  m_Protein = SEPresenceIndicator::Invalid;
}
//-------------------------------------------------------------------------------
bool SEUrinalysis::HasUrobilinogenResult() const
{
  return m_Urobilinogen == nullptr ? false : m_Urobilinogen->IsValid();
}
SEScalarMassPerVolume& SEUrinalysis::GetUrobilinogenResult()
{
  if (m_Urobilinogen == nullptr)
    m_Urobilinogen = new SEScalarMassPerVolume();
  return *m_Urobilinogen;
}
//-------------------------------------------------------------------------------
bool SEUrinalysis::HasNitriteResult() const
{
  return m_Nitrite != SEPresenceIndicator::Invalid;
}
SEPresenceIndicator SEUrinalysis::GetNitriteResult() const
{
  return m_Nitrite;
}
void SEUrinalysis::SetNitriteResult(SEPresenceIndicator p)
{
  m_Nitrite = p;
}
void SEUrinalysis::InvalidateNitriteResult()
{
  m_Nitrite = SEPresenceIndicator::Invalid;
}
//-------------------------------------------------------------------------------
bool SEUrinalysis::HasLeukocyteEsteraseResult() const
{
  return m_LeukocyteEsterase != SEPresenceIndicator::Invalid;
}
SEPresenceIndicator SEUrinalysis::GetLeukocyteEsteraseResult() const
{
  return m_LeukocyteEsterase;
}
void SEUrinalysis::SetLeukocyteEsteraseResult(SEPresenceIndicator p)
{
  m_LeukocyteEsterase = p;
}
void SEUrinalysis::InvalidateLeukocyteEsteraseResult()
{
  m_LeukocyteEsterase = SEPresenceIndicator::Invalid;
}
//-------------------------------------------------------------------------------
bool SEUrinalysis::HasMicroscopicResult() const
{
  return m_Microscopic != nullptr;
}
SEUrinalysisMicroscopic& SEUrinalysis::GetMicroscopicResult()
{
  if (m_Microscopic == nullptr)
    m_Microscopic = new SEUrinalysisMicroscopic();
  return *m_Microscopic;
}
void SEUrinalysis::RemoveMicroscopicResult()
{
  SAFE_DELETE(m_Microscopic);
}
//-------------------------------------------------------------------------------
bool SEUrinalysis::operator==(SEUrinalysis const& rhs) const
{
  if (this == &rhs)
    return true;

  return ((m_Bilirubin && rhs.m_Bilirubin) ? m_Bilirubin->operator==(*rhs.m_Bilirubin) : m_Bilirubin == rhs.m_Bilirubin)
    && ((m_SpecificGravity && rhs.m_SpecificGravity) ? m_SpecificGravity->operator==(*rhs.m_SpecificGravity) : m_SpecificGravity == rhs.m_SpecificGravity)
    && ((m_pH && rhs.m_pH) ? m_pH->operator==(*rhs.m_pH) : m_pH == rhs.m_pH)
    && ((m_Urobilinogen && rhs.m_Urobilinogen) ? m_Urobilinogen->operator==(*rhs.m_Urobilinogen) : m_Urobilinogen == rhs.m_Urobilinogen)
    && m_Color == rhs.m_Color
    && m_Appearance == rhs.m_Appearance
    && m_Glucose == rhs.m_Glucose
    && m_Ketone == rhs.m_Ketone
    && m_Blood == rhs.m_Blood
    && m_Protein == rhs.m_Protein
    && m_Nitrite == rhs.m_Nitrite
    && m_LeukocyteEsterase == rhs.m_LeukocyteEsterase;
}
bool SEUrinalysis::operator!=(SEUrinalysis const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}