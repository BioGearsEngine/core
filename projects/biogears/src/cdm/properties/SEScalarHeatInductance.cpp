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

#include <biogears/cdm/properties/SEScalarHeatInductance.h>

const HeatInductanceUnit HeatInductanceUnit::K_s_Per_W("K s/W");

CDM::ScalarHeatInductanceData* SEScalarHeatInductance::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarHeatInductanceData* data(new CDM::ScalarHeatInductanceData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool HeatInductanceUnit::IsValidUnit(const std::string& unit)
{
  if (K_s_Per_W.GetString().compare(unit) == 0)
    return true;
  return false;
}

const HeatInductanceUnit& HeatInductanceUnit::GetCompoundUnit(const std::string& unit)
{
  if (K_s_Per_W.GetString().compare(unit) == 0)
    return K_s_Per_W;
  std::stringstream err;
  err << unit << " is not a valid HeatInductance unit";
  throw CommonDataModelException(err.str());
}