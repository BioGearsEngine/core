#include "PatientActions.h"

#include <memory>

#include "AnesthesiaActions.h"
#include "EnvironmentActions.h"
#include "InhalerActions.h"
#include "PatientNutrition.h"
#include "Property.h"
#include "Scenario.h"

#include <biogears/schema/cdm/AnesthesiaActions.hxx>
#include <biogears/schema/cdm/EnvironmentActions.hxx>
#include <biogears/schema/cdm/InhalerActions.hxx>
#include <biogears/schema/cdm/PatientActions.hxx>
#include <biogears/schema/cdm/PatientConditions.hxx>

#include <biogears/cdm/properties/SEProperties.h>

#include <biogears/cdm/patient/actions/PatientActionsEnums.h>
#include <biogears/cdm/patient/actions/SEAcuteRespiratoryDistress.h>
#include <biogears/cdm/patient/actions/SEAcuteStress.h>
#include <biogears/cdm/patient/actions/SEAirwayObstruction.h>
#include <biogears/cdm/patient/actions/SEApnea.h>
#include <biogears/cdm/patient/actions/SEAsthmaAttack.h>
#include <biogears/cdm/patient/actions/SEBrainInjury.h>
#include <biogears/cdm/patient/actions/SEBreathHold.h>
#include <biogears/cdm/patient/actions/SEBronchoconstriction.h>
#include <biogears/cdm/patient/actions/SEBurnWound.h>
#include <biogears/cdm/patient/actions/SECardiacArrest.h>
#include <biogears/cdm/patient/actions/SEChestCompression.h>
#include <biogears/cdm/patient/actions/SEChestCompressionForce.h>
#include <biogears/cdm/patient/actions/SEChestCompressionForceScale.h>
#include <biogears/cdm/patient/actions/SEChestOcclusiveDressing.h>
#include <biogears/cdm/patient/actions/SEConsciousRespiration.h>
#include <biogears/cdm/patient/actions/SEConsciousRespirationCommand.h>
#include <biogears/cdm/patient/actions/SEConsumeNutrients.h>
#include <biogears/cdm/patient/actions/SEEbola.h>
#include <biogears/cdm/patient/actions/SEEscharotomy.h>
#include <biogears/cdm/patient/actions/SEExampleAction.h>
#include <biogears/cdm/patient/actions/SEExercise.h>
#include <biogears/cdm/patient/actions/SEForcedExhale.h>
#include <biogears/cdm/patient/actions/SEForcedInhale.h>
#include <biogears/cdm/patient/actions/SEHemorrhage.h>
#include <biogears/cdm/patient/actions/SEInfection.h>
#include <biogears/cdm/patient/actions/SEIntubation.h>
#include <biogears/cdm/patient/actions/SEMechanicalVentilation.h>
#include <biogears/cdm/patient/actions/SENasalCannula.h>
#include <biogears/cdm/patient/actions/SENeedleDecompression.h>
#include <biogears/cdm/patient/actions/SEOverride.h>
#include <biogears/cdm/patient/actions/SEPainStimulus.h>
#include <biogears/cdm/patient/actions/SEPatientAction.h>
#include <biogears/cdm/patient/actions/SEPatientAssessmentRequest.h>
#include <biogears/cdm/patient/actions/SEPericardialEffusion.h>
#include <biogears/cdm/patient/actions/SEPulmonaryShunt.h>
#include <biogears/cdm/patient/actions/SEPupillaryResponse.h>
#include <biogears/cdm/patient/actions/SERadiationAbsorbedDose.h>
#include <biogears/cdm/patient/actions/SESleep.h>
#include <biogears/cdm/patient/actions/SESubstanceBolus.h>
#include <biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h>
#include <biogears/cdm/patient/actions/SESubstanceInfusion.h>
#include <biogears/cdm/patient/actions/SESubstanceNasalDose.h>
#include <biogears/cdm/patient/actions/SESubstanceOralDose.h>
#include <biogears/cdm/patient/actions/SETensionPneumothorax.h>
#include <biogears/cdm/patient/actions/SETourniquet.h>
#include <biogears/cdm/patient/actions/SEUrinate.h>
#include <biogears/cdm/patient/actions/SEUseInhaler.h>
#include <biogears/cdm/scenario/SEAdvanceTime.h>
#include <biogears/cdm/scenario/SESerializeState.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/environment/actions/SEEnvironmentChange.h>
#include <biogears/cdm/system/environment/actions/SEThermalApplication.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEAnesthesiaMachineConfiguration.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEExpiratoryValveLeak.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEExpiratoryValveObstruction.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEInspiratoryValveLeak.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEInspiratoryValveObstruction.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEMaskLeak.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEOxygenTankPressureLoss.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEOxygenWallPortPressureLoss.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SESodaLimeFailure.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SETubeCuffLeak.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEVaporizerFailure.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEVentilatorPressureLoss.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEYPieceDisconnect.h>
#include <biogears/cdm/system/equipment/Inhaler/actions/SEInhalerConfiguration.h>

#define POLYMORPHIC_MARSHALL(paramName, typeName)                               \
  if (auto typeName = dynamic_cast<SE##typeName const*>(paramName); typeName) { \
    auto typeName##Data = std::make_unique<CDM::typeName##Data>();              \
    Marshall(*typeName, *typeName##Data);                                       \
    return std::move(typeName##Data);                                           \
  }

#define POLYMORPHIC_UNMARSHALL(paramName, typeName, schema)                                        \
  if (auto typeName##Data = dynamic_cast<CDM::typeName##Data const*>(paramName); typeName##Data) { \
    auto typeName = std::make_unique<SE##typeName>();                                              \
    schema::UnMarshall(*typeName##Data, *typeName);                                                \
    return std::move(typeName);                                                                    \
  }

namespace biogears {
namespace io {
  // class SEActionList
  std::vector<std::unique_ptr<SEAction>> PatientActions::action_factory(const CDM::ActionListData& in, SESubstanceManager& substances)
  {
    std::vector<std::unique_ptr<SEAction>> r_vec;
    for (auto action_data : in.Action()) {
      r_vec.emplace_back(factory(&action_data, substances));
    }
    return std::move(r_vec);
  }

  std::unique_ptr<SEAction> PatientActions::factory(CDM::ActionData const* actionData, SESubstanceManager& substances)
  {
    if (auto advData = dynamic_cast<CDM::AdvanceTimeData const*>(actionData); advData) {
      auto a = std::make_unique<SEAdvanceTime>();
      Scenario::UnMarshall(*advData, *a);
      return a;
    }

    if (auto stData = dynamic_cast<CDM::SerializeStateData const*>(actionData); stData) {
      auto a = std::make_unique<SESerializeState>();
      Scenario::UnMarshall(*stData, *a);
      return a;
    }

    if (auto environmentActionData = dynamic_cast<CDM::EnvironmentActionData const*>(actionData); environmentActionData) {

      if (auto EnvironmentChangeData = dynamic_cast<CDM::EnvironmentChangeData const*>(environmentActionData); EnvironmentChangeData) {
        auto EnvironmentChange = std::make_unique<SEEnvironmentChange>(substances);
        EnvironmentActions::UnMarshall(*EnvironmentChangeData, *EnvironmentChange);
        return std::move(EnvironmentChange);
      }

      POLYMORPHIC_UNMARSHALL(environmentActionData, ThermalApplication, EnvironmentActions)

      throw biogears::CommonDataModelException("PatientActions::factory: Unsupported Environment Action.");
    }

    if (auto patientActionData = dynamic_cast<CDM::PatientActionData const*>(actionData); patientActionData) {
      POLYMORPHIC_UNMARSHALL(patientActionData, PatientAssessmentRequest, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, AcuteStress, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, ExampleAction, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, AirwayObstruction, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, Apnea, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, BrainInjury, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, BurnWound, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, Escharotomy, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, Bronchoconstriction, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, CardiacArrest, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, AsthmaAttack, PatientActions)

      if (auto chestCompressionData = dynamic_cast<CDM::ChestCompressionData const*>(actionData); chestCompressionData) {
        POLYMORPHIC_UNMARSHALL(chestCompressionData, ChestCompressionForce, PatientActions)
        POLYMORPHIC_UNMARSHALL(chestCompressionData, ChestCompressionForceScale, PatientActions)
      }

      POLYMORPHIC_UNMARSHALL(patientActionData, ChestOcclusiveDressing, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, ConsciousRespiration, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, ConsumeNutrients, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, Ebola, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, Ebola, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, Intubation, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, MechanicalVentilation, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, NasalCannula, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, NeedleDecompression, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, Override, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, Hemorrhage, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, PainStimulus, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, PericardialEffusion, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, Infection, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, TensionPneumothorax, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, Tourniquet, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, PulmonaryShunt, PatientActions)
      POLYMORPHIC_UNMARSHALL(patientActionData, RadiationAbsorbedDose, PatientActions)

      if (auto substanceBolusData = dynamic_cast<CDM::SubstanceBolusData const*>(actionData); substanceBolusData) {
        auto substance = substances.GetSubstance(substanceBolusData->Substance());
        if (substance == nullptr) {
          throw biogears::CommonDataModelException("PatientActions:Factory - Unknown substance : " + substanceBolusData->Substance());
        }
        auto substanceBolusaction = std::make_unique<SESubstanceBolus>(*substance);
        Scenario::UnMarshall(*substanceBolusData, *substanceBolusaction);
        return substanceBolusaction;
      }

      if (auto substanceOralDoseData = dynamic_cast<CDM::SubstanceOralDoseData const*>(actionData); substanceOralDoseData) {
        auto substance = substances.GetSubstance(substanceOralDoseData->Substance());
        if (substance == nullptr) {
          throw biogears::CommonDataModelException("PatientActions:Factory - Unknown substance : " + substanceOralDoseData->Substance());
        }
        auto od = std::make_unique<SESubstanceOralDose>(*substance);
        Scenario::UnMarshall(*substanceOralDoseData, *od);
        return od;
      }

      if (auto substanceNasalDoseData = dynamic_cast<CDM::SubstanceNasalDoseData const*>(actionData); substanceNasalDoseData) {
        auto substance = substances.GetSubstance(substanceNasalDoseData->Substance());
        if (substance == nullptr) {
          throw biogears::CommonDataModelException("PatientActions:Factory - Unknown substance : " + substanceNasalDoseData->Substance());
        }
        auto substanceNasalDoseAction = std::make_unique<SESubstanceNasalDose>(*substance);
        Scenario::UnMarshall(*substanceNasalDoseData, *substanceNasalDoseAction);
        return substanceNasalDoseAction;
      }

      if (auto substanceInfusionData = dynamic_cast<CDM::SubstanceInfusionData const*>(actionData); substanceInfusionData) {
        auto substance = substances.GetSubstance(substanceInfusionData->Substance());
        if (substance == nullptr) {
          throw biogears::CommonDataModelException("PatientActions:Factory - Unknown substance : " + substanceInfusionData->Substance());
        }
        auto substanceInfusionAction = std::make_unique<SESubstanceInfusion>(*substance);
        Scenario::UnMarshall(*substanceInfusionData, *substanceInfusionAction);
        return substanceInfusionAction;
      }

      if (auto substanceCompoundInfusionData = dynamic_cast<CDM::SubstanceCompoundInfusionData const*>(actionData); substanceCompoundInfusionData) {
        auto compound = substances.GetCompound(substanceCompoundInfusionData->SubstanceCompound());
        if (compound == nullptr) {
          throw biogears::CommonDataModelException("PatientActions:Factory - Unknown substance : " + substanceCompoundInfusionData->SubstanceCompound());
        }
        auto substanceCompoundInfusionAction = std::make_unique<SESubstanceCompoundInfusion>(*compound);
        Scenario::UnMarshall(*substanceCompoundInfusionData, *substanceCompoundInfusionAction);
        return substanceCompoundInfusionAction;
      }

      POLYMORPHIC_UNMARSHALL(patientActionData, Urinate, PatientActions)
      throw biogears::CommonDataModelException("PatientActions:Factory - Unsupported Patient Action Received.");
    } else if (auto anesthesiaMachineActionData = dynamic_cast<CDM::AnesthesiaMachineActionData const*>(actionData); anesthesiaMachineActionData) {

      if (auto AnesthesiaMachineConfigurationData = dynamic_cast<CDM::AnesthesiaMachineConfigurationData const*>(anesthesiaMachineActionData); AnesthesiaMachineConfigurationData) {
        auto AnesthesiaMachineConfiguration = std::make_unique<SEAnesthesiaMachineConfiguration>(substances);
        AnesthesiaActions::UnMarshall(*AnesthesiaMachineConfigurationData, *AnesthesiaMachineConfiguration);
        return std::move(AnesthesiaMachineConfiguration);
      }

      POLYMORPHIC_UNMARSHALL(anesthesiaMachineActionData, OxygenWallPortPressureLoss, AnesthesiaActions)
      POLYMORPHIC_UNMARSHALL(anesthesiaMachineActionData, OxygenTankPressureLoss, AnesthesiaActions)
      POLYMORPHIC_UNMARSHALL(anesthesiaMachineActionData, ExpiratoryValveLeak, AnesthesiaActions)
      POLYMORPHIC_UNMARSHALL(anesthesiaMachineActionData, ExpiratoryValveObstruction, AnesthesiaActions)
      POLYMORPHIC_UNMARSHALL(anesthesiaMachineActionData, InspiratoryValveLeak, AnesthesiaActions)
      POLYMORPHIC_UNMARSHALL(anesthesiaMachineActionData, InspiratoryValveObstruction, AnesthesiaActions)
      POLYMORPHIC_UNMARSHALL(anesthesiaMachineActionData, MaskLeak, AnesthesiaActions)
      POLYMORPHIC_UNMARSHALL(anesthesiaMachineActionData, SodaLimeFailure, AnesthesiaActions)
      POLYMORPHIC_UNMARSHALL(anesthesiaMachineActionData, TubeCuffLeak, AnesthesiaActions)
      POLYMORPHIC_UNMARSHALL(anesthesiaMachineActionData, VaporizerFailure, AnesthesiaActions)
      POLYMORPHIC_UNMARSHALL(anesthesiaMachineActionData, VentilatorPressureLoss, AnesthesiaActions)
      POLYMORPHIC_UNMARSHALL(anesthesiaMachineActionData, YPieceDisconnect, AnesthesiaActions)
      throw biogears::CommonDataModelException("PatientActions:Factory - Unsupported Anesthesia Machine Action Received.");
    }

    else if (auto inhalerActionData = dynamic_cast<CDM::InhalerActionData const*>(actionData); inhalerActionData) {
      if (auto InhalerConfigurationData = dynamic_cast<CDM::InhalerConfigurationData const*>(anesthesiaMachineActionData); InhalerConfigurationData) {
        auto InhalerConfiguration = std::make_unique<SEInhalerConfiguration>(substances);
        InhalerActions::UnMarshall(*InhalerConfigurationData, *InhalerConfiguration);
        return std::move(InhalerConfiguration);
      }
      throw biogears::CommonDataModelException("PatientActions:Factory - Unsupported Inhaler Action Received.");
    }

    throw biogears::CommonDataModelException("PatientActions:Factory - Unsupported Action Received.");
  }

  std::unique_ptr<CDM::PatientActionData> PatientActions::factory(const SEPatientAction* patientAction)
  {
    POLYMORPHIC_MARSHALL(patientAction, PatientAssessmentRequest)
    POLYMORPHIC_MARSHALL(patientAction, AcuteRespiratoryDistress)
    POLYMORPHIC_MARSHALL(patientAction, AcuteStress)
    POLYMORPHIC_MARSHALL(patientAction, AirwayObstruction)
    POLYMORPHIC_MARSHALL(patientAction, Apnea)
    POLYMORPHIC_MARSHALL(patientAction, AsthmaAttack)
    POLYMORPHIC_MARSHALL(patientAction, BrainInjury)
    POLYMORPHIC_MARSHALL(patientAction, Bronchoconstriction)
    POLYMORPHIC_MARSHALL(patientAction, BurnWound)
    POLYMORPHIC_MARSHALL(patientAction, CardiacArrest)
    POLYMORPHIC_MARSHALL(patientAction, ChestCompression)
    POLYMORPHIC_MARSHALL(patientAction, ChestCompressionForce)
    POLYMORPHIC_MARSHALL(patientAction, ChestCompressionForceScale)
    POLYMORPHIC_MARSHALL(patientAction, ChestOcclusiveDressing)
    POLYMORPHIC_MARSHALL(patientAction, Ebola)
    POLYMORPHIC_MARSHALL(patientAction, Escharotomy)
    POLYMORPHIC_MARSHALL(patientAction, ConsciousRespiration)
    POLYMORPHIC_MARSHALL(patientAction, ConsumeNutrients)
    POLYMORPHIC_MARSHALL(patientAction, Exercise)
    POLYMORPHIC_MARSHALL(patientAction, Hemorrhage)
    POLYMORPHIC_MARSHALL(patientAction, Infection)
    POLYMORPHIC_MARSHALL(patientAction, Intubation)
    POLYMORPHIC_MARSHALL(patientAction, MechanicalVentilation)
    POLYMORPHIC_MARSHALL(patientAction, NasalCannula)
    POLYMORPHIC_MARSHALL(patientAction, NeedleDecompression)
    POLYMORPHIC_MARSHALL(patientAction, PainStimulus)
    POLYMORPHIC_MARSHALL(patientAction, PericardialEffusion)
    POLYMORPHIC_MARSHALL(patientAction, PulmonaryShunt)
    POLYMORPHIC_MARSHALL(patientAction, RadiationAbsorbedDose)
    POLYMORPHIC_MARSHALL(patientAction, TensionPneumothorax)
    POLYMORPHIC_MARSHALL(patientAction, Sleep)
    POLYMORPHIC_MARSHALL(patientAction, SubstanceAdministration)
    POLYMORPHIC_MARSHALL(patientAction, SubstanceBolus)
    POLYMORPHIC_MARSHALL(patientAction, SubstanceCompoundInfusion)
    POLYMORPHIC_MARSHALL(patientAction, SubstanceInfusion)
    POLYMORPHIC_MARSHALL(patientAction, SubstanceNasalDose)
    POLYMORPHIC_MARSHALL(patientAction, SubstanceOralDose)
    POLYMORPHIC_MARSHALL(patientAction, Tourniquet)
    POLYMORPHIC_MARSHALL(patientAction, Urinate)
    POLYMORPHIC_MARSHALL(patientAction, Override)
    throw biogears::CommonDataModelException("InhalerActions::factory does not support the derived SEInhalerAction. If you are not a developer contact upstream for support.");
  }

  //----------------------------------------------------------------------------------
  // class SEPatientAction
  void PatientActions::UnMarshall(const CDM::PatientActionData& in, SEPatientAction& out)
  {
    Scenario::UnMarshall(static_cast<const CDM::ActionData&>(in), static_cast<SEAction&>(out));
  }
  void PatientActions::Marshall(const SEPatientAction& in, CDM::PatientActionData& out)
  {
    Scenario::Marshall(static_cast<const SEAction&>(in), static_cast<CDM::ActionData&>(out));
  }
  //----------------------------------------------------------------------------------
  // class SEPatientAssessmentRequest
  void PatientActions::UnMarshall(const CDM::PatientAssessmentRequestData& in, SEPatientAssessmentRequest& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    out.m_Type = in.Type();
  }
  void PatientActions::Marshall(const SEPatientAssessmentRequest& in, CDM::PatientAssessmentRequestData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    if (in.HasType()) {
      out.Type(in.m_Type);
    }
  }
  // class SEAcuteRespiratoryDistressStress;
  void PatientActions::UnMarshall(const CDM::AcuteRespiratoryDistressData& in, SEAcuteRespiratoryDistress& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.Severity(), out.GetSeverity());
  }
  void PatientActions::Marshall(const SEAcuteRespiratoryDistress& in, CDM::AcuteRespiratoryDistressData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    out.Severity(std::make_unique<CDM::Scalar0To1Data>());
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEAcuteStress
  void PatientActions::UnMarshall(const CDM::AcuteStressData& in, SEAcuteStress& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.Severity(), out.GetSeverity());
  }
  void PatientActions::Marshall(const SEAcuteStress& in, CDM::AcuteStressData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEAirwayObstruction
  void PatientActions::UnMarshall(const CDM::AirwayObstructionData& in, SEAirwayObstruction& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.Severity(), out.GetSeverity());
  }
  void PatientActions::Marshall(const SEAirwayObstruction& in, CDM::AirwayObstructionData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEApnea
  void PatientActions::UnMarshall(const CDM::ApneaData& in, SEApnea& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.Severity(), out.GetSeverity());
  }

  void PatientActions::Marshall(const SEApnea& in, CDM::ApneaData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEAsthmaAttack
  void PatientActions::UnMarshall(const CDM::AsthmaAttackData& in, SEAsthmaAttack& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.Severity(), out.GetSeverity());
  }
  void PatientActions::Marshall(const SEAsthmaAttack& in, CDM::AsthmaAttackData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEBrainInjury
  void PatientActions::UnMarshall(const CDM::BrainInjuryData& in, SEBrainInjury& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.Severity(), out.GetSeverity());
    out.m_Type = in.Type();
  }
  void PatientActions::Marshall(const SEBrainInjury& in, CDM::BrainInjuryData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
    CDM_ENUM_MARSHALL_HELPER(in, out, Type)
  }
  //----------------------------------------------------------------------------------
  // class SEBreathHold
  void PatientActions::UnMarshall(const CDM::BreathHoldData& in, SEBreathHold& out)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::ConsciousRespirationCommandData&>(in), static_cast<SEConsciousRespirationCommand&>(out));
    io::Property::UnMarshall(in.Period(), out.GetPeriod());
  }
  void PatientActions::Marshall(const SEBreathHold& in, CDM::BreathHoldData& out)

  {
    PatientActions::Marshall(static_cast<const SEConsciousRespirationCommand&>(in), static_cast<CDM::ConsciousRespirationCommandData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Period)
  }
  //----------------------------------------------------------------------------------
  // class SEBronchoconstriction
  void PatientActions::UnMarshall(const CDM::BronchoconstrictionData& in, SEBronchoconstriction& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.Severity(), out.GetSeverity());
  }
  void PatientActions::Marshall(const SEBronchoconstriction& in, CDM::BronchoconstrictionData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEBurnWound
  void PatientActions::UnMarshall(const CDM::BurnWoundData& in, SEBurnWound& out)
  {
    // TODO: UPDATE TO SUPPORT NEW MEMBERS
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    out.SetTotalBodySurfaceArea(in.TotalBodySurfaceArea().value());

    out.m_compartmentsAffected.clear();
    for (const std::string compData : in.Compartments()) {
      out.m_compartmentsAffected.push_back(compData);
    }
  }
  void PatientActions::Marshall(const SEBurnWound& in, CDM::BurnWoundData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    if (in.m_TBSA != nullptr) {
      out.TotalBodySurfaceArea(std::make_unique<CDM::Scalar0To1Data>());
      io::Property::Marshall(*in.m_TBSA, out.TotalBodySurfaceArea());
    }
    if (!in.m_compartmentsAffected.empty()) {
      for (std::string compData : in.m_compartmentsAffected) {
        out.Compartments().push_back(compData);
      }
    }
  }
  //----------------------------------------------------------------------------------
  // class SECardiacArrest
  void PatientActions::UnMarshall(const CDM::CardiacArrestData& in, SECardiacArrest& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    out.m_State = in.State();
  }
  void PatientActions::Marshall(const SECardiacArrest& in, CDM::CardiacArrestData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    out.State(in.m_State);
  }
  //----------------------------------------------------------------------------------
  // class SEChestCompression
  void PatientActions::UnMarshall(const CDM::ChestCompressionData& in, SEChestCompression& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
  }
  void PatientActions::Marshall(const SEChestCompression& in, CDM::ChestCompressionData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
  }
  //----------------------------------------------------------------------------------
  // class SEChestCompressionForce
  void PatientActions::UnMarshall(const CDM::ChestCompressionForceData& in, SEChestCompressionForce& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.Force(), out.GetForce());
  }
  void PatientActions::Marshall(const SEChestCompressionForce& in, CDM::ChestCompressionForceData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Force)
  }
  //----------------------------------------------------------------------------------
  // class SEChestCompressionForceScale
  void PatientActions::UnMarshall(const CDM::ChestCompressionForceScaleData& in, SEChestCompressionForceScale& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.ForceScale(), out.GetForceScale());
    if (in.ForcePeriod().present()) {
      io::Property::UnMarshall(in.ForcePeriod(), out.GetForcePeriod());
    }
  }
  void PatientActions::Marshall(const SEChestCompressionForceScale& in, CDM::ChestCompressionForceScaleData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, ForceScale)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ForcePeriod)
  }
  //----------------------------------------------------------------------------------
  // class SEChestOcclusiveDressing
  void PatientActions::UnMarshall(const CDM::ChestOcclusiveDressingData& in, SEChestOcclusiveDressing& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    out.m_Side = in.Side();
    out.m_State = in.State();
  }
  void PatientActions::Marshall(const SEChestOcclusiveDressing& in, CDM::ChestOcclusiveDressingData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    out.State(in.m_State);
    CDM_ENUM_MARSHALL_HELPER(in, out, Side)
  }
  //----------------------------------------------------------------------------------
  // class SEConsciousRespiration
  void PatientActions::UnMarshall(const CDM::ConsciousRespirationData& in, SEConsciousRespiration& out)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    // Set this before our super class tells us to Clear if the action wants us to keep our current data
    out.m_ClearCommands = !in.AppendToPrevious();
    out.m_ClearCommands = true;
    CDM::ConsciousRespirationCommandData* command;
    for (unsigned int i = 0; i < in.Command().size(); i++) {
      command = (CDM::ConsciousRespirationCommandData*)&in.Command()[i];

      CDM::BreathHoldData* bh = dynamic_cast<CDM::BreathHoldData*>(command);
      if (bh != nullptr) {
        PatientActions::UnMarshall(*bh, out.AddBreathHold());
        continue;
      }

      CDM::ForcedInhaleData* fi = dynamic_cast<CDM::ForcedInhaleData*>(command);
      if (fi != nullptr) {
        PatientActions::UnMarshall(*fi, out.AddForcedInhale());
        continue;
      }

      CDM::ForcedExhaleData* fe = dynamic_cast<CDM::ForcedExhaleData*>(command);
      if (fe != nullptr) {
        PatientActions::UnMarshall(*fe, out.AddForcedExhale());
        continue;
      }

      CDM::UseInhalerData* si = dynamic_cast<CDM::UseInhalerData*>(command);
      if (si != nullptr) {
        PatientActions::UnMarshall(*si, out.AddUseInhaler());
        continue;
      }
    }
  }
  void PatientActions::Marshall(const SEConsciousRespiration& in, CDM::ConsciousRespirationData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));

    out.AppendToPrevious(false);
    for (SEConsciousRespirationCommand* cmd : in.m_Commands) {
      auto cmdData = std::unique_ptr<CDM::ConsciousRespirationCommandData>();
      PatientActions::Marshall(*cmd, *cmdData);
      out.Command().push_back(std::move(cmdData));
    }
  }
  //----------------------------------------------------------------------------------
  // class SEConsciousRespirationCommand
  void PatientActions::UnMarshall(const CDM::ConsciousRespirationCommandData& in, SEConsciousRespirationCommand& out)
  {
    out.Clear();

    if (in.Comment().present()) {
      out.m_Comment = in.Comment().get();
    }
  }
  void PatientActions::Marshall(const SEConsciousRespirationCommand& in, CDM::ConsciousRespirationCommandData& out)
  {
    out.Comment(in.m_Comment);
  }
  //----------------------------------------------------------------------------------
  // class SEConsumeNutrients
  void PatientActions::UnMarshall(const CDM::ConsumeNutrientsData& in, SEConsumeNutrients& out)
  {
    out.Clear();
    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    if (in.Nutrition().present()) {
      out.GetNutrition().Load(in.Nutrition().get());
    } else if (in.NutritionFile().present()) {
      out.SetNutritionFile(in.NutritionFile().get());
    }
  }
  void PatientActions::Marshall(const SEConsumeNutrients& in, CDM::ConsumeNutrientsData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_OPTIONAL_PATIENT_NUTRITION_MARSHALL_HELPER(in, out, Nutrition)
    out.NutritionFile(in.m_NutritionFile);
  }
  //----------------------------------------------------------------------------------
  // class SEEbola
  void PatientActions::UnMarshall(const CDM::EbolaData& in, SEEbola& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    out.m_Severity = in.Severity();
  }
  void PatientActions::Marshall(const SEEbola& in, CDM::EbolaData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_ENUM_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEEscharotomy
  void PatientActions::UnMarshall(const CDM::EscharotomyData& in, SEEscharotomy& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    out.m_Location = in.Location();
  }
  void PatientActions::Marshall(const SEEscharotomy& in, CDM::EscharotomyData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    out.Location(in.m_Location);
  }
  //----------------------------------------------------------------------------------
  // class SEExampleAction
  void PatientActions::UnMarshall(const CDM::ExampleActionData& in, SEExampleAction& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.Severity(), out.GetSeverity());
  }
  void PatientActions::Marshall(const SEExampleAction& in, CDM::ExampleActionData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEForcedInhale
  void PatientActions::UnMarshall(const CDM::ForcedInhaleData& in, SEForcedInhale& out)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::ConsciousRespirationCommandData&>(in), static_cast<SEConsciousRespirationCommand&>(out));
    io::Property::UnMarshall(in.InspiratoryCapacityFraction(), out.GetInspiratoryCapacityFraction());
    io::Property::UnMarshall(in.Period(), out.GetPeriod());
  }
  void PatientActions::Marshall(const SEForcedInhale& in, CDM::ForcedInhaleData& out)
  {
    PatientActions::Marshall(static_cast<const SEConsciousRespirationCommand&>(in), static_cast<CDM::ConsciousRespirationCommandData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Period)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, InspiratoryCapacityFraction)
  }
  //----------------------------------------------------------------------------------
  // class SEForcedExhale
  void PatientActions::UnMarshall(const CDM::ForcedExhaleData& in, SEForcedExhale& out)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::ConsciousRespirationCommandData&>(in), static_cast<SEConsciousRespirationCommand&>(out));
    io::Property::UnMarshall(in.ExpiratoryReserveVolumeFraction(), out.GetExpiratoryReserveVolumeFraction());
    io::Property::UnMarshall(in.Period(), out.GetPeriod());
  }
  void PatientActions::Marshall(const SEForcedExhale& in, CDM::ForcedExhaleData& out)
  {
    PatientActions::Marshall(static_cast<const SEConsciousRespirationCommand&>(in), static_cast<CDM::ConsciousRespirationCommandData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Period)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, ExpiratoryReserveVolumeFraction)
  }

  //----------------------------------------------------------------------------------
  // class SEExercise
  void UnMarshall(const CDM::GenericExerciseData& in, SEExercise::SEGeneric& out)
  {
    io::Property::UnMarshall(in.Intensity(), out.Intensity);
    io::Property::UnMarshall(in.DesiredWorkRate(), out.DesiredWorkRate);
  }
  void UnMarshall(const CDM::CyclingExerciseData& in, SEExercise::SECycling& out)
  {
    io::Property::UnMarshall(in.Cadence(), out.CadenceCycle);
    io::Property::UnMarshall(in.Power(), out.PowerCycle);
    if (in.AddedWeight().present()) {
      io::Property::UnMarshall(in.AddedWeight().get(), out.AddedWeight);
    } else {
      out.AddedWeight.SetValue(0, MassUnit::kg);
    }
  }
  void UnMarshall(const CDM::RunningExerciseData& in, SEExercise::SERunning& out)
  {
    io::Property::UnMarshall(in.Speed(), out.SpeedRun);
    io::Property::UnMarshall(in.Incline(), out.InclineRun);
    if (in.AddedWeight().present()) {
      io::Property::UnMarshall(in.AddedWeight().get(), out.AddedWeight);
    } else {
      out.AddedWeight.SetValue(0, MassUnit::kg);
    }
  }
  void UnMarshall(const CDM::StrengthExerciseData& in, SEExercise::SEStrengthTraining& out)
  {
    io::Property::UnMarshall(in.Weight(), out.WeightStrength);
    io::Property::UnMarshall(in.Repetitions(), out.RepsStrength);
  }
  void PatientActions::UnMarshall(const CDM::ExerciseData& in, SEExercise& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    if (in.GenericExercise().present()) {
      out.m_mode = SEExercise::ExerciseType ::GENERIC;
      io::UnMarshall(in.GenericExercise().get(), out.GetGenericExercise());
    }
    if (in.CyclingExercise().present()) {
      out.m_mode = SEExercise::ExerciseType ::CYCLING;
      io::UnMarshall(in.CyclingExercise().get(), out.GetCyclingExercise());
    }
    if (in.RunningExercise().present()) {
      out.m_mode = SEExercise::ExerciseType ::RUNNING;
      io::UnMarshall(in.RunningExercise().get(), out.GetRunningExercise());
    }
    if (in.StrengthExercise().present()) {
      out.m_mode = SEExercise::ExerciseType ::STRENGTH_TRAINING;
      io::UnMarshall(in.StrengthExercise().get(), out.GetStrengthExercise());
    }
  }
  //----------------------------------------------------------------------------------
  void Marshall(const SEExercise::SEGeneric& in, CDM::GenericExerciseData& out)
  {
    if (in.Intensity.IsValid()) {
      io::Property::Marshall(in.Intensity, out.Intensity());
    }
    if (in.DesiredWorkRate.IsValid()) {
      io::Property::Marshall(in.DesiredWorkRate, out.DesiredWorkRate());
    }
  }
  void Marshall(const SEExercise::SECycling& in, CDM::CyclingExerciseData& out)
  {
    out.Cadence(std::make_unique<CDM::ScalarFrequencyData>());
    out.Power(std::make_unique<CDM::ScalarPowerData>());

    io::Property::Marshall(in.CadenceCycle, out.Cadence());
    io::Property::Marshall(in.PowerCycle, out.Power());
    if (in.AddedWeight.IsValid()) {
      io::Property::Marshall(in.AddedWeight, out.AddedWeight());
    }
  }
  void Marshall(const SEExercise::SERunning& in, CDM::RunningExerciseData& out)
  {
    out.Speed(std::make_unique<CDM::ScalarLengthPerTimeData>());
    out.Incline(std::make_unique<CDM::Scalar0To1Data>());

    io::Property::Marshall(in.SpeedRun, out.Speed());
    io::Property::Marshall(in.InclineRun, out.Incline());
    if (in.AddedWeight.IsValid()) {
      io::Property::Marshall(in.AddedWeight, out.AddedWeight());
    }
  }
  void Marshall(const SEExercise::SEStrengthTraining& in, CDM::StrengthExerciseData& out)
  {
    out.Weight(std::make_unique<CDM::ScalarMassData>());
    out.Repetitions(std::make_unique<CDM::ScalarData>());

    io::Property::Marshall(in.WeightStrength, out.Weight());
    io::Property::Marshall(in.RepsStrength, out.Repetitions());
  }
  void PatientActions::Marshall(const SEExercise& in, CDM::ExerciseData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    if (in.HasGenericExercise()) {
      auto item = std::make_unique<CDM::GenericExerciseData>();
      io::Marshall(in.m_genericExercise, *item);
      out.GenericExercise(std::move(item));
    }
    if (in.HasCyclingExercise()) {
      auto item = std::make_unique<CDM::CyclingExerciseData>();
      io::Marshall(in.m_cyclingExercise, *item);
      out.CyclingExercise(std::move(item));
    }
    if (in.HasRunningExercise()) {
      auto item = std::make_unique<CDM::RunningExerciseData>();
      io::Marshall(in.m_runningExercise, *item);
      out.RunningExercise(std::move(item));
    }
    if (in.HasStrengthExercise()) {
      auto item = std::make_unique<CDM::StrengthExerciseData>();
      io::Marshall(in.m_strengthExercise, *item);
      out.StrengthExercise(std::move(item));
    }
  }
  //----------------------------------------------------------------------------------
  // class SEHemorrhage
  void PatientActions::UnMarshall(const CDM::HemorrhageData& in, SEHemorrhage& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    out.m_Compartment = in.Compartment();
    io::Property::UnMarshall(in.InitialRate(), out.GetInitialRate());
    io::Property::UnMarshall(in.BleedResistance(), out.GetBleedResistance());
    out.SetMCIS();
  }
  void PatientActions::Marshall(const SEHemorrhage& in, CDM::HemorrhageData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    out.Compartment(in.m_Compartment);
    CDM_PROPERTY_MARSHALL_HELPER(in, out, InitialRate)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, BleedResistance)
  }
  //----------------------------------------------------------------------------------
  // class SEInfection
  void PatientActions::UnMarshall(const CDM::InfectionData& in, SEInfection& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    out.m_Location = in.Location();
    out.m_Severity = in.Severity();
    io::Property::UnMarshall(in.MinimumInhibitoryConcentration(), out.GetMinimumInhibitoryConcentration());
  }
  void PatientActions::Marshall(const SEInfection& in, CDM::InfectionData& out)
  {

    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    out.Location(in.GetLocation());
    CDM_ENUM_MARSHALL_HELPER(in, out, Severity)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, MinimumInhibitoryConcentration)
  }
  //----------------------------------------------------------------------------------
  // class SEIntubation
  void PatientActions::UnMarshall(const CDM::IntubationData& in, SEIntubation& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    out.m_Type = in.Type();
  }
  void PatientActions::Marshall(const SEIntubation& in, CDM::IntubationData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_ENUM_MARSHALL_HELPER(in, out, Type)
  }
  //----------------------------------------------------------------------------------
  // class SEMechanicalVentilation
  void PatientActions::UnMarshall(const CDM::MechanicalVentilationData& in, const SESubstanceManager& substances, SEMechanicalVentilation& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    out.SetState(in.State());
    if (in.Flow().present()) {
      io::Property::UnMarshall(in.Flow(), out.GetFlow());
    } else {
      out.GetFlow().Invalidate();
    }
    if (in.Pressure().present()) {
      io::Property::UnMarshall(in.Pressure(), out.GetPressure());
    } else {
      out.GetPressure().Invalidate();
    }

    out.m_GasFractions.clear();
    out.m_cGasFractions.clear();

    for (const CDM::SubstanceFractionData& sfData : in.GasFraction()) {
      auto sub = substances.GetSubstance(sfData.Name());
      if (sub == nullptr) {
        throw CommonDataModelException("Substance not found : " + sfData.Name());
      }
      if (sub->GetState() != CDM::enumSubstanceState::Gas) {
        throw CommonDataModelException("Substance not gas : " + sfData.Name());
      }
      auto subFrac = std::make_unique<SESubstanceFraction>(*sub);
      subFrac->Load(sfData);
      out.m_GasFractions.push_back(subFrac.get());
      out.m_cGasFractions.push_back(subFrac.release());
    }

    if (!out.IsValid()) {
      throw CommonDataModelException("Unable to UnMarshall SEMechanicalVentilation with the given MechanicalVentilationData");
    }
  }
  void PatientActions::Marshall(const SEMechanicalVentilation& in, CDM::MechanicalVentilationData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_ENUM_MARSHALL_HELPER(in, out, State)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, Flow)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, Pressure)

    for (SESubstanceFraction* sf : in.m_GasFractions) {
      out.GasFraction().push_back(std::unique_ptr<CDM::SubstanceFractionData>(sf->Unload()));
    }
  }
  //----------------------------------------------------------------------------------
  // class SENasalCannula
  void PatientActions::UnMarshall(const CDM::NasalCannulaData& in, SENasalCannula& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    io::Property::UnMarshall(in.FlowRate(), out.GetFlowRate());
  }
  void PatientActions::Marshall(const SENasalCannula& in, CDM::NasalCannulaData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, FlowRate)
  }
  //----------------------------------------------------------------------------------
  // class SENasalStates
  void PatientActions::UnMarshall(const CDM::NasalStateData& in, SENasalState& out)
  {
    out.Clear();

    io::Property::UnMarshall(in.TotalNasalDose(), out.GetTotalNasalDose());
    out.m_UnreleasedDrugMasses.clear();
    for (auto umData : in.UnreleasedDrugMasses()) {
      SEScalarMass unrelMass;
      unrelMass.Load(umData);
      out.m_UnreleasedDrugMasses.push_back(unrelMass);
    }
    out.m_ReleasedDrugMasses.clear();
    for (auto rmData : in.ReleasedDrugMasses()) {
      SEScalarMass relMass;
      relMass.Load(rmData);
      out.m_ReleasedDrugMasses.push_back(relMass);
    }
  }
  void PatientActions::Marshall(const SENasalState& in, CDM::NasalStateData& out)
  {
    CDM_PROPERTY_MARSHALL_HELPER(in, out, TotalNasalDose);
    // CDM_PROPERTY_MARSHALL_HELPER(in, out, VenaCavaConcentration);

    out.UnreleasedDrugMasses().clear();
    for (auto umData : in.m_UnreleasedDrugMasses) {
      CDM::ScalarMassData mass;
      io::Property::Marshall(umData, mass);
      out.UnreleasedDrugMasses().push_back(mass);
    }
    out.ReleasedDrugMasses().clear();
    for (auto rmData : in.m_ReleasedDrugMasses) {
      CDM::ScalarMassData mass;
      io::Property::Marshall(rmData, mass);
      out.ReleasedDrugMasses().push_back(mass);
    }
    out.Substance(in.m_Substance->GetName());
  }
  //----------------------------------------------------------------------------------
  // class SENeedleDecompression
  void PatientActions::UnMarshall(const CDM::NeedleDecompressionData& in, SENeedleDecompression& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    out.m_Side = in.Side();
    out.m_State = in.State();
  }
  void PatientActions::Marshall(const SENeedleDecompression& in, CDM::NeedleDecompressionData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    out.State(in.m_State);
    CDM_ENUM_MARSHALL_HELPER(in, out, Side)
  }
  //----------------------------------------------------------------------------------
  // class SEOverride
  void PatientActions::UnMarshall(const CDM::OverrideData& in, SEOverride& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    out.SetOverrideState(in.State());
    out.SetOverrideConformance(in.Conformant());
    if (in.ArterialBloodPHOverride().present()) {
      io::Property::UnMarshall(in.ArterialBloodPHOverride(), out.GetArterialPHOverride());
    }
    if (in.VenousBloodPHOverride().present()) {
      io::Property::UnMarshall(in.VenousBloodPHOverride(), out.GetVenousPHOverride());
    }
    if (in.CarbonDioxideSaturationOverride().present()) {
      io::Property::UnMarshall(in.CarbonDioxideSaturationOverride(), out.GetCO2SaturationOverride());
    }
    if (in.CarbonMonoxideSaturationOverride().present()) {
      io::Property::UnMarshall(in.CarbonMonoxideSaturationOverride(), out.GetCOSaturationOverride());
    }
    if (in.OxygenSaturationOverride().present()) {
      io::Property::UnMarshall(in.OxygenSaturationOverride(), out.GetO2SaturationOverride());
    }
    if (in.PhosphateOverride().present()) {
      io::Property::UnMarshall(in.PhosphateOverride(), out.GetPhosphateOverride());
    }
    if (in.WhiteBloodCellCountOverride().present()) {
      io::Property::UnMarshall(in.WhiteBloodCellCountOverride(), out.GetWBCCountOverride());
    }
    if (in.TotalBilirubinOverride().present()) {
      io::Property::UnMarshall(in.TotalBilirubinOverride(), out.GetTotalBilirubinOverride());
    }
    if (in.CalciumConcentrationOverride().present()) {
      io::Property::UnMarshall(in.CalciumConcentrationOverride(), out.GetCalciumConcentrationOverride());
    }
    if (in.GlucoseConcentrationOverride().present()) {
      io::Property::UnMarshall(in.GlucoseConcentrationOverride(), out.GetGlucoseConcentrationOverride());
    }
    if (in.LactateConcentrationOverride().present()) {
      io::Property::UnMarshall(in.LactateConcentrationOverride(), out.GetLactateConcentrationOverride());
    }
    if (in.PotassiumConcentrationOverride().present()) {
      io::Property::UnMarshall(in.PotassiumConcentrationOverride(), out.GetPotassiumConcentrationOverride());
    }
    if (in.SodiumConcentrationOverride().present()) {
      io::Property::UnMarshall(in.SodiumConcentrationOverride(), out.GetSodiumConcentrationOverride());
    }
    if (in.BloodVolumeOverride().present()) {
      io::Property::UnMarshall(in.BloodVolumeOverride(), out.GetBloodVolumeOverride());
    }
    if (in.CardiacOutputOverride().present()) {
      io::Property::UnMarshall(in.CardiacOutputOverride(), out.GetCardiacOutputOverride());
    }
    if (in.DiastolicArterialPressureOverride().present()) {
      io::Property::UnMarshall(in.DiastolicArterialPressureOverride(), out.GetDiastolicArterialPressureOverride());
    }
    if (in.MeanArterialPressureOverride().present()) {
      io::Property::UnMarshall(in.MeanArterialPressureOverride(), out.GetMAPOverride());
    }
    if (in.HeartRateOverride().present()) {
      io::Property::UnMarshall(in.HeartRateOverride(), out.GetHeartRateOverride());
    }
    if (in.HeartStrokeVolumeOverride().present()) {
      io::Property::UnMarshall(in.HeartStrokeVolumeOverride(), out.GetHeartStrokeVolumeOverride());
    }
    if (in.SystolicArterialPressureOverride().present()) {
      io::Property::UnMarshall(in.SystolicArterialPressureOverride(), out.GetSystolicArterialPressureOverride());
    }
    if (in.InsulinSynthesisRateOverride().present()) {
      io::Property::UnMarshall(in.InsulinSynthesisRateOverride(), out.GetInsulinSynthesisRateOverride());
    }
    if (in.GlucagonSynthesisRateOverride().present()) {
      io::Property::UnMarshall(in.GlucagonSynthesisRateOverride(), out.GetGlucagonSynthesisRateOverride());
    }
    if (in.AchievedExerciseLevelOverride().present()) {
      io::Property::UnMarshall(in.AchievedExerciseLevelOverride(), out.GetAchievedExerciseLevelOverride());
    }
    if (in.CoreTemperatureOverride().present()) {
      io::Property::UnMarshall(in.CoreTemperatureOverride(), out.GetCoreTemperatureOverride());
    }
    if (in.CreatinineProductionRateOverride().present()) {
      io::Property::UnMarshall(in.CreatinineProductionRateOverride(), out.GetCreatinineProductionRateOverride());
    }
    if (in.ExerciseMeanArterialPressureDeltaOverride().present()) {
      io::Property::UnMarshall(in.ExerciseMeanArterialPressureDeltaOverride(), out.GetExerciseMeanArterialPressureDeltaOverride());
    }
    if (in.FatigueLevelOverride().present()) {
      io::Property::UnMarshall(in.FatigueLevelOverride(), out.GetFatigueLevelOverride());
    }
    if (in.LactateProductionRateOverride().present()) {
      io::Property::UnMarshall(in.LactateProductionRateOverride(), out.GetLactateProductionRateOverride());
    }
    if (in.SkinTemperatureOverride().present()) {
      io::Property::UnMarshall(in.SkinTemperatureOverride(), out.GetSkinTemperatureOverride());
    }
    if (in.SweatRateOverride().present()) {
      io::Property::UnMarshall(in.SweatRateOverride(), out.GetSweatRateOverride());
    }
    if (in.TotalMetabolicRateOverride().present()) {
      io::Property::UnMarshall(in.TotalMetabolicRateOverride(), out.GetTotalMetabolicOverride());
    }
    if (in.TotalWorkRateLevelOverride().present()) {
      io::Property::UnMarshall(in.TotalWorkRateLevelOverride(), out.GetTotalWorkRateLevelOverride());
    }
    if (in.SodiumLostToSweatOverride().present()) {
      io::Property::UnMarshall(in.SodiumLostToSweatOverride(), out.GetSodiumLostToSweatOverride());
    }
    if (in.PotassiumLostToSweatOverride().present()) {
      io::Property::UnMarshall(in.PotassiumLostToSweatOverride(), out.GetPotassiumLostToSweatOverride());
    }
    if (in.ChlorideLostToSweatOverride().present()) {
      io::Property::UnMarshall(in.ChlorideLostToSweatOverride(), out.GetChlorideLostToSweatOverride());
    }
    if (in.LeftAfferentArterioleResistanceOverride().present()) {
      io::Property::UnMarshall(in.LeftAfferentArterioleResistanceOverride(), out.GetLeftAfferentArterioleResistanceOverride());
    }
    if (in.LeftGlomerularFiltrationRateOverride().present()) {
      io::Property::UnMarshall(in.LeftGlomerularFiltrationRateOverride(), out.GetLeftGlomerularFiltrationRateOverride());
    }
    if (in.LeftReabsorptionRateOverride().present()) {
      io::Property::UnMarshall(in.LeftReabsorptionRateOverride(), out.GetLeftReaborptionRateOverride());
    }
    if (in.RenalBloodFlowOverride().present()) {
      io::Property::UnMarshall(in.RenalBloodFlowOverride(), out.GetRenalBloodFlowOverride());
    }
    if (in.RenalPlasmaFlowOverride().present()) {
      io::Property::UnMarshall(in.RenalPlasmaFlowOverride(), out.GetRenalPlasmaFlowOverride());
    }
    if (in.RightAfferentArterioleResistanceOverride().present()) {
      io::Property::UnMarshall(in.RightAfferentArterioleResistanceOverride(), out.GetRightAfferentArterioleResistanceOverride());
    }
    if (in.RightGlomerularFiltrationRateOverride().present()) {
      io::Property::UnMarshall(in.RightGlomerularFiltrationRateOverride(), out.GetRightGlomerularFiltrationRateOverride());
    }
    if (in.RightReabsorptionRateOverride().present()) {
      io::Property::UnMarshall(in.RightReabsorptionRateOverride(), out.GetRightReaborptionRateOverride());
    }
    if (in.UrinationRateOverride().present()) {
      io::Property::UnMarshall(in.UrinationRateOverride(), out.GetUrinationRateOverride());
    }
    if (in.UrineProductionRateOverride().present()) {
      io::Property::UnMarshall(in.UrineProductionRateOverride(), out.GetUrineProductionRateOverride());
    }
    if (in.UrineOsmolalityOverride().present()) {
      io::Property::UnMarshall(in.UrineOsmolalityOverride(), out.GetUrineOsmolalityOverride());
    }
    if (in.UrineVolumeOverride().present()) {
      io::Property::UnMarshall(in.UrineVolumeOverride(), out.GetUrineVolumeOverride());
    }
    if (in.UrineUreaNitrogenConcentrationOverride().present()) {
      io::Property::UnMarshall(in.UrineUreaNitrogenConcentrationOverride(), out.GetUrineUreaNitrogenConcentrationOverride());
    }
    if (in.ExpiratoryFlowOverride().present()) {
      io::Property::UnMarshall(in.ExpiratoryFlowOverride(), out.GetExpiratoryFlowOverride());
    }
    if (in.InspiratoryFlowOverride().present()) {
      io::Property::UnMarshall(in.InspiratoryFlowOverride(), out.GetInspiratoryFlowOverride());
    }
    if (in.PulmonaryComplianceOverride().present()) {
      io::Property::UnMarshall(in.PulmonaryComplianceOverride(), out.GetPulmonaryComplianceOverride());
    }
    if (in.PulmonaryResistanceOverride().present()) {
      io::Property::UnMarshall(in.PulmonaryResistanceOverride(), out.GetPulmonaryResistanceOverride());
    }
    if (in.RespirationRateOverride().present()) {
      io::Property::UnMarshall(in.RespirationRateOverride(), out.GetRespirationRateOverride());
    }
    if (in.TidalVolumeOverride().present()) {
      io::Property::UnMarshall(in.TidalVolumeOverride(), out.GetTidalVolumeOverride());
    }
    if (in.TargetPulmonaryVentilationOverride().present()) {
      io::Property::UnMarshall(in.TargetPulmonaryVentilationOverride(), out.GetTargetPulmonaryVentilationOverride());
    }
    if (in.TotalAlveolarVentilationOverride().present()) {
      io::Property::UnMarshall(in.TotalAlveolarVentilationOverride(), out.GetTotalAlveolarVentilationOverride());
    }
    if (in.TotalLungVolumeOverride().present()) {
      io::Property::UnMarshall(in.TotalLungVolumeOverride(), out.GetTotalLungVolumeOverride());
    }
    if (in.TotalPulmonaryVentilationOverride().present()) {
      io::Property::UnMarshall(in.TotalPulmonaryVentilationOverride(), out.GetTotalPulmonaryVentilationOverride());
    }
    if (in.ExtravascularFluidVolumeOverride().present()) {
      io::Property::UnMarshall(in.ExtravascularFluidVolumeOverride(), out.GetExtravascularFluidVolumeOverride());
    }
    if (in.IntracellularFluidVolumeOverride().present()) {
      io::Property::UnMarshall(in.IntracellularFluidVolumeOverride(), out.GetIntracellularFluidVolumeOverride());
    }
    if (in.LiverGlycogenOverride().present()) {
      io::Property::UnMarshall(in.LiverGlycogenOverride(), out.GetLiverGlycogenOverride());
    }
    if (in.MuscleGlycogenOverride().present()) {
      io::Property::UnMarshall(in.MuscleGlycogenOverride(), out.GetMuscleGlycogenOverride());
    }
    if (in.StoredProteinOverride().present()) {
      io::Property::UnMarshall(in.StoredProteinOverride(), out.GetStoredProteinOverride());
    }
    if (in.StoredFatOverride().present()) {
      io::Property::UnMarshall(in.StoredFatOverride(), out.GetStoredFatOverride());
    }

    if (!out.IsValid()) {
      throw CommonDataModelException("Unable to UnMarshall SEOverride from the provided OverrideData");
    }
  }
  void PatientActions::Marshall(const SEOverride& in, CDM::OverrideData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    if (in.HasOverrideState()) {
      out.State((in.m_OverrideState) ? CDM::enumOnOff::On : CDM::enumOnOff::Off);
    }
    if (in.HasOverrideConformance()) {
      out.Conformant((in.m_OverrideConformance) ? CDM::enumOnOff::On : CDM::enumOnOff::Off);
    }
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ArterialBloodPHOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, VenousBloodPHOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CarbonDioxideSaturationOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CarbonMonoxideSaturationOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, OxygenSaturationOverride)
    // GetBloodVolumeOverride
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PhosphateOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, WhiteBloodCellCountOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TotalBilirubinOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CalciumConcentrationOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, GlucoseConcentrationOverride)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LactateConcentrationOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PotassiumConcentrationOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SodiumConcentrationOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, BloodVolumeOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CardiacOutputOverride)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, DiastolicArterialPressureOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MeanArterialPressureOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, HeartRateOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, HeartStrokeVolumeOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SystolicArterialPressureOverride)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, InsulinSynthesisRateOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, GlucagonSynthesisRateOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, AchievedExerciseLevelOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CoreTemperatureOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CreatinineProductionRateOverride)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ExerciseMeanArterialPressureDeltaOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, FatigueLevelOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LactateProductionRateOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SkinTemperatureOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SweatRateOverride)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TotalMetabolicRateOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TotalWorkRateLevelOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SodiumLostToSweatOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PotassiumLostToSweatOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ChlorideLostToSweatOverride)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftAfferentArterioleResistanceOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftGlomerularFiltrationRateOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftReabsorptionRateOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RenalBloodFlowOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RenalPlasmaFlowOverride)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightAfferentArterioleResistanceOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightGlomerularFiltrationRateOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightReabsorptionRateOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, UrineProductionRateOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, UrineOsmolalityOverride)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, UrineVolumeOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, UrineUreaNitrogenConcentrationOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ExpiratoryFlowOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, InspiratoryFlowOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PulmonaryComplianceOverride)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PulmonaryResistanceOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RespirationRateOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TidalVolumeOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TargetPulmonaryVentilationOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TotalAlveolarVentilationOverride)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TotalLungVolumeOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TotalPulmonaryVentilationOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ExtravascularFluidVolumeOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, IntracellularFluidVolumeOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LiverGlycogenOverride)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MuscleGlycogenOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, StoredProteinOverride)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, StoredFatOverride)
  }

  //----------------------------------------------------------------------------------
  // class SEPainStimulus
  void PatientActions::UnMarshall(const CDM::PainStimulusData& in, SEPainStimulus& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.Severity(), out.GetSeverity());
    out.m_Location = in.Location();
  }
  void PatientActions::Marshall(const SEPainStimulus& in, CDM::PainStimulusData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));

    out.Location(in.m_Location);
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEPericardialEffusion
  void PatientActions::UnMarshall(const CDM::PericardialEffusionData& in, SEPericardialEffusion& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.EffusionRate(), out.GetEffusionRate());
  }
  void PatientActions::Marshall(const SEPericardialEffusion& in, CDM::PericardialEffusionData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, EffusionRate)
  }
  //----------------------------------------------------------------------------------
  // class SEPulmonaryShunt
  void PatientActions::UnMarshall(const CDM::PulmonaryShuntData& in, SEPulmonaryShunt& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.FlowRateScaling(), out.GetFlowRateScale());
  }
  void PatientActions::Marshall(const SEPulmonaryShunt& in, CDM::PulmonaryShuntData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, FlowRateScaling)
  }
  //----------------------------------------------------------------------------------
  // class SERadiationAbsorbedDose
  void PatientActions::UnMarshall(const CDM::RadiationAbsorbedDoseData& in, SERadiationAbsorbedDose& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.RadiationDose(), out.GetDose());
  }
  void PatientActions::Marshall(const SERadiationAbsorbedDose& in, CDM::RadiationAbsorbedDoseData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, RadiationDose)
  }
  //----------------------------------------------------------------------------------
  // class SESubstanceAdministration
  void PatientActions::UnMarshall(const CDM::SubstanceAdministrationData& in, SESubstanceAdministration& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
  }
  void PatientActions::Marshall(const SESubstanceAdministration& in, CDM::SubstanceAdministrationData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
  }
  //----------------------------------------------------------------------------------
  // class SESubstanceBolus
  void PatientActions::UnMarshall(const CDM::SubstanceBolusData& in, SESubstanceBolus& out)
  {
    Scenario::UnMarshall(static_cast<const CDM::SubstanceAdministrationData&>(in), static_cast<SESubstanceAdministration&>(out));
    io::Property::UnMarshall(in.Dose(), out.GetDose());
    io::Property::UnMarshall(in.Concentration(), out.GetConcentration());
    io::Property::UnMarshall(in.AdminTime(), out.GetAdminTime());
    out.m_AdminRoute = in.AdminRoute();
  }
  void PatientActions::Marshall(const SESubstanceBolus& in, CDM::SubstanceBolusData& out)
  {
    Scenario::Marshall(static_cast<const SESubstanceAdministration&>(in), static_cast<CDM::SubstanceAdministrationData&>(out));
    CDM_ENUM_MARSHALL_HELPER(in, out, AdminRoute)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Dose)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Concentration)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, AdminTime)
    out.Substance(in.m_Substance.GetName());
  }
  //----------------------------------------------------------------------------------
  // class SESubstanceBolusState
  void PatientActions::UnMarshall(const CDM::SubstanceBolusStateData& in, SESubstanceBolusState& out)
  {
    // TODO: Need to pass SubstanceManager to populate Substance based on name
    // NOTE: This might require us to throw exception
    // NOTE: This deffintly requires a refactor to not store internal substances as references

    io::Property::UnMarshall(in.ElapsedTime(), out.m_ElapsedTime);
    io::Property::UnMarshall(in.AdministeredDose(), out.m_AdministeredDose);
  }
  void PatientActions::Marshall(const SESubstanceBolusState& in, CDM::SubstanceBolusStateData& out)
  {
    out.Substance(in.m_Substance.GetName());
    out.ElapsedTime(std::make_unique<CDM::ScalarTimeData>());
    io::Property::Marshall(in.m_ElapsedTime, out.ElapsedTime());
    out.AdministeredDose(std::make_unique<CDM::ScalarVolumeData>());
    io::Property::Marshall(in.m_AdministeredDose, out.AdministeredDose());
  }
  //----------------------------------------------------------------------------------
  // class SESubstanceCompoundInfusion
  void PatientActions::UnMarshall(const CDM::SubstanceCompoundInfusionData& in, SESubstanceCompoundInfusion& out)
  {
    Scenario::UnMarshall(static_cast<const CDM::SubstanceAdministrationData&>(in), static_cast<SESubstanceAdministration&>(out));
    io::Property::UnMarshall(in.Rate(), out.GetRate());
    io::Property::UnMarshall(in.BagVolume(), out.GetBagVolume());
  }
  void PatientActions::Marshall(const SESubstanceCompoundInfusion& in, CDM::SubstanceCompoundInfusionData& out)
  {
    Scenario::Marshall(static_cast<const SESubstanceAdministration&>(in), static_cast<CDM::SubstanceAdministrationData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Rate)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, BagVolume)
    out.SubstanceCompound(in.m_Compound.GetName());
  }
  //----------------------------------------------------------------------------------
  // class SESubstanceInfusion
  void PatientActions::UnMarshall(const CDM::SubstanceInfusionData& in, SESubstanceInfusion& out)
  {
    Scenario::UnMarshall(static_cast<const CDM::SubstanceAdministrationData&>(in), static_cast<SESubstanceAdministration&>(out));
    io::Property::UnMarshall(in.Rate(), out.GetRate());
    io::Property::UnMarshall(in.Concentration(), out.GetConcentration());
  }
  void PatientActions::Marshall(const SESubstanceInfusion& in, CDM::SubstanceInfusionData& out)
  {
    Scenario::Marshall(static_cast<const SESubstanceAdministration&>(in), static_cast<CDM::SubstanceAdministrationData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Rate)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Concentration)
    out.Substance(in.m_Substance.GetName());
  }
//----------------------------------------------------------------------------------
  // class SESleep
  void PatientActions::UnMarshall(const CDM::SleepData& in, SESleep& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    out.m_SleepState = in.Sleep();
  }
  void PatientActions::Marshall(const SESleep& in, CDM::SleepData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    out.Sleep(in.GetSleepState());
  }
  //----------------------------------------------------------------------------------
  // class SESubstanceNasalDose
  void PatientActions::UnMarshall(const CDM::SubstanceNasalDoseData& in, SESubstanceNasalDose& out)
  {
    out.Clear();
    PatientActions::UnMarshall(static_cast<CDM::SubstanceAdministrationData const&>(in), static_cast<SESubstanceAdministration&>(out));

    io::PatientActions::UnMarshall((CDM::SubstanceAdministrationData const&)in, (SESubstanceAdministration&)out);
    io::Property::UnMarshall(in.Dose(), out.GetDose());
  }
  void PatientActions::Marshall(const SESubstanceNasalDose& in, CDM::SubstanceNasalDoseData& out)
  {
    io::PatientActions::Marshall(static_cast<SESubstanceAdministration const&>(in), static_cast<CDM::SubstanceAdministrationData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Dose);
  }
  //----------------------------------------------------------------------------------
  // class SESubstanceOralDose
  void PatientActions::UnMarshall(const CDM::SubstanceOralDoseData& in, SESubstanceOralDose& out)
  {
    out.Clear();
    io::PatientActions::UnMarshall((CDM::SubstanceAdministrationData const&)in, (SESubstanceAdministration&)out);
    io::Property::UnMarshall(in.Dose(), out.GetDose());
    out.m_AdminRoute = in.AdminRoute();
  }
  void PatientActions::Marshall(const SESubstanceOralDose& in, CDM::SubstanceOralDoseData& out)
  {
    Scenario::Marshall(static_cast<const SESubstanceAdministration&>(in), static_cast<CDM::SubstanceAdministrationData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Dose)

    out.AdminRoute(in.GetAdminRoute());
    out.Substance(in.m_Substance.GetName());
  }
  //----------------------------------------------------------------------------------
  // class SETensionPneumothorax
  void PatientActions::UnMarshall(const CDM::TensionPneumothoraxData& in, SETensionPneumothorax& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.Severity(), out.GetSeverity());
    out.m_Type = in.Type();
    out.m_Side = in.Side();
  }
  void PatientActions::Marshall(const SETensionPneumothorax& in, CDM::TensionPneumothoraxData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
    CDM_ENUM_MARSHALL_HELPER(in, out, Type)
    CDM_ENUM_MARSHALL_HELPER(in, out, Side)
  }
  //----------------------------------------------------------------------------------
  // class SETransmucosalStates
  void PatientActions::UnMarshall(const CDM::TransmucosalStateData& in, SETransmucosalState& out)
  {
    out.Clear();

    io::Property::UnMarshall(in.MouthSolidMass(), out.GetMouthSolidMass());
    io::Property::UnMarshall(in.SalivaConcentration(), out.GetSalivaConcentration());

    out.m_BuccalConcentrations.clear();
    for (auto brData : in.BuccalConcentrations()) {
      SEScalarMassPerVolume buc;
      io::Property::UnMarshall(brData, buc);
      out.m_BuccalConcentrations.push_back(buc);
    }
    out.m_SublingualConcentrations.clear();
    for (auto slData : in.SublingualConcentrations()) {
      SEScalarMassPerVolume s1;
      io::Property::UnMarshall(slData, s1);
      out.m_SublingualConcentrations.push_back(s1);
    }
  }
  void PatientActions::Marshall(const SETransmucosalState& in, CDM::TransmucosalStateData& out)
  {
    out.MouthSolidMass(std::unique_ptr<CDM::ScalarMassData>(in.m_MouthSolidMass->Unload()));
    out.SalivaConcentration(std::unique_ptr<CDM::ScalarMassPerVolumeData>(in.m_SalivaConcentration->Unload()));
    for (auto bcData : in.m_BuccalConcentrations) {
      out.BuccalConcentrations().push_back(std::unique_ptr<CDM::ScalarMassPerVolumeData>(bcData.Unload()));
    }
    for (auto slData : in.m_SublingualConcentrations) {
      out.SublingualConcentrations().push_back(std::unique_ptr<CDM::ScalarMassPerVolumeData>(slData.Unload()));
    }
    out.Substance(in.m_Substance->GetName());
  }
  //----------------------------------------------------------------------------------
  // class SETourniquet;
  void PatientActions::UnMarshall(const CDM::TourniquetData& in, SETourniquet& out)
  {
    out.Clear();

    io::PatientActions::UnMarshall((CDM::TourniquetData const&)in, (SEPatientAction&)out);
    out.m_Compartment = in.Compartment();
    out.m_TourniquetLevel = in.TourniquetLevel();
  }
  void PatientActions::Marshall(const SETourniquet& in, CDM::TourniquetData& out)
  {
    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));

    out.Compartment(in.m_Compartment);
    out.TourniquetLevel(in.m_TourniquetLevel);
  }
  //----------------------------------------------------------------------------------
  // class SEUrinate
  void PatientActions::UnMarshall(const CDM::UrinateData& in, SEUrinate& out)
  {
    out.Clear();

    Scenario::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
  }
  void PatientActions::Marshall(const SEUrinate& in, CDM::UrinateData& out)
  {

    Scenario::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
  }
  //----------------------------------------------------------------------------------
  // class SEUseInhaler
  void PatientActions::UnMarshall(const CDM::UseInhalerData& in, SEUseInhaler& out)
  {
    out.Clear();
    PatientActions::UnMarshall(static_cast<const CDM::ConsciousRespirationCommandData&>(in), static_cast<SEConsciousRespirationCommand&>(out));
  }
  void PatientActions::Marshall(const SEUseInhaler& in, CDM::UseInhalerData& out)

  {
    PatientActions::Marshall(static_cast<const SEConsciousRespirationCommand&>(in), static_cast<CDM::ConsciousRespirationCommandData&>(out));
  }
}
}