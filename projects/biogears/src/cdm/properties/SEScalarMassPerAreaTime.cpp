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
#include <biogears/cdm/properties/SEScalarMassPerAreaTime.h>

const MassPerAreaTimeUnit MassPerAreaTimeUnit::g_Per_cm2_s("g/cm^2 s");

CDM::ScalarMassPerAreaTimeData* SEScalarMassPerAreaTime::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarMassPerAreaTimeData* data(new CDM::ScalarMassPerAreaTimeData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool MassPerAreaTimeUnit::IsValidUnit(const std::string& unit)
{
  if (g_Per_cm2_s.GetString().compare(unit) == 0)
    return true;
  return false;
}

const MassPerAreaTimeUnit& MassPerAreaTimeUnit::GetCompoundUnit(const std::string& unit)
{
  if (g_Per_cm2_s.GetString().compare(unit) == 0)
    return g_Per_cm2_s;
  std::stringstream err;
  err << unit << " is not a valid MassPerAreaTime unit";
  throw CommonDataModelException(err.str());
}