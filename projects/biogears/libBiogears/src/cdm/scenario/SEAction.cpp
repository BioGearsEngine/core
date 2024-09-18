/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/
#include <biogears/cdm/scenario/SEAction.h>

#include "io/cdm/Actions.h"
#include "io/cdm/AnesthesiaActions.h"
#include "io/cdm/EnvironmentActions.h"
#include "io/cdm/EnvironmentConditions.h"
#include "io/cdm/InhalerActions.h"
#include "io/cdm/PatientActions.h"
#include "io/cdm/PatientAssessments.h"

#include <biogears/cdm/patient/actions/SEAcuteRespiratoryDistress.h>
#include <biogears/cdm/patient/actions/SEAcuteStress.h>
#include <biogears/cdm/patient/actions/SEAirwayObstruction.h>
#include <biogears/cdm/patient/actions/SEApnea.h>
#include <biogears/cdm/patient/actions/SEAsthmaAttack.h>
#include <biogears/cdm/patient/actions/SEBrainInjury.h>
#include <biogears/cdm/patient/actions/SEBronchoconstriction.h>
#include <biogears/cdm/patient/actions/SEBurnWound.h>
#include <biogears/cdm/patient/actions/SECardiacArrest.h>
#include <biogears/cdm/patient/actions/SEChestCompression.h>
#include <biogears/cdm/patient/actions/SEChestCompressionForce.h>
#include <biogears/cdm/patient/actions/SEChestCompressionForceScale.h>
#include <biogears/cdm/patient/actions/SEChestOcclusiveDressing.h>
#include <biogears/cdm/patient/actions/SEConsciousRespiration.h>
#include <biogears/cdm/patient/actions/SEConsumeNutrients.h>
#include <biogears/cdm/patient/actions/SEEbola.h>
#include <biogears/cdm/patient/actions/SEEscharotomy.h>
#include <biogears/cdm/patient/actions/SEExampleAction.h>
#include <biogears/cdm/patient/actions/SEExercise.h>
#include <biogears/cdm/patient/actions/SEHemorrhage.h>
#include <biogears/cdm/patient/actions/SEInfection.h>
#include <biogears/cdm/patient/actions/SEIntubation.h>
#include <biogears/cdm/patient/actions/SEMechanicalVentilation.h>
#include <biogears/cdm/patient/actions/SENasalCannula.h>
#include <biogears/cdm/patient/actions/SENeedleDecompression.h>
#include <biogears/cdm/patient/actions/SEOverride.h>
#include <biogears/cdm/patient/actions/SEPainStimulus.h>
#include <biogears/cdm/patient/actions/SEPatientAssessmentRequest.h>
#include <biogears/cdm/patient/actions/SEPericardialEffusion.h>
#include <biogears/cdm/patient/actions/SEPulmonaryShunt.h>
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
#include <biogears/cdm/scenario/SEAdvanceTime.h>
#include <biogears/cdm/scenario/SESerializeState.h>
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

namespace std {
template class vector<biogears::SEAction*>;
}

namespace biogears {
SEAction::SEAction(Logger* logger)
  : Loggable(logger)
{
  m_Comment = "";
}
//-----------------------------------------------------------------------------
SEAction::~SEAction()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEAction::Clear()
{
  m_Comment = "";
}
//-----------------------------------------------------------------------------
SEAction* SEAction::newFromBind(const CDM::ActionData& data, SESubstanceManager& substances, std::default_random_engine* rd)
{
  std::stringstream ss;
  SESubstance* substance;
  SESubstanceCompound* compound;

  CDM::ActionData* action = (CDM::ActionData*)&data;
  CDM::AdvanceTimeData* advData = dynamic_cast<CDM::AdvanceTimeData*>(action);
  if (advData != nullptr) {
    SEAdvanceTime* a = new SEAdvanceTime();
    io::Actions::UnMarshall(*advData, *a, rd);

    return a;
  }

  if (CDM::SerializeStateData* stData = dynamic_cast<CDM::SerializeStateData*>(action)) {
    SESerializeState* a = new SESerializeState();
    io::Actions::UnMarshall(*stData, *a);
    return a;
  }

  if (dynamic_cast<CDM::EnvironmentActionData*>(action) != nullptr) {
    if (dynamic_cast<CDM::EnvironmentChangeData*>(action) != nullptr) {
      SEEnvironmentChange* a = new SEEnvironmentChange(substances);
      io::EnvironmentActions::UnMarshall(*(CDM::EnvironmentChangeData*)action, *a);
      return a;
    }
    if (dynamic_cast<CDM::ThermalApplicationData*>(action) != nullptr) {
      SEThermalApplication* a = new SEThermalApplication();
      io::EnvironmentActions::UnMarshall(*(CDM::ThermalApplicationData*)action, *a);
      return a;
    }
    substances.GetLogger()->Error("Unsupported Environment Action Received", "SEScenario::Load");
  }

  if (dynamic_cast<CDM::PatientActionData*>(action) != nullptr) {
    if (dynamic_cast<CDM::PatientAssessmentRequestData*>(action) != nullptr) {
      SEPatientAssessmentRequest* a = new SEPatientAssessmentRequest();
      io::PatientActions::UnMarshall(*(CDM::PatientAssessmentRequestData*)action, *a, rd);
      return a;
    }

    if (CDM::AcuteRespiratoryDistressData* ardsData = dynamic_cast<CDM::AcuteRespiratoryDistressData*>(action)) {
      SEAcuteRespiratoryDistress* ards = new SEAcuteRespiratoryDistress();
      io::PatientActions::UnMarshall(*ardsData, *ards, rd);
      return ards;
    }

    if (CDM::AcuteStressData* aStressData = dynamic_cast<CDM::AcuteStressData*>(action)) {
      SEAcuteStress* a = new SEAcuteStress();
      io::PatientActions::UnMarshall(*aStressData, *a, rd);
      return a;
    }

    if (CDM::ExampleActionData* expAction = dynamic_cast<CDM::ExampleActionData*>(action)) {
      SEExampleAction* a = new SEExampleAction();
      io::PatientActions::UnMarshall(*expAction, *a, rd);
      return a;
    }

    if (CDM::AirwayObstructionData* airObData = dynamic_cast<CDM::AirwayObstructionData*>(action)) {
      SEAirwayObstruction* a = new SEAirwayObstruction();
      io::PatientActions::UnMarshall(*airObData, *a, rd);
      return a;
    }

    if (CDM::ApneaData* apneaData = dynamic_cast<CDM::ApneaData*>(action)) {
      SEApnea* a = new SEApnea();
      io::PatientActions::UnMarshall(*apneaData, *a, rd);
      return a;
    }

    if (CDM::BrainInjuryData* brainInjData = dynamic_cast<CDM::BrainInjuryData*>(action)) {
      SEBrainInjury* a = new SEBrainInjury();
      io::PatientActions::UnMarshall(*brainInjData, *a, rd);
      return a;
    }

    if (CDM::BurnWoundData* burnData = dynamic_cast<CDM::BurnWoundData*>(action)) {
      SEBurnWound* burn = new SEBurnWound();
      io::PatientActions::UnMarshall(*burnData, *burn, rd);
      return burn;
    }

    if (CDM::BronchoconstrictionData* bconData = dynamic_cast<CDM::BronchoconstrictionData*>(action)) {
      SEBronchoconstriction* a = new SEBronchoconstriction();
      io::PatientActions::UnMarshall(*bconData, *a, rd);
      return a;
    }

    if (CDM::CardiacArrestData* carrestData = dynamic_cast<CDM::CardiacArrestData*>(action)) {
      SECardiacArrest* a = new SECardiacArrest();
      io::PatientActions::UnMarshall(*carrestData, *a, rd);
      return a;
    }

    if (CDM::AsthmaAttackData* asthmaData = dynamic_cast<CDM::AsthmaAttackData*>(action)) {
      SEAsthmaAttack* a = new SEAsthmaAttack();
      io::PatientActions::UnMarshall(*asthmaData, *a, rd);
      return a;
    }

    if (CDM::ChestCompressionData* cprData = dynamic_cast<CDM::ChestCompressionData*>(action)) {

      if (CDM::ChestCompressionForceData* cprForce = dynamic_cast<CDM::ChestCompressionForceData*>(cprData)) {
        SEChestCompressionForce* a = new SEChestCompressionForce();
        io::PatientActions::UnMarshall(*cprForce, *a, rd);
        return a;
      }

      if (CDM::ChestCompressionForceScaleData* cprScale = dynamic_cast<CDM::ChestCompressionForceScaleData*>(cprData)) {
        SEChestCompressionForceScale* a = new SEChestCompressionForceScale();
        io::PatientActions::UnMarshall(*cprScale, *a, rd);
        return a;
      }
    }

    if (CDM::ChestOcclusiveDressingData* codData = dynamic_cast<CDM::ChestOcclusiveDressingData*>(action)) {
      SEChestOcclusiveDressing* a = new SEChestOcclusiveDressing();
      io::PatientActions::UnMarshall(*codData, *a, rd);
      return a;
    }

    if (CDM::ConsciousRespirationData* conRespData = dynamic_cast<CDM::ConsciousRespirationData*>(action)) {
      SEConsciousRespiration* a = new SEConsciousRespiration();
      io::PatientActions::UnMarshall(*conRespData, *a, rd);
      return a;
    }

    if (CDM::ConsumeNutrientsData* consumeData = dynamic_cast<CDM::ConsumeNutrientsData*>(action)) {
      SEConsumeNutrients* a = new SEConsumeNutrients();
      io::PatientActions::UnMarshall(*consumeData, *a, rd);
      return a;
    }

    if (CDM::EbolaData* ebolaData = dynamic_cast<CDM::EbolaData*>(action)) {
      SEEbola* ebola = new SEEbola();
      io::PatientActions::UnMarshall(*ebolaData, *ebola, rd);
      return ebola;
    }

    if (CDM::EscharotomyData* escharotomyData = dynamic_cast<CDM::EscharotomyData*>(action)) {
      SEEscharotomy* escharotomy = new SEEscharotomy();
      io::PatientActions::UnMarshall(*escharotomyData, *escharotomy, rd);
      return escharotomy;
    }

    if (CDM::ExerciseData* exerciseData = dynamic_cast<CDM::ExerciseData*>(action)) {
      SEExercise* a = new SEExercise();
      io::PatientActions::UnMarshall(*exerciseData, *a, rd);
      return a;
    }

    if (CDM::IntubationData* intub8Data = dynamic_cast<CDM::IntubationData*>(action)) {
      SEIntubation* a = new SEIntubation();
      io::PatientActions::UnMarshall(*intub8Data, *a, rd);
      return a;
    }

    if (CDM::MechanicalVentilationData* mechVentData = dynamic_cast<CDM::MechanicalVentilationData*>(action)) {
      SEMechanicalVentilation* a = new SEMechanicalVentilation();
      io::PatientActions::UnMarshall(*mechVentData, substances, *a, rd);
      return a;
    }

    if (CDM::NeedleDecompressionData* needlData = dynamic_cast<CDM::NeedleDecompressionData*>(action)) {
      SENeedleDecompression* a = new SENeedleDecompression();
      io::PatientActions::UnMarshall(*needlData, *a, rd);
      return a;
    }

    if (CDM::OverrideData* overrideData = dynamic_cast<CDM::OverrideData*>(action)) {
      SEOverride* a = new SEOverride();
      io::PatientActions::UnMarshall(*overrideData, *a, rd);
      return a;
    }

    if (CDM::HemorrhageData* hemData = dynamic_cast<CDM::HemorrhageData*>(action)) {
      SEHemorrhage* a = new SEHemorrhage();
      io::PatientActions::UnMarshall(*hemData, *a, rd);
      return a;
    }

    if (CDM::InfectionData* infData = dynamic_cast<CDM::InfectionData*>(action)) {
      SEInfection* infect = new SEInfection();
      io::PatientActions::UnMarshall(*infData, *infect, rd);
      return infect;
    }

    if (CDM::NasalCannulaData* nasalCannulaData = dynamic_cast<CDM::NasalCannulaData*>(action)) {
      SENasalCannula* a = new SENasalCannula();
      io::PatientActions::UnMarshall(*nasalCannulaData, *a, rd);
      return a;
    }

    if (CDM::PainStimulusData* painData = dynamic_cast<CDM::PainStimulusData*>(action)) {
      SEPainStimulus* a = new SEPainStimulus();
      io::PatientActions::UnMarshall(*painData, *a, rd);
      return a;
    }

    if (CDM::PericardialEffusionData* pericData = dynamic_cast<CDM::PericardialEffusionData*>(action)) {
      SEPericardialEffusion* a = new SEPericardialEffusion();
      io::PatientActions::UnMarshall(*pericData, *a, rd);
      return a;
    }

    if (CDM::PulmonaryShuntData* pulmData = dynamic_cast<CDM::PulmonaryShuntData*>(action)) {
      SEPulmonaryShunt* a = new SEPulmonaryShunt();
      io::PatientActions::UnMarshall(*pulmData, *a, rd);
      return a;
    }

    if (CDM::TensionPneumothoraxData* pneumoData = dynamic_cast<CDM::TensionPneumothoraxData*>(action)) {
      SETensionPneumothorax* a = new SETensionPneumothorax();
      io::PatientActions::UnMarshall(*pneumoData, *a, rd);
      return a;
    }

    if (CDM::RadiationAbsorbedDoseData* radabs = dynamic_cast<CDM::RadiationAbsorbedDoseData*>(action)) {
      SERadiationAbsorbedDose* a = new SERadiationAbsorbedDose();
      io::PatientActions::UnMarshall(*radabs, *a, rd);
      return a;
    }

    if (CDM::SubstanceBolusData* bolusData = dynamic_cast<CDM::SubstanceBolusData*>(action)) {
      substance = substances.GetSubstance(bolusData->Substance());
      if (substance == nullptr) {
        ss << "Unknown substance : " << bolusData->Substance();
        substances.GetLogger()->Fatal(ss.str(), "SEScenario::Load");
      }
      SESubstanceBolus* a = new SESubstanceBolus(*substance);
      io::PatientActions::UnMarshall(*bolusData, *a, rd);
      return a;
    }

    if (CDM::SubstanceNasalDoseData* nasalData = dynamic_cast<CDM::SubstanceNasalDoseData*>(action)) {
      substance = substances.GetSubstance(nasalData->Substance());
      if (substance == nullptr) {
        ss << "Unknown substance : " << nasalData->Substance();
        substances.GetLogger()->Fatal(ss.str(), "SEScenario::Load");
      }
      SESubstanceNasalDose* nd = new SESubstanceNasalDose(*substance);
      io::PatientActions::UnMarshall(*nasalData, *nd, rd);
      return nd;
    }

    if (CDM::SubstanceOralDoseData* oralData = dynamic_cast<CDM::SubstanceOralDoseData*>(action)) {
      substance = substances.GetSubstance(oralData->Substance());
      if (substance == nullptr) {
        ss << "Unknown substance : " << oralData->Substance();
        substances.GetLogger()->Fatal(ss.str(), "SEScenario::Load");
      }
      SESubstanceOralDose* od = new SESubstanceOralDose(*substance);
      io::PatientActions::UnMarshall(*oralData, *od, rd);
      return od;
    }

    if (CDM::SleepData* sleep = dynamic_cast<CDM::SleepData*>(action)) {
      SESleep* a = new SESleep();
      io::PatientActions::UnMarshall(*sleep, *a, rd);
      return a;
    }

    if (CDM::SubstanceInfusionData* subInfuzData = dynamic_cast<CDM::SubstanceInfusionData*>(action)) {
      substance = substances.GetSubstance(subInfuzData->Substance());
      if (substance == nullptr) {
        ss << "Unknown substance : " << subInfuzData->Substance();
        substances.GetLogger()->Fatal(ss.str(), "SEScenario::Load");
      }
      SESubstanceInfusion* a = new SESubstanceInfusion(*substance);
      io::PatientActions::UnMarshall(*subInfuzData, *a, rd);
      return a;
    }

    if (CDM::SubstanceCompoundInfusionData* subCInfuzData = dynamic_cast<CDM::SubstanceCompoundInfusionData*>(action)) {
      compound = substances.GetCompound(subCInfuzData->SubstanceCompound());
      if (compound == nullptr) {
        ss << "Unknown substance : " << subCInfuzData->SubstanceCompound();
        substances.GetLogger()->Fatal(ss.str(), "SEScenario::Load");
      }
      SESubstanceCompoundInfusion* a = new SESubstanceCompoundInfusion(*compound);
      io::PatientActions::UnMarshall(*subCInfuzData, *a, rd);
      return a;
    }

    if (CDM::TourniquetData* tourniquetData = dynamic_cast<CDM::TourniquetData*>(action)) {
      SETourniquet* tourniquet = new SETourniquet();
      io::PatientActions::UnMarshall(*tourniquetData, *tourniquet, rd);
      return tourniquet;
    }

    if (CDM::UrinateData* urinateData = dynamic_cast<CDM::UrinateData*>(action)) {
      SEUrinate* a = new SEUrinate();
      io::PatientActions::UnMarshall(*urinateData, *a, rd);
      return a;
    }
    substances.GetLogger()->Error("Unsupported Patient Action Received", "SEScenario::Load");
  } else if (dynamic_cast<CDM::AnesthesiaMachineActionData*>(action) != nullptr) {

    if (auto anConfig = dynamic_cast<CDM::AnesthesiaMachineConfigurationData*>(action)) {
      SEAnesthesiaMachineConfiguration* a = new SEAnesthesiaMachineConfiguration(substances);
      io::AnesthesiaActions::UnMarshall(*anConfig, *a, rd);
      return a;
    }

    if (auto anO2WallLoss = dynamic_cast<CDM::OxygenWallPortPressureLossData*>(action)) {
      SEOxygenWallPortPressureLoss* a = new SEOxygenWallPortPressureLoss();
      io::AnesthesiaActions::UnMarshall(*anO2WallLoss, *a, rd);
      return a;
    }

    if (auto anO2TankLoss = dynamic_cast<CDM::OxygenTankPressureLossData*>(action)) {
      SEOxygenTankPressureLoss* a = new SEOxygenTankPressureLoss();
      io::AnesthesiaActions::UnMarshall(*anO2TankLoss, *a, rd);
      return a;
    }

    if (auto anExLeak = dynamic_cast<CDM::ExpiratoryValveLeakData*>(action)) {
      SEExpiratoryValveLeak* a = new SEExpiratoryValveLeak();
      io::AnesthesiaActions::UnMarshall(*anExLeak, *a, rd);
      return a;
    }

    if (auto anExObs = dynamic_cast<CDM::ExpiratoryValveObstructionData*>(action)) {
      SEExpiratoryValveObstruction* a = new SEExpiratoryValveObstruction();
      io::AnesthesiaActions::UnMarshall(*anExObs, *a, rd);
      return a;
    }

    if (auto anInLeak = dynamic_cast<CDM::InspiratoryValveLeakData*>(action)) {
      SEInspiratoryValveLeak* a = new SEInspiratoryValveLeak();
      io::AnesthesiaActions::UnMarshall(*anInLeak, *a, rd);
      return a;
    }

    if (auto anInObs = dynamic_cast<CDM::InspiratoryValveObstructionData*>(action)) {
      SEInspiratoryValveObstruction* a = new SEInspiratoryValveObstruction();
      io::AnesthesiaActions::UnMarshall(*anInObs, *a, rd);
      return a;
    }

    if (auto anMskLeak = dynamic_cast<CDM::MaskLeakData*>(action)) {
      SEMaskLeak* a = new SEMaskLeak();
      io::AnesthesiaActions::UnMarshall(*anMskLeak, *a, rd);
      return a;
    }

    if (auto anSodaFail = dynamic_cast<CDM::SodaLimeFailureData*>(action)) {
      SESodaLimeFailure* a = new SESodaLimeFailure();
      io::AnesthesiaActions::UnMarshall(*anSodaFail, *a, rd);
      return a;
    }

    if (auto anTubLeak = dynamic_cast<CDM::TubeCuffLeakData*>(action)) {
      SETubeCuffLeak* a = new SETubeCuffLeak();
      io::AnesthesiaActions::UnMarshall(*anTubLeak, *a, rd);
      return a;
    }

    if (auto anVapFail = dynamic_cast<CDM::VaporizerFailureData*>(action)) {
      SEVaporizerFailure* a = new SEVaporizerFailure();
      io::AnesthesiaActions::UnMarshall(*anVapFail, *a, rd);
      return a;
    }

    if (auto anVentLoss = dynamic_cast<CDM::VentilatorPressureLossData*>(action)) {
      SEVentilatorPressureLoss* a = new SEVentilatorPressureLoss();
      io::AnesthesiaActions::UnMarshall(*anVentLoss, *a, rd);
      return a;
    }

    if (auto anYDisc = dynamic_cast<CDM::YPieceDisconnectData*>(action)) {
      SEYPieceDisconnect* a = new SEYPieceDisconnect();
      io::AnesthesiaActions::UnMarshall(*anYDisc, *a, rd);
      return a;
    }
    substances.GetLogger()->Error("Unsupported Anesthesia Machine Action Received", "SEScenario::Load");
  } else if (dynamic_cast<CDM::InhalerActionData*>(action) != nullptr) {
    CDM::InhalerConfigurationData* inhaleConfig = dynamic_cast<CDM::InhalerConfigurationData*>(action);
    if (inhaleConfig != nullptr) {
      SEInhalerConfiguration* a = new SEInhalerConfiguration(substances);
      io::InhalerActions::UnMarshall(*inhaleConfig, *a);
      return a;
    }
  }

  if (substances.GetLogger() != nullptr)
    substances.GetLogger()->Error("Unsupported Action Received", "SEAction::newFromBind");
  return nullptr;
}
//-----------------------------------------------------------------------------
const char* SEAction::GetComment() const
{
  return m_Comment.c_str();
}
//-----------------------------------------------------------------------------
void SEAction::SetComment(const char* comment)
{
  m_Comment = comment;
}
//-----------------------------------------------------------------------------
void SEAction::SetComment(const std::string& comment)
{
  m_Comment = comment;
}
//-----------------------------------------------------------------------------
bool SEAction::HasComment() const
{
  return m_Comment.empty() ? false : true;
}
//-----------------------------------------------------------------------------
void SEAction::InvalidateComment()
{
  m_Comment = "";
}
//-----------------------------------------------------------------------------
const char* SEAction::ToString() const
{
  std::stringstream ss;
  this->ToString(ss);
  m_StringRepresnetation = ss.str();
  return m_StringRepresnetation.c_str();
}
//-----------------------------------------------------------------------------
//-------------------------------------------------------------------------------
bool SEAction::operator==(const SEAction& rhs) const
{
  return m_Comment == rhs.m_Comment;
}
bool SEAction::operator!=(const SEAction& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}