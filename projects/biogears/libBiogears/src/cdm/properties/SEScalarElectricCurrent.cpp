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

#include <biogears/cdm/properties/SEScalarElectricCurrent.h>

namespace biogears {
const ElectricCurrentUnit ElectricCurrentUnit::A("A");

template class SEScalarQuantity<ElectricCurrentUnit>;

ElectricCurrentUnit::ElectricCurrentUnit(const char* u)
  : ElectricCurrentUnit(std::string { u })
{
}
//-----------------------------------------------------------------------------
ElectricCurrentUnit::ElectricCurrentUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-----------------------------------------------------------------------------
ElectricCurrentUnit::~ElectricCurrentUnit()
{
}
//-----------------------------------------------------------------------------
SEScalarElectricCurrent::SEScalarElectricCurrent()
{
}
//-----------------------------------------------------------------------------
SEScalarElectricCurrent::~SEScalarElectricCurrent()
{
}
//-----------------------------------------------------------------------------
CDM::ScalarElectricCurrentData* SEScalarElectricCurrent::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarElectricCurrentData* data(new CDM::ScalarElectricCurrentData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
bool ElectricCurrentUnit::IsValidUnit(const char* unit)
{
  if (strcmp(A.GetString(), unit) == 0)
    return true;
  return false;
}
//-----------------------------------------------------------------------------
bool ElectricCurrentUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
const ElectricCurrentUnit& ElectricCurrentUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(A.GetString(), unit) == 0)
    return A;
  std::stringstream err;
  err << unit << " is not a valid ElectricCurrent unit";
  throw CommonDataModelException(err.str());
}
//-----------------------------------------------------------------------------
const ElectricCurrentUnit& ElectricCurrentUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
bool ElectricCurrentUnit::operator==(const ElectricCurrentUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool ElectricCurrentUnit::operator!=(const ElectricCurrentUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarElectricCurrent::operator==(const SEScalarElectricCurrent& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarElectricCurrent::operator!=(const SEScalarElectricCurrent& obj) const
{
  return !(*this == obj);
}
}