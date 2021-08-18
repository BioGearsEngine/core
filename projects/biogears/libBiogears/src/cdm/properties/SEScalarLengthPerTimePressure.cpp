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

#include <biogears/cdm/properties/SEScalarLengthPerTimePressure.h>

namespace biogears {
const LengthPerTimePressureUnit LengthPerTimePressureUnit::m_Per_s_mmHg("m/s mmHg");
const LengthPerTimePressureUnit LengthPerTimePressureUnit::cm_Per_s_mmHg("cm/s mmHg");
const LengthPerTimePressureUnit LengthPerTimePressureUnit::m_Per_min_mmHg("m/min mmHg");
const LengthPerTimePressureUnit LengthPerTimePressureUnit::cm_Per_min_mmHg("cm/min mmHg");

template class SEScalarQuantity<LengthPerTimePressureUnit>;

LengthPerTimePressureUnit::LengthPerTimePressureUnit(const char* u)
  : LengthPerTimePressureUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
LengthPerTimePressureUnit::LengthPerTimePressureUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
LengthPerTimePressureUnit::~LengthPerTimePressureUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarLengthPerTimePressure::SEScalarLengthPerTimePressure()
{
}
//-------------------------------------------------------------------------------
SEScalarLengthPerTimePressure::~SEScalarLengthPerTimePressure()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarLengthPerTimePressureData* SEScalarLengthPerTimePressure::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarLengthPerTimePressureData* data(new CDM::ScalarLengthPerTimePressureData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool LengthPerTimePressureUnit::IsValidUnit(const char* unit)
{
  if (strcmp(m_Per_s_mmHg.GetString(), unit) == 0)
    return true;
  if (strcmp(cm_Per_s_mmHg.GetString(), unit) == 0)
    return true;
  if (strcmp(m_Per_min_mmHg.GetString(), unit) == 0)
    return true;
  if (strcmp(cm_Per_min_mmHg.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool LengthPerTimePressureUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const LengthPerTimePressureUnit& LengthPerTimePressureUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(m_Per_s_mmHg.GetString(), unit) == 0)
    return m_Per_s_mmHg;
  if (strcmp(cm_Per_s_mmHg.GetString(), unit) == 0)
    return cm_Per_s_mmHg;
  if (strcmp(m_Per_min_mmHg.GetString(), unit) == 0)
    return m_Per_min_mmHg;
  if (strcmp(cm_Per_min_mmHg.GetString(), unit) == 0)
    return cm_Per_min_mmHg;
  std::stringstream err;
  err << unit << " is not a valid LengthPerTimePressure unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const LengthPerTimePressureUnit& LengthPerTimePressureUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool LengthPerTimePressureUnit::operator==(const LengthPerTimePressureUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool LengthPerTimePressureUnit::operator!=(const LengthPerTimePressureUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------
bool SEScalarLengthPerTimePressure::operator==(const SEScalarLengthPerTimePressure& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarLengthPerTimePressure::operator!=(const SEScalarLengthPerTimePressure& obj) const
{
  return !(*this == obj);
}
}