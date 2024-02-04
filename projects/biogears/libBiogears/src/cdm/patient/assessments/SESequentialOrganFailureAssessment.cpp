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
#include <biogears/cdm/patient/assessments/SESequentialOrganFailureAssessment.h>

#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>

// Private Include
#include <io/cdm/PatientAssessments.h>

namespace biogears {
SESequentialOrganFailureAssessment::SESequentialOrganFailureAssessment()
{
  m_RespirationSOFA = nullptr;
  m_CoagulationSOFA = nullptr;
  m_LiverSOFA = nullptr;
  m_CardiovascularSOFA = nullptr;
  m_CentralNervousSOFA = nullptr;
  m_RenalSOFA = nullptr;
}
//-------------------------------------------------------------------------------
SESequentialOrganFailureAssessment::~SESequentialOrganFailureAssessment()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SESequentialOrganFailureAssessment::Clear()
{
  SEPatientAssessment::Clear();
  SAFE_DELETE(m_RespirationSOFA);
  SAFE_DELETE(m_CoagulationSOFA);
  SAFE_DELETE(m_LiverSOFA);
  SAFE_DELETE(m_CardiovascularSOFA);
  SAFE_DELETE(m_CentralNervousSOFA);
  SAFE_DELETE(m_RenalSOFA);
}
//-------------------------------------------------------------------------------
void SESequentialOrganFailureAssessment::Reset()
{
  SEPatientAssessment::Reset();
  INVALIDATE_PROPERTY(m_RespirationSOFA);
  INVALIDATE_PROPERTY(m_CoagulationSOFA);
  INVALIDATE_PROPERTY(m_LiverSOFA);
  INVALIDATE_PROPERTY(m_CardiovascularSOFA);
  INVALIDATE_PROPERTY(m_CentralNervousSOFA);
  INVALIDATE_PROPERTY(m_RenalSOFA);
}
//-------------------------------------------------------------------------------
bool SESequentialOrganFailureAssessment::Load(const CDM::SequentialOrganFailureAssessmentData& in)
{
  io::PatientAssessments::UnMarshall(in, *this);
  return true;
}
//-------------------------------------------------------------------------------
CDM::SequentialOrganFailureAssessmentData* SESequentialOrganFailureAssessment::Unload()
{
  CDM::SequentialOrganFailureAssessmentData* data = new CDM::SequentialOrganFailureAssessmentData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SESequentialOrganFailureAssessment::Unload(CDM::SequentialOrganFailureAssessmentData& data)
{
  io::PatientAssessments::Marshall(*this, data);
}
//-------------------------------------------------------------------------------
bool SESequentialOrganFailureAssessment::HasRespirationSOFA()
{
  return m_RespirationSOFA == nullptr ? false : m_RespirationSOFA->IsValid();
}
SEScalar& SESequentialOrganFailureAssessment::GetRespirationSOFA()
{
  if (m_RespirationSOFA == nullptr)
    m_RespirationSOFA = new SEScalar();
  return *m_RespirationSOFA;
}
//-------------------------------------------------------------------------------
bool SESequentialOrganFailureAssessment::HasCoagulationSOFA()
{
  return m_CoagulationSOFA == nullptr ? false : m_CoagulationSOFA->IsValid();
}
SEScalar& SESequentialOrganFailureAssessment::GetCoagulationSOFA()
{
  if (m_CoagulationSOFA == nullptr)
    m_CoagulationSOFA = new SEScalar();
  return *m_CoagulationSOFA;
}
//-------------------------------------------------------------------------------
bool SESequentialOrganFailureAssessment::HasLiverSOFA()
{
  return m_LiverSOFA == nullptr ? false : m_LiverSOFA->IsValid();
}
SEScalar& SESequentialOrganFailureAssessment::GetLiverSOFA()
{
  if (m_LiverSOFA == nullptr)
    m_LiverSOFA = new SEScalar();
  return *m_LiverSOFA;
}
//-------------------------------------------------------------------------------
bool SESequentialOrganFailureAssessment::HasCardiovascularSOFA()
{
  return m_CardiovascularSOFA == nullptr ? false : m_CardiovascularSOFA->IsValid();
}
SEScalar& SESequentialOrganFailureAssessment::GetCardiovascularSOFA()
{
  if (m_CardiovascularSOFA == nullptr)
    m_CardiovascularSOFA = new SEScalar();
  return *m_CardiovascularSOFA;
}
//-------------------------------------------------------------------------------
bool SESequentialOrganFailureAssessment::HasCentralNervousSOFA()
{
  return m_CentralNervousSOFA == nullptr ? false : m_CentralNervousSOFA->IsValid();
}
SEScalar& SESequentialOrganFailureAssessment::GetCentralNervousSOFA()
{
  if (m_CentralNervousSOFA == nullptr)
    m_CentralNervousSOFA = new SEScalar();
  return *m_CentralNervousSOFA;
}
//-------------------------------------------------------------------------------
bool SESequentialOrganFailureAssessment::HasRenalSOFA()
{
  return m_RenalSOFA == nullptr ? false : m_RenalSOFA->IsValid();
}
SEScalar& SESequentialOrganFailureAssessment::GetRenalSOFA()
{
  if (m_RenalSOFA == nullptr)
    m_RenalSOFA = new SEScalar();
  return *m_RenalSOFA;
}
//-------------------------------------------------------------------------------
bool SESequentialOrganFailureAssessment::operator==(SESequentialOrganFailureAssessment const& rhs) const
{
  if (this == &rhs)
    return true;

  return ((m_RespirationSOFA && rhs.m_RespirationSOFA) ? m_RespirationSOFA->operator==(*rhs.m_RespirationSOFA) : m_RespirationSOFA == rhs.m_RespirationSOFA)
    && ((m_CoagulationSOFA && rhs.m_CoagulationSOFA) ? m_CoagulationSOFA->operator==(*rhs.m_CoagulationSOFA) : m_CoagulationSOFA == rhs.m_CoagulationSOFA)
    && ((m_LiverSOFA && rhs.m_LiverSOFA) ? m_LiverSOFA->operator==(*rhs.m_LiverSOFA) : m_LiverSOFA == rhs.m_LiverSOFA)
    && ((m_CardiovascularSOFA && rhs.m_CardiovascularSOFA) ? m_CardiovascularSOFA->operator==(*rhs.m_CardiovascularSOFA) : m_CardiovascularSOFA == rhs.m_CardiovascularSOFA)
    && ((m_CentralNervousSOFA && rhs.m_CentralNervousSOFA) ? m_CentralNervousSOFA->operator==(*rhs.m_CentralNervousSOFA) : m_CentralNervousSOFA == rhs.m_CentralNervousSOFA)
    && ((m_RenalSOFA && rhs.m_RenalSOFA) ? m_RenalSOFA->operator==(*rhs.m_RenalSOFA) : m_RenalSOFA == rhs.m_RenalSOFA);
}
bool SESequentialOrganFailureAssessment::operator!=(SESequentialOrganFailureAssessment const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}