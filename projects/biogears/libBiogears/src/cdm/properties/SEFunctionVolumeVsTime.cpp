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
#include <biogears/cdm/properties/SEFunctionVolumeVsTime.h>

#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>

namespace biogears {
SEFunctionVolumeVsTime::SEFunctionVolumeVsTime()
  : SEFunction()
{
}

SEFunctionVolumeVsTime::~SEFunctionVolumeVsTime()
{
  Clear();
}

void SEFunctionVolumeVsTime::Clear()
{
  SEFunction::Clear();
  m_TimeUnit = nullptr;
  m_VolumeUnit = nullptr;
}

bool SEFunctionVolumeVsTime::Load(const CDM::FunctionVolumeVsTimeData& in)
{
  if (!SEFunction::Load(in))
    return false;
  m_TimeUnit = &TimeUnit::GetCompoundUnit(in.IndependentUnit().get());
  m_VolumeUnit = &VolumeUnit::GetCompoundUnit(in.DependentUnit().get());
  return IsValid();
}

CDM::FunctionVolumeVsTimeData* SEFunctionVolumeVsTime::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::FunctionVolumeVsTimeData* data(new CDM::FunctionVolumeVsTimeData());
  Unload(*data);
  return data;
}

void SEFunctionVolumeVsTime::Unload(CDM::FunctionVolumeVsTimeData& data) const
{
  SEFunction::Unload(data);
  data.IndependentUnit(m_TimeUnit->GetString());
  data.DependentUnit(m_VolumeUnit->GetString());
}

double SEFunctionVolumeVsTime::GetTimeValue(unsigned int index, const TimeUnit& unit)
{
  if (m_TimeUnit == nullptr)
    throw CommonDataModelException("No time units have been set");
  if (index >= m_Independent.size())
    throw CommonDataModelException("Independent index out of bounds");
  return Convert(m_Independent[index], *m_TimeUnit, unit);
}
std::vector<double>& SEFunctionVolumeVsTime::GetTime()
{
  return m_Independent;
}
const TimeUnit* SEFunctionVolumeVsTime::GetTimeUnit()
{
  return m_TimeUnit;
}
void SEFunctionVolumeVsTime::SetTimeUnit(const TimeUnit& unit)
{
  m_TimeUnit = &unit;
}

double SEFunctionVolumeVsTime::GetVolumeValue(unsigned int index, const VolumeUnit& unit)
{
  if (m_VolumeUnit == nullptr)
    throw CommonDataModelException("No volume units have been set");
  if (index >= m_Dependent.size())
    throw CommonDataModelException("Dependent index out of bounds");
  return Convert(m_Dependent[index], *m_VolumeUnit, unit);
}
std::vector<double>& SEFunctionVolumeVsTime::GetVolume()
{
  return m_Dependent;
}
const VolumeUnit* SEFunctionVolumeVsTime::GetVolumeUnit()
{
  return m_VolumeUnit;
}
void SEFunctionVolumeVsTime::SetVolumeUnit(const VolumeUnit& unit)
{
  m_VolumeUnit = &unit;
}
}