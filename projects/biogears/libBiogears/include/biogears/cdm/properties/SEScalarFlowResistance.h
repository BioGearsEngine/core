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
class BIOGEARS_API FlowResistanceUnit : public CCompoundUnit {
public:
  FlowResistanceUnit(const char* u);
  FlowResistanceUnit(const std::string& u);

  virtual ~FlowResistanceUnit();

  bool operator==(const FlowResistanceUnit&) const;
  bool operator!=(const FlowResistanceUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const FlowResistanceUnit& GetCompoundUnit(const char* unit);
  static const FlowResistanceUnit& GetCompoundUnit(const std::string& unit);

  static const FlowResistanceUnit cmH2O_s_Per_L;
  static const FlowResistanceUnit mmHg_s_Per_mL;
  static const FlowResistanceUnit mmHg_min_Per_mL;
  static const FlowResistanceUnit mmHg_min_Per_L;
  static const FlowResistanceUnit Pa_s_Per_m3;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<FlowResistanceUnit>;

class BIOGEARS_API SEScalarFlowResistance : public SEScalarQuantity<FlowResistanceUnit> {
public:
  SEScalarFlowResistance();
  virtual ~SEScalarFlowResistance();

  CDM::ScalarFlowResistanceData* Unload() const override;

  using SEScalarQuantity<FlowResistanceUnit>::SetValue;
  using SEScalarQuantity<FlowResistanceUnit>::GetValue;

  bool operator==(const SEScalarFlowResistance&) const;
  bool operator!=(const SEScalarFlowResistance&) const;
};
}