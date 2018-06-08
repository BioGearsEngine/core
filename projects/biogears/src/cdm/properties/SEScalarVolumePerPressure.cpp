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

#include <biogears/cdm/properties/SEScalarVolumePerPressure.h>
#include <biogears/cdm/stdafx.h>

const VolumePerPressureUnit VolumePerPressureUnit::L_Per_Pa("L/Pa");
const VolumePerPressureUnit VolumePerPressureUnit::L_Per_cmH2O("L/cmH2O");

CDM::ScalarVolumePerPressureData* SEScalarVolumePerPressure::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarVolumePerPressureData* data(new CDM::ScalarVolumePerPressureData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool VolumePerPressureUnit::IsValidUnit(const std::string& unit)
{
  if (L_Per_Pa.GetString().compare(unit) == 0)
    return true;
  if (L_Per_cmH2O.GetString().compare(unit) == 0)
    return true;
  return false;
}

const VolumePerPressureUnit& VolumePerPressureUnit::GetCompoundUnit(const std::string& unit)
{
  if (L_Per_Pa.GetString().compare(unit) == 0)
    return L_Per_Pa;
  if (L_Per_cmH2O.GetString().compare(unit) == 0)
    return L_Per_cmH2O;
  std::stringstream err;
  err << unit << " is not a valid VolumePerPressure unit";
  throw CommonDataModelException(err.str());
}