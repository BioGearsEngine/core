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

#include "io/cdm/PatientActions.h"
#include "io/cdm/Property.h"
#include "io/cdm/Scenario.h"
#include "io/cdm/Substance.h"

#include <biogears/cdm/engine/PhysiologyEngine.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>

#include <biogears/schema/cdm/Substance.hxx>

#include <map>
#include <vector>

namespace std {
template class map<string, biogears::SEHemorrhage*>;
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

//!
//!  Pair Wrapper
//!
template <typename KeyType, typename ValueType>
PairWrapper<KeyType, ValueType>::PairWrapper(typename std::pair<const KeyType, ValueType>* pair)
  : _ptr(pair)
{
}

template <typename KeyType, typename ValueType>
PairWrapper<KeyType, ValueType>::~PairWrapper()
{
}

template <typename KeyType, typename ValueType>
KeyType const& PairWrapper<KeyType, ValueType>::first()
{
  return _ptr->first;
}

template <typename KeyType, typename ValueType>
ValueType& PairWrapper<KeyType, ValueType>::second()
{
  return _ptr->second;
}
//!
//! MapIteratorWrapper
//!
template <typename KeyType, typename ValueType>
MapIteratorWrapper<KeyType, ValueType>::MapIteratorWrapper(typename std::map<KeyType, ValueType>::iterator itr)
  : _iterator(itr)
{
}

template <typename KeyType, typename ValueType>
MapIteratorWrapper<KeyType, ValueType>::~MapIteratorWrapper()
{
}

template <typename KeyType, typename ValueType>
bool MapIteratorWrapper<KeyType, ValueType>::operator==(MapIteratorWrapper const& rhs) const
{
  return _iterator == rhs._iterator;
}

template <typename KeyType, typename ValueType>
bool MapIteratorWrapper<KeyType, ValueType>::operator!=(MapIteratorWrapper const& rhs) const
{
  return _iterator != rhs._iterator;
}

template <typename KeyType, typename ValueType>
auto MapIteratorWrapper<KeyType, ValueType>::operator()() const -> PairWrapper<KeyType, ValueType>
{
  return PairWrapper<KeyType, ValueType>(_iterator.operator->());
}

template <typename KeyType, typename ValueType>
auto MapIteratorWrapper<KeyType, ValueType>::operator*() const -> PairWrapper<KeyType, ValueType>
{
  // Prefix  Operator

  return PairWrapper<KeyType, ValueType>(_iterator.operator->());
}

template <typename KeyType, typename ValueType>
auto MapIteratorWrapper<KeyType, ValueType>::operator++() -> MapIteratorWrapper&
{
  // Prefix  Operator                                         ~
  ++_iterator;
  return *this;
}

template <typename KeyType, typename ValueType>
auto MapIteratorWrapper<KeyType, ValueType>::operator++(int) -> MapIteratorWrapper
{
  // Postfix Operator
  MapIteratorWrapper temp { this->_iterator };
  ++_iterator;
  return temp;
}

//!
//! MapWrapper
//!
template <typename KeyType, typename ValueType>
MapWrapper<KeyType, ValueType>::MapWrapper(std::map<KeyType, ValueType>& given)
  : _map(given)
{
}

template <typename KeyType, typename ValueType>
MapWrapper<KeyType, ValueType>::~MapWrapper() { }

template <typename KeyType, typename ValueType>
auto MapWrapper<KeyType, ValueType>::begin() const -> MapIteratorWrapper<KeyType, ValueType>
{
  return { _map.begin() };
}
template <typename KeyType, typename ValueType>
auto MapWrapper<KeyType, ValueType>::end() const -> MapIteratorWrapper<KeyType, ValueType>
{
  return { _map.end() };
}

//!
//! VectorIteratorWrapper
//!
template <typename ValueType>
VectorIteratorWrapper<ValueType>::VectorIteratorWrapper(typename std::vector<ValueType>::iterator itr)
  : _iterator(itr)
{
}
template <typename ValueType>
VectorIteratorWrapper<ValueType>::~VectorIteratorWrapper()
{
}

template <typename ValueType>
ValueType const& VectorIteratorWrapper<ValueType>::value() const
{
  return *_iterator;
}

template <typename ValueType>
ValueType const& VectorIteratorWrapper<ValueType>::operator()() const
{
  return *_iterator;
}

template <typename ValueType>
bool VectorIteratorWrapper<ValueType>::operator==(VectorIteratorWrapper const& rhs) const
{
  return _iterator == rhs._iterator;
}

template <typename ValueType>
bool VectorIteratorWrapper<ValueType>::operator!=(VectorIteratorWrapper const& rhs) const
{
  return _iterator != rhs._iterator;
}

template <typename ValueType>
auto VectorIteratorWrapper<ValueType>::operator->() -> ValueType const*
{
  return _iterator.operator->();
}

template <typename ValueType>
auto VectorIteratorWrapper<ValueType>::operator*() -> ValueType const&
{
  return _iterator.operator*();
}

template <typename ValueType>
auto VectorIteratorWrapper<ValueType>::operator++() -> VectorIteratorWrapper&
{
  // Prefix  Operator
  ++_iterator;
  return { _iterator };
}
template <typename ValueType>
auto VectorIteratorWrapper<ValueType>::operator++(int) -> VectorIteratorWrapper
{
  // Postfix Operator
  VectorIteratorWrapper<ValueType> temp { _iterator };
  ++_iterator;
  return temp;
}

//!
//! VectorWrapper
//!
template <typename ValueType>
VectorWrapper<ValueType>::VectorWrapper(std::vector<ValueType>& given)
  : _vector { given }
{
}
template <typename ValueType>
VectorWrapper<ValueType>::~VectorWrapper()
{
}
template <typename ValueType>
auto VectorWrapper<ValueType>::begin() const -> VectorIteratorWrapper<ValueType>
{
  return { _vector.begin() };
}
template <typename ValueType>
auto VectorWrapper<ValueType>::end() const -> VectorIteratorWrapper<ValueType>
{
  return { _vector.end() };
}

// Template Specializations
template class PairWrapper<std::string, SEHemorrhage*>;
template class PairWrapper<std::string, SETourniquet*>;
template class PairWrapper<std::string, SEEscharotomy*>;
template class PairWrapper<std::string, SEPainStimulus*>;
template class PairWrapper<const SESubstance*, SESubstanceBolus*>;
template class PairWrapper<const SESubstance*, SESubstanceInfusion*>;
template class PairWrapper<const SESubstance*, SESubstanceOralDose*>;
template class PairWrapper<const SESubstance*, SESubstanceNasalDose*>;
template class PairWrapper<const SESubstanceCompound*, SESubstanceCompoundInfusion*>;

template class MapIteratorWrapper<std::string, SEHemorrhage*>;
template class MapIteratorWrapper<std::string, SETourniquet*>;
template class MapIteratorWrapper<std::string, SEEscharotomy*>;
template class MapIteratorWrapper<std::string, SEPainStimulus*>;
template class MapIteratorWrapper<const SESubstance*, SESubstanceBolus*>;
template class MapIteratorWrapper<const SESubstance*, SESubstanceInfusion*>;
template class MapIteratorWrapper<const SESubstance*, SESubstanceOralDose*>;
template class MapIteratorWrapper<const SESubstance*, SESubstanceNasalDose*>;
template class MapIteratorWrapper<const SESubstanceCompound*, SESubstanceCompoundInfusion*>;

template class MapWrapper<std::string, SEHemorrhage*>;
template class MapWrapper<std::string, SETourniquet*>;
template class MapWrapper<std::string, SEEscharotomy*>;
template class MapWrapper<std::string, SEPainStimulus*>;
template class MapWrapper<const SESubstance*, SESubstanceBolus*>;
template class MapWrapper<const SESubstance*, SESubstanceInfusion*>;
template class MapWrapper<const SESubstance*, SESubstanceOralDose*>;
template class MapWrapper<const SESubstance*, SESubstanceNasalDose*>;
template class MapWrapper<const SESubstanceCompound*, SESubstanceCompoundInfusion*>;
} // namespace biogears

namespace biogears {
SEPatientActionCollection::SEPatientActionCollection(SESubstanceManager& substances)
  : Loggable(substances.GetLogger())
  , m_Substances(substances)
{
  m_AcuteRespiratoryDistress = nullptr;
  m_AcuteStress = nullptr;
  m_ExampleAction = nullptr;
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
  m_Ebola = nullptr;
  m_Infection = nullptr;
  m_Intubation = nullptr;
  m_MechanicalVentilation = nullptr;
  m_NasalCannula = nullptr;
  m_LeftNeedleDecompression = nullptr;
  m_RightNeedleDecompression = nullptr;
  m_PericardialEffusion = nullptr;
  m_PulmonaryShunt = nullptr;
  m_RadiationAbsorbedDose = nullptr;
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
  RemoveExampleAction();
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
  RemoveEbola();
  RemoveInfection();
  RemoveIntubation();
  RemoveMechanicalVentilation();
  RemoveNasalCannula();
  RemoveLeftNeedleDecompression();
  RemoveRightNeedleDecompression();
  RemovePericardialEffusion();
  RemovePulmonaryShunt();
  RemoveRadiationAbsorbedDose();
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
bool SEPatientActionCollection::ProcessAction(const SEPatientAction& action, const PhysiologyEngine& engine)
{
  auto patientAssessment = dynamic_cast<const SEPatientAssessmentRequest*>(&action);
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

  auto ards = dynamic_cast<const SEAcuteRespiratoryDistress*>(&action);
  if (ards != nullptr) {
    if (m_AcuteRespiratoryDistress == nullptr) {
      m_AcuteRespiratoryDistress = new SEAcuteRespiratoryDistress();
    }
    CDM_PATIENT_ACTION_COPY(AcuteRespiratoryDistress, *ards, *m_AcuteRespiratoryDistress);

    if (!m_AcuteRespiratoryDistress->IsActive()) {
      RemoveAcuteRespiratoryDistress();
      return true;
    }
    return IsValid(*m_AcuteRespiratoryDistress);
  }

  auto aStress = dynamic_cast<const SEAcuteStress*>(&action);
  if (aStress != nullptr) {
    if (m_AcuteStress == nullptr) {
      m_AcuteStress = new SEAcuteStress();
    }
    CDM_PATIENT_ACTION_COPY(AcuteStress, *aStress, *m_AcuteStress)
    if (!m_AcuteStress->IsActive()) {
      RemoveAcuteStress();
      return true;
    }
    return IsValid(*m_AcuteStress);
  }

  auto expAction = dynamic_cast<const SEExampleAction*>(&action);
  if (expAction != nullptr) {
    if (m_ExampleAction == nullptr) {
      m_ExampleAction = new SEExampleAction();
    }
    CDM_PATIENT_ACTION_COPY(ExampleAction, *expAction, *m_ExampleAction)
    if (!m_ExampleAction->IsActive()) {
      RemoveExampleAction();
      return true;
    }
    return IsValid(*m_ExampleAction);
  }

  auto airwayObst = dynamic_cast<const SEAirwayObstruction*>(&action);
  if (airwayObst != nullptr) {
    if (m_AirwayObstruction == nullptr) {
      m_AirwayObstruction = new SEAirwayObstruction();
    }
    CDM_PATIENT_ACTION_COPY(AirwayObstruction, *airwayObst, *m_AirwayObstruction)
    if (!m_AirwayObstruction->IsActive()) {
      RemoveAirwayObstruction();
      return true;
    }
    return IsValid(*m_AirwayObstruction);
  }

  auto apnea = dynamic_cast<const SEApnea*>(&action);
  if (apnea != nullptr) {
    if (m_Apnea == nullptr) {
      m_Apnea = new SEApnea();
    }
    CDM_PATIENT_ACTION_COPY(Apnea, *apnea, *m_Apnea)
    if (!m_Apnea->IsActive()) {
      RemoveApnea();
      return true;
    }
    return IsValid(*m_Apnea);
  }

  auto asthmaattack = dynamic_cast<const SEAsthmaAttack*>(&action);
  if (asthmaattack != nullptr) {
    if (m_AsthmaAttack == nullptr) {
      m_AsthmaAttack = new SEAsthmaAttack();
    }
    CDM_PATIENT_ACTION_COPY(AsthmaAttack, *asthmaattack, *m_AsthmaAttack)
    if (!m_AsthmaAttack->IsActive()) {
      RemoveAsthmaAttack();
      return true;
    }
    return IsValid(*m_AsthmaAttack);
  }

  auto brainInjury = dynamic_cast<const SEBrainInjury*>(&action);
  if (brainInjury != nullptr) {
    if (m_BrainInjury == nullptr) {
      m_BrainInjury = new SEBrainInjury();
    }
    CDM_PATIENT_ACTION_COPY(BrainInjury, *brainInjury, *m_BrainInjury)
    if (!m_BrainInjury->IsActive()) {
      RemoveBrainInjury();
      return true;
    }
    return IsValid(*m_BrainInjury);
  }

  auto bronchoconstr = dynamic_cast<const SEBronchoconstriction*>(&action);
  if (bronchoconstr != nullptr) {
    if (m_Bronchoconstriction == nullptr) {
      m_Bronchoconstriction = new SEBronchoconstriction();
    }
    CDM_PATIENT_ACTION_COPY(Bronchoconstriction, *bronchoconstr, *m_Bronchoconstriction)
    if (!m_Bronchoconstriction->IsActive()) {
      RemoveBronchoconstriction();
      return true;
    }
    return IsValid(*m_Bronchoconstriction);
  }

  auto burn = dynamic_cast<const SEBurnWound*>(&action);
  if (burn != nullptr) {
    if (m_BurnWound == nullptr) {
      m_BurnWound = new SEBurnWound();
    }

    CDM_PATIENT_ACTION_COPY(BurnWound, *burn, *m_BurnWound)
    if (m_BurnWound->GetTimeOfBurn() == 0.0) {
      m_BurnWound->SetTimeOfBurn(engine.GetSimulationTime(TimeUnit::s));
    }
    if (!m_BurnWound->IsActive()) {
      RemoveBurnWound();
      return true;
    }
    return IsValid(*m_BurnWound);
  }

  auto cardiacarrest = dynamic_cast<const SECardiacArrest*>(&action);
  if (cardiacarrest != nullptr) {

    if (m_CardiacArrest == nullptr && cardiacarrest->IsActive()) {
      return true; // Ignore :CardiacArrest::Off request when no :CardiacArrest Event exist.
    }

    if (m_CardiacArrest == nullptr) {
      m_CardiacArrest = new SECardiacArrest();
    }

    CDM_PATIENT_ACTION_COPY(CardiacArrest, *cardiacarrest, *m_CardiacArrest)
    return IsValid(*m_CardiacArrest);
  }

  auto chestcomp = dynamic_cast<const SEChestCompression*>(&action);
  if (chestcomp != nullptr) {
    auto cprForce = dynamic_cast<const SEChestCompressionForce*>(chestcomp);
    if (cprForce != nullptr) {
      if (HasChestCompressionForceScale()) {
        RemoveChestCompression();
      }
      if (m_ChestCompression == nullptr) {
        m_ChestCompression = new SEChestCompressionForce();
      }
      CDM_PATIENT_ACTION_COPY(ChestCompressionForce, *cprForce, *((SEChestCompressionForce*)m_ChestCompression))
      if (!m_ChestCompression->IsActive()) {
        RemoveChestCompression();
        return true;
      }
      return IsValid(*m_ChestCompression);
    }
    auto cprScale = dynamic_cast<const SEChestCompressionForceScale*>(chestcomp);
    if (cprScale != nullptr) {
      if (HasChestCompressionForce()) {
        RemoveChestCompression();
      }
      if (m_ChestCompression == nullptr) {
        m_ChestCompression = new SEChestCompressionForceScale();
      }
      CDM_PATIENT_ACTION_COPY(ChestCompressionForce, *cprForce, *((SEChestCompressionForceScale*)m_ChestCompression))
      if (!m_ChestCompression->IsActive()) {
        RemoveChestCompression();
        return true;
      }
      return IsValid(*m_ChestCompression);
    }
    /// \error Unknown ChestCompression type
    return false;
  }

  auto chestOccl = dynamic_cast<const SEChestOcclusiveDressing*>(&action);
  if (chestOccl != nullptr) {
    if (chestOccl->GetSide() == SESide::Left) {
      if (m_LeftChestOcclusiveDressing == nullptr) {
        m_LeftChestOcclusiveDressing = new SEChestOcclusiveDressing();
      }
      CDM_PATIENT_ACTION_COPY(ChestOcclusiveDressing, *chestOccl, *m_LeftChestOcclusiveDressing)
      if (!m_LeftChestOcclusiveDressing->IsActive()) {
        RemoveLeftChestOcclusiveDressing();
        return true;
      }
      return IsValid(*m_LeftChestOcclusiveDressing);
    } else if (chestOccl->GetSide() == SESide::Right) {
      if (m_RightChestOcclusiveDressing == nullptr) {
        m_RightChestOcclusiveDressing = new SEChestOcclusiveDressing();
      }
      CDM_PATIENT_ACTION_COPY(ChestOcclusiveDressing, *chestOccl, *m_RightChestOcclusiveDressing)
      if (!m_RightChestOcclusiveDressing->IsActive()) {
        RemoveRightChestOcclusiveDressing();
        return true;
      }
      return IsValid(*m_RightChestOcclusiveDressing);
    }
  }

  auto conResp = dynamic_cast<const SEConsciousRespiration*>(&action);
  if (conResp != nullptr) {
    if (m_ConsciousRespiration == nullptr) {
      m_ConsciousRespiration = new SEConsciousRespiration();
    }
    CDM_PATIENT_ACTION_COPY(ConsciousRespiration, *conResp, *m_ConsciousRespiration)
    if (!m_ConsciousRespiration->IsActive()) {
      RemoveConsciousRespiration();
      return true;
    }
    return IsValid(*m_ConsciousRespiration);
  }

  auto consume = dynamic_cast<const SEConsumeNutrients*>(&action);
  if (consume != nullptr) {
    if (m_ConsumeNutrients == nullptr) {
      m_ConsumeNutrients = new SEConsumeNutrients();
    }
    CDM_PATIENT_ACTION_COPY(ConsumeNutrients, *consume, *m_ConsumeNutrients)
    if (!m_ConsumeNutrients->IsActive()) {
      RemoveConsumeNutrients();
      return true;
    }
    return IsValid(*m_ConsumeNutrients);
  }

  auto eschData = dynamic_cast<const SEEscharotomy*>(&action);
  if (eschData != nullptr) {
    auto eItr = m_Escharotomies.find(eschData->GetLocation());
    SEEscharotomy* escharotomy = std::make_unique<SEEscharotomy>().release();
    if (eItr != m_Escharotomies.end()) {
      escharotomy = m_Escharotomies[eschData->GetLocation()];
      CDM_PATIENT_ACTION_COPY(Escharotomy, *eschData, *escharotomy)
    } else {
      CDM_PATIENT_ACTION_COPY(Escharotomy, *eschData, *escharotomy)
      if (escharotomy->IsValid()) {
        m_Escharotomies[eschData->GetLocation()] = escharotomy;
        eItr = m_Escharotomies.find(eschData->GetLocation());
      } else {
        escharotomy->Clear();
      }
    }
    auto escharotomy2 = eItr->second;
    CDM_PATIENT_ACTION_COPY(Escharotomy, *eschData, *escharotomy2)
    if (!escharotomy2->IsActive()) {
      RemoveEscharotomy(escharotomy2->GetLocation());
      return true;
    }
    return IsValid(*escharotomy2);
  }

  auto ebola = dynamic_cast<const SEEbola*>(&action);
  if (ebola != nullptr) {
    if (m_Ebola == nullptr) {
      m_Ebola = new SEEbola();
    }
    CDM_PATIENT_ACTION_COPY(Ebola, *ebola, *m_Ebola)
    if (!m_Ebola->IsActive()) {
      RemoveEbola();
      return true;
    }
    return IsValid(*m_Ebola);
  }

  auto exercise = dynamic_cast<const SEExercise*>(&action);
  if (exercise != nullptr) {
    if (m_Exercise == nullptr) {
      m_Exercise = new SEExercise();
    }
    CDM_PATIENT_ACTION_COPY(Exercise, *exercise, *m_Exercise)
    if (!m_Exercise->IsActive()) {
      RemoveExercise();
      return true;
    }
    return IsValid(*m_Exercise);
  }

  auto hem = dynamic_cast<const SEHemorrhage*>(&action);
  if (hem != nullptr) {
    SEHemorrhage* myHem = m_Hemorrhages[hem->GetCompartment()];
    if (myHem == nullptr) {
      myHem = new SEHemorrhage();
      m_Hemorrhages[hem->GetCompartment()] = myHem;
    }
    CDM_PATIENT_ACTION_COPY(Hemorrhage, *hem, *myHem)

    if (!myHem->IsActive()) {
      RemoveHemorrhage(myHem->GetCompartment());
      return true;
    }
    return IsValid(*myHem);
  }

  auto infect = dynamic_cast<const SEInfection*>(&action);
  if (infect != nullptr) {
    if (m_Infection == nullptr) {
      m_Infection = new SEInfection();
    }
    CDM_PATIENT_ACTION_COPY(Infection, *infect, *m_Infection)
    if (!m_Infection->IsActive()) {
      m_Logger->Warning("Manual Eliminaton of Infection currently not supported.");
      return false;
    }
    return IsValid(*m_Infection);
  }

  auto intubation = dynamic_cast<const SEIntubation*>(&action);
  if (intubation != nullptr) {
    if (m_Intubation == nullptr) {
      m_Intubation = new SEIntubation();
    }
    CDM_PATIENT_ACTION_COPY(Intubation, *intubation, *m_Intubation)
    if (!m_Intubation->IsActive()) {
      Info("Turning off Intubation");
      RemoveIntubation();
      return true;
    }
    return IsValid(*m_Intubation);
  }

  auto mvData = dynamic_cast<const SEMechanicalVentilation*>(&action);
  if (mvData != nullptr) {
    if (m_MechanicalVentilation == nullptr) {
      m_MechanicalVentilation = new SEMechanicalVentilation();
    }
    CDM_PATIENT_ACTION_COPY(MechanicalVentilation, *mvData, *m_MechanicalVentilation)
    if (!m_MechanicalVentilation->IsActive()) {
      RemoveMechanicalVentilation();
      return true;
    }
    return IsValid(*m_MechanicalVentilation);
  }

  auto nCannula = dynamic_cast<const SENasalCannula*>(&action);
  if (nCannula != nullptr) {
    if (m_NasalCannula == nullptr) {
      m_NasalCannula = new SENasalCannula();
    }
    CDM_PATIENT_ACTION_COPY(NasalCannula, *nCannula, *m_NasalCannula)
    if (!m_NasalCannula->IsActive()) {
      RemoveNasalCannula();
      return true;
    }
    return IsValid(*m_NasalCannula);
  }

  auto needleDecomp = dynamic_cast<const SENeedleDecompression*>(&action);
  if (needleDecomp != nullptr) {
    if (needleDecomp->GetSide() == SESide::Left) {
      if (m_LeftNeedleDecompression == nullptr) {
        m_LeftNeedleDecompression = new SENeedleDecompression();
      }
      CDM_PATIENT_ACTION_COPY(NeedleDecompression, *needleDecomp, *m_LeftNeedleDecompression)
      if (!m_LeftNeedleDecompression->IsActive()) {
        RemoveLeftNeedleDecompression();
        return true;
      }
      return IsValid(*m_LeftNeedleDecompression);
    } else if (needleDecomp->GetSide() == SESide::Right) {
      if (m_RightNeedleDecompression == nullptr) {
        m_RightNeedleDecompression = new SENeedleDecompression();
      }
      CDM_PATIENT_ACTION_COPY(NeedleDecompression, *needleDecomp, *m_RightNeedleDecompression)
      if (!m_RightNeedleDecompression->IsActive()) {
        RemoveRightNeedleDecompression();
        return true;
      }
      return IsValid(*m_RightNeedleDecompression);
    } else {
      return false;
    }
  }

  auto pain = dynamic_cast<const SEPainStimulus*>(&action);
  if (pain != nullptr) {
    SEPainStimulus* mypain = m_PainStimuli[pain->GetLocation()];
    if (mypain == nullptr) {
      mypain = new SEPainStimulus();
      m_PainStimuli[pain->GetLocation()] = mypain;
    }
    CDM_PATIENT_ACTION_COPY(PainStimulus, *pain, *mypain)

    if (!mypain->IsActive()) {
      RemovePainStimulus(mypain->GetLocation());
      return true;
    }
    return IsValid(*mypain);
  }

  auto pericardialEff = dynamic_cast<const SEPericardialEffusion*>(&action);
  if (pericardialEff != nullptr) {
    if (m_PericardialEffusion == nullptr) {
      m_PericardialEffusion = new SEPericardialEffusion();
    }
    CDM_PATIENT_ACTION_COPY(PericardialEffusion, *pericardialEff, *m_PericardialEffusion)
    if (!m_PericardialEffusion->IsActive()) {
      RemovePericardialEffusion();
      return true;
    }
    return IsValid(*m_PericardialEffusion);
  }

  auto pulmonaryShunt = dynamic_cast<const SEPulmonaryShunt*>(&action);
  if (pulmonaryShunt != nullptr) {
    if (m_PulmonaryShunt == nullptr) {
      m_PulmonaryShunt = new SEPulmonaryShunt();
    }
    CDM_PATIENT_ACTION_COPY(PulmonaryShunt, *pulmonaryShunt, *m_PulmonaryShunt)
    if (!m_PulmonaryShunt->IsActive()) {
      RemovePulmonaryShunt();
      return true;
    }
    return IsValid(*m_PulmonaryShunt);
  }

  auto radabs = dynamic_cast<const SERadiationAbsorbedDose*>(&action);
  if (radabs != nullptr) {
    if (m_RadiationAbsorbedDose == nullptr)
      m_RadiationAbsorbedDose = new SERadiationAbsorbedDose();
    CDM_PATIENT_ACTION_COPY(RadiationAbsorbedDose, *radabs, *m_RadiationAbsorbedDose)
    if (!m_RadiationAbsorbedDose->IsActive()) {
      RemoveRadiationAbsorbedDose();
      return true;
    }
    return IsValid(*m_RadiationAbsorbedDose);
  }

  auto sleep = dynamic_cast<const SESleep*>(&action);
  if (sleep != nullptr) {
    if (m_Sleep == nullptr)
      m_Sleep = new SESleep();
    CDM_PATIENT_ACTION_COPY(Sleep, *sleep, *m_Sleep)
    if (!m_Sleep->IsActive()) {
      RemoveSleepState();
      return true;
    }
    return IsValid(*m_Sleep);
  }

  auto admin = dynamic_cast<const SESubstanceAdministration*>(&action);
  if (admin != nullptr) {
    return AdministerSubstance(*admin);
  }

  auto pneumo = dynamic_cast<const SETensionPneumothorax*>(&action);
  if (pneumo != nullptr) {
    if (pneumo->GetSide() == SESide::Left && pneumo->GetType() == SEPneumothoraxType::Open) {
      if (m_LeftOpenTensionPneumothorax == nullptr) {
        m_LeftOpenTensionPneumothorax = new SETensionPneumothorax();
      }
      CDM_PATIENT_ACTION_COPY(TensionPneumothorax, *pneumo, *m_LeftOpenTensionPneumothorax)
      if (!m_LeftOpenTensionPneumothorax->IsActive()) {
        RemoveLeftClosedTensionPneumothorax();
        return true;
      }
      return IsValid(*m_LeftOpenTensionPneumothorax);
    } else if (pneumo->GetSide() == SESide::Left && pneumo->GetType() == SEPneumothoraxType::Closed) {
      if (m_LeftClosedTensionPneumothorax == nullptr) {
        m_LeftClosedTensionPneumothorax = new SETensionPneumothorax();
      }
      CDM_PATIENT_ACTION_COPY(TensionPneumothorax, *pneumo, *m_LeftClosedTensionPneumothorax)
      if (!m_LeftClosedTensionPneumothorax->IsActive()) {
        RemoveLeftOpenTensionPneumothorax();
        return true;
      }
      return IsValid(*m_LeftClosedTensionPneumothorax);
    } else if (pneumo->GetSide() == SESide::Right && pneumo->GetType() == SEPneumothoraxType::Open) {
      if (m_RightOpenTensionPneumothorax == nullptr) {
        m_RightOpenTensionPneumothorax = new SETensionPneumothorax();
      }
      CDM_PATIENT_ACTION_COPY(TensionPneumothorax, *pneumo, *m_RightOpenTensionPneumothorax)
      if (!m_RightOpenTensionPneumothorax->IsActive()) {
        RemoveRightOpenTensionPneumothorax();
        return true;
      }
      return IsValid(*m_RightOpenTensionPneumothorax);
    } else if (pneumo->GetSide() == SESide::Right && pneumo->GetType() == SEPneumothoraxType::Closed) {
      if (m_RightClosedTensionPneumothorax == nullptr) {
        m_RightClosedTensionPneumothorax = new SETensionPneumothorax();
      }
      CDM_PATIENT_ACTION_COPY(TensionPneumothorax, *pneumo, *m_RightClosedTensionPneumothorax)
      if (!m_RightClosedTensionPneumothorax->IsActive()) {
        RemoveRightClosedTensionPneumothorax();
        return true;
      }
      return IsValid(*m_RightClosedTensionPneumothorax);
    }
    return false; // Duno what this is...
  }

  auto tournData = dynamic_cast<const SETourniquet*>(&action);
  if (tournData != nullptr) {
    auto tItr = m_Tourniquets.find(tournData->GetCompartment());

    bool matchingHem = false;
    bool validCmpt = false;
    std::stringstream warn;
    if (m_Hemorrhages.find(tournData->GetCompartment()) != m_Hemorrhages.end()) {
      matchingHem = true;
      SETourniquet* tourniquet = std::make_unique<SETourniquet>().release();
      if (tItr != m_Tourniquets.end()) {
        tourniquet = m_Tourniquets[tournData->GetCompartment()];
        CDM_PATIENT_ACTION_COPY(Tourniquet, *tournData, *tourniquet)
        validCmpt = true;
      } else {
        CDM_PATIENT_ACTION_COPY(Tourniquet, *tournData, *tourniquet)
        if (tourniquet->IsValid()) {
          m_Tourniquets[tournData->GetCompartment()] = tourniquet;
          tItr = m_Tourniquets.find(tournData->GetCompartment());
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
      auto Tourniquet = tItr->second;
      CDM_PATIENT_ACTION_COPY(Tourniquet, *tournData, *Tourniquet)
      if (!Tourniquet->IsActive()) {
        RemoveTourniquet(Tourniquet->GetCompartment());
        return true;
      }
      return IsValid(*Tourniquet);
    } else {
      Warning(warn);
      return false;
    }
  }

  auto urinate = dynamic_cast<const SEUrinate*>(&action);
  if (urinate != nullptr) {
    if (m_Urinate == nullptr) {
      m_Urinate = new SEUrinate();
    }
    CDM_PATIENT_ACTION_COPY(Urinate, *urinate, *m_Urinate)
    if (!m_Urinate->IsActive()) {
      RemoveUrinate();
      return true;
    }
    return IsValid(*m_Urinate);
  }

  auto orData = dynamic_cast<const SEOverride*>(&action);
  if (orData != nullptr) {
    if (m_OverrideAction == nullptr) {
      m_OverrideAction = new SEOverride();
    }
    CDM_PATIENT_ACTION_COPY(Override, *orData, *m_OverrideAction)
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
bool SEPatientActionCollection::HasExampleAction() const
{
  return m_ExampleAction == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEExampleAction* SEPatientActionCollection::GetExampleAction() const
{
  return m_ExampleAction;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveExampleAction()
{
  SAFE_DELETE(m_ExampleAction);
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
const MapWrapper<std::string, SEEscharotomy*> SEPatientActionCollection::GetEscharotomiesWrapper() const
{
  return MapWrapper<std::string, SEEscharotomy*>(const_cast<SEPatientActionCollection*>(this)->m_Escharotomies);
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
bool SEPatientActionCollection::HasEbola() const
{
  return m_Ebola == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEEbola* SEPatientActionCollection::GetEbola() const
{
  return m_Ebola;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveEbola()
{
  SAFE_DELETE(m_Ebola);
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
const MapWrapper<std::string, SEHemorrhage*> SEPatientActionCollection::GetHemorrhageWrapper() const
{
  return MapWrapper<std::string, SEHemorrhage*>(const_cast<SEPatientActionCollection*>(this)->m_Hemorrhages);
}
//!
//! API usuage for UE4 do not use internally
//!
SEHemorrhage const* SEPatientActionCollection::GetFirstHemorrhage() const
{
  if (m_Hemorrhages.begin() == m_Hemorrhages.end() || m_Hemorrhages.size() == 0) {
    return nullptr;
  }

  m_HemorrhageItr = m_Hemorrhages.begin();

  return m_HemorrhageItr->second;
}

//!
//! API usuage for UE4 do not use internally
//!
SEHemorrhage const* SEPatientActionCollection::GetNextHemorrhage() const
{
  if (m_Hemorrhages.begin() == m_Hemorrhages.end() || m_Hemorrhages.size() == 0) {
    return nullptr;
  }

  if (m_HemorrhageItr == m_Hemorrhages.end()) {
    return nullptr;
  }

  m_HemorrhageItr++;

  return (m_HemorrhageItr == m_Hemorrhages.end()) ? nullptr : m_HemorrhageItr->second;
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
bool SEPatientActionCollection::HasNasalCannula() const
{
  return m_NasalCannula == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SENasalCannula* SEPatientActionCollection::GetNasalCannula() const
{
  return m_NasalCannula;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveNasalCannula()
{
  SAFE_DELETE(m_NasalCannula);
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
const MapWrapper<std::string, SEPainStimulus*> SEPatientActionCollection::GetPainStimuliWrapper() const
{
  return const_cast<SEPatientActionCollection*>(this)->m_PainStimuli;
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
bool SEPatientActionCollection::HasRadiationAbsorbedDose() const
{
  return m_RadiationAbsorbedDose == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SERadiationAbsorbedDose* SEPatientActionCollection::GetRadiationAbsorbedDose() const
{
  return m_RadiationAbsorbedDose;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveRadiationAbsorbedDose()
{
  SAFE_DELETE(m_RadiationAbsorbedDose);
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
  return m_LeftOpenTensionPneumothorax == nullptr ? false : true; // m_LeftOpenTensionPneumothorax->IsValid();//TODO
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
  return m_LeftClosedTensionPneumothorax == nullptr ? false : true; // m_LeftClosedTensionPneumothorax->IsValid();//TODO
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
  return m_RightOpenTensionPneumothorax == nullptr ? false : true; // m_RightOpenTensionPneumothorax->IsValid();//TODO
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
  return m_RightClosedTensionPneumothorax == nullptr ? false : true; // m_RightClosedTensionPneumothorax->IsValid();//TODO
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
const MapWrapper<const SESubstance*, SESubstanceBolus*> SEPatientActionCollection::GetSubstanceBolusesWrapper() const
{
  return const_cast<SEPatientActionCollection*>(this)->m_SubstanceBolus;
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
const MapWrapper<const SESubstance*, SESubstanceInfusion*> SEPatientActionCollection::GetSubstanceInfusionsWrapper() const
{
  return const_cast<SEPatientActionCollection*>(this)->m_SubstanceInfusions;
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
const MapWrapper<const SESubstance*, SESubstanceOralDose*> SEPatientActionCollection::GetSubstanceOralDosesWrapper() const
{
  return const_cast<SEPatientActionCollection*>(this)->m_SubstanceOralDoses;
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
const MapWrapper<const SESubstance*, SESubstanceNasalDose*> SEPatientActionCollection::GetSubstanceNasalDosesWrapper() const
{
  return const_cast<SEPatientActionCollection*>(this)->m_SubstanceNasalDoses;
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
const MapWrapper<const SESubstanceCompound*, SESubstanceCompoundInfusion*> SEPatientActionCollection::GetSubstanceCompoundInfusionsWrapper() const
{
  return const_cast<SEPatientActionCollection*>(this)->m_SubstanceCompoundInfusions;
}
//-------------------------------------------------------------------------------
void SEPatientActionCollection::RemoveSubstanceCompoundInfusion(const SESubstanceCompound& cSub)
{
  SESubstanceCompoundInfusion* sci = m_SubstanceCompoundInfusions[&cSub];
  m_SubstanceCompoundInfusions.erase(&cSub);
  SAFE_DELETE(sci);
}
//-------------------------------------------------------------------------------
bool SEPatientActionCollection::AdministerSubstance(const SESubstanceAdministration& subAdmin)
{
  auto bolus = dynamic_cast<const SESubstanceBolus*>(&subAdmin);
  if (bolus != nullptr) {
    SESubstance* sub = &bolus->GetSubstance();
    SESubstanceBolus* myBolus = m_SubstanceBolus[sub];
    if (myBolus == nullptr) {
      myBolus = new SESubstanceBolus(*sub);
      m_SubstanceBolus[sub] = myBolus;
      m_Substances.AddActiveSubstance(*sub);
    }
    CDM_PATIENT_ACTION_COPY(SubstanceBolus, *bolus, *myBolus)
    if (!myBolus->IsActive()) {
      RemoveSubstanceBolus(*sub);
      return true;
    }
    return IsValid(*myBolus);
  }

  auto subInfusion = dynamic_cast<const SESubstanceInfusion*>(&subAdmin);
  if (subInfusion != nullptr) {
    SESubstance* sub = &subInfusion->GetSubstance();
    SESubstanceInfusion* mySubInfuse = m_SubstanceInfusions[sub];
    if (mySubInfuse == nullptr) {
      mySubInfuse = new SESubstanceInfusion(*sub);
      m_SubstanceInfusions[sub] = mySubInfuse;
      m_Substances.AddActiveSubstance(*sub);
    }
    CDM_PATIENT_ACTION_COPY(SubstanceInfusion, *subInfusion, *mySubInfuse)
    if (!mySubInfuse->IsActive()) {
      RemoveSubstanceInfusion(*sub);
      return true;
    }
    return IsValid(*mySubInfuse);
  }

  auto NasalDose = dynamic_cast<const SESubstanceNasalDose*>(&subAdmin);
  if (NasalDose != nullptr) {
    SESubstance* sub = &NasalDose->GetSubstance();
    SESubstanceNasalDose* myNasalDose = m_SubstanceNasalDoses[sub];
    if (myNasalDose == nullptr) {
      myNasalDose = new SESubstanceNasalDose(*sub);
      m_SubstanceNasalDoses[sub] = myNasalDose;
      m_Substances.AddActiveSubstance(*sub);
    }
    CDM_PATIENT_ACTION_COPY(SubstanceNasalDose, *NasalDose, *myNasalDose)
    if (!myNasalDose->IsActive()) {
      RemoveSubstanceNasalDose(*sub);
      return true;
    }
    return IsValid(*myNasalDose);
  }

  auto oralDose = dynamic_cast<const SESubstanceOralDose*>(&subAdmin);
  if (oralDose != nullptr) {
    SESubstance* sub = &oralDose->GetSubstance();
    SESubstanceOralDose* myOralDose = m_SubstanceOralDoses[sub];
    if (myOralDose == nullptr) {
      myOralDose = new SESubstanceOralDose(*sub);
      m_SubstanceOralDoses[sub] = myOralDose;
      m_Substances.AddActiveSubstance(*sub);
    }
    CDM_PATIENT_ACTION_COPY(SubstanceOralDose, *oralDose, *myOralDose)
    if (!myOralDose->IsActive()) {
      RemoveSubstanceOralDose(*sub);
      return true;
    }
    return IsValid(*myOralDose);
  }

  auto cSubInfusion = dynamic_cast<const SESubstanceCompoundInfusion*>(&subAdmin);
  if (cSubInfusion != nullptr) {
    SESubstanceCompound* cmpd = &cSubInfusion->GetSubstanceCompound();
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
    CDM_PATIENT_ACTION_COPY(SubstanceCompoundInfusion, *cSubInfusion, *mySubCompInfuse)
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
const MapWrapper<std::string, SETourniquet*> SEPatientActionCollection::GetTourniquetsWrapper() const
{
  return const_cast<SEPatientActionCollection*>(this)->m_Tourniquets;
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
