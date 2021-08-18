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
class BIOGEARS_API AmountPerMassUnit : public CCompoundUnit {
public:
  AmountPerMassUnit(const char* u);
  AmountPerMassUnit(const std::string& u);

  virtual ~AmountPerMassUnit();

  bool operator==(const AmountPerMassUnit&) const;
  bool operator!=(const AmountPerMassUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const AmountPerMassUnit& GetCompoundUnit(const char* unit);
  static const AmountPerMassUnit& GetCompoundUnit(const std::string& unit);

  static AmountPerMassUnit ct_Per_g;
  static AmountPerMassUnit ct_Per_ug;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<AmountPerMassUnit>;

class BIOGEARS_API SEScalarAmountPerMass : public SEScalarQuantity<AmountPerMassUnit> {

public:
  SEScalarAmountPerMass();
  virtual ~SEScalarAmountPerMass();

  using SEScalarQuantity<AmountPerMassUnit>::SetValue;
  using SEScalarQuantity<AmountPerMassUnit>::GetValue;

  CDM::ScalarAmountPerMassData* Unload() const override;    

  bool operator==(const SEScalarAmountPerMass&) const;
  bool operator!=(const SEScalarAmountPerMass&) const;
};
}