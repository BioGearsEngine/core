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

#include <biogears/cdm/properties/SEScalarHeatConductance.h>
#include <biogears/cdm/stdafx.h>

const HeatConductanceUnit HeatConductanceUnit::W_Per_K("W/K");
const HeatConductanceUnit HeatConductanceUnit::W_Per_C("W/degC");
const HeatConductanceUnit HeatConductanceUnit::kcal_Per_K_s("kcal/K s");
const HeatConductanceUnit HeatConductanceUnit::kcal_Per_C_s("kcal/degC s");

CDM::ScalarHeatConductanceData* SEScalarHeatConductance::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarHeatConductanceData* data(new CDM::ScalarHeatConductanceData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool HeatConductanceUnit::IsValidUnit(const std::string& unit)
{
  if (W_Per_K.GetString().compare(unit) == 0)
    return true;
  if (W_Per_C.GetString().compare(unit) == 0)
    return true;
  if (kcal_Per_K_s.GetString().compare(unit) == 0)
    return true;
  if (kcal_Per_C_s.GetString().compare(unit) == 0)
    return true;
  return false;
}

const HeatConductanceUnit& HeatConductanceUnit::GetCompoundUnit(const std::string& unit)
{
  if (W_Per_K.GetString().compare(unit) == 0)
    return W_Per_K;
  if (W_Per_C.GetString().compare(unit) == 0)
    return W_Per_C;
  if (kcal_Per_K_s.GetString().compare(unit) == 0)
    return kcal_Per_K_s;
  if (kcal_Per_C_s.GetString().compare(unit) == 0)
    return kcal_Per_C_s;
  std::stringstream err;
  err << unit << " is not a valid HeatConductance unit";
  throw CommonDataModelException(err.str());
}