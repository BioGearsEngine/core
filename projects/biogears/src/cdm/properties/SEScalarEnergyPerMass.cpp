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

const EnergyPerMassUnit EnergyPerMassUnit::J_Per_kg("J/kg");
const EnergyPerMassUnit EnergyPerMassUnit::kJ_Per_kg("kJ/kg");
const EnergyPerMassUnit EnergyPerMassUnit::mJ_Per_kg("mJ/kg");
const EnergyPerMassUnit EnergyPerMassUnit::kcal_Per_kg("kcal/kg");

CDM::ScalarEnergyPerMassData* SEScalarEnergyPerMass::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarEnergyPerMassData* data(new CDM::ScalarEnergyPerMassData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool EnergyPerMassUnit::IsValidUnit(const std::string& unit)
{
  if (J_Per_kg.GetString().compare(unit) == 0)
    return true;
  if (kJ_Per_kg.GetString().compare(unit) == 0)
    return true;
  if (mJ_Per_kg.GetString().compare(unit) == 0)
    return true;
  if (kcal_Per_kg.GetString().compare(unit) == 0)
    return true;
  return false;
}

const EnergyPerMassUnit& EnergyPerMassUnit::GetCompoundUnit(const std::string& unit)
{
  if (J_Per_kg.GetString().compare(unit) == 0)
    return J_Per_kg;
  if (kJ_Per_kg.GetString().compare(unit) == 0)
    return kJ_Per_kg;
  if (mJ_Per_kg.GetString().compare(unit) == 0)
    return mJ_Per_kg;
  if (kcal_Per_kg.GetString().compare(unit) == 0)
    return kcal_Per_kg;
  std::stringstream err;
  err << unit << " is not a valid EnergyPerMass unit";
  throw CommonDataModelException(err.str());
}