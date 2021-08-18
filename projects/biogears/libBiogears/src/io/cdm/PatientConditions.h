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

#define CDM_PATIENT_CONDITIONS_UNMARSHAL_HELPER(xsd, func)                           \
  if (m_##func) {                                                                    \
    xsd.func(std::make_unique<std::remove_reference<decltype(xsd.func())>::type>()); \
    io::PatientConditions::UnMarshall(*m_##func, xsd.func());                        \
  }
namespace io {
  class BIOGEARS_PRIVATE_API PatientConditions {
  public:
    //class SEConditionList;
    static std::vector<std::unique_ptr<SECondition>> condition_factory(const CDM::ConditionListData& in, SESubstanceManager& substances);
    static std::unique_ptr<SECondition> factory(const CDM::ConditionData& data, SESubstanceManager& substances);
    //template <typename SE, typename XSD>  option
    
    template <typename SE, typename XSD>
    static void Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    
    template <typename SE, typename XSD>
    static void UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);

    //SEPatientCondition
    static void Marshall(const CDM::PatientConditionData& in, SEPatientCondition& out);
    static void UnMarshall(const SEPatientCondition& in, CDM::PatientConditionData& out);
    //SEChronicAnemia
    static void Marshall(const CDM::ChronicAnemiaData& in, SEChronicAnemia& out);
    static void UnMarshall(const SEChronicAnemia& in, CDM::ChronicAnemiaData& out);
    //SEChronicHeartFailure
    static void Marshall(const CDM::ChronicHeartFailureData& in, SEChronicHeartFailure& out);
    static void UnMarshall(const SEChronicHeartFailure& in, CDM::ChronicHeartFailureData& out);
    //SEChronicVentricularSystolicDysfunction
    static void Marshall(const CDM::ChronicVentricularSystolicDysfunctionData& in, SEChronicVentricularSystolicDysfunction& out);
    static void UnMarshall(const SEChronicVentricularSystolicDysfunction& in, CDM::ChronicVentricularSystolicDysfunctionData& out);
    //SEChronicObstructivePulmonaryDisease
    static void Marshall(const CDM::ChronicObstructivePulmonaryDiseaseData& in, SEChronicObstructivePulmonaryDisease& out);
    static void UnMarshall(const SEChronicObstructivePulmonaryDisease& in, CDM::ChronicObstructivePulmonaryDiseaseData& out);
    //SEChronicPericardialEffusion
    static void Marshall(const CDM::ChronicPericardialEffusionData& in, SEChronicPericardialEffusion& out);
    static void UnMarshall(const SEChronicPericardialEffusion& in, CDM::ChronicPericardialEffusionData& out);
    //SEChronicRenalStenosis
    static void Marshall(const CDM::ChronicRenalStenosisData& in, SEChronicRenalStenosis& out);
    static void UnMarshall(const SEChronicRenalStenosis& in, CDM::ChronicRenalStenosisData& out);
    //SEDehydration
    static void Marshall(const CDM::DehydrationData& in, SEDehydration& out);
    static void UnMarshall(const SEDehydration& in, CDM::DehydrationData& out);
    //SEDiabetesType1
    static void Marshall(const CDM::DiabetesType1Data& in, SEDiabetesType1& out);
    static void UnMarshall(const SEDiabetesType1& in, CDM::DiabetesType1Data& out);
    //SEDiabetesType2
    static void Marshall(const CDM::DiabetesType2Data& in, SEDiabetesType2& out);
    static void UnMarshall(const SEDiabetesType2& in, CDM::DiabetesType2Data& out);
    //SEImpairedAlveolarExchange
    static void Marshall(const CDM::ImpairedAlveolarExchangeData& in, SEImpairedAlveolarExchange& out);
    static void UnMarshall(const SEImpairedAlveolarExchange& in, CDM::ImpairedAlveolarExchangeData& out);
    //SELobarPneumonia
    static void Marshall(const CDM::LobarPneumoniaData& in, SELobarPneumonia& out);
    static void UnMarshall(const SELobarPneumonia& in, CDM::LobarPneumoniaData& out);
    //SEStarvation
    static void Marshall(const CDM::StarvationData& in, SEStarvation& out);
    static void UnMarshall(const SEStarvation& in, CDM::StarvationData& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void PatientConditions::Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Invalidate();
    } else {
      Marshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void PatientConditions::UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    UnMarshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO
} //Namespace Biogears
