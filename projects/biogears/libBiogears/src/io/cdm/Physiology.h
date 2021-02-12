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

#include <biogears/schema/cdm/Physiology.hxx>

namespace biogears {
class SEPupillaryResponse;
class SEInflammatoryResponse;
class SEBloodChemistrySystem;
class SEenumHeartRhythm;
class SECardiovascularSystem;
class SEDrugSystem;
class SEEndocrineSystem;
class SEEnergySystem;
class SEGastrointestinalSystem;
class SEHepaticSystem;
class SENervousSystem;
class SERenalSystem;
class SERespiratorySystem;
class SETissueSystem;

#define CDM_PHYSIOLOGY_UNMARSHAL_HELPER(xsd, func)                           \
  if (m_##func) {                                                                    \
    xsd.func(std::make_unique<std::remove_reference<decltype(xsd.func())>::type>()); \
    io::Property::UnMarshall(*m_##func, xsd.func());                       \
  }
namespace io {
  class BIOGEARS_PRIVATE_API Physiology {
  public:
    //template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    //class SEPupillaryResponse
    static void Marshall(const CDM::PupillaryResponseData& in, SEPupillaryResponse& out);
    static void UnMarshall(const SEPupillaryResponse& in, CDM::PupillaryResponseData& out);
    //class SEInflammationState
    static void Marshall(const CDM::InflammatoryResponseData& in, SEInflammatoryResponse& out);
    static void UnMarshall(const SEInflammatoryResponse& in, CDM::InflammatoryResponseData& out);
    //class SEBloodChemistrySystem
    static void Marshall(const CDM::BloodChemistrySystemData& in, SEBloodChemistrySystem& out);
    static void UnMarshall(const SEBloodChemistrySystem& in, CDM::BloodChemistrySystemData& out);
    //class SECardiovascularSystem
    static void Marshall(const CDM::CardiovascularSystemData& in, SECardiovascularSystem& out);
    static void UnMarshall(const SECardiovascularSystem& in, CDM::CardiovascularSystemData& out);
    //class SEDrugSystem
    static void Marshall(const CDM::DrugSystemData& in, SEDrugSystem& out);
    static void UnMarshall(const SEDrugSystem& in, CDM::DrugSystemData& out);
    //class SEEndocrineSystem
    static void Marshall(const CDM::EndocrineSystemData& in, SEEndocrineSystem& out);
    static void UnMarshall(const SEEndocrineSystem& in, CDM::EndocrineSystemData& out);
    //class SEEnergySystem
    static void Marshall(const CDM::EnergySystemData& in, SEEnergySystem& out);
    static void UnMarshall(const SEEnergySystem& in, CDM::EnergySystemData& out);
    //class SEGastrointestinalSystem
    static void Marshall(const CDM::GastrointestinalSystemData& in, SEGastrointestinalSystem& out);
    static void UnMarshall(const SEGastrointestinalSystem& in, CDM::GastrointestinalSystemData& out);
    //class SEHepaticSystem
    static void Marshall(const CDM::HepaticSystemData& in, SEHepaticSystem& out);
    static void UnMarshall(const SEHepaticSystem& in, CDM::HepaticSystemData& out);
    //class SENervousSystem
    static void Marshall(const CDM::NervousSystemData& in, SENervousSystem& out);
    static void UnMarshall(const SENervousSystem& in, CDM::NervousSystemData& out);
    //class SERenalSystem
    static void Marshall(const CDM::RenalSystemData& in, SERenalSystem& out);
    static void UnMarshall(const SERenalSystem& in, CDM::RenalSystemData& out);
    //class SERespiratorySystem
    static void Marshall(const CDM::RespiratorySystemData& in, SERespiratorySystem& out);
    static void UnMarshall(const SERespiratorySystem& in, CDM::RespiratorySystemData& out);
    //class SETissueSystem
    static void Marshall(const CDM::TissueSystemData& in, SETissueSystem& out);
    static void UnMarshall(const SETissueSystem& in, CDM::TissueSystemData& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Physiology::Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      Marshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Physiology::UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    UnMarshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO
} //Namespace Biogears
