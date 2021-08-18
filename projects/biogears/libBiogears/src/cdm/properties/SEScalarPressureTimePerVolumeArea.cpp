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

#include <biogears/cdm/properties/SEScalarPressureTimePerVolumeArea.h>

namespace biogears {
const PressureTimePerVolumeAreaUnit PressureTimePerVolumeAreaUnit::mmHg_min_Per_mL_m2("mmHg min/mL m^2");
const PressureTimePerVolumeAreaUnit PressureTimePerVolumeAreaUnit::mmHg_s_Per_mL_m2("mmHg s/mL m^2");
const PressureTimePerVolumeAreaUnit PressureTimePerVolumeAreaUnit::dyn_s_Per_cm5_m2("dyn s/cm^5 m^2");

template class SEScalarQuantity<PressureTimePerVolumeAreaUnit>;

PressureTimePerVolumeAreaUnit::PressureTimePerVolumeAreaUnit(const char* u)
  : PressureTimePerVolumeAreaUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
PressureTimePerVolumeAreaUnit::PressureTimePerVolumeAreaUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
PressureTimePerVolumeAreaUnit::~PressureTimePerVolumeAreaUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarPressureTimePerVolumeArea::SEScalarPressureTimePerVolumeArea()
{
}
//-------------------------------------------------------------------------------
SEScalarPressureTimePerVolumeArea::~SEScalarPressureTimePerVolumeArea()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarPressureTimePerVolumeAreaData* SEScalarPressureTimePerVolumeArea::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarPressureTimePerVolumeAreaData* data(new CDM::ScalarPressureTimePerVolumeAreaData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool PressureTimePerVolumeAreaUnit::IsValidUnit(const char* unit)
{
  if (strcmp(mmHg_min_Per_mL_m2.GetString(), unit) == 0)
    return true;
  if (strcmp(mmHg_s_Per_mL_m2.GetString(), unit) == 0)
    return true;
  if (strcmp(dyn_s_Per_cm5_m2.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool PressureTimePerVolumeAreaUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const PressureTimePerVolumeAreaUnit& PressureTimePerVolumeAreaUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(mmHg_min_Per_mL_m2.GetString(), unit) == 0)
    return mmHg_min_Per_mL_m2;
  if (strcmp(mmHg_s_Per_mL_m2.GetString(), unit) == 0)
    return mmHg_s_Per_mL_m2;
  if (strcmp(dyn_s_Per_cm5_m2.GetString(), unit) == 0)
    return dyn_s_Per_cm5_m2;
  std::stringstream err;
  err << unit << " is not a valid PressureTimePerVolumeArea unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const PressureTimePerVolumeAreaUnit& PressureTimePerVolumeAreaUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool PressureTimePerVolumeAreaUnit::operator==(const PressureTimePerVolumeAreaUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool PressureTimePerVolumeAreaUnit::operator!=(const PressureTimePerVolumeAreaUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarPressureTimePerVolumeArea::operator==(const SEScalarPressureTimePerVolumeArea& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarPressureTimePerVolumeArea::operator!=(const SEScalarPressureTimePerVolumeArea& obj) const
{
  return !(*this == obj);
}
}