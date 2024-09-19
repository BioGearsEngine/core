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
#include <biogears/exports.h>

#include <biogears/cdm/enums/SEPatientActionsEnums.h>
#include <biogears/cdm/enums/SEPatientEnums.h>

#include <biogears/schema/cdm/PatientActions.hxx>

#define CDM_PATIENT_ACTIONS_MARSHALL_HELPER(in, out, func)                           \
  if (in.m_##func) {                                                                 \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::PatientActions::Marshall(*in.m_##func, out.func());                          \
  }

#define CDM_OPTIONAL_PATIENT_ACTIONS_MARSHALL_HELPER(in, out, func) \
  if (in.m_##func) {                                                \
    io::PatientActions::Marshall(*in.m_##func, out.func());         \
  }

#define SE_PATIENT_ACTIONS_ENUM_MARSHALL_HELPER(in, out, func)                       \
  if (in.Has##func()) {                                                              \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::PatientActions::Marshall(in.m_##func, out.func());                           \
  }

#define SE_OPTIONAL_PATIENT_ACTIONS_ENUM_MARSHALL_HELPER(in, out, func) \
  io::PatientActions::Marshall(in.m_##func, out.func());

#define CDM_PATIENT_ACTION_COPY(type, in, out)   \
  {                                              \
    CDM::##type##Data middle;                    \
    io::PatientActions::Marshall(in, middle);    \
    io::PatientActions::UnMarshall(middle, out); \
  }

namespace biogears {
class SESubstanceManager;
class SEAction;

class SEPatientAction;
class SEPatientActionCollection;
class SEPatientAssessmentRequest;
class SEAcuteRespiratoryDistress;
class SEAcuteStress;
class SEExampleAction;
class SEAirwayObstruction;
class SEApnea;
class SEAsthmaAttack;
class SEBrainInjury;
class SEBronchoconstriction;
class SEBurnWound;
class SECardiacArrest;
class SEChestCompression;
class SEChestCompressionForce;
class SEChestCompressionForceScale;
class SEChestOcclusiveDressing;
class SEConsciousRespirationCommand;
class SEEbola;
class SEEscharotomy;
class SEForcedInhale;
class SEForcedExhale;
class SEBreathHold;
class SEUseInhaler;
class SEConsciousRespiration;
class SEConsumeNutrients;
class SEExercise;
class SEHemorrhage;
class SEInfection;
class SEIntubation;
class SEMechanicalVentilation;
class SENasalState;
class SENasalCannula;
class SENeedleDecompression;
class SEPainStimulus;
class SEPericardialEffusion;
class SEPulmonaryShunt;
class SERadiationAbsorbedDose;
class SETensionPneumothorax;
class SESleep;
class SESubstanceAdministration;
class SESubstanceBolus;
class SESubstanceBolusState;
class SESubstanceCompoundInfusion;
class SESubstanceInfusion;
class SESubstanceNasalDose;
class SESubstanceOralDose;
class SETourniquet;
class SETransmucosalState;
class SEUrinate;
class SEOverride;

namespace io {
  class BIOGEARS_PRIVATE_API PatientActions {
  public:
    // class Factories;
    static std::unique_ptr<SEAction> factory(CDM::PatientActionData const* patientActionData, SESubstanceManager& substances, std::default_random_engine* rd = nullptr);
    static std::unique_ptr<CDM::PatientActionData> factory(const SEPatientAction* data);

    static void Marshall(const SEPatientActionCollection& in, std::vector<std::unique_ptr<CDM::ActionData>>& out);
    // template <typename SE, typename XSD>  option
    template <typename SE, typename XSD, std::enable_if_t<std::is_enum<SE>::value>* = nullptr>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);

    template <typename SE, typename XSD, std::enable_if_t<!std::is_enum<SE>::value>* = nullptr>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);

    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    // class SEPatientAction;
    static void UnMarshall(const CDM::PatientActionData& in, SEPatientAction& out);
    static void Marshall(const SEPatientAction& in, CDM::PatientActionData& out);
    // class SEPatientAssessmentRequest;
    static void UnMarshall(const CDM::PatientAssessmentRequestData& in, SEPatientAssessmentRequest& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEPatientAssessmentRequest& in, CDM::PatientAssessmentRequestData& out);
    // class SEAcuteRespiratoryDistressStress;
    static void UnMarshall(const CDM::AcuteRespiratoryDistressData& in, SEAcuteRespiratoryDistress& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEAcuteRespiratoryDistress& in, CDM::AcuteRespiratoryDistressData& out);
    // class SEAcuteStress;
    static void UnMarshall(const CDM::AcuteStressData& in, SEAcuteStress& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEAcuteStress& in, CDM::AcuteStressData& out);
    // class SEExampleAction;
    static void UnMarshall(const CDM::ExampleActionData& in, SEExampleAction& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEExampleAction& in, CDM::ExampleActionData& out);
    // class SEAirwayObstruction;
    static void UnMarshall(const CDM::AirwayObstructionData& in, SEAirwayObstruction& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEAirwayObstruction& in, CDM::AirwayObstructionData& out);
    // class SEApnea;
    static void UnMarshall(const CDM::ApneaData& in, SEApnea& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEApnea& in, CDM::ApneaData& out);
    // class SEAsthmaAttack;
    static void UnMarshall(const CDM::AsthmaAttackData& in, SEAsthmaAttack& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEAsthmaAttack& in, CDM::AsthmaAttackData& out);
    // class SEBrainInjury;
    static void UnMarshall(const CDM::BrainInjuryData& in, SEBrainInjury& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEBrainInjury& in, CDM::BrainInjuryData& out);
    // class SEBronchoconstriction;
    static void UnMarshall(const CDM::BronchoconstrictionData& in, SEBronchoconstriction& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEBronchoconstriction& in, CDM::BronchoconstrictionData& out);
    // class SEBurnWound;
    static void UnMarshall(const CDM::BurnWoundData& in, SEBurnWound& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEBurnWound& in, CDM::BurnWoundData& out);
    // class SECardiacArrest;
    static void UnMarshall(const CDM::CardiacArrestData& in, SECardiacArrest& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SECardiacArrest& in, CDM::CardiacArrestData& out);
    // class SEChestCompression;
    static void UnMarshall(const CDM::ChestCompressionData& in, SEChestCompression& out);
    static void Marshall(const SEChestCompression& in, CDM::ChestCompressionData& out);
    // class SEChestCompressionForce;
    static void UnMarshall(const CDM::ChestCompressionForceData& in, SEChestCompressionForce& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEChestCompressionForce& in, CDM::ChestCompressionForceData& out);
    // class SEChestCompressionForceScale;
    static void UnMarshall(const CDM::ChestCompressionForceScaleData& in, SEChestCompressionForceScale& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEChestCompressionForceScale& in, CDM::ChestCompressionForceScaleData& out);
    // class SEChestOcclusiveDressing;
    static void UnMarshall(const CDM::ChestOcclusiveDressingData& in, SEChestOcclusiveDressing& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEChestOcclusiveDressing& in, CDM::ChestOcclusiveDressingData& out);
    // class SEConsciousRespirationCommand;
    static void UnMarshall(const CDM::ConsciousRespirationCommandData& in, SEConsciousRespirationCommand& out);
    static void Marshall(const SEConsciousRespirationCommand& in, CDM::ConsciousRespirationCommandData& out);
    // class SEEbola;
    static void UnMarshall(const CDM::EbolaData& in, SEEbola& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEEbola& in, CDM::EbolaData& out);
    // class SEEscharotomy
    static void UnMarshall(const CDM::EscharotomyData& in, SEEscharotomy& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEEscharotomy& in, CDM::EscharotomyData& out);
    // class SEExercise;
    static void UnMarshall(const CDM::ExerciseData& in, SEExercise& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEExercise& in, CDM::ExerciseData& out);
    // class SEForcedInhale;
    static void UnMarshall(const CDM::ForcedInhaleData& in, SEForcedInhale& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEForcedInhale& in, CDM::ForcedInhaleData& out);
    // class SEForcedExhale;
    static void UnMarshall(const CDM::ForcedExhaleData& in, SEForcedExhale& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEForcedExhale& in, CDM::ForcedExhaleData& out);
    // class SEBreathHold;
    static void UnMarshall(const CDM::BreathHoldData& in, SEBreathHold& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEBreathHold& in, CDM::BreathHoldData& out);
    // class SEUseInhaler;
    static void UnMarshall(const CDM::UseInhalerData& in, SEUseInhaler& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEUseInhaler& in, CDM::UseInhalerData& out);
    // class SEConsciousRespiration;
    static void UnMarshall(const CDM::ConsciousRespirationData& in, SEConsciousRespiration& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEConsciousRespiration& in, CDM::ConsciousRespirationData& out);
    // class SEConsumeNutrients;
    static void UnMarshall(const CDM::ConsumeNutrientsData& in, SEConsumeNutrients& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEConsumeNutrients& in, CDM::ConsumeNutrientsData& out);
    // class SEHemorrhage;
    static void UnMarshall(const CDM::HemorrhageData& in, SEHemorrhage& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEHemorrhage& in, CDM::HemorrhageData& out);
    // class SEInfection;
    static void UnMarshall(const CDM::InfectionData& in, SEInfection& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEInfection& in, CDM::InfectionData& out);
    // class SEIntubation;
    static void UnMarshall(const CDM::IntubationData& in, SEIntubation& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEIntubation& in, CDM::IntubationData& out);
    // class SEMechanicalVentilation;
    static void UnMarshall(const CDM::MechanicalVentilationData& in, const SESubstanceManager& substances, SEMechanicalVentilation& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEMechanicalVentilation& in, CDM::MechanicalVentilationData& out);
    // class SENasalCannula;
    static void UnMarshall(const CDM::NasalCannulaData& in, SENasalCannula& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SENasalCannula& in, CDM::NasalCannulaData& out);
    // class SENasalState;
    static void UnMarshall(const CDM::NasalStateData& in, SENasalState& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SENasalState& in, CDM::NasalStateData& out);
    // class SENeedleDecompression;
    static void UnMarshall(const CDM::NeedleDecompressionData& in, SENeedleDecompression& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SENeedleDecompression& in, CDM::NeedleDecompressionData& out);
    // class SEPainStimulus;
    static void UnMarshall(const CDM::PainStimulusData& in, SEPainStimulus& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEPainStimulus& in, CDM::PainStimulusData& out);
    // class SEPericardialEffusion;
    static void UnMarshall(const CDM::PericardialEffusionData& in, SEPericardialEffusion& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEPericardialEffusion& in, CDM::PericardialEffusionData& out);
    // class SEPulmonaryShunt;
    static void UnMarshall(const CDM::PulmonaryShuntData& in, SEPulmonaryShunt& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEPulmonaryShunt& in, CDM::PulmonaryShuntData& out);
    // class SERadiationAbsorbedDose;
    static void UnMarshall(const CDM::RadiationAbsorbedDoseData& in, SERadiationAbsorbedDose& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SERadiationAbsorbedDose& in, CDM::RadiationAbsorbedDoseData& out);
    // class SESleep;
    static void UnMarshall(const CDM::SleepData& in, SESleep& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SESleep& in, CDM::SleepData& out);
    // class SESubstanceNasalDose;
    static void UnMarshall(const CDM::SubstanceNasalDoseData& in, SESubstanceNasalDose& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SESubstanceNasalDose& in, CDM::SubstanceNasalDoseData& out);
    // class SETensionPneumothorax;
    static void UnMarshall(const CDM::TensionPneumothoraxData& in, SETensionPneumothorax& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SETensionPneumothorax& in, CDM::TensionPneumothoraxData& out);
    // class SESubstanceAdministration;
    static void UnMarshall(const CDM::SubstanceAdministrationData& in, SESubstanceAdministration& out);
    static void Marshall(const SESubstanceAdministration& in, CDM::SubstanceAdministrationData& out);
    // class SESubstanceBolus;
    static void UnMarshall(const CDM::SubstanceBolusData& in, SESubstanceBolus& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SESubstanceBolus& in, CDM::SubstanceBolusData& out);
    // class SESubstanceBolusState;
    static void UnMarshall(const CDM::SubstanceBolusStateData& in, SESubstanceBolusState& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SESubstanceBolusState& in, CDM::SubstanceBolusStateData& out);
    // class SESubstanceCompoundInfusion;
    static void UnMarshall(const CDM::SubstanceCompoundInfusionData& in, SESubstanceCompoundInfusion& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SESubstanceCompoundInfusion& in, CDM::SubstanceCompoundInfusionData& out);
    // class SESubstanceInfusion;
    static void UnMarshall(const CDM::SubstanceInfusionData& in, SESubstanceInfusion& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SESubstanceInfusion& in, CDM::SubstanceInfusionData& out);
    // class SESubstanceOralDose;
    static void UnMarshall(const CDM::SubstanceOralDoseData& in, SESubstanceOralDose& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SESubstanceOralDose& in, CDM::SubstanceOralDoseData& out);
    // class SETransmucosalState;
    static void UnMarshall(const CDM::TransmucosalStateData& in, SETransmucosalState& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SETransmucosalState& in, CDM::TransmucosalStateData& out);
    // class SETourniquet;
    static void UnMarshall(const CDM::TourniquetData& in, SETourniquet& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SETourniquet& in, CDM::TourniquetData& out);
    // class SEUrinate;
    static void UnMarshall(const CDM::UrinateData& in, SEUrinate& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEUrinate& in, CDM::UrinateData& out);
    // class SEOverride;
    static void UnMarshall(const CDM::OverrideData& in, SEOverride& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEOverride& in, CDM::OverrideData& out);

    // SEBrainInjuryType
    static void UnMarshall(const CDM::enumBrainInjuryType& in, SEBrainInjuryType& out);
    static void Marshall(const SEBrainInjuryType& in, CDM::enumBrainInjuryType& out);
    // SEBolusAdministration
    static void UnMarshall(const CDM::enumBolusAdministration& in, SEBolusAdministration& out);
    static void Marshall(const SEBolusAdministration& in, CDM::enumBolusAdministration& out);
    // SEBurnDegree
    static void UnMarshall(const CDM::enumBurnDegree& in, SEBurnDegree& out);
    static void Marshall(const SEBurnDegree& in, CDM::enumBurnDegree& out);
    //  SEInfectionSeverity
    static void UnMarshall(const CDM::enumInfectionSeverity& in, SEInfectionSeverity& out);
    static void Marshall(const SEInfectionSeverity& in, CDM::enumInfectionSeverity& out);
    // SEIntubationType
    static void UnMarshall(const CDM::enumIntubationType& in, SEIntubationType& out);
    static void Marshall(const SEIntubationType& in, CDM::enumIntubationType& out);
    // SEPatientAssessmentType
    static void UnMarshall(const CDM::enumPatientAssessment& in, SEPatientAssessmentType& out);
    static void Marshall(const SEPatientAssessmentType& in, CDM::enumPatientAssessment& out);
    // SEPneumothoraxType
    static void UnMarshall(const CDM::enumPneumothoraxType& in, SEPneumothoraxType& out);
    static void Marshall(const SEPneumothoraxType& in, CDM::enumPneumothoraxType& out);
    // SEOralAdministrationType
    static void UnMarshall(const CDM::enumOralAdministration in, SEOralAdministrationType& out);
    static void Marshall(const SEOralAdministrationType& in, CDM::enumOralAdministration& out);
    //  SETourniquetApplicationType
    static void UnMarshall(const CDM::enumTourniquetApplicationLevel& in, SETourniquetApplicationType& out);
    static void Marshall(const SETourniquetApplicationType& in, CDM::enumTourniquetApplicationLevel& out);
  };

  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD, std::enable_if_t<std::is_enum<SE>::value>*>
  void PatientActions::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present() || option_in->empty()) {
      out = SE::Invalid;
    } else {
      io::PatientActions::UnMarshall(option_in.get(), out);
    }
  }

  template <typename SE, typename XSD, std::enable_if_t<!std::is_enum<SE>::value>*>
  void PatientActions::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      io::PatientActions::UnMarshall(option_in.get(), out);
    }
  }

  template <typename SE, typename XSD>
  void PatientActions::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {

    auto item = std::make_unique<XSD>();
    io::PatientActions::Marshall(in, *item);
    option_out.set(std::move(item));
  }
} // Namespace IO

// Operators
bool operator==(CDM::enumBrainInjuryType const& lhs, SEBrainInjuryType const& rhs);
bool operator==(CDM::enumBolusAdministration const& lhs, SEBolusAdministration const& rhs);
bool operator==(CDM::enumBurnDegree const& lhs, SEBurnDegree const& rhs);
bool operator==(CDM::enumInfectionSeverity const& lhs, SEInfectionSeverity const& rhs);
bool operator==(CDM::enumIntubationType const& lhs, SEIntubationType const& rhs);
bool operator==(CDM::enumPatientAssessment const& lhs, SEPatientAssessmentType const& rhs);
bool operator==(CDM::enumPneumothoraxType const& lhs, SEPneumothoraxType const& rhs);
bool operator==(CDM::enumOralAdministration const& lhs, SEOralAdministrationType const& rhs);
bool operator==(CDM::enumTourniquetApplicationLevel const& lhs, SETourniquetApplicationType const& rhs);

inline bool operator==(SEBrainInjuryType const& lhs, CDM::enumBrainInjuryType const& rhs)
{
  return rhs == lhs;
}
inline bool operator==(SEBolusAdministration const& lhs, CDM::enumBolusAdministration const& rhs)
{
  return rhs == lhs;
}
inline bool operator==(SEBurnDegree const& lhs, CDM::enumBurnDegree const& rhs)
{
  return rhs == lhs;
}
inline bool operator==(SEInfectionSeverity const& lhs, CDM::enumInfectionSeverity const& rhs)
{
  return rhs == lhs;
}
inline bool operator==(SEIntubationType const& lhs, CDM::enumIntubationType const& rhs)
{
  return rhs == lhs;
}
inline bool operator==(SEPatientAssessmentType const& lhs, CDM::enumPatientAssessment const& rhs)
{
  return rhs == lhs;
}
inline bool operator==(SEPneumothoraxType const& lhs, CDM::enumPneumothoraxType const& rhs)
{
  return rhs == lhs;
}
inline bool operator==(SEOralAdministrationType const& lhs, CDM::enumOralAdministration const& rhs)
{
  return rhs == lhs;
}
inline bool operator==(SETourniquetApplicationType const& lhs, CDM::enumTourniquetApplicationLevel const& rhs)
{
  return rhs == lhs;
}

inline bool operator!=(CDM::enumBrainInjuryType const& lhs, SEBrainInjuryType const& rhs)
{
  return !(lhs == rhs);
}
inline bool operator!=(CDM::enumBolusAdministration const& lhs, SEBolusAdministration const& rhs)
{
  return !(lhs == rhs);
}
inline bool operator!=(CDM::enumBurnDegree const& lhs, SEBurnDegree const& rhs)
{
  return !(lhs == rhs);
}
inline bool operator!=(CDM::enumInfectionSeverity const& lhs, SEInfectionSeverity const& rhs)
{
  return !(lhs == rhs);
}
inline bool operator!=(CDM::enumIntubationType const& lhs, SEIntubationType const& rhs)
{
  return !(lhs == rhs);
}
inline bool operator!=(CDM::enumPatientAssessment const& lhs, SEPatientAssessmentType const& rhs)
{
  return !(lhs == rhs);
}
inline bool operator!=(CDM::enumPneumothoraxType const& lhs, SEPneumothoraxType const& rhs)
{
  return !(lhs == rhs);
}
inline bool operator!=(CDM::enumOralAdministration const& lhs, SEOralAdministrationType const& rhs)
{
  return !(lhs == rhs);
}
inline bool operator!=(CDM::enumTourniquetApplicationLevel const& lhs, SETourniquetApplicationType const& rhs)
{
  return !(lhs == rhs);
}

inline bool operator!=(SEBrainInjuryType const& lhs, CDM::enumBrainInjuryType const& rhs)
{
  return !(rhs == lhs);
}
inline bool operator!=(SEBolusAdministration const& lhs, CDM::enumBolusAdministration const& rhs)
{
  return !(rhs == lhs);
}
inline bool operator!=(SEBurnDegree const& lhs, CDM::enumBurnDegree const& rhs)
{
  return !(rhs == lhs);
}
inline bool operator!=(SEInfectionSeverity const& lhs, CDM::enumInfectionSeverity const& rhs)
{
  return !(rhs == lhs);
} // Namespace Biogears
inline bool operator!=(SEIntubationType const& lhs, CDM::enumIntubationType const& rhs)
{
  return !(rhs == lhs);
}
inline bool operator!=(SEPatientAssessmentType const& lhs, CDM::enumPatientAssessment const& rhs)
{
  return !(rhs == lhs);
}
inline bool operator!=(SEPneumothoraxType const& lhs, CDM::enumPneumothoraxType const& rhs)
{
  return !(rhs == lhs);
}
inline bool operator!=(SEOralAdministrationType const& lhs, CDM::enumOralAdministration const& rhs)
{
  return !(rhs == lhs);
}
inline bool operator!=(SETourniquetApplicationType const& lhs, CDM::enumTourniquetApplicationLevel const& rhs)
{
  return !(rhs == lhs);
}

}
