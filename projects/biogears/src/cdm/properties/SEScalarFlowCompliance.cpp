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

const FlowComplianceUnit FlowComplianceUnit::L_Per_cmH2O("L/cmH2O");
const FlowComplianceUnit FlowComplianceUnit::mL_Per_mmHg("mL/mmHg");
const FlowComplianceUnit FlowComplianceUnit::mL_Per_cmH2O("mL/cmH2O");
const FlowComplianceUnit FlowComplianceUnit::m3_Per_Pa("m^3/Pa");

CDM::ScalarFlowComplianceData* SEScalarFlowCompliance::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarFlowComplianceData* data(new CDM::ScalarFlowComplianceData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool FlowComplianceUnit::IsValidUnit(const std::string& unit)
{
  if (L_Per_cmH2O.GetString().compare(unit) == 0)
    return true;
  if (mL_Per_mmHg.GetString().compare(unit) == 0)
    return true;
  if (mL_Per_cmH2O.GetString().compare(unit) == 0)
    return true;
  if (m3_Per_Pa.GetString().compare(unit) == 0)
    return true;
  return false;
}

const FlowComplianceUnit& FlowComplianceUnit::GetCompoundUnit(const std::string& unit)
{
  if (L_Per_cmH2O.GetString().compare(unit) == 0)
    return L_Per_cmH2O;
  if (mL_Per_mmHg.GetString().compare(unit) == 0)
    return mL_Per_mmHg;
  if (mL_Per_cmH2O.GetString().compare(unit) == 0)
    return mL_Per_cmH2O;
  if (m3_Per_Pa.GetString().compare(unit) == 0)
    return m3_Per_Pa;
  std::stringstream err;
  err << unit << " is not a valid FlowCompliance unit";
  throw CommonDataModelException(err.str());
}