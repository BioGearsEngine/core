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

#include <biogears/cdm/properties/SEScalarElectricResistance.h>

namespace biogears {
const ElectricResistanceUnit ElectricResistanceUnit::Ohm("ohm");

template class SEScalarQuantity<ElectricResistanceUnit>;

ElectricResistanceUnit::ElectricResistanceUnit(const char* u)
  : ElectricResistanceUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
ElectricResistanceUnit::ElectricResistanceUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
ElectricResistanceUnit::~ElectricResistanceUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarElectricResistance::SEScalarElectricResistance()
{
}
//-------------------------------------------------------------------------------
SEScalarElectricResistance::~SEScalarElectricResistance()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarElectricResistanceData* SEScalarElectricResistance::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarElectricResistanceData* data(new CDM::ScalarElectricResistanceData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool ElectricResistanceUnit::IsValidUnit(const char* unit)
{
  if (strcmp(Ohm.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool ElectricResistanceUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const ElectricResistanceUnit& ElectricResistanceUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(Ohm.GetString(), unit) == 0)
    return Ohm;
  std::stringstream err;
  err << unit << " is not a valid ElectricResistance unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const ElectricResistanceUnit& ElectricResistanceUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool ElectricResistanceUnit::operator==(const ElectricResistanceUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool ElectricResistanceUnit::operator!=(const ElectricResistanceUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarElectricResistance::operator==(const SEScalarElectricResistance& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarElectricResistance::operator!=(const SEScalarElectricResistance& obj) const
{
  return !(*this == obj);
}
}