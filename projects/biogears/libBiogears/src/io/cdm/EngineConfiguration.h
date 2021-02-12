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
#include <memory>

#include "biogears/cdm/CommonDataModel.h"
#include <biogears/exports.h>
#include <biogears/schema/cdm/EngineConfiguration.hxx>
#include <type_traits>

namespace biogears {
class PhysiologyEngineConfiguration;
class PhysiologyEngineStabilization;
class PhysiologyEngineTimedStabilization;
class PhysiologyEngineTimedConditionStabilization;
class PhysiologyEngineDynamicStabilization;
class PhysiologyEngineDynamicStabilizationCriteria;
class PhysiologyEngineTimedStabilizationCriteria;

#define CDM_ENGINE_CONFIGURATION_UNMARSHAL_HELPER(xsd, func)                         \
  if (m_##func) {                                                                    \
    xsd.func(std::make_unique<std::remove_reference<decltype(xsd.func())>::type>()); \
    io::Property::UnMarshall(*m_##func, xsd.func());                                 \
  }
namespace io {
  class BIOGEARS_PRIVATE_API EngineConfiguration {
  public:
    //template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    //class PhysiologyEngineConfiguration
    static void Marshall(const CDM::PhysiologyEngineConfigurationData& in, PhysiologyEngineConfiguration& out);
    static void UnMarshall(const PhysiologyEngineConfiguration& in, CDM::PhysiologyEngineConfigurationData& out);
    //class PhysiologyEngineStabilization
    static void Marshall(const CDM::PhysiologyEngineStabilizationData& in, PhysiologyEngineStabilization& out);
    static void UnMarshall(const PhysiologyEngineStabilization& in, CDM::PhysiologyEngineStabilizationData& out);
    //class PhysiologyEngineTimedStabilization
    static void Marshall(const CDM::PhysiologyEngineTimedStabilizationData& in, PhysiologyEngineTimedStabilization& out);
    static void UnMarshall(const PhysiologyEngineTimedStabilization& in, CDM::PhysiologyEngineTimedStabilizationData& out);
    //class PhysiologyEngineTimedConditionStabilization
    static void Marshall(const CDM::PhysiologyEngineTimedConditionStabilizationData& in, PhysiologyEngineTimedStabilizationCriteria& out);
    static void UnMarshall(const PhysiologyEngineTimedStabilizationCriteria& in, CDM::PhysiologyEngineTimedConditionStabilizationData& out);
    //class PhysiologyEngineDynamicStabilization
    static void Marshall(const CDM::PhysiologyEngineDynamicStabilizationData& in, PhysiologyEngineDynamicStabilization& out);
    static void UnMarshall(const PhysiologyEngineDynamicStabilization& in, CDM::PhysiologyEngineDynamicStabilizationData& out);
    //class PhysiologyEngineDynamicStabilizationCriteria
    static void Marshall(const CDM::PhysiologyEngineDynamicStabilizationCriteriaData& in, PhysiologyEngineDynamicStabilizationCriteria& out);
    static void UnMarshall(const PhysiologyEngineDynamicStabilizationCriteria& in, CDM::PhysiologyEngineDynamicStabilizationCriteriaData& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void EngineConfiguration::Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      Marshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void EngineConfiguration::UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    UnMarshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO
} //Namespace Biogears
