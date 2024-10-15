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

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/exports.h>

#include <biogears/schema/cdm/ElectroCardioGram.hxx>

#define CDM_ELECTRO_CARDIOGRAM_PTR_MARSHALL_HELPER(in, out, func)                        \
  if (in.m_##func) {                                                                 \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::ElectroCardioGram::Marshall(*in.m_##func, out.func());                       \
  }

#define CDM_OPTIONAL_ELECTRO_CARDIOGRAM_PTR_MARSHALL_HELPER(in, out, func) \
  if (in.m_##func) {                                                   \
    io::ElectroCardioGram::Marshall(*in.m_##func, out.func());         \
  }

#define CDM_ELECTRO_CARDIOGRAM_COPY(type, in, out) \
  {                                                           \
    CDM::##type##Data middle;                                 \
    io::ElectroCardioGram::Marshall(in, middle);    \
    io::ElectroCardioGram::UnMarshall(middle, out); \
  }

namespace biogears {
class SEElectroCardioGram;
class SEElectroCardioGramInterpolationWaveform;
class SEElectroCardioGramInterpolator;
class SEElectroCardioGramWaveformLeadNumber;

namespace io {
  class BIOGEARS_PRIVATE_API ElectroCardioGram {
  public:
    // template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    // class SEElectroCardioGram
    static void UnMarshall(const CDM::ElectroCardioGramData& in, SEElectroCardioGram& out);
    static void Marshall(const SEElectroCardioGram& in, CDM::ElectroCardioGramData& out);
    // class SEElectroCardioGram
    static void UnMarshall(const CDM::ElectroCardioGramWaveformLeadNumberData& in, SEElectroCardioGramWaveformLeadNumber& out);
    static void Marshall(const SEElectroCardioGramWaveformLeadNumber& in, CDM::ElectroCardioGramWaveformLeadNumberData& out);
    // class SEElectroCardioGramInterpolationWaveform
    static void UnMarshall(const CDM::ElectroCardioGramInterpolationWaveformData& in, SEElectroCardioGramInterpolationWaveform& out);
    static void Marshall(const SEElectroCardioGramInterpolationWaveform& in, CDM::ElectroCardioGramInterpolationWaveformData& out);
    // class SEElectroCardioGramWaveformInterpolator
    static void UnMarshall(const CDM::ElectroCardioGramInterpolatorData& in, SEElectroCardioGramInterpolator& out);
    static void Marshall(const SEElectroCardioGramInterpolator& in, CDM::ElectroCardioGramInterpolatorData& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void ElectroCardioGram::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      UnMarshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void ElectroCardioGram::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    Marshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO
} // Namespace Biogears
