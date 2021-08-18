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
class BIOGEARS_API EnergyPerMassUnit : public CCompoundUnit {
public:
  EnergyPerMassUnit(const char* u);
  EnergyPerMassUnit(const std::string& u);

  virtual ~EnergyPerMassUnit(); 

  bool operator==(const EnergyPerMassUnit&) const;
  bool operator!=(const EnergyPerMassUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const EnergyPerMassUnit& GetCompoundUnit(const char* unit);
  static const EnergyPerMassUnit& GetCompoundUnit(const std::string& unit);

  static const EnergyPerMassUnit J_Per_kg;
  static const EnergyPerMassUnit kJ_Per_kg;
  static const EnergyPerMassUnit mJ_Per_kg;
  static const EnergyPerMassUnit kcal_Per_kg;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<EnergyPerMassUnit>;

class BIOGEARS_API SEScalarEnergyPerMass : public SEScalarQuantity<EnergyPerMassUnit> {
public:
  SEScalarEnergyPerMass();
  virtual ~SEScalarEnergyPerMass();

  CDM::ScalarEnergyPerMassData* Unload() const override;   

  using SEScalarQuantity<EnergyPerMassUnit>::SetValue;
  using SEScalarQuantity<EnergyPerMassUnit>::GetValue;

  bool operator==(const SEScalarEnergyPerMass&) const;
  bool operator!=(const SEScalarEnergyPerMass&) const;
};
}