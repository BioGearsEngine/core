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

#include <biogears/cdm/properties/SEScalarInversePressure.h>
#include <biogears/cdm/stdafx.h>

const InversePressureUnit InversePressureUnit::Inverse_Pa("1/Pa");
const InversePressureUnit InversePressureUnit::Inverse_mmHg("1/mmHg");
const InversePressureUnit InversePressureUnit::Inverse_cmH2O("1/cmH2O");
const InversePressureUnit InversePressureUnit::Inverse_atm("1/atm");

CDM::ScalarInversePressureData* SEScalarInversePressure::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarInversePressureData* data(new CDM::ScalarInversePressureData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool InversePressureUnit::IsValidUnit(const std::string& unit)
{
  if (Inverse_Pa.GetString().compare(unit) == 0)
    return true;
  if (Inverse_mmHg.GetString().compare(unit) == 0)
    return true;
  if (Inverse_cmH2O.GetString().compare(unit) == 0)
    return true;
  if (Inverse_atm.GetString().compare(unit) == 0)
    return true;
  return false;
}

const InversePressureUnit& InversePressureUnit::GetCompoundUnit(const std::string& unit)
{
  if (Inverse_Pa.GetString().compare(unit) == 0)
    return Inverse_Pa;
  if (Inverse_mmHg.GetString().compare(unit) == 0)
    return Inverse_mmHg;
  if (Inverse_cmH2O.GetString().compare(unit) == 0)
    return Inverse_cmH2O;
  if (Inverse_atm.GetString().compare(unit) == 0)
    return Inverse_atm;
  std::stringstream err;
  err << unit << " is not a valid InversePressure unit";
  throw CommonDataModelException(err.str());
}