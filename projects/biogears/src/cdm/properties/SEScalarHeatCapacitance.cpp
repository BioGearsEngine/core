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

#include <biogears/cdm/properties/SEScalarHeatCapacitance.h>

const HeatCapacitanceUnit HeatCapacitanceUnit::J_Per_K("J/K");
const HeatCapacitanceUnit HeatCapacitanceUnit::kJ_Per_K("kJ/K");
const HeatCapacitanceUnit HeatCapacitanceUnit::kcal_Per_K("kcal/K");
const HeatCapacitanceUnit HeatCapacitanceUnit::kcal_Per_C("kcal/degC");

CDM::ScalarHeatCapacitanceData* SEScalarHeatCapacitance::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarHeatCapacitanceData* data(new CDM::ScalarHeatCapacitanceData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool HeatCapacitanceUnit::IsValidUnit(const std::string& unit)
{
  if (J_Per_K.GetString().compare(unit) == 0)
    return true;
  if (kJ_Per_K.GetString().compare(unit) == 0)
    return true;
  if (kcal_Per_K.GetString().compare(unit) == 0)
    return true;
  if (kcal_Per_C.GetString().compare(unit) == 0)
    return true;
  return false;
}

const HeatCapacitanceUnit& HeatCapacitanceUnit::GetCompoundUnit(const std::string& unit)
{
  if (J_Per_K.GetString().compare(unit) == 0)
    return J_Per_K;
  if (kJ_Per_K.GetString().compare(unit) == 0)
    return kJ_Per_K;
  if (kcal_Per_K.GetString().compare(unit) == 0)
    return kcal_Per_K;
  if (kcal_Per_C.GetString().compare(unit) == 0)
    return kcal_Per_C;
  std::stringstream err;
  err << unit << " is not a valid HeatCapacitance unit";
  throw CommonDataModelException(err.str());
}