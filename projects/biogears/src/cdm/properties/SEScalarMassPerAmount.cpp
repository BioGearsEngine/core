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
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>

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

CDM::ScalarMassPerAmountData* SEScalarMassPerAmount::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarMassPerAmountData* data(new CDM::ScalarMassPerAmountData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool MassPerAmountUnit::IsValidUnit(const std::string& unit)
{
  if (g_Per_ct.GetString().compare(unit) == 0)
    return true;
  if (g_Per_mol.GetString().compare(unit) == 0)
    return true;
  if (g_Per_mmol.GetString().compare(unit) == 0)
    return true;
  if (g_Per_umol.GetString().compare(unit) == 0)
    return true;
  if (mg_Per_mol.GetString().compare(unit) == 0)
    return true;
  if (mg_Per_mmol.GetString().compare(unit) == 0)
    return true;
  if (kg_Per_mol.GetString().compare(unit) == 0)
    return true;
  if (ug_Per_mmol.GetString().compare(unit) == 0)
    return true;
  if (ug_Per_mol.GetString().compare(unit) == 0)
    return true;
  if (ug_Per_ct.GetString().compare(unit) == 0)
    return true;
  if (pg_Per_ct.GetString().compare(unit) == 0)
    return true;
  return false;
}

const MassPerAmountUnit& MassPerAmountUnit::GetCompoundUnit(const std::string& unit)
{
  if (g_Per_ct.GetString().compare(unit) == 0)
    return g_Per_ct;
  if (g_Per_mol.GetString().compare(unit) == 0)
    return g_Per_mol;
  if (g_Per_mmol.GetString().compare(unit) == 0)
    return g_Per_mmol;
  if (g_Per_umol.GetString().compare(unit) == 0)
    return g_Per_umol;
  if (mg_Per_mol.GetString().compare(unit) == 0)
    return mg_Per_mol;
  if (mg_Per_mmol.GetString().compare(unit) == 0)
    return mg_Per_mmol;
  if (kg_Per_mol.GetString().compare(unit) == 0)
    return kg_Per_mol;
  if (ug_Per_mmol.GetString().compare(unit) == 0)
    return ug_Per_mmol;
  if (ug_Per_mol.GetString().compare(unit) == 0)
    return ug_Per_mol;
  if (ug_Per_ct.GetString().compare(unit) == 0)
    return ug_Per_ct;
  if (pg_Per_ct.GetString().compare(unit) == 0)
    return pg_Per_ct;
  std::stringstream err;
  err << unit << " is not a valid MassPerAmount unit";
  throw CommonDataModelException(err.str());
}