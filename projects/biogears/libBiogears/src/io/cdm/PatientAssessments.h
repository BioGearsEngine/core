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
#include <biogears/exports.h>

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/enums/SEPatientAssessmentEnums.h>
#include <biogears/cdm/properties/SEProperties.h>

#include <biogears/schema/cdm/PatientAssessments.hxx>

#define CDM_PATIENT_ASSESSMENTS_MARSHALL_HELPER(in, out, func)                       \
  if (in.m_##func) {                                                                 \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::PatientAssessments::Marshall(*in.m_##func, out.func());                      \
  }

#define CDM_OPTIONAL_PATIENT_ASSESSMENTS_MARSHALL_HELPER(in, out, func) \
  if (in.m_##func) {                                                    \
    io::PatientAssessments::Marshall(*in.m_##func, out.func());         \
  }

#define SE_PATIENT_ASSESSMENT_ENUM_MARSHALL_HELPER(in, out, func)                    \
  if (in.Has##func()) {                                                              \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::PatientAssessments::Marshall(in.m_##func, out.func());                       \
  }

#define SE_OPTIONAL_PATIENT_ASSESSMENT_ENUM_MARSHALL_HELPER(in, out, func) \
  io::PatientAssessments::Marshall(in.m_##func, out.func());

namespace biogears {
class SEPatientAssessment;
class SEArterialBloodGasAnalysis;
class SECompleteBloodCount;
class SEComprehensiveMetabolicPanel;
class SEProthrombinTime;
class SEPsychomotorVigilanceTask;
class SEPulmonaryFunctionTest;
class SESequentialOrganFailureAssessment;
class SEUrinalysis;
class SEUrinalysisMicroscopic;

// SEProthrombinTime.h
// SEPsychomotorVigilanceTask.h
// SESequentialOrganFailureAssessment.h

namespace io {
  class BIOGEARS_PRIVATE_API PatientAssessments {
  public:
    // template <typename SE, typename XSD>  option
    template <typename SE, typename XSD, std::enable_if_t<std::is_enum<SE>::value>* = nullptr>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD, std::enable_if_t<!std::is_enum<SE>::value>* = nullptr>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    // class SEPatientAssessment
    static void UnMarshall(const CDM::PatientAssessmentData& in, SEPatientAssessment& out);
    static void Marshall(const SEPatientAssessment& in, CDM::PatientAssessmentData& out);
    // class SEArterialBloodGasAnalysis
    static void UnMarshall(const CDM::ArterialBloodGasAnalysisData& in, SEArterialBloodGasAnalysis& out);
    static void Marshall(const SEArterialBloodGasAnalysis& in, CDM::ArterialBloodGasAnalysisData& out);
    // class SEPulmonaryFunctionTest
    static void UnMarshall(const CDM::PulmonaryFunctionTestData& in, SEPulmonaryFunctionTest& out);
    static void Marshall(const SEPulmonaryFunctionTest& in, CDM::PulmonaryFunctionTestData& out);
    // class SECompleteBloodCount
    static void UnMarshall(const CDM::CompleteBloodCountData& in, SECompleteBloodCount& out);
    static void Marshall(const SECompleteBloodCount& in, CDM::CompleteBloodCountData& out);
    // class SEComprehensiveMetabolicPanel
    static void UnMarshall(const CDM::ComprehensiveMetabolicPanelData& in, SEComprehensiveMetabolicPanel& out);
    static void Marshall(const SEComprehensiveMetabolicPanel& in, CDM::ComprehensiveMetabolicPanelData& out);
    // class SEUrinalysis
    static void UnMarshall(const CDM::UrinalysisData& in, SEUrinalysis& out);
    static void Marshall(const SEUrinalysis& in, CDM::UrinalysisData& out);
    // class SEUrinalysisMicroscopic
    static void UnMarshall(const CDM::UrinalysisMicroscopicData& in, SEUrinalysisMicroscopic& out);
    static void Marshall(const SEUrinalysisMicroscopic& in, CDM::UrinalysisMicroscopicData& out);
    // class SEProthrombinTime.h
    static void UnMarshall(const CDM::ProthrombinTimeData& in, SEProthrombinTime& out);
    static void Marshall(const SEProthrombinTime& in, CDM::ProthrombinTimeData& out);
    // class SEPsychomotorVigilanceTask.h
    static void UnMarshall(const CDM::PsychomotorVigilanceTaskData& in, SEPsychomotorVigilanceTask& out);
    static void Marshall(const SEPsychomotorVigilanceTask& in, CDM::PsychomotorVigilanceTaskData& out);
    // class SESequentialOrganFailureAssessment.h
    static void UnMarshall(const CDM::SequentialOrganFailureAssessmentData& in, SESequentialOrganFailureAssessment& out);
    static void Marshall(const SESequentialOrganFailureAssessment& in, CDM::SequentialOrganFailureAssessmentData& out);

    //----
    // SEPresenceIndicator
    static void UnMarshall(const CDM::enumPresenceIndicator& in, SEPresenceIndicator& out);
    static void Marshall(const SEPresenceIndicator& in, CDM::enumPresenceIndicator& out);
    // SEClarityIndicator
    static void UnMarshall(const CDM::enumClarityIndicator& in, SEClarityIndicator& out);
    static void Marshall(const SEClarityIndicator& in, CDM::enumClarityIndicator& out);
    // SEUrineColor
    static void UnMarshall(const CDM::enumUrineColor& in, SEUrineColor& out);
    static void Marshall(const SEUrineColor& in, CDM::enumUrineColor& out);
    // SEMicroscopicObservationType
    static void UnMarshall(const CDM::enumMicroscopicObservationType& in, SEMicroscopicObservationType& out);
    static void Marshall(const SEMicroscopicObservationType& in, CDM::enumMicroscopicObservationType& out);
    // SEMicroscopicObservationAmount
    static void UnMarshall(const CDM::enumMicroscopicObservationAmount& in, SEMicroscopicObservationAmount& out);
    static void Marshall(const SEMicroscopicObservationAmount& in, CDM::enumMicroscopicObservationAmount& out);
  };
  //-------------------------------------------------------------------------------
  template <typename SE, typename XSD, std::enable_if_t<std::is_enum<SE>::value>*>
  void PatientAssessments::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present() || option_in->empty()) {
      out = SE::Invalid;
    } else {
      UnMarshall(option_in.get(), out);
    }
  }

  template <typename SE, typename XSD, std::enable_if_t<!std::is_enum<SE>::value>*>
  void PatientAssessments::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      UnMarshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void PatientAssessments::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    Marshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO

// Operators
bool operator==(CDM::enumPresenceIndicator const& lhs, SEPresenceIndicator const& rhs);
bool operator==(CDM::enumClarityIndicator const& lhs, SEClarityIndicator const& rhs);
bool operator==(CDM::enumUrineColor const& lhs, SEUrineColor const& rhs);
bool operator==(CDM::enumMicroscopicObservationType const& lhs, SEMicroscopicObservationType const& rhs);
bool operator==(CDM::enumMicroscopicObservationAmount const& lhs, SEMicroscopicObservationAmount const& rhs);

inline bool operator==(SEPresenceIndicator const& lhs, CDM::enumPresenceIndicator const& rhs)
{
  return rhs == lhs;
}
inline bool operator==(SEClarityIndicator const& lhs, CDM::enumClarityIndicator const& rhs)
{
  return rhs == lhs;
}
inline bool operator==(SEUrineColor const& lhs, CDM::enumUrineColor const& rhs)
{
  return rhs == lhs;
}
inline bool operator==(SEMicroscopicObservationType const& lhs, CDM::enumMicroscopicObservationType const& rhs)
{
  return rhs == lhs;
}
inline bool operator==(SEMicroscopicObservationAmount const& lhs, CDM::enumMicroscopicObservationAmount const& rhs)
{
  return rhs == lhs;
}

inline bool operator!=(CDM::enumPresenceIndicator const& lhs, SEPresenceIndicator const& rhs)
{
  return !(lhs == rhs);
}
inline bool operator!=(CDM::enumClarityIndicator const& lhs, SEClarityIndicator const& rhs)
{
  return !(lhs == rhs);
}
inline bool operator!=(CDM::enumUrineColor const& lhs, SEUrineColor const& rhs)
{
  return !(lhs == rhs);
}
inline bool operator!=(CDM::enumMicroscopicObservationType const& lhs, SEMicroscopicObservationType const& rhs)
{
  return !(lhs == rhs);
}
inline bool operator!=(CDM::enumMicroscopicObservationAmount const& lhs, SEMicroscopicObservationAmount const& rhs)
{
  return !(lhs == rhs);
}

inline bool operator!=(SEPresenceIndicator const& lhs, CDM::enumPresenceIndicator const& rhs)
{
  return !(rhs == lhs);
}
inline bool operator!=(SEClarityIndicator const& lhs, CDM::enumClarityIndicator const& rhs)
{
  return !(rhs == lhs);
}
inline bool operator!=(SEUrineColor const& lhs, CDM::enumUrineColor const& rhs)
{
  return !(rhs == lhs);
}
inline bool operator!=(SEMicroscopicObservationType const& lhs, CDM::enumMicroscopicObservationType const& rhs)
{
  return !(rhs == lhs);
}
inline bool operator!=(SEMicroscopicObservationAmount const& lhs, CDM::enumMicroscopicObservationAmount const& rhs)
{
  return !(rhs == lhs);
}

} // Namespace Biogears
