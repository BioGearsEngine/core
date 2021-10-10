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

#include <biogears/cdm/properties/SEScalarFrequency.h>

namespace biogears {
const FrequencyUnit FrequencyUnit::Per_min("1/min");
const FrequencyUnit FrequencyUnit::Per_s("1/s");
const FrequencyUnit FrequencyUnit::Hz("Hz");
const FrequencyUnit FrequencyUnit::Per_hr("1/hr");

template class SEScalarQuantity<FrequencyUnit>;

FrequencyUnit::FrequencyUnit(const char* u)
  : FrequencyUnit(std::string { u })
{
}
//-----------------------------------------------------------------------------
FrequencyUnit::FrequencyUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-----------------------------------------------------------------------------
FrequencyUnit::~FrequencyUnit()
{
}
//-----------------------------------------------------------------------------
SEScalarFrequency::SEScalarFrequency()
{
}
//-----------------------------------------------------------------------------
SEScalarFrequency::~SEScalarFrequency()
{
}
//-----------------------------------------------------------------------------
CDM::ScalarFrequencyData* SEScalarFrequency::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarFrequencyData* data(new CDM::ScalarFrequencyData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
bool FrequencyUnit::IsValidUnit(const char* unit)
{
  if (strcmp(Per_min.GetString(), unit) == 0)
    return true;
  if (strcmp(Per_s.GetString(), unit) == 0)
    return true;
  if (strcmp(Hz.GetString(), unit) == 0)
    return true;
  if (strcmp(Per_hr.GetString(), unit) == 0)
    return true;

  return false;
}
//-----------------------------------------------------------------------------
bool FrequencyUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
const FrequencyUnit& FrequencyUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(Per_min.GetString(), unit) == 0)
    return Per_min;
  if (strcmp(Per_s.GetString(), unit) == 0)
    return Per_s;
  if (strcmp(Hz.GetString(), unit) == 0)
    return Hz;
  if (strcmp(Per_hr.GetString(), unit) == 0)
    return Per_hr;
  std::stringstream err;
  err << unit << " is not a valid Frequency unit";
  throw CommonDataModelException(err.str());
}
//-----------------------------------------------------------------------------
const FrequencyUnit& FrequencyUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
bool FrequencyUnit::operator==(const FrequencyUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool FrequencyUnit::operator!=(const FrequencyUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------
bool SEScalarFrequency::operator==(const SEScalarFrequency& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarFrequency::operator!=(const SEScalarFrequency& obj) const
{
  return !(*this == obj);
}
}