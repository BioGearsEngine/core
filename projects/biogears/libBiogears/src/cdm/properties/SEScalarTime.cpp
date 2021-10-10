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

#include <biogears/cdm/properties/SEScalarTime.h>

namespace biogears {
#pragma push_macro("Time")
#undef min
const TimeUnit TimeUnit::s("s");
const TimeUnit TimeUnit::min("min");
const TimeUnit TimeUnit::hr("hr");
const TimeUnit TimeUnit::day("day");
const TimeUnit TimeUnit::yr("yr");
#pragma pop_macro("Time")

template class SEScalarQuantity<TimeUnit>;

TimeUnit::TimeUnit(const char* u)
  : TimeUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
TimeUnit::TimeUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
TimeUnit::~TimeUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarTime::SEScalarTime()
{
}
//-------------------------------------------------------------------------------
SEScalarTime::~SEScalarTime()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarTimeData* SEScalarTime::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarTimeData* data(new CDM::ScalarTimeData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool TimeUnit::IsValidUnit(const char* unit)
{
  if (strcmp(s.GetString(), unit) == 0)
    return true;
  if (strcmp(min.GetString(), unit) == 0)
    return true;
  if (strcmp(hr.GetString(), unit) == 0)
    return true;
  if (strcmp(day.GetString(), unit) == 0)
    return true;
  if (strcmp(yr.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool TimeUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const TimeUnit& TimeUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(s.GetString(), unit) == 0)
    return s;
  if (strcmp(min.GetString(), unit) == 0)
    return min;
  if (strcmp(hr.GetString(), unit) == 0)
    return hr;
  if (strcmp(day.GetString(), unit) == 0)
    return day;
  if (strcmp(yr.GetString(), unit) == 0)
    return yr;
  std::stringstream err;
  err << unit << " is not a valid Amount unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const TimeUnit& TimeUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool TimeUnit::operator==(const TimeUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool TimeUnit::operator!=(const TimeUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarTime::operator==(const SEScalarTime& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarTime::operator!=(const SEScalarTime& obj) const
{
  return !(*this == obj);
}
}