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
#include <biogears/cdm/scenario/SEScenarioAutoSerialization.h>

#include "io/cdm/Scenario.h"

#include <biogears/cdm/properties/SEScalarTime.h>

#include <biogears/schema/cdm/Scenario.hxx>

namespace biogears {
SEScenarioAutoSerialization::SEScenarioAutoSerialization(Logger* logger)
  : Loggable(logger)
{
  m_Period = nullptr;
  m_PeriodTimeStamps = SEOnOff::Invalid;
  m_AfterActions = SEOnOff::Invalid;
  m_ReloadState = SEOnOff::Invalid;
  m_Directory = "";
  m_FileName = "";
}
//-----------------------------------------------------------------------------
SEScenarioAutoSerialization::~SEScenarioAutoSerialization()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEScenarioAutoSerialization::Clear()
{
  SAFE_DELETE(m_Period);
  m_PeriodTimeStamps = SEOnOff::Invalid;
  m_AfterActions = SEOnOff::Invalid;
  m_AfterActions = SEOnOff::Invalid;
  m_Directory = "";
  m_FileName = "";
}
//-----------------------------------------------------------------------------
bool SEScenarioAutoSerialization::IsValid() const
{
  if (!HasPeriod())
    return false;
  if (m_Period->IsNegative())
    return false;
  if (!HasPeriodTimeStamps())
    return false;
  if (!HasAfterActions())
    return false;
  if (!HasReloadState())
    return false;
  if (!HasDirectory())
    return false;
  if (!HasFileName())
    return false;
  return true;
}
//-----------------------------------------------------------------------------
bool SEScenarioAutoSerialization::Load(const CDM::ScenarioAutoSerializationData& in)
{
  io::Scenario::UnMarshall(in, *this);
  return true;
}
//-----------------------------------------------------------------------------
CDM::ScenarioAutoSerializationData* SEScenarioAutoSerialization::Unload() const
{
  CDM::ScenarioAutoSerializationData* data(new CDM::ScenarioAutoSerializationData());
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEScenarioAutoSerialization::Unload(CDM::ScenarioAutoSerializationData& data) const
{
  io::Scenario::Marshall(*this, data);
}
//-----------------------------------------------------------------------------
bool SEScenarioAutoSerialization::HasPeriod() const
{
  return m_Period == nullptr ? false : m_Period->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarTime& SEScenarioAutoSerialization::GetPeriod()
{
  if (m_Period == nullptr)
    m_Period = new SEScalarTime();
  return *m_Period;
}
//-----------------------------------------------------------------------------
double SEScenarioAutoSerialization::GetPeriod(const TimeUnit& unit) const
{
  if (m_Period == nullptr)
    return SEScalar::dNaN();
  return m_Period->GetValue(unit);
}
//-----------------------------------------------------------------------------
SEOnOff SEScenarioAutoSerialization::GetPeriodTimeStamps() const
{
  return m_PeriodTimeStamps;
}
//-----------------------------------------------------------------------------
void SEScenarioAutoSerialization::SetPeriodTimeStamps(SEOnOff v)
{
  m_PeriodTimeStamps = v;
}
//-----------------------------------------------------------------------------
bool SEScenarioAutoSerialization::HasPeriodTimeStamps() const
{
  return m_PeriodTimeStamps == (SEOnOff::Invalid) ? false : true;
}
//-----------------------------------------------------------------------------
void SEScenarioAutoSerialization::InvalidatePeriodTimeStamps()
{
  m_PeriodTimeStamps = SEOnOff::Invalid;
}
//-----------------------------------------------------------------------------
SEOnOff SEScenarioAutoSerialization::GetAfterActions() const
{
  return m_AfterActions;
}
//-----------------------------------------------------------------------------
void SEScenarioAutoSerialization::SetAfterActions(SEOnOff v)
{
  m_AfterActions = v;
}
//-----------------------------------------------------------------------------
bool SEScenarioAutoSerialization::HasAfterActions() const
{
  return m_AfterActions == (SEOnOff::Invalid) ? false : true;
}
//-----------------------------------------------------------------------------
void SEScenarioAutoSerialization::InvalidateAfterActions()
{
  m_AfterActions = SEOnOff::Invalid;
}
//-----------------------------------------------------------------------------
SEOnOff SEScenarioAutoSerialization::GetReloadState() const
{
  return m_ReloadState;
}
//-----------------------------------------------------------------------------
void SEScenarioAutoSerialization::SetReloadState(SEOnOff v)
{
  m_ReloadState = v;
}
//-----------------------------------------------------------------------------
bool SEScenarioAutoSerialization::HasReloadState() const
{
  return m_ReloadState == (SEOnOff::Invalid) ? false : true;
}
//-----------------------------------------------------------------------------
void SEScenarioAutoSerialization::InvalidateReloadState()
{
  m_ReloadState = SEOnOff::Invalid;
}
//-----------------------------------------------------------------------------
const char* SEScenarioAutoSerialization::GetDirectory() const
{
  return m_Directory.c_str();
}
//-----------------------------------------------------------------------------
void SEScenarioAutoSerialization::SetDirectory(const char* dir)
{
  m_Directory = dir;
}
//-----------------------------------------------------------------------------
void SEScenarioAutoSerialization::SetDirectory(const std::string& dir)
{
  m_Directory = dir;
}
//-----------------------------------------------------------------------------
bool SEScenarioAutoSerialization::HasDirectory() const
{
  return m_Directory.empty() ? false : true;
}
//-----------------------------------------------------------------------------
void SEScenarioAutoSerialization::InvalidateDirectory()
{
  m_Directory = "";
}
//-----------------------------------------------------------------------------
const char* SEScenarioAutoSerialization::GetFileName() const
{
  return m_FileName.c_str();
}
//-----------------------------------------------------------------------------
void SEScenarioAutoSerialization::SetFileName(const char* name)
{
  m_FileName = name;
}
//-----------------------------------------------------------------------------
void SEScenarioAutoSerialization::SetFileName(const std::string& name)
{
  m_FileName = name;
}
//-----------------------------------------------------------------------------
bool SEScenarioAutoSerialization::HasFileName() const
{
  return m_FileName.empty() ? false : true;
}
//-----------------------------------------------------------------------------
void SEScenarioAutoSerialization::InvalidateFileName()
{
  m_FileName = "";
}
//-----------------------------------------------------------------------------
bool SEScenarioAutoSerialization::operator==(SEScenarioAutoSerialization const& rhs) const
{
  if (this == &rhs)
    return true;

  bool equivilant = ((m_Period && rhs.m_Period) ? m_Period->operator==(*rhs.m_Period) : m_Period == rhs.m_Period)
    ;equivilant &= m_PeriodTimeStamps == rhs.m_PeriodTimeStamps
    ;equivilant &= m_AfterActions == rhs.m_AfterActions
    ;equivilant &= m_ReloadState == rhs.m_ReloadState
    ;equivilant &= m_Directory == rhs.m_Directory
    ;equivilant &= m_FileName == rhs.m_FileName;
  return equivilant;
}
bool SEScenarioAutoSerialization::operator!=(SEScenarioAutoSerialization const& rhs) const
{
  return !(*this == rhs);
}
//-----------------------------------------------------------------------------
}