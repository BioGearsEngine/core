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
#include <biogears/cdm/patient/assessments/SEPsychomotorVigilanceTask.h>

#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/system/physiology/SENervousSystem.h>

namespace biogears {
SECompleteBloodCount::SECompleteBloodCount(Logger* logger)
  : SEPatientAssessment(logger)
{
  m_Hematocrit = nullptr;
  m_Hemoglobin = nullptr;
}

SECompleteBloodCount::~SECompleteBloodCount()
{
  Clear();
}

void SECompleteBloodCount::Clear()
{
  SEPatientAssessment::Clear();
  SAFE_DELETE(m_Hematocrit);
  SAFE_DELETE(m_Hemoglobin);
}

void SECompleteBloodCount::Reset()
{
  SEPatientAssessment::Reset();
  INVALIDATE_PROPERTY(m_Hematocrit);
  INVALIDATE_PROPERTY(m_Hemoglobin);
}

bool SECompleteBloodCount::Load(const CDM::CompleteBloodCountData& in)
{
  SEPatientAssessment::Load(in);
  // TODO
  return true;
}

CDM::CompleteBloodCountData* SECompleteBloodCount::Unload()
{
  CDM::CompleteBloodCountData* data = new CDM::CompleteBloodCountData();
  Unload(*data);
  return data;
}

void SECompleteBloodCount::Unload(CDM::CompleteBloodCountData& data)
{
  SEPatientAssessment::Unload(data);
  if (HasHematocrit())
    data.Hematocrit(std::unique_ptr<CDM::ScalarFractionData>(m_Hematocrit->Unload()));
  if (HasHemoglobin())
    data.Hemoglobin(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_Hemoglobin->Unload()));
}

bool SECompleteBloodCount::HasHematocrit()
{
  return m_Hematocrit == nullptr ? false : m_Hematocrit->IsValid();
}
SEScalarFraction& SECompleteBloodCount::GetHematocrit()
{
  if (m_Hematocrit == nullptr)
    m_Hematocrit = new SEScalarFraction();
  return *m_Hematocrit;
}

bool SECompleteBloodCount::HasHemoglobin()
{
  return m_Hemoglobin == nullptr ? false : m_Hemoglobin->IsValid();
}
SEScalarMassPerVolume& SECompleteBloodCount::GetHemoglobin()
{
  if (m_Hemoglobin == nullptr)
    m_Hemoglobin = new SEScalarMassPerVolume();
  return *m_Hemoglobin;
}

}