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
class BIOGEARS_API AreaPerTimePressureUnit : public CCompoundUnit {
public:
  AreaPerTimePressureUnit(const char* u);
  AreaPerTimePressureUnit(const std::string& u);

  virtual ~AreaPerTimePressureUnit(); 

  bool operator==(const AreaPerTimePressureUnit&) const;
  bool operator!=(const AreaPerTimePressureUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const AreaPerTimePressureUnit& GetCompoundUnit(const char* unit);
  static const AreaPerTimePressureUnit& GetCompoundUnit(const std::string& unit);

  static const AreaPerTimePressureUnit m2_Per_s_mmHg;
  static const AreaPerTimePressureUnit cm2_Per_s_mmHg;
  static const AreaPerTimePressureUnit m2_Per_min_mmHg;
  static const AreaPerTimePressureUnit cm2_Per_min_mmHg;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<AreaPerTimePressureUnit>;

class BIOGEARS_API SEScalarAreaPerTimePressure : public SEScalarQuantity<AreaPerTimePressureUnit> {
public:
  SEScalarAreaPerTimePressure();
  virtual ~SEScalarAreaPerTimePressure();

  CDM::ScalarAreaPerTimePressureData* Unload() const override;   

  using SEScalarQuantity<AreaPerTimePressureUnit>::SetValue;
  using SEScalarQuantity<AreaPerTimePressureUnit>::GetValue;

  bool operator==(const SEScalarAreaPerTimePressure&) const;
  bool operator!=(const SEScalarAreaPerTimePressure&) const;
};
}