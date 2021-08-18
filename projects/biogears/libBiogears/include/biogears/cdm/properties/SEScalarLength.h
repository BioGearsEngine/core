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
class BIOGEARS_API LengthUnit : public CCompoundUnit {
public:
  LengthUnit(const char* u);
  LengthUnit(const std::string& u);

  virtual ~LengthUnit();

  bool operator==(const LengthUnit&) const;
  bool operator!=(const LengthUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const LengthUnit& GetCompoundUnit(const char* unit);
  static const LengthUnit& GetCompoundUnit(const std::string& unit);

  static const LengthUnit m;
  static const LengthUnit cm;
  static const LengthUnit mm;
  static const LengthUnit um;
  static const LengthUnit inch;
  static const LengthUnit ft;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<LengthUnit>;

class BIOGEARS_API SEScalarLength : public SEScalarQuantity<LengthUnit> {
public:
  SEScalarLength();
  virtual ~SEScalarLength();

  CDM::ScalarLengthData* Unload() const override;

  using SEScalarQuantity<LengthUnit>::SetValue;
  using SEScalarQuantity<LengthUnit>::GetValue;

  bool operator==(const SEScalarLength&) const;
  bool operator!=(const SEScalarLength&) const;
};
}