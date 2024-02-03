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

#include <biogears/cdm/properties/SEScalarHeatInductance.h>

// Private Includes
#include <io/cdm/Property.h>

namespace biogears {
const HeatInductanceUnit HeatInductanceUnit::K_s_Per_W("K s/W");

template class SEScalarQuantity<HeatInductanceUnit>;

HeatInductanceUnit::HeatInductanceUnit(const char* u)
  : HeatInductanceUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
HeatInductanceUnit::HeatInductanceUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
HeatInductanceUnit::~HeatInductanceUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarHeatInductance::SEScalarHeatInductance()
{
}
//------------------------------------------------------------------------------
SEScalarHeatInductance::~SEScalarHeatInductance()
{
}
//-------------------------------------------------------------------------------
bool SEScalarHeatInductance::Load(const CDM::ScalarHeatInductanceData& in)
{
  io::Property::UnMarshall(in, *this);
  return IsValid();
}
//-------------------------------------------------------------------------------
CDM::ScalarHeatInductanceData* SEScalarHeatInductance::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarHeatInductanceData* data(new CDM::ScalarHeatInductanceData());
  Unload(*data);
  return data;
} //-------------------------------------------------------------------------------
void SEScalarHeatInductance::Unload(CDM::ScalarHeatInductanceData& data) const
{
  io::Property::Marshall(*this, data);
}
//-------------------------------------------------------------------------------
bool HeatInductanceUnit::IsValidUnit(const char* unit)
{
  if (strcmp(K_s_Per_W.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool HeatInductanceUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const HeatInductanceUnit& HeatInductanceUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(K_s_Per_W.GetString(), unit) == 0)
    return K_s_Per_W;
  std::stringstream err;
  err << unit << " is not a valid HeatInductance unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const HeatInductanceUnit& HeatInductanceUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}

//-------------------------------------------------------------------------------
}