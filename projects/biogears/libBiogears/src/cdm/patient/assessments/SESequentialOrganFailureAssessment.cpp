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

SESequentialOrganFailureAssessment::~SESequentialOrganFailureAssessment()
{
  Clear();
}

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

bool SESequentialOrganFailureAssessment::Load(const CDM::SequentialOrganFailureAssessmentData& in)
{
  SEPatientAssessment::Load(in);
  GetRespirationSOFA().Load(in.RespirationSOFA());
  GetCoagulationSOFA().Load(in.CoagulationSOFA());
  GetLiverSOFA().Load(in.LiverSOFA());
  GetCardiovascularSOFA().Load(in.CardiovascularSOFA());
  GetCentralNervousSOFA().Load(in.CentralNervousSOFA());
  GetRenalSOFA().Load(in.RenalSOFA());
  return true;
}

CDM::SequentialOrganFailureAssessmentData* SESequentialOrganFailureAssessment::Unload()
{
  CDM::SequentialOrganFailureAssessmentData* data = new CDM::SequentialOrganFailureAssessmentData();
  Unload(*data);
  return data;
}

void SESequentialOrganFailureAssessment::Unload(CDM::SequentialOrganFailureAssessmentData& data)
{
  SEPatientAssessment::Unload(data);
  if (HasRespirationSOFA())
    data.RespirationSOFA(std::unique_ptr<CDM::ScalarData>(m_RespirationSOFA->Unload()));
  if (HasCoagulationSOFA())
    data.CoagulationSOFA(std::unique_ptr<CDM::ScalarData>(m_CoagulationSOFA->Unload()));
  if (HasLiverSOFA())
    data.LiverSOFA(std::unique_ptr<CDM::ScalarData>(m_LiverSOFA->Unload()));
  if (HasCardiovascularSOFA())
    data.CardiovascularSOFA(std::unique_ptr<CDM::ScalarData>(m_CardiovascularSOFA->Unload()));
  if (HasCentralNervousSOFA())
    data.CentralNervousSOFA(std::unique_ptr<CDM::ScalarData>(m_CentralNervousSOFA->Unload()));
  if (HasRenalSOFA())
    data.RenalSOFA(std::unique_ptr<CDM::ScalarData>(m_RenalSOFA->Unload()));
}

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

}