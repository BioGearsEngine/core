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

#include <biogears/schema/cdm/SubstanceQuantity.hxx>

#define CDM_SUBSTANCE_QUANTITY_UNMARSHAL_HELPER(in, out, func)                       \
  if (in.m_##func) {                                                                 \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::SubstanceQuantity::UnMarshall(*in.m_##func, out.func());                     \
  }

#define CDM_OPTIONAL_SUBSTANCE_QUANTITY_UNMARSHAL_HELPER(in, out, func) \
  if (in.m_##func) {                                                    \
    io::SubstanceQuantity::UnMarshall(*in.m_##func, out.func());        \
  }

namespace biogears {

class SESubstanceQuantity;
class SEGasSubstanceQuantity;
class SELiquidSubstanceQuantity;

namespace io {
  class BIOGEARS_PRIVATE_API SubstanceQuantity {
  public:
    //template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    //class SESubstanceQuantity
    static void Marshall(const CDM::SubstanceQuantityData& in, SESubstanceQuantity& out);
    static void UnMarshall(const SESubstanceQuantity& in, CDM::SubstanceQuantityData& out);
    //class SEGasSubstanceQuantity
    static void Marshall(const CDM::GasSubstanceQuantityData& in, SEGasSubstanceQuantity& out);
    static void UnMarshall(SEGasSubstanceQuantity& in, CDM::GasSubstanceQuantityData& out);
    //class SELiquidSubstanceQuantity
    static void Marshall(const CDM::LiquidSubstanceQuantityData& in, SELiquidSubstanceQuantity& out);
    static void UnMarshall(SELiquidSubstanceQuantity& in, CDM::LiquidSubstanceQuantityData& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void SubstanceQuantity::Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Invalidate();
    } else {
      Marshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void SubstanceQuantity::UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    UnMarshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO
} //Namespace Biogears
