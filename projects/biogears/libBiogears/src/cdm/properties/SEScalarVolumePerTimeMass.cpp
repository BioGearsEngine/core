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

#include <biogears/cdm/properties/SEScalarVolumePerTimeMass.h>

namespace biogears {
const VolumePerTimeMassUnit VolumePerTimeMassUnit::L_Per_s_g("L/s g");
const VolumePerTimeMassUnit VolumePerTimeMassUnit::mL_Per_s_g("mL / s g");
const VolumePerTimeMassUnit VolumePerTimeMassUnit::mL_Per_min_kg("mL/min kg");
const VolumePerTimeMassUnit VolumePerTimeMassUnit::mL_Per_s_kg("mL/s kg");
const VolumePerTimeMassUnit VolumePerTimeMassUnit::uL_Per_min_kg("uL/min kg");

template class SEScalarQuantity<VolumePerTimeMassUnit>;

VolumePerTimeMassUnit::VolumePerTimeMassUnit(const char* u)
  : VolumePerTimeMassUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
VolumePerTimeMassUnit::~VolumePerTimeMassUnit()
{
}
//-------------------------------------------------------------------------------
VolumePerTimeMassUnit::VolumePerTimeMassUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTimeMass::SEScalarVolumePerTimeMass()
{

}
//-------------------------------------------------------------------------------
SEScalarVolumePerTimeMass::~SEScalarVolumePerTimeMass()
{

}
//-------------------------------------------------------------------------------
CDM::ScalarVolumePerTimeMassData* SEScalarVolumePerTimeMass::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarVolumePerTimeMassData* data(new CDM::ScalarVolumePerTimeMassData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool VolumePerTimeMassUnit::IsValidUnit(const char* unit)
{
  if (strcmp(L_Per_s_g.GetString(), unit) == 0)
    return true;
  if (strcmp(mL_Per_s_g.GetString(), unit) == 0)
    return true;
  if (strcmp(mL_Per_min_kg.GetString(), unit) == 0)
    return true;
  if (strcmp(mL_Per_s_kg.GetString(), unit) == 0)
    return true;
  if (strcmp(uL_Per_min_kg.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool VolumePerTimeMassUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const VolumePerTimeMassUnit& VolumePerTimeMassUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(L_Per_s_g.GetString(), unit) == 0)
    return L_Per_s_g;
  if (strcmp(mL_Per_s_g.GetString(), unit) == 0)
    return mL_Per_s_g;
  if (strcmp(mL_Per_min_kg.GetString(), unit) == 0)
    return mL_Per_min_kg;
  if (strcmp(mL_Per_s_kg.GetString(), unit) == 0)
    return mL_Per_s_kg;
  if (strcmp(uL_Per_min_kg.GetString(), unit) == 0)
    return uL_Per_min_kg;
  std::stringstream err;
  err << unit << " is not a valid VolumePerTimeMass unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const VolumePerTimeMassUnit& VolumePerTimeMassUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------

bool VolumePerTimeMassUnit::operator==(const VolumePerTimeMassUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool VolumePerTimeMassUnit::operator!=(const VolumePerTimeMassUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarVolumePerTimeMass::operator==(const SEScalarVolumePerTimeMass& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarVolumePerTimeMass::operator!=(const SEScalarVolumePerTimeMass& obj) const
{
  return !(*this == obj);
}
}