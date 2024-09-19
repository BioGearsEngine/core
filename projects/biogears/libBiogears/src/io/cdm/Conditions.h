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

#include <random>
#include <vector>
#include <memory>
#include <type_traits>

#include "Conditions.h"

#include <biogears/cdm/enums/SEActionEnums.h>
#include <biogears/schema/cdm/Conditions.hxx>

// Question: To Serialize Invalid units or not to Serialize?
//           TO Throw an exception when a member is invalid?
#define CDM_CONDITIONS_MARSHALL_HELPER(in, out, func)                                        \
  if (in.m_##func && in.m_##func->IsValid()) {                                               \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>());         \
    io::Conditions::Marshall(*in.m_##func, out.func());                                      \
  } /* else if (in.m_##func) {                                                               \
     throw biogears::CommonDataModelException("func is InValid and cannot be Unmarshalled"); \
   }*/

#define CDM_OPTIONAL_CONDITIONS_MARSHALL_HELPER(in, out, func)                              \
  if (in.m_##func && in.m_##func->IsValid()) {                                              \
    io::Conditions::Marshall(*in.m_##func, out.func());                                     \
  } /*else if (in.m_##func) {                                                               \
    throw biogears::CommonDataModelException("func is InValid and cannot be Unmarshalled"); \
  }*/

#define SE_CONDITIONS_ENUM_MARSHALL_HELPER(in, out, func)                            \
  if (in.Has##func()) {                                                              \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::Conditions::Marshall(in.m_##func, out.func());                               \
  }

#define SE_OPTIONAL_CONDITIONS_ENUM_MARSHALL_HELPER(in, out, func) \
  io::Conditions::Marshall(in.m_##func, out.func());

namespace biogears {

class SECondition;
class SEConditionList;
class SESubstanceManager;
class SEConditionManager;

namespace io {
  class BIOGEARS_PRIVATE_API Conditions {
  public:
    // class Factories;
    static std::vector<std::unique_ptr<SECondition>> condition_factory(const CDM::ConditionListData& in, SESubstanceManager& substances, std::default_random_engine* rd = nullptr);

    static std::unique_ptr<SECondition> factory(CDM::ConditionData const* conditionData, SESubstanceManager& substances, std::default_random_engine* rd = nullptr);
    static std::unique_ptr<CDM::ConditionData> factory(const SECondition* data);

    static void Marshall(SEConditionManager const& in, std::vector<std::unique_ptr<CDM::ConditionData>>& out);
    // template <typename SE, typename XSD>  option
    template <typename SE, typename XSD, std::enable_if_t<std::is_enum<SE>::value>* = nullptr>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD, std::enable_if_t<!std::is_enum<SE>::value>* = nullptr>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out, std::default_random_engine* rd = nullptr);
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);

    // class Condition;
    static void UnMarshall(const CDM::ConditionData& in, SECondition& out);
    static void Marshall(const SECondition& in, CDM::ConditionData& out);
    // class ConditionList
    // static void UnMarshall(const CDM::ConditionListData& in, SEConditionList& out, std::default_random_engine* re = nullptr);
    // static void Marshall(const SEConditionList& in, CDM::ConditionListData& out);
  };

  //----------------------------------------------------------------------------------

  template <typename SE, typename XSD, std::enable_if_t<std::is_enum<SE>::value>*>
  void Conditions::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present() || option_in->empty()) {
      out = SE::Invalid;
    } else {
      UnMarshall(option_in.get(), out);
    }
  }

  template <typename SE, typename XSD, std::enable_if_t<!std::is_enum<SE>::value>*>
  void Conditions::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out, std::default_random_engine* rd)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      io::Conditions::UnMarshall(option_in.get(), out, rd);
    }
  }

  template <typename SE, typename XSD>
  void Conditions::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    io::Conditions::Marshall(in, *item);
    option_out.set(std::move(item));
  }
} // namespace io
} // namespace biogears
