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

#include <biogears/cdm/properties/SEScalarLength.h>

namespace biogears {
const LengthUnit LengthUnit::m("m");
const LengthUnit LengthUnit::cm("cm");
const LengthUnit LengthUnit::mm("mm");
const LengthUnit LengthUnit::um("um");
const LengthUnit LengthUnit::inch("in");
const LengthUnit LengthUnit::ft("ft");

template class SEScalarQuantity<LengthUnit>;

LengthUnit::LengthUnit(const char* u)
  : LengthUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
LengthUnit::LengthUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
LengthUnit::~LengthUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarLength::SEScalarLength()
{
}
//-------------------------------------------------------------------------------
SEScalarLength::~SEScalarLength()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarLengthData* SEScalarLength::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarLengthData* data(new CDM::ScalarLengthData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool LengthUnit::IsValidUnit(const char* unit)
{
  if (strcmp(m.GetString(), unit) == 0)
    return true;
  if (strcmp(cm.GetString(), unit) == 0)
    return true;
  if (strcmp(mm.GetString(), unit) == 0)
    return true;
  if (strcmp(um.GetString(), unit) == 0)
    return true;
  if (strcmp(inch.GetString(), unit) == 0)
    return true;
  if (strcmp(ft.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool LengthUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const LengthUnit& LengthUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(m.GetString(), unit) == 0)
    return m;
  if (strcmp(cm.GetString(), unit) == 0)
    return cm;
  if (strcmp(mm.GetString(), unit) == 0)
    return mm;
  if (strcmp(um.GetString(), unit) == 0)
    return um;
  if (strcmp(inch.GetString(), unit) == 0)
    return inch;
  if (strcmp(ft.GetString(), unit) == 0)
    return ft;
  std::stringstream err;
  err << unit << " is not a valid Length unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const LengthUnit& LengthUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool LengthUnit::operator==(const LengthUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool LengthUnit::operator!=(const LengthUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------
bool SEScalarLength::operator==(const SEScalarLength& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarLength::operator!=(const SEScalarLength& obj) const
{
  return !(*this == obj);
}
}