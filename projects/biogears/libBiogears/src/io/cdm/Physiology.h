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

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/enums/SEPhysiologyEnums.h>
#include <biogears/exports.h>

#include <biogears/schema/cdm/Physiology.hxx>

#define CDM_PHYSIOLOGY_MARSHALL_HELPER(in, out, func)                                \
  if (in.m_##func) {                                                                 \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::Physiology::Marshall(*in.m_##func, out.func());                              \
  }

#define CDM_OPTIONAL_PHYSIOLOGY_MARSHALL_HELPER(in, out, func) \
  if (in.m_##func) {                                           \
    io::Physiology::Marshall(*in.m_##func, out.func());        \
  }

#define SE_PHYSIOLOGY_ENUM_MARSHALL_HELPER(in, out, func)                            \
  if (in.Has##func()) {                                                              \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::Physiology::Marshall(in.m_##func, out.func());                               \
  }

#define SE_OPTIONAL_PHYSIOLOGY_ENUM_MARSHALL_HELPER(in, out, func) \
  io::Physiology::Marshall(in.m_##func, out.func());

#define CDM_PHYSIOLOGY_COPY(type, in, out) \
  {                                        \
    CDM::##type##Data middle;              \
    io::Patient::Marshall(in, middle);     \
    io::Patient::UnMarshall(middle, out);  \
  }

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
class SEDrugTransitState;
class SEHepaticSystem;
class SENervousSystem;
class SERenalSystem;
class SERespiratorySystem;
class SETissueSystem;

namespace io {
  class BIOGEARS_PRIVATE_API Physiology {
  public:
    // template <typename SE, typename XSD>  option
    template <typename SE, typename XSD, std::enable_if_t<std::is_enum<SE>::value>* = nullptr>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD, std::enable_if_t<!std::is_enum<SE>::value>* = nullptr>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out, std::default_random_engine* rd = nullptr);
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    // class SEPupillaryResponse
    static void UnMarshall(const CDM::PupillaryResponseData& in, SEPupillaryResponse& out);
    static void Marshall(const SEPupillaryResponse& in, CDM::PupillaryResponseData& out);
    // class SEInflammationState
    static void UnMarshall(const CDM::InflammatoryResponseData& in, SEInflammatoryResponse& out);
    static void Marshall(const SEInflammatoryResponse& in, CDM::InflammatoryResponseData& out);
    // class SEBloodChemistrySystem
    static void UnMarshall(const CDM::BloodChemistrySystemData& in, SEBloodChemistrySystem& out);
    static void Marshall(const SEBloodChemistrySystem& in, CDM::BloodChemistrySystemData& out);
    // class SECardiovascularSystem
    static void UnMarshall(const CDM::CardiovascularSystemData& in, SECardiovascularSystem& out);
    static void Marshall(const SECardiovascularSystem& in, CDM::CardiovascularSystemData& out);
    // class SEDrugSystem
    static void UnMarshall(const CDM::DrugSystemData& in, SEDrugSystem& out);
    static void Marshall(const SEDrugSystem& in, CDM::DrugSystemData& out);
    // class SEEndocrineSystem
    static void UnMarshall(const CDM::EndocrineSystemData& in, SEEndocrineSystem& out);
    static void Marshall(const SEEndocrineSystem& in, CDM::EndocrineSystemData& out);
    // class SEEnergySystem
    static void UnMarshall(const CDM::EnergySystemData& in, SEEnergySystem& out);
    static void Marshall(const SEEnergySystem& in, CDM::EnergySystemData& out);
    // class SEGastrointestinalSystem
    static void UnMarshall(const CDM::GastrointestinalSystemData& in, SEGastrointestinalSystem& out);
    static void Marshall(const SEGastrointestinalSystem& in, CDM::GastrointestinalSystemData& out);
    // class SEDrugTransitState
    static void UnMarshall(const CDM::DrugTransitStateData& in, SEDrugTransitState& out);
    static void Marshall(const SEDrugTransitState& in, CDM::DrugTransitStateData& out);
    // class SEHepaticSystem
    static void UnMarshall(const CDM::HepaticSystemData& in, SEHepaticSystem& out);
    static void Marshall(const SEHepaticSystem& in, CDM::HepaticSystemData& out);
    // class SENervousSystem
    static void UnMarshall(const CDM::NervousSystemData& in, SENervousSystem& out);
    static void Marshall(const SENervousSystem& in, CDM::NervousSystemData& out);
    // class SERenalSystem
    static void UnMarshall(const CDM::RenalSystemData& in, SERenalSystem& out);
    static void Marshall(const SERenalSystem& in, CDM::RenalSystemData& out);
    // class SERespiratorySystem
    static void UnMarshall(const CDM::RespiratorySystemData& in, SERespiratorySystem& out);
    static void Marshall(const SERespiratorySystem& in, CDM::RespiratorySystemData& out);
    // class SETissueSystem
    static void UnMarshall(const CDM::TissueSystemData& in, SETissueSystem& out);
    static void Marshall(const SETissueSystem& in, CDM::TissueSystemData& out);

    // SEInflammationSource
    static void UnMarshall(const CDM::enumInflammationSource& in, SEInflammationSource& out);
    static void Marshall(const SEInflammationSource& in, CDM::enumInflammationSource& out);
    // SEHeartRhythm
    static void UnMarshall(const CDM::enumHeartRhythm& in, SEHeartRhythm& out);
    static void Marshall(const SEHeartRhythm& in, CDM::enumHeartRhythm& out);
    // SESleepState
    static void UnMarshall(const CDM::enumSleepState& in, SESleepState& out);
    static void Marshall(const SESleepState& in, CDM::enumSleepState& out);
  };
  //----------------------------------------------------------------------------------

  template <typename SE, typename XSD, std::enable_if_t<std::is_enum<SE>::value>*>
  void Physiology::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present() || option_in->empty()) {
      out = SE::Invalid;
    } else {
      UnMarshall(option_in.get(), out);
    }
  }

  template <typename SE, typename XSD, std::enable_if_t<!std::is_enum<SE>::value>*>
  void Physiology::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out, std::default_random_engine* rd)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      UnMarshall(option_in.get(), out);
    }
  }

  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Physiology::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    Marshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO

// Operators
bool operator==(CDM::enumInflammationSource const& lhs, SEInflammationSource const& rhs);
bool operator==(CDM::enumHeartRhythm const& lhs, SEHeartRhythm const& rhs);
bool operator==(CDM::enumSleepState const& lhs, SESleepState const& rhs);

inline bool operator==(SEInflammationSource const& lhs, CDM::enumInflammationSource const& rhs)
{
  return rhs == lhs;
}
inline bool operator==(SEHeartRhythm const& lhs, CDM::enumHeartRhythm const& rhs)
{
  return rhs == lhs;
}
inline bool operator==(SESleepState const& lhs, CDM::enumSleepState const& rhs)
{
  return rhs == lhs;
}

inline bool operator!=(CDM::enumInflammationSource const& lhs, SEInflammationSource const& rhs)
{
  return !(lhs == rhs);
}
inline bool operator!=(CDM::enumHeartRhythm const& lhs, SEHeartRhythm const& rhs)
{
  return !(lhs == rhs);
}
inline bool operator!=(CDM::enumSleepState const& lhs, SESleepState const& rhs)
{
  return !(lhs == rhs);
}

inline bool operator!=(SEInflammationSource const& lhs, CDM::enumInflammationSource const& rhs)
{
  return !(rhs == lhs);
}
inline bool operator!=(SEHeartRhythm const& lhs, CDM::enumHeartRhythm const& rhs)
{
  return !(rhs == lhs);
}
inline bool operator!=(SESleepState const& lhs, CDM::enumSleepState const& rhs)
{
  return !(rhs == lhs);
}

} // Namespace Biogears
