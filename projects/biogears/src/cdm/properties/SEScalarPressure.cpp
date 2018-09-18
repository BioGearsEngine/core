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

#include <biogears/cdm/properties/SEScalarPressure.h>

const PressureUnit PressureUnit::Pa("Pa");
const PressureUnit PressureUnit::mmHg("mmHg");
const PressureUnit PressureUnit::cmH2O("cmH2O");
const PressureUnit PressureUnit::psi("psi");
const PressureUnit PressureUnit::atm("atm");

CDM::ScalarPressureData* SEScalarPressure::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarPressureData* data(new CDM::ScalarPressureData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool PressureUnit::IsValidUnit(const std::string& unit)
{
  if (Pa.GetString().compare(unit) == 0)
    return true;
  if (mmHg.GetString().compare(unit) == 0)
    return true;
  if (cmH2O.GetString().compare(unit) == 0)
    return true;
  if (psi.GetString().compare(unit) == 0)
    return true;
  if (atm.GetString().compare(unit) == 0)
    return true;
  return false;
}

const PressureUnit& PressureUnit::GetCompoundUnit(const std::string& unit)
{
  if (Pa.GetString().compare(unit) == 0)
    return Pa;
  if (mmHg.GetString().compare(unit) == 0)
    return mmHg;
  if (cmH2O.GetString().compare(unit) == 0)
    return cmH2O;
  if (psi.GetString().compare(unit) == 0)
    return psi;
  if (atm.GetString().compare(unit) == 0)
    return atm;
  std::stringstream err;
  err << unit << " is not a valid Pressure unit";
  throw CommonDataModelException(err.str());
}