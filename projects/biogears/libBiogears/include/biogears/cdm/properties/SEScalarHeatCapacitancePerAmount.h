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
class BIOGEARS_API HeatCapacitancePerAmountUnit : public CCompoundUnit {
public:
  HeatCapacitancePerAmountUnit(const char* u);
  HeatCapacitancePerAmountUnit(const std::string& u);

  virtual ~HeatCapacitancePerAmountUnit();     

  bool operator==(const HeatCapacitancePerAmountUnit&) const;
  bool operator!=(const HeatCapacitancePerAmountUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const HeatCapacitancePerAmountUnit& GetCompoundUnit(const char* unit);
  static const HeatCapacitancePerAmountUnit& GetCompoundUnit(const std::string& unit);

  static const HeatCapacitancePerAmountUnit J_Per_K_mol;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<HeatCapacitancePerAmountUnit>;

class BIOGEARS_API SEScalarHeatCapacitancePerAmount : public SEScalarQuantity<HeatCapacitancePerAmountUnit> {
public:
  SEScalarHeatCapacitancePerAmount();
  virtual ~SEScalarHeatCapacitancePerAmount();

  CDM::ScalarHeatCapacitancePerAmountData* Unload() const override;

  using SEScalarQuantity<HeatCapacitancePerAmountUnit>::SetValue;
  using SEScalarQuantity<HeatCapacitancePerAmountUnit>::GetValue;

  bool operator==(const SEScalarHeatCapacitancePerAmount&) const;
  bool operator!=(const SEScalarHeatCapacitancePerAmount&) const;
};
}