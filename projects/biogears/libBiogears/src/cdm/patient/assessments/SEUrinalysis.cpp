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

// Private Include
#include <io/cdm/PatientAssessments.h>

namespace biogears {
SEUrinalysis::SEUrinalysis()
{
  m_Color = CDM::enumUrineColor::value(-1);
  m_Appearance = CDM::enumClarityIndicator::value(-1);
  m_Glucose = CDM::enumPresenceIndicator::value(-1);
  m_Ketone = CDM::enumPresenceIndicator::value(-1);
  m_Bilirubin = nullptr;
  m_SpecificGravity = nullptr;
  m_Blood = CDM::enumPresenceIndicator::value(-1);
  m_pH = nullptr;
  m_Protein = CDM::enumPresenceIndicator::value(-1);
  m_Urobilinogen = nullptr;
  m_Nitrite = CDM::enumPresenceIndicator::value(-1);
  m_LeukocyteEsterase = CDM::enumPresenceIndicator::value(-1);

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
  m_Color = CDM::enumUrineColor::value(-1);
  m_Appearance = CDM::enumClarityIndicator::value(-1);
  m_Glucose = CDM::enumPresenceIndicator::value(-1);
  m_Ketone = CDM::enumPresenceIndicator::value(-1);
  SAFE_DELETE(m_Bilirubin);
  SAFE_DELETE(m_SpecificGravity);
  m_Blood = CDM::enumPresenceIndicator::value(-1);
  SAFE_DELETE(m_pH);
  m_Protein = CDM::enumPresenceIndicator::value(-1);
  SAFE_DELETE(m_Urobilinogen);
  m_Nitrite = CDM::enumPresenceIndicator::value(-1);
  m_LeukocyteEsterase = CDM::enumPresenceIndicator::value(-1);

  SAFE_DELETE(m_Microscopic);
}
//-------------------------------------------------------------------------------
void SEUrinalysis::Reset()
{
  SEPatientAssessment::Reset();
  m_Color = CDM::enumUrineColor::value(-1);
  m_Appearance = CDM::enumClarityIndicator::value(-1);
  m_Glucose = CDM::enumPresenceIndicator::value(-1);
  m_Ketone = CDM::enumPresenceIndicator::value(-1);
  INVALIDATE_PROPERTY(m_Bilirubin);
  INVALIDATE_PROPERTY(m_SpecificGravity);
  m_Blood = CDM::enumPresenceIndicator::value(-1);
  INVALIDATE_PROPERTY(m_pH);
  m_Protein = CDM::enumPresenceIndicator::value(-1);
  INVALIDATE_PROPERTY(m_Urobilinogen);
  m_Nitrite = CDM::enumPresenceIndicator::value(-1);
  m_LeukocyteEsterase = CDM::enumPresenceIndicator::value(-1);

  SAFE_DELETE(m_Microscopic);
}
//-------------------------------------------------------------------------------
bool SEUrinalysis::Load(const CDM::UrinalysisData& in)
{
  io::PatientAssessments::UnMarshall(in, *this);
  return true;
}
//-------------------------------------------------------------------------------
CDM::UrinalysisData* SEUrinalysis::Unload()
{
  CDM::UrinalysisData* data = new CDM::UrinalysisData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SEUrinalysis::Unload(CDM::UrinalysisData& data)
{
  io::PatientAssessments::Marshall(*this, data);
}
//-------------------------------------------------------------------------------
bool SEUrinalysis::HasColorResult() const
{
  return m_Color != CDM::enumUrineColor::value(-1);
}
CDM::enumUrineColor::value SEUrinalysis::GetColorResult() const
{
  return m_Color;
}
void SEUrinalysis::SetColorResult(CDM::enumUrineColor::value color)
{
  m_Color = color;
}
void SEUrinalysis::InvalidateColorResult()
{
  m_Color = CDM::enumUrineColor::value(-1);
}
//-------------------------------------------------------------------------------
bool SEUrinalysis::HasAppearanceResult() const
{
  return m_Appearance != CDM::enumClarityIndicator::value(-1);
}
CDM::enumClarityIndicator::value SEUrinalysis::GetAppearanceResult() const
{
  return m_Appearance;
}
void SEUrinalysis::SetAppearanceResult(CDM::enumClarityIndicator::value c)
{
  m_Appearance = c;
}
void SEUrinalysis::InvalidateAppearanceResult()
{
  m_Appearance = CDM::enumClarityIndicator::value(-1);
}
//-------------------------------------------------------------------------------
bool SEUrinalysis::HasGlucoseResult() const
{
  return m_Glucose != CDM::enumPresenceIndicator::value(-1);
}
CDM::enumPresenceIndicator::value SEUrinalysis::GetGlucoseResult() const
{
  return m_Glucose;
}
void SEUrinalysis::SetGlucoseResult(CDM::enumPresenceIndicator::value p)
{
  m_Glucose = p;
}
void SEUrinalysis::InvalidateGlucoseResult()
{
  m_Glucose = CDM::enumPresenceIndicator::value(-1);
}
//-------------------------------------------------------------------------------
bool SEUrinalysis::HasKetoneResult() const
{
  return m_Ketone != CDM::enumPresenceIndicator::value(-1);
}
CDM::enumPresenceIndicator::value SEUrinalysis::GetKetoneResult() const
{
  return m_Ketone;
}
void SEUrinalysis::SetKetoneResult(CDM::enumPresenceIndicator::value p)
{
  m_Ketone = p;
}
void SEUrinalysis::InvalidateKetoneResult()
{
  m_Ketone = CDM::enumPresenceIndicator::value(-1);
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
  return m_Blood != CDM::enumPresenceIndicator::value(-1);
}
CDM::enumPresenceIndicator::value SEUrinalysis::GetBloodResult() const
{
  return m_Blood;
}
void SEUrinalysis::SetBloodResult(CDM::enumPresenceIndicator::value p)
{
  m_Blood = p;
}
void SEUrinalysis::InvalidateBloodResult()
{
  m_Blood = CDM::enumPresenceIndicator::value(-1);
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
  return m_Protein != CDM::enumPresenceIndicator::value(-1);
}
CDM::enumPresenceIndicator::value SEUrinalysis::GetProteinResult() const
{
  return m_Protein;
}
void SEUrinalysis::SetProteinResult(CDM::enumPresenceIndicator::value p)
{
  m_Protein = p;
}
void SEUrinalysis::InvalidateProteinResult()
{
  m_Protein = CDM::enumPresenceIndicator::value(-1);
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
  return m_Nitrite != CDM::enumPresenceIndicator::value(-1);
}
CDM::enumPresenceIndicator::value SEUrinalysis::GetNitriteResult() const
{
  return m_Nitrite;
}
void SEUrinalysis::SetNitriteResult(CDM::enumPresenceIndicator::value p)
{
  m_Nitrite = p;
}
void SEUrinalysis::InvalidateNitriteResult()
{
  m_Nitrite = CDM::enumPresenceIndicator::value(-1);
}
//-------------------------------------------------------------------------------
bool SEUrinalysis::HasLeukocyteEsteraseResult() const
{
  return m_LeukocyteEsterase != CDM::enumPresenceIndicator::value(-1);
}
CDM::enumPresenceIndicator::value SEUrinalysis::GetLeukocyteEsteraseResult() const
{
  return m_LeukocyteEsterase;
}
void SEUrinalysis::SetLeukocyteEsteraseResult(CDM::enumPresenceIndicator::value p)
{
  m_LeukocyteEsterase = p;
}
void SEUrinalysis::InvalidateLeukocyteEsteraseResult()
{
  m_LeukocyteEsterase = CDM::enumPresenceIndicator::value(-1);
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