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
#include <biogears/exports.h>

#include <memory>
#include <random>

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/schema/cdm/EnvironmentConditions.hxx>

#define CDM_ENVIRONMENT_CONDITIONS_PTR_MARSHALL_HELPER(in, out, func)                    \
  if (in.m_##func) {                                                                 \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::EnvironmentConditions::Marshall(*in.m_##func, out.func());                   \
  }

#define CDM_OPTIONAL_ENVIRONMENT_CONDITIONS_PTR_MARSHALL_HELPER(in, out, func) \
  if (in.m_##func) {                                                       \
    io::EnvironmentConditions::Marshall(*in.m_##func, out.func());         \
  }

namespace biogears {
class SECondition;
class SESubstanceManager;
class SEEnvironmentCondition;
class SEInitialEnvironment;

namespace io {
  class BIOGEARS_PRIVATE_API EnvironmentConditions {
  public:
    // class Factories;
    static std::unique_ptr<SECondition> factory(CDM::EnvironmentConditionData const* environmentConditionData, SESubstanceManager& substances, std::default_random_engine* rd = nullptr);
    static std::unique_ptr<CDM::EnvironmentConditionData> factory(const SEEnvironmentCondition* data);

    // template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    // class SEEnvironmentCondition
    static void UnMarshall(const CDM::EnvironmentConditionData& in, SEEnvironmentCondition& out);
    static void Marshall(const SEEnvironmentCondition& in, CDM::EnvironmentConditionData& out);
    // class SEInitialEnvironment
    static void UnMarshall(const CDM::InitialEnvironmentData& in, SEInitialEnvironment& out);
    static void Marshall(const SEInitialEnvironment& in, CDM::InitialEnvironmentData& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void EnvironmentConditions::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      UnMarshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void EnvironmentConditions::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    Marshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO
} // Namespace Biogears
