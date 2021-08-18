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

namespace biogears {
AmountPerVolumeUnit AmountPerVolumeUnit::mol_Per_L("mol/L");
AmountPerVolumeUnit AmountPerVolumeUnit::mol_Per_mL("mol/mL");
AmountPerVolumeUnit AmountPerVolumeUnit::mmol_Per_L("mmol/L");
AmountPerVolumeUnit AmountPerVolumeUnit::mmol_Per_mL("mmol/mL");
AmountPerVolumeUnit AmountPerVolumeUnit::ct_Per_L("ct/L");
AmountPerVolumeUnit AmountPerVolumeUnit::ct_Per_uL("ct/uL");

AmountPerVolumeUnit::AmountPerVolumeUnit(const char* u)
  : AmountPerVolumeUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
AmountPerVolumeUnit::AmountPerVolumeUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
AmountPerVolumeUnit::~AmountPerVolumeUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarAmountPerVolume::SEScalarAmountPerVolume()
{
}
//-------------------------------------------------------------------------------
SEScalarAmountPerVolume::~SEScalarAmountPerVolume()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarAmountPerVolumeData* SEScalarAmountPerVolume::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarAmountPerVolumeData* data(new CDM::ScalarAmountPerVolumeData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool AmountPerVolumeUnit::IsValidUnit(const char* unit)
{
  if (strcmp(mol_Per_L.GetString(), unit) == 0)
    return true;
  if (strcmp(mol_Per_mL.GetString(), unit) == 0)
    return true;
  if (strcmp(mmol_Per_L.GetString(), unit) == 0)
    return true;
  if (strcmp(mmol_Per_mL.GetString(), unit) == 0)
    return true;
  if (strcmp(ct_Per_L.GetString(), unit) == 0)
    return true;
  if (strcmp(ct_Per_uL.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool AmountPerVolumeUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const AmountPerVolumeUnit& AmountPerVolumeUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(mol_Per_L.GetString(), unit) == 0)
    return mol_Per_L;
  if (strcmp(mol_Per_mL.GetString(), unit) == 0)
    return mol_Per_mL;
  if (strcmp(mmol_Per_L.GetString(), unit) == 0)
    return mmol_Per_L;
  if (strcmp(mmol_Per_mL.GetString(), unit) == 0)
    return mmol_Per_mL;
  if (strcmp(ct_Per_L.GetString(), unit) == 0)
    return ct_Per_L;
  if (strcmp(ct_Per_uL.GetString(), unit) == 0)
    return ct_Per_uL;
  std::stringstream err;
  err << unit << " is not a valid AmountPerVolume unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const AmountPerVolumeUnit& AmountPerVolumeUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool AmountPerVolumeUnit::operator==(const AmountPerVolumeUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool AmountPerVolumeUnit::operator!=(const AmountPerVolumeUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarAmountPerVolume::operator==(const SEScalarAmountPerVolume& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarAmountPerVolume::operator!=(const SEScalarAmountPerVolume& obj) const
{
  return !(*this == obj);
}
}