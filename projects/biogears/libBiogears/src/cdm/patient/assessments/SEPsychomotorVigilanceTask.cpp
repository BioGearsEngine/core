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
SEPsychomotorVigilanceTask::SEPsychomotorVigilanceTask()
{
  m_AttentionLapses = nullptr;
  m_ReactionTime = nullptr;
}

SEPsychomotorVigilanceTask::~SEPsychomotorVigilanceTask()
{
  Clear();
}

void SEPsychomotorVigilanceTask::Clear()
{
  SEPatientAssessment::Clear();
  SAFE_DELETE(m_AttentionLapses);
  SAFE_DELETE(m_ReactionTime);
}

void SEPsychomotorVigilanceTask::Reset()
{
  SEPatientAssessment::Reset();
  INVALIDATE_PROPERTY(m_AttentionLapses);
  INVALIDATE_PROPERTY(m_ReactionTime);
}

bool SEPsychomotorVigilanceTask::Load(const CDM::PsychomotorVigilanceTaskData& in)
{
  SEPatientAssessment::Load(in);
  // TODO
  return true;
}

CDM::PsychomotorVigilanceTaskData* SEPsychomotorVigilanceTask::Unload()
{
  CDM::PsychomotorVigilanceTaskData* data = new CDM::PsychomotorVigilanceTaskData();
  Unload(*data);
  return data;
}

void SEPsychomotorVigilanceTask::Unload(CDM::PsychomotorVigilanceTaskData& data)
{
  SEPatientAssessment::Unload(data);
  if (HasAttentionLapses())
    data.AttentionLapses(std::unique_ptr<CDM::ScalarData>(m_AttentionLapses->Unload()));
  if (HasReactionTime())
    data.ReactionTime(std::unique_ptr<CDM::ScalarTimeData>(m_ReactionTime->Unload()));
}

bool SEPsychomotorVigilanceTask::HasAttentionLapses()
{
  return m_AttentionLapses == nullptr ? false : m_AttentionLapses->IsValid();
}
SEScalar& SEPsychomotorVigilanceTask::GetAttentionLapses()
{
  if (m_AttentionLapses == nullptr)
    m_AttentionLapses = new SEScalar();
  return *m_AttentionLapses;
}

bool SEPsychomotorVigilanceTask::HasReactionTime()
{
  return m_ReactionTime == nullptr ? false : m_ReactionTime->IsValid();
}
SEScalarTime& SEPsychomotorVigilanceTask::GetReactionTime()
{
  if (m_ReactionTime == nullptr)
    m_ReactionTime = new SEScalarTime();
  return *m_ReactionTime;
}

}