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
class BIOGEARS_API PressureUnit : public CCompoundUnit {
public:
  PressureUnit(const char* u);
  PressureUnit(const std::string& u);

  virtual ~PressureUnit();

  bool operator==(const PressureUnit&) const;
  bool operator!=(const PressureUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const PressureUnit& GetCompoundUnit(const char* unit);
  static const PressureUnit& GetCompoundUnit(const std::string& unit);

  static const PressureUnit Pa;
  static const PressureUnit mmHg;
  static const PressureUnit cmH2O;
  static const PressureUnit psi;
  static const PressureUnit atm;
};

#pragma warning(disable : 4661)
extern template class SEScalarQuantity<PressureUnit>;

class BIOGEARS_API SEScalarPressure : public SEScalarQuantity<PressureUnit> {
public:
  SEScalarPressure();
  virtual ~SEScalarPressure();

  using SEScalarQuantity<PressureUnit>::SetValue;
  using SEScalarQuantity<PressureUnit>::GetValue;
};
#pragma warning(default : 4661)
}