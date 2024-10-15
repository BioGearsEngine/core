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

namespace biogears {
class BIOGEARS_API OsmolarityUnit : public CCompoundUnit {
public:
  OsmolarityUnit(const char* u);
  OsmolarityUnit(const std::string& u);

  virtual ~OsmolarityUnit();   

  bool operator==(const OsmolarityUnit&) const;
  bool operator!=(const OsmolarityUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const OsmolarityUnit& GetCompoundUnit(const char* unit);
  static const OsmolarityUnit& GetCompoundUnit(const std::string& unit);

  static const OsmolarityUnit Osm_Per_L;
  static const OsmolarityUnit mOsm_Per_L;
};

#pragma warning(disable : 4661)
extern template class SEScalarQuantity<OsmolarityUnit>;

class BIOGEARS_API SEScalarOsmolarity : public SEScalarQuantity<OsmolarityUnit> {
public:
  SEScalarOsmolarity();
  virtual ~SEScalarOsmolarity();

  using SEScalarQuantity<OsmolarityUnit>::SetValue;
  using SEScalarQuantity<OsmolarityUnit>::GetValue;
};
}