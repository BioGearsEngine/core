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

#include <biogears/cdm/properties/SEScalarVolumePerTime.h>

namespace biogears {
const VolumePerTimeUnit VolumePerTimeUnit::L_Per_s("L/s");
const VolumePerTimeUnit VolumePerTimeUnit::mL_Per_s("mL/s");
const VolumePerTimeUnit VolumePerTimeUnit::mL_Per_day("mL/day");
const VolumePerTimeUnit VolumePerTimeUnit::L_Per_day("L/day");
const VolumePerTimeUnit VolumePerTimeUnit::L_Per_min("L/min");
const VolumePerTimeUnit VolumePerTimeUnit::m3_Per_s("m^3/s");
const VolumePerTimeUnit VolumePerTimeUnit::mL_Per_min("mL/min");
const VolumePerTimeUnit VolumePerTimeUnit::mL_Per_hr("mL/hr");

template class SEScalarQuantity<VolumePerTimeUnit>;

VolumePerTimeUnit::VolumePerTimeUnit(const char* u)
  : VolumePerTimeUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
VolumePerTimeUnit::VolumePerTimeUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
VolumePerTimeUnit::~VolumePerTimeUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTime::SEScalarVolumePerTime()
{
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTime::~SEScalarVolumePerTime()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarVolumePerTimeData* SEScalarVolumePerTime::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarVolumePerTimeData* data(new CDM::ScalarVolumePerTimeData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool VolumePerTimeUnit::IsValidUnit(const char* unit)
{
  if (strcmp(L_Per_s.GetString(), unit) == 0)
    return true;
  if (strcmp(mL_Per_s.GetString(), unit) == 0)
    return true;
  if (strcmp(mL_Per_day.GetString(), unit) == 0)
    return true;
  if (strcmp(L_Per_day.GetString(), unit) == 0)
    return true;
  if (strcmp(L_Per_min.GetString(), unit) == 0)
    return true;
  if (strcmp(m3_Per_s.GetString(), unit) == 0)
    return true;
  if (strcmp(mL_Per_min.GetString(), unit) == 0)
    return true;
  if (strcmp(mL_Per_hr.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool VolumePerTimeUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const VolumePerTimeUnit& VolumePerTimeUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(L_Per_s.GetString(), unit) == 0)
    return L_Per_s;
  if (strcmp(mL_Per_s.GetString(), unit) == 0)
    return mL_Per_s;
  if (strcmp(mL_Per_day.GetString(), unit) == 0)
    return mL_Per_day;
  if (strcmp(L_Per_day.GetString(), unit) == 0)
    return L_Per_day;
  if (strcmp(L_Per_min.GetString(), unit) == 0)
    return L_Per_min;
  if (strcmp(m3_Per_s.GetString(), unit) == 0)
    return m3_Per_s;
  if (strcmp(mL_Per_min.GetString(), unit) == 0)
    return mL_Per_min;
  if (strcmp(mL_Per_hr.GetString(), unit) == 0)
    return mL_Per_hr;
  std::stringstream err;
  err << unit << " is not a valid VolumePerTime unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const VolumePerTimeUnit& VolumePerTimeUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool VolumePerTimeUnit::operator==(const VolumePerTimeUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool VolumePerTimeUnit::operator!=(const VolumePerTimeUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarVolumePerTime::operator==(const SEScalarVolumePerTime& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarVolumePerTime::operator!=(const SEScalarVolumePerTime& obj) const
{
  return !(*this == obj);
}
}