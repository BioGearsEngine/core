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

#include <biogears/cdm/properties/SEScalarFlowCompliance.h>

namespace biogears {
const FlowComplianceUnit FlowComplianceUnit::L_Per_cmH2O("L/cmH2O");
const FlowComplianceUnit FlowComplianceUnit::mL_Per_mmHg("mL/mmHg");
const FlowComplianceUnit FlowComplianceUnit::mL_Per_cmH2O("mL/cmH2O");
const FlowComplianceUnit FlowComplianceUnit::m3_Per_Pa("m^3/Pa");

template class SEScalarQuantity<FlowComplianceUnit>;

FlowComplianceUnit::FlowComplianceUnit(const char* u)
  : FlowComplianceUnit(std::string { u })
{
}
//-----------------------------------------------------------------------------
FlowComplianceUnit::FlowComplianceUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-----------------------------------------------------------------------------
FlowComplianceUnit::~FlowComplianceUnit()
{
}
//-----------------------------------------------------------------------------
SEScalarFlowCompliance::SEScalarFlowCompliance()
{
}
//-----------------------------------------------------------------------------
SEScalarFlowCompliance::~SEScalarFlowCompliance()
{
}
//-----------------------------------------------------------------------------
CDM::ScalarFlowComplianceData* SEScalarFlowCompliance::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarFlowComplianceData* data(new CDM::ScalarFlowComplianceData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
bool FlowComplianceUnit::IsValidUnit(const char* unit)
{
  if (strcmp(L_Per_cmH2O.GetString(), unit) == 0)
    return true;
  if (strcmp(mL_Per_mmHg.GetString(), unit) == 0)
    return true;
  if (strcmp(mL_Per_cmH2O.GetString(), unit) == 0)
    return true;
  if (strcmp(m3_Per_Pa.GetString(), unit) == 0)
    return true;
  return false;
}
//-----------------------------------------------------------------------------
bool FlowComplianceUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
const FlowComplianceUnit& FlowComplianceUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(L_Per_cmH2O.GetString(), unit) == 0)
    return L_Per_cmH2O;
  if (strcmp(mL_Per_mmHg.GetString(), unit) == 0)
    return mL_Per_mmHg;
  if (strcmp(mL_Per_cmH2O.GetString(), unit) == 0)
    return mL_Per_cmH2O;
  if (strcmp(m3_Per_Pa.GetString(), unit) == 0)
    return m3_Per_Pa;
  std::stringstream err;
  err << unit << " is not a valid FlowCompliance unit";
  throw CommonDataModelException(err.str());
}
//-----------------------------------------------------------------------------
const FlowComplianceUnit& FlowComplianceUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
bool FlowComplianceUnit::operator==(const FlowComplianceUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool FlowComplianceUnit::operator!=(const FlowComplianceUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarFlowCompliance::operator==(const SEScalarFlowCompliance& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarFlowCompliance::operator!=(const SEScalarFlowCompliance& obj) const
{
  return !(*this == obj);
}
}