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

#include <biogears/cdm/properties/SEScalarTimeMassPerVolume.h>

namespace biogears {
const TimeMassPerVolumeUnit TimeMassPerVolumeUnit::s_g_Per_L("s g/L");
const TimeMassPerVolumeUnit TimeMassPerVolumeUnit::min_g_Per_L("min g/L");
const TimeMassPerVolumeUnit TimeMassPerVolumeUnit::hr_g_Per_L("hr g/L");
const TimeMassPerVolumeUnit TimeMassPerVolumeUnit::s_ug_Per_mL("s ug/mL");
const TimeMassPerVolumeUnit TimeMassPerVolumeUnit::min_ug_Per_mL("min ug/mL");
const TimeMassPerVolumeUnit TimeMassPerVolumeUnit::hr_ug_Per_mL("hr ug/mL");

template class SEScalarQuantity<TimeMassPerVolumeUnit>;

TimeMassPerVolumeUnit::TimeMassPerVolumeUnit(const char* u)
  : TimeMassPerVolumeUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
TimeMassPerVolumeUnit::TimeMassPerVolumeUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
TimeMassPerVolumeUnit::~TimeMassPerVolumeUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarTimeMassPerVolume::SEScalarTimeMassPerVolume()
{
}
//-------------------------------------------------------------------------------
SEScalarTimeMassPerVolume::~SEScalarTimeMassPerVolume()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarTimeMassPerVolumeData* SEScalarTimeMassPerVolume::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarTimeMassPerVolumeData* data(new CDM::ScalarTimeMassPerVolumeData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool TimeMassPerVolumeUnit::IsValidUnit(const char* unit)
{
  if (strcmp(s_g_Per_L.GetString(), unit) == 0)
    return true;
  if (strcmp(min_g_Per_L.GetString(), unit) == 0)
    return true;
  if (strcmp(hr_g_Per_L.GetString(), unit) == 0)
    return true;
  if (strcmp(s_ug_Per_mL.GetString(), unit) == 0)
    return true;
  if (strcmp(min_ug_Per_mL.GetString(), unit) == 0)
    return true;
  if (strcmp(hr_ug_Per_mL.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool TimeMassPerVolumeUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const TimeMassPerVolumeUnit& TimeMassPerVolumeUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(s_g_Per_L.GetString(), unit) == 0)
    return s_g_Per_L;
  if (strcmp(min_g_Per_L.GetString(), unit) == 0)
    return min_g_Per_L;
  if (strcmp(hr_g_Per_L.GetString(), unit) == 0)
    return hr_g_Per_L;
  if (strcmp(s_ug_Per_mL.GetString(), unit) == 0)
    return s_ug_Per_mL;
  if (strcmp(min_ug_Per_mL.GetString(), unit) == 0)
    return min_ug_Per_mL;
  if (strcmp(hr_ug_Per_mL.GetString(), unit) == 0)
    return hr_ug_Per_mL;
  std::stringstream err;
  err << unit << " is not a valid TimeMassPerVolume unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const TimeMassPerVolumeUnit& TimeMassPerVolumeUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool TimeMassPerVolumeUnit::operator==(const TimeMassPerVolumeUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool TimeMassPerVolumeUnit::operator!=(const TimeMassPerVolumeUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarTimeMassPerVolume::operator==(const SEScalarTimeMassPerVolume& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarTimeMassPerVolume::operator!=(const SEScalarTimeMassPerVolume& obj) const
{
  return !(*this == obj);
}
}