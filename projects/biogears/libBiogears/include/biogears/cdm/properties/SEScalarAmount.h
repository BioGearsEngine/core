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
class BIOGEARS_API AmountUnit : public CCompoundUnit {
public:
  AmountUnit(const char* u);
  AmountUnit(const std::string& u);

  virtual ~AmountUnit();

  bool operator==(const AmountUnit&) const;
  bool operator!=(const AmountUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);

  static const AmountUnit& GetCompoundUnit(const char* unit);
  static const AmountUnit& GetCompoundUnit(const std::string& unit);

  static const AmountUnit mol;
  static const AmountUnit pmol;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<AmountUnit>;

class BIOGEARS_API SEScalarAmount : public SEScalarQuantity<AmountUnit> {
public:
  SEScalarAmount();
  virtual ~SEScalarAmount();

  virtual CDM::ScalarAmountData* Unload() const override;   

  using SEScalarQuantity<AmountUnit>::SetValue;
  using SEScalarQuantity<AmountUnit>::GetValue;

  bool operator==(const SEScalarAmount&) const;
  bool operator!=(const SEScalarAmount&) const;
};
}