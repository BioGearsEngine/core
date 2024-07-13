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

#include <biogears/schema/cdm/EnvironmentActions.hxx>

#include <random>

namespace biogears {
class SEEnvironmentAction;
class SEEnvironmentChange;
class SEThermalApplication;
class SEAction;

#define CDM_ENVIRONMENT_ACTIONS_MARSHALL_HELPER(in, out, func)                              \
  if (in.m_##func) {                                                                 \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::EnvironmentActions::Marshall(*in.m_##func, out.func());                            \
  }

#define CDM_OPTIONAL_ENVIRONMENT_ACTIONS_MARSHALL_HELPER(in, out, func) \
  if (in.m_##func) {                                             \
    io::EnvironmentActions::Marshall(*in.m_##func, out.func());        \
  }

namespace io {
  class BIOGEARS_PRIVATE_API EnvironmentActions {
  public:
    //template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    //class SEEnvironmentAction
    static void UnMarshall(const CDM::EnvironmentActionData& in, SEEnvironmentAction& out);
    static void Marshall(const SEEnvironmentAction& in, CDM::EnvironmentActionData& out);
    //class SEEnvironmentChange
    static void UnMarshall(const CDM::EnvironmentChangeData& in, SEEnvironmentChange& out);
    static void Marshall(const SEEnvironmentChange& in, CDM::EnvironmentChangeData& out);
    //class SEThermalApplication
    static void UnMarshall(const CDM::ThermalApplicationData& in, SEThermalApplication& out);
    static void Marshall(const SEThermalApplication& in, CDM::ThermalApplicationData& out);
    // Factories
    static std::unique_ptr<CDM::EnvironmentActionData> factory(const SEEnvironmentAction*);


    //-----------------------------------------------------------------------------
    static void Copy(const SEEnvironmentAction& in, SEEnvironmentAction& out);
    static void Copy(const SEEnvironmentChange& in, SEEnvironmentChange& out);
    static void Copy(const SEThermalApplication& in, SEThermalApplication& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void EnvironmentActions::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Invalidate();
    } else {
      UnMarshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void EnvironmentActions::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    Marshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO
} //Namespace Biogears
