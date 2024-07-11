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
#include <type_traits>

#include "Actions.h"

#include <biogears/cdm/enums/SEActionEnums.h>
#include <biogears/schema/cdm/Actions.hxx>

// Question: To Serialize Invalid units or not to Serialize?
//           TO Throw an exception when a member is invalid?
#define CDM_ACTIONS_MARSHALL_HELPER(in, out, func)                                           \
  if (in.m_##func && in.m_##func->IsValid()) {                                               \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>());         \
    io::Actions::Marshall(*in.m_##func, out.func());                                         \
  } /* else if (in.m_##func) {                                                               \
     throw biogears::CommonDataModelException("func is InValid and cannot be Unmarshalled"); \
   }*/

#define CDM_OPTIONAL_ACTIONS_MARSHALL_HELPER(in, out, func)                                 \
  if (in.m_##func && in.m_##func->IsValid()) {                                              \
    io::Actions::Marshall(*in.m_##func, out.func());                                        \
  } /*else if (in.m_##func) {                                                               \
    throw biogears::CommonDataModelException("func is InValid and cannot be Unmarshalled"); \
  }*/

#define SE_ACTIONS_ENUM_MARSHALL_HELPER(in, out, func)                               \
  if (in.Has##func()) {                                                              \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::Actions::Marshall(in.m_##func, out.func());                                  \
  }

#define SE_OPTIONAL_ACTIONS_ENUM_MARSHALL_HELPER(in, out, func) \
  io::Actions::Marshall(in.m_##func, out.func());

namespace biogears {

class SEAction;
class SEActionList;
class SEAdvanceTime;
//enum class SESerializationType;
class SESerializeState;
//class SERandomSeed;

namespace io {
  class BIOGEARS_PRIVATE_API Actions {
  public:
    // template <typename SE, typename XSD>  option
    template <typename SE, typename XSD, std::enable_if_t<std::is_enum<SE>::value>* = nullptr>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD, std::enable_if_t<!std::is_enum<SE>::value>* = nullptr>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out, std::default_random_engine* rd = nullptr);
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);

    // class SEAction;
    static void UnMarshall(const CDM::ActionData& in, SEAction& out);
    static void Marshall(const SEAction& in, CDM::ActionData& out);
    //// class SEActionList;
    //static void UnMarshall(const CDM::ActionListData& in, SEActionList& out, std::default_random_engine* rd = nullptr);
    //static void Marshall(const SEActionList& in, CDM::ActionListData& out);
    // class SEAdvanceTime;
    static void UnMarshall(const CDM::AdvanceTimeData& in, SEAdvanceTime& out, std::default_random_engine* re = nullptr);
    static void Marshall(const SEAdvanceTime& in, CDM::AdvanceTimeData& out);
    // class SESerializationType;
    static void UnMarshall(const CDM::enumSerializationType& in, SESerializationType& out);
    static void Marshall(const SESerializationType& in, CDM::enumSerializationType& out);
    // class SESerializeState;
    static void UnMarshall(const CDM::SerializeStateData& in, SESerializeState& out);
    static void Marshall(const SESerializeState& in, CDM::SerializeStateData& out);
    // class SERandomSeed
    //static void UnMarshall(const CDM::RandomSeed& in, SERandomSeed& out, std::default_random_engine* rd = nullptr);
    //static void Marshall(const SERandomSeed& in, CDM::RandomSeed& out);
  };

  //----------------------------------------------------------------------------------

  template <typename SE, typename XSD, std::enable_if_t<std::is_enum<SE>::value>*>
  void Actions::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out = SE::Invalid;
    } else {
      UnMarshall(option_in.get(), out);
    }
  }

  template <typename SE, typename XSD, std::enable_if_t<!std::is_enum<SE>::value>*>
  void Actions::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out, std::default_random_engine* rd)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      io::Actions::UnMarshall(option_in.get(), out, rd);
    }
  }

  template <typename SE, typename XSD>
  void Actions::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    io::Actions::Marshall(in, *item);
    option_out.set(std::move(item));
  }
} // namespace io

// Operators
bool operator==(CDM::enumSerializationType const& lhs, SESerializationType const& rhs);

inline bool operator==(SESerializationType const& lhs, CDM::enumSerializationType const& rhs)
{
  return rhs == lhs;
}

inline bool operator!=(CDM::enumSerializationType const& lhs, SESerializationType const& rhs)
{
  return !(lhs == rhs);
}

inline bool operator!=(SESerializationType const& lhs, CDM::enumSerializationType const& rhs)
{
  return !(rhs == lhs);
}

} // namespace biogears
