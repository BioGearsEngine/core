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
class BIOGEARS_API HeatCapacitancePerMassUnit : public CCompoundUnit {
public:
  HeatCapacitancePerMassUnit(const char* u);
  HeatCapacitancePerMassUnit(const std::string& u);

  virtual ~HeatCapacitancePerMassUnit();

  bool operator==(const HeatCapacitancePerMassUnit&) const;
  bool operator!=(const HeatCapacitancePerMassUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const HeatCapacitancePerMassUnit& GetCompoundUnit(const char* unit);
  static const HeatCapacitancePerMassUnit& GetCompoundUnit(const std::string& unit);

  static const HeatCapacitancePerMassUnit J_Per_K_kg;
  static const HeatCapacitancePerMassUnit kJ_Per_K_kg;
  static const HeatCapacitancePerMassUnit kcal_Per_K_kg;
  static const HeatCapacitancePerMassUnit kcal_Per_C_kg;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<HeatCapacitancePerMassUnit>;

class BIOGEARS_API SEScalarHeatCapacitancePerMass : public SEScalarQuantity<HeatCapacitancePerMassUnit> {
public:
  SEScalarHeatCapacitancePerMass();
  virtual ~SEScalarHeatCapacitancePerMass();

  CDM::ScalarHeatCapacitancePerMassData* Unload() const override;

  using SEScalarQuantity<HeatCapacitancePerMassUnit>::SetValue;
  using SEScalarQuantity<HeatCapacitancePerMassUnit>::GetValue;

  bool operator==(const SEScalarHeatCapacitancePerMass&) const;
  bool operator!=(const SEScalarHeatCapacitancePerMass&) const;
};
}