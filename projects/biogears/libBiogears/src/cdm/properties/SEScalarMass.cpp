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

#include <biogears/cdm/properties/SEScalarMass.h>

// Private Includes
#include <io/cdm/Property.h>

namespace biogears {
const MassUnit MassUnit::g("g");
const MassUnit MassUnit::ug("ug");
const MassUnit MassUnit::mg("mg");
const MassUnit MassUnit::kg("kg");
const MassUnit MassUnit::lb("lb");

template class SEScalarQuantity<MassUnit>;

MassUnit::MassUnit(const char* u)
  : MassUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
MassUnit::MassUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
MassUnit::~MassUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarMass::SEScalarMass()
{
}
//-------------------------------------------------------------------------------
SEScalarMass::SEScalarMass(double value, MassUnit const& unit)
  : SEScalarQuantity(value,unit)
{

}
//-------------------------------------------------------------------------------
SEScalarMass::~SEScalarMass()
{
}
//-------------------------------------------------------------------------------
bool SEScalarMass::Load(const CDM::ScalarMassData& in)
{
  io::Property::UnMarshall(in, *this);
  return IsValid();
}
//-------------------------------------------------------------------------------
CDM::ScalarMassData* SEScalarMass::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarMassData* data(new CDM::ScalarMassData());
  Unload(*data);
  return data;
} //-------------------------------------------------------------------------------
void SEScalarMass::Unload(CDM::ScalarMassData& data) const
{
  io::Property::Marshall(*this, data);
}
//-------------------------------------------------------------------------------
bool MassUnit::IsValidUnit(const char* unit)
{
  if (strcmp(g.GetString(), unit) == 0)
    return true;
  if (strcmp(ug.GetString(), unit) == 0)
    return true;
  if (strcmp(mg.GetString(), unit) == 0)
    return true;
  if (strcmp(kg.GetString(), unit) == 0)
    return true;
  if (strcmp(lb.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool MassUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const MassUnit& MassUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(g.GetString(), unit) == 0)
    return g;
  if (strcmp(ug.GetString(), unit) == 0)
    return ug;
  if (strcmp(mg.GetString(), unit) == 0)
    return mg;
  if (strcmp(kg.GetString(), unit) == 0)
    return kg;
  if (strcmp(lb.GetString(), unit) == 0)
    return lb;
  std::stringstream err;
  err << unit << " is not a valid Mass unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const MassUnit& MassUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}

//-------------------------------------------------------------------------------
}