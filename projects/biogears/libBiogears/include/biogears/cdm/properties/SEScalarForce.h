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
class BIOGEARS_API ForceUnit : public CCompoundUnit {
public:
  ForceUnit(const char* u);
  ForceUnit(const std::string& u);

  virtual ~ForceUnit();

  bool operator==(const ForceUnit&) const;
  bool operator!=(const ForceUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const ForceUnit& GetCompoundUnit(const char* unit);
  static const ForceUnit& GetCompoundUnit(const std::string& unit);

  static const ForceUnit N;
  static const ForceUnit lbf;
  static const ForceUnit dyn;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<ForceUnit>;

class BIOGEARS_API SEScalarForce : public SEScalarQuantity<ForceUnit> {
public:
  SEScalarForce();
  virtual ~SEScalarForce();

  CDM::ScalarForceData* Unload() const override;

  using SEScalarQuantity<ForceUnit>::SetValue;
  using SEScalarQuantity<ForceUnit>::GetValue;

  bool operator==(const SEScalarForce&) const;
  bool operator!=(const SEScalarForce&) const;
};
}