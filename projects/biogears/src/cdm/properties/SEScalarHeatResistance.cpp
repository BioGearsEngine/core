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

#include <biogears/cdm/properties/SEScalarHeatResistance.h>

const HeatResistanceUnit HeatResistanceUnit::K_Per_W("K/W");
const HeatResistanceUnit HeatResistanceUnit::C_Per_W("degC/W");
const HeatResistanceUnit HeatResistanceUnit::K_s_Per_kcal("K s/kcal");
const HeatResistanceUnit HeatResistanceUnit::C_s_Per_kcal("degC s/kcal");

CDM::ScalarHeatResistanceData* SEScalarHeatResistance::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarHeatResistanceData* data(new CDM::ScalarHeatResistanceData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool HeatResistanceUnit::IsValidUnit(const std::string& unit)
{
  if (K_Per_W.GetString().compare(unit) == 0)
    return true;
  if (C_Per_W.GetString().compare(unit) == 0)
    return true;
  if (K_s_Per_kcal.GetString().compare(unit) == 0)
    return true;
  if (C_s_Per_kcal.GetString().compare(unit) == 0)
    return true;
  return false;
}

const HeatResistanceUnit& HeatResistanceUnit::GetCompoundUnit(const std::string& unit)
{
  if (K_Per_W.GetString().compare(unit) == 0)
    return K_Per_W;
  if (C_Per_W.GetString().compare(unit) == 0)
    return C_Per_W;
  if (K_s_Per_kcal.GetString().compare(unit) == 0)
    return K_s_Per_kcal;
  if (C_s_Per_kcal.GetString().compare(unit) == 0)
    return C_s_Per_kcal;
  std::stringstream err;
  err << unit << " is not a valid HeatResistance unit";
  throw CommonDataModelException(err.str());
}