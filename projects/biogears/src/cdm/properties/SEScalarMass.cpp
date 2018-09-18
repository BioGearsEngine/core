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

#include <biogears/cdm/properties/SEScalarMass.h>

const MassUnit MassUnit::g("g");
const MassUnit MassUnit::ug("ug");
const MassUnit MassUnit::mg("mg");
const MassUnit MassUnit::kg("kg");
const MassUnit MassUnit::lb("lb");

CDM::ScalarMassData* SEScalarMass::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarMassData* data(new CDM::ScalarMassData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool MassUnit::IsValidUnit(const std::string& unit)
{
  if (g.GetString().compare(unit) == 0)
    return true;
  if (ug.GetString().compare(unit) == 0)
    return true;
  if (mg.GetString().compare(unit) == 0)
    return true;
  if (kg.GetString().compare(unit) == 0)
    return true;
  if (lb.GetString().compare(unit) == 0)
    return true;
  return false;
}

const MassUnit& MassUnit::GetCompoundUnit(const std::string& unit)
{
  if (g.GetString().compare(unit) == 0)
    return g;
  if (ug.GetString().compare(unit) == 0)
    return ug;
  if (mg.GetString().compare(unit) == 0)
    return mg;
  if (kg.GetString().compare(unit) == 0)
    return kg;
  if (lb.GetString().compare(unit) == 0)
    return lb;
  std::stringstream err;
  err << unit << " is not a valid Mass unit";
  throw CommonDataModelException(err.str());
}