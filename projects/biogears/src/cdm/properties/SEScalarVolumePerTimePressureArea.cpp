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
#include <biogears/cdm/properties/SEScalarVolumePerTimePressureArea.h>

const VolumePerTimePressureAreaUnit VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2("mL/min mmHg m^2");
const VolumePerTimePressureAreaUnit VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2("mL/s mmHg m^2");

CDM::ScalarVolumePerTimePressureAreaData* SEScalarVolumePerTimePressureArea::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarVolumePerTimePressureAreaData* data(new CDM::ScalarVolumePerTimePressureAreaData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool VolumePerTimePressureAreaUnit::IsValidUnit(const std::string& unit)
{
  if (mL_Per_min_mmHg_m2.GetString().compare(unit) == 0)
    return true;
  if (mL_Per_s_mmHg_m2.GetString().compare(unit) == 0)
    return true; 
  return false;
}

const VolumePerTimePressureAreaUnit& VolumePerTimePressureAreaUnit::GetCompoundUnit(const std::string& unit)
{
  if (mL_Per_min_mmHg_m2.GetString().compare(unit) == 0)
    return mL_Per_min_mmHg_m2;
  if (mL_Per_s_mmHg_m2.GetString().compare(unit) == 0)
    return mL_Per_s_mmHg_m2;  
  std::stringstream err;
  err << unit << " is not a valid VolumePerTimePressureArea unit";
  throw CommonDataModelException(err.str());
}