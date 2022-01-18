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

#include <biogears/schema/cdm/PatientActions.hxx>

//#include <memory>

namespace biogears {
class SESubstanceManager;
class SEAction;

class SEPatientAction;
class SEPatientAssessmentRequest;
class SEAcuteRespiratoryDistress;
class SEAcuteStress;
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
class SENeedleDecompression;
class SEPainStimulus;
class SEPericardialEffusion;
class SEPulmonaryShunt;
class SERadiationAbsorbedDose;
class SETensionPneumothorax;
class SESubstanceAdministration;
class SESubstanceBolus;
class SESubstanceBolusState;
class SESubstanceCompoundInfusion;
class SESubstanceInfusion;
class SEUrinate;
class SEOverride;

#define CDM_PATIENT_ACTIONS_UNMARSHAL_HELPER(xsd, func)                              \
  if (m_##func) {                                                                    \
    xsd.func(std::make_unique<std::remove_reference<decltype(xsd.func())>::type>()); \
    io::PatientActions::UnMarshall(*m_##func, xsd.func());                           \
  }

#define CDM_OPTIONAL_PATIENT_ACTIONS_UNMARSHAL_HELPER(xsd, func) \
  if (in.m_##func) {                                             \
    io::PatientActions::UnMarshall(*in.m_##func, out.func());

namespace io {
  class BIOGEARS_PRIVATE_API PatientActions {
  public:
    //class SEActionList;
    static std::vector<std::unique_ptr<SEAction>> action_factory(const CDM::ActionListData& in, SESubstanceManager& substances);
    static std::unique_ptr<SEAction> factory(const CDM::ActionData& data, SESubstanceManager& substances);
    //template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    //class SEPatientAction;
    static void Marshall(const CDM::PatientActionData& in, SEPatientAction& out);
    static void UnMarshall(const SEPatientAction& in, CDM::PatientActionData& out);
    //class SEPatientAssessmentRequest;
    static void Marshall(const CDM::PatientAssessmentRequestData& in, SEPatientAssessmentRequest& out);
    static void UnMarshall(const SEPatientAssessmentRequest& in, CDM::PatientAssessmentRequestData& out);
    //class SEAcuteRespiratoryDistressStress;
    static void Marshall(const CDM::AcuteRespiratoryDistressData& in, SEAcuteRespiratoryDistress& out);
    static void UnMarshall(const SEAcuteRespiratoryDistress& in, CDM::AcuteRespiratoryDistressData& out);
    //class SEAcuteStress;
    static void Marshall(const CDM::AcuteStressData& in, SEAcuteStress& out);
    static void UnMarshall(const SEAcuteStress& in, CDM::AcuteStressData& out);
    //class SEAirwayObstruction;
    static void Marshall(const CDM::AirwayObstructionData& in, SEAirwayObstruction& out);
    static void UnMarshall(const SEAirwayObstruction& in, CDM::AirwayObstructionData& out);
    //class SEApnea;
    static void Marshall(const CDM::ApneaData& in, SEApnea& out);
    static void UnMarshall(const SEApnea& in, CDM::ApneaData& out);
    //class SEAsthmaAttack;
    static void Marshall(const CDM::AsthmaAttackData& in, SEAsthmaAttack& out);
    static void UnMarshall(const SEAsthmaAttack& in, CDM::AsthmaAttackData& out);
    //class SEBrainInjury;
    static void Marshall(const CDM::BrainInjuryData& in, SEBrainInjury& out);
    static void UnMarshall(const SEBrainInjury& in, CDM::BrainInjuryData& out);
    //class SEBronchoconstriction;
    static void Marshall(const CDM::BronchoconstrictionData& in, SEBronchoconstriction& out);
    static void UnMarshall(const SEBronchoconstriction& in, CDM::BronchoconstrictionData& out);
    //class SEBurnWound;
    static void Marshall(const CDM::BurnWoundData& in, SEBurnWound& out);
    static void UnMarshall(const SEBurnWound& in, CDM::BurnWoundData& out);
    //class SECardiacArrest;
    static void Marshall(const CDM::CardiacArrestData& in, SECardiacArrest& out);
    static void UnMarshall(const SECardiacArrest& in, CDM::CardiacArrestData& out);
    //class SEChestCompression;
    static void Marshall(const CDM::ChestCompressionData& in, SEChestCompression& out);
    static void UnMarshall(const SEChestCompression& in, CDM::ChestCompressionData& out);
    //class SEChestCompressionForce;
    static void Marshall(const CDM::ChestCompressionForceData& in, SEChestCompressionForce& out);
    static void UnMarshall(const SEChestCompressionForce& in, CDM::ChestCompressionForceData& out);
    //class SEChestCompressionForceScale;
    static void Marshall(const CDM::ChestCompressionForceScaleData& in, SEChestCompressionForceScale& out);
    static void UnMarshall(const SEChestCompressionForceScale& in, CDM::ChestCompressionForceScaleData& out);
    //class SEChestOcclusiveDressing;
    static void Marshall(const CDM::ChestOcclusiveDressingData& in, SEChestOcclusiveDressing& out);
    static void UnMarshall(const SEChestOcclusiveDressing& in, CDM::ChestOcclusiveDressingData& out);
    //class SEConsciousRespirationCommand;
    static void Marshall(const CDM::ConsciousRespirationCommandData& in, SEConsciousRespirationCommand& out);
    static void UnMarshall(const SEConsciousRespirationCommand& in, CDM::ConsciousRespirationCommandData& out);
    //class SEEscharotomy
    static void Marshall(const CDM::EscharotomyData& in, SEEscharotomy& out);
    static void UnMarshall(const SEEscharotomy& in, CDM::EscharotomyData& out);
    //class SEForcedInhale;
    static void Marshall(const CDM::ForcedInhaleData& in, SEForcedInhale& out);
    static void UnMarshall(const SEForcedInhale& in, CDM::ForcedInhaleData& out);
    //class SEForcedExhale;
    static void Marshall(const CDM::ForcedExhaleData& in, SEForcedExhale& out);
    static void UnMarshall(const SEForcedExhale& in, CDM::ForcedExhaleData& out);
    //class SEBreathHold;
    static void Marshall(const CDM::BreathHoldData& in, SEBreathHold& out);
    static void UnMarshall(const SEBreathHold& in, CDM::BreathHoldData& out);
    //class SEUseInhaler;
    static void Marshall(const CDM::UseInhalerData& in, SEUseInhaler& out);
    static void UnMarshall(const SEUseInhaler& in, CDM::UseInhalerData& out);
    //class SEConsciousRespiration;
    static void Marshall(const CDM::ConsciousRespirationData& in, SEConsciousRespiration& out);
    static void UnMarshall(const SEConsciousRespiration& in, CDM::ConsciousRespirationData& out);
    //class SEConsumeNutrients;
    static void Marshall(const CDM::ConsumeNutrientsData& in, SEConsumeNutrients& out);
    static void UnMarshall(const SEConsumeNutrients& in, CDM::ConsumeNutrientsData& out);
    //class SEExercise;
    static void Marshall(const CDM::ExerciseData& in, SEExercise& out);
    static void UnMarshall(const SEExercise& in, CDM::ExerciseData& out);
    //class SEHemorrhage;
    static void Marshall(const CDM::HemorrhageData& in, SEHemorrhage& out);
    static void UnMarshall(const SEHemorrhage& in, CDM::HemorrhageData& out);
    //class SEInfection;
    static void Marshall(const CDM::InfectionData& in, SEInfection& out);
    static void UnMarshall(const SEInfection& in, CDM::InfectionData& out);
    //class SEIntubation;
    static void Marshall(const CDM::IntubationData& in, SEIntubation& out);
    static void UnMarshall(const SEIntubation& in, CDM::IntubationData& out);
    //class SEMechanicalVentilation;
    static void Marshall(const CDM::MechanicalVentilationData& in, const SESubstanceManager& substances, SEMechanicalVentilation& out);
    static void UnMarshall(const SEMechanicalVentilation& in, CDM::MechanicalVentilationData& out);
    //class SENeedleDecompression;
    static void Marshall(const CDM::NeedleDecompressionData& in, SENeedleDecompression& out);
    static void UnMarshall(const SENeedleDecompression& in, CDM::NeedleDecompressionData& out);
    //class SEPainStimulus;
    static void Marshall(const CDM::PainStimulusData& in, SEPainStimulus& out);
    static void UnMarshall(const SEPainStimulus& in, CDM::PainStimulusData& out);
    //class SEPericardialEffusion;
    static void Marshall(const CDM::PericardialEffusionData& in, SEPericardialEffusion& out);
    static void UnMarshall(const SEPericardialEffusion& in, CDM::PericardialEffusionData& out);
    //class SEPulmonaryShunt;
    static void Marshall(const CDM::PulmonaryShuntData& in, SEPulmonaryShunt& out);
    static void UnMarshall(const SEPulmonaryShunt& in, CDM::PulmonaryShuntData& out);
    //class SERadiationAbsorbedDose;
    static void Marshall(const CDM::RadiationAbsorbedDoseData& in, SERadiationAbsorbedDose& out);
    static void UnMarshall(const SERadiationAbsorbedDose& in, CDM::RadiationAbsorbedDoseData& out);
    //class SETensionPneumothorax;
    static void Marshall(const CDM::TensionPneumothoraxData& in, SETensionPneumothorax& out);
    static void UnMarshall(const SETensionPneumothorax& in, CDM::TensionPneumothoraxData& out);
    //class SESubstanceAdministration;
    static void Marshall(const CDM::SubstanceAdministrationData& in, SESubstanceAdministration& out);
    static void UnMarshall(const SESubstanceAdministration& in, CDM::SubstanceAdministrationData& out);
    //class SESubstanceBolus;
    static void Marshall(const CDM::SubstanceBolusData& in, SESubstanceBolus& out);
    static void UnMarshall(const SESubstanceBolus& in, CDM::SubstanceBolusData& out);
    //class SESubstanceBolusState;
    static void Marshall(const CDM::SubstanceBolusStateData& in, SESubstanceBolusState& out);
    static void UnMarshall(const SESubstanceBolusState& in, CDM::SubstanceBolusStateData& out);
    //class SESubstanceCompoundInfusion;
    static void Marshall(const CDM::SubstanceCompoundInfusionData& in, SESubstanceCompoundInfusion& out);
    static void UnMarshall(const SESubstanceCompoundInfusion& in, CDM::SubstanceCompoundInfusionData& out);
    //class SESubstanceInfusion;
    static void Marshall(const CDM::SubstanceInfusionData& in, SESubstanceInfusion& out);
    static void UnMarshall(const SESubstanceInfusion& in, CDM::SubstanceInfusionData& out);
    //class SEUrinate;
    static void Marshall(const CDM::UrinateData& in, SEUrinate& out);
    static void UnMarshall(const SEUrinate& in, CDM::UrinateData& out);
    //class SEOverride;
    static void Marshall(const CDM::OverrideData& in, SEOverride& out);
    static void UnMarshall(const SEOverride& in, CDM::OverrideData& out);
  };

  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void PatientActions::Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Clear()();
    } else {
      Marshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void PatientActions::UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    UnMarshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO
} //Namespace Biogears
