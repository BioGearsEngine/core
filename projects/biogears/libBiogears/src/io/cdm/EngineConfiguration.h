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

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/exports.h>
#include <biogears/schema/cdm/EngineConfiguration.hxx>
#include <type_traits>

#define CDM_BIOGEARS_CONFIGURATION_MARSHALL_HELPER(in, out, func)                   \
  if (in.m_##func) {                                                                 \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::EngineConfiguration::Marshall(*in.m_##func, out.func());                   \
  }

#define CDM_OPTIONAL_BIOGEARS_CONFIGURATION_MARSHALL_HELPER(in, out, func) \
  if (in.m_##func) {                                                        \
    io::EngineConfiguration::Marshall(*in.m_##func, out.func());          \
  }

namespace biogears {
class PhysiologyEngineConfiguration;
class PhysiologyEngineStabilization;
class PhysiologyEngineTimedStabilization;
class PhysiologyEngineTimedConditionStabilization;
class PhysiologyEngineDynamicStabilization;
class PhysiologyEngineDynamicStabilizationCriteria;
class PhysiologyEngineTimedStabilizationCriteria;

namespace io {
  class BIOGEARS_PRIVATE_API EngineConfiguration {
  public:
    //template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    //class PhysiologyEngineConfiguration
    static void UnMarshall(const CDM::PhysiologyEngineConfigurationData& in, PhysiologyEngineConfiguration& out);
    static void Marshall(const PhysiologyEngineConfiguration& in, CDM::PhysiologyEngineConfigurationData& out);
    //class PhysiologyEngineStabilization
    static void UnMarshall(const CDM::PhysiologyEngineStabilizationData& in, PhysiologyEngineStabilization& out);
    static void Marshall(const PhysiologyEngineStabilization& in, CDM::PhysiologyEngineStabilizationData& out);
    //class PhysiologyEngineTimedStabilization
    static void UnMarshall(const CDM::PhysiologyEngineTimedStabilizationData& in, PhysiologyEngineTimedStabilization& out);
    static void Marshall(const PhysiologyEngineTimedStabilization& in, CDM::PhysiologyEngineTimedStabilizationData& out);
    //class PhysiologyEngineTimedConditionStabilization
    static void UnMarshall(const CDM::PhysiologyEngineTimedConditionStabilizationData& in, PhysiologyEngineTimedStabilizationCriteria& out);
    static void Marshall(const PhysiologyEngineTimedStabilizationCriteria& in, CDM::PhysiologyEngineTimedConditionStabilizationData& out);
    //class PhysiologyEngineDynamicStabilization
    static void UnMarshall(const CDM::PhysiologyEngineDynamicStabilizationData& in, PhysiologyEngineDynamicStabilization& out);
    static void Marshall(const PhysiologyEngineDynamicStabilization& in, CDM::PhysiologyEngineDynamicStabilizationData& out);
    //class PhysiologyEngineDynamicStabilizationCriteria
    static void UnMarshall(const CDM::PhysiologyEngineDynamicStabilizationCriteriaData& in, PhysiologyEngineDynamicStabilizationCriteria& out);
    static void Marshall(const PhysiologyEngineDynamicStabilizationCriteria& in, CDM::PhysiologyEngineDynamicStabilizationCriteriaData& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void EngineConfiguration::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      UnMarshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void EngineConfiguration::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    Marshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO
} //Namespace Biogears
