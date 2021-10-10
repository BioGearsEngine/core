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

#include <biogears/cdm/properties/SEScalarVolumePerTimePressureArea.h>

namespace biogears {
const VolumePerTimePressureAreaUnit VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2("mL/min mmHg m^2");
const VolumePerTimePressureAreaUnit VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2("mL/s mmHg m^2");

template class SEScalarQuantity<VolumePerTimePressureAreaUnit>;
VolumePerTimePressureAreaUnit::VolumePerTimePressureAreaUnit(const char* u)
  : VolumePerTimePressureAreaUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
VolumePerTimePressureAreaUnit::VolumePerTimePressureAreaUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
VolumePerTimePressureAreaUnit::~VolumePerTimePressureAreaUnit(){
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTimePressureArea::SEScalarVolumePerTimePressureArea()
{
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTimePressureArea::~SEScalarVolumePerTimePressureArea()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarVolumePerTimePressureAreaData* SEScalarVolumePerTimePressureArea::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarVolumePerTimePressureAreaData* data(new CDM::ScalarVolumePerTimePressureAreaData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool VolumePerTimePressureAreaUnit::IsValidUnit(const char* unit)
{
  if (strcmp(mL_Per_min_mmHg_m2.GetString(), unit) == 0)
    return true;
  if (strcmp(mL_Per_s_mmHg_m2.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool VolumePerTimePressureAreaUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const VolumePerTimePressureAreaUnit& VolumePerTimePressureAreaUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(mL_Per_min_mmHg_m2.GetString(), unit) == 0)
    return mL_Per_min_mmHg_m2;
  if (strcmp(mL_Per_s_mmHg_m2.GetString(), unit) == 0)
    return mL_Per_s_mmHg_m2;
  std::stringstream err;
  err << unit << " is not a valid VolumePerTimePressureArea unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const VolumePerTimePressureAreaUnit& VolumePerTimePressureAreaUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------

bool VolumePerTimePressureAreaUnit::operator==(const VolumePerTimePressureAreaUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool VolumePerTimePressureAreaUnit::operator!=(const VolumePerTimePressureAreaUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarVolumePerTimePressureArea::operator==(const SEScalarVolumePerTimePressureArea& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarVolumePerTimePressureArea::operator!=(const SEScalarVolumePerTimePressureArea& obj) const
{
  return !(*this == obj);
}
}