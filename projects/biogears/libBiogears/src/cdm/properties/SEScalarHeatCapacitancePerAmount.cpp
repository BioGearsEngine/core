/*******************************+*******************************************************
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

#include <biogears/cdm/properties/SEScalarHeatCapacitancePerAmount.h>

namespace biogears {
const HeatCapacitancePerAmountUnit HeatCapacitancePerAmountUnit::J_Per_K_mol("J/K mol");

template class SEScalarQuantity<HeatCapacitancePerAmountUnit>;

HeatCapacitancePerAmountUnit::HeatCapacitancePerAmountUnit(const char* u)
  : HeatCapacitancePerAmountUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
HeatCapacitancePerAmountUnit::HeatCapacitancePerAmountUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
HeatCapacitancePerAmountUnit::~HeatCapacitancePerAmountUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarHeatCapacitancePerAmount::SEScalarHeatCapacitancePerAmount()
{
}
//-------------------------------------------------------------------------------
SEScalarHeatCapacitancePerAmount::~SEScalarHeatCapacitancePerAmount()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarHeatCapacitancePerAmountData* SEScalarHeatCapacitancePerAmount::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarHeatCapacitancePerAmountData* data(new CDM::ScalarHeatCapacitancePerAmountData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool HeatCapacitancePerAmountUnit::IsValidUnit(const char* unit)
{
  if (strcmp(J_Per_K_mol.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool HeatCapacitancePerAmountUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const HeatCapacitancePerAmountUnit& HeatCapacitancePerAmountUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(J_Per_K_mol.GetString(), unit) == 0)
    return J_Per_K_mol;
  std::stringstream err;
  err << unit << " is not a valid HeatCapacitancePerAmount unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const HeatCapacitancePerAmountUnit& HeatCapacitancePerAmountUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool HeatCapacitancePerAmountUnit::operator==(const HeatCapacitancePerAmountUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool HeatCapacitancePerAmountUnit::operator!=(const HeatCapacitancePerAmountUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------
bool SEScalarHeatCapacitancePerAmount::operator==(const SEScalarHeatCapacitancePerAmount& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarHeatCapacitancePerAmount::operator!=(const SEScalarHeatCapacitancePerAmount& obj) const
{
  return !(*this == obj);
}
}