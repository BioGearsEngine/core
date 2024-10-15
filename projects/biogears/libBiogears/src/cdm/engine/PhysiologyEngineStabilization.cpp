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

#include "io/cdm/EngineConfiguration.h"
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/schema/cdm/EngineConfiguration.hxx>

namespace biogears {
//-------------------------------------------------------------------------------
PhysiologyEngineStabilization::PhysiologyEngineStabilization(Logger* logger)
  : Loggable(logger)
{
  m_StabilizationDuration = nullptr;
  m_CurrentTime = nullptr;
}
//-------------------------------------------------------------------------------
PhysiologyEngineStabilization::~PhysiologyEngineStabilization()
{
  Clear();
}
//-------------------------------------------------------------------------------
void PhysiologyEngineStabilization::Clear()
{
  m_LogProgress = true;
  m_Canceled = false;
  SAFE_DELETE(m_StabilizationDuration);
  SAFE_DELETE(m_CurrentTime);
}

//-------------------------------------------------------------------------------
void PhysiologyEngineStabilization::LogProgress(bool b)
{
  m_LogProgress = b;
}
//-------------------------------------------------------------------------------
void PhysiologyEngineStabilization::CancelStabilization()
{
  m_Canceled = true;
}
//-------------------------------------------------------------------------------
bool PhysiologyEngineStabilization::HasStabilizationDuration() const
{
  return m_StabilizationDuration == nullptr ? false : m_StabilizationDuration->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarTime& PhysiologyEngineStabilization::GetStabilizationDuration()
{
  if (m_StabilizationDuration == nullptr)
    m_StabilizationDuration = new SEScalarTime();
  return *m_StabilizationDuration;
}
//-------------------------------------------------------------------------------
bool PhysiologyEngineStabilization::HasCurrentTime() const
{
  return m_CurrentTime == nullptr ? false : m_CurrentTime->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarTime& PhysiologyEngineStabilization::GetCurrentTime()
{
  if (m_CurrentTime == nullptr)
    m_CurrentTime = new SEScalarTime();
  return *m_CurrentTime;
}
//-------------------------------------------------------------------------------

bool PhysiologyEngineStabilization::operator==(PhysiologyEngineStabilization const& rhs) const
{
  bool equivilant = m_Canceled == rhs.m_Canceled;
  ;
  equivilant &= m_LogProgress == rhs.m_LogProgress;
  equivilant &= ((m_CurrentTime && rhs.m_CurrentTime) ? m_CurrentTime->operator==(*rhs.m_CurrentTime)
                                                     : m_CurrentTime == rhs.m_CurrentTime);
  equivilant &= ((m_StabilizationDuration && rhs.m_StabilizationDuration) 
      ? m_StabilizationDuration->operator==(*rhs.m_StabilizationDuration)
      : m_StabilizationDuration == rhs.m_StabilizationDuration);
  return equivilant;
}

bool PhysiologyEngineStabilization::operator!=(PhysiologyEngineStabilization const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}
