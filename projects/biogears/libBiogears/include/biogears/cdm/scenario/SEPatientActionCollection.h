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

#pragma once
#include <biogears/exports.h>

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/patient/actions/SEAcuteRespiratoryDistress.h>
#include <biogears/cdm/patient/actions/SEAcuteStress.h>
#include <biogears/cdm/patient/actions/SEExampleAction.h>
#include <biogears/cdm/patient/actions/SEAirwayObstruction.h>
#include <biogears/cdm/patient/actions/SEApnea.h>
#include <biogears/cdm/patient/actions/SEAsthmaAttack.h>
#include <biogears/cdm/patient/actions/SEBrainInjury.h>
#include <biogears/cdm/patient/actions/SEBronchoconstriction.h>
#include <biogears/cdm/patient/actions/SEBurnWound.h>
#include <biogears/cdm/patient/actions/SECardiacArrest.h>
#include <biogears/cdm/patient/actions/SEChestCompressionForce.h>
#include <biogears/cdm/patient/actions/SEChestCompressionForceScale.h>
#include <biogears/cdm/patient/actions/SEChestOcclusiveDressing.h>
#include <biogears/cdm/patient/actions/SEConsciousRespiration.h>
#include <biogears/cdm/patient/actions/SEConsumeNutrients.h>
#include <biogears/cdm/patient/actions/SEEscharotomy.h>
#include <biogears/cdm/patient/actions/SEExercise.h>
#include <biogears/cdm/patient/actions/SEEbola.h>
#include <biogears/cdm/patient/actions/SEHemorrhage.h>
#include <biogears/cdm/patient/actions/SEInfection.h>
#include <biogears/cdm/patient/actions/SEIntubation.h>
#include <biogears/cdm/patient/actions/SENasalCannula.h>
#include <biogears/cdm/patient/actions/SEMechanicalVentilation.h>
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
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/physiology/SEGastrointestinalSystem.h>

#pragma warning(disable : 4661)

namespace std {
extern template class map<string, biogears::SEHemorrhage*>;
extern template class map<string, biogears::SETourniquet*>;
extern template class map<string, biogears::SEEscharotomy*>;
extern template class map<string, biogears::SEPainStimulus*>;
extern template class map<const biogears::SESubstance*, biogears::SESubstanceBolus*>;
extern template class map<const biogears::SESubstance*, biogears::SESubstanceInfusion*>;
extern template class map<const biogears::SESubstance*, biogears::SESubstanceOralDose*>;
extern template class map<const biogears::SESubstance*, biogears::SESubstanceNasalDose*>;
extern template class map<const biogears::SESubstanceCompound*, biogears::SESubstanceCompoundInfusion*>;
}
#pragma warning(default : 4661)


namespace biogears {
class PhysiologyEngine;
namespace io {
  class PatientActions;
}
//!
//! This is an emergency fix for working with UE4
//! If this stays around until 7.6 this code will
//! be renamed and refactoredto its own file
//! The problem is that standard_iterator is a 
//! derived type and is very hard to properly export 
//! to a DLL but our clients need access to iterative substances
//! So this code and its wonderful I wrote this in an hourness is an emergcy fix
//! To avoid crashes when different sides of the DLL boundry disagree on the size of std_iterator
//! When executing foreach loops
  
template <typename KeyType, typename ValueType>
class PairWrapper {
public:
  PairWrapper(typename std::pair<const KeyType, ValueType>*);
  ~PairWrapper();

  KeyType const& first();
  ValueType& second();
private:
  typename std::pair<const KeyType, ValueType>* _ptr;
};

template <typename KeyType, typename ValueType>
class MapIteratorWrapper {
public:
  MapIteratorWrapper(typename std::map<KeyType, ValueType>::iterator itr);
  ~MapIteratorWrapper();


  bool operator==(MapIteratorWrapper const& rhs) const;
  bool operator!=(MapIteratorWrapper const& rhs) const;

 
  auto operator()() const -> PairWrapper<KeyType, ValueType>;
  auto operator*()  const -> PairWrapper<KeyType, ValueType>;

  MapIteratorWrapper& operator++(); //Prefix  Operator
  MapIteratorWrapper  operator++(int); //Postfix Operator

private:
  typename std::map<KeyType, ValueType>::iterator _iterator;
};

template <typename KeyType, typename ValueType>
class MapWrapper {
public:
  MapWrapper(std::map<KeyType, ValueType>& given);
  ~MapWrapper();

  auto begin() const -> MapIteratorWrapper<KeyType, ValueType>;
  auto end() const -> MapIteratorWrapper<KeyType, ValueType>;

private:
  std::map<KeyType, ValueType>& _map;
};

template <typename ValueType>
class VectorIteratorWrapper {
public:
  VectorIteratorWrapper(typename std::vector<ValueType>::iterator itr);
  ~VectorIteratorWrapper();

  ValueType const& value() const;

  bool operator==(VectorIteratorWrapper const& rhs) const;
  bool operator!=(VectorIteratorWrapper const& rhs) const;

  ValueType const& operator()() const;
  ValueType const* operator->();
  ValueType const& operator*();

  VectorIteratorWrapper& operator++(); //Prefix  Operator
  VectorIteratorWrapper operator++(int); //Postfix Operator
private:
  typename std::vector<ValueType>::iterator _iterator;
};

template <typename ValueType>
class VectorWrapper {
public:
  VectorWrapper(std::vector<ValueType>& given);
  ~VectorWrapper();

  VectorIteratorWrapper<ValueType> begin() const;
  VectorIteratorWrapper<ValueType> end() const;

private:
  std::vector<ValueType>& _vector;
};

#pragma warning(disable : 4661)

extern template class PairWrapper<std::string, SEHemorrhage*>;
extern template class PairWrapper<std::string, SETourniquet*>;
extern template class PairWrapper<std::string, SEEscharotomy*>;
extern template class PairWrapper<std::string, SEPainStimulus*>;
extern template class PairWrapper<const SESubstance*, SESubstanceBolus*>;
extern template class PairWrapper<const SESubstance*, SESubstanceInfusion*>;
extern template class PairWrapper<const SESubstance*, SESubstanceOralDose*>;
extern template class PairWrapper<const SESubstance*, SESubstanceNasalDose*>;
extern template class PairWrapper<const SESubstanceCompound*, SESubstanceCompoundInfusion*>;

extern template class MapIteratorWrapper<std::string, SEHemorrhage*>;
extern template class MapIteratorWrapper<std::string, SETourniquet*>;
extern template class MapIteratorWrapper<std::string, SEEscharotomy*>;
extern template class MapIteratorWrapper<std::string, SEPainStimulus*>;
extern template class MapIteratorWrapper<const SESubstance*, SESubstanceBolus*>;
extern template class MapIteratorWrapper<const SESubstance*, SESubstanceInfusion*>;
extern template class MapIteratorWrapper<const SESubstance*, SESubstanceOralDose*>;
extern template class MapIteratorWrapper<const SESubstance*, SESubstanceNasalDose*>;
extern template class MapIteratorWrapper<const SESubstanceCompound*, SESubstanceCompoundInfusion*>;

extern template class MapWrapper<std::string, SEHemorrhage*>;
extern template class MapWrapper<std::string, SETourniquet*>;
extern template class MapWrapper<std::string, SEEscharotomy*>;
extern template class MapWrapper<std::string, SEPainStimulus*>;
extern template class MapWrapper<const SESubstance*, SESubstanceBolus*>;
extern template class MapWrapper<const SESubstance*, SESubstanceInfusion*>;
extern template class MapWrapper<const SESubstance*, SESubstanceOralDose*>;
extern template class MapWrapper<const SESubstance*, SESubstanceNasalDose*>;
extern template class MapWrapper<const SESubstanceCompound*, SESubstanceCompoundInfusion*>;

#pragma warning(default : 4661)

}

//Marks the end of Wrapper Code


namespace biogears {
class BIOGEARS_API SEPatientActionCollection : public Loggable {
  friend class io::PatientActions;

public:
  SEPatientActionCollection(SESubstanceManager&);
  ~SEPatientActionCollection();

  void Clear();

  bool ProcessAction(const SEPatientAction& action, const PhysiologyEngine& engine);

  bool HasAcuteRespiratoryDistress() const;
  SEAcuteRespiratoryDistress* GetAcuteRespiratoryDistress() const;
  void RemoveAcuteRespiratoryDistress();

  bool HasAcuteStress() const;
  SEAcuteStress* GetAcuteStress() const;
  void RemoveAcuteStress();

  bool HasExampleAction() const;
  SEExampleAction* GetExampleAction() const;
  void RemoveExampleAction();

  bool HasAirwayObstruction() const;
  SEAirwayObstruction* GetAirwayObstruction() const;
  void RemoveAirwayObstruction();

  bool HasApnea() const;
  SEApnea* GetApnea() const;
  void RemoveApnea();

  bool HasAsthmaAttack() const;
  SEAsthmaAttack* GetAsthmaAttack() const;
  void RemoveAsthmaAttack();

  bool HasBrainInjury() const;
  SEBrainInjury* GetBrainInjury() const;
  void RemoveBrainInjury();

  bool HasBronchoconstriction() const;
  SEBronchoconstriction* GetBronchoconstriction() const;
  void RemoveBronchoconstriction();

  bool HasBurnWound() const;
  SEBurnWound* GetBurnWound() const;
  void RemoveBurnWound();

  bool HasCardiacArrest() const;
  SECardiacArrest* GetCardiacArrest() const;
  void RemoveCardiacArrest();

  bool HasChestCompression() const;
  void RemoveChestCompression();
  bool HasChestCompressionForce() const;
  SEChestCompressionForce* GetChestCompressionForce() const;
  bool HasChestCompressionForceScale() const;
  SEChestCompressionForceScale* GetChestCompressionForceScale() const;

  bool HasChestOcclusiveDressing() const;
  bool HasLeftChestOcclusiveDressing() const;
  SEChestOcclusiveDressing* GetLeftChestOcclusiveDressing() const;
  void RemoveLeftChestOcclusiveDressing();
  bool HasRightChestOcclusiveDressing() const;
  SEChestOcclusiveDressing* GetRightChestOcclusiveDressing() const;
  void RemoveRightChestOcclusiveDressing();

  bool HasConsciousRespiration() const;
  SEConsciousRespiration* GetConsciousRespiration() const;
  void RemoveConsciousRespiration();

  bool HasConsumeNutrients() const;
  SEConsumeNutrients* GetConsumeNutrients() const;
  void RemoveConsumeNutrients();

  bool HasEscharotomy() const;
  const std::map<std::string, SEEscharotomy*>& GetEscharotomies() const;
  const MapWrapper<std::string, SEEscharotomy*> GetEscharotomiesWrapper() const;
  void RemoveEscharotomy(const char* cmpt);
  void RemoveEscharotomy(const std::string& cmpt);

  bool HasEbola() const;
  SEEbola* GetEbola() const;
  void RemoveEbola();

  bool HasExercise() const;
  SEExercise* GetExercise() const;
  void RemoveExercise();

  bool HasHemorrhage() const;
  const std::map<std::string, SEHemorrhage*>& GetHemorrhages() const;
  const MapWrapper<std::string, SEHemorrhage*> GetHemorrhageWrapper() const;

  SEHemorrhage const* GetFirstHemorrhage() const; //< Must be called after anything that would invalidate the underlying map
  SEHemorrhage const* GetNextHemorrhage() const; //< returns the iterators target or nullptr when itr == end.  Assumes no nullptr will ever be stored in Active hemmorhage map.

  void RemoveHemorrhage(const char* cmpt);
  void RemoveHemorrhage(const std::string& cmpt);

  bool HasInfection() const;
  SEInfection* GetInfection() const;
  void RemoveInfection();

  bool HasIntubation() const;
  SEIntubation* GetIntubation() const;
  void RemoveIntubation();

  bool HasMechanicalVentilation() const;
  SEMechanicalVentilation* GetMechanicalVentilation() const;
  void RemoveMechanicalVentilation();

  bool HasNasalCannula() const;
  SENasalCannula* GetNasalCannula() const;
  void RemoveNasalCannula();

  bool HasNeedleDecompression() const;

  bool HasLeftNeedleDecompression() const;
  SENeedleDecompression* GetLeftNeedleDecompression() const;
  void RemoveLeftNeedleDecompression();

  bool HasRightNeedleDecompression() const;
  SENeedleDecompression* GetRightNeedleDecompression() const;
  void RemoveRightNeedleDecompression();

  bool HasPainStimulus() const;
  const std::map<std::string, SEPainStimulus*>& GetPainStimuli() const;
  const MapWrapper<std::string, SEPainStimulus*> GetPainStimuliWrapper() const;
  void RemovePainStimulus(const char* loc);
  void RemovePainStimulus(const std::string& loc);

  bool HasPericardialEffusion() const;
  SEPericardialEffusion* GetPericardialEffusion() const;
  void RemovePericardialEffusion();

  bool HasPulmonaryShunt() const;
  SEPulmonaryShunt* GetPulmonaryShunt() const;
  void RemovePulmonaryShunt();

  bool HasRadiationAbsorbedDose() const;
  SERadiationAbsorbedDose* GetRadiationAbsorbedDose() const;
  void RemoveRadiationAbsorbedDose();

  bool HasSleepState() const;
  SESleep* GetSleepState() const;
  void RemoveSleepState();

  bool HasTensionPneumothorax() const;

  bool HasLeftClosedTensionPneumothorax() const;
  SETensionPneumothorax* GetLeftClosedTensionPneumothorax() const;
  void RemoveLeftClosedTensionPneumothorax();

  bool HasLeftOpenTensionPneumothorax() const;
  SETensionPneumothorax* GetLeftOpenTensionPneumothorax() const;
  void RemoveLeftOpenTensionPneumothorax();

  bool HasRightClosedTensionPneumothorax() const;
  SETensionPneumothorax* GetRightClosedTensionPneumothorax() const;
  void RemoveRightClosedTensionPneumothorax();

  bool HasRightOpenTensionPneumothorax() const;
  SETensionPneumothorax* GetRightOpenTensionPneumothorax() const;
  void RemoveRightOpenTensionPneumothorax();

  const std::map<const SESubstance*, SESubstanceBolus*>& GetSubstanceBoluses() const;
  const MapWrapper<const SESubstance*, SESubstanceBolus*> GetSubstanceBolusesWrapper() const;
  void RemoveSubstanceBolus(const SESubstance& sub);

  const std::map<const SESubstance*, SESubstanceInfusion*>& GetSubstanceInfusions() const;
  const MapWrapper<const SESubstance*, SESubstanceInfusion*> GetSubstanceInfusionsWrapper() const;
  void RemoveSubstanceInfusion(const SESubstance& sub);

  const std::map<const SESubstance*, SESubstanceNasalDose*>& GetSubstanceNasalDoses() const;
  const MapWrapper<const SESubstance*, SESubstanceNasalDose*> GetSubstanceNasalDosesWrapper() const;
  void RemoveSubstanceNasalDose(const SESubstance& sub);

  const std::map<const SESubstance*, SESubstanceOralDose*>& GetSubstanceOralDoses() const;
  const MapWrapper<const SESubstance*, SESubstanceOralDose*> GetSubstanceOralDosesWrapper() const;
  void RemoveSubstanceOralDose(const SESubstance& sub);

  const std::map<const SESubstanceCompound*, SESubstanceCompoundInfusion*>& GetSubstanceCompoundInfusions() const;
  const MapWrapper<const SESubstanceCompound*, SESubstanceCompoundInfusion*> GetSubstanceCompoundInfusionsWrapper() const;
  void RemoveSubstanceCompoundInfusion(const SESubstanceCompound& sub);

  bool HasTourniquet() const;
  const std::map<std::string, SETourniquet*>& GetTourniquets() const;
  const MapWrapper<std::string, SETourniquet*> GetTourniquetsWrapper() const;
  void RemoveTourniquet(const char* cmpt);
  void RemoveTourniquet(const std::string& cmpt);

  bool HasUrinate() const;
  SEUrinate* GetUrinate() const;
  void RemoveUrinate();

  bool HasOverride() const;
  SEOverride* GetOverride();
  void RemoveOverride();

protected:
  bool IsValid(const SEPatientAction& action);

  SEAcuteRespiratoryDistress* m_AcuteRespiratoryDistress;
  SEAcuteStress* m_AcuteStress;
  SEExampleAction* m_ExampleAction;
  SEAirwayObstruction* m_AirwayObstruction;
  SEApnea* m_Apnea;
  SEAsthmaAttack* m_AsthmaAttack;
  SEBrainInjury* m_BrainInjury;
  SEBronchoconstriction* m_Bronchoconstriction;
  SEBurnWound* m_BurnWound;
  SECardiacArrest* m_CardiacArrest;
  SEChestCompression* m_ChestCompression;
  SEChestOcclusiveDressing* m_LeftChestOcclusiveDressing;
  SEChestOcclusiveDressing* m_RightChestOcclusiveDressing;
  SEConsciousRespiration* m_ConsciousRespiration;
  SEConsumeNutrients* m_ConsumeNutrients;
  SEEbola* m_Ebola;
  SEExercise* m_Exercise;
  SEInfection* m_Infection;
  SEIntubation* m_Intubation;
  SEMechanicalVentilation* m_MechanicalVentilation;
  SENasalCannula* m_NasalCannula;
  SENeedleDecompression* m_LeftNeedleDecompression;
  SENeedleDecompression* m_RightNeedleDecompression;
  SEPericardialEffusion* m_PericardialEffusion;
  SEPulmonaryShunt* m_PulmonaryShunt;
  SERadiationAbsorbedDose* m_RadiationAbsorbedDose;
  SESleep* m_Sleep;
  SETensionPneumothorax* m_LeftClosedTensionPneumothorax;
  SETensionPneumothorax* m_LeftOpenTensionPneumothorax;
  SETensionPneumothorax* m_RightClosedTensionPneumothorax;
  SETensionPneumothorax* m_RightOpenTensionPneumothorax;
  SEUrinate* m_Urinate;
  SEOverride* m_OverrideAction;

  std::map<std::string, SEHemorrhage*> m_Hemorrhages;
  mutable std::map<std::string, SEHemorrhage*>::const_iterator m_HemorrhageItr;

  std::map<std::string, SETourniquet*> m_Tourniquets;
  std::map<std::string, SEEscharotomy*> m_Escharotomies;
  std::map<std::string, SEPainStimulus*> m_PainStimuli;
  std::map<const SESubstance*, SESubstanceBolus*> m_SubstanceBolus;
  std::map<const SESubstance*, SESubstanceInfusion*> m_SubstanceInfusions;
  std::map<const SESubstance*, SESubstanceOralDose*> m_SubstanceOralDoses;
  std::map<const SESubstance*, SESubstanceNasalDose*> m_SubstanceNasalDoses;
  std::map<const SESubstanceCompound*, SESubstanceCompoundInfusion*> m_SubstanceCompoundInfusions;

  bool AdministerSubstance(const SESubstanceAdministration& subAdmin);

  SESubstanceManager& m_Substances;
  std::stringstream m_ss;
};
}