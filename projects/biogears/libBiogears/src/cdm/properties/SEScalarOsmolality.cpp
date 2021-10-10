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

#include <biogears/cdm/properties/SEScalarOsmolality.h>

namespace biogears {
const OsmolalityUnit OsmolalityUnit::Osm_Per_kg("Osm/kg");
const OsmolalityUnit OsmolalityUnit::mOsm_Per_kg("mOsm/kg");

template class SEScalarQuantity<OsmolalityUnit>;

OsmolalityUnit::OsmolalityUnit(const char* u)
  : OsmolalityUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
OsmolalityUnit::OsmolalityUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
OsmolalityUnit::~OsmolalityUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarOsmolality::SEScalarOsmolality()
{
}
//-------------------------------------------------------------------------------
SEScalarOsmolality::~SEScalarOsmolality()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarOsmolalityData* SEScalarOsmolality::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarOsmolalityData* data(new CDM::ScalarOsmolalityData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool OsmolalityUnit::IsValidUnit(const char* unit)
{
  if (strcmp(Osm_Per_kg.GetString(), unit) == 0)
    return true;
  if (strcmp(mOsm_Per_kg.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool OsmolalityUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const OsmolalityUnit& OsmolalityUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(Osm_Per_kg.GetString(), unit) == 0)
    return Osm_Per_kg;
  if (strcmp(mOsm_Per_kg.GetString(), unit) == 0)
    return mOsm_Per_kg;
  std::stringstream err;
  err << unit << " is not a valid Osmolality unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const OsmolalityUnit& OsmolalityUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool OsmolalityUnit::operator==(const OsmolalityUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool OsmolalityUnit::operator!=(const OsmolalityUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarOsmolality::operator==(const SEScalarOsmolality& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarOsmolality::operator!=(const SEScalarOsmolality& obj) const
{
  return !(*this == obj);
}
}