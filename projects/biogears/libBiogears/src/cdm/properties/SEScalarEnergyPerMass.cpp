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

#include <biogears/cdm/properties/SEScalarEnergyPerMass.h>

namespace biogears {
const EnergyPerMassUnit EnergyPerMassUnit::J_Per_kg("J/kg");
const EnergyPerMassUnit EnergyPerMassUnit::kJ_Per_kg("kJ/kg");
const EnergyPerMassUnit EnergyPerMassUnit::mJ_Per_kg("mJ/kg");
const EnergyPerMassUnit EnergyPerMassUnit::kcal_Per_kg("kcal/kg");

template class SEScalarQuantity<EnergyPerMassUnit>;

EnergyPerMassUnit::EnergyPerMassUnit(const char* u)
  : EnergyPerMassUnit(std::string { u })
{
}
//-----------------------------------------------------------------------------
EnergyPerMassUnit::EnergyPerMassUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
EnergyPerMassUnit::~EnergyPerMassUnit()
{
}
//-----------------------------------------------------------------------------
SEScalarEnergyPerMass::SEScalarEnergyPerMass()
{
}
//-----------------------------------------------------------------------------
SEScalarEnergyPerMass::~SEScalarEnergyPerMass()
{
}
//-----------------------------------------------------------------------------
CDM::ScalarEnergyPerMassData* SEScalarEnergyPerMass::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarEnergyPerMassData* data(new CDM::ScalarEnergyPerMassData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
bool EnergyPerMassUnit::IsValidUnit(const char* unit)
{
  if (strcmp(J_Per_kg.GetString(), unit) == 0)
    return true;
  if (strcmp(kJ_Per_kg.GetString(), unit) == 0)
    return true;
  if (strcmp(mJ_Per_kg.GetString(), unit) == 0)
    return true;
  if (strcmp(kcal_Per_kg.GetString(), unit) == 0)
    return true;
  return false;
}
//-----------------------------------------------------------------------------
bool EnergyPerMassUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
const EnergyPerMassUnit& EnergyPerMassUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(J_Per_kg.GetString(), unit) == 0)
    return J_Per_kg;
  if (strcmp(kJ_Per_kg.GetString(), unit) == 0)
    return kJ_Per_kg;
  if (strcmp(mJ_Per_kg.GetString(), unit) == 0)
    return mJ_Per_kg;
  if (strcmp(kcal_Per_kg.GetString(), unit) == 0)
    return kcal_Per_kg;
  std::stringstream err;
  err << unit << " is not a valid EnergyPerMass unit";
  throw CommonDataModelException(err.str());
}
//-----------------------------------------------------------------------------
const EnergyPerMassUnit& EnergyPerMassUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
bool EnergyPerMassUnit::operator==(const EnergyPerMassUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool EnergyPerMassUnit::operator!=(const EnergyPerMassUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarEnergyPerMass::operator==(const SEScalarEnergyPerMass& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarEnergyPerMass::operator!=(const SEScalarEnergyPerMass& obj) const
{
  return !(*this == obj);
}
}