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
class BIOGEARS_API LengthPerTimePressureUnit : public CCompoundUnit {
public:
  LengthPerTimePressureUnit(const char* u);
  LengthPerTimePressureUnit(const std::string& u);

  virtual ~LengthPerTimePressureUnit();    

  bool operator==(const LengthPerTimePressureUnit&) const;
  bool operator!=(const LengthPerTimePressureUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const LengthPerTimePressureUnit& GetCompoundUnit(const char* unit);
  static const LengthPerTimePressureUnit& GetCompoundUnit(const std::string& unit);

  static const LengthPerTimePressureUnit m_Per_s_mmHg;
  static const LengthPerTimePressureUnit cm_Per_s_mmHg;
  static const LengthPerTimePressureUnit m_Per_min_mmHg;
  static const LengthPerTimePressureUnit cm_Per_min_mmHg;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<LengthPerTimePressureUnit>;

class BIOGEARS_API SEScalarLengthPerTimePressure : public SEScalarQuantity<LengthPerTimePressureUnit> {
public:
  SEScalarLengthPerTimePressure();
  virtual ~SEScalarLengthPerTimePressure();

  CDM::ScalarLengthPerTimePressureData* Unload() const override;

  using SEScalarQuantity<LengthPerTimePressureUnit>::SetValue;
  using SEScalarQuantity<LengthPerTimePressureUnit>::GetValue;

  bool operator==(const SEScalarLengthPerTimePressure&) const;
  bool operator!=(const SEScalarLengthPerTimePressure&) const;
};
}