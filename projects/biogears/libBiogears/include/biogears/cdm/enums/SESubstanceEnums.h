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

enum class SESubstanceState {
  Invalid = -1,
  Solid = 0,
  Liquid,
  Gas,
  Molecular
};

enum class SESubstanceIonicState {
  Invalid = -1,
  WeakBase = 0,
  Base,
  Acid,
  Neutral,
  Zwitterion
};

enum class SESubstanceBindingProtein {
  Invalid = -1,
  Albumin = 0,
  Lipoprotein,
  AAG,
};

enum class SECharge {
  Invalid = -1,
  Positive = 0,
  Neutral,
  Negative,

};

enum class SESubstanceClass {
  Invalid = -1,
  Anesthetic = 0,
  Antibiotic,
  WholeBlood,
  Ion,
  Opioid,
  ReversalAgent,
  Sedative
};

inline std::string ToString(const SESubstanceState& bit)
{
  switch (bit) {
  case SESubstanceState::Solid:
    return "Solid";
  case SESubstanceState::Liquid:
    return "Liquid";
  case SESubstanceState::Gas:
    return "Gas";
  case SESubstanceState::Molecular:
    return "Molecular";
  default:
    return "Invalid";
  }
}

inline std::string ToString(const SESubstanceIonicState& bit)
{
  switch (bit) {
  case SESubstanceIonicState::WeakBase:
    return "WeakBase";
  case SESubstanceIonicState::Base:
    return "Base";
  case SESubstanceIonicState::Acid:
    return "Acid";
  case SESubstanceIonicState::Neutral:
    return "Neutral";
  case SESubstanceIonicState::Zwitterion:
    return "Zwitterion";
  default:
    return "Invalid";
  }
}

inline std::string ToString(const SESubstanceBindingProtein& bit)
{
  switch (bit) {
  case SESubstanceBindingProtein::Albumin:
    return "Albumin";
  case SESubstanceBindingProtein::Lipoprotein:
    return "Lipoprotein";
  case SESubstanceBindingProtein::AAG:
    return "AAG";
  default:
    return "Invalid";
  }
}

inline std::string ToString(const SECharge& bit)
{
  switch (bit) {
  case SECharge::Positive:
    return "Positive";
  case SECharge::Neutral:
    return "Neutral";
  case SECharge::Negative:
    return "Negative";
  default:
    return "Invalid";
  }
}

inline std::string ToString(const SESubstanceClass& bit)
{
  switch (bit) {
  case SESubstanceClass::Anesthetic:
    return "Anesthetic";
  case SESubstanceClass::Antibiotic:
    return "Antibiotic";
  case SESubstanceClass::WholeBlood:
    return "WholeBlood";
  case SESubstanceClass::Ion:
    return "Ion";
  case SESubstanceClass::Opioid:
    return "Opioid";
  case SESubstanceClass::ReversalAgent:
    return "ReversalAgent";
  case SESubstanceClass::Sedative:
    return "Sedative";
  default:
    return "Invalid";
  }
}

inline std::ostream& operator<<(std::ostream& os, const SESubstanceState& pt)
{
  os << ToString(pt);
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const SESubstanceIonicState& pt)
{
  os << ToString(pt);
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const SESubstanceBindingProtein& pt)
{
  os << ToString(pt);
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const SECharge& pt)
{
  os << ToString(pt);
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const SESubstanceClass& pt)
{
  os << ToString(pt);
  return os;
}

}
