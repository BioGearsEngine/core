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

#include <biogears/cdm/properties/SEScalarElectricCharge.h>

namespace biogears {
const ElectricChargeUnit ElectricChargeUnit::C("C");

ElectricChargeUnit::ElectricChargeUnit(const char* u)
  : ElectricChargeUnit(std::string(u))
{
}
//-----------------------------------------------------------------------------
ElectricChargeUnit::ElectricChargeUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-----------------------------------------------------------------------------
CDM::ScalarElectricChargeData* SEScalarElectricCharge::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarElectricChargeData* data(new CDM::ScalarElectricChargeData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
bool ElectricChargeUnit::IsValidUnit(const char* unit)
{
  if (strcmp(C.GetString(),unit) == 0)
    return true;
  return false;
}
//-----------------------------------------------------------------------------
bool ElectricChargeUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
const ElectricChargeUnit& ElectricChargeUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(C.GetString(),unit) == 0)
    return C;
  std::stringstream err;
  err << unit << " is not a valid ElectricCharge unit";
  throw CommonDataModelException(err.str());
}
//-----------------------------------------------------------------------------
const ElectricChargeUnit& ElectricChargeUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
}