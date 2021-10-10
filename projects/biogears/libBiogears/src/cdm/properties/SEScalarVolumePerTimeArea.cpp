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

#include <biogears/cdm/properties/SEScalarVolumePerTimeArea.h>

namespace biogears {
const VolumePerTimeAreaUnit VolumePerTimeAreaUnit::mL_Per_min_m2("mL/min m^2");
const VolumePerTimeAreaUnit VolumePerTimeAreaUnit::mL_Per_s_m2("mL/s m^2");
const VolumePerTimeAreaUnit VolumePerTimeAreaUnit::L_Per_min_m2("L/min m^2");

template class SEScalarQuantity<VolumePerTimeAreaUnit>;

VolumePerTimeAreaUnit::VolumePerTimeAreaUnit(const char* u)
  : VolumePerTimeAreaUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
VolumePerTimeAreaUnit::VolumePerTimeAreaUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
VolumePerTimeAreaUnit::~VolumePerTimeAreaUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTimeArea::SEScalarVolumePerTimeArea()
{

}
//-------------------------------------------------------------------------------
SEScalarVolumePerTimeArea::~SEScalarVolumePerTimeArea()
{

}
//-------------------------------------------------------------------------------
CDM::ScalarVolumePerTimeAreaData* SEScalarVolumePerTimeArea::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarVolumePerTimeAreaData* data(new CDM::ScalarVolumePerTimeAreaData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool VolumePerTimeAreaUnit::IsValidUnit(const char* unit)
{
  if (strcmp(mL_Per_min_m2.GetString(), unit) == 0)
    return true;
  if (strcmp(mL_Per_s_m2.GetString(), unit) == 0)
    return true;
  if (strcmp(L_Per_min_m2.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool VolumePerTimeAreaUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const VolumePerTimeAreaUnit& VolumePerTimeAreaUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(mL_Per_min_m2.GetString(), unit) == 0)
    return mL_Per_min_m2;
  if (strcmp(mL_Per_s_m2.GetString(), unit) == 0)
    return mL_Per_s_m2;
  if (strcmp(L_Per_min_m2.GetString(), unit) == 0)
    return L_Per_min_m2;
  std::stringstream err;
  err << unit << " is not a valid VolumePerTimeArea unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const VolumePerTimeAreaUnit& VolumePerTimeAreaUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------

bool VolumePerTimeAreaUnit::operator==(const VolumePerTimeAreaUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool VolumePerTimeAreaUnit::operator!=(const VolumePerTimeAreaUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarVolumePerTimeArea::operator==(const SEScalarVolumePerTimeArea& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarVolumePerTimeArea::operator!=(const SEScalarVolumePerTimeArea& obj) const
{
  return !(*this == obj);
}
}