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

#include <biogears/schema/cdm/PatientAssessments.hxx>

#define CDM_PATIENT_ASSESSMENTS_UNMARSHAL_HELPER(in, out, func)                      \
  if (in.m_##func) {                                                                 \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::PatientAssessments::UnMarshall(*in.m_##func, out.func());                    \
  }

#define CDM_OPTIONAL_PATIENT_ASSESSMENTS_UNMARSHAL_HELPER(in, out, func) \
  if (in.m_##func) {                                                     \
    io::PatientAssessments::UnMarshall(*in.m_##func, out.func());        \
  }

namespace biogears {
class SEPatientAssessment;
class SEArterialBloodGasAnalysis;
class SEPulmonaryFunctionTest;
class SECompleteBloodCount;
class SEComprehensiveMetabolicPanel;
class SEUrinalysis;
class SEUrinalysisMicroscopic;

namespace io {
  class BIOGEARS_PRIVATE_API PatientAssessments {
  public:
    //template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    //class SEPatientAssessment
    static void Marshall(const CDM::PatientAssessmentData& in, SEPatientAssessment& out);
    static void UnMarshall(const SEPatientAssessment& in, CDM::PatientAssessmentData& out);
    //class SEArterialBloodGasAnalysis
    static void Marshall(const CDM::ArterialBloodGasAnalysisData& in, SEArterialBloodGasAnalysis& out);
    static void UnMarshall(const SEArterialBloodGasAnalysis& in, CDM::ArterialBloodGasAnalysisData& out);
    //class SEPulmonaryFunctionTest
    static void Marshall(const CDM::PulmonaryFunctionTestData& in, SEPulmonaryFunctionTest& out);
    static void UnMarshall(const SEPulmonaryFunctionTest& in, CDM::PulmonaryFunctionTestData& out);
    //class SECompleteBloodCount
    static void Marshall(const CDM::CompleteBloodCountData& in, SECompleteBloodCount& out);
    static void UnMarshall(const SECompleteBloodCount& in, CDM::CompleteBloodCountData& out);
    //class SEComprehensiveMetabolicPanel
    static void Marshall(const CDM::ComprehensiveMetabolicPanelData& in, SEComprehensiveMetabolicPanel& out);
    static void UnMarshall(const SEComprehensiveMetabolicPanel& in, CDM::ComprehensiveMetabolicPanelData& out);
    //class SEUrinalysis
    static void Marshall(const CDM::UrinalysisData& in, SEUrinalysis& out);
    static void UnMarshall(const SEUrinalysis& in, CDM::UrinalysisData& out);
    //class SEUrinalysisMicroscopic
    static void Marshall(const CDM::UrinalysisMicroscopicData& in, SEUrinalysisMicroscopic& out);
    static void UnMarshall(const SEUrinalysisMicroscopic& in, CDM::UrinalysisMicroscopicData& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void PatientAssessments::Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      Marshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void PatientAssessments::UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    UnMarshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO
} //Namespace Biogears
