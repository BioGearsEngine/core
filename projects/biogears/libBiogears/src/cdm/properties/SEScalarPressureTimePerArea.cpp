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

#include <biogears/cdm/properties/SEScalarPressureTimePerArea.h>

namespace biogears {
const PressureTimePerAreaUnit PressureTimePerAreaUnit::mmHg_Per_mL_m2("mmHg/mL m^2");
const PressureTimePerAreaUnit PressureTimePerAreaUnit::cmH2O_Per_mL_m2("cmH2O/mL m^2");

template class SEScalarQuantity<PressureTimePerAreaUnit>;

PressureTimePerAreaUnit::~PressureTimePerAreaUnit(){

}
//-------------------------------------------------------------------------------
PressureTimePerAreaUnit::PressureTimePerAreaUnit(const char* u)
  : PressureTimePerAreaUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
PressureTimePerAreaUnit::PressureTimePerAreaUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
SEScalarPressureTimePerArea::SEScalarPressureTimePerArea()
{
}
//-------------------------------------------------------------------------------
SEScalarPressureTimePerArea::~SEScalarPressureTimePerArea()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarPressureTimePerAreaData* SEScalarPressureTimePerArea::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarPressureTimePerAreaData* data(new CDM::ScalarPressureTimePerAreaData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool PressureTimePerAreaUnit::IsValidUnit(const char* unit)
{
  if (strcmp(mmHg_Per_mL_m2.GetString(), unit) == 0)
    return true;
  if (strcmp(cmH2O_Per_mL_m2.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool PressureTimePerAreaUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const PressureTimePerAreaUnit& PressureTimePerAreaUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(mmHg_Per_mL_m2.GetString(), unit) == 0)
    return mmHg_Per_mL_m2;
  if (strcmp(cmH2O_Per_mL_m2.GetString(), unit) == 0)
    return cmH2O_Per_mL_m2;
  std::stringstream err;
  err << unit << " is not a valid PressureTimePerArea unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const PressureTimePerAreaUnit& PressureTimePerAreaUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool PressureTimePerAreaUnit::operator==(const PressureTimePerAreaUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool PressureTimePerAreaUnit::operator!=(const PressureTimePerAreaUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarPressureTimePerArea::operator==(const SEScalarPressureTimePerArea& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarPressureTimePerArea::operator!=(const SEScalarPressureTimePerArea& obj) const
{
  return !(*this == obj);
}
}