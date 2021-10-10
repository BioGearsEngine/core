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
class BIOGEARS_API PressureTimePerAreaUnit : public CCompoundUnit {
public:
  PressureTimePerAreaUnit(const char* u);
  PressureTimePerAreaUnit(const std::string& u);

  virtual ~PressureTimePerAreaUnit(); 

  bool operator==(const PressureTimePerAreaUnit&) const;
  bool operator!=(const PressureTimePerAreaUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const PressureTimePerAreaUnit& GetCompoundUnit(const char* unit);
  static const PressureTimePerAreaUnit& GetCompoundUnit(const std::string& unit);

  static const PressureTimePerAreaUnit mmHg_Per_mL_m2;
  static const PressureTimePerAreaUnit cmH2O_Per_mL_m2;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<PressureTimePerAreaUnit>;

class BIOGEARS_API SEScalarPressureTimePerArea : public SEScalarQuantity<PressureTimePerAreaUnit> {
public:
  SEScalarPressureTimePerArea();
  virtual ~SEScalarPressureTimePerArea();

  CDM::ScalarPressureTimePerAreaData* Unload() const override;

  using SEScalarQuantity<PressureTimePerAreaUnit>::SetValue;
  using SEScalarQuantity<PressureTimePerAreaUnit>::GetValue;

  bool operator==(const SEScalarPressureTimePerArea&) const;
  bool operator!=(const SEScalarPressureTimePerArea&) const;
};
}