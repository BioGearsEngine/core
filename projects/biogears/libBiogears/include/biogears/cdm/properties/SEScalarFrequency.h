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
class BIOGEARS_API FrequencyUnit : public CCompoundUnit {
public:
  FrequencyUnit(const char* u);
  FrequencyUnit(const std::string& u);

  virtual ~FrequencyUnit();

  bool operator==(const FrequencyUnit&) const;
  bool operator!=(const FrequencyUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const FrequencyUnit& GetCompoundUnit(const char* unit);
  static const FrequencyUnit& GetCompoundUnit(const std::string& unit);

  static const FrequencyUnit Per_min;
  static const FrequencyUnit Per_s;
  static const FrequencyUnit Hz;
  static const FrequencyUnit Per_hr;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<FrequencyUnit>;

class BIOGEARS_API SEScalarFrequency : public SEScalarQuantity<FrequencyUnit> {
public:
  SEScalarFrequency();
  virtual ~SEScalarFrequency();

  CDM::ScalarFrequencyData* Unload() const override;

  using SEScalarQuantity<FrequencyUnit>::SetValue;
  using SEScalarQuantity<FrequencyUnit>::GetValue;

  bool operator==(const SEScalarFrequency&) const;
  bool operator!=(const SEScalarFrequency&) const;
};
}