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

#pragma once
#include <biogears/cdm/properties/SEScalarQuantity.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
class BIOGEARS_API OsmolalityUnit : public CCompoundUnit {
public:
  OsmolalityUnit(const char* u);
  OsmolalityUnit(const std::string& u);

  virtual ~OsmolalityUnit();

  bool operator==(const OsmolalityUnit&) const;
  bool operator!=(const OsmolalityUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const OsmolalityUnit& GetCompoundUnit(const char* unit);
  static const OsmolalityUnit& GetCompoundUnit(const std::string& unit);

  static const OsmolalityUnit Osm_Per_kg;
  static const OsmolalityUnit mOsm_Per_kg;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<OsmolalityUnit>;

class BIOGEARS_API SEScalarOsmolality : public SEScalarQuantity<OsmolalityUnit> {
public:
  SEScalarOsmolality();
  virtual ~SEScalarOsmolality();

  CDM::ScalarOsmolalityData* Unload() const override;

  using SEScalarQuantity<OsmolalityUnit>::SetValue;
  using SEScalarQuantity<OsmolalityUnit>::GetValue;

  bool operator==(const SEScalarOsmolality&) const;
  bool operator!=(const SEScalarOsmolality&) const;
};
}