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

namespace biogears {
const HeatResistanceAreaUnit HeatResistanceAreaUnit::rsi("rsi");
const HeatResistanceAreaUnit HeatResistanceAreaUnit::clo("clo");
const HeatResistanceAreaUnit HeatResistanceAreaUnit::rValue("rValue");
const HeatResistanceAreaUnit HeatResistanceAreaUnit::tog("tog");

template class SEScalarQuantity<HeatResistanceAreaUnit>;

HeatResistanceAreaUnit::HeatResistanceAreaUnit(const char* u)
  : HeatResistanceAreaUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
HeatResistanceAreaUnit::HeatResistanceAreaUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
HeatResistanceAreaUnit::~HeatResistanceAreaUnit(){
}
//-------------------------------------------------------------------------------
SEScalarHeatResistanceArea::SEScalarHeatResistanceArea()
{
}
//-------------------------------------------------------------------------------
SEScalarHeatResistanceArea::~SEScalarHeatResistanceArea()
{
}

//-------------------------------------------------------------------------------
CDM::ScalarHeatResistanceAreaData* SEScalarHeatResistanceArea::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarHeatResistanceAreaData* data(new CDM::ScalarHeatResistanceAreaData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool HeatResistanceAreaUnit::IsValidUnit(const char* unit)
{
  if (strcmp(rsi.GetString(), unit) == 0)
    return true;
  if (strcmp(clo.GetString(), unit) == 0)
    return true;
  if (strcmp(rValue.GetString(), unit) == 0)
    return true;
  if (strcmp(tog.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool HeatResistanceAreaUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const HeatResistanceAreaUnit& HeatResistanceAreaUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(rsi.GetString(), unit) == 0)
    return rsi;
  if (strcmp(clo.GetString(), unit) == 0)
    return clo;
  if (strcmp(rValue.GetString(), unit) == 0)
    return rValue;
  if (strcmp(tog.GetString(), unit) == 0)
    return tog;
  std::stringstream err;
  err << unit << " is not a valid HeatResistanceArea unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const HeatResistanceAreaUnit& HeatResistanceAreaUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool HeatResistanceAreaUnit::operator==(const HeatResistanceAreaUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool HeatResistanceAreaUnit::operator!=(const HeatResistanceAreaUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------
bool SEScalarHeatResistanceArea::operator==(const SEScalarHeatResistanceArea& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarHeatResistanceArea::operator!=(const SEScalarHeatResistanceArea& obj) const
{
  return !(*this == obj);
}
}