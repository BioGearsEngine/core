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

#include <biogears/schema/cdm/PatientConditions.hxx>

#define CDM_PATIENT_CONDITIONS_MARSHALL_HELPER(in, out, func)                       \
  if (in.m_##func) {                                                                 \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::PatientConditions::Marshall(*in.m_##func, out.func());                     \
  }

#define CDM_OPTIONAL_PATIENT_CONDITIONS_MARSHALL_HELPER(in, out, func) \
  if (in.m_##func) {                                                    \
    io::PatientConditions::Marshall(*in.m_##func, out.func());        \
  }

namespace biogears {
class SESubstanceManager;
class SECondition;

class SEPatientCondition;
class SEChronicAnemia;
class SEChronicHeartFailure;
class SEChronicVentricularSystolicDysfunction;
class SEChronicObstructivePulmonaryDisease;
class SEChronicPericardialEffusion;
class SEChronicRenalStenosis;
class SEDehydration;
class SEDiabetesType1;
class SEDiabetesType2;
class SEImpairedAlveolarExchange;
class SELobarPneumonia;
class SEStarvation;

namespace io {
  class BIOGEARS_PRIVATE_API PatientConditions {
  public:
    //class SEConditionList;
    static std::vector<std::unique_ptr<SECondition>> condition_factory(const CDM::ConditionListData& in, SESubstanceManager& substances);
    static std::unique_ptr<SECondition> factory(const CDM::ConditionData* data, SESubstanceManager& substances);
    static std::unique_ptr<CDM::ConditionData> factory(const SECondition* data);
    //template <typename SE, typename XSD>  option    
    template <typename SE, typename XSD>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);

    //SEPatientCondition
    static void UnMarshall(const CDM::PatientConditionData& in, SEPatientCondition& out);
    static void Marshall(const SEPatientCondition& in, CDM::PatientConditionData& out);
    //SEChronicAnemia
    static void UnMarshall(const CDM::ChronicAnemiaData& in, SEChronicAnemia& out);
    static void Marshall(const SEChronicAnemia& in, CDM::ChronicAnemiaData& out);
    //SEChronicHeartFailure
    static void UnMarshall(const CDM::ChronicHeartFailureData& in, SEChronicHeartFailure& out);
    static void Marshall(const SEChronicHeartFailure& in, CDM::ChronicHeartFailureData& out);
    //SEChronicVentricularSystolicDysfunction
    static void UnMarshall(const CDM::ChronicVentricularSystolicDysfunctionData& in, SEChronicVentricularSystolicDysfunction& out);
    static void Marshall(const SEChronicVentricularSystolicDysfunction& in, CDM::ChronicVentricularSystolicDysfunctionData& out);
    //SEChronicObstructivePulmonaryDisease
    static void UnMarshall(const CDM::ChronicObstructivePulmonaryDiseaseData& in, SEChronicObstructivePulmonaryDisease& out);
    static void Marshall(const SEChronicObstructivePulmonaryDisease& in, CDM::ChronicObstructivePulmonaryDiseaseData& out);
    //SEChronicPericardialEffusion
    static void UnMarshall(const CDM::ChronicPericardialEffusionData& in, SEChronicPericardialEffusion& out);
    static void Marshall(const SEChronicPericardialEffusion& in, CDM::ChronicPericardialEffusionData& out);
    //SEChronicRenalStenosis
    static void UnMarshall(const CDM::ChronicRenalStenosisData& in, SEChronicRenalStenosis& out);
    static void Marshall(const SEChronicRenalStenosis& in, CDM::ChronicRenalStenosisData& out);
    //SEDehydration
    static void UnMarshall(const CDM::DehydrationData& in, SEDehydration& out);
    static void Marshall(const SEDehydration& in, CDM::DehydrationData& out);
    //SEDiabetesType1
    static void UnMarshall(const CDM::DiabetesType1Data& in, SEDiabetesType1& out);
    static void Marshall(const SEDiabetesType1& in, CDM::DiabetesType1Data& out);
    //SEDiabetesType2
    static void UnMarshall(const CDM::DiabetesType2Data& in, SEDiabetesType2& out);
    static void Marshall(const SEDiabetesType2& in, CDM::DiabetesType2Data& out);
    //SEImpairedAlveolarExchange
    static void UnMarshall(const CDM::ImpairedAlveolarExchangeData& in, SEImpairedAlveolarExchange& out);
    static void Marshall(const SEImpairedAlveolarExchange& in, CDM::ImpairedAlveolarExchangeData& out);
    //SELobarPneumonia
    static void UnMarshall(const CDM::LobarPneumoniaData& in, SELobarPneumonia& out);
    static void Marshall(const SELobarPneumonia& in, CDM::LobarPneumoniaData& out);
    //SEStarvation
    static void UnMarshall(const CDM::StarvationData& in, SEStarvation& out);
    static void Marshall(const SEStarvation& in, CDM::StarvationData& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void PatientConditions::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Invalidate();
    } else {
      UnMarshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void PatientConditions::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    Marshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO
} //Namespace Biogears
