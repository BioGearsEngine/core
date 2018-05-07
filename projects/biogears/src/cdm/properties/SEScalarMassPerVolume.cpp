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
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>

const MassPerVolumeUnit MassPerVolumeUnit::g_Per_dL("g/dL");
const MassPerVolumeUnit MassPerVolumeUnit::g_Per_cm3("g/cm^3");
const MassPerVolumeUnit MassPerVolumeUnit::g_Per_m3("g/m^3");
const MassPerVolumeUnit MassPerVolumeUnit::ug_Per_mL("ug/mL");
const MassPerVolumeUnit MassPerVolumeUnit::mg_Per_m3("mg/m^3");
const MassPerVolumeUnit MassPerVolumeUnit::kg_Per_m3("kg/m^3");
const MassPerVolumeUnit MassPerVolumeUnit::ug_Per_L("ug/L");
const MassPerVolumeUnit MassPerVolumeUnit::g_Per_L("g/L");
const MassPerVolumeUnit MassPerVolumeUnit::g_Per_mL("g/mL");
const MassPerVolumeUnit MassPerVolumeUnit::mg_Per_mL("mg/mL");
const MassPerVolumeUnit MassPerVolumeUnit::mg_Per_L("mg/L");
const MassPerVolumeUnit MassPerVolumeUnit::mg_Per_dL("mg/dL");
const MassPerVolumeUnit MassPerVolumeUnit::kg_Per_mL("kg/mL");
const MassPerVolumeUnit MassPerVolumeUnit::kg_Per_L("kg/L");

CDM::ScalarMassPerVolumeData* SEScalarMassPerVolume::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarMassPerVolumeData* data(new CDM::ScalarMassPerVolumeData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool MassPerVolumeUnit::IsValidUnit(const std::string& unit)
{
  if (g_Per_dL.GetString().compare(unit) == 0)
    return true;
  if (g_Per_cm3.GetString().compare(unit) == 0)
	  return true;
  if (g_Per_m3.GetString().compare(unit) == 0)
    return true;
  if (ug_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (mg_Per_m3.GetString().compare(unit) == 0)
    return true;
  if (kg_Per_m3.GetString().compare(unit) == 0)
    return true;
  if (ug_Per_L.GetString().compare(unit) == 0)
    return true;
  if (g_Per_L.GetString().compare(unit) == 0)
    return true;
  if (g_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (mg_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (mg_Per_L.GetString().compare(unit) == 0)
    return true;
  if (mg_Per_dL.GetString().compare(unit) == 0)
    return true;
  if (kg_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (kg_Per_L.GetString().compare(unit) == 0)
	  return true;
  return false;
}

const MassPerVolumeUnit& MassPerVolumeUnit::GetCompoundUnit(const std::string& unit)
{
  if (g_Per_dL.GetString().compare(unit) == 0)
    return g_Per_dL;
  if (g_Per_cm3.GetString().compare(unit) == 0)
	  return g_Per_cm3;
  if (g_Per_m3.GetString().compare(unit) == 0)
    return g_Per_m3;
  if (ug_Per_mL.GetString().compare(unit) == 0)
    return ug_Per_mL;
  if (mg_Per_m3.GetString().compare(unit) == 0)
    return mg_Per_m3;
  if (kg_Per_m3.GetString().compare(unit) == 0)
    return kg_Per_m3;
  if (ug_Per_L.GetString().compare(unit) == 0)
    return ug_Per_L;
  if (g_Per_L.GetString().compare(unit) == 0)
    return g_Per_L;
  if (g_Per_mL.GetString().compare(unit) == 0)
    return g_Per_mL;
  if (mg_Per_mL.GetString().compare(unit) == 0)
    return mg_Per_mL;
  if (mg_Per_L.GetString().compare(unit) == 0)
    return mg_Per_L;
  if (mg_Per_dL.GetString().compare(unit) == 0)
    return mg_Per_dL;
  if (kg_Per_mL.GetString().compare(unit) == 0)
    return kg_Per_mL;
  if (kg_Per_L.GetString().compare(unit) == 0)
	  return kg_Per_L;
  std::stringstream err;
  err << unit << " is not a valid MassPerVolume unit";
  throw CommonDataModelException(err.str());
}