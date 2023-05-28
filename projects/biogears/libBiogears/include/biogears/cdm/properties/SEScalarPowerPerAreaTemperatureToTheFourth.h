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

OPEN_BIOGEARS_NAMESPACE
class BIOGEARS_API PowerPerAreaTemperatureToTheFourthUnit : public CCompoundUnit {
public:
  PowerPerAreaTemperatureToTheFourthUnit(const char* u);
  PowerPerAreaTemperatureToTheFourthUnit(const std::string& u);

  virtual ~PowerPerAreaTemperatureToTheFourthUnit();  

  bool operator==(const PowerPerAreaTemperatureToTheFourthUnit&) const;
  bool operator!=(const PowerPerAreaTemperatureToTheFourthUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const PowerPerAreaTemperatureToTheFourthUnit& GetCompoundUnit(const char* unit);
  static const PowerPerAreaTemperatureToTheFourthUnit& GetCompoundUnit(const std::string& unit);

  static const PowerPerAreaTemperatureToTheFourthUnit W_Per_m2_K4;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<PowerPerAreaTemperatureToTheFourthUnit>;

class BIOGEARS_API SEScalarPowerPerAreaTemperatureToTheFourth : public SEScalarQuantity<PowerPerAreaTemperatureToTheFourthUnit> {
public:
  SEScalarPowerPerAreaTemperatureToTheFourth();
  virtual ~SEScalarPowerPerAreaTemperatureToTheFourth();

  CDM::ScalarPowerPerAreaTemperatureToTheFourthData* Unload() const override;

  using SEScalarQuantity<PowerPerAreaTemperatureToTheFourthUnit>::SetValue;
  using SEScalarQuantity<PowerPerAreaTemperatureToTheFourthUnit>::GetValue;

  bool operator==(const SEScalarPowerPerAreaTemperatureToTheFourth&) const;
  bool operator!=(const SEScalarPowerPerAreaTemperatureToTheFourth&) const;
};
CLOSE_BIOGEARS_NAMESPACE