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

#include <biogears/cdm/properties/SEScalarArea.h>

namespace biogears {
const AreaUnit AreaUnit::cm2("cm^2");
const AreaUnit AreaUnit::m2("m^2");

template class SEScalarQuantity<AreaUnit>;

AreaUnit::AreaUnit(const char* u)
  : AreaUnit(std::string { u })
{
}
//-----------------------------------------------------------------------------
AreaUnit::AreaUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-----------------------------------------------------------------------------
AreaUnit::~AreaUnit(){
}
//-----------------------------------------------------------------------------
SEScalarArea::SEScalarArea()
{
}
//-----------------------------------------------------------------------------
SEScalarArea::~SEScalarArea()
{
}
//-----------------------------------------------------------------------------
CDM::ScalarAreaData* SEScalarArea::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarAreaData* data(new CDM::ScalarAreaData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
bool AreaUnit::IsValidUnit(const char* unit)
{
  if (strcmp(cm2.GetString(), unit) == 0)
    return true;
  if (strcmp(m2.GetString(), unit) == 0)
    return true;
  return false;
}
//-----------------------------------------------------------------------------
bool AreaUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
const AreaUnit& AreaUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(cm2.GetString(), unit) == 0)
    return cm2;
  if (strcmp(m2.GetString(), unit) == 0)
    return m2;
  std::stringstream err;
  err << unit << " is not a valid Area unit";
  throw CommonDataModelException(err.str());
}
//-----------------------------------------------------------------------------
const AreaUnit& AreaUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
bool AreaUnit::operator==(const AreaUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool AreaUnit::operator!=(const AreaUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarArea::operator==(const SEScalarArea& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarArea::operator!=(const SEScalarArea& obj) const
{
  return !(*this == obj);
}
}