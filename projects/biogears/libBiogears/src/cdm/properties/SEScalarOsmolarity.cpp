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

#include <biogears/cdm/properties/SEScalarOsmolarity.h>

namespace biogears {
const OsmolarityUnit OsmolarityUnit::Osm_Per_L("Osm/L");
const OsmolarityUnit OsmolarityUnit::mOsm_Per_L("mOsm/L");

template class SEScalarQuantity<OsmolarityUnit>;

OsmolarityUnit::OsmolarityUnit(const char* u)
  : OsmolarityUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
OsmolarityUnit::OsmolarityUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
OsmolarityUnit::~OsmolarityUnit()
{
}
//-------------------------------------------------------------------------------
 SEScalarOsmolarity::SEScalarOsmolarity(){
 }
 //-------------------------------------------------------------------------------
  SEScalarOsmolarity::~SEScalarOsmolarity(){
  }
//-------------------------------------------------------------------------------
CDM::ScalarOsmolarityData* SEScalarOsmolarity::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarOsmolarityData* data(new CDM::ScalarOsmolarityData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
bool OsmolarityUnit::IsValidUnit(const char* unit)
{
  if (strcmp(Osm_Per_L.GetString(), unit) == 0)
    return true;
  if (strcmp(mOsm_Per_L.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool OsmolarityUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const OsmolarityUnit& OsmolarityUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(Osm_Per_L.GetString(), unit) == 0)
    return Osm_Per_L;
  if (strcmp(mOsm_Per_L.GetString(), unit) == 0)
    return mOsm_Per_L;
  std::stringstream err;
  err << unit << " is not a valid Osmolarity unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const OsmolarityUnit& OsmolarityUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
bool OsmolarityUnit::operator==(const OsmolarityUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool OsmolarityUnit::operator!=(const OsmolarityUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarOsmolarity::operator==(const SEScalarOsmolarity& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarOsmolarity::operator!=(const SEScalarOsmolarity& obj) const
{
  return !(*this == obj);
}
}