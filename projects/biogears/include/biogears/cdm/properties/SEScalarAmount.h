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
#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/schema/ScalarAmountData.hxx>

class BIOGEARS_API AmountUnit : public CCompoundUnit
{
public:
  AmountUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~AmountUnit() {}

  static bool IsValidUnit(const std::string& unit);
  static const AmountUnit& GetCompoundUnit(const std::string& unit);

  static const AmountUnit mol;
  static const AmountUnit pmol;
};

class BIOGEARS_API SEScalarAmount : public SEScalarQuantity<AmountUnit>
{
public:
  SEScalarAmount() {}
	virtual ~SEScalarAmount() {}

  virtual CDM::ScalarAmountData* Unload() const;
};
