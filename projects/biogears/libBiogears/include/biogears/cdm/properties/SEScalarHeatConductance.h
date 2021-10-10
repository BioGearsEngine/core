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
class BIOGEARS_API HeatConductanceUnit : public CCompoundUnit {
public:
  HeatConductanceUnit(const char* u);
  HeatConductanceUnit(const std::string& u);

  virtual ~HeatConductanceUnit();    

  bool operator==(const HeatConductanceUnit&) const;
  bool operator!=(const HeatConductanceUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const HeatConductanceUnit& GetCompoundUnit(const char* unit);
  static const HeatConductanceUnit& GetCompoundUnit(const std::string& unit);

  static const HeatConductanceUnit W_Per_K;
  static const HeatConductanceUnit W_Per_C;
  static const HeatConductanceUnit kcal_Per_K_s;
  static const HeatConductanceUnit kcal_Per_C_s;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<HeatConductanceUnit>;

class BIOGEARS_API SEScalarHeatConductance : public SEScalarQuantity<HeatConductanceUnit> {
public:
  SEScalarHeatConductance();
  virtual ~SEScalarHeatConductance();

  CDM::ScalarHeatConductanceData* Unload() const override;

  using SEScalarQuantity<HeatConductanceUnit>::SetValue;
  using SEScalarQuantity<HeatConductanceUnit>::GetValue;

  bool operator==(const SEScalarHeatConductance&) const;
  bool operator!=(const SEScalarHeatConductance&) const;
};
}