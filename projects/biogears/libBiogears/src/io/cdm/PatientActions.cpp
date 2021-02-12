#include "PatientActions.h"

#include "Property.h"
#include "Scenario.h"

#include <biogears/schema/cdm/AnesthesiaActions.hxx>
#include <biogears/schema/cdm/EnvironmentActions.hxx>
#include <biogears/schema/cdm/InhalerActions.hxx>
#include <biogears/schema/cdm/PatientActions.hxx>
#include <biogears/schema/cdm/PatientConditions.hxx>

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
#include <biogears/cdm/patient/actions/SEExercise.h>
#include <biogears/cdm/patient/actions/SEForcedExhale.h>
#include <biogears/cdm/patient/actions/SEForcedInhale.h>
#include <biogears/cdm/patient/actions/SEHemorrhage.h>
#include <biogears/cdm/patient/actions/SEInfection.h>
#include <biogears/cdm/patient/actions/SEIntubation.h>
#include <biogears/cdm/patient/actions/SEMechanicalVentilation.h>
#include <biogears/cdm/patient/actions/SENeedleDecompression.h>
#include <biogears/cdm/patient/actions/SEOverride.h>
#include <biogears/cdm/patient/actions/SEPainStimulus.h>
#include <biogears/cdm/patient/actions/SEPatientAction.h>
#include <biogears/cdm/patient/actions/SEPatientActionsEnums.h>
#include <biogears/cdm/patient/actions/SEPatientAssessmentRequest.h>
#include <biogears/cdm/patient/actions/SEPericardialEffusion.h>
#include <biogears/cdm/patient/actions/SEPupillaryResponse.h>
#include <biogears/cdm/patient/actions/SESubstanceBolus.h>
#include <biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h>
#include <biogears/cdm/patient/actions/SESubstanceInfusion.h>
#include <biogears/cdm/patient/actions/SESubstanceOralDose.h>
#include <biogears/cdm/patient/actions/SETensionPneumothorax.h>
#include <biogears/cdm/patient/actions/SEUrinate.h>
#include <biogears/cdm/patient/actions/SEUseInhaler.h>

#include <biogears/cdm/properties/SEScalarFraction.h>

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

namespace biogears {
namespace io {
  //class SEActionList
  std::vector<std::unique_ptr<SEAction>> PatientActions::action_factory(const CDM::ActionListData& in, SESubstanceManager& substances)
  {
    std::vector<std::unique_ptr<SEAction>> r_vec;
    for (const auto action_data : in.Action()) {
      r_vec.emplace_back(factory(action_data, substances));
    }
    return std::move(r_vec);
  }
  //----------------------------------------------------------------------------------
  //class SECondition
  std::unique_ptr<SEAction> PatientActions::factory(const CDM::ActionData& data, SESubstanceManager& substances)
  {
    std::stringstream ss;
    SESubstance* substance;
    SESubstanceCompound* compound;

    // Anesthesia Machine Actions
    CDM::AnesthesiaMachineConfigurationData* anConfig;
    // Anesthesia Machine Failure Action
    CDM::ExpiratoryValveLeakData* anExLeak;
    CDM::ExpiratoryValveObstructionData* anExObs;
    CDM::InspiratoryValveLeakData* anInLeak;
    CDM::InspiratoryValveObstructionData* anInObs;
    CDM::MaskLeakData* anMskLeak;
    CDM::SodaLimeFailureData* anSodaFail;
    CDM::TubeCuffLeakData* anTubLeak;
    CDM::VaporizerFailureData* anVapFail;
    CDM::VentilatorPressureLossData* anVentLoss;
    CDM::YPieceDisconnectData* anYDisc;
    // Anesthesia Machine Incidents
    CDM::OxygenWallPortPressureLossData* anO2WallLoss;
    CDM::OxygenTankPressureLossData* anO2TankLoss;

    CDM::ActionData* action = (CDM::ActionData*)&data;
    CDM::AdvanceTimeData* advData = dynamic_cast<CDM::AdvanceTimeData*>(action);
    if (advData != nullptr) {
      auto a = std::make_unique<SEAdvanceTime>();
      Scenario::Marshall(*advData, *a);
      return a;
    }

    CDM::SerializeStateData* stData = dynamic_cast<CDM::SerializeStateData*>(action);
    if (stData != nullptr) {
      auto a = std::make_unique<SESerializeState>();
      Scenario::Marshall(*stData, *a);
      return a;
    }

    if (dynamic_cast<CDM::EnvironmentActionData*>(action) != nullptr) {
      if (dynamic_cast<CDM::EnvironmentChangeData*>(action) != nullptr) {
        auto a = std::make_unique<SEEnvironmentChange>(substances);
        Scenario::Marshall(*(CDM::EnvironmentChangeData*)action, *a);
        return a;
      }
      if (dynamic_cast<CDM::ThermalApplicationData*>(action) != nullptr) {
        auto a = std::make_unique<SEThermalApplication>();
        Scenario::Marshall(*(CDM::ThermalApplicationData*)action, *a);
        return a;
      }
      substances.GetLogger()->Error("Unsupported Environment Action Received", "SEScenario::Load");
    }

    if (dynamic_cast<CDM::PatientActionData*>(action) != nullptr) {
      if (dynamic_cast<CDM::PatientAssessmentRequestData*>(action) != nullptr) {
        auto a = std::make_unique<SEPatientAssessmentRequest>();
        Scenario::Marshall(*(CDM::PatientAssessmentRequestData*)action, *a);
        return a;
      }

      CDM::AcuteStressData* aStressData = dynamic_cast<CDM::AcuteStressData*>(action);
      if (aStressData != nullptr) {
        auto a = std::make_unique<SEAcuteStress>();
        Scenario::Marshall(*aStressData, *a);
        return a;
      }

      CDM::AirwayObstructionData* airObData = dynamic_cast<CDM::AirwayObstructionData*>(action);
      if (airObData != nullptr) {
        auto a = std::make_unique<SEAirwayObstruction>();
        Scenario::Marshall(*airObData, *a);
        return a;
      }

      CDM::ApneaData* apneaData = dynamic_cast<CDM::ApneaData*>(action);
      if (apneaData != nullptr) {
        auto a = std::make_unique<SEApnea>();
        Scenario::Marshall(*apneaData, *a);
        return a;
      }

      CDM::BrainInjuryData* brainInjData = dynamic_cast<CDM::BrainInjuryData*>(action);
      if (brainInjData != nullptr) {
        auto a = std::make_unique<SEBrainInjury>();
        Scenario::Marshall(*brainInjData, *a);
        return a;
      }

      CDM::BurnWoundData* burnData = dynamic_cast<CDM::BurnWoundData*>(action);
      if (burnData != nullptr) {
        auto burn = std::make_unique<SEBurnWound>();
        Scenario::Marshall(*burnData, *burn);
        return burn;
      }

      CDM::BronchoconstrictionData* bconData = dynamic_cast<CDM::BronchoconstrictionData*>(action);
      if (bconData != nullptr) {
        auto a = std::make_unique<SEBronchoconstriction>();
        Scenario::Marshall(*bconData, *a);
        return a;
      }

      CDM::CardiacArrestData* carrestData = dynamic_cast<CDM::CardiacArrestData*>(action);
      if (carrestData != nullptr) {
        auto a = std::make_unique<SECardiacArrest>();
        Scenario::Marshall(*carrestData, *a);
        return a;
      }

      CDM::AsthmaAttackData* asthmaData = dynamic_cast<CDM::AsthmaAttackData*>(action);
      if (asthmaData != nullptr) {
        auto a = std::make_unique<SEAsthmaAttack>();
        Scenario::Marshall(*asthmaData, *a);
        return a;
      }

      CDM::ChestCompressionData* cprData = dynamic_cast<CDM::ChestCompressionData*>(action);
      if (cprData != nullptr) {
        CDM::ChestCompressionForceData* cprForce = dynamic_cast<CDM::ChestCompressionForceData*>(cprData);
        if (cprForce != nullptr) {
          auto a = std::make_unique<SEChestCompressionForce>();
          Scenario::Marshall(*cprForce, *a);
          return a;
        }
        CDM::ChestCompressionForceScaleData* cprScale = dynamic_cast<CDM::ChestCompressionForceScaleData*>(cprData);
        if (cprScale != nullptr) {
          auto a = std::make_unique<SEChestCompressionForceScale>();
          Scenario::Marshall(*cprScale, *a);
          return a;
        }
      }

      CDM::ChestOcclusiveDressingData* codData = dynamic_cast<CDM::ChestOcclusiveDressingData*>(action);
      if (codData != nullptr) {
        auto a = std::make_unique<SEChestOcclusiveDressing>();
        Scenario::Marshall(*codData, *a);
        return a;
      }

      CDM::ConsciousRespirationData* conRespData = dynamic_cast<CDM::ConsciousRespirationData*>(action);
      if (conRespData != nullptr) {
        auto a = std::make_unique<SEConsciousRespiration>();
        PatientActions::Marshall(*conRespData, *a);
        return a;
      }

      CDM::ConsumeNutrientsData* consumeData = dynamic_cast<CDM::ConsumeNutrientsData*>(action);
      if (consumeData != nullptr) {
        auto a = std::make_unique<SEConsumeNutrients>();
        Scenario::Marshall(*consumeData, *a);
        return a;
      }

      CDM::ExerciseData* exerciseData = dynamic_cast<CDM::ExerciseData*>(action);
      if (exerciseData != nullptr) {
        auto a = std::make_unique<SEExercise>();
        Scenario::Marshall(*exerciseData, *a);
        return a;
      }

      CDM::IntubationData* intub8Data = dynamic_cast<CDM::IntubationData*>(action);
      if (intub8Data != nullptr) {
        auto a = std::make_unique<SEIntubation>();
        Scenario::Marshall(*intub8Data, *a);
        return a;
      }

      CDM::MechanicalVentilationData* mechVentData = dynamic_cast<CDM::MechanicalVentilationData*>(action);
      if (mechVentData != nullptr) {
        auto a = std::make_unique<SEMechanicalVentilation>();
        PatientActions::Marshall(*mechVentData, substances, *a);
        return a;
      }

      CDM::NeedleDecompressionData* needlData = dynamic_cast<CDM::NeedleDecompressionData*>(action);
      if (needlData != nullptr) {
        auto a = std::make_unique<SENeedleDecompression>();
        Scenario::Marshall(*needlData, *a);
        return a;
      }

      CDM::OverrideData* overrideData = dynamic_cast<CDM::OverrideData*>(action);
      if (overrideData != nullptr) {
        auto a = std::make_unique<SEOverride>();
        Scenario::Marshall(*overrideData, *a);
        return a;
      }

      CDM::HemorrhageData* hemData = dynamic_cast<CDM::HemorrhageData*>(action);
      if (hemData != nullptr) {
        auto a = std::make_unique<SEHemorrhage>();
        Scenario::Marshall(*hemData, *a);
        return a;
      }

      CDM::PainStimulusData* painData = dynamic_cast<CDM::PainStimulusData*>(action);
      if (painData != nullptr) {
        auto a = std::make_unique<SEPainStimulus>();
        Scenario::Marshall(*painData, *a);
        return a;
      }

      CDM::PericardialEffusionData* pericData = dynamic_cast<CDM::PericardialEffusionData*>(action);
      if (pericData != nullptr) {
        auto a = std::make_unique<SEPericardialEffusion>();
        Scenario::Marshall(*pericData, *a);
        return a;
      }

      CDM::InfectionData* infection = dynamic_cast<CDM::InfectionData*>(action);
      if (infection != nullptr) {
        auto inf = std::make_unique<SEInfection>();
        PatientActions::Marshall(*infection, *inf);
        return inf;
      }

      CDM::TensionPneumothoraxData* pneumoData = dynamic_cast<CDM::TensionPneumothoraxData*>(action);
      if (pneumoData != nullptr) {
        auto a = std::make_unique<SETensionPneumothorax>();
        Scenario::Marshall(*pneumoData, *a);
        return a;
      }

      CDM::SubstanceBolusData* bolusData = dynamic_cast<CDM::SubstanceBolusData*>(action);
      if (bolusData != nullptr) {
        substance = substances.GetSubstance(bolusData->Substance());
        if (substance == nullptr) {
          ss << "Unknown substance : " << bolusData->Substance();
          substances.GetLogger()->Fatal(ss, "SEScenario::Load");
        }
        auto a = std::make_unique<SESubstanceBolus>(*substance);
        Scenario::Marshall(*bolusData, *a);
        return a;
      }

      CDM::SubstanceOralDoseData* oralData = dynamic_cast<CDM::SubstanceOralDoseData*>(action);
      if (oralData != nullptr) {
        substance = substances.GetSubstance(oralData->Substance());
        if (substance == nullptr) {
          ss << "Unknown substance : " << oralData->Substance();
          substances.GetLogger()->Fatal(ss, "SEScenario::Load");
        }
        auto od = std::make_unique<SESubstanceOralDose>(*substance);
        Scenario::Marshall(*oralData, *od);
        return od;
      }

      CDM::SubstanceInfusionData* subInfuzData = dynamic_cast<CDM::SubstanceInfusionData*>(action);
      if (subInfuzData != nullptr) {
        substance = substances.GetSubstance(subInfuzData->Substance());
        if (substance == nullptr) {
          ss << "Unknown substance : " << subInfuzData->Substance();
          substances.GetLogger()->Fatal(ss, "SEScenario::Load");
        }
        auto a = std::make_unique<SESubstanceInfusion>(*substance);
        Scenario::Marshall(*subInfuzData, *a);
        return a;
      }

      CDM::SubstanceCompoundInfusionData* subCInfuzData = dynamic_cast<CDM::SubstanceCompoundInfusionData*>(action);
      if (subCInfuzData != nullptr) {
        compound = substances.GetCompound(subCInfuzData->SubstanceCompound());
        if (compound == nullptr) {
          ss << "Unknown substance : " << subCInfuzData->SubstanceCompound();
          substances.GetLogger()->Fatal(ss, "SEScenario::Load");
        }
        auto a = std::make_unique<SESubstanceCompoundInfusion>(*compound);
        Scenario::Marshall(*subCInfuzData, *a);
        return a;
      }

      CDM::UrinateData* urinateData = dynamic_cast<CDM::UrinateData*>(action);
      if (urinateData != nullptr) {
        auto a = std::make_unique<SEUrinate>();
        PatientActions::Marshall(*urinateData, *a);
        return a;
      }
      substances.GetLogger()->Error("Unsupported Patient Action Received", "SEScenario::Load");
    } else if (dynamic_cast<CDM::AnesthesiaMachineActionData*>(action) != nullptr) {
      anConfig = dynamic_cast<CDM::AnesthesiaMachineConfigurationData*>(action);
      if (anConfig != nullptr) {
        auto a = std::make_unique<SEAnesthesiaMachineConfiguration>(substances);
        Scenario::Marshall(*anConfig, *a);
        return a;
      }

      anO2WallLoss = dynamic_cast<CDM::OxygenWallPortPressureLossData*>(action);
      if (anO2WallLoss != nullptr) {
        auto a = std::make_unique<SEOxygenWallPortPressureLoss>();
        Scenario::Marshall(*anO2WallLoss, *a);
        return a;
      }

      anO2TankLoss = dynamic_cast<CDM::OxygenTankPressureLossData*>(action);
      if (anO2TankLoss != nullptr) {
        auto a = std::make_unique<SEOxygenTankPressureLoss>();
        Scenario::Marshall(*anO2TankLoss, *a);
        return a;
      }

      anExLeak = dynamic_cast<CDM::ExpiratoryValveLeakData*>(action);
      if (anExLeak != nullptr) {
        auto a = std::make_unique<SEExpiratoryValveLeak>();
        Scenario::Marshall(*anExLeak, *a);
        return a;
      }

      anExObs = dynamic_cast<CDM::ExpiratoryValveObstructionData*>(action);
      if (anExObs != nullptr) {
        auto a = std::make_unique<SEExpiratoryValveObstruction>();
        Scenario::Marshall(*anExObs, *a);
        return a;
      }

      anInLeak = dynamic_cast<CDM::InspiratoryValveLeakData*>(action);
      if (anInLeak != nullptr) {
        auto a = std::make_unique<SEInspiratoryValveLeak>();
        Scenario::Marshall(*anInLeak, *a);
        return a;
      }

      anInObs = dynamic_cast<CDM::InspiratoryValveObstructionData*>(action);
      if (anInObs != nullptr) {
        auto a = std::make_unique<SEInspiratoryValveObstruction>();
        Scenario::Marshall(*anInObs, *a);
        return a;
      }

      anMskLeak = dynamic_cast<CDM::MaskLeakData*>(action);
      if (anMskLeak != nullptr) {
        auto a = std::make_unique<SEMaskLeak>();
        Scenario::Marshall(*anMskLeak, *a);
        return a;
      }

      anSodaFail = dynamic_cast<CDM::SodaLimeFailureData*>(action);
      if (anSodaFail != nullptr) {
        auto a = std::make_unique<SESodaLimeFailure>();
        Scenario::Marshall(*anSodaFail, *a);
        return a;
      }

      anTubLeak = dynamic_cast<CDM::TubeCuffLeakData*>(action);
      if (anTubLeak != nullptr) {
        auto a = std::make_unique<SETubeCuffLeak>();
        Scenario::Marshall(*anTubLeak, *a);
        return a;
      }

      anVapFail = dynamic_cast<CDM::VaporizerFailureData*>(action);
      if (anVapFail != nullptr) {
        auto a = std::make_unique<SEVaporizerFailure>();
        Scenario::Marshall(*anVapFail, *a);
        return a;
      }

      anVentLoss = dynamic_cast<CDM::VentilatorPressureLossData*>(action);
      if (anVentLoss != nullptr) {
        auto a = std::make_unique<SEVentilatorPressureLoss>();
        Scenario::Marshall(*anVentLoss, *a);
        return a;
      }

      anYDisc = dynamic_cast<CDM::YPieceDisconnectData*>(action);
      if (anYDisc != nullptr) {
        auto a = std::make_unique<SEYPieceDisconnect>();
        Scenario::Marshall(*anYDisc, *a);
        return a;
      }
      substances.GetLogger()->Error("Unsupported Anesthesia Machine Action Received", "SEScenario::Load");
    } else if (dynamic_cast<CDM::InhalerActionData*>(action) != nullptr) {
      CDM::InhalerConfigurationData* inhaleConfig = dynamic_cast<CDM::InhalerConfigurationData*>(action);
      if (inhaleConfig != nullptr) {
        auto a = std::make_unique<SEInhalerConfiguration>(substances);
        Scenario::Marshall(*inhaleConfig, *a);
        return a;
      }
    }

    if (substances.GetLogger() != nullptr)
      substances.GetLogger()->Error("Unsupported Action Received", "SEAction::newFromBind");
    return nullptr;
  }
  //----------------------------------------------------------------------------------
  //class SEPatientAction
  void PatientActions::Marshall(const CDM::PatientActionData& in, SEPatientAction& out)
  {
    Scenario::Marshall(static_cast<const CDM::ActionData&>(in), static_cast<SEAction&>(out));
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEPatientAction& in, CDM::PatientActionData& out)
  {
    Scenario::UnMarshall(static_cast<const SEAction&>(in), static_cast<CDM::ActionData&>(out));
  }
  //----------------------------------------------------------------------------------
  //class SEPatientAssessmentRequest
  void PatientActions::Marshall(const CDM::PatientAssessmentRequestData& in, SEPatientAssessmentRequest& out)
  {
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    out.m_Type = in.Type();
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEPatientAssessmentRequest& in, CDM::PatientAssessmentRequestData& out)
  {
    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    if (in.HasType()) {
      out.Type(in.m_Type);
    }
  }
  //----------------------------------------------------------------------------------
  //class SEAcuteStress
  void PatientActions::Marshall(const CDM::AcuteStressData& in, SEAcuteStress& out)
  {
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    io::Property::Marshall(in.Severity(), out.GetSeverity());
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEAcuteStress& in, CDM::AcuteStressData& out)
  {
    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    if (in.m_Severity != nullptr) {
      io::Property::UnMarshall(*in.m_Severity, out.Severity());
    }
  }
  //----------------------------------------------------------------------------------
  //class SEAirwayObstruction
  void PatientActions::Marshall(const CDM::AirwayObstructionData& in, SEAirwayObstruction& out)
  {
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    io::Property::Marshall(in.Severity(), out.GetSeverity());
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEAirwayObstruction& in, CDM::AirwayObstructionData& out)
  {
    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    if (in.m_Severity != nullptr) {
      io::Property::UnMarshall(*in.m_Severity, out.Severity());
    }
  }
  //----------------------------------------------------------------------------------
  //class SEApnea
  void PatientActions::Marshall(const CDM::ApneaData& in, SEApnea& out)
  {
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    io::Property::Marshall(in.Severity(), out.GetSeverity());
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEApnea& in, CDM::ApneaData& out)
  {
    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    if (in.m_Severity != nullptr) {
      io::Property::UnMarshall(*in.m_Severity, out.Severity());
    }
  }
  //----------------------------------------------------------------------------------
  //class SEAsthmaAttack
  void PatientActions::Marshall(const CDM::AsthmaAttackData& in, SEAsthmaAttack& out)
  {
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    io::Property::Marshall(in.Severity(), out.GetSeverity());
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEAsthmaAttack& in, CDM::AsthmaAttackData& out)
  {
    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    CDM_PROPERTY_UNMARSHAL_HELPER(out, Severity)
  }
  //----------------------------------------------------------------------------------
  //class SEBrainInjury
  void PatientActions::Marshall(const CDM::BrainInjuryData& in, SEBrainInjury& out)
  {
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    io::Property::Marshall(in.Severity(), out.GetSeverity());
    out.m_Type = in.Type();
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEBrainInjury& in, CDM::BrainInjuryData& out)
  {
    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    if (in.m_Severity != nullptr) {
      io::Property::UnMarshall(*in.m_Severity, out.Severity());
    }
    if (in.HasType()) {
      out.Type(in.m_Type);
    }
  }
  //----------------------------------------------------------------------------------
  //class SEBronchoconstriction
  void PatientActions::Marshall(const CDM::BronchoconstrictionData& in, SEBronchoconstriction& out)
  {
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    io::Property::Marshall(in.Severity(), out.GetSeverity());
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEBronchoconstriction& in, CDM::BronchoconstrictionData& out)
  {
    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    if (in.m_Severity != nullptr) {
      io::Property::UnMarshall(*in.m_Severity, out.Severity());
    }
  }
  //----------------------------------------------------------------------------------
  //class SEBurnWound
  void PatientActions::Marshall(const CDM::BurnWoundData& in, SEBurnWound& out)
  {
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    io::Property::Marshall(in.TotalBodySurfaceArea(), out.GetTotalBodySurfaceArea());
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEBurnWound& in, CDM::BurnWoundData& out)
  {
    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    if (in.m_TBSA != nullptr) {
      io::Property::UnMarshall(*in.m_TBSA, out.TotalBodySurfaceArea());
    }
  }
  //----------------------------------------------------------------------------------
  //class SECardiacArrest
  void PatientActions::Marshall(const CDM::CardiacArrestData& in, SECardiacArrest& out)
  {
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    out.m_State = in.State();
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SECardiacArrest& in, CDM::CardiacArrestData& out)
  {
    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    out.State(in.m_State);
  }
  //----------------------------------------------------------------------------------
  //class SEChestCompression
  void PatientActions::Marshall(const CDM::ChestCompressionData& in, SEChestCompression& out)
  {
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEChestCompression& in, CDM::ChestCompressionData& out)
  {
    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
  }
  //----------------------------------------------------------------------------------
  //class SEChestCompressionForce
  void PatientActions::Marshall(const CDM::ChestCompressionForceData& in, SEChestCompressionForce& out)
  {
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    io::Property::Marshall(in.Force(), out.GetForce());
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEChestCompressionForce& in, CDM::ChestCompressionForceData& out)
  {
    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    if (in.m_Force != nullptr) {
      io::Property::UnMarshall(*in.m_Force, out.Force());
    }
  }
  //----------------------------------------------------------------------------------
  //class SEChestCompressionForceScale
  void PatientActions::Marshall(const CDM::ChestCompressionForceScaleData& in, SEChestCompressionForceScale& out)
  {
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    io::Property::Marshall(in.ForceScale(), out.GetForceScale());
    if (in.ForcePeriod().present()) {
      io::Property::Marshall(in.ForcePeriod(), out.GetForcePeriod());
    }
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEChestCompressionForceScale& in, CDM::ChestCompressionForceScaleData& out)
  {
    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    if (in.m_ForceScale != nullptr) {
      io::Property::UnMarshall(*in.m_ForceScale, out.ForceScale());
    }
    if (in.m_ForcePeriod != nullptr) {
      io::Property::UnMarshall(*in.m_ForcePeriod, out.ForcePeriod());
    }
  }
  //----------------------------------------------------------------------------------
  //class SEChestOcclusiveDressing
  void PatientActions::Marshall(const CDM::ChestOcclusiveDressingData& in, SEChestOcclusiveDressing& out)
  {
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    out.m_Side = in.Side();
    out.m_State = in.State();
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEChestOcclusiveDressing& in, CDM::ChestOcclusiveDressingData& out)
  {
    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    out.State(in.m_State);
    if (in.HasSide()) {
      out.Side(in.m_Side);
    }
  }
  //----------------------------------------------------------------------------------
  //class SEConsciousRespirationCommand
  void PatientActions::Marshall(const CDM::ConsciousRespirationCommandData& in, SEConsciousRespirationCommand& out)
  {
    if (in.Comment().present()) {
      out.m_Comment = in.Comment().get();
    }
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEConsciousRespirationCommand& in, CDM::ConsciousRespirationCommandData& out)
  {
    if (in.HasComment()) {
      out.Comment(in.m_Comment);
    }
  }
  //----------------------------------------------------------------------------------
  //class SEForcedInhale
  void PatientActions::Marshall(const CDM::ForcedInhaleData& in, SEForcedInhale& out)
  {
    PatientActions::Marshall(static_cast<const CDM::ConsciousRespirationCommandData&>(in), static_cast<SEConsciousRespirationCommand&>(out));
    io::Property::Marshall(in.InspiratoryCapacityFraction(), out.GetInspiratoryCapacityFraction());
    io::Property::Marshall(in.Period(), out.GetPeriod());
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEForcedInhale& in, CDM::ForcedInhaleData& out)
  {
    PatientActions::UnMarshall(static_cast<const SEConsciousRespirationCommand&>(in), static_cast<CDM::ConsciousRespirationCommandData&>(out));
    if (in.m_InspiratoryCapacityFraction != nullptr) {
      io::Property::UnMarshall(*in.m_InspiratoryCapacityFraction, out.InspiratoryCapacityFraction());
    }
    if (in.m_Period != nullptr) {
      io::Property::UnMarshall(*in.m_Period, out.Period());
    }
  }
  //----------------------------------------------------------------------------------
  //class SEForcedExhale
  void PatientActions::Marshall(const CDM::ForcedExhaleData& in, SEForcedExhale& out)
  {
    PatientActions::Marshall(static_cast<const CDM::ConsciousRespirationCommandData&>(in), static_cast<SEConsciousRespirationCommand&>(out));
    io::Property::Marshall(in.ExpiratoryReserveVolumeFraction(), out.GetExpiratoryReserveVolumeFraction());
    io::Property::Marshall(in.Period(), out.GetPeriod());
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEForcedExhale& in, CDM::ForcedExhaleData& out)
  {
    PatientActions::UnMarshall(static_cast<const SEConsciousRespirationCommand&>(in), static_cast<CDM::ConsciousRespirationCommandData&>(out));
    if (in.m_ExpiratoryReserveVolumeFraction != nullptr) {
      io::Property::UnMarshall(*in.m_ExpiratoryReserveVolumeFraction, out.ExpiratoryReserveVolumeFraction());
    }
    if (in.m_Period != nullptr) {
      io::Property::UnMarshall(*in.m_Period, out.Period());
    }
  }
  //----------------------------------------------------------------------------------
  //class SEBreathHold
  void PatientActions::Marshall(const CDM::BreathHoldData& in, SEBreathHold& out)
  {
    PatientActions::Marshall(static_cast<const CDM::ConsciousRespirationCommandData&>(in), static_cast<SEConsciousRespirationCommand&>(out));
    io::Property::Marshall(in.Period(), out.GetPeriod());
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEBreathHold& in, CDM::BreathHoldData& out)
  {
    PatientActions::UnMarshall(static_cast<const SEConsciousRespirationCommand&>(in), static_cast<CDM::ConsciousRespirationCommandData&>(out));
    if (in.m_Period != nullptr) {
      io::Property::UnMarshall(*in.m_Period, out.Period());
    }
  }
  //----------------------------------------------------------------------------------
  //class SEUseInhaler
  void PatientActions::Marshall(const CDM::UseInhalerData& in, SEUseInhaler& out)
  {
    PatientActions::Marshall(static_cast<const CDM::ConsciousRespirationCommandData&>(in), static_cast<SEConsciousRespirationCommand&>(out));
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEUseInhaler& in, CDM::UseInhalerData& out)
  {
    PatientActions::UnMarshall(static_cast<const SEConsciousRespirationCommand&>(in), static_cast<CDM::ConsciousRespirationCommandData&>(out));
  }
  //----------------------------------------------------------------------------------
  //class SEConsciousRespiration
  void PatientActions::Marshall(const CDM::ConsciousRespirationData& in, SEConsciousRespiration& out)
  {
    PatientActions::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    // Set this before our super class tells us to Clear if the action wants us to keep our current data
    out.m_ClearCommands = !in.AppendToPrevious();
    out.m_ClearCommands = true;
    CDM::ConsciousRespirationCommandData* command;
    for (unsigned int i = 0; i < in.Command().size(); i++) {
      command = (CDM::ConsciousRespirationCommandData*)&in.Command()[i];

      CDM::BreathHoldData* bh = dynamic_cast<CDM::BreathHoldData*>(command);
      if (bh != nullptr) {
        PatientActions::Marshall(*bh, out.AddBreathHold());
        continue;
      }

      CDM::ForcedInhaleData* fi = dynamic_cast<CDM::ForcedInhaleData*>(command);
      if (fi != nullptr) {
        PatientActions::Marshall(*fi, out.AddForcedInhale());
        continue;
      }

      CDM::ForcedExhaleData* fe = dynamic_cast<CDM::ForcedExhaleData*>(command);
      if (fe != nullptr) {
        PatientActions::Marshall(*fe, out.AddForcedExhale());
        continue;
      }

      CDM::UseInhalerData* si = dynamic_cast<CDM::UseInhalerData*>(command);
      if (si != nullptr) {
        PatientActions::Marshall(*si, out.AddUseInhaler());
        continue;
      }
    }
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEConsciousRespiration& in, CDM::ConsciousRespirationData& out)
  {
    PatientActions::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));

    out.AppendToPrevious(false);
    for (SEConsciousRespirationCommand* cmd : in.m_Commands) {
      auto cmdData = std::unique_ptr<CDM::ConsciousRespirationCommandData>();
      PatientActions::UnMarshall(*cmd, *cmdData);
      out.Command().push_back(std::move(cmdData));
    }
  }
  //----------------------------------------------------------------------------------
  //class SEConsumeNutrients
  void PatientActions::Marshall(const CDM::ConsumeNutrientsData& in, SEConsumeNutrients& out)
  {
    PatientActions::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    if (in.Nutrition().present()) {
      out.GetNutrition().Load(in.Nutrition().get());
    } else if (in.NutritionFile().present()) {
      out.SetNutritionFile(in.NutritionFile().get());
    }
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEConsumeNutrients& in, CDM::ConsumeNutrientsData& out)
  {
    PatientActions::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    if (in.HasNutrition()) {
      out.Nutrition(std::unique_ptr<CDM::NutritionData>(in.m_Nutrition->Unload()));
    }
    if (in.HasNutritionFile()) {
      out.NutritionFile(in.m_NutritionFile);
    }
  }
  //----------------------------------------------------------------------------------
  //class SEExercise
  void Marshall(const CDM::GenericExerciseData& in, SEExercise::SEGeneric& out)
  {
    io::Property::Marshall(in.Intensity(), out.Intensity);
    io::Property::Marshall(in.DesiredWorkRate(), out.DesiredWorkRate);
  }
  void Marshall(const CDM::CyclingExerciseData& in, SEExercise::SECycling& out)
  {
    io::Property::Marshall(in.Cadence(), out.CadenceCycle);
    io::Property::Marshall(in.Power(), out.PowerCycle);
    if (in.AddedWeight().present()) {
      io::Property::Marshall(in.AddedWeight().get(), out.AddedWeight);
    } else {
      out.AddedWeight.SetValue(0, MassUnit::kg);
    }
  }
  void Marshall(const CDM::RunningExerciseData& in, SEExercise::SERunning& out)
  {
    io::Property::Marshall(in.Speed(), out.SpeedRun);
    io::Property::Marshall(in.Incline(), out.InclineRun);
    if (in.AddedWeight().present()) {
      io::Property::Marshall(in.AddedWeight().get(), out.AddedWeight);
    } else {
      out.AddedWeight.SetValue(0, MassUnit::kg);
    }
  }
  void Marshall(const CDM::StrengthExerciseData& in, SEExercise::SEStrengthTraining& out)
  {
    io::Property::Marshall(in.Weight(), out.WeightStrength);
    io::Property::Marshall(in.Repetitions(), out.RepsStrength);
  }
  void PatientActions::Marshall(const CDM::ExerciseData& in, SEExercise& out)
  {
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    if (in.GenericExercise().present()) {
      out.m_mode = SEExercise::ExerciseType ::GENERIC;
      io::Marshall(in.GenericExercise().get(), out.GetGenericExercise());
    }
    if (in.CyclingExercise().present()) {
      out.m_mode = SEExercise::ExerciseType ::CYCLING;
      io::Marshall(in.CyclingExercise().get(), out.GetCyclingExercise());
    }
    if (in.RunningExercise().present()) {
      out.m_mode = SEExercise::ExerciseType ::RUNNING;
      io::Marshall(in.RunningExercise().get(), out.GetRunningExercise());
    }
    if (in.StrengthExercise().present()) {
      out.m_mode = SEExercise::ExerciseType ::STRENGTH_TRAINING;
      io::Marshall(in.StrengthExercise().get(), out.GetStrengthExercise());
    }
  }
  //----------------------------------------------------------------------------------
  void UnMarshall(const SEExercise::SEGeneric& in, CDM::GenericExerciseData& out)
  {
    io::Property::UnMarshall(in.Intensity, out.Intensity());
    io::Property::UnMarshall(in.DesiredWorkRate, out.DesiredWorkRate());
  }
  void UnMarshall(const SEExercise::SECycling& in, CDM::CyclingExerciseData& out)
  {
    io::Property::UnMarshall(in.CadenceCycle, out.Cadence());
    io::Property::UnMarshall(in.PowerCycle, out.Power());
    io::Property::UnMarshall(in.AddedWeight, out.AddedWeight());
  }
  void UnMarshall(const SEExercise::SERunning& in, CDM::RunningExerciseData& out)
  {
    io::Property::UnMarshall(in.SpeedRun, out.Speed());
    io::Property::UnMarshall(in.InclineRun, out.Incline());
    io::Property::UnMarshall(in.AddedWeight, out.AddedWeight());
  }
  void UnMarshall(const SEExercise::SEStrengthTraining& in, CDM::StrengthExerciseData& out)
  {
    io::Property::UnMarshall(in.WeightStrength, out.Weight());
    io::Property::UnMarshall(in.RepsStrength, out.Repetitions());
  }
  void PatientActions::UnMarshall(const SEExercise& in, CDM::ExerciseData& out)
  {
    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    if (in.HasGenericExercise()) {
      auto item = std::make_unique<CDM::GenericExerciseData>();
      io::UnMarshall(in.m_genericExercise, *item);
      out.GenericExercise(std::move(item));
    }
    if (in.HasCyclingExercise()) {
      auto item = std::make_unique<CDM::CyclingExerciseData>();
      io::UnMarshall(in.m_cyclingExercise, *item);
      out.CyclingExercise(std::move(item));
    }
    if (in.HasRunningExercise()) {
      auto item = std::make_unique<CDM::RunningExerciseData>();
      io::UnMarshall(in.m_runningExercise, *item);
      out.RunningExercise(std::move(item));
    }
    if (in.HasStrengthExercise()) {
      auto item = std::make_unique<CDM::StrengthExerciseData>();
      io::UnMarshall(in.m_strengthExercise, *item);
      out.StrengthExercise(std::move(item));
    }
  }
  //----------------------------------------------------------------------------------
  //class SEHemorrhage
  void PatientActions::Marshall(const CDM::HemorrhageData& in, SEHemorrhage& out)
  {
    out.Clear();
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    out.m_Compartment = in.Compartment();
    io::Property::Marshall(in.InitialRate(), out.GetInitialRate());
    //Place compartments in torso in a map so that we don't get too messy with nested conditionals.  Each vector is digits 2-4 of the MCIS code
    out.organMap["VenaCava"] = std::vector<unsigned int> { 6, 6, 0 };
    out.organMap["LeftLung"] = std::vector<unsigned int> { 7, 1, 0 };
    out.organMap["RightLung"] = std::vector<unsigned int> { 7, 1, 0 };
    out.organMap["Myocardium"] = std::vector<unsigned int> { 7, 2, 0 };
    out.organMap["Liver"] = std::vector<unsigned int> { 8, 1, 0 };
    out.organMap["Spleen"] = std::vector<unsigned int> { 8, 2, 0 };
    out.organMap["Splanchnic"] = std::vector<unsigned int> { 8, 3, 0 };
    out.organMap["LeftKidney"] = std::vector<unsigned int> { 8, 4, 0 };
    out.organMap["RightKidney"] = std::vector<unsigned int> { 8, 4, 0 };
    out.organMap["SmallIntestine"] = std::vector<unsigned int> { 8, 5, 0 };
    out.organMap["LargeIntestine"] = std::vector<unsigned int> { 8, 6, 0 };
    out.SetMCIS();
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEHemorrhage& in, CDM::HemorrhageData& out)
  {
    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    if (in.HasCompartment()) {
      out.Compartment(in.m_Compartment);
    }
    if (in.HasInitialRate()) {
      io::Property::UnMarshall(*in.m_InitialRate, out.InitialRate());
    }
  }
  //----------------------------------------------------------------------------------
  //class SESepsis
  void PatientActions::Marshall(const CDM::InfectionData& in, SEInfection& out)
  {
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    out.m_Location = in.Location();
    out.m_Severity = in.Severity();
    io::Property::Marshall(in.MinimumInhibitoryConcentration(), out.GetMinimumInhibitoryConcentration());
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEInfection& in, CDM::InfectionData& out)
  {

    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    if (in.HasLocation()) {
      out.Location(in.GetLocation());
    }
    if (in.HasSeverity()) {
      out.Severity(in.m_Severity);
    }
    if (in.HasMinimumInhibitoryConcentration()) {
      io::Property::UnMarshall(*in.m_MinimumInhibitoryConcentration, out.MinimumInhibitoryConcentration());
    }
  }
  //----------------------------------------------------------------------------------
  //class SEIntubation
  void PatientActions::Marshall(const CDM::IntubationData& in, SEIntubation& out)
  {
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    out.m_Type = in.Type();
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEIntubation& in, CDM::IntubationData& out)
  {
    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    if (in.HasType()) {
      out.Type(in.m_Type);
    }
  }
  //----------------------------------------------------------------------------------
  //class SEMechanicalVentilation
  void PatientActions::Marshall(const CDM::MechanicalVentilationData& in, const SESubstanceManager& substances, SEMechanicalVentilation& out)
  {
    out.Clear();
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    out.SetState(in.State());
    if (in.Flow().present()) {
      io::Property::Marshall(in.Flow(), out.GetFlow());
    } else {
      out.GetFlow().Invalidate();
    }
    if (in.Pressure().present()) {
      io::Property::Marshall(in.Pressure(), out.GetPressure());
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
      throw CommonDataModelException("Unable to Marshall SEMechanicalVentilation with the given MechanicalVentilationData");
    }
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEMechanicalVentilation& in, CDM::MechanicalVentilationData& out)
  {
    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    if (in.HasState()) {
      out.State(in.m_State);
    }
    if (in.HasFlow()) {
      io::Property::UnMarshall(*in.m_Flow, out.Flow());
    }
    if (in.HasPressure()) {
      io::Property::UnMarshall(*in.m_Pressure, out.Pressure());
    }

    for (SESubstanceFraction* sf : in.m_GasFractions) {
      out.GasFraction().push_back(std::unique_ptr<CDM::SubstanceFractionData>(sf->Unload()));
    }
  }
  //----------------------------------------------------------------------------------
  //class SENeedleDecompression
  void PatientActions::Marshall(const CDM::NeedleDecompressionData& in, SENeedleDecompression& out)
  {
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    out.m_Side = in.Side();
    out.m_State = in.State();
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SENeedleDecompression& in, CDM::NeedleDecompressionData& out)
  {
    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    out.State(in.m_State);
    if (in.HasSide()) {
      out.Side(in.m_Side);
    }
  }
  //----------------------------------------------------------------------------------
  //class SEPainStimulus
  void PatientActions::Marshall(const CDM::PainStimulusData& in, SEPainStimulus& out)
  {
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    io::Property::Marshall(in.Severity(), out.GetSeverity());
    out.m_Location = in.Location();
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEPainStimulus& in, CDM::PainStimulusData& out)
  {
    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    if (in.m_Severity != nullptr) {
      io::Property::UnMarshall(*in.m_Severity, out.Severity());
    }
    if (in.HasLocation()) {
      out.Location(in.m_Location);
    }
  }
  //----------------------------------------------------------------------------------
  //class SEPericardialEffusion
  void PatientActions::Marshall(const CDM::PericardialEffusionData& in, SEPericardialEffusion& out)
  {
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    io::Property::Marshall(in.EffusionRate(), out.GetEffusionRate());
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEPericardialEffusion& in, CDM::PericardialEffusionData& out)
  {
    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    if (in.m_EffusionRate != nullptr) {
      io::Property::UnMarshall(*in.m_EffusionRate, out.EffusionRate());
    }
  }
  //----------------------------------------------------------------------------------
  //class SETensionPneumothorax
  void PatientActions::Marshall(const CDM::TensionPneumothoraxData& in, SETensionPneumothorax& out)
  {
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
    io::Property::Marshall(in.Severity(), out.GetSeverity());
    out.m_Type = in.Type();
    out.m_Side = in.Side();
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SETensionPneumothorax& in, CDM::TensionPneumothoraxData& out)
  {
    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    if (in.m_Severity != nullptr) {
      io::Property::UnMarshall(*in.m_Severity, out.Severity());
    }
    if (in.HasType()) {
      out.Type(in.m_Type);
    }
    if (in.HasSide()) {
      out.Side(in.m_Side);
    }
  }
  //----------------------------------------------------------------------------------
  //class SESubstanceAdministration
  void PatientActions::Marshall(const CDM::SubstanceAdministrationData& in, SESubstanceAdministration& out)
  {
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SESubstanceAdministration& in, CDM::SubstanceAdministrationData& out)
  {
    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
  }
  //----------------------------------------------------------------------------------
  //class SESubstanceBolus
  void PatientActions::Marshall(const CDM::SubstanceBolusData& in, SESubstanceBolus& out)
  {
    Scenario::Marshall(static_cast<const CDM::SubstanceAdministrationData&>(in), static_cast<SESubstanceAdministration&>(out));
    io::Property::Marshall(in.Dose(), out.GetDose());
    io::Property::Marshall(in.Concentration(), out.GetConcentration());
    out.m_AdminRoute = in.AdminRoute();
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SESubstanceBolus& in, CDM::SubstanceBolusData& out)
  {
    Scenario::UnMarshall(static_cast<const SESubstanceAdministration&>(in), static_cast<CDM::SubstanceAdministrationData&>(out));

    if (in.m_Dose != nullptr) {
      io::Property::UnMarshall(*in.m_Dose, out.Dose());
    }
    if (in.m_Concentration != nullptr) {
      io::Property::UnMarshall(*in.m_Concentration, out.Concentration());
    }
    if (in.HasAdminRoute()) {
      out.AdminRoute(in.m_AdminRoute);
    }
    out.Substance(in.m_Substance.GetName());
  }
  //----------------------------------------------------------------------------------
  //class SESubstanceBolusState
  void PatientActions::Marshall(const CDM::SubstanceBolusStateData& in, SESubstanceBolusState& out)
  {
    io::Property::Marshall(in.ElapsedTime(), out.m_elapsedTime);
    io::Property::Marshall(in.AdministeredDose(), out.m_administeredDose);
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SESubstanceBolusState& in, CDM::SubstanceBolusStateData& out)
  {
    out.Substance(in.m_substance.GetName());
    io::Property::UnMarshall(in.m_elapsedTime, out.ElapsedTime());
    io::Property::UnMarshall(in.m_administeredDose, out.AdministeredDose());
  }
  //----------------------------------------------------------------------------------
  //class SESubstanceCompoundInfusion
  void PatientActions::Marshall(const CDM::SubstanceCompoundInfusionData& in, SESubstanceCompoundInfusion& out)
  {
    Scenario::Marshall(static_cast<const CDM::SubstanceAdministrationData&>(in), static_cast<SESubstanceAdministration&>(out));

    io::Property::Marshall(in.Rate(), out.GetRate());
    io::Property::Marshall(in.BagVolume(), out.GetBagVolume());
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SESubstanceCompoundInfusion& in, CDM::SubstanceCompoundInfusionData& out)
  {
    Scenario::UnMarshall(static_cast<const SESubstanceAdministration&>(in), static_cast<CDM::SubstanceAdministrationData&>(out));
    if (in.m_Rate != nullptr) {
      io::Property::UnMarshall(*in.m_Rate, out.Rate());
    }
    if (in.m_BagVolume != nullptr) {
      io::Property::UnMarshall(*in.m_BagVolume, out.BagVolume());
    }
    out.SubstanceCompound(in.m_Compound.GetName());
  }
  //----------------------------------------------------------------------------------
  //class SESubstanceInfusion
  void PatientActions::Marshall(const CDM::SubstanceInfusionData& in, SESubstanceInfusion& out)
  {
    Scenario::Marshall(static_cast<const CDM::SubstanceAdministrationData&>(in), static_cast<SESubstanceAdministration&>(out));
    io::Property::Marshall(in.Rate(), out.GetRate());
    io::Property::Marshall(in.Concentration(), out.GetConcentration());
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SESubstanceInfusion& in, CDM::SubstanceInfusionData& out)
  {
    Scenario::UnMarshall(static_cast<const SESubstanceAdministration&>(in), static_cast<CDM::SubstanceAdministrationData&>(out));
    if (in.m_Rate != nullptr) {
      io::Property::UnMarshall(*in.m_Rate, out.Rate());
    }
    if (in.m_Concentration != nullptr) {
      io::Property::UnMarshall(*in.m_Concentration, out.Concentration());
    }
    out.Substance(in.m_Substance.GetName());
  }
  //----------------------------------------------------------------------------------
  //class SEUrinate
  void PatientActions::Marshall(const CDM::UrinateData& in, SEUrinate& out)
  {
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEUrinate& in, CDM::UrinateData& out)
  {
    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
  }
  //----------------------------------------------------------------------------------
  //class SEOverride
  void PatientActions::Marshall(const CDM::OverrideData& in, SEOverride& out)
  {
    Scenario::Marshall(static_cast<const CDM::PatientActionData&>(in), static_cast<SEPatientAction&>(out));

    out.SetOverrideState(in.State());
    out.SetOverrideConformance(in.Conformant());
    if (in.ArterialBloodPHOverride().present()) {
      io::Property::Marshall(in.ArterialBloodPHOverride(), out.GetArterialPHOverride());
    } else {
      out.GetArterialPHOverride().Invalidate();
    }
    if (in.VenousBloodPHOverride().present()) {
      io::Property::Marshall(in.VenousBloodPHOverride(), out.GetVenousPHOverride());
    } else {
      out.GetVenousPHOverride().Invalidate();
    }
    if (in.CarbonDioxideSaturationOverride().present()) {
      io::Property::Marshall(in.CarbonDioxideSaturationOverride(), out.GetCO2SaturationOverride());
    } else {
      out.GetCO2SaturationOverride().Invalidate();
    }
    if (in.CarbonMonoxideSaturationOverride().present()) {
      io::Property::Marshall(in.CarbonMonoxideSaturationOverride(), out.GetCOSaturationOverride());
    } else {
      out.GetCOSaturationOverride().Invalidate();
    }
    if (in.OxygenSaturationOverride().present()) {
      io::Property::Marshall(in.OxygenSaturationOverride(), out.GetO2SaturationOverride());
    } else {
      out.GetO2SaturationOverride().Invalidate();
    }
    if (in.PhosphateOverride().present()) {
      io::Property::Marshall(in.PhosphateOverride(), out.GetPhosphateOverride());
    } else {
      out.GetPhosphateOverride().Invalidate();
    }
    if (in.WhiteBloodCellCountOverride().present()) {
      io::Property::Marshall(in.WhiteBloodCellCountOverride(), out.GetWBCCountOverride());
    } else {
      out.GetWBCCountOverride().Invalidate();
    }
    if (in.TotalBilirubinOverride().present()) {
      io::Property::Marshall(in.TotalBilirubinOverride(), out.GetTotalBilirubinOverride());
    } else {
      out.GetTotalBilirubinOverride().Invalidate();
    }
    if (in.CalciumConcentrationOverride().present()) {
      io::Property::Marshall(in.CalciumConcentrationOverride(), out.GetCalciumConcentrationOverride());
    } else {
      out.GetCalciumConcentrationOverride().Invalidate();
    }
    if (in.GlucoseConcentrationOverride().present()) {
      io::Property::Marshall(in.GlucoseConcentrationOverride(), out.GetGlucoseConcentrationOverride());
    } else {
      out.GetGlucoseConcentrationOverride().Invalidate();
    }
    if (in.LactateConcentrationOverride().present()) {
      io::Property::Marshall(in.LactateConcentrationOverride(), out.GetLactateConcentrationOverride());
    } else {
      out.GetLactateConcentrationOverride().Invalidate();
    }
    if (in.PotassiumConcentrationOverride().present()) {
      io::Property::Marshall(in.PotassiumConcentrationOverride(), out.GetPotassiumConcentrationOverride());
    } else {
      out.GetPotassiumConcentrationOverride().Invalidate();
    }
    if (in.SodiumConcentrationOverride().present()) {
      io::Property::Marshall(in.SodiumConcentrationOverride(), out.GetSodiumConcentrationOverride());
    } else {
      out.GetSodiumConcentrationOverride().Invalidate();
    }
    if (in.BloodVolumeOverride().present()) {
      io::Property::Marshall(in.BloodVolumeOverride(), out.GetBloodVolumeOverride());
    } else {
      out.GetBloodVolumeOverride().Invalidate();
    }
    if (in.CardiacOutputOverride().present()) {
      io::Property::Marshall(in.CardiacOutputOverride(), out.GetCardiacOutputOverride());
    } else {
      out.GetCardiacOutputOverride().Invalidate();
    }
    if (in.DiastolicArterialPressureOverride().present()) {
      io::Property::Marshall(in.DiastolicArterialPressureOverride(), out.GetDiastolicArterialPressureOverride());
    } else {
      out.GetDiastolicArterialPressureOverride().Invalidate();
    }
    if (in.MeanArterialPressureOverride().present()) {
      io::Property::Marshall(in.MeanArterialPressureOverride(), out.GetMAPOverride());
    } else {
      out.GetMAPOverride().Invalidate();
    }
    if (in.HeartRateOverride().present()) {
      io::Property::Marshall(in.HeartRateOverride(), out.GetHeartRateOverride());
    } else {
      out.GetHeartRateOverride().Invalidate();
    }
    if (in.HeartStrokeVolumeOverride().present()) {
      io::Property::Marshall(in.HeartStrokeVolumeOverride(), out.GetHeartStrokeVolumeOverride());
    } else {
      out.GetHeartStrokeVolumeOverride().Invalidate();
    }
    if (in.SystolicArterialPressureOverride().present()) {
      io::Property::Marshall(in.SystolicArterialPressureOverride(), out.GetSystolicArterialPressureOverride());
    } else {
      out.GetSystolicArterialPressureOverride().Invalidate();
    }
    if (in.InsulinSynthesisRateOverride().present()) {
      io::Property::Marshall(in.InsulinSynthesisRateOverride(), out.GetInsulinSynthesisRateOverride());
    } else {
      out.GetInsulinSynthesisRateOverride().Invalidate();
    }
    if (in.GlucagonSynthesisRateOverride().present()) {
      io::Property::Marshall(in.GlucagonSynthesisRateOverride(), out.GetGlucagonSynthesisRateOverride());
    } else {
      out.GetGlucagonSynthesisRateOverride().Invalidate();
    }
    if (in.AchievedExerciseLevelOverride().present()) {
      io::Property::Marshall(in.AchievedExerciseLevelOverride(), out.GetAchievedExerciseLevelOverride());
    } else {
      out.GetAchievedExerciseLevelOverride().Invalidate();
    }
    if (in.CoreTemperatureOverride().present()) {
      io::Property::Marshall(in.CoreTemperatureOverride(), out.GetCoreTemperatureOverride());
    } else {
      out.GetCoreTemperatureOverride().Invalidate();
    }
    if (in.CreatinineProductionRateOverride().present()) {
      io::Property::Marshall(in.CreatinineProductionRateOverride(), out.GetCreatinineProductionRateOverride());
    } else {
      out.GetCreatinineProductionRateOverride().Invalidate();
    }
    if (in.ExerciseMeanArterialPressureDeltaOverride().present()) {
      io::Property::Marshall(in.ExerciseMeanArterialPressureDeltaOverride(), out.GetExerciseMeanArterialPressureDeltaOverride());
    } else {
      out.GetExerciseMeanArterialPressureDeltaOverride().Invalidate();
    }
    if (in.FatigueLevelOverride().present()) {
      io::Property::Marshall(in.FatigueLevelOverride(), out.GetFatigueLevelOverride());
    } else {
      out.GetFatigueLevelOverride().Invalidate();
    }
    if (in.LactateProductionRateOverride().present()) {
      io::Property::Marshall(in.LactateProductionRateOverride(), out.GetLactateProductionRateOverride());
    } else {
      out.GetLactateProductionRateOverride().Invalidate();
    }
    if (in.SkinTemperatureOverride().present()) {
      io::Property::Marshall(in.SkinTemperatureOverride(), out.GetSkinTemperatureOverride());
    } else {
      out.GetSkinTemperatureOverride().Invalidate();
    }
    if (in.SweatRateOverride().present()) {
      io::Property::Marshall(in.SweatRateOverride(), out.GetSweatRateOverride());
    } else {
      out.GetSweatRateOverride().Invalidate();
    }
    if (in.TotalMetabolicRateOverride().present()) {
      io::Property::Marshall(in.TotalMetabolicRateOverride(), out.GetTotalMetabolicOverride());
    } else {
      out.GetTotalMetabolicOverride().Invalidate();
    }
    if (in.TotalWorkRateLevelOverride().present()) {
      io::Property::Marshall(in.TotalWorkRateLevelOverride(), out.GetTotalWorkRateLevelOverride());
    } else {
      out.GetTotalWorkRateLevelOverride().Invalidate();
    }
    if (in.SodiumLostToSweatOverride().present()) {
      io::Property::Marshall(in.SodiumLostToSweatOverride(), out.GetSodiumLostToSweatOverride());
    } else {
      out.GetSodiumLostToSweatOverride().Invalidate();
    }
    if (in.PotassiumLostToSweatOverride().present()) {
      io::Property::Marshall(in.PotassiumLostToSweatOverride(), out.GetPotassiumLostToSweatOverride());
    } else {
      out.GetPotassiumLostToSweatOverride().Invalidate();
    }
    if (in.ChlorideLostToSweatOverride().present()) {
      io::Property::Marshall(in.ChlorideLostToSweatOverride(), out.GetChlorideLostToSweatOverride());
    } else {
      out.GetChlorideLostToSweatOverride().Invalidate();
    }
    if (in.LeftAfferentArterioleResistanceOverride().present()) {
      io::Property::Marshall(in.LeftAfferentArterioleResistanceOverride(), out.GetLeftAfferentArterioleResistanceOverride());
    } else {
      out.GetLeftAfferentArterioleResistanceOverride().Invalidate();
    }
    if (in.LeftGlomerularFiltrationRateOverride().present()) {
      io::Property::Marshall(in.LeftGlomerularFiltrationRateOverride(), out.GetLeftGlomerularFiltrationRateOverride());
    } else {
      out.GetLeftGlomerularFiltrationRateOverride().Invalidate();
    }
    if (in.LeftReabsorptionRateOverride().present()) {
      io::Property::Marshall(in.LeftReabsorptionRateOverride(), out.GetLeftReaborptionRateOverride());
    } else {
      out.GetLeftReaborptionRateOverride().Invalidate();
    }
    if (in.RenalBloodFlowOverride().present()) {
      io::Property::Marshall(in.RenalBloodFlowOverride(), out.GetRenalBloodFlowOverride());
    } else {
      out.GetRenalBloodFlowOverride().Invalidate();
    }
    if (in.RenalPlasmaFlowOverride().present()) {
      io::Property::Marshall(in.RenalPlasmaFlowOverride(), out.GetRenalPlasmaFlowOverride());
    } else {
      out.GetRenalPlasmaFlowOverride().Invalidate();
    }
    if (in.RightAfferentArterioleResistanceOverride().present()) {
      io::Property::Marshall(in.RightAfferentArterioleResistanceOverride(), out.GetRightAfferentArterioleResistanceOverride());
    } else {
      out.GetRightAfferentArterioleResistanceOverride().Invalidate();
    }
    if (in.RightGlomerularFiltrationRateOverride().present()) {
      io::Property::Marshall(in.RightGlomerularFiltrationRateOverride(), out.GetRightGlomerularFiltrationRateOverride());
    } else {
      out.GetRightGlomerularFiltrationRateOverride().Invalidate();
    }
    if (in.RightReabsorptionRateOverride().present()) {
      io::Property::Marshall(in.RightReabsorptionRateOverride(), out.GetRightReaborptionRateOverride());
    } else {
      out.GetRightReaborptionRateOverride().Invalidate();
    }
    if (in.UrinationRateOverride().present()) {
      io::Property::Marshall(in.UrinationRateOverride(), out.GetUrinationRateOverride());
    } else {
      out.GetUrinationRateOverride().Invalidate();
    }
    if (in.UrineProductionRateOverride().present()) {
      io::Property::Marshall(in.UrineProductionRateOverride(), out.GetUrineProductionRateOverride());
    } else {
      out.GetUrineProductionRateOverride().Invalidate();
    }
    if (in.UrineOsmolalityOverride().present()) {
      io::Property::Marshall(in.UrineOsmolalityOverride(), out.GetUrineOsmolalityOverride());
    } else {
      out.GetUrineOsmolalityOverride().Invalidate();
    }
    if (in.UrineVolumeOverride().present()) {
      io::Property::Marshall(in.UrineVolumeOverride(), out.GetUrineVolumeOverride());
    } else {
      out.GetUrineVolumeOverride().Invalidate();
    }
    if (in.UrineUreaNitrogenConcentrationOverride().present()) {
      io::Property::Marshall(in.UrineUreaNitrogenConcentrationOverride(), out.GetUrineUreaNitrogenConcentrationOverride());
    } else {
      out.GetUrineUreaNitrogenConcentrationOverride().Invalidate();
    }
    if (in.ExpiratoryFlowOverride().present()) {
      io::Property::Marshall(in.ExpiratoryFlowOverride(), out.GetExpiratoryFlowOverride());
    } else {
      out.GetExpiratoryFlowOverride().Invalidate();
    }
    if (in.InspiratoryFlowOverride().present()) {
      io::Property::Marshall(in.InspiratoryFlowOverride(), out.GetInspiratoryFlowOverride());
    } else {
      out.GetInspiratoryFlowOverride().Invalidate();
    }
    if (in.PulmonaryComplianceOverride().present()) {
      io::Property::Marshall(in.PulmonaryComplianceOverride(), out.GetPulmonaryComplianceOverride());
    } else {
      out.GetPulmonaryComplianceOverride().Invalidate();
    }
    if (in.PulmonaryResistanceOverride().present()) {
      io::Property::Marshall(in.PulmonaryResistanceOverride(), out.GetPulmonaryResistanceOverride());
    } else {
      out.GetPulmonaryResistanceOverride().Invalidate();
    }
    if (in.RespirationRateOverride().present()) {
      io::Property::Marshall(in.RespirationRateOverride(), out.GetRespirationRateOverride());
    } else {
      out.GetRespirationRateOverride().Invalidate();
    }
    if (in.TidalVolumeOverride().present()) {
      io::Property::Marshall(in.TidalVolumeOverride(), out.GetTidalVolumeOverride());
    } else {
      out.GetTidalVolumeOverride().Invalidate();
    }
    if (in.TargetPulmonaryVentilationOverride().present()) {
      io::Property::Marshall(in.TargetPulmonaryVentilationOverride(), out.GetTargetPulmonaryVentilationOverride());
    } else {
      out.GetTargetPulmonaryVentilationOverride().Invalidate();
    }
    if (in.TotalAlveolarVentilationOverride().present()) {
      io::Property::Marshall(in.TotalAlveolarVentilationOverride(), out.GetTotalAlveolarVentilationOverride());
    } else {
      out.GetTotalAlveolarVentilationOverride().Invalidate();
    }
    if (in.TotalLungVolumeOverride().present()) {
      io::Property::Marshall(in.TotalLungVolumeOverride(), out.GetTotalLungVolumeOverride());
    } else {
      out.GetTotalLungVolumeOverride().Invalidate();
    }
    if (in.TotalPulmonaryVentilationOverride().present()) {
      io::Property::Marshall(in.TotalPulmonaryVentilationOverride(), out.GetTotalPulmonaryVentilationOverride());
    } else {
      out.GetTotalPulmonaryVentilationOverride().Invalidate();
    }
    if (in.ExtravascularFluidVolumeOverride().present()) {
      io::Property::Marshall(in.ExtravascularFluidVolumeOverride(), out.GetExtravascularFluidVolumeOverride());
    } else {
      out.GetExtravascularFluidVolumeOverride().Invalidate();
    }
    if (in.IntracellularFluidVolumeOverride().present()) {
      io::Property::Marshall(in.IntracellularFluidVolumeOverride(), out.GetIntracellularFluidVolumeOverride());
    } else {
      out.GetIntracellularFluidVolumeOverride().Invalidate();
    }
    if (in.LiverGlycogenOverride().present()) {
      io::Property::Marshall(in.LiverGlycogenOverride(), out.GetLiverGlycogenOverride());
    } else {
      out.GetLiverGlycogenOverride().Invalidate();
    }
    if (in.MuscleGlycogenOverride().present()) {
      io::Property::Marshall(in.MuscleGlycogenOverride(), out.GetMuscleGlycogenOverride());
    } else {
      out.GetMuscleGlycogenOverride().Invalidate();
    }
    if (in.StoredProteinOverride().present()) {
      io::Property::Marshall(in.StoredProteinOverride(), out.GetStoredProteinOverride());
    } else {
      out.GetStoredProteinOverride().Invalidate();
    }
    if (in.StoredFatOverride().present()) {
      io::Property::Marshall(in.StoredFatOverride(), out.GetStoredFatOverride());
    } else {
      out.GetStoredFatOverride().Invalidate();
    }

    if (!out.IsValid()) {
      throw CommonDataModelException("Unable to marshall SEOverride from the provided OverrideData");
    }
  }
  //----------------------------------------------------------------------------------
  void PatientActions::UnMarshall(const SEOverride& in, CDM::OverrideData& out)
  {
    Scenario::UnMarshall(static_cast<const SEPatientAction&>(in), static_cast<CDM::PatientActionData&>(out));
    if (in.HasOverrideState()) {
      out.State((in.m_OverrideState) ? CDM::enumOnOff::On : CDM::enumOnOff::Off);
    }
    if (in.HasOverrideConformance()) {
      out.Conformant((in.m_OverrideConformance) ? CDM::enumOnOff::On : CDM::enumOnOff::Off);
    }
    if (in.HasArterialPHOverride()) {
      io::Property::UnMarshall(*in.m_ArterialPHOR, out.ArterialBloodPHOverride());
    }
    if (in.HasVenousPHOverride()) {
      io::Property::UnMarshall(*in.m_VenousPHOR, out.VenousBloodPHOverride());
    }
    if (in.HasCO2SaturationOverride()) {
      io::Property::UnMarshall(*in.m_CO2SaturationOR, out.CarbonDioxideSaturationOverride());
    }
    if (in.HasCOSaturationOverride()) {
      io::Property::UnMarshall(*in.m_COSaturationOR, out.CarbonMonoxideSaturationOverride());
    }
    if (in.HasO2SaturationOverride()) {
      io::Property::UnMarshall(*in.m_O2SaturationOR, out.OxygenSaturationOverride());
    }
    if (in.HasPhosphateOverride()) {
      io::Property::UnMarshall(*in.m_PhosphateOR, out.PhosphateOverride());
    }
    if (in.HasWBCCountOverride()) {
      io::Property::UnMarshall(*in.m_WBCCountOR, out.WhiteBloodCellCountOverride());
    }
    if (in.HasTotalBilirubinOverride()) {
      io::Property::UnMarshall(*in.m_TotalBilirubinOR, out.TotalBilirubinOverride());
    }
    if (in.HasCalciumConcentrationOverride()) {
      io::Property::UnMarshall(*in.m_CalciumConcentrationOR, out.CalciumConcentrationOverride());
    }
    if (in.HasGlucoseConcentrationOverride()) {
      io::Property::UnMarshall(*in.m_GlucoseConcentrationOR, out.GlucoseConcentrationOverride());
    }
    if (in.HasLactateConcentrationOverride()) {
      io::Property::UnMarshall(*in.m_LactateConcentrationOR, out.LactateConcentrationOverride());
    }
    if (in.HasPotassiumConcentrationOverride()) {
      io::Property::UnMarshall(*in.m_PotassiumConcentrationOR, out.PotassiumConcentrationOverride());
    }
    if (in.HasSodiumConcentrationOverride()) {
      io::Property::UnMarshall(*in.m_SodiumConcentrationOR, out.SodiumConcentrationOverride());
    }
    if (in.HasBloodVolumeOverride()) {
      io::Property::UnMarshall(*in.m_BloodVolumeOR, out.BloodVolumeOverride());
    }
    if (in.HasCardiacOutputOverride()) {
      io::Property::UnMarshall(*in.m_CardiacOutputOR, out.CardiacOutputOverride());
    }
    if (in.HasDiastolicArterialPressureOverride()) {
      io::Property::UnMarshall(*in.m_DiastolicArtPressureOR, out.DiastolicArterialPressureOverride());
    }
    if (in.HasMAPOverride()) {
      io::Property::UnMarshall(*in.m_MeanArtPressureOR, out.MeanArterialPressureOverride());
    }
    if (in.HasHeartRateOverride()) {
      io::Property::UnMarshall(*in.m_HeartRateOR, out.HeartRateOverride());
    }
    if (in.HasHeartStrokeVolumeOverride()) {
      io::Property::UnMarshall(*in.m_HeartStrokeVolumeOR, out.HeartStrokeVolumeOverride());
    }
    if (in.HasSystolicArterialPressureOverride()) {
      io::Property::UnMarshall(*in.m_SystolicArtPressureOR, out.SystolicArterialPressureOverride());
    }
    if (in.HasInsulinSynthesisRateOverride()) {
      io::Property::UnMarshall(*in.m_InsulinSynthesisRateOR, out.InsulinSynthesisRateOverride());
    }
    if (in.HasGlucagonSynthesisRateOverride()) {
      io::Property::UnMarshall(*in.m_GlucagonSynthesisRateOR, out.GlucagonSynthesisRateOverride());
    }
    if (in.HasAchievedExerciseLevelOverride()) {
      io::Property::UnMarshall(*in.m_AcheivedExerciseLevelOR, out.AchievedExerciseLevelOverride());
    }
    if (in.HasCoreTemperatureOverride()) {
      io::Property::UnMarshall(*in.m_CoreTemperatureOR, out.CoreTemperatureOverride());
    }
    if (in.HasCreatinineProductionRateOverride()) {
      io::Property::UnMarshall(*in.m_CreatinineProductionRateOR, out.CreatinineProductionRateOverride());
    }
    if (in.HasExerciseMeanArterialPressureDeltaOverride()) {
      io::Property::UnMarshall(*in.m_ExerciseMeanArterialPressureDeltaOR, out.ExerciseMeanArterialPressureDeltaOverride());
    }
    if (in.HasFatigueLevelOverride()) {
      io::Property::UnMarshall(*in.m_FatigueLevelOR, out.FatigueLevelOverride());
    }
    if (in.HasLactateProductionRateOverride()) {
      io::Property::UnMarshall(*in.m_LactateProductionRateOR, out.LactateProductionRateOverride());
    }
    if (in.HasSkinTemperatureOverride()) {
      io::Property::UnMarshall(*in.m_SkinTemperatureOR, out.SkinTemperatureOverride());
    }
    if (in.HasSweatRateOverride()) {
      io::Property::UnMarshall(*in.m_SweatRateOR, out.SweatRateOverride());
    }
    if (in.HasTotalMetabolicOverride()) {
      io::Property::UnMarshall(*in.m_TotalMetabolicOR, out.TotalMetabolicRateOverride());
    }
    if (in.HasTotalWorkRateLevelOverride()) {
      io::Property::UnMarshall(*in.m_TotalWorkRateLevelOR, out.TotalWorkRateLevelOverride());
    }
    if (in.HasSodiumLostToSweatOverride()) {
      io::Property::UnMarshall(*in.m_SodiumLostToSweatOR, out.SodiumLostToSweatOverride());
    }
    if (in.HasPotassiumLostToSweatOverride()) {
      io::Property::UnMarshall(*in.m_PotassiumLostToSweatOR, out.PotassiumLostToSweatOverride());
    }
    if (in.HasChlorideLostToSweatOverride()) {
      io::Property::UnMarshall(*in.m_ChlorideLostToSweatOR, out.ChlorideLostToSweatOverride());
    }
    if (in.HasLeftAfferentArterioleResistanceOverride()) {
      io::Property::UnMarshall(*in.m_LAfferentArterioleResistOR, out.LeftAfferentArterioleResistanceOverride());
    }
    if (in.HasLeftGlomerularFiltrationRateOverride()) {
      io::Property::UnMarshall(*in.m__LeftGlomerularFiltrationOR, out.LeftGlomerularFiltrationRateOverride());
      ;
    }
    if (in.HasLeftReaborptionRateOverride()) {
      io::Property::UnMarshall(*in.m_LReabsorptionRateOR, out.LeftReabsorptionRateOverride());
    }
    if (in.HasRenalBloodFlowOverride()) {
      io::Property::UnMarshall(*in.m_RenalBloodFlowOR, out.RenalBloodFlowOverride());
    }
    if (in.HasRenalPlasmaFlowOverride()) {
      io::Property::UnMarshall(*in.m_RenalPlasmaOR, out.RenalPlasmaFlowOverride());
    }
    if (in.HasRightAfferentArterioleResistanceOverride()) {
      io::Property::UnMarshall(*in.m_RAfferentArterioleResistOR, out.RightAfferentArterioleResistanceOverride());
    }
    if (in.HasRightGlomerularFiltrationRateOverride()) {
      io::Property::UnMarshall(*in.m__RightGlomerularFiltrationOR, out.RightGlomerularFiltrationRateOverride());
    }
    if (in.HasRightReaborptionRateOverride()) {
      io::Property::UnMarshall(*in.m_RReabsorptionRateOR, out.RightReabsorptionRateOverride());
    }
    if (in.HasUrineProductionRateOverride()) {
      io::Property::UnMarshall(*in.m_UrineProductionRateOR, out.UrineProductionRateOverride());
    }
    if (in.HasUrineOsmolalityOverride()) {
      io::Property::UnMarshall(*in.m_UrineOsmolalityOR, out.UrineOsmolalityOverride());
    }
    if (in.HasUrineVolumeOverride()) {
      io::Property::UnMarshall(*in.m_UrineVolumeOR, out.UrineVolumeOverride());
    }
    if (in.HasUrineUreaNitrogenConcentrationOverride()) {
      io::Property::UnMarshall(*in.m_UrineUreaNitrogenConcentrationOverrideOR, out.UrineUreaNitrogenConcentrationOverride());
    }
    if (in.HasExpiratoryFlowOverride()) {
      io::Property::UnMarshall(*in.m_ExpiratoryFlowOR, out.ExpiratoryFlowOverride());
    }
    if (in.HasInspiratoryFlowOverride()) {
      io::Property::UnMarshall(*in.m_InspiratoryFlowOR, out.InspiratoryFlowOverride());
    }
    if (in.HasPulmonaryComplianceOverride()) {
      io::Property::UnMarshall(*in.m_PulmonaryComplianceOR, out.PulmonaryComplianceOverride());
    }
    if (in.HasPulmonaryResistanceOverride()) {
      io::Property::UnMarshall(*in.m_PulmonaryResistanceOR, out.PulmonaryResistanceOverride());
    }
    if (in.HasRespirationRateOverride()) {
      io::Property::UnMarshall(*in.m_RespirationRateOR, out.RespirationRateOverride());
    }
    if (in.HasTidalVolumeOverride()) {
      io::Property::UnMarshall(*in.m_TidalVolumeOR, out.TidalVolumeOverride());
    }
    if (in.HasTargetPulmonaryVentilationOverride()) {
      io::Property::UnMarshall(*in.m_TargetPulmonaryVentilationOR, out.TargetPulmonaryVentilationOverride());
    }
    if (in.HasTotalAlveolarVentilationOverride()) {
      io::Property::UnMarshall(*in.m_TotalAlveolarVentilationOR, out.TotalAlveolarVentilationOverride());
    }
    if (in.HasTotalLungVolumeOverride()) {
      io::Property::UnMarshall(*in.m_TotalLungVolumeOR, out.TotalLungVolumeOverride());
    }
    if (in.HasTotalPulmonaryVentilationOverride()) {
      io::Property::UnMarshall(*in.m_TotalPulmonaryVentilationOR, out.TotalPulmonaryVentilationOverride());
    }
    if (in.HasExtravascularFluidVolumeOverride()) {
      io::Property::UnMarshall(*in.m_ExtravascularFluidVolumeOR, out.ExtravascularFluidVolumeOverride());
    }
    if (in.HasIntracellularFluidVolumeOverride()) {
      io::Property::UnMarshall(*in.m_IntracellularFluidVolumeOR, out.IntracellularFluidVolumeOverride());
    }
    if (in.HasLiverGlycogenOverride()) {
      io::Property::UnMarshall(*in.m_LiverGlycogenOR, out.LiverGlycogenOverride());
    }
    if (in.HasMuscleGlycogenOverride()) {
      io::Property::UnMarshall(*in.m_MuscleGlycogenOR, out.MuscleGlycogenOverride());
    }
    if (in.HasStoredProteinOverride()) {
      io::Property::UnMarshall(*in.m_StoredProteinOR, out.StoredProteinOverride());
    }
    if (in.HasStoredFatOverride()) {
      io::Property::UnMarshall(*in.m_StoredFatOR, out.StoredFatOverride());
    }
  }
  //----------------------------------------------------------------------------------
}
}