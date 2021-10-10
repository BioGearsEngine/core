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

#include <biogears/cdm/properties/SEScalarAmountPerTime.h>

namespace biogears {
AmountPerTimeUnit AmountPerTimeUnit::mol_Per_day("mol/day");
AmountPerTimeUnit AmountPerTimeUnit::mol_Per_s("mol/s");
AmountPerTimeUnit AmountPerTimeUnit::umol_Per_s("umol/s");
AmountPerTimeUnit AmountPerTimeUnit::mmol_Per_min("mmol/min");
AmountPerTimeUnit AmountPerTimeUnit::pmol_Per_min("pmol/min");
AmountPerTimeUnit AmountPerTimeUnit::umol_Per_min("umol/min");

template class SEScalarQuantity<AmountPerTimeUnit>;
;

AmountPerTimeUnit::AmountPerTimeUnit(const char* u)
  : AmountPerTimeUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
AmountPerTimeUnit::AmountPerTimeUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
AmountPerTimeUnit::~AmountPerTimeUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarAmountPerTime::SEScalarAmountPerTime()
{
}
//-------------------------------------------------------------------------------
SEScalarAmountPerTime::~SEScalarAmountPerTime()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarAmountPerTimeData* SEScalarAmountPerTime::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarAmountPerTimeData* data(new CDM::ScalarAmountPerTimeData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool AmountPerTimeUnit::IsValidUnit(const char* unit)
{
  if (strcmp(mol_Per_day.GetString(), unit) == 0)
    return true;
  if (strcmp(mol_Per_s.GetString(), unit) == 0)
    return true;
  if (strcmp(umol_Per_s.GetString(), unit) == 0)
    return true;
  if (strcmp(mmol_Per_min.GetString(), unit) == 0)
    return true;
  if (strcmp(pmol_Per_min.GetString(), unit) == 0)
    return true;
  if (strcmp(umol_Per_min.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool AmountPerTimeUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const AmountPerTimeUnit& AmountPerTimeUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(mol_Per_day.GetString(), unit) == 0)
    return mol_Per_day;
  if (strcmp(mol_Per_s.GetString(), unit) == 0)
    return mol_Per_s;
  if (strcmp(umol_Per_s.GetString(), unit) == 0)
    return umol_Per_s;
  if (strcmp(mmol_Per_min.GetString(), unit) == 0)
    return mmol_Per_min;
  if (strcmp(pmol_Per_min.GetString(), unit) == 0)
    return pmol_Per_min;
  if (strcmp(umol_Per_min.GetString(), unit) == 0)
    return umol_Per_min;
  std::stringstream err;
  err << unit << " is not a valid AmountPerTime unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const AmountPerTimeUnit& AmountPerTimeUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool AmountPerTimeUnit::operator==(const AmountPerTimeUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool AmountPerTimeUnit::operator!=(const AmountPerTimeUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarAmountPerTime::operator==(const SEScalarAmountPerTime& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarAmountPerTime::operator!=(const SEScalarAmountPerTime& obj) const
{
  return !(*this == obj);
}
}