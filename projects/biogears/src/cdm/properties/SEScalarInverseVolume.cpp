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
#include <biogears/cdm/properties/SEScalarInverseVolume.h>

const InverseVolumeUnit InverseVolumeUnit::Inverse_L("1/L");
const InverseVolumeUnit InverseVolumeUnit::Inverse_mL("1/mL");

CDM::ScalarInverseVolumeData* SEScalarInverseVolume::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarInverseVolumeData* data(new CDM::ScalarInverseVolumeData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool InverseVolumeUnit::IsValidUnit(const std::string& unit)
{
  if (Inverse_L.GetString().compare(unit) == 0)
    return true;
  if (Inverse_mL.GetString().compare(unit) == 0)
    return true;
  return false;
}

const InverseVolumeUnit& InverseVolumeUnit::GetCompoundUnit(const std::string& unit)
{
  if (Inverse_L.GetString().compare(unit) == 0)
    return Inverse_L;
  if (Inverse_mL.GetString().compare(unit) == 0)
    return Inverse_mL;
  std::stringstream err;
  err << unit << " is not a valid Volume unit";
  throw CommonDataModelException(err.str());
}