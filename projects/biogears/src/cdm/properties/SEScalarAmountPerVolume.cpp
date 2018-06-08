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

#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/stdafx.h>

AmountPerVolumeUnit AmountPerVolumeUnit::mol_Per_L("mol/L");
AmountPerVolumeUnit AmountPerVolumeUnit::mol_Per_mL("mol/mL");
AmountPerVolumeUnit AmountPerVolumeUnit::mmol_Per_L("mmol/L");
AmountPerVolumeUnit AmountPerVolumeUnit::mmol_Per_mL("mmol/mL");
AmountPerVolumeUnit AmountPerVolumeUnit::ct_Per_L("ct/L");
AmountPerVolumeUnit AmountPerVolumeUnit::ct_Per_uL("ct/uL");

CDM::ScalarAmountPerVolumeData* SEScalarAmountPerVolume::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarAmountPerVolumeData* data(new CDM::ScalarAmountPerVolumeData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool AmountPerVolumeUnit::IsValidUnit(const std::string& unit)
{
  if (mol_Per_L.GetString().compare(unit) == 0)
    return true;
  if (mol_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (mmol_Per_L.GetString().compare(unit) == 0)
    return true;
  if (mmol_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (ct_Per_L.GetString().compare(unit) == 0)
    return true;
  if (ct_Per_uL.GetString().compare(unit) == 0)
    return true;
  return false;
}

const AmountPerVolumeUnit& AmountPerVolumeUnit::GetCompoundUnit(const std::string& unit)
{
  if (mol_Per_L.GetString().compare(unit) == 0)
    return mol_Per_L;
  if (mol_Per_mL.GetString().compare(unit) == 0)
    return mol_Per_mL;
  if (mmol_Per_L.GetString().compare(unit) == 0)
    return mmol_Per_L;
  if (mmol_Per_mL.GetString().compare(unit) == 0)
    return mmol_Per_mL;
  if (ct_Per_L.GetString().compare(unit) == 0)
    return ct_Per_L;
  if (ct_Per_uL.GetString().compare(unit) == 0)
    return ct_Per_uL;
  std::stringstream err;
  err << unit << " is not a valid AmountPerVolume unit";
  throw CommonDataModelException(err.str());
}