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
class BIOGEARS_API FlowComplianceUnit : public CCompoundUnit {
public:
  FlowComplianceUnit(const char* u);
  FlowComplianceUnit(const std::string& u);

  virtual ~FlowComplianceUnit();

  bool operator==(const FlowComplianceUnit&) const;
  bool operator!=(const FlowComplianceUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const FlowComplianceUnit& GetCompoundUnit(const char* unit);
  static const FlowComplianceUnit& GetCompoundUnit(const std::string& unit);

  static const FlowComplianceUnit L_Per_cmH2O;
  static const FlowComplianceUnit mL_Per_mmHg;
  static const FlowComplianceUnit mL_Per_cmH2O;
  static const FlowComplianceUnit m3_Per_Pa;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<FlowComplianceUnit>;

class BIOGEARS_API SEScalarFlowCompliance : public SEScalarQuantity<FlowComplianceUnit> {
public:
  SEScalarFlowCompliance();
  virtual ~SEScalarFlowCompliance();

  CDM::ScalarFlowComplianceData* Unload() const override;

  using SEScalarQuantity<FlowComplianceUnit>::SetValue;
  using SEScalarQuantity<FlowComplianceUnit>::GetValue;

  bool operator==(const SEScalarFlowCompliance&) const;
  bool operator!=(const SEScalarFlowCompliance&) const;
};
}