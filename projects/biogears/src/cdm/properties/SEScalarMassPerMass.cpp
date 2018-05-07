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

#include <biogears/cdm/stdafx.h>
#include <biogears/cdm/properties/SEScalarMassPerMass.h>

const MassPerMassUnit MassPerMassUnit::ug_Per_kg("ug/kg");
const MassPerMassUnit MassPerMassUnit::mg_Per_g("mg/g");

CDM::ScalarMassPerMassData* SEScalarMassPerMass::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarMassPerMassData* data(new CDM::ScalarMassPerMassData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool MassPerMassUnit::IsValidUnit(const std::string& unit)
{
  if (ug_Per_kg.GetString().compare(unit) == 0)
    return true;
  if (mg_Per_g.GetString().compare(unit) == 0)
    return true;
  return false;
}

const MassPerMassUnit& MassPerMassUnit::GetCompoundUnit(const std::string& unit)
{
  if (ug_Per_kg.GetString().compare(unit) == 0)
    return ug_Per_kg;
  if (mg_Per_g.GetString().compare(unit) == 0)
    return mg_Per_g;
  std::stringstream err;
  err << unit << " is not a valid MassPerMass unit";
  throw CommonDataModelException(err.str());
}