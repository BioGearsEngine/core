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

#include <biogears/cdm/properties/SEScalarVolume.h>

namespace biogears {
const VolumeUnit VolumeUnit::L("L");
const VolumeUnit VolumeUnit::dL("dL");
const VolumeUnit VolumeUnit::mL("mL");
const VolumeUnit VolumeUnit::uL("uL");
const VolumeUnit VolumeUnit::m3("m^3");

template class SEScalarQuantity<VolumeUnit>;

VolumeUnit::VolumeUnit(const char* u)
  : VolumeUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
VolumeUnit::VolumeUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
VolumeUnit::~VolumeUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarVolume::SEScalarVolume()
{
}
//-------------------------------------------------------------------------------
SEScalarVolume::~SEScalarVolume()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarVolumeData* SEScalarVolume::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarVolumeData* data(new CDM::ScalarVolumeData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool VolumeUnit::IsValidUnit(const char* unit)
{
  if (strcmp(L.GetString(), unit) == 0)
    return true;
  if (strcmp(dL.GetString(), unit) == 0)
    return true;
  if (strcmp(mL.GetString(), unit) == 0)
    return true;
  if (strcmp(uL.GetString(), unit) == 0)
    return true;
  if (strcmp(m3.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool VolumeUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const VolumeUnit& VolumeUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(L.GetString(), unit) == 0)
    return L;
  if (strcmp(mL.GetString(), unit) == 0)
    return mL;
  if (strcmp(m3.GetString(), unit) == 0)
    return m3;
  if (strcmp(dL.GetString(), unit) == 0)
    return dL;
  if (strcmp(uL.GetString(), unit) == 0)
    return uL;
  std::stringstream err;
  err << unit << " is not a valid Volume unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const VolumeUnit& VolumeUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool VolumeUnit::operator==(const VolumeUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool VolumeUnit::operator!=(const VolumeUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarVolume::operator==(const SEScalarVolume& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarVolume::operator!=(const SEScalarVolume& obj) const
{
  return !(*this == obj);
}
}