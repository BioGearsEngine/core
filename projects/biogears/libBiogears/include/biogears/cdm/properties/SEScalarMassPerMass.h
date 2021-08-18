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
class BIOGEARS_API MassPerMassUnit : public CCompoundUnit {
public:
  MassPerMassUnit(const char* u);
  MassPerMassUnit(const std::string& u);

  virtual ~MassPerMassUnit();

  bool operator==(const MassPerMassUnit&) const;
  bool operator!=(const MassPerMassUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const MassPerMassUnit& GetCompoundUnit(const char* unit);
  static const MassPerMassUnit& GetCompoundUnit(const std::string& unit);

  static const MassPerMassUnit ug_Per_kg;
  static const MassPerMassUnit mg_Per_g;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<MassPerMassUnit>;

class BIOGEARS_API SEScalarMassPerMass : public SEScalarQuantity<MassPerMassUnit> {
public:
  SEScalarMassPerMass();
  virtual ~SEScalarMassPerMass();

  CDM::ScalarMassPerMassData* Unload() const override;

  using SEScalarQuantity<MassPerMassUnit>::SetValue;
  using SEScalarQuantity<MassPerMassUnit>::GetValue;

  bool operator==(const SEScalarMassPerMass&) const;
  bool operator!=(const SEScalarMassPerMass&) const;
};
}