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

#include <biogears/cdm/properties/SEScalarAmountPerMass.h>
#include <biogears/cdm/stdafx.h>

AmountPerMassUnit AmountPerMassUnit::ct_Per_g("ct/g");
AmountPerMassUnit AmountPerMassUnit::ct_Per_ug("ct/ug");

CDM::ScalarAmountPerMassData* SEScalarAmountPerMass::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarAmountPerMassData* data(new CDM::ScalarAmountPerMassData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool AmountPerMassUnit::IsValidUnit(const std::string& unit)
{
  if (ct_Per_g.GetString().compare(unit) == 0)
    return true;
  if (ct_Per_ug.GetString().compare(unit) == 0)
    return true;
  return false;
}
const AmountPerMassUnit& AmountPerMassUnit::GetCompoundUnit(const std::string& unit)
{
  if (ct_Per_g.GetString().compare(unit) == 0)
    return ct_Per_g;
  if (ct_Per_ug.GetString().compare(unit) == 0)
    return ct_Per_ug;
  std::stringstream err;
  err << unit << " is not a valid AmountPerMass unit";
  throw CommonDataModelException(err.str());
}