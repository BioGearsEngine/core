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
class BIOGEARS_API HeatResistanceUnit : public CCompoundUnit {
public:
  HeatResistanceUnit(const char* u);
  HeatResistanceUnit(const std::string& u);

  virtual ~HeatResistanceUnit();  

  bool operator==(const HeatResistanceUnit&) const;
  bool operator!=(const HeatResistanceUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const HeatResistanceUnit& GetCompoundUnit(const char* unit);
  static const HeatResistanceUnit& GetCompoundUnit(const std::string& unit);

  static const HeatResistanceUnit K_Per_W;
  static const HeatResistanceUnit C_Per_W;
  static const HeatResistanceUnit K_s_Per_kcal;
  static const HeatResistanceUnit C_s_Per_kcal;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<HeatResistanceUnit>;

class BIOGEARS_API SEScalarHeatResistance : public SEScalarQuantity<HeatResistanceUnit> {
public:
  SEScalarHeatResistance();
  virtual ~SEScalarHeatResistance();

  CDM::ScalarHeatResistanceData* Unload() const override;

  using SEScalarQuantity<HeatResistanceUnit>::SetValue;
  using SEScalarQuantity<HeatResistanceUnit>::GetValue;

  bool operator==(const SEScalarHeatResistance&) const;
  bool operator!=(const SEScalarHeatResistance&) const;
};
}