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
class BIOGEARS_API TemperatureUnit : public CCompoundUnit {
public:
  TemperatureUnit(const char* u);
  TemperatureUnit(const std::string& u);

  virtual ~TemperatureUnit();

  bool operator==(const TemperatureUnit&) const;
  bool operator!=(const TemperatureUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const TemperatureUnit& GetCompoundUnit(const char* unit);
  static const TemperatureUnit& GetCompoundUnit(const std::string& unit);

  static const TemperatureUnit F;
  static const TemperatureUnit C;
  static const TemperatureUnit K;
  static const TemperatureUnit R;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<TemperatureUnit>;

class BIOGEARS_API SEScalarTemperature : public SEScalarQuantity<TemperatureUnit> {
public:
  SEScalarTemperature();
  virtual ~SEScalarTemperature();

  CDM::ScalarTemperatureData* Unload() const override;

  using SEScalarQuantity<TemperatureUnit>::SetValue;
  double GetValue(const TemperatureUnit& unit) const override; // Zero is not Zero for all units, gotta remove that logic for this scalar type    

  bool operator==(const SEScalarTemperature&) const;
  bool operator!=(const SEScalarTemperature&) const;
};
}
