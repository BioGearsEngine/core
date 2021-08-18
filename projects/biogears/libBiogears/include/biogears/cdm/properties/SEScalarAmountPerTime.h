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
class BIOGEARS_API AmountPerTimeUnit : public CCompoundUnit {
public:
  AmountPerTimeUnit(const char* u);
  AmountPerTimeUnit(const std::string& u);

  virtual ~AmountPerTimeUnit() override;

  bool operator==(const AmountPerTimeUnit&) const;
  bool operator!=(const AmountPerTimeUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const AmountPerTimeUnit& GetCompoundUnit(const char* unit);
  static const AmountPerTimeUnit& GetCompoundUnit(const std::string& unit);

  static AmountPerTimeUnit mol_Per_day;
  static AmountPerTimeUnit mol_Per_s;
  static AmountPerTimeUnit umol_Per_s;
  static AmountPerTimeUnit mmol_Per_min;
  static AmountPerTimeUnit pmol_Per_min;
  static AmountPerTimeUnit umol_Per_min;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<AmountPerTimeUnit>;

class BIOGEARS_API SEScalarAmountPerTime : public SEScalarQuantity<AmountPerTimeUnit> {

public:
  SEScalarAmountPerTime();
  virtual ~SEScalarAmountPerTime();

  CDM::ScalarAmountPerTimeData* Unload() const override;

  using SEScalarQuantity<AmountPerTimeUnit>::SetValue;
  using SEScalarQuantity<AmountPerTimeUnit>::GetValue;

  bool operator==(const SEScalarAmountPerTime&) const;
  bool operator!=(const SEScalarAmountPerTime&) const;
};
}