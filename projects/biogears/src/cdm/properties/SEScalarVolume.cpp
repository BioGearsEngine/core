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

#include <biogears/cdm/properties/SEScalarVolume.h>

const VolumeUnit VolumeUnit::L("L");
const VolumeUnit VolumeUnit::dL("dL");
const VolumeUnit VolumeUnit::mL("mL");
const VolumeUnit VolumeUnit::uL("uL");
const VolumeUnit VolumeUnit::m3("m^3");

CDM::ScalarVolumeData* SEScalarVolume::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarVolumeData* data(new CDM::ScalarVolumeData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool VolumeUnit::IsValidUnit(const std::string& unit)
{
  if (L.GetString().compare(unit) == 0)
    return true;
  if (dL.GetString().compare(unit) == 0)
    return true;
  if (mL.GetString().compare(unit) == 0)
    return true;
  if (uL.GetString().compare(unit) == 0)
    return true;
  if (m3.GetString().compare(unit) == 0)
    return true;
  return false;
}

const VolumeUnit& VolumeUnit::GetCompoundUnit(const std::string& unit)
{
  if (L.GetString().compare(unit) == 0)
    return L;
  if (mL.GetString().compare(unit) == 0)
    return mL;
  if (m3.GetString().compare(unit) == 0)
    return m3;
  if (dL.GetString().compare(unit) == 0)
    return dL;
  if (uL.GetString().compare(unit) == 0)
    return uL;
  std::stringstream err;
  err << unit << " is not a valid Volume unit";
  throw CommonDataModelException(err.str());
}
