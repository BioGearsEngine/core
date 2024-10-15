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
class BIOGEARS_API HeatResistanceAreaUnit : public CCompoundUnit {
public:
  HeatResistanceAreaUnit(const char* u);
  HeatResistanceAreaUnit(const std::string& u);

  virtual ~HeatResistanceAreaUnit(); 

  bool operator==(const HeatResistanceAreaUnit&) const;
  bool operator!=(const HeatResistanceAreaUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const HeatResistanceAreaUnit& GetCompoundUnit(const char* unit);
  static const HeatResistanceAreaUnit& GetCompoundUnit(const std::string& unit);

  static const HeatResistanceAreaUnit rsi;
  static const HeatResistanceAreaUnit clo;
  static const HeatResistanceAreaUnit rValue;
  static const HeatResistanceAreaUnit tog;
};

#pragma warning(disable : 4661)
extern template class SEScalarQuantity<HeatResistanceAreaUnit>;

class BIOGEARS_API SEScalarHeatResistanceArea : public SEScalarQuantity<HeatResistanceAreaUnit> {
public:
  SEScalarHeatResistanceArea();
  virtual ~SEScalarHeatResistanceArea();

  using SEScalarQuantity<HeatResistanceAreaUnit>::SetValue;
  using SEScalarQuantity<HeatResistanceAreaUnit>::GetValue;

};
}