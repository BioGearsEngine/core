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

namespace biogears {
class BIOGEARS_API AmountPerVolumeUnit : public CCompoundUnit {
public:
  AmountPerVolumeUnit(const char* u);
  AmountPerVolumeUnit(const std::string& u);

  virtual ~AmountPerVolumeUnit() = default;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const AmountPerVolumeUnit& GetCompoundUnit(const char* unit);
  static const AmountPerVolumeUnit& GetCompoundUnit(const std::string& unit);

  static AmountPerVolumeUnit mol_Per_L;
  static AmountPerVolumeUnit mol_Per_mL;
  static AmountPerVolumeUnit mmol_Per_L;
  static AmountPerVolumeUnit mmol_Per_mL;
  static AmountPerVolumeUnit ct_Per_L;
  static AmountPerVolumeUnit ct_Per_uL;
};

class BIOGEARS_API SEScalarAmountPerVolume : public SEScalarQuantity<AmountPerVolumeUnit> {

public:
  SEScalarAmountPerVolume() = default;
  virtual ~SEScalarAmountPerVolume() = default;

  CDM::ScalarAmountPerVolumeData* Unload() const override;
};
}