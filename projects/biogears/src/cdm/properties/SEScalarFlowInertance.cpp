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

#include <biogears/cdm/properties/SEScalarFlowInertance.h>

const FlowInertanceUnit FlowInertanceUnit::mmHg_s2_Per_mL("mmHg s^2/mL");
const FlowInertanceUnit FlowInertanceUnit::mmHg_s2_Per_L("mmHg s^2/L");
const FlowInertanceUnit FlowInertanceUnit::cmH2O_s2_Per_mL("cmH2O s^2/mL");
const FlowInertanceUnit FlowInertanceUnit::cmH2O_s2_Per_L("cmH2O s^2/L");
const FlowInertanceUnit FlowInertanceUnit::Pa_s2_Per_m3("Pa s^2/m^3");

CDM::ScalarFlowInertanceData* SEScalarFlowInertance::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarFlowInertanceData* data(new CDM::ScalarFlowInertanceData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool FlowInertanceUnit::IsValidUnit(const std::string& unit)
{
  if (mmHg_s2_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (mmHg_s2_Per_L.GetString().compare(unit) == 0)
    return true;
  if (cmH2O_s2_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (cmH2O_s2_Per_L.GetString().compare(unit) == 0)
    return true;
  if (Pa_s2_Per_m3.GetString().compare(unit) == 0)
    return true;
  return false;
}

const FlowInertanceUnit& FlowInertanceUnit::GetCompoundUnit(const std::string& unit)
{
  if (mmHg_s2_Per_mL.GetString().compare(unit) == 0)
    return mmHg_s2_Per_mL;
  if (mmHg_s2_Per_L.GetString().compare(unit) == 0)
    return mmHg_s2_Per_L;
  if (cmH2O_s2_Per_mL.GetString().compare(unit) == 0)
    return cmH2O_s2_Per_mL;
  if (cmH2O_s2_Per_L.GetString().compare(unit) == 0)
    return cmH2O_s2_Per_L;
  if (Pa_s2_Per_m3.GetString().compare(unit) == 0)
    return Pa_s2_Per_m3;
  std::stringstream err;
  err << unit << " is not a valid FlowInertance unit";
  throw CommonDataModelException(err.str());
}