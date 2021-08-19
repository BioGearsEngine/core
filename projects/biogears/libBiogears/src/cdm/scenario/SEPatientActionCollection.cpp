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
#include <biogears/cdm/scenario/SEPatientActionCollection.h>

#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>

namespace std {
template class map<string, biogears::SEHemorrhage*>;
template class map<string, biogears::SEHemorrhage*>::iterator;
template class map<string, biogears::SEHemorrhage*>::const_iterator;
template class map<string, biogears::SETourniquet*>;
template class map<string, biogears::SEEscharotomy*>;
template class map<string, biogears::SEPainStimulus*>;
template class map<const biogears::SESubstance*, biogears::SESubstanceBolus*>;
template class map<const biogears::SESubstance*, biogears::SESubstanceInfusion*>;
template class map<const biogears::SESubstance*, biogears::SESubstanceOralDose*>;
template class map<const biogears::SESubstance*, biogears::SESubstanceNasalDose*>;
template class map<const biogears::SESubstanceCompound*, biogears::SESubstanceCompoundInfusion*>;
}

namespace biogears {
SEPatientActionCollection::SEPatientActionCollection(SESubstanceManager& substances)
  : Loggable(substances.GetLogger())
  , m_Substances(substances)
{
  m_AcuteRespiratoryDistress = nullptr;
  m_AcuteStress = nullptr;
  m_AirwayObstruction = nullptr;
  m_Apnea = nullptr;
  m_AsthmaAttack = nullptr;
  m_BrainInjury = nullptr;
  m_Bronchoconstriction = nullptr;
  m_BurnWound = nullptr;
  m_CardiacArrest = nullptr;
  m_ChestCompression = nullptr;
  m_ConsciousRespiration = nullptr;
  m_ConsumeNutrients = nullptr;
  m_LeftChestOcclusiveDressing = nullptr;
  m_RightChestOcclusiveDressing = nullptr;
  m_Exercise = nullptr;
  m_Infection = nullptr;
  m_Intubation = nullptr;
  m_MechanicalVentilation = nullptr;
  m_LeftNeedleDecompression = nullptr;
  m_RightNeedleDecompression = nullptr;
  m_PericardialEffusion = nullptr;
  m_PulmonaryShunt = nullptr;
  m_Sleep = nullptr;
  m_LeftOpenTensionPneumothorax = nullptr;
  m_LeftClosedTensionPneumothorax = nullptr;
  m_RightOpenTensionPneumothorax = nullptr;
  m_RightClosedTensionPneumothorax = nullptr;
  m_Urinate = nullptr;
  m_OverrideAction = nullptr;
}
//-------------------------------------------------------------------------------
SEPatientActionCollection::~SEPatientActionCollection()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::Clear()
{
  RemoveAcuteRespiratoryDistress();
  RemoveAcuteStress();
  RemoveAirwayObstruction();
  RemoveApnea();
  RemoveAsthmaAttack();
  RemoveBrainInjury();
  RemoveBronchoconstriction();
  RemoveBurnWound();
  RemoveChestCompression();
  RemoveCardiacArrest();
  RemoveConsciousRespiration();
  RemoveConsumeNutrients();
  RemoveLeftChestOcclusiveDressing();
  RemoveRightChestOcclusiveDressing();
  RemoveExercise();
  RemoveInfection();
  RemoveIntubation();
  RemoveMechanicalVentilation();
  RemoveLeftNeedleDecompression();
  RemoveRightNeedleDecompression();
  RemovePericardialEffusion();
  RemovePulmonaryShunt();
  RemoveSleepState();
  RemoveLeftOpenTensionPneumothorax();
  RemoveLeftClosedTensionPneumothorax();
  RemoveRightOpenTensionPneumothorax();
  RemoveRightClosedTensionPneumothorax();
  RemoveUrinate();
  RemoveOverride();

  DELETE_MAP_SECOND(m_Escharotomies);
  DELETE_MAP_SECOND(m_Hemorrhages);
  DELETE_MAP_SECOND(m_PainStimuli);
  DELETE_MAP_SECOND(m_SubstanceBolus);
  DELETE_MAP_SECOND(m_SubstanceInfusions);
  DELETE_MAP_SECOND(m_SubstanceOralDoses);
  DELETE_MAP_SECOND(m_SubstanceCompoundInfusions);
  DELETE_MAP_SECOND(m_Tourniquets);
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::Unload(std::vector<CDM::ActionData*>& to)
{
  if (HasAcuteRespiratoryDistress()) {
    to.push_back(GetAcuteRespiratoryDistress()->Unload());
  }
  if (HasAcuteStress()) {
    to.push_back(GetAcuteStress()->Unload());
  }
  if (HasAirwayObstruction()) {
    to.push_back(GetAirwayObstruction()->Unload());
  }
  if (HasApnea()) {
    to.push_back(GetApnea()->Unload());
  }
  if (HasAsthmaAttack()) {
    to.push_back(GetAsthmaAttack()->Unload());
  }
  if (HasBrainInjury()) {
    to.push_back(GetBrainInjury()->Unload());
  }
  if (HasBronchoconstriction()) {
    to.push_back(GetBronchoconstriction()->Unload());
  }
  if (HasBurnWound()) {
    to.push_back(GetBurnWound()->Unload());
  }
  if (HasCardiacArrest()) {
    to.push_back(GetCardiacArrest()->Unload());
  }
  if (HasChestCompressionForce()) {
    to.push_back(GetChestCompressionForce()->Unload());
  }
  if (HasChestCompressionForceScale()) {
    to.push_back(GetChestCompressionForceScale()->Unload());
  }
  if (HasLeftChestOcclusiveDressing()) {
    to.push_back(GetLeftChestOcclusiveDressing()->Unload());
  }
  if (HasRightChestOcclusiveDressing()) {
    to.push_back(GetRightChestOcclusiveDressing()->Unload());
  }
  if (HasConsciousRespiration()) {
    to.push_back(GetConsciousRespiration()->Unload());
  }
  if (HasConsumeNutrients()) {
    to.push_back(GetConsumeNutrients()->Unload());
  }
  if (HasEscharotomy()) {
    for (auto itr : GetEscharotomies()) {
      to.push_back(itr.second->Unload());
    }
  }
  if (HasExercise()) {
    to.push_back(GetExercise()->Unload());
  }
  if (HasHemorrhage()) {
    for (auto itr : GetHemorrhages()) {
      to.push_back(itr.second->Unload());
    }
  }
  if (HasInfection()) {
    to.push_back(GetInfection()->Unload());
  }
  if (HasIntubation()) {
    to.push_back(GetIntubation()->Unload());
  }
  if (HasMechanicalVentilation()) {
    to.push_back(GetMechanicalVentilation()->Unload());
  }
  if (HasLeftNeedleDecompression()) {
    to.push_back(GetLeftNeedleDecompression()->Unload());
  }
  if (HasRightNeedleDecompression()) {
    to.push_back(GetRightNeedleDecompression()->Unload());
  }
  if (HasPainStimulus()) {
    for (auto itr : GetPainStimuli()) {
      to.push_back(itr.second->Unload());
    }
  }
  if (HasPericardialEffusion()) {
    to.push_back(GetPericardialEffusion()->Unload());
  }
  if (HasPulmonaryShunt()) {
    to.push_back(GetPulmonaryShunt()->Unload());
  }
  if (HasSleepState()) {
    to.push_back(GetSleepState()->Unload());
  }
  if (HasLeftClosedTensionPneumothorax()) {
    to.push_back(GetLeftClosedTensionPneumothorax()->Unload());
  }
  if (HasLeftOpenTensionPneumothorax()) {
    to.push_back(GetLeftOpenTensionPneumothorax()->Unload());
  }
  if (HasRightClosedTensionPneumothorax()) {
    to.push_back(GetRightClosedTensionPneumothorax()->Unload());
  }
  if (HasRightOpenTensionPneumothorax()) {
    to.push_back(GetRightOpenTensionPneumothorax()->Unload());
  }
  for (auto itr : GetSubstanceBoluses()) {
    to.push_back(itr.second->Unload());
  }
  for (auto itr : GetSubstanceInfusions()) {
    to.push_back(itr.second->Unload());
  }
  for (auto itr : GetSubstanceOralDoses()) {
    to.push_back(itr.second->Unload());
  }
  for (auto itr : GetSubstanceCompoundInfusions()) {
    to.push_back(itr.second->Unload());
  }
  if (HasTourniquet()) {
    for (auto itr : GetTourniquets()) {
      to.push_back(itr.second->Unload());
    }
  }
  if (HasUrinate()) {
    to.push_back(GetUrinate()->Unload());
  }
  if (HasOverride()) {
    to.push_back((GetOverride()->Unload()));
  }
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::ProcessAction(const SEPatientAction& action)
{
  if (!IsValid(action)) {
    return false;
  }
  CDM::PatientActionData* bind = action.Unload();
  bool b = ProcessAction(*bind);
  delete bind;
  return b;
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::ProcessAction(const CDM::PatientActionData& action)
{
  const CDM::PatientAssessmentRequestData* patientAssessment = dynamic_cast<const CDM::PatientAssessmentRequestData*>(&action);
  if (patientAssessment != nullptr) {
    // TODO just add this to a list?
    // Not doing anything with this, assessment actions
    // are currently only being handled by the Engine ScenarioExec methods.
    // Which probably just write out the assessment requested to a file
    // Assessments are really a real time API thing, not really a scenario thing
    // If they are a scenario thing, its just to write out the data.
    return true; //?
  }

  // We are creating our own copy of these actions
  // There can be multiple actions in a Scenario that are associated
  // with a specific function, such as hemorrhage, and we only need
  // to have a single action in our list associated with a hemorrhage.
  // We just overwrite our saved action with the current state of that action
  // So if the 4 hemorrhage actions come in, and lower the rate each time for a
  // certain compartment, we just update the single hemorrhage action associated
  // with that compartment.
  // SO, we make our own copy and manage that copy (i.e. by updating a single action)

  const CDM::AcuteRespiratoryDistressData* ards = dynamic_cast<const CDM::AcuteRespiratoryDistressData*>(&action);
  if (ards != nullptr) {
    if (m_AcuteRespiratoryDistress == nullptr) {
      m_AcuteRespiratoryDistress = new SEAcuteRespiratoryDistress();
    }
    m_AcuteRespiratoryDistress->Load(*ards);
    if (!m_AcuteRespiratoryDistress->IsActive()) {
      RemoveAcuteRespiratoryDistress();
      return true;
    }
    return IsValid(*m_AcuteRespiratoryDistress);
  }

  const CDM::AcuteStressData* aStress = dynamic_cast<const CDM::AcuteStressData*>(&action);
  if (aStress != nullptr) {
    if (m_AcuteStress == nullptr) {
      m_AcuteStress = new SEAcuteStress();
    }
    m_AcuteStress->Load(*aStress);
    if (!m_AcuteStress->IsActive()) {
      RemoveAcuteStress();
      return true;
    }
    return IsValid(*m_AcuteStress);
  }

  const CDM::AirwayObstructionData* airwayObst = dynamic_cast<const CDM::AirwayObstructionData*>(&action);
  if (airwayObst != nullptr) {
    if (m_AirwayObstruction == nullptr) {
      m_AirwayObstruction = new SEAirwayObstruction();
    }
    m_AirwayObstruction->Load(*airwayObst);
    if (!m_AirwayObstruction->IsActive()) {
      RemoveAirwayObstruction();
      return true;
    }
    return IsValid(*m_AirwayObstruction);
  }

  const CDM::ApneaData* apnea = dynamic_cast<const CDM::ApneaData*>(&action);
  if (apnea != nullptr) {
    if (m_Apnea == nullptr) {
      m_Apnea = new SEApnea();
    }
    m_Apnea->Load(*apnea);
    if (!m_Apnea->IsActive()) {
      RemoveApnea();
      return true;
    }
    return IsValid(*m_Apnea);
  }

  const CDM::AsthmaAttackData* asthmaattack = dynamic_cast<const CDM::AsthmaAttackData*>(&action);
  if (asthmaattack != nullptr) {
    if (m_AsthmaAttack == nullptr) {
      m_AsthmaAttack = new SEAsthmaAttack();
    }
    m_AsthmaAttack->Load(*asthmaattack);
    if (!m_AsthmaAttack->IsActive()) {
      RemoveAsthmaAttack();
      return true;
    }
    return IsValid(*m_AsthmaAttack);
  }

  const CDM::BrainInjuryData* brainInjury = dynamic_cast<const CDM::BrainInjuryData*>(&action);
  if (brainInjury != nullptr) {
    if (m_BrainInjury == nullptr) {
      m_BrainInjury = new SEBrainInjury();
    }
    m_BrainInjury->Load(*brainInjury);
    if (!m_BrainInjury->IsActive()) {
      RemoveBrainInjury();
      return true;
    }
    return IsValid(*m_BrainInjury);
  }

  const CDM::BronchoconstrictionData* bronchoconstr = dynamic_cast<const CDM::BronchoconstrictionData*>(&action);
  if (bronchoconstr != nullptr) {
    if (m_Bronchoconstriction == nullptr) {
      m_Bronchoconstriction = new SEBronchoconstriction();
    }
    m_Bronchoconstriction->Load(*bronchoconstr);
    if (!m_Bronchoconstriction->IsActive()) {
      RemoveBronchoconstriction();
      return true;
    }
    return IsValid(*m_Bronchoconstriction);
  }

  const CDM::BurnWoundData* burn = dynamic_cast<const CDM::BurnWoundData*>(&action);
  if (burn != nullptr) {
    if (m_BurnWound == nullptr) {
      m_BurnWound = new SEBurnWound();
    }
    m_BurnWound->Load(*burn);
    if (!m_BurnWound->IsActive()) {
      RemoveBurnWound();
      return true;
    }
    return IsValid(*m_BurnWound);
  }

  const CDM::CardiacArrestData* cardiacarrest = dynamic_cast<const CDM::CardiacArrestData*>(&action);
  if (cardiacarrest != nullptr) {

    if (m_CardiacArrest == nullptr && cardiacarrest->State() == CDM::enumOnOff::Off) {
      return true; //Ignore :CardiacArrest::Off request when no :CardiacArrest Event exist.
    }

    if (m_CardiacArrest == nullptr) {
      m_CardiacArrest = new SECardiacArrest();
    }

    m_CardiacArrest->Load(*cardiacarrest);
    return IsValid(*m_CardiacArrest);
  }

  const CDM::ChestCompressionData* chestcomp = dynamic_cast<const CDM::ChestCompressionData*>(&action);
  if (chestcomp != nullptr) {
    const CDM::ChestCompressionForceData* cprForce = dynamic_cast<const CDM::ChestCompressionForceData*>(chestcomp);
    if (cprForce != nullptr) {
      if (HasChestCompressionForceScale()) {
        RemoveChestCompression();
      }
      if (m_ChestCompression == nullptr) {
        m_ChestCompression = new SEChestCompressionForce();
      }
      ((SEChestCompressionForce*)m_ChestCompression)->Load(*cprForce);
      if (!m_ChestCompression->IsActive()) {
        RemoveChestCompression();
        return true;
      }
      return IsValid(*m_ChestCompression);
    }
    const CDM::ChestCompressionForceScaleData* cprScale = dynamic_cast<const CDM::ChestCompressionForceScaleData*>(chestcomp);
    if (cprScale != nullptr) {
      if (HasChestCompressionForce()) {
        RemoveChestCompression();
      }
      if (m_ChestCompression == nullptr) {
        m_ChestCompression = new SEChestCompressionForceScale();
      }
      ((SEChestCompressionForceScale*)m_ChestCompression)->Load(*cprScale);
      if (!m_ChestCompression->IsActive()) {
        RemoveChestCompression();
        return true;
      }
      return IsValid(*m_ChestCompression);
    }
    /// \error Unknown ChestCompression type
    return false;
  }

  const CDM::ChestOcclusiveDressingData* chestOccl = dynamic_cast<const CDM::ChestOcclusiveDressingData*>(&action);
  if (chestOccl != nullptr) {
    if (chestOccl->Side() == CDM::enumSide::Left) {
      if (m_LeftChestOcclusiveDressing == nullptr) {
        m_LeftChestOcclusiveDressing = new SEChestOcclusiveDressing();
      }
      m_LeftChestOcclusiveDressing->Load(*chestOccl);
      if (!m_LeftChestOcclusiveDressing->IsActive()) {
        RemoveLeftChestOcclusiveDressing();
        return true;
      }
      return IsValid(*m_LeftChestOcclusiveDressing);
    } else if (chestOccl->Side() == CDM::enumSide::Right) {
      if (m_RightChestOcclusiveDressing == nullptr) {
        m_RightChestOcclusiveDressing = new SEChestOcclusiveDressing();
      }
      m_RightChestOcclusiveDressing->Load(*chestOccl);
      if (!m_RightChestOcclusiveDressing->IsActive()) {
        RemoveRightChestOcclusiveDressing();
        return true;
      }
      return IsValid(*m_RightChestOcclusiveDressing);
    }
  }

  const CDM::ConsciousRespirationData* conResp = dynamic_cast<const CDM::ConsciousRespirationData*>(&action);
  if (conResp != nullptr) {
    if (m_ConsciousRespiration == nullptr) {
      m_ConsciousRespiration = new SEConsciousRespiration();
    }
    m_ConsciousRespiration->Load(*conResp, m_Substances);
    if (!m_ConsciousRespiration->IsActive()) {
      RemoveConsciousRespiration();
      return true;
    }
    return IsValid(*m_ConsciousRespiration);
  }

  const CDM::ConsumeNutrientsData* consume = dynamic_cast<const CDM::ConsumeNutrientsData*>(&action);
  if (consume != nullptr) {
    if (m_ConsumeNutrients == nullptr) {
      m_ConsumeNutrients = new SEConsumeNutrients();
    }
    m_ConsumeNutrients->Load(*consume);
    if (!m_ConsumeNutrients->IsActive()) {
      RemoveConsumeNutrients();
      return true;
    }
    return IsValid(*m_ConsumeNutrients);
  }

  const CDM::EscharotomyData* eschData = dynamic_cast<const CDM::EscharotomyData*>(&action);
  if (eschData != nullptr) {
    auto eItr = m_Escharotomies.find(eschData->Location());
    SEEscharotomy* escharotomy = std::make_unique<SEEscharotomy>().release();
    if (eItr != m_Escharotomies.end()) {
      escharotomy = m_Escharotomies[eschData->Location()];
      escharotomy->Load(*eschData);
    } else {
      escharotomy->Load(*eschData);
      if (escharotomy->IsValid()) {
        m_Escharotomies[eschData->Location()] = escharotomy;
        eItr = m_Escharotomies.find(eschData->Location());
      } else {
        escharotomy->Clear();
      }
    }
    auto escharotomy2 = eItr->second;
    escharotomy2->Load(*eschData);
    if (!escharotomy2->IsActive()) {
      RemoveEscharotomy(escharotomy2->GetLocation());
      return true;
    }
    return IsValid(*escharotomy2);
  }

  const CDM::ExerciseData* exercise = dynamic_cast<const CDM::ExerciseData*>(&action);
  if (exercise != nullptr) {
    if (m_Exercise == nullptr) {
      m_Exercise = new SEExercise();
    }
    m_Exercise->Load(*exercise);
    if (!m_Exercise->IsActive()) {
      RemoveExercise();
      return true;
    }
    return IsValid(*m_Exercise);
  }

  const CDM::HemorrhageData* hem = dynamic_cast<const CDM::HemorrhageData*>(&action);
  if (hem != nullptr) {
    SEHemorrhage* myHem = m_Hemorrhages[hem->Compartment()];
    if (myHem == nullptr) {
      myHem = new SEHemorrhage();
      m_Hemorrhages[hem->Compartment()] = myHem;
    }
    myHem->Load(*hem);

    if (!myHem->IsActive()) {
      RemoveHemorrhage(myHem->GetCompartment());
      return true;
    }
    return IsValid(*myHem);
  }

  const CDM::InfectionData* infect = dynamic_cast<const CDM::InfectionData*>(&action);
  if (infect != nullptr) {
    if (m_Infection == nullptr) {
      m_Infection = new SEInfection();
    }
    m_Infection->Load(*infect);
    if (!m_Infection->IsActive()) {
      m_Logger->Warning("Manual Eliminaton of Infection currently not supported.");
      return false;
    }
    return IsValid(*m_Infection);
  }

  const CDM::IntubationData* intubation = dynamic_cast<const CDM::IntubationData*>(&action);
  if (intubation != nullptr) {
    if (m_Intubation == nullptr) {
      m_Intubation = new SEIntubation();
    }
    m_Intubation->Load(*intubation);
    if (!m_Intubation->IsActive()) {
      Info("Turning off Intubation");
      RemoveIntubation();
      return true;
    }
    return IsValid(*m_Intubation);
  }

  const CDM::MechanicalVentilationData* mvData = dynamic_cast<const CDM::MechanicalVentilationData*>(&action);
  if (mvData != nullptr) {
    if (m_MechanicalVentilation == nullptr) {
      m_MechanicalVentilation = new SEMechanicalVentilation();
    }
    m_MechanicalVentilation->Load(*mvData, m_Substances);
    if (!m_MechanicalVentilation->IsActive()) {
      RemoveMechanicalVentilation();
      return true;
    }
    return IsValid(*m_MechanicalVentilation);
  }

  const CDM::NeedleDecompressionData* needleDecomp = dynamic_cast<const CDM::NeedleDecompressionData*>(&action);
  if (needleDecomp != nullptr) {
    if (needleDecomp->Side() == CDM::enumSide::Left) {
      if (m_LeftNeedleDecompression == nullptr) {
        m_LeftNeedleDecompression = new SENeedleDecompression();
      }
      m_LeftNeedleDecompression->Load(*needleDecomp);
      if (!m_LeftNeedleDecompression->IsActive()) {
        RemoveLeftNeedleDecompression();
        return true;
      }
      return IsValid(*m_LeftNeedleDecompression);
    } else if (needleDecomp->Side() == CDM::enumSide::Right) {
      if (m_RightNeedleDecompression == nullptr) {
        m_RightNeedleDecompression = new SENeedleDecompression();
      }
      m_RightNeedleDecompression->Load(*needleDecomp);
      if (!m_RightNeedleDecompression->IsActive()) {
        RemoveRightNeedleDecompression();
        return true;
      }
      return IsValid(*m_RightNeedleDecompression);
    } else {
      return false;
    }
  }

  const CDM::PainStimulusData* pain = dynamic_cast<const CDM::PainStimulusData*>(&action);
  if (pain != nullptr) {
    SEPainStimulus* mypain = m_PainStimuli[pain->Location()];
    if (mypain == nullptr) {
      mypain = new SEPainStimulus();
      m_PainStimuli[pain->Location()] = mypain;
    }
    mypain->Load(*pain);

    if (!mypain->IsActive()) {
      RemovePainStimulus(mypain->GetLocation());
      return true;
    }
    return IsValid(*mypain);
  }

  const CDM::PericardialEffusionData* pericardialEff = dynamic_cast<const CDM::PericardialEffusionData*>(&action);
  if (pericardialEff != nullptr) {
    if (m_PericardialEffusion == nullptr) {
      m_PericardialEffusion = new SEPericardialEffusion();
    }
    m_PericardialEffusion->Load(*pericardialEff);
    if (!m_PericardialEffusion->IsActive()) {
      RemovePericardialEffusion();
      return true;
    }
    return IsValid(*m_PericardialEffusion);
  }

  const CDM::PulmonaryShuntData* pulmonaryShunt = dynamic_cast<const CDM::PulmonaryShuntData*>(&action);
  if (pulmonaryShunt != nullptr) {
    if (m_PulmonaryShunt == nullptr) {
      m_PulmonaryShunt = new SEPulmonaryShunt();
    }
    m_PulmonaryShunt->Load(*pulmonaryShunt);
    if (!m_PulmonaryShunt->IsActive()) {
      RemovePulmonaryShunt();
      return true;
    }
    return IsValid(*m_PulmonaryShunt);
  }

  const CDM::SleepData* sleep = dynamic_cast<const CDM::SleepData*>(&action);
  if (sleep != nullptr) {
    if (m_Sleep == nullptr)
      m_Sleep = new SESleep();
    m_Sleep->Load(*sleep);
    if (!m_Sleep->IsActive()) {
      RemoveSleepState();
      return true;
    }
    return IsValid(*m_Sleep);
  }

  const CDM::SubstanceAdministrationData* admin = dynamic_cast<const CDM::SubstanceAdministrationData*>(&action);
  if (admin != nullptr) {
    return AdministerSubstance(*admin);
  }

  const CDM::TensionPneumothoraxData* pneumo = dynamic_cast<const CDM::TensionPneumothoraxData*>(&action);
  if (pneumo != nullptr) {
    if (pneumo->Side() == CDM::enumSide::Left && pneumo->Type() == CDM::enumPneumothoraxType::Open) {
      if (m_LeftOpenTensionPneumothorax == nullptr) {
        m_LeftOpenTensionPneumothorax = new SETensionPneumothorax();
      }
      m_LeftOpenTensionPneumothorax->Load(*pneumo);
      if (!m_LeftOpenTensionPneumothorax->IsActive()) {
        RemoveLeftClosedTensionPneumothorax();
        return true;
      }
      return IsValid(*m_LeftOpenTensionPneumothorax);
    } else if (pneumo->Side() == CDM::enumSide::Left && pneumo->Type() == CDM::enumPneumothoraxType::Closed) {
      if (m_LeftClosedTensionPneumothorax == nullptr) {
        m_LeftClosedTensionPneumothorax = new SETensionPneumothorax();
      }
      m_LeftClosedTensionPneumothorax->Load(*pneumo);
      if (!m_LeftClosedTensionPneumothorax->IsActive()) {
        RemoveLeftOpenTensionPneumothorax();
        return true;
      }
      return IsValid(*m_LeftClosedTensionPneumothorax);
    } else if (pneumo->Side() == CDM::enumSide::Right && pneumo->Type() == CDM::enumPneumothoraxType::Open) {
      if (m_RightOpenTensionPneumothorax == nullptr) {
        m_RightOpenTensionPneumothorax = new SETensionPneumothorax();
      }
      m_RightOpenTensionPneumothorax->Load(*pneumo);
      if (!m_RightOpenTensionPneumothorax->IsActive()) {
        RemoveRightOpenTensionPneumothorax();
        return true;
      }
      return IsValid(*m_RightOpenTensionPneumothorax);
    } else if (pneumo->Side() == CDM::enumSide::Right && pneumo->Type() == CDM::enumPneumothoraxType::Closed) {
      if (m_RightClosedTensionPneumothorax == nullptr) {
        m_RightClosedTensionPneumothorax = new SETensionPneumothorax();
      }
      m_RightClosedTensionPneumothorax->Load(*pneumo);
      if (!m_RightClosedTensionPneumothorax->IsActive()) {
        RemoveRightClosedTensionPneumothorax();
        return true;
      }
      return IsValid(*m_RightClosedTensionPneumothorax);
    }
    return false; // Duno what this is...
  }

  const CDM::TourniquetData* tournData = dynamic_cast<const CDM::TourniquetData*>(&action);
  if (tournData != nullptr) {
    auto tItr = m_Tourniquets.find(tournData->Compartment());

    bool matchingHem = false;
    bool validCmpt = false;
    std::stringstream warn;
    if (m_Hemorrhages.find(tournData->Compartment()) != m_Hemorrhages.end()) {
      matchingHem = true;
      SETourniquet* tourniquet = std::make_unique<SETourniquet>().release();
      if (tItr != m_Tourniquets.end()) {
        tourniquet = m_Tourniquets[tournData->Compartment()];
        tourniquet->Load(*tournData);
        validCmpt = true;
      } else {
        tourniquet->Load(*tournData);
        if (tourniquet->IsValid()) {
          m_Tourniquets[tournData->Compartment()] = tourniquet;
          tItr = m_Tourniquets.find(tournData->Compartment());
          validCmpt = true;
        } else {
          tourniquet->Clear();
          warn << "\t Invalid tourniquet location:  Valid options are LeftArm, LeftLeg, RightArm, RightLeg" << std::endl;
        }
      }
    } else {
      warn << "\n\t Tourniquet cannot be applied to a compartment without a hemorrhage" << std::endl;
    }
    if (matchingHem && validCmpt) {
      auto turniquet = tItr->second;
      turniquet->Load(*tournData);
      if (!turniquet->IsActive()) {
        RemoveTourniquet(turniquet->GetCompartment());
        return true;
      }
      return IsValid(*turniquet);
    } else {
      Warning(warn);
      return false;
    }
  }

  const CDM::UrinateData* urinate = dynamic_cast<const CDM::UrinateData*>(&action);
  if (urinate != nullptr) {
    if (m_Urinate == nullptr) {
      m_Urinate = new SEUrinate();
    }
    m_Urinate->Load(*urinate);
    if (!m_Urinate->IsActive()) {
      RemoveUrinate();
      return true;
    }
    return IsValid(*m_Urinate);
  }

  const CDM::OverrideData* orData = dynamic_cast<const CDM::OverrideData*>(&action);
  if (orData != nullptr) {
    if (m_OverrideAction == nullptr) {
      m_OverrideAction = new SEOverride();
    }
    m_OverrideAction->Load(*orData);
    if (!m_OverrideAction->IsActive()) {
      RemoveOverride();
      return true;
    }
    return IsValid(*m_OverrideAction);
  }

  /// \error Unsupported Action
  Error("Unsupported Action");
  return false;
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::IsValid(const SEPatientAction& action)
{
  if (!action.IsValid()) {
    Error("Invalid Patient Action");
    return false;
  }
  return true;
}

//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasAcuteRespiratoryDistress() const
{
  return m_AcuteRespiratoryDistress == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEAcuteRespiratoryDistress* SEPatientActionCollection::GetAcuteRespiratoryDistress() const
{
  return m_AcuteRespiratoryDistress;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveAcuteRespiratoryDistress()
{
  SAFE_DELETE(m_AcuteRespiratoryDistress);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasAcuteStress() const
{
  return m_AcuteStress == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEAcuteStress* SEPatientActionCollection::GetAcuteStress() const
{
  return m_AcuteStress;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveAcuteStress()
{
  SAFE_DELETE(m_AcuteStress);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasAirwayObstruction() const
{
  return m_AirwayObstruction == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEAirwayObstruction* SEPatientActionCollection::GetAirwayObstruction() const
{
  return m_AirwayObstruction;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveAirwayObstruction()
{
  SAFE_DELETE(m_AirwayObstruction);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasApnea() const
{
  return m_Apnea == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEApnea* SEPatientActionCollection::GetApnea() const
{
  return m_Apnea;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveApnea()
{
  SAFE_DELETE(m_Apnea);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasAsthmaAttack() const
{
  return m_AsthmaAttack == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEAsthmaAttack* SEPatientActionCollection::GetAsthmaAttack() const
{
  return m_AsthmaAttack;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveAsthmaAttack()
{
  SAFE_DELETE(m_AsthmaAttack);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasBrainInjury() const
{
  return m_BrainInjury == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEBrainInjury* SEPatientActionCollection::GetBrainInjury() const
{
  return m_BrainInjury;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveBrainInjury()
{
  SAFE_DELETE(m_BrainInjury);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasBronchoconstriction() const
{
  return m_Bronchoconstriction == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEBronchoconstriction* SEPatientActionCollection::GetBronchoconstriction() const
{
  return m_Bronchoconstriction;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveBronchoconstriction()
{
  SAFE_DELETE(m_Bronchoconstriction);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasBurnWound() const
{
  return m_BurnWound == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEBurnWound* SEPatientActionCollection::GetBurnWound() const
{
  return m_BurnWound;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveBurnWound()
{
  SAFE_DELETE(m_BurnWound);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasCardiacArrest() const
{
  return m_CardiacArrest == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SECardiacArrest* SEPatientActionCollection::GetCardiacArrest() const
{
  return m_CardiacArrest;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveCardiacArrest()
{
  SAFE_DELETE(m_CardiacArrest);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasChestCompression() const
{
  return m_ChestCompression == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveChestCompression()
{
  SAFE_DELETE(m_ChestCompression);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasChestCompressionForce() const
{
  return m_ChestCompression == nullptr ? false : dynamic_cast<SEChestCompressionForce*>(m_ChestCompression) != nullptr;
}
//-------------------------------------------------------------------------------
SEChestCompressionForce* SEPatientActionCollection::GetChestCompressionForce() const
{
  return dynamic_cast<SEChestCompressionForce*>(m_ChestCompression);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasChestCompressionForceScale() const
{
  return m_ChestCompression == nullptr ? false : dynamic_cast<SEChestCompressionForceScale*>(m_ChestCompression) != nullptr;
}
//-------------------------------------------------------------------------------
SEChestCompressionForceScale* SEPatientActionCollection::GetChestCompressionForceScale() const
{
  return dynamic_cast<SEChestCompressionForceScale*>(m_ChestCompression);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasChestOcclusiveDressing() const
{
  return m_LeftChestOcclusiveDressing != nullptr || m_RightChestOcclusiveDressing != nullptr ? true : false;
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasLeftChestOcclusiveDressing() const
{
  return m_LeftChestOcclusiveDressing != nullptr ? true : false;
}
//-------------------------------------------------------------------------------
SEChestOcclusiveDressing* SEPatientActionCollection::GetLeftChestOcclusiveDressing() const
{
  return m_LeftChestOcclusiveDressing;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveLeftChestOcclusiveDressing()
{
  SAFE_DELETE(m_LeftChestOcclusiveDressing);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasRightChestOcclusiveDressing() const
{
  return m_RightChestOcclusiveDressing != nullptr ? true : false;
}
//-------------------------------------------------------------------------------
SEChestOcclusiveDressing* SEPatientActionCollection::GetRightChestOcclusiveDressing() const
{
  return m_RightChestOcclusiveDressing;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveRightChestOcclusiveDressing()
{
  SAFE_DELETE(m_RightChestOcclusiveDressing);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasConsciousRespiration() const
{
  return m_ConsciousRespiration == nullptr ? false : m_ConsciousRespiration->IsValid();
}
//-------------------------------------------------------------------------------
SEConsciousRespiration* SEPatientActionCollection::GetConsciousRespiration() const
{
  return m_ConsciousRespiration;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveConsciousRespiration()
{
  SAFE_DELETE(m_ConsciousRespiration);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasConsumeNutrients() const
{
  return m_ConsumeNutrients == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEConsumeNutrients* SEPatientActionCollection::GetConsumeNutrients() const
{
  return m_ConsumeNutrients;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveConsumeNutrients()
{
  SAFE_DELETE(m_ConsumeNutrients);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasEscharotomy() const
{
  return !m_Escharotomies.empty();
}
//-------------------------------------------------------------------------------
const std::map<std::string, SEEscharotomy*>& SEPatientActionCollection::GetEscharotomies() const
{
  return m_Escharotomies;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveEscharotomy(const char* cmpt)
{
  RemoveEscharotomy(std::string { cmpt });
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveEscharotomy(const std::string& cmpt)
{
  SEEscharotomy* h = m_Escharotomies[cmpt];
  m_Escharotomies.erase(cmpt);
  SAFE_DELETE(h);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasExercise() const
{
  return m_Exercise == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEExercise* SEPatientActionCollection::GetExercise() const
{
  return m_Exercise;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveExercise()
{
  SAFE_DELETE(m_Exercise);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasHemorrhage() const
{
  return m_Hemorrhages.empty() ? false : true;
}
//-------------------------------------------------------------------------------
const std::map<std::string, SEHemorrhage*>& SEPatientActionCollection::GetHemorrhages() const
{
  return m_Hemorrhages;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveHemorrhage(const char* cmpt)
{
  RemoveHemorrhage(std::string { cmpt });
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveHemorrhage(const std::string& cmpt)
{
  SEHemorrhage* h = m_Hemorrhages[cmpt];
  m_Hemorrhages.erase(cmpt);
  SAFE_DELETE(h);
  if (m_Tourniquets.count(cmpt) != 0) {
    Info("Hemorrhage stopped but tourniquet still active.  If you want to remove tourniquet, set tourniquet level to None");
  }
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasInfection() const
{
  return m_Infection == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEInfection* SEPatientActionCollection::GetInfection() const
{
  return m_Infection;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveInfection()
{
  SAFE_DELETE(m_Infection);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasIntubation() const
{
  return m_Intubation == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEIntubation* SEPatientActionCollection::GetIntubation() const
{
  return m_Intubation;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveIntubation()
{
  SAFE_DELETE(m_Intubation);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasMechanicalVentilation() const
{
  return m_MechanicalVentilation == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEMechanicalVentilation* SEPatientActionCollection::GetMechanicalVentilation() const
{
  return m_MechanicalVentilation;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveMechanicalVentilation()
{
  SAFE_DELETE(m_MechanicalVentilation);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasNeedleDecompression() const
{
  return m_LeftNeedleDecompression != nullptr || m_RightNeedleDecompression ? true : false;
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasLeftNeedleDecompression() const
{
  return m_LeftNeedleDecompression == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SENeedleDecompression* SEPatientActionCollection::GetLeftNeedleDecompression() const
{
  return m_LeftNeedleDecompression;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveLeftNeedleDecompression()
{
  SAFE_DELETE(m_LeftNeedleDecompression);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasRightNeedleDecompression() const
{
  return m_RightNeedleDecompression == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SENeedleDecompression* SEPatientActionCollection::GetRightNeedleDecompression() const
{
  return m_RightNeedleDecompression;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveRightNeedleDecompression()
{
  SAFE_DELETE(m_RightNeedleDecompression);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasPainStimulus() const
{
  return m_PainStimuli.empty() ? false : true;
}
//-------------------------------------------------------------------------------
const std::map<std::string, SEPainStimulus*>& SEPatientActionCollection::GetPainStimuli() const
{
  return m_PainStimuli;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemovePainStimulus(const char* cmpt)
{
  RemovePainStimulus(std::string { cmpt });
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemovePainStimulus(const std::string& cmpt)
{
  SEPainStimulus* p = m_PainStimuli[cmpt];
  m_PainStimuli.erase(cmpt);
  SAFE_DELETE(p);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasPericardialEffusion() const
{
  return m_PericardialEffusion == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEPericardialEffusion* SEPatientActionCollection::GetPericardialEffusion() const
{
  return m_PericardialEffusion;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemovePericardialEffusion()
{
  SAFE_DELETE(m_PericardialEffusion);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasPulmonaryShunt() const
{
  return m_PulmonaryShunt == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEPulmonaryShunt* SEPatientActionCollection::GetPulmonaryShunt() const
{
  return m_PulmonaryShunt;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemovePulmonaryShunt()
{
  SAFE_DELETE(m_PulmonaryShunt);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasSleepState() const
{
  return m_Sleep == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SESleep* SEPatientActionCollection::GetSleepState() const
{
  return m_Sleep;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveSleepState()
{
  SAFE_DELETE(m_Sleep);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasTensionPneumothorax() const
{
  if (m_LeftOpenTensionPneumothorax != nullptr) {
    //&&m_LeftOpenTensionPneumothorax->IsValid())// TODO
    return true;
  }
  if (m_LeftClosedTensionPneumothorax != nullptr) {
    //&&m_LeftClosedTensionPneumothorax->IsValid())// TODO
    return true;
  }
  if (m_RightOpenTensionPneumothorax != nullptr) {
    //&&m_RightOpenTensionPneumothorax->IsValid())// TODO
    return true;
  }
  if (m_RightClosedTensionPneumothorax != nullptr) {
    //&&m_RightClosedTensionPneumothorax->IsValid())// TODO
    return true;
  }
  return false;
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasLeftOpenTensionPneumothorax() const
{
  return m_LeftOpenTensionPneumothorax == nullptr ? false : true; //m_LeftOpenTensionPneumothorax->IsValid();//TODO
}
//-------------------------------------------------------------------------------
SETensionPneumothorax* SEPatientActionCollection::GetLeftOpenTensionPneumothorax() const
{
  return m_LeftOpenTensionPneumothorax;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveLeftOpenTensionPneumothorax()
{
  SAFE_DELETE(m_LeftOpenTensionPneumothorax);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasLeftClosedTensionPneumothorax() const
{
  return m_LeftClosedTensionPneumothorax == nullptr ? false : true; //m_LeftClosedTensionPneumothorax->IsValid();//TODO
}
//-------------------------------------------------------------------------------
SETensionPneumothorax* SEPatientActionCollection::GetLeftClosedTensionPneumothorax() const
{
  return m_LeftClosedTensionPneumothorax;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveLeftClosedTensionPneumothorax()
{
  SAFE_DELETE(m_LeftClosedTensionPneumothorax);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasRightOpenTensionPneumothorax() const
{
  return m_RightOpenTensionPneumothorax == nullptr ? false : true; //m_RightOpenTensionPneumothorax->IsValid();//TODO
}
//-------------------------------------------------------------------------------
SETensionPneumothorax* SEPatientActionCollection::GetRightOpenTensionPneumothorax() const
{
  return m_RightOpenTensionPneumothorax;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveRightOpenTensionPneumothorax()
{
  SAFE_DELETE(m_RightOpenTensionPneumothorax);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasRightClosedTensionPneumothorax() const
{
  return m_RightClosedTensionPneumothorax == nullptr ? false : true; //m_RightClosedTensionPneumothorax->IsValid();//TODO
}
//-------------------------------------------------------------------------------
SETensionPneumothorax* SEPatientActionCollection::GetRightClosedTensionPneumothorax() const
{
  return m_RightClosedTensionPneumothorax;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveRightClosedTensionPneumothorax()
{
  SAFE_DELETE(m_RightClosedTensionPneumothorax);
}
//-------------------------------------------------------------------------------
const std::map<const SESubstance*, SESubstanceBolus*>& SEPatientActionCollection::GetSubstanceBoluses() const
{
  return m_SubstanceBolus;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveSubstanceBolus(const SESubstance& sub)
{
  SESubstanceBolus* b = m_SubstanceBolus[&sub];
  m_SubstanceBolus.erase(&sub);
  SAFE_DELETE(b);
}
//-------------------------------------------------------------------------------
const std::map<const SESubstance*, SESubstanceInfusion*>& SEPatientActionCollection::GetSubstanceInfusions() const
{
  return m_SubstanceInfusions;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveSubstanceInfusion(const SESubstance& sub)
{
  SESubstanceInfusion* si = m_SubstanceInfusions[&sub];
  m_SubstanceInfusions.erase(&sub);
  SAFE_DELETE(si);
}
//-------------------------------------------------------------------------------
const std::map<const SESubstance*, SESubstanceOralDose*>& SEPatientActionCollection::GetSubstanceOralDoses() const
{
  return m_SubstanceOralDoses;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveSubstanceOralDose(const SESubstance& sub)
{
  SESubstanceOralDose* od = m_SubstanceOralDoses[&sub];
  m_SubstanceOralDoses.erase(&sub);
  SAFE_DELETE(od);
}
//-------------------------------------------------------------------------------
const std::map<const SESubstance*, SESubstanceNasalDose*>& SEPatientActionCollection::GetSubstanceNasalDoses() const
{
  return m_SubstanceNasalDoses;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveSubstanceNasalDose(const SESubstance& sub)
{
  SESubstanceNasalDose* od = m_SubstanceNasalDoses[&sub];
  m_SubstanceNasalDoses.erase(&sub);
  SAFE_DELETE(od);
}

//-------------------------------------------------------------------------------
const std::map<const SESubstanceCompound*, SESubstanceCompoundInfusion*>& SEPatientActionCollection::GetSubstanceCompoundInfusions() const
{
  return m_SubstanceCompoundInfusions;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveSubstanceCompoundInfusion(const SESubstanceCompound& cSub)
{
  SESubstanceCompoundInfusion* sci = m_SubstanceCompoundInfusions[&cSub];
  m_SubstanceCompoundInfusions.erase(&cSub);
  SAFE_DELETE(sci);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::AdministerSubstance(const CDM::SubstanceAdministrationData& subAdmin)
{
  const CDM::SubstanceBolusData* bolus = dynamic_cast<const CDM::SubstanceBolusData*>(&subAdmin);
  if (bolus != nullptr) {
    SESubstance* sub = m_Substances.GetSubstance(bolus->Substance());
    if (sub == nullptr) {
      m_ss << "Unknown substance : " << bolus->Substance();
      Error(m_ss);
      return false;
    }
    SESubstanceBolus* myBolus = m_SubstanceBolus[sub];
    if (myBolus == nullptr) {
      myBolus = new SESubstanceBolus(*sub);
      m_SubstanceBolus[sub] = myBolus;
      m_Substances.AddActiveSubstance(*sub);
    }
    myBolus->Load(*bolus);
    if (!myBolus->IsActive()) {
      RemoveSubstanceBolus(*sub);
      return true;
    }
    return IsValid(*myBolus);
  }

  const CDM::SubstanceInfusionData* subInfusion = dynamic_cast<const CDM::SubstanceInfusionData*>(&subAdmin);
  if (subInfusion != nullptr) {
    SESubstance* sub = m_Substances.GetSubstance(subInfusion->Substance());
    if (sub == nullptr) {
      m_ss << "Unknown substance : " << subInfusion->Substance();
      Error(m_ss);
      return false;
    }
    SESubstanceInfusion* mySubInfuse = m_SubstanceInfusions[sub];
    if (mySubInfuse == nullptr) {
      mySubInfuse = new SESubstanceInfusion(*sub);
      m_SubstanceInfusions[sub] = mySubInfuse;
      m_Substances.AddActiveSubstance(*sub);
    }
    mySubInfuse->Load(*subInfusion);
    if (!mySubInfuse->IsActive()) {
      RemoveSubstanceInfusion(*sub);
      return true;
    }
    return IsValid(*mySubInfuse);
  }

  const CDM::SubstanceNasalDoseData* NasalDose = dynamic_cast<const CDM::SubstanceNasalDoseData*>(&subAdmin);
  if (NasalDose != nullptr) {
    SESubstance* sub = m_Substances.GetSubstance(NasalDose->Substance());
    if (sub == nullptr) {
      m_ss << "Unknown substance : " << NasalDose->Substance();
      Error(m_ss);
      return false;
    }
    SESubstanceNasalDose* myNasalDose = m_SubstanceNasalDoses[sub];
    if (myNasalDose == nullptr) {
      myNasalDose = new SESubstanceNasalDose(*sub);
      m_SubstanceNasalDoses[sub] = myNasalDose;
      m_Substances.AddActiveSubstance(*sub);
    }
    myNasalDose->Load(*NasalDose);
    if (!myNasalDose->IsActive()) {
      RemoveSubstanceNasalDose(*sub);
      return true;
    }
    return IsValid(*myNasalDose);
  }

  const CDM::SubstanceOralDoseData* oralDose = dynamic_cast<const CDM::SubstanceOralDoseData*>(&subAdmin);
  if (oralDose != nullptr) {
    SESubstance* sub = m_Substances.GetSubstance(oralDose->Substance());
    if (sub == nullptr) {
      m_ss << "Unknown substance : " << oralDose->Substance();
      Error(m_ss);
      return false;
    }
    SESubstanceOralDose* myOralDose = m_SubstanceOralDoses[sub];
    if (myOralDose == nullptr) {
      myOralDose = new SESubstanceOralDose(*sub);
      m_SubstanceOralDoses[sub] = myOralDose;
      m_Substances.AddActiveSubstance(*sub);
    }
    myOralDose->Load(*oralDose);
    if (!myOralDose->IsActive()) {
      RemoveSubstanceOralDose(*sub);
      return true;
    }
    return IsValid(*myOralDose);
  }

  const CDM::SubstanceCompoundInfusionData* cSubInfusion = dynamic_cast<const CDM::SubstanceCompoundInfusionData*>(&subAdmin);
  if (cSubInfusion != nullptr) {
    SESubstanceCompound* cmpd = m_Substances.GetCompound(cSubInfusion->SubstanceCompound());
    if (cmpd == nullptr) {
      m_ss << "Unknown compound : " << cSubInfusion->SubstanceCompound();
      Error(m_ss);
      return false;
    }
    SESubstanceCompoundInfusion* mySubCompInfuse = m_SubstanceCompoundInfusions[cmpd];
    if (mySubCompInfuse == nullptr) {
      mySubCompInfuse = new SESubstanceCompoundInfusion(*cmpd);
      m_SubstanceCompoundInfusions[cmpd] = mySubCompInfuse;
      m_Substances.AddActiveCompound(*cmpd);
      // Activate the compound component substances
      const SESubstanceCompound& sc = mySubCompInfuse->GetSubstanceCompound();
      for (auto const& scc : sc.GetComponents()) {
        m_Substances.AddActiveSubstance(scc.GetSubstance());
      }
    }
    mySubCompInfuse->Load(*cSubInfusion);
    if (!mySubCompInfuse->IsActive()) {
      RemoveSubstanceCompoundInfusion(*cmpd);
      return true;
    }
    return IsValid(*mySubCompInfuse);
  }
  return false;
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasTourniquet() const
{
  return !m_Tourniquets.empty();
}
//-------------------------------------------------------------------------------
const std::map<std::string, SETourniquet*>& SEPatientActionCollection::GetTourniquets() const
{
  return m_Tourniquets;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveTourniquet(const char* cmpt)
{
  RemoveTourniquet(std::string { cmpt });
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveTourniquet(const std::string& cmpt)
{
  SETourniquet* h = m_Tourniquets[cmpt];
  m_Tourniquets.erase(cmpt);
  SAFE_DELETE(h);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasUrinate() const
{
  return m_Urinate == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEUrinate* SEPatientActionCollection::GetUrinate() const
{
  return m_Urinate;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveUrinate()
{
  SAFE_DELETE(m_Urinate);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::HasOverride() const
{
  return m_OverrideAction == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEOverride* SEPatientActionCollection::GetOverride()
{
  return m_OverrideAction;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveOverride()
{
  SAFE_DELETE(m_OverrideAction);
}
//-------------------------------------------------------------------------------
}
