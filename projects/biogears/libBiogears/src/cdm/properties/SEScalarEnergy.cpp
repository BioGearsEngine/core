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

#include <biogears/cdm/properties/SEScalarEnergy.h>

namespace biogears {
const EnergyUnit EnergyUnit::J("J");
const EnergyUnit EnergyUnit::mJ("mJ");
const EnergyUnit EnergyUnit::kJ("kJ");
const EnergyUnit EnergyUnit::kcal("kcal");

template class SEScalarQuantity<EnergyUnit>;

EnergyUnit::EnergyUnit(const char* u)
  : EnergyUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
EnergyUnit::EnergyUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
EnergyUnit::~EnergyUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarEnergy::SEScalarEnergy()
{
}
//-------------------------------------------------------------------------------
SEScalarEnergy::~SEScalarEnergy()
{
}

//-------------------------------------------------------------------------------
CDM::ScalarEnergyData* SEScalarEnergy::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarEnergyData* data(new CDM::ScalarEnergyData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool EnergyUnit::IsValidUnit(const char* unit)
{
  if (strcmp(J.GetString(), unit) == 0)
    return true;
  if (strcmp(mJ.GetString(), unit) == 0)
    return true;
  if (strcmp(kJ.GetString(), unit) == 0)
    return true;
  if (strcmp(kcal.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool EnergyUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const EnergyUnit& EnergyUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(J.GetString(), unit) == 0)
    return J;
  if (strcmp(mJ.GetString(), unit) == 0)
    return mJ;
  if (strcmp(kJ.GetString(), unit) == 0)
    return kJ;
  if (strcmp(kcal.GetString(), unit) == 0)
    return kcal;
  std::stringstream err;
  err << unit << " is not a valid Energy unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const EnergyUnit& EnergyUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool EnergyUnit::operator==(const EnergyUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool EnergyUnit::operator!=(const EnergyUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarEnergy::operator==(const SEScalarEnergy& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarEnergy::operator!=(const SEScalarEnergy& obj) const
{
  return !(*this == obj);
}
}