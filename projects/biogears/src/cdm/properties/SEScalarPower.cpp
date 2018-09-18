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

#include <biogears/cdm/properties/SEScalarPower.h>

const PowerUnit PowerUnit::W("W");
const PowerUnit PowerUnit::kcal_Per_s("kcal/s");
const PowerUnit PowerUnit::kcal_Per_hr("kcal/hr");
const PowerUnit PowerUnit::kcal_Per_day("kcal/day");
const PowerUnit PowerUnit::J_Per_s("J/s");
const PowerUnit PowerUnit::BTU_Per_hr("BTU/hr");

CDM::ScalarPowerData* SEScalarPower::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarPowerData* data(new CDM::ScalarPowerData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool PowerUnit::IsValidUnit(const std::string& unit)
{
  if (W.GetString().compare(unit) == 0)
    return true;
  if (kcal_Per_s.GetString().compare(unit) == 0)
    return true;
  if (kcal_Per_hr.GetString().compare(unit) == 0)
    return true;
  if (kcal_Per_day.GetString().compare(unit) == 0)
    return true;
  if (J_Per_s.GetString().compare(unit) == 0)
    return true;
  if (BTU_Per_hr.GetString().compare(unit) == 0)
    return true;
  return false;
}

const PowerUnit& PowerUnit::GetCompoundUnit(const std::string& unit)
{
  if (W.GetString().compare(unit) == 0)
    return W;
  if (kcal_Per_s.GetString().compare(unit) == 0)
    return kcal_Per_s;
  if (kcal_Per_hr.GetString().compare(unit) == 0)
    return kcal_Per_hr;
  if (kcal_Per_day.GetString().compare(unit) == 0)
    return kcal_Per_day;
  if (J_Per_s.GetString().compare(unit) == 0)
    return J_Per_s;
  if (BTU_Per_hr.GetString().compare(unit) == 0)
    return BTU_Per_hr;
  std::stringstream err;
  err << unit << " is not a valid Power unit";
  throw CommonDataModelException(err.str());
}