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

#include <biogears/cdm/properties/SEScalarHeatCapacitancePerMass.h>
namespace biogears {
const HeatCapacitancePerMassUnit HeatCapacitancePerMassUnit::J_Per_K_kg("J/K kg");
const HeatCapacitancePerMassUnit HeatCapacitancePerMassUnit::kJ_Per_K_kg("kJ/K kg");
const HeatCapacitancePerMassUnit HeatCapacitancePerMassUnit::kcal_Per_K_kg("kcal/K kg");
const HeatCapacitancePerMassUnit HeatCapacitancePerMassUnit::kcal_Per_C_kg("kcal/degC kg");

template class SEScalarQuantity<HeatCapacitancePerMassUnit>;

HeatCapacitancePerMassUnit::HeatCapacitancePerMassUnit(const char* u)
  : HeatCapacitancePerMassUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
HeatCapacitancePerMassUnit::HeatCapacitancePerMassUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
HeatCapacitancePerMassUnit::~HeatCapacitancePerMassUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarHeatCapacitancePerMass::SEScalarHeatCapacitancePerMass()
{
}
//-------------------------------------------------------------------------------
SEScalarHeatCapacitancePerMass::~SEScalarHeatCapacitancePerMass()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarHeatCapacitancePerMassData* SEScalarHeatCapacitancePerMass::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarHeatCapacitancePerMassData* data(new CDM::ScalarHeatCapacitancePerMassData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool HeatCapacitancePerMassUnit::IsValidUnit(const char* unit)
{
  if (strcmp(J_Per_K_kg.GetString(), unit) == 0)
    return true;
  if (strcmp(kJ_Per_K_kg.GetString(), unit) == 0)
    return true;
  if (strcmp(kcal_Per_K_kg.GetString(), unit) == 0)
    return true;
  if (strcmp(kcal_Per_C_kg.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool HeatCapacitancePerMassUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const HeatCapacitancePerMassUnit& HeatCapacitancePerMassUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(J_Per_K_kg.GetString(), unit) == 0)
    return J_Per_K_kg;
  if (strcmp(kJ_Per_K_kg.GetString(), unit) == 0)
    return kJ_Per_K_kg;
  if (strcmp(kcal_Per_K_kg.GetString(), unit) == 0)
    return kcal_Per_K_kg;
  if (strcmp(kcal_Per_C_kg.GetString(), unit) == 0)
    return kcal_Per_C_kg;
  std::stringstream err;
  err << unit << " is not a valid HeatCapacitancePerMass unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const HeatCapacitancePerMassUnit& HeatCapacitancePerMassUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool HeatCapacitancePerMassUnit::operator==(const HeatCapacitancePerMassUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool HeatCapacitancePerMassUnit::operator!=(const HeatCapacitancePerMassUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------
bool SEScalarHeatCapacitancePerMass::operator==(const SEScalarHeatCapacitancePerMass& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarHeatCapacitancePerMass::operator!=(const SEScalarHeatCapacitancePerMass& obj) const
{
  return !(*this == obj);
}
}