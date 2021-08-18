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
class BIOGEARS_API ElectricResistanceUnit : public CCompoundUnit {
public:
  ElectricResistanceUnit(const char* u);
  ElectricResistanceUnit(const std::string& u);

  virtual ~ElectricResistanceUnit();

  bool operator==(const ElectricResistanceUnit&) const;
  bool operator!=(const ElectricResistanceUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const ElectricResistanceUnit& GetCompoundUnit(const char* unit);
  static const ElectricResistanceUnit& GetCompoundUnit(const std::string& unit);

  static const ElectricResistanceUnit Ohm;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<ElectricResistanceUnit>;

class BIOGEARS_API SEScalarElectricResistance : public SEScalarQuantity<ElectricResistanceUnit> {
public:
  SEScalarElectricResistance();
  virtual ~SEScalarElectricResistance();

  CDM::ScalarElectricResistanceData* Unload() const override;

  using SEScalarQuantity<ElectricResistanceUnit>::SetValue;
  using SEScalarQuantity<ElectricResistanceUnit>::GetValue;

  bool operator==(const SEScalarElectricResistance&) const;
  bool operator!=(const SEScalarElectricResistance&) const;
};
}