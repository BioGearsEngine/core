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

#include <biogears/cdm/properties/SEScalarMassPerAreaTime.h>

namespace biogears {
const MassPerAreaTimeUnit MassPerAreaTimeUnit::g_Per_cm2_s("g/cm^2 s");

template class SEScalarQuantity<MassPerAreaTimeUnit>;

MassPerAreaTimeUnit::MassPerAreaTimeUnit(const char* u)
  : MassPerAreaTimeUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
MassPerAreaTimeUnit::MassPerAreaTimeUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
MassPerAreaTimeUnit::~MassPerAreaTimeUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarMassPerAreaTime::SEScalarMassPerAreaTime()
{
}
//-------------------------------------------------------------------------------
SEScalarMassPerAreaTime::~SEScalarMassPerAreaTime()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarMassPerAreaTimeData* SEScalarMassPerAreaTime::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarMassPerAreaTimeData* data(new CDM::ScalarMassPerAreaTimeData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool MassPerAreaTimeUnit::IsValidUnit(const char* unit)
{
  if (strcmp(g_Per_cm2_s.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool MassPerAreaTimeUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const MassPerAreaTimeUnit& MassPerAreaTimeUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(g_Per_cm2_s.GetString(), unit) == 0)
    return g_Per_cm2_s;
  std::stringstream err;
  err << unit << " is not a valid MassPerAreaTime unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const MassPerAreaTimeUnit& MassPerAreaTimeUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool MassPerAreaTimeUnit::operator==(const MassPerAreaTimeUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool MassPerAreaTimeUnit::operator!=(const MassPerAreaTimeUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarMassPerAreaTime::operator==(const SEScalarMassPerAreaTime& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarMassPerAreaTime::operator!=(const SEScalarMassPerAreaTime& obj) const
{
  return !(*this == obj);
}
}