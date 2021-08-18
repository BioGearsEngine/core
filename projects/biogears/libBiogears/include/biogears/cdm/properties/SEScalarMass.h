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
class BIOGEARS_API MassUnit : public CCompoundUnit {
public:
  static const MassUnit ug;
  static const MassUnit mg;
  static const MassUnit g;
  static const MassUnit kg;
  static const MassUnit lb;

  MassUnit(const char* u);
  MassUnit(const std::string& u);

  virtual ~MassUnit();

  bool operator==(const MassUnit&) const;
  bool operator!=(const MassUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const MassUnit& GetCompoundUnit(const char* unit);
  static const MassUnit& GetCompoundUnit(const std::string& unit);
};
BG_EXT template class BIOGEARS_API SEScalarQuantity<MassUnit>;

class BIOGEARS_API SEScalarMass : public SEScalarQuantity<MassUnit> {
public:
  SEScalarMass();
  virtual ~SEScalarMass();

  CDM::ScalarMassData* Unload() const override;

  using SEScalarQuantity<MassUnit>::SetValue;
  using SEScalarQuantity<MassUnit>::GetValue;

  bool operator==(const SEScalarMass&) const;
  bool operator!=(const SEScalarMass&) const;
};
}