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
class BIOGEARS_API EnergyUnit : public CCompoundUnit {
public:
  EnergyUnit(const char* u);
  EnergyUnit(const std::string& u);

  virtual ~EnergyUnit();

  bool operator==(const EnergyUnit&) const;
  bool operator!=(const EnergyUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const EnergyUnit& GetCompoundUnit(const char* unit);
  static const EnergyUnit& GetCompoundUnit(const std::string& unit);

  static const EnergyUnit J;
  static const EnergyUnit mJ;
  static const EnergyUnit kJ;
  static const EnergyUnit kcal;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<EnergyUnit>;

class BIOGEARS_API SEScalarEnergy : public SEScalarQuantity<EnergyUnit> {
public:
  SEScalarEnergy();
  virtual ~SEScalarEnergy();

  CDM::ScalarEnergyData* Unload() const override;

  using SEScalarQuantity<EnergyUnit>::SetValue;
  using SEScalarQuantity<EnergyUnit>::GetValue;

  bool operator==(const SEScalarEnergy&) const;
  bool operator!=(const SEScalarEnergy&) const;
};
}