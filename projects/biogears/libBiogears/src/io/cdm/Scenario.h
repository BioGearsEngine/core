/**************************************************************************************
Copyright 2019 Applied Research Associates, Inc.
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

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/exports.h>

#include "Actions.h"
#include "Conditions.h"

#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/schema/cdm/Scenario.hxx>

#define CDM_SCENARIO_MARSHALL_HELPER(in, out, func)                                  \
  if (in.m_##func) {                                                                 \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::Scenario::Marshall(*in.m_##func, out.func());                                \
  }

#define CDM_OPTIONAL_SCENARIO_MARSHALL_HELPER(in, out, func) \
  if (in.m_##func) {                                         \
    io::Scenario::Marshall(*in.m_##func, out.func());        \
  }
namespace biogears {

class SEScenario;
class SEScenarioInitialParameters;
class SEScenarioAutoSerialization;


namespace io {
  class BIOGEARS_PRIVATE_API Scenario {
  public:

    // template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);

     // class SEScenario;
    static void UnMarshall(const CDM::ScenarioData& in, SEScenario& out);
    static void Marshall(const SEScenario& in, CDM::ScenarioData& out);
    static void Marshall(const SEScenario& in, CDM::ActionListData& out);
    // class SEScenarioInitialParameters;
    static void UnMarshall(const CDM::ScenarioInitialParametersData& in, SEScenarioInitialParameters& out);
    static void Marshall(const SEScenarioInitialParameters& in, CDM::ScenarioInitialParametersData& out);
    // class SEScenarioAutoSerialization;
    static void UnMarshall(const CDM::ScenarioAutoSerializationData& in, SEScenarioAutoSerialization& out);
    static void Marshall(const SEScenarioAutoSerialization& in, CDM::ScenarioAutoSerializationData& out);
    
    static std::unique_ptr<CDM::ActionData> factory(const SEAction* in);
  };

  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Scenario::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      io::Scenario::UnMarshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Scenario::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    io::Scenario::Marshall(in, *item);
    option_out.set(*item);
  }
}
}
