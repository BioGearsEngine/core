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
#include <biogears/cdm/patient/actions/SEAcuteStress.h>
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
#include <biogears/cdm/patient/actions/SEExercise.h>
#include <biogears/cdm/patient/actions/SEHemorrhage.h>
#include <biogears/cdm/patient/actions/SEInfection.h>
#include <biogears/cdm/patient/actions/SEIntubation.h>
#include <biogears/cdm/patient/actions/SEMechanicalVentilation.h>
#include <biogears/cdm/patient/actions/SENeedleDecompression.h>
#include <biogears/cdm/patient/actions/SEOverride.h>
#include <biogears/cdm/patient/actions/SEPainStimulus.h>
#include <biogears/cdm/patient/actions/SEPatientAssessmentRequest.h>
#include <biogears/cdm/patient/actions/SEPericardialEffusion.h>
#include <biogears/cdm/patient/actions/SESleep.h>
#include <biogears/cdm/patient/actions/SESubstanceBolus.h>
#include <biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h>
#include <biogears/cdm/patient/actions/SESubstanceInfusion.h>
#include <biogears/cdm/patient/actions/SESubstanceOralDose.h>
#include <biogears/cdm/patient/actions/SETensionPneumothorax.h>
#include <biogears/cdm/patient/actions/SETourniquet.h>
#include <biogears/cdm/patient/actions/SEUrinate.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/physiology/SEGastrointestinalSystem.h>

namespace biogears {
class BIOGEARS_API SEPatientActionCollection : public Loggable {
public:
  SEPatientActionCollection(SESubstanceManager&);
  ~SEPatientActionCollection();

  void Clear();

  void Unload(std::vector<CDM::ActionData*>& to);

  bool ProcessAction(const SEPatientAction& action);
  bool ProcessAction(const CDM::PatientActionData& action);

  bool HasAcuteStress() const;
  SEAcuteStress* GetAcuteStress() const;
  void RemoveAcuteStress();

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

  bool HasExercise() const;
  SEExercise* GetExercise() const;
  void RemoveExercise();

  bool HasHemorrhage() const;
  const std::map<std::string, SEHemorrhage*>& GetHemorrhages() const;
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

  bool HasNeedleDecompression() const;

  bool HasLeftNeedleDecompression() const;
  SENeedleDecompression* GetLeftNeedleDecompression() const;
  void RemoveLeftNeedleDecompression();

  bool HasRightNeedleDecompression() const;
  SENeedleDecompression* GetRightNeedleDecompression() const;
  void RemoveRightNeedleDecompression();

  bool HasPainStimulus() const;
  const std::map<std::string, SEPainStimulus*>& GetPainStimuli() const;
  void RemovePainStimulus(const char* loc);
  void RemovePainStimulus(const std::string& loc);

  bool HasPericardialEffusion() const;
  SEPericardialEffusion* GetPericardialEffusion() const;
  void RemovePericardialEffusion();

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
  void RemoveSubstanceBolus(const SESubstance& sub);

  const std::map<const SESubstance*, SESubstanceInfusion*>& GetSubstanceInfusions() const;
  void RemoveSubstanceInfusion(const SESubstance& sub);

  const std::map<const SESubstance*, SESubstanceOralDose*>& GetSubstanceOralDoses() const;
  void RemoveSubstanceOralDose(const SESubstance& sub);

  const std::map<const SESubstanceCompound*, SESubstanceCompoundInfusion*>& GetSubstanceCompoundInfusions() const;
  void RemoveSubstanceCompoundInfusion(const SESubstanceCompound& sub);

  bool HasTourniquet() const;
  const std::map<std::string, SETourniquet*>& GetTourniquets() const;
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

  SEAcuteStress* m_AcuteStress;
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
  SEExercise* m_Exercise;
  SEInfection* m_Infection;
  SEIntubation* m_Intubation;
  SEMechanicalVentilation* m_MechanicalVentilation;
  SENeedleDecompression* m_LeftNeedleDecompression;
  SENeedleDecompression* m_RightNeedleDecompression;
  SEPericardialEffusion* m_PericardialEffusion;
  SESleep* m_Sleep;
  SETensionPneumothorax* m_LeftClosedTensionPneumothorax;
  SETensionPneumothorax* m_LeftOpenTensionPneumothorax;
  SETensionPneumothorax* m_RightClosedTensionPneumothorax;
  SETensionPneumothorax* m_RightOpenTensionPneumothorax;
  SEUrinate* m_Urinate;
  SEOverride* m_OverrideAction;

  std::map<std::string, SEHemorrhage*> m_Hemorrhages;
  std::map<std::string, SETourniquet*> m_Tourniquets;
  std::map<std::string, SEPainStimulus*> m_PainStimuli;
  std::map<const SESubstance*, SESubstanceBolus*> m_SubstanceBolus;
  std::map<const SESubstance*, SESubstanceInfusion*> m_SubstanceInfusions;
  std::map<const SESubstance*, SESubstanceOralDose*> m_SubstanceOralDoses;
  std::map<const SESubstanceCompound*, SESubstanceCompoundInfusion*> m_SubstanceCompoundInfusions;

  bool AdministerSubstance(const CDM::SubstanceAdministrationData& subAdmin);

  SESubstanceManager& m_Substances;
  std::stringstream m_ss;
};
}