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
#include <biogears/cdm/engine/PhysiologyEngineStabilization.h>

#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/schema/cdm/EngineConfiguration.hxx>

namespace biogears {
PhysiologyEngineStabilization::PhysiologyEngineStabilization(Logger* logger)
  : Loggable(logger)
{
  m_StabilizationDuration = nullptr;
  m_currentTime_s = 0;
}

PhysiologyEngineStabilization::~PhysiologyEngineStabilization()
{
  Clear();
}

void PhysiologyEngineStabilization::Clear()
{
  m_LogProgress = true;
  SAFE_DELETE(m_StabilizationDuration);
}

bool PhysiologyEngineStabilization::Load(const CDM::PhysiologyEngineStabilizationData& in)
{
  Clear();
  return true;
}

CDM::PhysiologyEngineStabilizationData* PhysiologyEngineStabilization::Unload() const
{
  CDM::PhysiologyEngineStabilizationData* data(new CDM::PhysiologyEngineStabilizationData());
  Unload(*data);
  return data;
}

void PhysiologyEngineStabilization::Unload(CDM::PhysiologyEngineStabilizationData& data) const
{

}

void PhysiologyEngineStabilization::LogProgress(bool b)
{
  m_LogProgress = b;
}


void PhysiologyEngineStabilization::CancelStabilization()
{
  m_Cancelled = true;
}

bool PhysiologyEngineStabilization::HasStabilizationDuration()
{
  return m_StabilizationDuration == nullptr ? false : m_StabilizationDuration->IsValid();
}
SEScalarTime& PhysiologyEngineStabilization::GetStabilizationDuration()
{
  if (m_StabilizationDuration == nullptr)
    m_StabilizationDuration = new SEScalarTime();
  return *m_StabilizationDuration;
}
}