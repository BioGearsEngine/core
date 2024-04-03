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

#include <ostream>

namespace biogears {
enum class SESex {
  Invalid = -1,
  Male = 0,
  Female,

};

enum class SEBloodType {
  Invalid = -1,
  A = 0,
  B,
  AB,
  O
};
enum  class SEPatientEventType {
  Invalid = -1,
  AcuteLungInjury = 0,
  AcuteRespiratoryDistress,
  Antidiuresis,
  Asystole,
  Bradycardia,
  Bradypnea,
  BrainOxygenDeficit,
  CardiacArrest,
  CardiogenicShock,
  CompartmentSyndromeLeftArm,
  CompartmentSyndromeRightArm,
  CompartmentSyndromeLeftLeg,
  CompartmentSyndromeRightLeg,
  CompartmentSyndromeAbdominal,
  CriticalBrainOxygenDeficit,
  Dehydration,
  Diuresis,
  MildDiarrhea,
  SevereDiarrhea,
  Fasciculation,
  Fatigue,
  FlaccidParalysis,
  FunctionalIncontinence,
  MildHeadache,
  SevereHeadache,
  HemolyticTransfusionReaction,
  Hypercapnia,
  Hyperglycemia,
  MildDiaphoresis,
  ModerateDiaphoresis,
  SevereDiaphoresis,
  MildHyperkalemia,
  SevereHyperkalemia,
  MildHypernatremia,
  SevereHypernatremia,
  Hyperthermia,
  Hypoglycemia,
  HypoglycemicShock,
  HypoglycemicComa,
  MildHypothermia,
  ModerateHypothermia,
  SevereHypothermia,
  MildHypokalemia,
  SevereHypokalemia,
  MildHyponatremia,
  SevereHyponatremia,
  Hypoxia,
  HypovolemicShock,
  IntracranialHypertension,
  IntracranialHypotension,
  IrreversibleState,
  Ketoacidosis,
  LacticAcidosis,
  LiverGlycogenDepleted,
  MaximumPulmonaryVentilationRate,
  MetabolicAcidosis,
  MetabolicAlkalosis,
  MildWeakness,
  ModerateWeakness,
  MildSecretions,
  ModerateSecretions,
  MuscleCatabolism,
  MuscleGlycogenDepleted,
  MyocardiumOxygenDeficit,
  Natriuresis,
  Nausea,
  NutritionDepleted,
  PulselessRhythm,
  RenalHypoperfusion,
  RespiratoryAcidosis,
  RespiratoryAlkalosis,
  SevereAcuteRespiratoryDistress,
  SevereSecretions,
  Seizures,
  Shivering,
  StartOfCardiacCycle,
  StartOfExhale,
  StartOfInhale,
  SevereSepsis,
  Tachycardia,
  Tachypnea,
  Vomiting,
  _TotalPatientEvents,
  // helpers for iterating over the enum
  _begin = 0,
  _end = _TotalPatientEvents
};

inline std::string ToString(const SESex& e)
{
  switch (e) {
  case SESex::Male:
    return "Male";
  case SESex::Female:
    return "Female";
  default:
    return "Invalid";
  }
}
inline std::string ToString(const SEBloodType& e)
{
  switch (e) {
  case SEBloodType::A:
    return "A";
  case SEBloodType::B:
    return "B";
  case SEBloodType::AB:
    return "AB";
  case SEBloodType::O:
    return "O";
  default:
    return "Invalid";
  }
}
inline std::string ToString(const SEPatientEventType& e)
{
  switch (e) {
  case SEPatientEventType::AcuteLungInjury:
    return "AcuteLungInjury";
  case SEPatientEventType::AcuteRespiratoryDistress:
    return "AcuteRespiratoryDistress";
  case SEPatientEventType::Antidiuresis:
    return "Antidiuresis";
  case SEPatientEventType::Asystole:
    return "Asystole";
  case SEPatientEventType::Bradycardia:
    return "Bradycardia";
  case SEPatientEventType::Bradypnea:
    return "Bradypnea";
  case SEPatientEventType::BrainOxygenDeficit:
    return "BrainOxygenDeficit";
  case SEPatientEventType::CardiacArrest:
    return "CardiacArrest";
  case SEPatientEventType::CardiogenicShock:
    return "CardiogenicShock";
  case SEPatientEventType::CompartmentSyndromeLeftArm:
    return "CompartmentSyndromeLeftArm";
  case SEPatientEventType::CompartmentSyndromeRightArm:
    return "CompartmentSyndromeRightArm";
  case SEPatientEventType::CompartmentSyndromeLeftLeg:
    return "CompartmentSyndromeLeftLeg";
  case SEPatientEventType::CompartmentSyndromeRightLeg:
    return "CompartmentSyndromeRightLeg";
  case SEPatientEventType::CompartmentSyndromeAbdominal:
    return "CompartmentSyndromeAbdominal";
  case SEPatientEventType::CriticalBrainOxygenDeficit:
    return "CriticalBrainOxygenDeficit";
  case SEPatientEventType::Dehydration:
    return "Dehydration";
  case SEPatientEventType::Diuresis:
    return "Diuresis";
  case SEPatientEventType::MildDiarrhea:
    return "MildDiarrhea";
  case SEPatientEventType::SevereDiarrhea:
    return "SevereDiarrhea";
  case SEPatientEventType::Fasciculation:
    return "Fasciculation";
  case SEPatientEventType::Fatigue:
    return "Fatigue";
  case SEPatientEventType::FlaccidParalysis:
    return "FlaccidParalysis";
  case SEPatientEventType::FunctionalIncontinence:
    return "FunctionalIncontinence";
  case SEPatientEventType::MildHeadache:
    return "MildHeadache";
  case SEPatientEventType::SevereHeadache:
    return "SevereHeadache";
  case SEPatientEventType::HemolyticTransfusionReaction:
    return "HemolyticTransfusionReaction";
  case SEPatientEventType::Hypercapnia:
    return "Hypercapnia";
  case SEPatientEventType::Hyperglycemia:
    return "Hyperglycemia";
  case SEPatientEventType::MildDiaphoresis:
    return "MildDiaphoresis";
  case SEPatientEventType::ModerateDiaphoresis:
    return "ModerateDiaphoresis";
  case SEPatientEventType::SevereDiaphoresis:
    return "SevereDiaphoresis";
  case SEPatientEventType::MildHyperkalemia:
    return "MildHyperkalemia";
  case SEPatientEventType::SevereHyperkalemia:
    return "SevereHyperkalemia";
  case SEPatientEventType::MildHypernatremia:
    return "MildHypernatremia";
  case SEPatientEventType::SevereHypernatremia:
    return "SevereHypernatremia";
  case SEPatientEventType::Hyperthermia:
    return "Hyperthermia";
  case SEPatientEventType::Hypoglycemia:
    return "Hypoglycemia";
  case SEPatientEventType::HypoglycemicShock:
    return "HypoglycemicShock";
  case SEPatientEventType::HypoglycemicComa:
    return "HypoglycemicComa";
  case SEPatientEventType::MildHypothermia:
    return "MildHypothermia";
  case SEPatientEventType::ModerateHypothermia:
    return "ModerateHypothermia";
  case SEPatientEventType::SevereHypothermia:
    return "SevereHypothermia";
  case SEPatientEventType::MildHypokalemia:
    return "MildHypokalemia";
  case SEPatientEventType::SevereHypokalemia:
    return "SevereHypokalemia";
  case SEPatientEventType::MildHyponatremia:
    return "MildHyponatremia";
  case SEPatientEventType::SevereHyponatremia:
    return "SevereHyponatremia";
  case SEPatientEventType::Hypoxia:
    return "Hypoxia";
  case SEPatientEventType::HypovolemicShock:
    return "HypovolemicShock";
  case SEPatientEventType::IntracranialHypertension:
    return "IntracranialHypertension";
  case SEPatientEventType::IntracranialHypotension:
    return "IntracranialHypotension";
  case SEPatientEventType::IrreversibleState:
    return "IrreversibleState";
  case SEPatientEventType::Ketoacidosis:
    return "Ketoacidosis";
  case SEPatientEventType::LacticAcidosis:
    return "LacticAcidosis";
  case SEPatientEventType::LiverGlycogenDepleted:
    return "LiverGlycogenDepleted";
  case SEPatientEventType::MaximumPulmonaryVentilationRate:
    return "MaximumPulmonaryVentilationRate";
  case SEPatientEventType::MetabolicAcidosis:
    return "MetabolicAcidosis";
  case SEPatientEventType::MetabolicAlkalosis:
    return "MetabolicAlkalosis";
  case SEPatientEventType::MildWeakness:
    return "MildWeakness";
  case SEPatientEventType::ModerateWeakness:
    return "ModerateWeakness";
  case SEPatientEventType::MildSecretions:
    return "MildSecretions";
  case SEPatientEventType::ModerateSecretions:
    return "ModerateSecretions";
  case SEPatientEventType::MuscleCatabolism:
    return "MuscleCatabolism";
  case SEPatientEventType::MuscleGlycogenDepleted:
    return "MuscleGlycogenDepleted";
  case SEPatientEventType::MyocardiumOxygenDeficit:
    return "MyocardiumOxygenDeficit";
  case SEPatientEventType::Natriuresis:
    return "Natriuresis";
  case SEPatientEventType::Nausea:
    return "Nausea";
  case SEPatientEventType::NutritionDepleted:
    return "NutritionDepleted";
  case SEPatientEventType::PulselessRhythm:
    return "PulselessRhythm";
  case SEPatientEventType::RenalHypoperfusion:
    return "RenalHypoperfusion";
  case SEPatientEventType::RespiratoryAcidosis:
    return "RespiratoryAcidosis";
  case SEPatientEventType::RespiratoryAlkalosis:
    return "RespiratoryAlkalosis";
  case SEPatientEventType::SevereAcuteRespiratoryDistress:
    return "SevereAcuteRespiratoryDistress";
  case SEPatientEventType::SevereSecretions:
    return "SevereSecretions";
  case SEPatientEventType::Seizures:
    return "Seizures";
  case SEPatientEventType::Shivering:
    return "Shivering";
  case SEPatientEventType::StartOfCardiacCycle:
    return "StartOfCardiacCycle";
  case SEPatientEventType::StartOfExhale:
    return "StartOfExhale";
  case SEPatientEventType::StartOfInhale:
    return "StartOfInhale";
  case SEPatientEventType::SevereSepsis:
    return "SevereSepsis";
  case SEPatientEventType::Tachycardia:
    return "Tachycardia";
  case SEPatientEventType::Tachypnea:
    return "Tachypnea";
  case SEPatientEventType::Vomiting:
    return "Vomiting";
  default:
    return "Invalid";
  }
}

inline std::ostream& operator<<(std::ostream& os, const SESex& e)
{
  os << ToString(e);
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SEBloodType& e)
{
  os << ToString(e);
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SEPatientEventType& e)
{
  os << ToString(e);
  return os;
}

}