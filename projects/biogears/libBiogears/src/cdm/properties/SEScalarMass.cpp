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
SEScalarMass::~SEScalarMass()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarMassData* SEScalarMass::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarMassData* data(new CDM::ScalarMassData());
  SEScalarQuantity::Unload(*data);
  return data;
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
bool MassUnit::operator==(const MassUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool MassUnit::operator!=(const MassUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------
bool SEScalarMass::operator==(const SEScalarMass& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarMass::operator!=(const SEScalarMass& obj) const
{
  return !(*this == obj);
}
}