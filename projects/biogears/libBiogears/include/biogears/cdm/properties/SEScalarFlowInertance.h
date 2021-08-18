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
class BIOGEARS_API FlowInertanceUnit : public CCompoundUnit {
public:
  FlowInertanceUnit(const char* u);
  FlowInertanceUnit(const std::string& u);

  virtual ~FlowInertanceUnit();

  bool operator==(const FlowInertanceUnit&) const;
  bool operator!=(const FlowInertanceUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const FlowInertanceUnit& GetCompoundUnit(const char* unit);
  static const FlowInertanceUnit& GetCompoundUnit(const std::string& unit);

  static const FlowInertanceUnit mmHg_s2_Per_mL;
  static const FlowInertanceUnit mmHg_s2_Per_L;
  static const FlowInertanceUnit cmH2O_s2_Per_mL;
  static const FlowInertanceUnit cmH2O_s2_Per_L;
  static const FlowInertanceUnit Pa_s2_Per_m3;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<FlowInertanceUnit>;

class BIOGEARS_API SEScalarFlowInertance : public SEScalarQuantity<FlowInertanceUnit> {
public:
  SEScalarFlowInertance();
  virtual ~SEScalarFlowInertance();

  CDM::ScalarFlowInertanceData* Unload() const override;

  using SEScalarQuantity<FlowInertanceUnit>::SetValue;
  using SEScalarQuantity<FlowInertanceUnit>::GetValue;

  bool operator==(const SEScalarFlowInertance&) const;
  bool operator!=(const SEScalarFlowInertance&) const;
};
}