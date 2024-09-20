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
class BIOGEARS_API FlowElastanceUnit : public CCompoundUnit {
public:
  FlowElastanceUnit(const char* u);
  FlowElastanceUnit(const std::string& u);

  virtual ~FlowElastanceUnit();

  bool operator==(const FlowElastanceUnit&) const;
  bool operator!=(const FlowElastanceUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const FlowElastanceUnit& GetCompoundUnit(const char* unit);
  static const FlowElastanceUnit& GetCompoundUnit(const std::string& unit);

  static const FlowElastanceUnit cmH2O_Per_L;
  static const FlowElastanceUnit mmHg_Per_mL;
  static const FlowElastanceUnit Pa_Per_m3;
};

#pragma warning(disable : 4661)
extern template class SEScalarQuantity<FlowElastanceUnit>;

class BIOGEARS_API SEScalarFlowElastance : public SEScalarQuantity<FlowElastanceUnit> {
public:
  SEScalarFlowElastance();
  virtual ~SEScalarFlowElastance();

  using SEScalarQuantity<FlowElastanceUnit>::SetValue;
  using SEScalarQuantity<FlowElastanceUnit>::GetValue;
};
#pragma warning(default : 4661)
}