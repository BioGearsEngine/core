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

#include <biogears/cdm/properties/SEScalarElectricPotential.h>

namespace biogears {
const ElectricPotentialUnit ElectricPotentialUnit::V("V");
const ElectricPotentialUnit ElectricPotentialUnit::mV("mV");

template class SEScalarQuantity<ElectricPotentialUnit>;

ElectricPotentialUnit::ElectricPotentialUnit(const char* u)
  : ElectricPotentialUnit(std::string { u })
{
}
//-----------------------------------------------------------------------------
ElectricPotentialUnit::ElectricPotentialUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-----------------------------------------------------------------------------
ElectricPotentialUnit::~ElectricPotentialUnit()
{
}
//-----------------------------------------------------------------------------
SEScalarElectricPotential::SEScalarElectricPotential()
{
}
//-----------------------------------------------------------------------------
SEScalarElectricPotential::~SEScalarElectricPotential()
{
}
//-----------------------------------------------------------------------------
CDM::ScalarElectricPotentialData* SEScalarElectricPotential::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarElectricPotentialData* data(new CDM::ScalarElectricPotentialData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
bool ElectricPotentialUnit::IsValidUnit(const char* unit)
{
  if (strcmp(V.GetString(), unit) == 0)
    return true;
  if (strcmp(mV.GetString(), unit) == 0)
    return true;
  return false;
}
//-----------------------------------------------------------------------------
bool ElectricPotentialUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
const ElectricPotentialUnit& ElectricPotentialUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(V.GetString(), unit) == 0)
    return V;
  if (strcmp(mV.GetString(), unit) == 0)
    return mV;
  std::stringstream err;
  err << unit << " is not a valid Electric Potential unit";
  throw CommonDataModelException(err.str());
}
//-----------------------------------------------------------------------------
const ElectricPotentialUnit& ElectricPotentialUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
bool ElectricPotentialUnit::operator==(const ElectricPotentialUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool ElectricPotentialUnit::operator!=(const ElectricPotentialUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarElectricPotential::operator==(const SEScalarElectricPotential& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarElectricPotential::operator!=(const SEScalarElectricPotential& obj) const
{
  return !(*this == obj);
}
}