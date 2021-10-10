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

#include <biogears/cdm/properties/SEScalarVolumePerPressure.h>

namespace biogears {
const VolumePerPressureUnit VolumePerPressureUnit::L_Per_Pa("L/Pa");
const VolumePerPressureUnit VolumePerPressureUnit::L_Per_cmH2O("L/cmH2O");

template class SEScalarQuantity<VolumePerPressureUnit>;

VolumePerPressureUnit::VolumePerPressureUnit(const char* u)
  : VolumePerPressureUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
VolumePerPressureUnit::VolumePerPressureUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
VolumePerPressureUnit::~VolumePerPressureUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarVolumePerPressure::SEScalarVolumePerPressure()
{

}
//-------------------------------------------------------------------------------
SEScalarVolumePerPressure::~SEScalarVolumePerPressure()
{

}
//-------------------------------------------------------------------------------
CDM::ScalarVolumePerPressureData* SEScalarVolumePerPressure::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarVolumePerPressureData* data(new CDM::ScalarVolumePerPressureData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool VolumePerPressureUnit::IsValidUnit(const char* unit)
{
  if (strcmp(L_Per_Pa.GetString(), unit) == 0)
    return true;
  if (strcmp(L_Per_cmH2O.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool VolumePerPressureUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const VolumePerPressureUnit& VolumePerPressureUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(L_Per_Pa.GetString(), unit) == 0)
    return L_Per_Pa;
  if (strcmp(L_Per_cmH2O.GetString(), unit) == 0)
    return L_Per_cmH2O;
  std::stringstream err;
  err << unit << " is not a valid VolumePerPressure unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const VolumePerPressureUnit& VolumePerPressureUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool VolumePerPressureUnit::operator==(const VolumePerPressureUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool VolumePerPressureUnit::operator!=(const VolumePerPressureUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarVolumePerPressure::operator==(const SEScalarVolumePerPressure& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarVolumePerPressure::operator!=(const SEScalarVolumePerPressure& obj) const
{
  return !(*this == obj);
}
}