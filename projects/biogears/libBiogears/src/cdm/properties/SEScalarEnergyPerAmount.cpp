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

#include <biogears/cdm/properties/SEScalarEnergyPerAmount.h>

namespace biogears {
const EnergyPerAmountUnit EnergyPerAmountUnit::kcal_Per_mol("kcal/mol");
const EnergyPerAmountUnit EnergyPerAmountUnit::kJ_Per_mol("kJ/mol");

template class SEScalarQuantity<EnergyPerAmountUnit>;

EnergyPerAmountUnit::EnergyPerAmountUnit(const char* u)
  : EnergyPerAmountUnit(std::string { u })
{
}
//-----------------------------------------------------------------------------
EnergyPerAmountUnit::EnergyPerAmountUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-----------------------------------------------------------------------------
EnergyPerAmountUnit::~EnergyPerAmountUnit()
{
}
//-----------------------------------------------------------------------------
SEScalarEnergyPerAmount::SEScalarEnergyPerAmount()
{
}
//-----------------------------------------------------------------------------
SEScalarEnergyPerAmount::~SEScalarEnergyPerAmount()
{
}
//-----------------------------------------------------------------------------
CDM::ScalarEnergyPerAmountData* SEScalarEnergyPerAmount::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarEnergyPerAmountData* data(new CDM::ScalarEnergyPerAmountData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
bool EnergyPerAmountUnit::IsValidUnit(const char* unit)
{
  if (strcmp(kcal_Per_mol.GetString(), unit) == 0)
    return true;
  if (strcmp(kJ_Per_mol.GetString(), unit) == 0)
    return true;
  return false;
}
//-----------------------------------------------------------------------------
bool EnergyPerAmountUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
const EnergyPerAmountUnit& EnergyPerAmountUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(kcal_Per_mol.GetString(), unit) == 0)
    return kcal_Per_mol;
  if (strcmp(kJ_Per_mol.GetString(), unit) == 0)
    return kJ_Per_mol;
  std::stringstream err;
  err << unit << " is not a valid EnergyPerAmount unit";
  throw CommonDataModelException(err.str());
}
//-----------------------------------------------------------------------------
const EnergyPerAmountUnit& EnergyPerAmountUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
bool EnergyPerAmountUnit::operator==(const EnergyPerAmountUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool EnergyPerAmountUnit::operator!=(const EnergyPerAmountUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarEnergyPerAmount::operator==(const SEScalarEnergyPerAmount& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarEnergyPerAmount::operator!=(const SEScalarEnergyPerAmount& obj) const
{
  return !(*this == obj);
}
}