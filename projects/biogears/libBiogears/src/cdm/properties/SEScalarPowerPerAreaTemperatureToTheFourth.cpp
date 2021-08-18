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

#include <biogears/cdm/properties/SEScalarPowerPerAreaTemperatureToTheFourth.h>

namespace biogears {
const PowerPerAreaTemperatureToTheFourthUnit PowerPerAreaTemperatureToTheFourthUnit::W_Per_m2_K4("W/ m^2 K^4");

template class SEScalarQuantity<PowerPerAreaTemperatureToTheFourthUnit>;

PowerPerAreaTemperatureToTheFourthUnit::PowerPerAreaTemperatureToTheFourthUnit(const char* u)
  : PowerPerAreaTemperatureToTheFourthUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
PowerPerAreaTemperatureToTheFourthUnit::PowerPerAreaTemperatureToTheFourthUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
PowerPerAreaTemperatureToTheFourthUnit::~PowerPerAreaTemperatureToTheFourthUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarPowerPerAreaTemperatureToTheFourth::SEScalarPowerPerAreaTemperatureToTheFourth()
{
}
//-------------------------------------------------------------------------------
SEScalarPowerPerAreaTemperatureToTheFourth::~SEScalarPowerPerAreaTemperatureToTheFourth()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarPowerPerAreaTemperatureToTheFourthData* SEScalarPowerPerAreaTemperatureToTheFourth::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarPowerPerAreaTemperatureToTheFourthData* data(new CDM::ScalarPowerPerAreaTemperatureToTheFourthData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool PowerPerAreaTemperatureToTheFourthUnit::IsValidUnit(const char* unit)
{
  if (strcmp(W_Per_m2_K4.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool PowerPerAreaTemperatureToTheFourthUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const PowerPerAreaTemperatureToTheFourthUnit& PowerPerAreaTemperatureToTheFourthUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(W_Per_m2_K4.GetString(), unit) == 0)
    return W_Per_m2_K4;
  std::stringstream err;
  err << unit << " is not a valid PowerPerAreaTemperatureToTheFourth unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const PowerPerAreaTemperatureToTheFourthUnit& PowerPerAreaTemperatureToTheFourthUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool PowerPerAreaTemperatureToTheFourthUnit::operator==(const PowerPerAreaTemperatureToTheFourthUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool PowerPerAreaTemperatureToTheFourthUnit::operator!=(const PowerPerAreaTemperatureToTheFourthUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarPowerPerAreaTemperatureToTheFourth::operator==(const SEScalarPowerPerAreaTemperatureToTheFourth& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarPowerPerAreaTemperatureToTheFourth::operator!=(const SEScalarPowerPerAreaTemperatureToTheFourth& obj) const
{
  return !(*this == obj);
}
}