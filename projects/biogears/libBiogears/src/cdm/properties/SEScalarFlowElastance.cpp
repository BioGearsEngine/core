/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in Elastance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#include <biogears/cdm/properties/SEScalarFlowElastance.h>

namespace biogears {
const FlowElastanceUnit FlowElastanceUnit::cmH2O_Per_L("cmH2O/L");
const FlowElastanceUnit FlowElastanceUnit::mmHg_Per_mL("mmHg/mL");
const FlowElastanceUnit FlowElastanceUnit::Pa_Per_m3("Pa/m^3");

template class SEScalarQuantity<FlowElastanceUnit>;

FlowElastanceUnit::FlowElastanceUnit(const char* u)
  : FlowElastanceUnit(std::string { u })
{
}
//-----------------------------------------------------------------------------
FlowElastanceUnit::FlowElastanceUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-----------------------------------------------------------------------------
FlowElastanceUnit::~FlowElastanceUnit()
{
}
//-----------------------------------------------------------------------------
SEScalarFlowElastance::SEScalarFlowElastance()
{
}
//-----------------------------------------------------------------------------
SEScalarFlowElastance::~SEScalarFlowElastance()
{
}
//-----------------------------------------------------------------------------
CDM::ScalarFlowElastanceData* SEScalarFlowElastance::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarFlowElastanceData* data(new CDM::ScalarFlowElastanceData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
bool FlowElastanceUnit::IsValidUnit(const char* unit)
{
  if (strcmp(cmH2O_Per_L.GetString(), unit) == 0)
    return true;
  if (strcmp(mmHg_Per_mL.GetString(), unit) == 0)
    return true;
  if (strcmp(Pa_Per_m3.GetString(), unit) == 0)
    return true;
  return false;
}
//-----------------------------------------------------------------------------
bool FlowElastanceUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
const FlowElastanceUnit& FlowElastanceUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(cmH2O_Per_L.GetString(), unit) == 0)
    return cmH2O_Per_L;
  if (strcmp(mmHg_Per_mL.GetString(), unit) == 0)
    return mmHg_Per_mL;
  if (strcmp(Pa_Per_m3.GetString(), unit) == 0)
    return Pa_Per_m3;
  std::stringstream err;
  err << unit << " is not a valid FlowElastance unit";
  throw CommonDataModelException(err.str());
}
//-----------------------------------------------------------------------------
const FlowElastanceUnit& FlowElastanceUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
bool FlowElastanceUnit::operator==(const FlowElastanceUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool FlowElastanceUnit::operator!=(const FlowElastanceUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarFlowElastance::operator==(const SEScalarFlowElastance& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarFlowElastance::operator!=(const SEScalarFlowElastance& obj) const
{
  return !(*this == obj);
}
}