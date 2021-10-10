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
class BIOGEARS_API HeatInductanceUnit : public CCompoundUnit {
public:
  HeatInductanceUnit(const char* u);
  HeatInductanceUnit(const std::string& u);

  virtual ~HeatInductanceUnit();

  bool operator==(const HeatInductanceUnit&) const;
  bool operator!=(const HeatInductanceUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const HeatInductanceUnit& GetCompoundUnit(const char* unit);
  static const HeatInductanceUnit& GetCompoundUnit(const std::string& unit);

  static const HeatInductanceUnit K_s_Per_W;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<HeatInductanceUnit>;

class BIOGEARS_API SEScalarHeatInductance : public SEScalarQuantity<HeatInductanceUnit> {
public:
  SEScalarHeatInductance();
  virtual ~SEScalarHeatInductance();

  CDM::ScalarHeatInductanceData* Unload() const override;

  using SEScalarQuantity<HeatInductanceUnit>::SetValue;
  using SEScalarQuantity<HeatInductanceUnit>::GetValue;

  bool operator==(const SEScalarHeatInductance&) const;
  bool operator!=(const SEScalarHeatInductance&) const;
};
}