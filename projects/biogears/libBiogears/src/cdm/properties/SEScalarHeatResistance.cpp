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

#include <biogears/cdm/properties/SEScalarHeatResistance.h>

namespace biogears {
const HeatResistanceUnit HeatResistanceUnit::K_Per_W("K/W");
const HeatResistanceUnit HeatResistanceUnit::C_Per_W("degC/W");
const HeatResistanceUnit HeatResistanceUnit::K_s_Per_kcal("K s/kcal");
const HeatResistanceUnit HeatResistanceUnit::C_s_Per_kcal("degC s/kcal");

template class SEScalarQuantity<HeatResistanceUnit>;

HeatResistanceUnit::HeatResistanceUnit(const char* u)
  : HeatResistanceUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
HeatResistanceUnit::HeatResistanceUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
HeatResistanceUnit::~HeatResistanceUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarHeatResistance::SEScalarHeatResistance()
{
}
//-------------------------------------------------------------------------------
SEScalarHeatResistance::~SEScalarHeatResistance()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarHeatResistanceData* SEScalarHeatResistance::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarHeatResistanceData* data(new CDM::ScalarHeatResistanceData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool HeatResistanceUnit::IsValidUnit(const char* unit)
{
  if (strcmp(K_Per_W.GetString(), unit) == 0)
    return true;
  if (strcmp(C_Per_W.GetString(), unit) == 0)
    return true;
  if (strcmp(K_s_Per_kcal.GetString(), unit) == 0)
    return true;
  if (strcmp(C_s_Per_kcal.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool HeatResistanceUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const HeatResistanceUnit& HeatResistanceUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(K_Per_W.GetString(), unit) == 0)
    return K_Per_W;
  if (strcmp(C_Per_W.GetString(), unit) == 0)
    return C_Per_W;
  if (strcmp(K_s_Per_kcal.GetString(), unit) == 0)
    return K_s_Per_kcal;
  if (strcmp(C_s_Per_kcal.GetString(), unit) == 0)
    return C_s_Per_kcal;
  std::stringstream err;
  err << unit << " is not a valid HeatResistance unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const HeatResistanceUnit& HeatResistanceUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool HeatResistanceUnit::operator==(const HeatResistanceUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool HeatResistanceUnit::operator!=(const HeatResistanceUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------
bool SEScalarHeatResistance::operator==(const SEScalarHeatResistance& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarHeatResistance::operator!=(const SEScalarHeatResistance& obj) const
{
  return !(*this == obj);
}
}