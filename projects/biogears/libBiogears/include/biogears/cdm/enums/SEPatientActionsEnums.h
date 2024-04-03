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

enum class SEBrainInjuryType {
  Invalid = -1,
  Diffuse = 0,
  LeftFocal,
  RightFocal
};

enum class SEBolusAdministration {
  Invalid = -1,
  Intraarterial = 0,
  Intramuscular,
  Intravenous
};

enum class SEBurnDegree {
  Invalid = -1,
  First = 0,
  Second,
  Third
};

enum class SEInfectionSeverity {
  Invalid = -1,
  Eliminated = 0,
  Mild,
  Moderate,
  Severe,

};

enum class SEIntubationType {
  Invalid = -1,
  Off = 0,
  Esophageal,
  LeftMainstem,
  RightMainstem,
  Tracheal
};

enum class SEPatientAssessmentType {
  Invalid = -1,
  ArterialBloodGasAnalysis = 0,
  PulmonaryFunctionTest,
  ProthrombinTime,
  PsychomotorVigilanceTask,
  CompleteBloodCount,
  ComprehensiveMetabolicPanel,
  SequentialOrganFailureAssessment,
  Urinalysis
};

enum class SEPneumothoraxType {
  Invalid = -1,
  Open = 0,
  Closed
};

enum class SEOralAdministrationType {
  Invalid = -1,
  Transmucosal = 0,
  Gastrointestinal
};

enum class SETourniquetApplicationType {
  Invalid = -1,
  Applied = 0,
  Misapplied,
  NotApplied
};

inline std::string ToString(const SEBrainInjuryType& bit)
{
  switch (bit) {
  case SEBrainInjuryType::Diffuse:
    return "Diffuse";
  case SEBrainInjuryType::LeftFocal:
    return "LeftFocal";
  case SEBrainInjuryType::RightFocal:
    return "RightFocal";
  default:
    return "Invalid";
  }
}
inline std::string ToString(const SEBolusAdministration& ba)
{
  switch (ba) {
  case SEBolusAdministration::Intraarterial:
    return "Intraarterial";
  case SEBolusAdministration::Intramuscular:
    return "Intramuscular";
  case SEBolusAdministration::Intravenous:
    return "Intravenous";
  default:
    return "Invalid";
  }
}

inline std::string ToString(const SEBurnDegree& bd)
{
  switch (bd) {

  case SEBurnDegree ::First:
    return "First";
  case SEBurnDegree::Second:
    return "Second";
  case SEBurnDegree::Third:
    return "Third";
  default:
    return "Invalid";
  }
}

inline std::string ToString(const SEInfectionSeverity& ba)
{
  switch (ba) {
  case SEInfectionSeverity::Eliminated:
    return "Eliminated";
  case SEInfectionSeverity::Mild:
    return "Mild";
  case SEInfectionSeverity::Moderate:
    return "Moderate";
  case SEInfectionSeverity::Severe:
    return "Severe";
  default:
    return "Invalid";
  }
}
inline std::string ToString(const SEIntubationType& it)
{
  switch (it) {
  case SEIntubationType::Off:
    return "Off";
  case SEIntubationType::Esophageal:
    return "Esophageal";
  case SEIntubationType::LeftMainstem:
    return "LeftMainstem";
  case SEIntubationType::RightMainstem:
    return "RightMainstem";
  case SEIntubationType::Tracheal:
    return "Tracheal";
  default:
    return "Invalid";
  }
}
inline std::string ToString(const SEPatientAssessmentType& pa)
{
  switch (pa) {
  case SEPatientAssessmentType::ArterialBloodGasAnalysis:
    return "ArterialBloodGasAnalysis";
  case SEPatientAssessmentType::PulmonaryFunctionTest:
    return "PulmonaryFunctionTest";
  case SEPatientAssessmentType::ProthrombinTime:
    return "ProthrombinTime";
  case SEPatientAssessmentType::PsychomotorVigilanceTask:
    return "PsychomotorVigilanceTask";
  case SEPatientAssessmentType::CompleteBloodCount:
    return "CompleteBloodCount";
  case SEPatientAssessmentType::ComprehensiveMetabolicPanel:
    return "ComprehensiveMetabolicPanel";
  case SEPatientAssessmentType::SequentialOrganFailureAssessment:
    return "SequentialOrganFailureAssessment";
  case SEPatientAssessmentType::Urinalysis:
    return "Urinalysis";
  default:
    return "Invalid";
  }
}
inline std::string ToString(const SEPneumothoraxType& pt)
{
  switch (pt) {
  case SEPneumothoraxType::Open:
    return "Open";
  case SEPneumothoraxType::Closed:
    return "Closed";
  default:
    return "Invalid";
  }
}
inline std::string ToString(const SEOralAdministrationType& pt)
{
  switch (pt) {
  case SEOralAdministrationType::Transmucosal:
    return "Transmucosal";
  case SEOralAdministrationType::Gastrointestinal:
    return "Gastrointestinal";
  default:
    return "Invalid";
  }
}
inline std::string ToString(const SETourniquetApplicationType& pt)
{
  switch (pt) {
  case SETourniquetApplicationType::Applied:
    return "Applied";
  case SETourniquetApplicationType::Misapplied:
    return "Misapplied";
  case SETourniquetApplicationType::NotApplied:
    return "NotApplied";
  default:
    return "Invalid";
  }
}

inline std::ostream& operator<<(std::ostream& os, const SEBrainInjuryType& bit)
{
  os << ToString(bit);
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SEBolusAdministration& ba)
{
  os << ToString(ba);
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SEBurnDegree& bd)
{
  os << ToString(bd);
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SEInfectionSeverity& is)
{
  os << ToString(is);
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SEIntubationType& it)
{
  os << ToString(it);
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SEPatientAssessmentType& pa)
{
  os << ToString(pa);
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SEPneumothoraxType& pt)
{
  os << ToString(pt);
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SEOralAdministrationType& pt)
{
  os << ToString(pt);
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SETourniquetApplicationType& pt)
{
  os << ToString(pt);
  return os;
}
}
