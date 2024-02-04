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

#include <biogears/cdm/properties/SEScalarMassPerMass.h>

// Private Includes
#include <io/cdm/Property.h>

namespace biogears {
const MassPerMassUnit MassPerMassUnit::ug_Per_kg("ug/kg");
const MassPerMassUnit MassPerMassUnit::mg_Per_g("mg/g");

template class SEScalarQuantity<MassPerMassUnit>;

MassPerMassUnit::MassPerMassUnit(const char* u)
  : MassPerMassUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
MassPerMassUnit::MassPerMassUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
MassPerMassUnit::~MassPerMassUnit()
{
} //-------------------------------------------------------------------------------
SEScalarMassPerMass::SEScalarMassPerMass()
{
}
//-------------------------------------------------------------------------------
SEScalarMassPerMass::~SEScalarMassPerMass()
{
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
bool SEScalarMassPerMass::Load(const CDM::ScalarMassPerMassData& in)
{
  io::Property::UnMarshall(in, *this);
  return IsValid();
}
//-------------------------------------------------------------------------------
CDM::ScalarMassPerMassData* SEScalarMassPerMass::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarMassPerMassData* data(new CDM::ScalarMassPerMassData());
  Unload(*data);
  return data;
} //-------------------------------------------------------------------------------
void SEScalarMassPerMass::Unload(CDM::ScalarMassPerMassData& data) const
{
  io::Property::Marshall(*this, data);
}
//-------------------------------------------------------------------------------
bool MassPerMassUnit::IsValidUnit(const char* unit)
{
  if (strcmp(ug_Per_kg.GetString(), unit) == 0)
    return true;
  if (strcmp(mg_Per_g.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool MassPerMassUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const MassPerMassUnit& MassPerMassUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(ug_Per_kg.GetString(), unit) == 0)
    return ug_Per_kg;
  if (strcmp(mg_Per_g.GetString(), unit) == 0)
    return mg_Per_g;
  std::stringstream err;
  err << unit << " is not a valid MassPerMass unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const MassPerMassUnit& MassPerMassUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}

//-------------------------------------------------------------------------------
}