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

#include <biogears/cdm/properties/SEScalarMassPerVolume.h>

namespace biogears {

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

template class SEScalarQuantity<MassPerVolumeUnit>;

MassPerVolumeUnit::MassPerVolumeUnit(const char* u)
  : MassPerVolumeUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
MassPerVolumeUnit::MassPerVolumeUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
MassPerVolumeUnit::~MassPerVolumeUnit()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarMassPerVolumeData* SEScalarMassPerVolume::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarMassPerVolumeData* data(new CDM::ScalarMassPerVolumeData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool MassPerVolumeUnit::IsValidUnit(const char* unit)
{
  if (strcmp(g_Per_dL.GetString(), unit) == 0)
    return true;
  if (strcmp(g_Per_cm3.GetString(), unit) == 0)
    return true;
  if (strcmp(g_Per_m3.GetString(), unit) == 0)
    return true;
  if (strcmp(ug_Per_mL.GetString(), unit) == 0)
    return true;
  if (strcmp(mg_Per_m3.GetString(), unit) == 0)
    return true;
  if (strcmp(kg_Per_m3.GetString(), unit) == 0)
    return true;
  if (strcmp(ug_Per_L.GetString(), unit) == 0)
    return true;
  if (strcmp(g_Per_L.GetString(), unit) == 0)
    return true;
  if (strcmp(g_Per_mL.GetString(), unit) == 0)
    return true;
  if (strcmp(mg_Per_mL.GetString(), unit) == 0)
    return true;
  if (strcmp(mg_Per_L.GetString(), unit) == 0)
    return true;
  if (strcmp(mg_Per_dL.GetString(), unit) == 0)
    return true;
  if (strcmp(kg_Per_mL.GetString(), unit) == 0)
    return true;
  if (strcmp(kg_Per_L.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool MassPerVolumeUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const MassPerVolumeUnit& MassPerVolumeUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(g_Per_dL.GetString(), unit) == 0)
    return g_Per_dL;
  if (strcmp(g_Per_cm3.GetString(), unit) == 0)
    return g_Per_cm3;
  if (strcmp(g_Per_m3.GetString(), unit) == 0)
    return g_Per_m3;
  if (strcmp(ug_Per_mL.GetString(), unit) == 0)
    return ug_Per_mL;
  if (strcmp(mg_Per_m3.GetString(), unit) == 0)
    return mg_Per_m3;
  if (strcmp(kg_Per_m3.GetString(), unit) == 0)
    return kg_Per_m3;
  if (strcmp(ug_Per_L.GetString(), unit) == 0)
    return ug_Per_L;
  if (strcmp(g_Per_L.GetString(), unit) == 0)
    return g_Per_L;
  if (strcmp(g_Per_mL.GetString(), unit) == 0)
    return g_Per_mL;
  if (strcmp(mg_Per_mL.GetString(), unit) == 0)
    return mg_Per_mL;
  if (strcmp(mg_Per_L.GetString(), unit) == 0)
    return mg_Per_L;
  if (strcmp(mg_Per_dL.GetString(), unit) == 0)
    return mg_Per_dL;
  if (strcmp(kg_Per_mL.GetString(), unit) == 0)
    return kg_Per_mL;
  if (strcmp(kg_Per_L.GetString(), unit) == 0)
    return kg_Per_L;
  std::stringstream err;
  err << unit << " is not a valid MassPerVolume unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const MassPerVolumeUnit& MassPerVolumeUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
SEScalarMassPerVolume::SEScalarMassPerVolume()
{
}
//-------------------------------------------------------------------------------
SEScalarMassPerVolume::SEScalarMassPerVolume(const SEScalarMassPerVolume& obj)
  : SEScalarQuantity<MassPerVolumeUnit>(obj)
{
}
//-------------------------------------------------------------------------------
SEScalarMassPerVolume::SEScalarMassPerVolume(double v, const MassPerVolumeUnit& u)
  : SEScalarQuantity<MassPerVolumeUnit>(v, u)
{
}
//-------------------------------------------------------------------------------
SEScalarMassPerVolume::~SEScalarMassPerVolume()
{
}
//-------------------------------------------------------------------------------
bool MassPerVolumeUnit::operator==(const MassPerVolumeUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool MassPerVolumeUnit::operator!=(const MassPerVolumeUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarMassPerVolume::operator==(const SEScalarMassPerVolume& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarMassPerVolume::operator!=(const SEScalarMassPerVolume& obj) const
{
  return !(*this == obj);
}
}