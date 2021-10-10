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

#include <biogears/cdm/properties/SEScalarTemperature.h>

namespace biogears {
const TemperatureUnit TemperatureUnit::F("degF");
const TemperatureUnit TemperatureUnit::C("degC");
const TemperatureUnit TemperatureUnit::K("K");
const TemperatureUnit TemperatureUnit::R("degR");

template class SEScalarQuantity<TemperatureUnit>;

TemperatureUnit::TemperatureUnit(const char* u)
  : TemperatureUnit(std::string{ u })
{
}
//-------------------------------------------------------------------------------
TemperatureUnit::~TemperatureUnit()
{}
//-------------------------------------------------------------------------------
TemperatureUnit::TemperatureUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
CDM::ScalarTemperatureData* SEScalarTemperature::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarTemperatureData* data(new CDM::ScalarTemperatureData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool TemperatureUnit::IsValidUnit(const char* unit)
{
  if (strcmp(F.GetString(),unit) == 0)
    return true;
  if (strcmp(C.GetString(),unit) == 0)
    return true;
  if (strcmp(K.GetString(),unit) == 0)
    return true;
  if (strcmp(R.GetString(),unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool TemperatureUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const TemperatureUnit& TemperatureUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(F.GetString(),unit) == 0)
    return F;
  if (strcmp(C.GetString(),unit) == 0)
    return C;
  if (strcmp(K.GetString(),unit) == 0)
    return K;
  if (strcmp(R.GetString(),unit) == 0)
    return R;
  std::stringstream err;
  err << unit << " is not a valid Temperature unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const TemperatureUnit& TemperatureUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
double SEScalarTemperature::GetValue(const TemperatureUnit& unit) const
{
  if (std::isnan(m_value))
    throw CommonDataModelException("Value is NaN");
  if (std::isinf(m_value))
    return m_value;
  if (m_unit == &unit)
    return m_value;
  return Convert(m_value, *m_unit, unit);
}
//-------------------------------------------------------------------------------
bool TemperatureUnit::operator==(const TemperatureUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool TemperatureUnit::operator!=(const TemperatureUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------
SEScalarTemperature::SEScalarTemperature(){}
SEScalarTemperature::~SEScalarTemperature(){}
//-------------------------------------------------------------------------------
bool SEScalarTemperature::operator==(const SEScalarTemperature& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarTemperature::operator!=(const SEScalarTemperature& obj) const
{
  return !(*this == obj);
}
}
