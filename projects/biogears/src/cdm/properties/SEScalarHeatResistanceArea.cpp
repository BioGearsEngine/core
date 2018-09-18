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

#include <biogears/cdm/properties/SEScalarHeatResistanceArea.h>

const HeatResistanceAreaUnit HeatResistanceAreaUnit::rsi("rsi");
const HeatResistanceAreaUnit HeatResistanceAreaUnit::clo("clo");
const HeatResistanceAreaUnit HeatResistanceAreaUnit::rValue("rValue");
const HeatResistanceAreaUnit HeatResistanceAreaUnit::tog("tog");

CDM::ScalarHeatResistanceAreaData* SEScalarHeatResistanceArea::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarHeatResistanceAreaData* data(new CDM::ScalarHeatResistanceAreaData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool HeatResistanceAreaUnit::IsValidUnit(const std::string& unit)
{
  if (rsi.GetString().compare(unit) == 0)
    return true;
  if (clo.GetString().compare(unit) == 0)
    return true;
  if (rValue.GetString().compare(unit) == 0)
    return true;
  if (tog.GetString().compare(unit) == 0)
    return true;
  return false;
}

const HeatResistanceAreaUnit& HeatResistanceAreaUnit::GetCompoundUnit(const std::string& unit)
{
  if (rsi.GetString().compare(unit) == 0)
    return rsi;
  if (clo.GetString().compare(unit) == 0)
    return clo;
  if (rValue.GetString().compare(unit) == 0)
    return rValue;
  if (tog.GetString().compare(unit) == 0)
    return tog;
  std::stringstream err;
  err << unit << " is not a valid HeatResistanceArea unit";
  throw CommonDataModelException(err.str());
}