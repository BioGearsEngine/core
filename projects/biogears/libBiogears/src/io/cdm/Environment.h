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

#include <biogears/schema/cdm/Environment.hxx>

namespace biogears {
class SEEnvironmentalConditions;
class SEActiveHeating;
class SEActiveCooling;
class SEAppliedTemperature;
class SEEnvironment;

#define CDM_ENVIRONMENT_UNMARSHAL_HELPER(xsd, func)                           \
  if (m_##func) {                                                                    \
    xsd.func(std::make_unique<std::remove_reference<decltype(xsd.func())>::type>()); \
    io::Property::UnMarshall(*m_##func, xsd.func());                       \
  }
namespace io {
  class BIOGEARS_PRIVATE_API Environment {
  public:
    //template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    //class SEEnvironmentalConditions
    static void Marshall(const CDM::EnvironmentalConditionsData& in, SEEnvironmentalConditions& out);
    static void UnMarshall(const SEEnvironmentalConditions& in, CDM::EnvironmentalConditionsData& out);
    //class SEActiveHeating
    static void Marshall(const CDM::ActiveHeatingData& in, SEActiveHeating& out);
    static void UnMarshall(const SEActiveHeating& in, CDM::ActiveHeatingData& out);
    //class SEActiveCooling
    static void Marshall(const CDM::ActiveCoolingData& in, SEActiveCooling& out);
    static void UnMarshall(const SEActiveCooling& in, CDM::ActiveCoolingData& out);
    //class SEAppliedTemperature
    static void Marshall(const CDM::AppliedTemperatureData& in, SEAppliedTemperature& out);
    static void UnMarshall(const SEAppliedTemperature& in, CDM::AppliedTemperatureData& out);
    //class SEEnvironment
    static void Marshall(const CDM::EnvironmentData& in, SEEnvironment& out);
    static void UnMarshall(const SEEnvironment& in, CDM::EnvironmentData& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Environment::Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      Marshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Environment::UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    UnMarshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO
} //Namespace Biogears
