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
class BIOGEARS_API HeatCapacitanceUnit : public CCompoundUnit {
public:
  HeatCapacitanceUnit(const char* u);
  HeatCapacitanceUnit(const std::string& u);

  virtual ~HeatCapacitanceUnit();       

  bool operator==(const HeatCapacitanceUnit&) const;
  bool operator!=(const HeatCapacitanceUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const HeatCapacitanceUnit& GetCompoundUnit(const char* unit);
  static const HeatCapacitanceUnit& GetCompoundUnit(const std::string& unit);

  static const HeatCapacitanceUnit J_Per_K;
  static const HeatCapacitanceUnit kJ_Per_K;
  static const HeatCapacitanceUnit kcal_Per_K;
  static const HeatCapacitanceUnit kcal_Per_C;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<HeatCapacitanceUnit>;

class BIOGEARS_API SEScalarHeatCapacitance : public SEScalarQuantity<HeatCapacitanceUnit> {
public:
  SEScalarHeatCapacitance();
  virtual ~SEScalarHeatCapacitance();

  CDM::ScalarHeatCapacitanceData* Unload() const override;

  using SEScalarQuantity<HeatCapacitanceUnit>::SetValue;
  using SEScalarQuantity<HeatCapacitanceUnit>::GetValue;

  bool operator==(const SEScalarHeatCapacitance&) const;
  bool operator!=(const SEScalarHeatCapacitance&) const;
};
}