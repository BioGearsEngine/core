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

#include <biogears/schema/cdm/ElectroCardioGram.hxx>

namespace biogears {
class SEElectroCardioGram;
class SEElectroCardioGramInterpolatorWaveform;
class SEElectroCardioGramInterpolator;

#define CDM_ELECTRO_CARDIO_GRAM_UNMARSHAL_HELPER(xsd, func)                                      \
  if (m_##func) {                                                                    \
    xsd.func(std::make_unique<std::remove_reference<decltype(xsd.func())>::type>()); \
    io::Property::UnMarshall(*m_##func, xsd.func());                       \
  }
namespace io {
  class BIOGEARS_PRIVATE_API ElectroCardioGram {
  public:
    //template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    //class SEElectroCardioGram
    static void Marshall(const CDM::ElectroCardioGramData& in, SEElectroCardioGram& out);
    static void UnMarshall(const SEElectroCardioGram& in, CDM::ElectroCardioGramData& out);
    //class SEElectroCardioGramInterpolationWaveform
    static void Marshall(const CDM::ElectroCardioGramInterpolationWaveformData& in, SEElectroCardioGramInterpolatorWaveform& out);
    static void UnMarshall(const SEElectroCardioGramInterpolatorWaveform& in, CDM::ElectroCardioGramInterpolationWaveformData& out);
    //class SEElectroCardioGramWaveformInterpolator
    static void Marshall(const CDM::ElectroCardioGramWaveformInterpolatorData& in, SEElectroCardioGramInterpolator& out);
    static void UnMarshall(const SEElectroCardioGramInterpolator& in, CDM::ElectroCardioGramWaveformInterpolatorData& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void ElectroCardioGram::Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Invalidate();
    } else {
      Marshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void ElectroCardioGram::UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    UnMarshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO
} //Namespace Biogears
