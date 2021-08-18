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
class BIOGEARS_API MassPerAmountUnit : public CCompoundUnit {
public:
  MassPerAmountUnit(const char* u);
  MassPerAmountUnit(const std::string& u);

  virtual ~MassPerAmountUnit();

  bool operator==(const MassPerAmountUnit&) const;
  bool operator!=(const MassPerAmountUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const MassPerAmountUnit& GetCompoundUnit(const char* unit);
  static const MassPerAmountUnit& GetCompoundUnit(const std::string& unit);

  static const MassPerAmountUnit g_Per_ct;
  static const MassPerAmountUnit g_Per_mol;
  static const MassPerAmountUnit g_Per_mmol;
  static const MassPerAmountUnit g_Per_umol;
  static const MassPerAmountUnit mg_Per_mol;
  static const MassPerAmountUnit mg_Per_mmol;
  static const MassPerAmountUnit kg_Per_mol;
  static const MassPerAmountUnit ug_Per_mol;
  static const MassPerAmountUnit ug_Per_mmol;
  static const MassPerAmountUnit ug_Per_ct;
  static const MassPerAmountUnit pg_Per_ct;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<MassPerAmountUnit>;

class BIOGEARS_API SEScalarMassPerAmount : public SEScalarQuantity<MassPerAmountUnit> {
public:
  SEScalarMassPerAmount();
  virtual ~SEScalarMassPerAmount();

  CDM::ScalarMassPerAmountData* Unload() const override;

  using SEScalarQuantity<MassPerAmountUnit>::SetValue;
  using SEScalarQuantity<MassPerAmountUnit>::GetValue;

  bool operator==(const SEScalarMassPerAmount&) const;
  bool operator!=(const SEScalarMassPerAmount&) const;
};
}