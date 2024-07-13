#include "PatientActions.h"

#include <memory>
#include <random>

#include "Actions.h"
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

#include <biogears/cdm/enums/SEPatientActionsEnums.h>
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

#define STOCASTIC_POLYMORPHIC_UNMARSHALL(paramName, typeName, schema, engine)                      \
  if (auto typeName##Data = dynamic_cast<CDM::typeName##Data const*>(paramName); typeName##Data) { \
    auto typeName = std::make_unique<SE##typeName>();                                              \
    schema::UnMarshall(*typeName##Data, *typeName, engine);                                        \
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
      auto advanceTime = std::make_unique<SEAdvanceTime>();
      Actions::UnMarshall(*advData, *advanceTime);
      return advanceTime;
    }

    if (auto stData = dynamic_cast<CDM::SerializeStateData const*>(actionData); stData) {
      auto serializeState = std::make_unique<SESerializeState>();
      Actions::UnMarshall(*stData, *serializeState);
      return serializeState;
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
        PatientActions::UnMarshall(*substanceBolusData, *substanceBolusaction);
        return substanceBolusaction;
      }

      if (auto substanceOralDoseData = dynamic_cast<CDM::SubstanceOralDoseData const*>(actionData); substanceOralDoseData) {
        auto substance = substances.GetSubstance(substanceOralDoseData->Substance());
        if (substance == nullptr) {
          throw biogears::CommonDataModelException("PatientActions:Factory - Unknown substance : " + substanceOralDoseData->Substance());
        }
        auto od = std::make_unique<SESubstanceOralDose>(*substance);
        PatientActions::UnMarshall(*substanceOralDoseData, *od);
        return od;
      }

      if (auto substanceNasalDoseData = dynamic_cast<CDM::SubstanceNasalDoseData const*>(actionData); substanceNasalDoseData) {
        auto substance = substances.GetSubstance(substanceNasalDoseData->Substance());
        if (substance == nullptr) {
          throw biogears::CommonDataModelException("PatientActions:Factory - Unknown substance : " + substanceNasalDoseData->Substance());
        }
        auto substanceNasalDoseAction = std::make_unique<SESubstanceNasalDose>(*substance);
        PatientActions::UnMarshall(*substanceNasalDoseData, *substanceNasalDoseAction);
        return substanceNasalDoseAction;
      }

      if (auto substanceInfusionData = dynamic_cast<CDM::SubstanceInfusionData const*>(actionData); substanceInfusionData) {
        auto substance = substances.GetSubstance(substanceInfusionData->Substance());
        if (substance == nullptr) {
          throw biogears::CommonDataModelException("PatientActions:Factory - Unknown substance : " + substanceInfusionData->Substance());
        }
        auto substanceInfusionAction = std::make_unique<SESubstanceInfusion>(*substance);
        PatientActions::UnMarshall(*substanceInfusionData, *substanceInfusionAction);
        return substanceInfusionAction;
      }

      if (auto substanceCompoundInfusionData = dynamic_cast<CDM::SubstanceCompoundInfusionData const*>(actionData); substanceCompoundInfusionData) {
        auto compound = substances.GetCompound(substanceCompoundInfusionData->SubstanceCompound());
        if (compound == nullptr) {
          throw biogears::CommonDataModelException("PatientActions:Factory - Unknown substance : " + substanceCompoundInfusionData->SubstanceCompound());
        }
        auto substanceCompoundInfusionAction = std::make_unique<SESubstanceCompoundInfusion>(*compound);
        PatientActions::UnMarshall(*substanceCompoundInfusionData, *substanceCompoundInfusionAction);
        return substanceCompoundInfusionAction;
      }

      POLYMORPHIC_UNMARSHALL(patientActionData, Urinate, PatientActions)
      throw biogears::CommonDataModelException("PatientActions:Factory - Unsupported Patient Action Received.");

    }
    
    if (auto anesthesiaMachineActionData = dynamic_cast<CDM::AnesthesiaMachineActionData const*>(actionData); anesthesiaMachineActionData) 
    {


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

    if (auto inhalerActionData = dynamic_cast<CDM::InhalerActionData const*>(actionData); inhalerActionData) {
      if (auto InhalerConfigurationData = dynamic_cast<CDM::InhalerConfigurationData const*>(inhalerActionData); InhalerConfigurationData) {
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
    if (auto PatientAssessmentRequest = dynamic_cast<SEPatientAssessmentRequest const*>(patientAction); PatientAssessmentRequest) {
      auto PatientAssessmentRequestData = std::make_unique<CDM::PatientAssessmentRequestData>();
      Marshall(*PatientAssessmentRequest, *PatientAssessmentRequestData);
      return std::move(PatientAssessmentRequestData);
    }
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
    Actions::UnMarshall(static_cast<const CDM::ActionData&>(in), static_cast<SEAction&>(out));
  }
  void PatientActions::Marshall(const SEPatientAction& in, CDM::PatientActionData& out)
  {
    Actions::Marshall(static_cast<const SEAction&>(in), static_cast<CDM::ActionData&>(out));
  }
  //----------------------------------------------------------------------------------
  // class SEPatientAssessmentRequest
  void PatientActions::UnMarshall(const CDM::PatientAssessmentRequestData& in, SEPatientAssessmentRequest& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    PatientActions::UnMarshall(in.Type(), out.m_Type);
  }
  void PatientActions::Marshall(const SEPatientAssessmentRequest& in, CDM::PatientAssessmentRequestData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    SE_PATIENT_ACTIONS_ENUM_MARSHALL_HELPER(in, out, Type)
  }
  // class SEAcuteRespiratoryDistressStress;
  void PatientActions::UnMarshall(const CDM::AcuteRespiratoryDistressData& in, SEAcuteRespiratoryDistress& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.Severity(), out.GetSeverity(), rd);
  }
  void PatientActions::Marshall(const SEAcuteRespiratoryDistress& in, CDM::AcuteRespiratoryDistressData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEAcuteStress
  void PatientActions::UnMarshall(const CDM::AcuteStressData& in, SEAcuteStress& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.Severity(), out.GetSeverity(), rd);
  }
  void PatientActions::Marshall(const SEAcuteStress& in, CDM::AcuteStressData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEExampleAction

  void PatientActions::UnMarshall(const CDM::ExampleActionData& in, SEExampleAction& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.Severity(), out.GetSeverity(), rd);
  }

  void PatientActions::Marshall(const SEExampleAction& in, CDM::ExampleActionData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEAirwayObstruction
  void PatientActions::UnMarshall(const CDM::AirwayObstructionData& in, SEAirwayObstruction& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.Severity(), out.GetSeverity(), rd);
  }
  void PatientActions::Marshall(const SEAirwayObstruction& in, CDM::AirwayObstructionData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEApnea
  void PatientActions::UnMarshall(const CDM::ApneaData& in, SEApnea& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.Severity(), out.GetSeverity(), rd);
  }
  void PatientActions::Marshall(const SEApnea& in, CDM::ApneaData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEAsthmaAttack
  void PatientActions::UnMarshall(const CDM::AsthmaAttackData& in, SEAsthmaAttack& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.Severity(), out.GetSeverity(), rd);
  }
  void PatientActions::Marshall(const SEAsthmaAttack& in, CDM::AsthmaAttackData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEBrainInjury
  void PatientActions::UnMarshall(const CDM::BrainInjuryData& in, SEBrainInjury& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.Severity(), out.GetSeverity(), rd);
    io::PatientActions::UnMarshall(in.Type(), out.m_Type);
  }
  void PatientActions::Marshall(const SEBrainInjury& in, CDM::BrainInjuryData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
    SE_PATIENT_ACTIONS_ENUM_MARSHALL_HELPER(in, out, Type)
  }
  //----------------------------------------------------------------------------------
  // class SEBronchoconstriction
  void PatientActions::UnMarshall(const CDM::BronchoconstrictionData& in, SEBronchoconstriction& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.Severity(), out.GetSeverity(), rd);
  }
  void PatientActions::Marshall(const SEBronchoconstriction& in, CDM::BronchoconstrictionData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEBurnWound
  void PatientActions::UnMarshall(const CDM::BurnWoundData& in, SEBurnWound& out, std::default_random_engine* rd)
  {
    // TODO: UPDATE TO SUPPORT NEW MEMBERS
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    out.SetTotalBodySurfaceArea(in.TotalBodySurfaceArea().value());

    out.m_compartmentsAffected.clear();
    for (const std::string compData : in.Compartments()) {
      out.m_compartmentsAffected.push_back(compData);
    }
  }
  void PatientActions::Marshall(const SEBurnWound& in, CDM::BurnWoundData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
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
  void PatientActions::UnMarshall(const CDM::CardiacArrestData& in, SECardiacArrest& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    Property::UnMarshall(in.State(), out.m_State);
  }
  void PatientActions::Marshall(const SECardiacArrest& in, CDM::CardiacArrestData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    out.State(std::make_unique<std::remove_reference<decltype(out.State())>::type>());
    Property::Marshall(in.m_State, out.State());
  }
  //----------------------------------------------------------------------------------
  // class SEChestCompression
  void PatientActions::UnMarshall(const CDM::ChestCompressionData& in, SEChestCompression& out)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
  }
  void PatientActions::Marshall(const SEChestCompression& in, CDM::ChestCompressionData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
  }
  //----------------------------------------------------------------------------------
  // class SEChestCompressionForce
  void PatientActions::UnMarshall(const CDM::ChestCompressionForceData& in, SEChestCompressionForce& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.Force(), out.GetForce(), rd);
  }
  void PatientActions::Marshall(const SEChestCompressionForce& in, CDM::ChestCompressionForceData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Force)
  }
  //----------------------------------------------------------------------------------
  // class SEChestCompressionForceScale
  void PatientActions::UnMarshall(const CDM::ChestCompressionForceScaleData& in, SEChestCompressionForceScale& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.ForceScale(), out.GetForceScale(), rd);
    if (in.ForcePeriod().present()) {
      io::Property::UnMarshall(in.ForcePeriod(), out.GetForcePeriod(), rd);
    }
  }
  void PatientActions::Marshall(const SEChestCompressionForceScale& in, CDM::ChestCompressionForceScaleData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, ForceScale)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ForcePeriod)
  }
  //----------------------------------------------------------------------------------
  // class SEChestOcclusiveDressing
  void PatientActions::UnMarshall(const CDM::ChestOcclusiveDressingData& in, SEChestOcclusiveDressing& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    Property::UnMarshall(in.Side(), out.m_Side);
    Property::UnMarshall(in.State(), out.m_State);
  }
  void PatientActions::Marshall(const SEChestOcclusiveDressing& in, CDM::ChestOcclusiveDressingData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    out.State(std::make_unique<std::remove_reference<decltype(out.State())>::type>());

    out.State(std::make_unique<std::remove_reference<decltype(out.State())>::type>());
    io::Property::Marshall(in.m_State, out.State());

    SE_PROPERTY_ENUM_MARSHALL_HELPER(in, out, Side)
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
  // class SEEbola
  void PatientActions::UnMarshall(const CDM::EbolaData& in, SEEbola& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    PatientActions::UnMarshall(in.Severity(), out.m_Severity);
  }
  void PatientActions::Marshall(const SEEbola& in, CDM::EbolaData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    SE_PATIENT_ACTIONS_ENUM_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEEscharotomy
  void PatientActions::UnMarshall(const CDM::EscharotomyData& in, SEEscharotomy& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    out.m_Location = in.Location();
  }
  void PatientActions::Marshall(const SEEscharotomy& in, CDM::EscharotomyData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    out.Location(in.m_Location);
  }
  //----------------------------------------------------------------------------------
  // class SEForcedInhale
  void PatientActions::UnMarshall(const CDM::ForcedInhaleData& in, SEForcedInhale& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::ConsciousRespirationCommandData&>(in), static_cast<SEConsciousRespirationCommand&>(out));
    io::Property::UnMarshall(in.InspiratoryCapacityFraction(), out.GetInspiratoryCapacityFraction(), rd);
    io::Property::UnMarshall(in.Period(), out.GetPeriod(), rd);
  }
  void PatientActions::Marshall(const SEForcedInhale& in, CDM::ForcedInhaleData& out)
  {
    PatientActions::Marshall(static_cast<const SEConsciousRespirationCommand&>(in), static_cast<CDM::ConsciousRespirationCommandData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Period)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, InspiratoryCapacityFraction)
  }
  //----------------------------------------------------------------------------------
  // class SEForcedExhale
  void PatientActions::UnMarshall(const CDM::ForcedExhaleData& in, SEForcedExhale& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::ConsciousRespirationCommandData&>(in), static_cast<SEConsciousRespirationCommand&>(out));
    io::Property::UnMarshall(in.ExpiratoryReserveVolumeFraction(), out.GetExpiratoryReserveVolumeFraction(), rd);
    io::Property::UnMarshall(in.Period(), out.GetPeriod(), rd);
  }
  void PatientActions::Marshall(const SEForcedExhale& in, CDM::ForcedExhaleData& out)
  {
    PatientActions::Marshall(static_cast<const SEConsciousRespirationCommand&>(in), static_cast<CDM::ConsciousRespirationCommandData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Period)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, ExpiratoryReserveVolumeFraction)
  }
  //----------------------------------------------------------------------------------
  // class SEBreathHold
  void PatientActions::UnMarshall(const CDM::BreathHoldData& in, SEBreathHold& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::ConsciousRespirationCommandData&>(in), static_cast<SEConsciousRespirationCommand&>(out));
    io::Property::UnMarshall(in.Period(), out.GetPeriod(), rd);
  }
  void PatientActions::Marshall(const SEBreathHold& in, CDM::BreathHoldData& out)
  {
    PatientActions::Marshall(static_cast<const SEConsciousRespirationCommand&>(in), static_cast<CDM::ConsciousRespirationCommandData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Period)
  }
  //----------------------------------------------------------------------------------
  // class SEUseInhaler
  void PatientActions::UnMarshall(const CDM::UseInhalerData& in, SEUseInhaler& out, std::default_random_engine* rd)
  {
    out.Clear();
    PatientActions::UnMarshall(static_cast<const CDM::ConsciousRespirationCommandData&>(in), static_cast<SEConsciousRespirationCommand&>(out));
  }
  void PatientActions::Marshall(const SEUseInhaler& in, CDM::UseInhalerData& out)
  {
    PatientActions::Marshall(static_cast<const SEConsciousRespirationCommand&>(in), static_cast<CDM::ConsciousRespirationCommandData&>(out));
  }
  //----------------------------------------------------------------------------------
  // class SEConsciousRespiration
  void PatientActions::UnMarshall(const CDM::ConsciousRespirationData& in, SEConsciousRespiration& out, std::default_random_engine* rd)
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
      auto cmdData = std::make_unique<CDM::ConsciousRespirationCommandData>();
      PatientActions::Marshall(*cmd, *cmdData);
      out.Command().push_back(std::move(cmdData));
    }
  }
  //----------------------------------------------------------------------------------
  // class SEConsumeNutrients
  void PatientActions::UnMarshall(const CDM::ConsumeNutrientsData& in, SEConsumeNutrients& out, std::default_random_engine* rd)
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
  // class SEExercise
  void UnMarshall(const CDM::GenericExerciseData& in, SEExercise::SEGeneric& out, std::default_random_engine* rd)
  {
    io::Property::UnMarshall(in.Intensity(), out.Intensity, rd);
    io::Property::UnMarshall(in.DesiredWorkRate(), out.DesiredWorkRate, rd);
  }
  void UnMarshall(const CDM::CyclingExerciseData& in, SEExercise::SECycling& out, std::default_random_engine* rd)
  {
    io::Property::UnMarshall(in.Cadence(), out.CadenceCycle, rd);
    io::Property::UnMarshall(in.Power(), out.PowerCycle, rd);
    if (in.AddedWeight().present()) {
      io::Property::UnMarshall(in.AddedWeight().get(), out.AddedWeight, rd);
    } else {
      out.AddedWeight.SetValue(0, MassUnit::kg);
    }
  }
  void UnMarshall(const CDM::RunningExerciseData& in, SEExercise::SERunning& out, std::default_random_engine* rd)
  {
    io::Property::UnMarshall(in.Speed(), out.SpeedRun, rd);
    io::Property::UnMarshall(in.Incline(), out.InclineRun, rd);
    if (in.AddedWeight().present()) {
      io::Property::UnMarshall(in.AddedWeight().get(), out.AddedWeight, rd);
    } else {
      out.AddedWeight.SetValue(0, MassUnit::kg);
    }
  }
  void UnMarshall(const CDM::StrengthExerciseData& in, SEExercise::SEStrengthTraining& out, std::default_random_engine* rd)
  {
    io::Property::UnMarshall(in.Weight(), out.WeightStrength, rd);
    io::Property::UnMarshall(in.Repetitions(), out.RepsStrength, rd);
  }
  void PatientActions::UnMarshall(const CDM::ExerciseData& in, SEExercise& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    if (in.GenericExercise().present()) {
      out.m_mode = SEExercise::ExerciseType ::GENERIC;
      io::UnMarshall(in.GenericExercise().get(), out.GetGenericExercise(), rd);
    }
    if (in.CyclingExercise().present()) {
      out.m_mode = SEExercise::ExerciseType ::CYCLING;
      io::UnMarshall(in.CyclingExercise().get(), out.GetCyclingExercise(), rd);
    }
    if (in.RunningExercise().present()) {
      out.m_mode = SEExercise::ExerciseType ::RUNNING;
      io::UnMarshall(in.RunningExercise().get(), out.GetRunningExercise(), rd);
    }
    if (in.StrengthExercise().present()) {
      out.m_mode = SEExercise::ExerciseType ::STRENGTH_TRAINING;
      io::UnMarshall(in.StrengthExercise().get(), out.GetStrengthExercise(), rd);
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
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
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
  void PatientActions::UnMarshall(const CDM::HemorrhageData& in, SEHemorrhage& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    out.m_Compartment = in.Compartment();
    io::Property::UnMarshall(in.InitialRate(), out.GetInitialRate(), rd);
    io::Property::UnMarshall(in.BleedResistance(), out.GetBleedResistance(), rd);
    out.SetMCIS();
  }
  void PatientActions::Marshall(const SEHemorrhage& in, CDM::HemorrhageData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    out.Compartment(in.m_Compartment);
    CDM_PROPERTY_MARSHALL_HELPER(in, out, InitialRate)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, BleedResistance)
  }
  //----------------------------------------------------------------------------------
  // class SEInfection
  void PatientActions::UnMarshall(const CDM::InfectionData& in, SEInfection& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    out.m_Location = in.Location();
    io::PatientActions::UnMarshall(in.Severity(), out.m_Severity);
    io::Property::UnMarshall(in.MinimumInhibitoryConcentration(), out.GetMinimumInhibitoryConcentration(), rd);
  }
  void PatientActions::Marshall(const SEInfection& in, CDM::InfectionData& out)
  {

    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    out.Location(in.GetLocation());
    SE_PATIENT_ACTIONS_ENUM_MARSHALL_HELPER(in, out, Severity)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, MinimumInhibitoryConcentration)
  }
  //----------------------------------------------------------------------------------
  // class SEIntubation
  void PatientActions::UnMarshall(const CDM::IntubationData& in, SEIntubation& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    io::PatientActions::UnMarshall(in.Type(), out.m_Type);
  }
  void PatientActions::Marshall(const SEIntubation& in, CDM::IntubationData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    SE_PATIENT_ACTIONS_ENUM_MARSHALL_HELPER(in, out, Type)
  }
  //----------------------------------------------------------------------------------
  // class SEMechanicalVentilation
  void PatientActions::UnMarshall(const CDM::MechanicalVentilationData& in, const SESubstanceManager& substances, SEMechanicalVentilation& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    Property::UnMarshall(in.State(), out.m_State);
    if (in.Flow().present()) {
      io::Property::UnMarshall(in.Flow(), out.GetFlow(), rd);
    } else {
      out.GetFlow().Invalidate();
    }
    if (in.Pressure().present()) {
      io::Property::UnMarshall(in.Pressure(), out.GetPressure(), rd);
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
      if (sub->GetState() != SESubstanceState::Gas) {
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
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    out.State(std::make_unique<std::remove_reference<decltype(out.State())>::type>());
    Property::Marshall(in.GetState(), out.State());
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, Flow)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, Pressure)

    for (SESubstanceFraction* sf : in.m_GasFractions) {
      out.GasFraction().push_back(std::unique_ptr<CDM::SubstanceFractionData>(sf->Unload()));
    }
  }
  //----------------------------------------------------------------------------------
  // class SENasalCannula
  void PatientActions::UnMarshall(const CDM::NasalCannulaData& in, SENasalCannula& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    io::Property::UnMarshall(in.FlowRate(), out.GetFlowRate(), rd);
  }
  void PatientActions::Marshall(const SENasalCannula& in, CDM::NasalCannulaData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, FlowRate)
  }
  //----------------------------------------------------------------------------------
  // class SENasalStates
  void PatientActions::UnMarshall(const CDM::NasalStateData& in, SENasalState& out, std::default_random_engine* rd)
  {
    out.Clear();

    io::Property::UnMarshall(in.TotalNasalDose(), out.GetTotalNasalDose(), rd);
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
  void PatientActions::UnMarshall(const CDM::NeedleDecompressionData& in, SENeedleDecompression& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    ;
    Property::UnMarshall(in.Side(), out.m_Side);
    Property::UnMarshall(in.State(), out.m_State);
  }
  void PatientActions::Marshall(const SENeedleDecompression& in, CDM::NeedleDecompressionData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    out.State(std::make_unique<std::remove_reference<decltype(out.State())>::type>());
    Property::Marshall(in.m_State, out.State());

    SE_PROPERTY_ENUM_MARSHALL_HELPER(in, out, Side)
  }
  //----------------------------------------------------------------------------------
  // class SEOverride
  void PatientActions::UnMarshall(const CDM::OverrideData& in, SEOverride& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    Property::UnMarshall(in.State(), out.m_OverrideState);
    Property::UnMarshall(in.Conformant(), out.m_OverrideConformance);

    if (in.ArterialBloodPHOverride().present()) {
      io::Property::UnMarshall(in.ArterialBloodPHOverride(), out.GetArterialPHOverride(), rd);
    }
    if (in.VenousBloodPHOverride().present()) {
      io::Property::UnMarshall(in.VenousBloodPHOverride(), out.GetVenousPHOverride(), rd);
    }
    if (in.CarbonDioxideSaturationOverride().present()) {
      io::Property::UnMarshall(in.CarbonDioxideSaturationOverride(), out.GetCO2SaturationOverride(), rd);
    }
    if (in.CarbonMonoxideSaturationOverride().present()) {
      io::Property::UnMarshall(in.CarbonMonoxideSaturationOverride(), out.GetCOSaturationOverride(), rd);
    }
    if (in.OxygenSaturationOverride().present()) {
      io::Property::UnMarshall(in.OxygenSaturationOverride(), out.GetO2SaturationOverride(), rd);
    }
    if (in.PhosphateOverride().present()) {
      io::Property::UnMarshall(in.PhosphateOverride(), out.GetPhosphateOverride(), rd);
    }
    if (in.WhiteBloodCellCountOverride().present()) {
      io::Property::UnMarshall(in.WhiteBloodCellCountOverride(), out.GetWBCCountOverride(), rd);
    }
    if (in.TotalBilirubinOverride().present()) {
      io::Property::UnMarshall(in.TotalBilirubinOverride(), out.GetTotalBilirubinOverride(), rd);
    }
    if (in.CalciumConcentrationOverride().present()) {
      io::Property::UnMarshall(in.CalciumConcentrationOverride(), out.GetCalciumConcentrationOverride(), rd);
    }
    if (in.GlucoseConcentrationOverride().present()) {
      io::Property::UnMarshall(in.GlucoseConcentrationOverride(), out.GetGlucoseConcentrationOverride(), rd);
    }
    if (in.LactateConcentrationOverride().present()) {
      io::Property::UnMarshall(in.LactateConcentrationOverride(), out.GetLactateConcentrationOverride(), rd);
    }
    if (in.PotassiumConcentrationOverride().present()) {
      io::Property::UnMarshall(in.PotassiumConcentrationOverride(), out.GetPotassiumConcentrationOverride(), rd);
    }
    if (in.SodiumConcentrationOverride().present()) {
      io::Property::UnMarshall(in.SodiumConcentrationOverride(), out.GetSodiumConcentrationOverride(), rd);
    }
    if (in.BloodVolumeOverride().present()) {
      io::Property::UnMarshall(in.BloodVolumeOverride(), out.GetBloodVolumeOverride(), rd);
    }
    if (in.CardiacOutputOverride().present()) {
      io::Property::UnMarshall(in.CardiacOutputOverride(), out.GetCardiacOutputOverride(), rd);
    }
    if (in.DiastolicArterialPressureOverride().present()) {
      io::Property::UnMarshall(in.DiastolicArterialPressureOverride(), out.GetDiastolicArterialPressureOverride(), rd);
    }
    if (in.MeanArterialPressureOverride().present()) {
      io::Property::UnMarshall(in.MeanArterialPressureOverride(), out.GetMAPOverride(), rd);
    }
    if (in.HeartRateOverride().present()) {
      io::Property::UnMarshall(in.HeartRateOverride(), out.GetHeartRateOverride(), rd);
    }
    if (in.HeartStrokeVolumeOverride().present()) {
      io::Property::UnMarshall(in.HeartStrokeVolumeOverride(), out.GetHeartStrokeVolumeOverride(), rd);
    }
    if (in.SystolicArterialPressureOverride().present()) {
      io::Property::UnMarshall(in.SystolicArterialPressureOverride(), out.GetSystolicArterialPressureOverride(), rd);
    }
    if (in.InsulinSynthesisRateOverride().present()) {
      io::Property::UnMarshall(in.InsulinSynthesisRateOverride(), out.GetInsulinSynthesisRateOverride(), rd);
    }
    if (in.GlucagonSynthesisRateOverride().present()) {
      io::Property::UnMarshall(in.GlucagonSynthesisRateOverride(), out.GetGlucagonSynthesisRateOverride(), rd);
    }
    if (in.AchievedExerciseLevelOverride().present()) {
      io::Property::UnMarshall(in.AchievedExerciseLevelOverride(), out.GetAchievedExerciseLevelOverride(), rd);
    }
    if (in.CoreTemperatureOverride().present()) {
      io::Property::UnMarshall(in.CoreTemperatureOverride(), out.GetCoreTemperatureOverride(), rd);
    }
    if (in.CreatinineProductionRateOverride().present()) {
      io::Property::UnMarshall(in.CreatinineProductionRateOverride(), out.GetCreatinineProductionRateOverride(), rd);
    }
    if (in.ExerciseMeanArterialPressureDeltaOverride().present()) {
      io::Property::UnMarshall(in.ExerciseMeanArterialPressureDeltaOverride(), out.GetExerciseMeanArterialPressureDeltaOverride(), rd);
    }
    if (in.FatigueLevelOverride().present()) {
      io::Property::UnMarshall(in.FatigueLevelOverride(), out.GetFatigueLevelOverride(), rd);
    }
    if (in.LactateProductionRateOverride().present()) {
      io::Property::UnMarshall(in.LactateProductionRateOverride(), out.GetLactateProductionRateOverride(), rd);
    }
    if (in.SkinTemperatureOverride().present()) {
      io::Property::UnMarshall(in.SkinTemperatureOverride(), out.GetSkinTemperatureOverride(), rd);
    }
    if (in.SweatRateOverride().present()) {
      io::Property::UnMarshall(in.SweatRateOverride(), out.GetSweatRateOverride(), rd);
    }
    if (in.TotalMetabolicRateOverride().present()) {
      io::Property::UnMarshall(in.TotalMetabolicRateOverride(), out.GetTotalMetabolicOverride(), rd);
    }
    if (in.TotalWorkRateLevelOverride().present()) {
      io::Property::UnMarshall(in.TotalWorkRateLevelOverride(), out.GetTotalWorkRateLevelOverride(), rd);
    }
    if (in.SodiumLostToSweatOverride().present()) {
      io::Property::UnMarshall(in.SodiumLostToSweatOverride(), out.GetSodiumLostToSweatOverride(), rd);
    }
    if (in.PotassiumLostToSweatOverride().present()) {
      io::Property::UnMarshall(in.PotassiumLostToSweatOverride(), out.GetPotassiumLostToSweatOverride(), rd);
    }
    if (in.ChlorideLostToSweatOverride().present()) {
      io::Property::UnMarshall(in.ChlorideLostToSweatOverride(), out.GetChlorideLostToSweatOverride(), rd);
    }
    if (in.LeftAfferentArterioleResistanceOverride().present()) {
      io::Property::UnMarshall(in.LeftAfferentArterioleResistanceOverride(), out.GetLeftAfferentArterioleResistanceOverride(), rd);
    }
    if (in.LeftGlomerularFiltrationRateOverride().present()) {
      io::Property::UnMarshall(in.LeftGlomerularFiltrationRateOverride(), out.GetLeftGlomerularFiltrationRateOverride(), rd);
    }
    if (in.LeftReabsorptionRateOverride().present()) {
      io::Property::UnMarshall(in.LeftReabsorptionRateOverride(), out.GetLeftReaborptionRateOverride(), rd);
    }
    if (in.RenalBloodFlowOverride().present()) {
      io::Property::UnMarshall(in.RenalBloodFlowOverride(), out.GetRenalBloodFlowOverride(), rd);
    }
    if (in.RenalPlasmaFlowOverride().present()) {
      io::Property::UnMarshall(in.RenalPlasmaFlowOverride(), out.GetRenalPlasmaFlowOverride(), rd);
    }
    if (in.RightAfferentArterioleResistanceOverride().present()) {
      io::Property::UnMarshall(in.RightAfferentArterioleResistanceOverride(), out.GetRightAfferentArterioleResistanceOverride(), rd);
    }
    if (in.RightGlomerularFiltrationRateOverride().present()) {
      io::Property::UnMarshall(in.RightGlomerularFiltrationRateOverride(), out.GetRightGlomerularFiltrationRateOverride(), rd);
    }
    if (in.RightReabsorptionRateOverride().present()) {
      io::Property::UnMarshall(in.RightReabsorptionRateOverride(), out.GetRightReaborptionRateOverride(), rd);
    }
    if (in.UrinationRateOverride().present()) {
      io::Property::UnMarshall(in.UrinationRateOverride(), out.GetUrinationRateOverride(), rd);
    }
    if (in.UrineProductionRateOverride().present()) {
      io::Property::UnMarshall(in.UrineProductionRateOverride(), out.GetUrineProductionRateOverride(), rd);
    }
    if (in.UrineOsmolalityOverride().present()) {
      io::Property::UnMarshall(in.UrineOsmolalityOverride(), out.GetUrineOsmolalityOverride(), rd);
    }
    if (in.UrineVolumeOverride().present()) {
      io::Property::UnMarshall(in.UrineVolumeOverride(), out.GetUrineVolumeOverride(), rd);
    }
    if (in.UrineUreaNitrogenConcentrationOverride().present()) {
      io::Property::UnMarshall(in.UrineUreaNitrogenConcentrationOverride(), out.GetUrineUreaNitrogenConcentrationOverride(), rd);
    }
    if (in.ExpiratoryFlowOverride().present()) {
      io::Property::UnMarshall(in.ExpiratoryFlowOverride(), out.GetExpiratoryFlowOverride(), rd);
    }
    if (in.InspiratoryFlowOverride().present()) {
      io::Property::UnMarshall(in.InspiratoryFlowOverride(), out.GetInspiratoryFlowOverride(), rd);
    }
    if (in.PulmonaryComplianceOverride().present()) {
      io::Property::UnMarshall(in.PulmonaryComplianceOverride(), out.GetPulmonaryComplianceOverride(), rd);
    }
    if (in.PulmonaryResistanceOverride().present()) {
      io::Property::UnMarshall(in.PulmonaryResistanceOverride(), out.GetPulmonaryResistanceOverride(), rd);
    }
    if (in.RespirationRateOverride().present()) {
      io::Property::UnMarshall(in.RespirationRateOverride(), out.GetRespirationRateOverride(), rd);
    }
    if (in.TidalVolumeOverride().present()) {
      io::Property::UnMarshall(in.TidalVolumeOverride(), out.GetTidalVolumeOverride(), rd);
    }
    if (in.TargetPulmonaryVentilationOverride().present()) {
      io::Property::UnMarshall(in.TargetPulmonaryVentilationOverride(), out.GetTargetPulmonaryVentilationOverride(), rd);
    }
    if (in.TotalAlveolarVentilationOverride().present()) {
      io::Property::UnMarshall(in.TotalAlveolarVentilationOverride(), out.GetTotalAlveolarVentilationOverride(), rd);
    }
    if (in.TotalLungVolumeOverride().present()) {
      io::Property::UnMarshall(in.TotalLungVolumeOverride(), out.GetTotalLungVolumeOverride(), rd);
    }
    if (in.TotalPulmonaryVentilationOverride().present()) {
      io::Property::UnMarshall(in.TotalPulmonaryVentilationOverride(), out.GetTotalPulmonaryVentilationOverride(), rd);
    }
    if (in.ExtravascularFluidVolumeOverride().present()) {
      io::Property::UnMarshall(in.ExtravascularFluidVolumeOverride(), out.GetExtravascularFluidVolumeOverride(), rd);
    }
    if (in.IntracellularFluidVolumeOverride().present()) {
      io::Property::UnMarshall(in.IntracellularFluidVolumeOverride(), out.GetIntracellularFluidVolumeOverride(), rd);
    }
    if (in.LiverGlycogenOverride().present()) {
      io::Property::UnMarshall(in.LiverGlycogenOverride(), out.GetLiverGlycogenOverride(), rd);
    }
    if (in.MuscleGlycogenOverride().present()) {
      io::Property::UnMarshall(in.MuscleGlycogenOverride(), out.GetMuscleGlycogenOverride(), rd);
    }
    if (in.StoredProteinOverride().present()) {
      io::Property::UnMarshall(in.StoredProteinOverride(), out.GetStoredProteinOverride(), rd);
    }
    if (in.StoredFatOverride().present()) {
      io::Property::UnMarshall(in.StoredFatOverride(), out.GetStoredFatOverride(), rd);
    }

    if (!out.IsValid()) {
      throw CommonDataModelException("Unable to marshall SEOverride from the provided OverrideData");
    }
  }
  void PatientActions::Marshall(const SEOverride& in, CDM::OverrideData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    if (in.HasOverrideState()) {
      out.State(std::make_unique<std::remove_reference<decltype(out.State())>::type>());
      Property::Marshall(in.m_OverrideState, out.State());
    }
    if (in.HasOverrideConformance()) {
      out.Conformant(std::make_unique<std::remove_reference<decltype(out.Conformant())>::type>());
      Property::Marshall(in.m_OverrideConformance, out.Conformant());
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
  void PatientActions::UnMarshall(const CDM::PainStimulusData& in, SEPainStimulus& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.Severity(), out.GetSeverity(), rd);
    out.m_Location = in.Location();
  }
  void PatientActions::Marshall(const SEPainStimulus& in, CDM::PainStimulusData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));

    out.Location(in.m_Location);
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEPericardialEffusion
  void PatientActions::UnMarshall(const CDM::PericardialEffusionData& in, SEPericardialEffusion& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.EffusionRate(), out.GetEffusionRate(), rd);
  }
  void PatientActions::Marshall(const SEPericardialEffusion& in, CDM::PericardialEffusionData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, EffusionRate)
  }
  //----------------------------------------------------------------------------------
  // class SEPulmonaryShunt
  void PatientActions::UnMarshall(const CDM::PulmonaryShuntData& in, SEPulmonaryShunt& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.FlowRateScaling(), out.GetFlowRateScale(), rd);
  }
  void PatientActions::Marshall(const SEPulmonaryShunt& in, CDM::PulmonaryShuntData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, FlowRateScaling)
  }
  //----------------------------------------------------------------------------------
  // class SERadiationAbsorbedDose
  void PatientActions::UnMarshall(const CDM::RadiationAbsorbedDoseData& in, SERadiationAbsorbedDose& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.RadiationDose(), out.GetDose(), rd);
  }
  void PatientActions::Marshall(const SERadiationAbsorbedDose& in, CDM::RadiationAbsorbedDoseData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, RadiationDose)
  }
  //----------------------------------------------------------------------------------
  // class SESleep
  void PatientActions::UnMarshall(const CDM::SleepData& in, SESleep& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    Property::UnMarshall(in.Sleep(), out.m_SleepState);
  }
  void PatientActions::Marshall(const SESleep& in, CDM::SleepData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    out.Sleep(std::make_unique<std::remove_reference<decltype(out.Sleep())>::type>());
    Property::Marshall(in.m_SleepState, out.Sleep());
  }
  //----------------------------------------------------------------------------------
  // class SESubstanceAdministration
  void PatientActions::UnMarshall(const CDM::SubstanceAdministrationData& in, SESubstanceAdministration& out)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
  }
  void PatientActions::Marshall(const SESubstanceAdministration& in, CDM::SubstanceAdministrationData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
  }
  //----------------------------------------------------------------------------------
  // class SESubstanceBolus
  void PatientActions::UnMarshall(const CDM::SubstanceBolusData& in, SESubstanceBolus& out, std::default_random_engine* rd)
  {
    out.Clear();
    PatientActions::UnMarshall(static_cast<const CDM::SubstanceAdministrationData&>(in), static_cast<SESubstanceAdministration&>(out));
    io::Property::UnMarshall(in.Dose(), out.GetDose(), rd);
    io::Property::UnMarshall(in.Concentration(), out.GetConcentration(), rd);
    io::Property::UnMarshall(in.AdminTime(), out.GetAdminTime(), rd);
    io::PatientActions::UnMarshall(in.AdminRoute(), out.m_AdminRoute);
  }
  void PatientActions::Marshall(const SESubstanceBolus& in, CDM::SubstanceBolusData& out)
  {
    PatientActions::Marshall(static_cast<const SESubstanceAdministration&>(in), static_cast<CDM::SubstanceAdministrationData&>(out));
    SE_PATIENT_ACTIONS_ENUM_MARSHALL_HELPER(in, out, AdminRoute)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Dose)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Concentration)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, AdminTime)
    out.Substance(in.m_Substance.GetName());
  }
  //----------------------------------------------------------------------------------
  // class SESubstanceBolusState
  void PatientActions::UnMarshall(const CDM::SubstanceBolusStateData& in, SESubstanceBolusState& out, std::default_random_engine* rd)
  {
    // TODO: Need to pass SubstanceManager to populate Substance based on name
    // NOTE: This might require us to throw exception
    // NOTE: This deffintly requires a refactor to not store internal substances as references

    io::Property::UnMarshall(in.ElapsedTime(), out.m_ElapsedTime, rd);
    io::Property::UnMarshall(in.AdministeredDose(), out.m_AdministeredDose, rd);
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
  void PatientActions::UnMarshall(const CDM::SubstanceCompoundInfusionData& in, SESubstanceCompoundInfusion& out, std::default_random_engine* rd)
  {
    out.Clear();
    PatientActions::UnMarshall(static_cast<const CDM::SubstanceAdministrationData&>(in), static_cast<SESubstanceAdministration&>(out));
    io::Property::UnMarshall(in.Rate(), out.GetRate(), rd);
    io::Property::UnMarshall(in.BagVolume(), out.GetBagVolume(), rd);
  }
  void PatientActions::Marshall(const SESubstanceCompoundInfusion& in, CDM::SubstanceCompoundInfusionData& out)
  {
    PatientActions::Marshall(static_cast<const SESubstanceAdministration&>(in), static_cast<CDM::SubstanceAdministrationData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Rate)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, BagVolume)
    out.SubstanceCompound(in.m_Compound.GetName());
  }
  //----------------------------------------------------------------------------------
  // class SESubstanceInfusion
  void PatientActions::UnMarshall(const CDM::SubstanceInfusionData& in, SESubstanceInfusion& out, std::default_random_engine* rd)
  {
    out.Clear();
    PatientActions::UnMarshall(static_cast<const CDM::SubstanceAdministrationData&>(in), static_cast<SESubstanceAdministration&>(out));
    io::Property::UnMarshall(in.Rate(), out.GetRate(), rd);
    io::Property::UnMarshall(in.Concentration(), out.GetConcentration(), rd);
  }
  void PatientActions::Marshall(const SESubstanceInfusion& in, CDM::SubstanceInfusionData& out)
  {
    PatientActions::Marshall(static_cast<const SESubstanceAdministration&>(in), static_cast<CDM::SubstanceAdministrationData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Rate)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Concentration)
    out.Substance(in.m_Substance.GetName());
  }
  //----------------------------------------------------------------------------------
  // class SESubstanceNasalDose
  void PatientActions::UnMarshall(const CDM::SubstanceNasalDoseData& in, SESubstanceNasalDose& out, std::default_random_engine* rd)
  {
    out.Clear();
    PatientActions::UnMarshall(static_cast<CDM::SubstanceAdministrationData const&>(in), static_cast<SESubstanceAdministration&>(out));

    io::PatientActions::UnMarshall((CDM::SubstanceAdministrationData const&)in, (SESubstanceAdministration&)out);
    io::Property::UnMarshall(in.Dose(), out.GetDose(), rd);
  }
  void PatientActions::Marshall(const SESubstanceNasalDose& in, CDM::SubstanceNasalDoseData& out)
  {
    io::PatientActions::Marshall(static_cast<SESubstanceAdministration const&>(in), static_cast<CDM::SubstanceAdministrationData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Dose);
  }
  //----------------------------------------------------------------------------------
  // class SESubstanceOralDose
  void PatientActions::UnMarshall(const CDM::SubstanceOralDoseData& in, SESubstanceOralDose& out, std::default_random_engine* rd)
  {
    out.Clear();
    io::PatientActions::UnMarshall((CDM::SubstanceAdministrationData const&)in, (SESubstanceAdministration&)out);
    io::Property::UnMarshall(in.Dose(), out.GetDose(), rd);
    io::PatientActions::UnMarshall(in.AdminRoute(), out.m_AdminRoute);
  }
  void PatientActions::Marshall(const SESubstanceOralDose& in, CDM::SubstanceOralDoseData& out)
  {
    PatientActions::Marshall(static_cast<const SESubstanceAdministration&>(in), static_cast<CDM::SubstanceAdministrationData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Dose)

    SE_PATIENT_ACTIONS_ENUM_MARSHALL_HELPER(in, out, AdminRoute)
    out.Substance(in.m_Substance.GetName());
  }
  //----------------------------------------------------------------------------------
  // class SETensionPneumothorax
  void PatientActions::UnMarshall(const CDM::TensionPneumothoraxData& in, SETensionPneumothorax& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    io::Property::UnMarshall(in.Severity(), out.GetSeverity(), rd);

    io::PatientActions::UnMarshall(in.Type(), out.m_Type);
    io::Property::UnMarshall(in.Side(), out.m_Side);
  }
  void PatientActions::Marshall(const SETensionPneumothorax& in, CDM::TensionPneumothoraxData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
    SE_PATIENT_ACTIONS_ENUM_MARSHALL_HELPER(in, out, Type)
    SE_PROPERTY_ENUM_MARSHALL_HELPER(in, out, Side)
  }
  //----------------------------------------------------------------------------------
  // class SETransmucosalStates
  void PatientActions::UnMarshall(const CDM::TransmucosalStateData& in, SETransmucosalState& out, std::default_random_engine* rd)
  {
    out.Clear();

    io::Property::UnMarshall(in.MouthSolidMass(), out.GetMouthSolidMass(), rd);
    io::Property::UnMarshall(in.SalivaConcentration(), out.GetSalivaConcentration(), rd);

    out.m_BuccalConcentrations.clear();
    for (auto brData : in.BuccalConcentrations()) {
      SEScalarMassPerVolume buc;
      io::Property::UnMarshall(brData, buc, rd);
      out.m_BuccalConcentrations.push_back(buc);
    }
    out.m_SublingualConcentrations.clear();
    for (auto slData : in.SublingualConcentrations()) {
      SEScalarMassPerVolume s1;
      io::Property::UnMarshall(slData, s1, rd);
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
  void PatientActions::UnMarshall(const CDM::TourniquetData& in, SETourniquet& out, std::default_random_engine* rd)
  {
    out.Clear();

    io::PatientActions::UnMarshall((CDM::TourniquetData const&)in, (SEPatientAction&)out);
    out.m_Compartment = in.Compartment();
    io::PatientActions::UnMarshall(in.TourniquetLevel(), out.m_TourniquetLevel);
  }
  void PatientActions::Marshall(const SETourniquet& in, CDM::TourniquetData& out)
  {
    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));

    out.Compartment(in.m_Compartment);
    SE_PATIENT_ACTIONS_ENUM_MARSHALL_HELPER(in, out, TourniquetLevel)
  }
  //----------------------------------------------------------------------------------
  // class SEUrinate
  void PatientActions::UnMarshall(const CDM::UrinateData& in, SEUrinate& out, std::default_random_engine* rd)
  {
    out.Clear();

    PatientActions::UnMarshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
  }
  void PatientActions::Marshall(const SEUrinate& in, CDM::UrinateData& out)
  {

    PatientActions::Marshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
  }
  //----------------------------------------------------------------------------------
  // SEBrainInjuryType
  void PatientActions::UnMarshall(const CDM::enumBrainInjuryType& in, SEBrainInjuryType& out)
  {
    try {
      switch (in) {
      case CDM::enumBrainInjuryType::Diffuse:
        out = SEBrainInjuryType::Diffuse;
        break;
      case CDM::enumBrainInjuryType::LeftFocal:
        out = SEBrainInjuryType::LeftFocal;
        break;
      case CDM::enumBrainInjuryType::RightFocal:
        out = SEBrainInjuryType::RightFocal;
        break;
      default:
        out = SEBrainInjuryType::Invalid;
        break;
      }
    } catch (xsd::cxx::tree::unexpected_enumerator<char>) {
      out = SEBrainInjuryType::Invalid;
    }
  }
  void PatientActions::Marshall(const SEBrainInjuryType& in, CDM::enumBrainInjuryType& out)
  {
    switch (in) {
    case SEBrainInjuryType::Diffuse:
      out = CDM::enumBrainInjuryType::Diffuse;
      break;
    case SEBrainInjuryType::LeftFocal:
      out = CDM::enumBrainInjuryType::LeftFocal;
      break;
    case SEBrainInjuryType::RightFocal:
      out = CDM::enumBrainInjuryType::RightFocal;
      break;
    default:
      out = "";
      break;
    }
  }
  // SEBolusAdministration
  void PatientActions::UnMarshall(const CDM::enumBolusAdministration& in, SEBolusAdministration& out)
  {
    try {
      switch (in) {
      case CDM::enumBolusAdministration::Intraarterial:
        out = SEBolusAdministration::Intraarterial;
        break;
      case CDM::enumBolusAdministration::Intramuscular:
        out = SEBolusAdministration::Intramuscular;
        break;
      case CDM::enumBolusAdministration::Intravenous:
        out = SEBolusAdministration::Intravenous;
        break;
      default:
        out = SEBolusAdministration::Invalid;
        break;
      }
    } catch (xsd::cxx::tree::unexpected_enumerator<char>) {
      out = SEBolusAdministration::Invalid;
    }
  }
  void PatientActions::Marshall(const SEBolusAdministration& in, CDM::enumBolusAdministration& out)
  {
    switch (in) {
    case SEBolusAdministration::Intraarterial:
      out = CDM::enumBolusAdministration::Intraarterial;
      break;
    case SEBolusAdministration::Intramuscular:
      out = CDM::enumBolusAdministration::Intramuscular;
      break;
    case SEBolusAdministration::Intravenous:
      out = CDM::enumBolusAdministration::Intravenous;
      break;
    default:
      out = "";
      break;
    }
  }
  // SEBurnDegree
  void PatientActions::UnMarshall(const CDM::enumBurnDegree& in, SEBurnDegree& out)
  {
    try {
      switch (in) {
      case CDM::enumBurnDegree::First:
        out = SEBurnDegree::First;
        break;
      case CDM::enumBurnDegree::Second:
        out = SEBurnDegree::Second;
        break;
      case CDM::enumBurnDegree::Third:
        out = SEBurnDegree::Third;
        break;
      default:
        out = SEBurnDegree::Invalid;
        break;
      }
    } catch (xsd::cxx::tree::unexpected_enumerator<char>) {
      out = SEBurnDegree::Invalid;
    }
  }
  void PatientActions::Marshall(const SEBurnDegree& in, CDM::enumBurnDegree& out)
  {
    switch (in) {
    case SEBurnDegree::First:
      out = CDM::enumBurnDegree::First;
      break;
    case SEBurnDegree::Second:
      out = CDM::enumBurnDegree::Second;
      break;
    case SEBurnDegree::Third:
      out = CDM::enumBurnDegree::Third;
      break;
    default:
      out = "";
      break;
    }
  }
  //  SEInfectionSeverity
  void PatientActions::UnMarshall(const CDM::enumInfectionSeverity& in, SEInfectionSeverity& out)
  {
    try {
      switch (in) {
      case CDM::enumInfectionSeverity::Eliminated:
        out = SEInfectionSeverity::Eliminated;
        break;
      case CDM::enumInfectionSeverity::Mild:
        out = SEInfectionSeverity::Mild;
        break;
      case CDM::enumInfectionSeverity::Moderate:
        out = SEInfectionSeverity::Moderate;
        break;
      case CDM::enumInfectionSeverity::Severe:
        out = SEInfectionSeverity::Severe;
        break;
      default:
        out = SEInfectionSeverity::Invalid;
        break;
      }
    } catch (xsd::cxx::tree::unexpected_enumerator<char>) {
      out = SEInfectionSeverity::Invalid;
    }
  }
  void PatientActions::Marshall(const SEInfectionSeverity& in, CDM::enumInfectionSeverity& out)
  {
    switch (in) {
    case SEInfectionSeverity::Eliminated:
      out = CDM::enumInfectionSeverity::Eliminated;
      break;
    case SEInfectionSeverity::Mild:
      out = CDM::enumInfectionSeverity::Mild;
      break;
    case SEInfectionSeverity::Moderate:
      out = CDM::enumInfectionSeverity::Moderate;
      break;
    case SEInfectionSeverity::Severe:
      out = CDM::enumInfectionSeverity::Severe;
      break;
    default:
      out = "";
      break;
    }
  }
  // SEIntubationType
  void PatientActions::UnMarshall(const CDM::enumIntubationType& in, SEIntubationType& out)
  {
    try {
      switch (in) {
      case CDM::enumIntubationType::Esophageal:
        out = SEIntubationType::Esophageal;
        break;
      case CDM::enumIntubationType::LeftMainstem:
        out = SEIntubationType::LeftMainstem;
        break;
      case CDM::enumIntubationType::RightMainstem:
        out = SEIntubationType::RightMainstem;
        break;
      case CDM::enumIntubationType::Tracheal:
        out = SEIntubationType::Tracheal;
        break;
      default:
        out = SEIntubationType::Invalid;
        break;
      }
    } catch (xsd::cxx::tree::unexpected_enumerator<char>) {
      out = SEIntubationType::Invalid;
    }
  }
  void PatientActions::Marshall(const SEIntubationType& in, CDM::enumIntubationType& out)
  {
    switch (in) {
    case SEIntubationType::Esophageal:
      out = CDM::enumIntubationType::Esophageal;
      break;
    case SEIntubationType::LeftMainstem:
      out = CDM::enumIntubationType::LeftMainstem;
      break;
    case SEIntubationType::RightMainstem:
      out = CDM::enumIntubationType::RightMainstem;
      break;
    case SEIntubationType::Tracheal:
      out = CDM::enumIntubationType::Tracheal;
      break;
    default:
      out = "";
      break;
    }
  }
  // SEPatientAssessmentType
  void PatientActions::UnMarshall(const CDM::enumPatientAssessment& in, SEPatientAssessmentType& out)
  {
    try {
      switch (in) {
      case CDM::enumPatientAssessment::ArterialBloodGasAnalysis:
        out = SEPatientAssessmentType::ArterialBloodGasAnalysis;
        break;
      case CDM::enumPatientAssessment::PulmonaryFunctionTest:
        out = SEPatientAssessmentType::PulmonaryFunctionTest;
        break;
      case CDM::enumPatientAssessment::ProthrombinTime:
        out = SEPatientAssessmentType::ProthrombinTime;
        break;
      case CDM::enumPatientAssessment::PsychomotorVigilanceTask:
        out = SEPatientAssessmentType::PsychomotorVigilanceTask;
        break;
      case CDM::enumPatientAssessment::CompleteBloodCount:
        out = SEPatientAssessmentType::CompleteBloodCount;
        break;
      case CDM::enumPatientAssessment::ComprehensiveMetabolicPanel:
        out = SEPatientAssessmentType::ComprehensiveMetabolicPanel;
        break;
      case CDM::enumPatientAssessment::SequentialOrganFailureAssessment:
        out = SEPatientAssessmentType::SequentialOrganFailureAssessment;
        break;
      case CDM::enumPatientAssessment::Urinalysis:
        out = SEPatientAssessmentType::Urinalysis;
        break;
      default:
        out = SEPatientAssessmentType::Invalid;
        break;
      }
    } catch (xsd::cxx::tree::unexpected_enumerator<char>) {
      out = SEPatientAssessmentType::Invalid;
    }
  }
  void PatientActions::Marshall(const SEPatientAssessmentType& in, CDM::enumPatientAssessment& out)
  {
    switch (in) {
    case SEPatientAssessmentType::ArterialBloodGasAnalysis:
      out = CDM::enumPatientAssessment::ArterialBloodGasAnalysis;
      break;
    case SEPatientAssessmentType::PulmonaryFunctionTest:
      out = CDM::enumPatientAssessment::PulmonaryFunctionTest;
      break;
    case SEPatientAssessmentType::ProthrombinTime:
      out = CDM::enumPatientAssessment::ProthrombinTime;
      break;
    case SEPatientAssessmentType::PsychomotorVigilanceTask:
      out = CDM::enumPatientAssessment::PsychomotorVigilanceTask;
      break;
    case SEPatientAssessmentType::CompleteBloodCount:
      out = CDM::enumPatientAssessment::CompleteBloodCount;
      break;
    case SEPatientAssessmentType::ComprehensiveMetabolicPanel:
      out = CDM::enumPatientAssessment::ComprehensiveMetabolicPanel;
      break;
    case SEPatientAssessmentType::SequentialOrganFailureAssessment:
      out = CDM::enumPatientAssessment::SequentialOrganFailureAssessment;
      break;
    case SEPatientAssessmentType::Urinalysis:
      out = CDM::enumPatientAssessment::Urinalysis;
      break;
    default:
      out = "";
      break;
    }
  }
  // SEPneumothoraxType
  void PatientActions::UnMarshall(const CDM::enumPneumothoraxType& in, SEPneumothoraxType& out)
  {
    try {
      switch (in) {
      case CDM::enumPneumothoraxType::Open:
        out = SEPneumothoraxType::Open;
        break;
      case CDM::enumPneumothoraxType::Closed:
        out = SEPneumothoraxType::Closed;
        break;
      default:
        out = SEPneumothoraxType::Invalid;
        break;
      }
    } catch (xsd::cxx::tree::unexpected_enumerator<char>) {
      out = SEPneumothoraxType::Invalid;
    }
  }
  void PatientActions::Marshall(const SEPneumothoraxType& in, CDM::enumPneumothoraxType& out)
  {
    switch (in) {
    case SEPneumothoraxType::Open:
      out = CDM::enumPneumothoraxType::Open;
      break;
    case SEPneumothoraxType::Closed:
      out = CDM::enumPneumothoraxType::Closed;
      break;
    default:
      out = "";
      break;
    }
  }
  // SEOralAdministrationType
  void PatientActions::UnMarshall(const CDM::enumOralAdministration in, SEOralAdministrationType& out)
  {
    try {
      switch (in) {
      case CDM::enumOralAdministration::Transmucosal:
        out = SEOralAdministrationType::Transmucosal;
        break;
      case CDM::enumOralAdministration::Gastrointestinal:
        out = SEOralAdministrationType::Gastrointestinal;
        break;
      default:
        out = SEOralAdministrationType::Invalid;
        break;
      }
    } catch (xsd::cxx::tree::unexpected_enumerator<char>) {
      out = SEOralAdministrationType::Invalid;
    }
  }
  void PatientActions::Marshall(const SEOralAdministrationType& in, CDM::enumOralAdministration& out)
  {
    switch (in) {
    case SEOralAdministrationType::Transmucosal:
      out = CDM::enumOralAdministration::Transmucosal;
      break;
    case SEOralAdministrationType::Gastrointestinal:
      out = CDM::enumOralAdministration::Gastrointestinal;
      break;
    default:
      out = "";
      break;
    }
  }
  //  SETourniquetApplicationType
  void PatientActions::UnMarshall(const CDM::enumTourniquetApplicationLevel& in, SETourniquetApplicationType& out)
  {
    try {
      switch (in) {
      case CDM::enumTourniquetApplicationLevel::Applied:
        out = SETourniquetApplicationType::Applied;
        break;
      case CDM::enumTourniquetApplicationLevel::Misapplied:
        out = SETourniquetApplicationType::Misapplied;
        break;
      case CDM::enumTourniquetApplicationLevel::None:
        out = SETourniquetApplicationType::NotApplied;
        break;
      default:
        out = SETourniquetApplicationType::Invalid;
        break;
      }
    } catch (xsd::cxx::tree::unexpected_enumerator<char>) {
      out = SETourniquetApplicationType::Invalid;
    }
  }
  void PatientActions::Marshall(const SETourniquetApplicationType& in, CDM::enumTourniquetApplicationLevel& out)
  {
    switch (in) {
    case SETourniquetApplicationType::Applied:
      out = CDM::enumTourniquetApplicationLevel::Applied;
      break;
    case SETourniquetApplicationType::Misapplied:
      out = CDM::enumTourniquetApplicationLevel::Misapplied;
      break;
    case SETourniquetApplicationType::NotApplied:
      out = CDM::enumTourniquetApplicationLevel::None;
      break;
    default:
      out = "";
      break;
    }
  }
  //----------------------------------------------------------------------------------

}

bool operator==(CDM::enumBrainInjuryType const& lhs, SEBrainInjuryType const& rhs)
{
  switch (rhs) {
  case SEBrainInjuryType::Diffuse:
    return (CDM::enumBrainInjuryType::Diffuse == lhs);
  case SEBrainInjuryType::LeftFocal:
    return (CDM::enumBrainInjuryType::LeftFocal == lhs);
  case SEBrainInjuryType::RightFocal:
    return (CDM::enumBrainInjuryType::RightFocal == lhs);
  case SEBrainInjuryType::Invalid:
    return ((CDM::enumBrainInjuryType::value)-1 == lhs);
  default:
    return false;
  }
}
bool operator==(CDM::enumBolusAdministration const& lhs, SEBolusAdministration const& rhs)
{
  switch (rhs) {
  case SEBolusAdministration::Intraarterial:
    return (CDM::enumBolusAdministration::Intraarterial == lhs);
  case SEBolusAdministration::Intramuscular:
    return (CDM::enumBolusAdministration::Intramuscular == lhs);
  case SEBolusAdministration::Intravenous:
    return (CDM::enumBolusAdministration::Intravenous == lhs);
  case SEBolusAdministration::Invalid:
    return ((CDM::enumBolusAdministration::value)-1 == lhs);
  default:
    return false;
  }
}
bool operator==(CDM::enumBurnDegree const& lhs, SEBurnDegree const& rhs)
{
  switch (rhs) {
  case SEBurnDegree::First:
    return (CDM::enumBurnDegree::First == lhs);
  case SEBurnDegree::Second:
    return (CDM::enumBurnDegree::Second == lhs);
  case SEBurnDegree::Third:
    return (CDM::enumBurnDegree::Third == lhs);
  case SEBurnDegree::Invalid:
    return ((CDM::enumBurnDegree::value)-1 == lhs);
  default:
    return false;
  }
}
bool operator==(CDM::enumInfectionSeverity const& lhs, SEInfectionSeverity const& rhs)
{
  switch (rhs) {
  case SEInfectionSeverity::Eliminated:
    return (CDM::enumInfectionSeverity::Eliminated == lhs);
  case SEInfectionSeverity::Mild:
    return (CDM::enumInfectionSeverity::Mild == lhs);
  case SEInfectionSeverity::Moderate:
    return (CDM::enumInfectionSeverity::Moderate == lhs);
  case SEInfectionSeverity::Severe:
    return (CDM::enumInfectionSeverity::Severe == lhs);
  case SEInfectionSeverity::Invalid:
    return ((CDM::enumInfectionSeverity::value)-1 == lhs);
  default:
    return false;
  }
}
bool operator==(CDM::enumIntubationType const& lhs, SEIntubationType const& rhs)
{
  switch (rhs) {
  case SEIntubationType::Off:
    return (CDM::enumIntubationType::Off == lhs);
  case SEIntubationType::Esophageal:
    return (CDM::enumIntubationType::Esophageal == lhs);
  case SEIntubationType::LeftMainstem:
    return (CDM::enumIntubationType::LeftMainstem == lhs);
  case SEIntubationType::RightMainstem:
    return (CDM::enumIntubationType::RightMainstem == lhs);
  case SEIntubationType::Tracheal:
    return (CDM::enumIntubationType::Tracheal == lhs);
  case SEIntubationType::Invalid:
    return ((CDM::enumIntubationType::value)-1 == lhs);
  default:
    return false;
  }
}

bool operator==(CDM::enumPatientAssessment const& lhs, SEPatientAssessmentType const& rhs)
{
  switch (rhs) {
  case SEPatientAssessmentType::ArterialBloodGasAnalysis:
    return (CDM::enumPatientAssessment::ArterialBloodGasAnalysis == lhs);
  case SEPatientAssessmentType::PulmonaryFunctionTest:
    return (CDM::enumPatientAssessment::PulmonaryFunctionTest == lhs);
  case SEPatientAssessmentType::ProthrombinTime:
    return (CDM::enumPatientAssessment::ProthrombinTime == lhs);
  case SEPatientAssessmentType::PsychomotorVigilanceTask:
    return (CDM::enumPatientAssessment::PsychomotorVigilanceTask == lhs);
  case SEPatientAssessmentType::CompleteBloodCount:
    return (CDM::enumPatientAssessment::CompleteBloodCount == lhs);
  case SEPatientAssessmentType::ComprehensiveMetabolicPanel:
    return (CDM::enumPatientAssessment::ComprehensiveMetabolicPanel == lhs);
  case SEPatientAssessmentType::SequentialOrganFailureAssessment:
    return (CDM::enumPatientAssessment::SequentialOrganFailureAssessment == lhs);
  case SEPatientAssessmentType::Urinalysis:
    return (CDM::enumPatientAssessment::Urinalysis == lhs);
  case SEPatientAssessmentType::Invalid:
    return ((CDM::enumPatientAssessment::value)-1 == lhs);
  default:
    return false;
  }
}
bool operator==(CDM::enumPneumothoraxType const& lhs, SEPneumothoraxType const& rhs)
{
  switch (rhs) {
  case SEPneumothoraxType::Open:
    return (CDM::enumPneumothoraxType::Open == lhs);
  case SEPneumothoraxType::Closed:
    return (CDM::enumPneumothoraxType::Closed == lhs);
  case SEPneumothoraxType::Invalid:
    return ((CDM::enumPneumothoraxType::value)-1 == lhs);
  default:
    return false;
  }
}
bool operator==(CDM::enumOralAdministration const& lhs, SEOralAdministrationType const& rhs)
{
  switch (rhs) {
  case SEOralAdministrationType::Gastrointestinal:
    return (CDM::enumOralAdministration::Gastrointestinal == lhs);
  case SEOralAdministrationType::Transmucosal:
    return (CDM::enumOralAdministration::Transmucosal == lhs);
  case SEOralAdministrationType::Invalid:
    return ((CDM::enumOralAdministration::value)-1 == lhs);
  default:
    return false;
  }
}
bool operator==(CDM::enumTourniquetApplicationLevel const& lhs, SETourniquetApplicationType const& rhs)
{
  switch (rhs) {
  case SETourniquetApplicationType::NotApplied:
    return (CDM::enumTourniquetApplicationLevel::None == lhs);
  case SETourniquetApplicationType::Misapplied:
    return (CDM::enumTourniquetApplicationLevel::Misapplied == lhs);
  case SETourniquetApplicationType::Applied:
    return (CDM::enumTourniquetApplicationLevel::Applied == lhs);
  case SETourniquetApplicationType::Invalid:
    return ((CDM::enumTourniquetApplicationLevel::value)-1 == lhs);
  default:
    return false;
  }
}
}
