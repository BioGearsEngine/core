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

const TemperatureUnit TemperatureUnit::F("degF");
const TemperatureUnit TemperatureUnit::C("degC");
const TemperatureUnit TemperatureUnit::K("K");
const TemperatureUnit TemperatureUnit::R("degR");

CDM::ScalarTemperatureData* SEScalarTemperature::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarTemperatureData* data(new CDM::ScalarTemperatureData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool TemperatureUnit::IsValidUnit(const std::string& unit)
{
  if (F.GetString().compare(unit) == 0)
    return true;
  if (C.GetString().compare(unit) == 0)
    return true;
  if (K.GetString().compare(unit) == 0)
    return true;
  if (R.GetString().compare(unit) == 0)
    return true;
  return false;
}

const TemperatureUnit& TemperatureUnit::GetCompoundUnit(const std::string& unit)
{
  if (F.GetString().compare(unit) == 0)
    return F;
  if (C.GetString().compare(unit) == 0)
    return C;
  if (K.GetString().compare(unit) == 0)
    return K;
  if (R.GetString().compare(unit) == 0)
    return R;
  std::stringstream err;
  err << unit << " is not a valid Temperature unit";
  throw CommonDataModelException(err.str());
}

double SEScalarTemperature::GetValue(const TemperatureUnit& unit) const
{
  if (m_isnan)
    throw CommonDataModelException("Value is NaN");
  if (m_isinf)
    return m_value;
  if (m_unit == &unit)
    return m_value;
  return Convert(m_value, *m_unit, unit);
}