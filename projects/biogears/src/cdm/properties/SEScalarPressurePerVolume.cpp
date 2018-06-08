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

#include <biogears/cdm/properties/SEScalarPressurePerVolume.h>
#include <biogears/cdm/stdafx.h>

const PressurePerVolumeUnit PressurePerVolumeUnit::mmHg_Per_mL("mmHg/mL");
const PressurePerVolumeUnit PressurePerVolumeUnit::cmH2O_Per_mL("cmH2O/mL");

CDM::ScalarPressurePerVolumeData* SEScalarPressurePerVolume::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarPressurePerVolumeData* data(new CDM::ScalarPressurePerVolumeData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool PressurePerVolumeUnit::IsValidUnit(const std::string& unit)
{
  if (mmHg_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (cmH2O_Per_mL.GetString().compare(unit) == 0)
    return true;
  return false;
}

const PressurePerVolumeUnit& PressurePerVolumeUnit::GetCompoundUnit(const std::string& unit)
{
  if (mmHg_Per_mL.GetString().compare(unit) == 0)
    return mmHg_Per_mL;
  if (cmH2O_Per_mL.GetString().compare(unit) == 0)
    return cmH2O_Per_mL;
  std::stringstream err;
  err << unit << " is not a valid PressurePerVolume unit";
  throw CommonDataModelException(err.str());
}