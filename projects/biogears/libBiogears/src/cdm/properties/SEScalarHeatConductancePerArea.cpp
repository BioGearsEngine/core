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

#include <biogears/cdm/properties/SEScalarHeatConductancePerArea.h>

// Private Includes
#include <io/cdm/Property.h>

namespace biogears {
const HeatConductancePerAreaUnit HeatConductancePerAreaUnit::W_Per_m2_K("W/m^2 K");
const HeatConductancePerAreaUnit HeatConductancePerAreaUnit::W_Per_m2_C("W/m^2 degC");
const HeatConductancePerAreaUnit HeatConductancePerAreaUnit::BTU_Per_hr_ft2_C("BTU/hr ft^2 degC");

template class SEScalarQuantity<HeatConductancePerAreaUnit>;

HeatConductancePerAreaUnit::HeatConductancePerAreaUnit(const char* u)
  : HeatConductancePerAreaUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
HeatConductancePerAreaUnit::HeatConductancePerAreaUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
HeatConductancePerAreaUnit::~HeatConductancePerAreaUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarHeatConductancePerArea::SEScalarHeatConductancePerArea()
{
}
//-------------------------------------------------------------------------------
SEScalarHeatConductancePerArea::~SEScalarHeatConductancePerArea()
{
}
//-------------------------------------------------------------------------------
bool SEScalarHeatConductancePerArea::Load(const CDM::ScalarHeatConductancePerAreaData& in)
{
  io::Property::UnMarshall(in, *this);
  return IsValid();
}
//-------------------------------------------------------------------------------
CDM::ScalarHeatConductancePerAreaData* SEScalarHeatConductancePerArea::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarHeatConductancePerAreaData* data(new CDM::ScalarHeatConductancePerAreaData());
  Unload(*data);
  return data;
} //-------------------------------------------------------------------------------
void SEScalarHeatConductancePerArea::Unload(CDM::ScalarHeatConductancePerAreaData& data) const
{
  io::Property::Marshall(*this, data);
}
//-------------------------------------------------------------------------------
bool HeatConductancePerAreaUnit::IsValidUnit(const char* unit)
{
  if (strcmp(W_Per_m2_K.GetString(), unit) == 0)
    return true;
  if (strcmp(W_Per_m2_C.GetString(), unit) == 0)
    return true;
  if (strcmp(BTU_Per_hr_ft2_C.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool HeatConductancePerAreaUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const HeatConductancePerAreaUnit& HeatConductancePerAreaUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(W_Per_m2_K.GetString(), unit) == 0)
    return W_Per_m2_K;
  if (strcmp(W_Per_m2_C.GetString(), unit) == 0)
    return W_Per_m2_C;
  if (strcmp(BTU_Per_hr_ft2_C.GetString(), unit) == 0)
    return BTU_Per_hr_ft2_C;
  std::stringstream err;
  err << unit << " is not a valid HeatConductancePerArea unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const HeatConductancePerAreaUnit& HeatConductancePerAreaUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}

//-------------------------------------------------------------------------------
}