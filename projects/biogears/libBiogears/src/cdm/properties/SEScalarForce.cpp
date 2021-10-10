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

#include <biogears/cdm/properties/SEScalarForce.h>

namespace biogears {
const ForceUnit ForceUnit::N("N");
const ForceUnit ForceUnit::lbf("lbf");
const ForceUnit ForceUnit::dyn("dyn");

template class SEScalarQuantity<ForceUnit>;

ForceUnit::ForceUnit(const char* u)
  : ForceUnit(std::string { u })
{
}
//-----------------------------------------------------------------------------
ForceUnit::ForceUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-----------------------------------------------------------------------------
ForceUnit::~ForceUnit()
{
}
//-----------------------------------------------------------------------------
SEScalarForce::SEScalarForce()
{
}
//-----------------------------------------------------------------------------
SEScalarForce::~SEScalarForce()
{
}
//-----------------------------------------------------------------------------
CDM::ScalarForceData* SEScalarForce::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarForceData* data(new CDM::ScalarForceData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
bool ForceUnit::IsValidUnit(const char* unit)
{
  if (strcmp(N.GetString(), unit) == 0)
    return true;
  if (strcmp(lbf.GetString(), unit) == 0)
    return true;
  if (strcmp(dyn.GetString(), unit) == 0)
    return true;
  return false;
}
//-----------------------------------------------------------------------------
bool ForceUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
const ForceUnit& ForceUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(N.GetString(), unit) == 0)
    return N;
  if (strcmp(lbf.GetString(), unit) == 0)
    return lbf;
  if (strcmp(dyn.GetString(), unit) == 0)
    return dyn;
  std::stringstream err;
  err << unit << " is not a valid Force unit";
  throw CommonDataModelException(err.str());
}
//-----------------------------------------------------------------------------
const ForceUnit& ForceUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
bool ForceUnit::operator==(const ForceUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool ForceUnit::operator!=(const ForceUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarForce::operator==(const SEScalarForce& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarForce::operator!=(const SEScalarForce& obj) const
{
  return !(*this == obj);
}
}