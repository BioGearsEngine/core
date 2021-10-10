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

#include <biogears/cdm/properties/SEScalarHeatCapacitance.h>

namespace biogears {
const HeatCapacitanceUnit HeatCapacitanceUnit::J_Per_K("J/K");
const HeatCapacitanceUnit HeatCapacitanceUnit::kJ_Per_K("kJ/K");
const HeatCapacitanceUnit HeatCapacitanceUnit::kcal_Per_K("kcal/K");
const HeatCapacitanceUnit HeatCapacitanceUnit::kcal_Per_C("kcal/degC");

template class SEScalarQuantity<HeatCapacitanceUnit>;

HeatCapacitanceUnit::HeatCapacitanceUnit(const char* u)
  : HeatCapacitanceUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
HeatCapacitanceUnit::HeatCapacitanceUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
HeatCapacitanceUnit::~HeatCapacitanceUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarHeatCapacitance::SEScalarHeatCapacitance()
{
}
//-------------------------------------------------------------------------------
SEScalarHeatCapacitance::~SEScalarHeatCapacitance()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarHeatCapacitanceData* SEScalarHeatCapacitance::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarHeatCapacitanceData* data(new CDM::ScalarHeatCapacitanceData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool HeatCapacitanceUnit::IsValidUnit(const char* unit)
{
  if (strcmp(J_Per_K.GetString(), unit) == 0)
    return true;
  if (strcmp(kJ_Per_K.GetString(), unit) == 0)
    return true;
  if (strcmp(kcal_Per_K.GetString(), unit) == 0)
    return true;
  if (strcmp(kcal_Per_C.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool HeatCapacitanceUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const HeatCapacitanceUnit& HeatCapacitanceUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(J_Per_K.GetString(), unit) == 0)
    return J_Per_K;
  if (strcmp(kJ_Per_K.GetString(), unit) == 0)
    return kJ_Per_K;
  if (strcmp(kcal_Per_K.GetString(), unit) == 0)
    return kcal_Per_K;
  if (strcmp(kcal_Per_C.GetString(), unit) == 0)
    return kcal_Per_C;
  std::stringstream err;
  err << unit << " is not a valid HeatCapacitance unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const HeatCapacitanceUnit& HeatCapacitanceUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool HeatCapacitanceUnit::operator==(const HeatCapacitanceUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool HeatCapacitanceUnit::operator!=(const HeatCapacitanceUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------
bool SEScalarHeatCapacitance::operator==(const SEScalarHeatCapacitance& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarHeatCapacitance::operator!=(const SEScalarHeatCapacitance& obj) const
{
  return !(*this == obj);
}
}