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
#include <random>

#include "biogears/cdm/CommonDataModel.h"
#include <biogears/exports.h>

#include <biogears/schema/cdm/InhalerActions.hxx>

#define CDM_INHALER_ACTIONS_PTR_MARSHALL_HELPER(in, out, func)                           \
  if (in.m_##func) {                                                                 \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::InhalerActions::Marshall(*in.m_##func, out.func());                          \
  }

#define CDM_OPTIONAL_INHALER_ACTIONS_PTR_MARSHALL_HELPER(in, out, func) \
  if (in.m_##func) {                                                \
    io::InhalerActions::Marshall(*in.m_##func, out.func());         \
  }

namespace biogears {
class SEAction;
class SEInhalerAction;
class SEInhalerActionCollection;
class SEInhalerConfiguration;
class SESubstanceManager;

namespace io {
  class BIOGEARS_PRIVATE_API InhalerActions {
  public:
    // Factories
    static std::unique_ptr<CDM::InhalerActionData> factory(const SEInhalerAction*);
    static std::unique_ptr<SEAction> factory(CDM::InhalerActionData const* inhalerActionData, SESubstanceManager& substances, std::default_random_engine* rd = nullptr);

    static void Marshall(const SEInhalerActionCollection& in, std::vector<std::unique_ptr<CDM::ActionData>>& out);
    // template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    // class SEInhalerAction
    static void UnMarshall(const CDM::InhalerActionData& in, SEInhalerAction& out);
    static void Marshall(const SEInhalerAction& in, CDM::InhalerActionData& out);
    // class SEInhalerConfiguration
    static void UnMarshall(const CDM::InhalerConfigurationData& in, SEInhalerConfiguration& out);
    static void Marshall(const SEInhalerConfiguration& in, CDM::InhalerConfigurationData& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void InhalerActions::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Invalidate();
    } else {
      UnMarshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void InhalerActions::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    Marshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO
} // Namespace Biogears
