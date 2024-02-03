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

#include <biogears/cdm/properties/SEScalarElectricCapacitance.h>

// Private Includes
#include <io/cdm/Property.h>

namespace biogears {
const ElectricCapacitanceUnit ElectricCapacitanceUnit::F("F");

template class SEScalarQuantity<ElectricCapacitanceUnit>;

ElectricCapacitanceUnit::ElectricCapacitanceUnit(const char* u)
  : ElectricCapacitanceUnit(std::string { u })
{
}
//-----------------------------------------------------------------------------
ElectricCapacitanceUnit::ElectricCapacitanceUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-----------------------------------------------------------------------------
ElectricCapacitanceUnit::~ElectricCapacitanceUnit()
{
}
//-----------------------------------------------------------------------------
SEScalarElectricCapacitance::SEScalarElectricCapacitance(){
}
//-----------------------------------------------------------------------------
SEScalarElectricCapacitance::~SEScalarElectricCapacitance(){
}
//-----------------------------------------------------------------------------
bool SEScalarElectricCapacitance::Load(const CDM::ScalarElectricCapacitanceData& in)
{
  io::Property::UnMarshall(in, *this);
  return IsValid();
}
//-------------------------------------------------------------------------------
CDM::ScalarElectricCapacitanceData* SEScalarElectricCapacitance::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarElectricCapacitanceData* data(new CDM::ScalarElectricCapacitanceData());
  Unload(*data);
  return data;
} //-------------------------------------------------------------------------------
void SEScalarElectricCapacitance::Unload(CDM::ScalarElectricCapacitanceData& data) const
{
  io::Property::Marshall(*this, data);
}
//-----------------------------------------------------------------------------
bool ElectricCapacitanceUnit::IsValidUnit(const char* unit)
{
  if (strcmp(F.GetString(), unit) == 0)
    return true;
  return false;
}
//-----------------------------------------------------------------------------
bool ElectricCapacitanceUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
const ElectricCapacitanceUnit& ElectricCapacitanceUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(F.GetString(), unit) == 0)
    return F;
  std::stringstream err;
  err << unit << " is not a valid ElectricCapacitance unit";
  throw CommonDataModelException(err.str());
}
//-----------------------------------------------------------------------------
const ElectricCapacitanceUnit& ElectricCapacitanceUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
}