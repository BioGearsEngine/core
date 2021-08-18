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
class BIOGEARS_API ElectricCurrentUnit : public CCompoundUnit {
public:
  ElectricCurrentUnit(const char* u);
  ElectricCurrentUnit(const std::string& u);

  virtual ~ElectricCurrentUnit();

  bool operator==(const ElectricCurrentUnit&) const;
  bool operator!=(const ElectricCurrentUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const ElectricCurrentUnit& GetCompoundUnit(const char* unit);
  static const ElectricCurrentUnit& GetCompoundUnit(const std::string& unit);

  static const ElectricCurrentUnit A;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<ElectricCurrentUnit>;

class BIOGEARS_API SEScalarElectricCurrent : public SEScalarQuantity<ElectricCurrentUnit> {
public:
  SEScalarElectricCurrent();
  virtual ~SEScalarElectricCurrent();

  CDM::ScalarElectricCurrentData* Unload() const override;

  using SEScalarQuantity<ElectricCurrentUnit>::SetValue;
  using SEScalarQuantity<ElectricCurrentUnit>::GetValue;

  bool operator==(const SEScalarElectricCurrent&) const;
  bool operator!=(const SEScalarElectricCurrent&) const;
};
}