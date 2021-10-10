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

namespace biogears {
const InversePressureUnit InversePressureUnit::Inverse_Pa("1/Pa");
const InversePressureUnit InversePressureUnit::Inverse_mmHg("1/mmHg");
const InversePressureUnit InversePressureUnit::Inverse_cmH2O("1/cmH2O");
const InversePressureUnit InversePressureUnit::Inverse_atm("1/atm");

template class SEScalarQuantity<InversePressureUnit>;

InversePressureUnit::InversePressureUnit(const char* u)
  : InversePressureUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
InversePressureUnit::InversePressureUnit(const std::string& u)
  : CCompoundUnit(u)
{
} //-------------------------------------------------------------------------------
InversePressureUnit::~InversePressureUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarInversePressure::SEScalarInversePressure()
{
}
//-------------------------------------------------------------------------------
SEScalarInversePressure::~SEScalarInversePressure()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarInversePressureData* SEScalarInversePressure::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarInversePressureData* data(new CDM::ScalarInversePressureData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool InversePressureUnit::IsValidUnit(const char* unit)
{
  if (strcmp(Inverse_Pa.GetString(), unit) == 0)
    return true;
  if (strcmp(Inverse_mmHg.GetString(), unit) == 0)
    return true;
  if (strcmp(Inverse_cmH2O.GetString(), unit) == 0)
    return true;
  if (strcmp(Inverse_atm.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool InversePressureUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const InversePressureUnit& InversePressureUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(Inverse_Pa.GetString(), unit) == 0)
    return Inverse_Pa;
  if (strcmp(Inverse_mmHg.GetString(), unit) == 0)
    return Inverse_mmHg;
  if (strcmp(Inverse_cmH2O.GetString(), unit) == 0)
    return Inverse_cmH2O;
  if (strcmp(Inverse_atm.GetString(), unit) == 0)
    return Inverse_atm;
  std::stringstream err;
  err << unit << " is not a valid InversePressure unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const InversePressureUnit& InversePressureUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool InversePressureUnit::operator==(const InversePressureUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool InversePressureUnit::operator!=(const InversePressureUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------
bool SEScalarInversePressure::operator==(const SEScalarInversePressure& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarInversePressure::operator!=(const SEScalarInversePressure& obj) const
{
  return !(*this == obj);
}
}