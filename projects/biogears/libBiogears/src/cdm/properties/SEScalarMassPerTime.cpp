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

#include <biogears/cdm/properties/SEScalarMassPerTime.h>

namespace biogears {
const MassPerTimeUnit MassPerTimeUnit::g_Per_s("g/s");
const MassPerTimeUnit MassPerTimeUnit::g_Per_min("g/min");
const MassPerTimeUnit MassPerTimeUnit::g_Per_day("g/day");
const MassPerTimeUnit MassPerTimeUnit::mg_Per_s("mg/s");
const MassPerTimeUnit MassPerTimeUnit::mg_Per_min("mg/min");
const MassPerTimeUnit MassPerTimeUnit::ug_Per_s("ug/s");
const MassPerTimeUnit MassPerTimeUnit::kg_Per_s("kg/s");
const MassPerTimeUnit MassPerTimeUnit::ug_Per_min("ug/min");

template class SEScalarQuantity<MassPerTimeUnit>;

MassPerTimeUnit::MassPerTimeUnit(const char* u)
  : MassPerTimeUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
MassPerTimeUnit::MassPerTimeUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
MassPerTimeUnit::~MassPerTimeUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarMassPerTime::SEScalarMassPerTime()
{
}
//-------------------------------------------------------------------------------
SEScalarMassPerTime::~SEScalarMassPerTime()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarMassPerTimeData* SEScalarMassPerTime::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarMassPerTimeData* data(new CDM::ScalarMassPerTimeData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool MassPerTimeUnit::IsValidUnit(const char* unit)
{
  if (strcmp(g_Per_s.GetString(), unit) == 0)
    return true;
  if (strcmp(g_Per_min.GetString(), unit) == 0)
    return true;
  if (strcmp(g_Per_day.GetString(), unit) == 0)
    return true;
  if (strcmp(mg_Per_s.GetString(), unit) == 0)
    return true;
  if (strcmp(mg_Per_min.GetString(), unit) == 0)
    return true;
  if (strcmp(ug_Per_s.GetString(), unit) == 0)
    return true;
  if (strcmp(kg_Per_s.GetString(), unit) == 0)
    return true;
  if (strcmp(ug_Per_min.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool MassPerTimeUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const MassPerTimeUnit& MassPerTimeUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(g_Per_s.GetString(), unit) == 0)
    return g_Per_s;
  if (strcmp(g_Per_min.GetString(), unit) == 0)
    return g_Per_min;
  if (strcmp(g_Per_day.GetString(), unit) == 0)
    return g_Per_day;
  if (strcmp(mg_Per_s.GetString(), unit) == 0)
    return mg_Per_s;
  if (strcmp(mg_Per_min.GetString(), unit) == 0)
    return mg_Per_min;
  if (strcmp(ug_Per_s.GetString(), unit) == 0)
    return ug_Per_s;
  if (strcmp(kg_Per_s.GetString(), unit) == 0)
    return kg_Per_s;
  if (strcmp(ug_Per_min.GetString(), unit) == 0)
    return ug_Per_min;
  std::stringstream err;
  err << unit << " is not a valid MassPerTime unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const MassPerTimeUnit& MassPerTimeUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool MassPerTimeUnit::operator==(const MassPerTimeUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool MassPerTimeUnit::operator!=(const MassPerTimeUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarMassPerTime::operator==(const SEScalarMassPerTime& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarMassPerTime::operator!=(const SEScalarMassPerTime& obj) const
{
  return !(*this == obj);
}
}