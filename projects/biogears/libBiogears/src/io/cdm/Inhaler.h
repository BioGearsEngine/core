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

#include <biogears/schema/cdm/Inhaler.hxx>

#define CDM_INHALER_PTR_MARSHALL_HELPER(in, out, func)                                  \
  if (in.m_##func) {                                                                 \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::Inhaler::Marshall(*in.m_##func, out.func());                               \
  }

#define CDM_OPTIONAL_INHALER_PTR_MARSHALL_HELPER(in, out, func) \
  if (in.m_##func) {                                         \
    io::Inhaler::Marshall(*in.m_##func, out.func());       \
  }
namespace biogears {
class SEInhaler;

namespace io {
  class BIOGEARS_PRIVATE_API Inhaler {

  public:
    //template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    //class SEInhaler
    static void UnMarshall(const CDM::InhalerData& in, SEInhaler& out);
    static void Marshall(const SEInhaler& in, CDM::InhalerData& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Inhaler::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      UnMarshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Inhaler::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    Marshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO
} //Namespace Biogears
