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
class BIOGEARS_API ElectricInductanceUnit : public CCompoundUnit {
public:
  ElectricInductanceUnit(const char* u);
  ElectricInductanceUnit(const std::string& u);

  virtual ~ElectricInductanceUnit();

  bool operator==(const ElectricInductanceUnit&) const;
  bool operator!=(const ElectricInductanceUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const ElectricInductanceUnit& GetCompoundUnit(const char* unit);
  static const ElectricInductanceUnit& GetCompoundUnit(const std::string& unit);

  static const ElectricInductanceUnit H;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<ElectricInductanceUnit>;

class BIOGEARS_API SEScalarElectricInductance : public SEScalarQuantity<ElectricInductanceUnit> {
public:
  SEScalarElectricInductance();
  virtual ~SEScalarElectricInductance() ;

  CDM::ScalarElectricInductanceData* Unload() const override;   

  using SEScalarQuantity<ElectricInductanceUnit>::SetValue;
  using SEScalarQuantity<ElectricInductanceUnit>::GetValue;

  bool operator==(const SEScalarElectricInductance&) const;
  bool operator!=(const SEScalarElectricInductance&) const;
};
}