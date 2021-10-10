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

#include <biogears/cdm/properties/SEScalarPower.h>

namespace biogears {
const PowerUnit PowerUnit::W("W");
const PowerUnit PowerUnit::kcal_Per_s("kcal/s");
const PowerUnit PowerUnit::kcal_Per_hr("kcal/hr");
const PowerUnit PowerUnit::kcal_Per_day("kcal/day");
const PowerUnit PowerUnit::J_Per_s("J/s");
const PowerUnit PowerUnit::BTU_Per_hr("BTU/hr");

template class SEScalarQuantity<PowerUnit>;

PowerUnit::PowerUnit(const char* u)
  : PowerUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
PowerUnit::PowerUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
PowerUnit::~PowerUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarPower::SEScalarPower()
{
}
//-------------------------------------------------------------------------------
SEScalarPower::~SEScalarPower()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarPowerData* SEScalarPower::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarPowerData* data(new CDM::ScalarPowerData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool PowerUnit::IsValidUnit(const char* unit)
{
  if (strcmp(W.GetString(), unit) == 0)
    return true;
  if (strcmp(kcal_Per_s.GetString(), unit) == 0)
    return true;
  if (strcmp(kcal_Per_hr.GetString(), unit) == 0)
    return true;
  if (strcmp(kcal_Per_day.GetString(), unit) == 0)
    return true;
  if (strcmp(J_Per_s.GetString(), unit) == 0)
    return true;
  if (strcmp(BTU_Per_hr.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool PowerUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const PowerUnit& PowerUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(W.GetString(), unit) == 0)
    return W;
  if (strcmp(kcal_Per_s.GetString(), unit) == 0)
    return kcal_Per_s;
  if (strcmp(kcal_Per_hr.GetString(), unit) == 0)
    return kcal_Per_hr;
  if (strcmp(kcal_Per_day.GetString(), unit) == 0)
    return kcal_Per_day;
  if (strcmp(J_Per_s.GetString(), unit) == 0)
    return J_Per_s;
  if (strcmp(BTU_Per_hr.GetString(), unit) == 0)
    return BTU_Per_hr;
  std::stringstream err;
  err << unit << " is not a valid Power unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const PowerUnit& PowerUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool PowerUnit::operator==(const PowerUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool PowerUnit::operator!=(const PowerUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarPower::operator==(const SEScalarPower& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarPower::operator!=(const SEScalarPower& obj) const
{
  return !(*this == obj);
}
}