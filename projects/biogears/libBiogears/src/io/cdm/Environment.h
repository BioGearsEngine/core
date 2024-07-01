
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

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/enums/SEEnvironmentEnums.h>
#include <biogears/exports.h>

#include <biogears/schema/cdm/Environment.hxx>

#define CDM_ENVIRONMENT_MARSHALL_HELPER(in, out, func)                               \
  if (in.m_##func) {                                                                 \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::Environment::Marshall(*in.m_##func, out.func());                             \
  }

#define CDM_OPTIONAL_ENVIRONMENT_MARSHALL_HELPER(in, out, func) \
  if (in.m_##func) {                                            \
    io::Environment::Marshall(*in.m_##func, out.func());        \
  }

#define SE_ENVIRONMENT_ENUM_MARSHALL_HELPER(in, out, func)                           \
  if (in.Has##func()) {                                                              \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::Environment::Marshall(in.m_##func, out.func());                              \
  }

#define SE_OPTIONAL_ENVIRONMENT_ENUM_MARSHALL_HELPER(in, out, func) \
  io::Environment::Marshall(in.m_##func, out.func());

namespace biogears {
class SEEnvironment;
class SEEnvironmentalConditions;
class SEActiveHeating;
class SEActiveCooling;
class SEAppliedTemperature;

namespace io {
  class BIOGEARS_PRIVATE_API Environment {
  public:
    // template <typename SE, typename XSD>  option
    template <typename SE, typename XSD, std::enable_if_t<std::is_enum<SE>::value>* = nullptr>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD, std::enable_if_t<!std::is_enum<SE>::value>* = nullptr>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);

    // class SEEnvironmentalConditions
    static void UnMarshall(const CDM::EnvironmentalConditionsData& in, SEEnvironmentalConditions& out);
    static void Marshall(const SEEnvironmentalConditions& in, CDM::EnvironmentalConditionsData& out);
    // class SEActiveHeating
    static void UnMarshall(const CDM::ActiveHeatingData& in, SEActiveHeating& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEActiveHeating& in, CDM::ActiveHeatingData& out);
    // class SEActiveCooling
    static void UnMarshall(const CDM::ActiveCoolingData& in, SEActiveCooling& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEActiveCooling& in, CDM::ActiveCoolingData& out);
    // class SEAppliedTemperature
    static void UnMarshall(const CDM::AppliedTemperatureData& in, SEAppliedTemperature& out);
    static void Marshall(const SEAppliedTemperature& in, CDM::AppliedTemperatureData& out);
    // class SEEnvironment
    static void UnMarshall(const CDM::EnvironmentData& in, SEEnvironment& out);
    static void Marshall(const SEEnvironment& in, CDM::EnvironmentData& out);

    // class SESurroundingType;
    static void UnMarshall(const CDM::enumSurroundingType& in, SESurroundingType& out);
    static void Marshall(const SESurroundingType& in, CDM::enumSurroundingType& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD, std::enable_if_t<std::is_enum<SE>::value>*>
  void Environment::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present() || option_in->empty()) {
      out = SE::Invalid;
    } else {
      io::Environment::UnMarshall(option_in.get(), out);
    }
  }

  template <typename SE, typename XSD, std::enable_if_t<!std::is_enum<SE>::value>*>
  void Environment::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      UnMarshall(option_in.get(), out);
    }
  }

  template <typename SE, typename XSD>
  void Environment::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    io::Environment::Marshall(in, *item);
    option_out.set(std::move(item));
  }
} // Namespace IO

// Operators
bool operator==(CDM::enumSurroundingType const& lhs, SESurroundingType const& rhs);

inline bool operator==(SESurroundingType const& lhs, CDM::enumSurroundingType const& rhs)
{
  return rhs == lhs;
}

inline bool operator!=(CDM::enumSurroundingType const& lhs, SESurroundingType const& rhs)
{
  return !(lhs == rhs);
}

inline bool operator!=(SESurroundingType const& lhs, CDM::enumSurroundingType const& rhs)
{
  return !(rhs == lhs);
}

} // Namespace Biogears
