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

#include <biogears/cdm/properties/SEScalarElectricInductance.h>


namespace biogears {
const ElectricInductanceUnit ElectricInductanceUnit::H("H");

template class SEScalarQuantity<ElectricInductanceUnit>;
ElectricInductanceUnit::ElectricInductanceUnit(const char* u)
  : ElectricInductanceUnit(std::string { u })
{
}
//-----------------------------------------------------------------------------
ElectricInductanceUnit::ElectricInductanceUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-----------------------------------------------------------------------------
ElectricInductanceUnit ::~ElectricInductanceUnit()
{
}
//-----------------------------------------------------------------------------
SEScalarElectricInductance::SEScalarElectricInductance()
{
}
//-----------------------------------------------------------------------------
SEScalarElectricInductance::~SEScalarElectricInductance()
{
}
//-----------------------------------------------------------------------------
CDM::ScalarElectricInductanceData* SEScalarElectricInductance::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarElectricInductanceData* data(new CDM::ScalarElectricInductanceData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
bool ElectricInductanceUnit::IsValidUnit(const char* unit)
{
  if (strcmp(H.GetString(), unit) == 0)
    return true;
  return false;
}
//-----------------------------------------------------------------------------
bool ElectricInductanceUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
const ElectricInductanceUnit& ElectricInductanceUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(H.GetString(), unit) == 0)
    return H;
  std::stringstream err;
  err << unit << " is not a valid ElectricInductance unit";
  throw CommonDataModelException(err.str());
}
//-----------------------------------------------------------------------------
const ElectricInductanceUnit& ElectricInductanceUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
bool ElectricInductanceUnit::operator==(const ElectricInductanceUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool ElectricInductanceUnit::operator!=(const ElectricInductanceUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarElectricInductance::operator==(const SEScalarElectricInductance& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarElectricInductance::operator!=(const SEScalarElectricInductance& obj) const
{
  return !(*this == obj);
}
}