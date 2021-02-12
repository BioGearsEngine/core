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

enum class SEPatientAssessmentType {
  Invalid = -1,
  PulmonaryFunctionTest = 0,
  CompleteBloodCount,
  ComprehensiveMetabolicPanel,
  Urinalysis
};
enum class SEBrainInjuryType {
  Invalid = -1,
  Diffuse = 0,
  LeftFocal,
  RightFocal
};
enum class SEIntubationType {
  Invalid = -1,
  Off = 0,
  Esophageal,
  LeftMainstem,
  RightMainstem,
  Tracheal
};
enum class SEPneumothoraxType {
  Invalid = -1,
  Open = 0,
  Closed
};

enum class SEBolusAdministration {
  Invalid = -1,
  Intraarterial = 0,
  Intramuscular,
  Intravenous
};
enum class SEInfectionSeverity {
  Eliminated,
  Mild,
  Moderate,
  Severe
};

inline std::ostream& operator<<(std::ostream& os, const SEPatientAssessmentType& pa)
{
  switch (pa) {
  case SEPatientAssessmentType::PulmonaryFunctionTest:
    os << "PulmonaryFunctionTest";
    break;
  case SEPatientAssessmentType::CompleteBloodCount:
    os << "CompleteBloodCount";
    break;
  case SEPatientAssessmentType::ComprehensiveMetabolicPanel:
    os << "ComprehensiveMetabolicPanel";
    break;
  case SEPatientAssessmentType::Urinalysis:
    os << "Urinalysis";
    break;
  default:
    os << "Invalid";
  }
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SEBrainInjuryType& bit)
{
  switch (bit) {
  case SEBrainInjuryType::Diffuse:
    os << "Diffuse";
    break;
  case SEBrainInjuryType::LeftFocal:
    os << "LeftFocal";
    break;
  case SEBrainInjuryType::RightFocal:
    os << "RightFocal";
    break;
  default:
    os << "Invalid";
  }
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SEIntubationType& it)
{
  switch (it) {
  case SEIntubationType::Off:
    os << "Off";
    break;
  case SEIntubationType::Esophageal:
    os << "Esophageal";
    break;
  case SEIntubationType::LeftMainstem:
    os << "LeftMainstem";
    break;
  case SEIntubationType::RightMainstem:
    os << "RightMainstem";
    break;
  case SEIntubationType::Tracheal:
    os << "Tracheal";
    break;
  default:
    os << "Invalid";
  }
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SEPneumothoraxType& pt)
{
  switch (pt) {
  case SEPneumothoraxType::Open:
    os << "Open";
    break;
  case SEPneumothoraxType::Closed:
    os << "Closed";
    break;
  default:
    os << "Invalid";
  }
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SEBolusAdministration& ba)
{
  switch (ba) {
  case SEBolusAdministration::Intraarterial:
    os << "Intraarterial";
    break;
  case SEBolusAdministration::Intramuscular:
    os << "Intramuscular";
    break;
  case SEBolusAdministration::Intravenous:
    os << "Intravenous";
    break;
  default:
    os << "Invalid";
  }
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const SEInfectionSeverity& ba)
{
  switch (ba) {
  case SEInfectionSeverity::Eliminated:
    os << "Eliminated";
    break;
  case SEInfectionSeverity::Mild:
    os << "Mild";
    break;
  case SEInfectionSeverity::Moderate:
    os << "Moderate";
    break;
  case SEInfectionSeverity::Severe:
    os << "Severe";
    break;
  default:
    os << "Invalid";
  }
  return os;
}

}
