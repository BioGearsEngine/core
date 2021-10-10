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

#include <biogears/cdm/properties/SEScalarLengthPerTime.h>

namespace biogears {
const LengthPerTimeUnit LengthPerTimeUnit::m_Per_s("m/s");
const LengthPerTimeUnit LengthPerTimeUnit::cm_Per_s("cm/s");
const LengthPerTimeUnit LengthPerTimeUnit::m_Per_min("m/min");
const LengthPerTimeUnit LengthPerTimeUnit::cm_Per_min("cm/min");
const LengthPerTimeUnit LengthPerTimeUnit::ft_Per_s("ft/s");
const LengthPerTimeUnit LengthPerTimeUnit::ft_Per_min("ft/min");

template class SEScalarQuantity<LengthPerTimeUnit>;

LengthPerTimeUnit::LengthPerTimeUnit(const char* u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
LengthPerTimeUnit::LengthPerTimeUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
LengthPerTimeUnit::~LengthPerTimeUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarLengthPerTime::SEScalarLengthPerTime()
{
}
//-------------------------------------------------------------------------------
SEScalarLengthPerTime::~SEScalarLengthPerTime()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarLengthPerTimeData* SEScalarLengthPerTime::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarLengthPerTimeData* data(new CDM::ScalarLengthPerTimeData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool LengthPerTimeUnit::IsValidUnit(const char* unit)
{
  if (strcmp(m_Per_s.GetString(), unit) == 0)
    return true;
  if (strcmp(cm_Per_s.GetString(), unit) == 0)
    return true;
  if (strcmp(m_Per_min.GetString(), unit) == 0)
    return true;
  if (strcmp(cm_Per_min.GetString(), unit) == 0)
    return true;
  if (strcmp(ft_Per_s.GetString(), unit) == 0)
    return true;
  if (strcmp(ft_Per_min.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool LengthPerTimeUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const LengthPerTimeUnit& LengthPerTimeUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(m_Per_s.GetString(), unit) == 0)
    return m_Per_s;
  if (strcmp(cm_Per_s.GetString(), unit) == 0)
    return cm_Per_s;
  if (strcmp(m_Per_min.GetString(), unit) == 0)
    return m_Per_min;
  if (strcmp(cm_Per_min.GetString(), unit) == 0)
    return cm_Per_min;
  if (strcmp(ft_Per_s.GetString(), unit) == 0)
    return ft_Per_s;
  if (strcmp(ft_Per_min.GetString(), unit) == 0)
    return ft_Per_min;
  std::stringstream err;
  err << unit << " is not a valid LengthPerTime unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const LengthPerTimeUnit& LengthPerTimeUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool LengthPerTimeUnit::operator==(const LengthPerTimeUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool LengthPerTimeUnit::operator!=(const LengthPerTimeUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------
bool SEScalarLengthPerTime::operator==(const SEScalarLengthPerTime& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarLengthPerTime::operator!=(const SEScalarLengthPerTime& obj) const
{
  return !(*this == obj);
}
}