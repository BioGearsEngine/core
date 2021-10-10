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

#include <biogears/cdm/properties/SEScalarMassPerAmount.h>

namespace biogears {
const MassPerAmountUnit MassPerAmountUnit::g_Per_ct("g/ct");
const MassPerAmountUnit MassPerAmountUnit::g_Per_mol("g/mol");
const MassPerAmountUnit MassPerAmountUnit::g_Per_mmol("g/mmol");
const MassPerAmountUnit MassPerAmountUnit::g_Per_umol("g/umol");
const MassPerAmountUnit MassPerAmountUnit::mg_Per_mol("mg/mol");
const MassPerAmountUnit MassPerAmountUnit::mg_Per_mmol("mg/mmol");
const MassPerAmountUnit MassPerAmountUnit::kg_Per_mol("kg/mol");
const MassPerAmountUnit MassPerAmountUnit::ug_Per_mmol("ug/mmol");
const MassPerAmountUnit MassPerAmountUnit::ug_Per_mol("ug/mol");
const MassPerAmountUnit MassPerAmountUnit::ug_Per_ct("ug/ct");
const MassPerAmountUnit MassPerAmountUnit::pg_Per_ct("pg/ct");

template class SEScalarQuantity<MassPerAmountUnit>;

MassPerAmountUnit::MassPerAmountUnit(const char* u)
  : MassPerAmountUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
MassPerAmountUnit::MassPerAmountUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
MassPerAmountUnit::~MassPerAmountUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarMassPerAmount::SEScalarMassPerAmount()
{
}
//-------------------------------------------------------------------------------
SEScalarMassPerAmount::~SEScalarMassPerAmount()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarMassPerAmountData* SEScalarMassPerAmount::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarMassPerAmountData* data(new CDM::ScalarMassPerAmountData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool MassPerAmountUnit::IsValidUnit(const char* unit)
{
  if (strcmp(g_Per_ct.GetString(), unit) == 0)
    return true;
  if (strcmp(g_Per_mol.GetString(), unit) == 0)
    return true;
  if (strcmp(g_Per_mmol.GetString(), unit) == 0)
    return true;
  if (strcmp(g_Per_umol.GetString(), unit) == 0)
    return true;
  if (strcmp(mg_Per_mol.GetString(), unit) == 0)
    return true;
  if (strcmp(mg_Per_mmol.GetString(), unit) == 0)
    return true;
  if (strcmp(kg_Per_mol.GetString(), unit) == 0)
    return true;
  if (strcmp(ug_Per_mmol.GetString(), unit) == 0)
    return true;
  if (strcmp(ug_Per_mol.GetString(), unit) == 0)
    return true;
  if (strcmp(ug_Per_ct.GetString(), unit) == 0)
    return true;
  if (strcmp(pg_Per_ct.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool MassPerAmountUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const MassPerAmountUnit& MassPerAmountUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(g_Per_ct.GetString(), unit) == 0)
    return g_Per_ct;
  if (strcmp(g_Per_mol.GetString(), unit) == 0)
    return g_Per_mol;
  if (strcmp(g_Per_mmol.GetString(), unit) == 0)
    return g_Per_mmol;
  if (strcmp(g_Per_umol.GetString(), unit) == 0)
    return g_Per_umol;
  if (strcmp(mg_Per_mol.GetString(), unit) == 0)
    return mg_Per_mol;
  if (strcmp(mg_Per_mmol.GetString(), unit) == 0)
    return mg_Per_mmol;
  if (strcmp(kg_Per_mol.GetString(), unit) == 0)
    return kg_Per_mol;
  if (strcmp(ug_Per_mmol.GetString(), unit) == 0)
    return ug_Per_mmol;
  if (strcmp(ug_Per_mol.GetString(), unit) == 0)
    return ug_Per_mol;
  if (strcmp(ug_Per_ct.GetString(), unit) == 0)
    return ug_Per_ct;
  if (strcmp(pg_Per_ct.GetString(), unit) == 0)
    return pg_Per_ct;
  std::stringstream err;
  err << unit << " is not a valid MassPerAmount unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const MassPerAmountUnit& MassPerAmountUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool MassPerAmountUnit::operator==(const MassPerAmountUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool MassPerAmountUnit::operator!=(const MassPerAmountUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------
bool SEScalarMassPerAmount::operator==(const SEScalarMassPerAmount& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarMassPerAmount::operator!=(const SEScalarMassPerAmount& obj) const
{
  return !(*this == obj);
}
}