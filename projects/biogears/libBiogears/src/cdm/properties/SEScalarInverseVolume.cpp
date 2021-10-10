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

#include <biogears/cdm/properties/SEScalarInverseVolume.h>

namespace biogears {
const InverseVolumeUnit InverseVolumeUnit::Inverse_L("1/L");
const InverseVolumeUnit InverseVolumeUnit::Inverse_mL("1/mL");

template class SEScalarQuantity<InverseVolumeUnit>;

InverseVolumeUnit::InverseVolumeUnit(const char* u)
  : InverseVolumeUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
InverseVolumeUnit::InverseVolumeUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
InverseVolumeUnit::~InverseVolumeUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarInverseVolume::SEScalarInverseVolume()
{
}
//-------------------------------------------------------------------------------
SEScalarInverseVolume::~SEScalarInverseVolume()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarInverseVolumeData* SEScalarInverseVolume::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarInverseVolumeData* data(new CDM::ScalarInverseVolumeData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool InverseVolumeUnit::IsValidUnit(const char* unit)
{
  if (strcmp(Inverse_L.GetString(), unit) == 0)
    return true;
  if (strcmp(Inverse_mL.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool InverseVolumeUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const InverseVolumeUnit& InverseVolumeUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(Inverse_L.GetString(), unit) == 0)
    return Inverse_L;
  if (strcmp(Inverse_mL.GetString(), unit) == 0)
    return Inverse_mL;
  std::stringstream err;
  err << unit << " is not a valid Volume unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const InverseVolumeUnit& InverseVolumeUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool InverseVolumeUnit::operator==(const InverseVolumeUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool InverseVolumeUnit::operator!=(const InverseVolumeUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------
bool SEScalarInverseVolume::operator==(const SEScalarInverseVolume& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarInverseVolume::operator!=(const SEScalarInverseVolume& obj) const
{
  return !(*this == obj);
}
}