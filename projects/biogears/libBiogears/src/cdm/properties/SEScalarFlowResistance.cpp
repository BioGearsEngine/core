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

#include <biogears/cdm/properties/SEScalarFlowResistance.h>

namespace biogears {
const FlowResistanceUnit FlowResistanceUnit::cmH2O_s_Per_L("cmH2O s/L");
const FlowResistanceUnit FlowResistanceUnit::mmHg_s_Per_mL("mmHg s/mL");
const FlowResistanceUnit FlowResistanceUnit::mmHg_min_Per_mL("mmHg min/mL");
const FlowResistanceUnit FlowResistanceUnit::mmHg_min_Per_L("mmHg min/L");
const FlowResistanceUnit FlowResistanceUnit::Pa_s_Per_m3("Pa s/m^3");

template class SEScalarQuantity<FlowResistanceUnit>;

FlowResistanceUnit::FlowResistanceUnit(const char* u)
  : FlowResistanceUnit(std::string { u })
{
}
//-----------------------------------------------------------------------------
FlowResistanceUnit::FlowResistanceUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-----------------------------------------------------------------------------
FlowResistanceUnit::~FlowResistanceUnit()
{
}
//-----------------------------------------------------------------------------
SEScalarFlowResistance::SEScalarFlowResistance()
{
}
//-----------------------------------------------------------------------------
SEScalarFlowResistance::~SEScalarFlowResistance()
{
}
//-----------------------------------------------------------------------------
CDM::ScalarFlowResistanceData* SEScalarFlowResistance::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarFlowResistanceData* data(new CDM::ScalarFlowResistanceData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
bool FlowResistanceUnit::IsValidUnit(const char* unit)
{
  if (strcmp(cmH2O_s_Per_L.GetString(), unit) == 0)
    return true;
  if (strcmp(mmHg_s_Per_mL.GetString(), unit) == 0)
    return true;
  if (strcmp(mmHg_min_Per_mL.GetString(), unit) == 0)
    return true;
  if (strcmp(mmHg_min_Per_L.GetString(), unit) == 0)
    return true;
  if (strcmp(Pa_s_Per_m3.GetString(), unit) == 0)
    return true;
  return false;
}
//-----------------------------------------------------------------------------
bool FlowResistanceUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
const FlowResistanceUnit& FlowResistanceUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(cmH2O_s_Per_L.GetString(), unit) == 0)
    return cmH2O_s_Per_L;
  if (strcmp(mmHg_s_Per_mL.GetString(), unit) == 0)
    return mmHg_s_Per_mL;
  if (strcmp(mmHg_min_Per_mL.GetString(), unit) == 0)
    return mmHg_min_Per_mL;
  if (strcmp(mmHg_min_Per_L.GetString(), unit) == 0)
    return mmHg_min_Per_L;
  if (strcmp(Pa_s_Per_m3.GetString(), unit) == 0)
    return Pa_s_Per_m3;
  std::stringstream err;
  err << unit << " is not a valid FlowResistance unit";
  throw CommonDataModelException(err.str());
}
//-----------------------------------------------------------------------------
const FlowResistanceUnit& FlowResistanceUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
bool FlowResistanceUnit::operator==(const FlowResistanceUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool FlowResistanceUnit::operator!=(const FlowResistanceUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarFlowResistance::operator==(const SEScalarFlowResistance& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarFlowResistance::operator!=(const SEScalarFlowResistance& obj) const
{
  return !(*this == obj);
}
}