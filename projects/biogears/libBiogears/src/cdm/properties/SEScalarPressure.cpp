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

namespace biogears {
const PressureUnit PressureUnit::Pa("Pa");
const PressureUnit PressureUnit::mmHg("mmHg");
const PressureUnit PressureUnit::cmH2O("cmH2O");
const PressureUnit PressureUnit::psi("psi");
const PressureUnit PressureUnit::atm("atm");

template class SEScalarQuantity<PressureUnit>;

PressureUnit::PressureUnit(const char* u)
  : PressureUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
PressureUnit::PressureUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
PressureUnit::~PressureUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarPressure::SEScalarPressure()
{
}
//-------------------------------------------------------------------------------
SEScalarPressure::~SEScalarPressure()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarPressureData* SEScalarPressure::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarPressureData* data(new CDM::ScalarPressureData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool PressureUnit::IsValidUnit(const char* unit)
{
  if (strcmp(Pa.GetString(), unit) == 0)
    return true;
  if (strcmp(mmHg.GetString(), unit) == 0)
    return true;
  if (strcmp(cmH2O.GetString(), unit) == 0)
    return true;
  if (strcmp(psi.GetString(), unit) == 0)
    return true;
  if (strcmp(atm.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool PressureUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const PressureUnit& PressureUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(Pa.GetString(), unit) == 0)
    return Pa;
  if (strcmp(mmHg.GetString(), unit) == 0)
    return mmHg;
  if (strcmp(cmH2O.GetString(), unit) == 0)
    return cmH2O;
  if (strcmp(psi.GetString(), unit) == 0)
    return psi;
  if (strcmp(atm.GetString(), unit) == 0)
    return atm;
  std::stringstream err;
  err << unit << " is not a valid Pressure unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const PressureUnit& PressureUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool PressureUnit::operator==(const PressureUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool PressureUnit::operator!=(const PressureUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarPressure::operator==(const SEScalarPressure& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarPressure::operator!=(const SEScalarPressure& obj) const
{
  return !(*this == obj);
}
}