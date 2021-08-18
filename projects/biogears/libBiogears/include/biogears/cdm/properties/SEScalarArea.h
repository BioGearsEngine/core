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
class BIOGEARS_API AreaUnit : public CCompoundUnit {
public:
  AreaUnit(const char* u);
  AreaUnit(const std::string& u);

  virtual ~AreaUnit();     

  bool operator==(const AreaUnit&) const;
  bool operator!=(const AreaUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const AreaUnit& GetCompoundUnit(const char* unit);
  static const AreaUnit& GetCompoundUnit(const std::string& unit);

  static const AreaUnit cm2;
  static const AreaUnit m2;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<AreaUnit>;

class BIOGEARS_API SEScalarArea : public SEScalarQuantity<AreaUnit> {
public:
  SEScalarArea();
  virtual ~SEScalarArea();

  CDM::ScalarAreaData* Unload() const override;   

  using SEScalarQuantity<AreaUnit>::SetValue;
  using SEScalarQuantity<AreaUnit>::GetValue;

  bool operator==(const SEScalarArea&) const;
  bool operator!=(const SEScalarArea&) const;
};
}