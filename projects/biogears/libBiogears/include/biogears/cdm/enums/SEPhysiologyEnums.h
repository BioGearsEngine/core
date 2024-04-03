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
enum class SEInflammationSource {
  Invalid = -1,
  Burn = 0,
  Hemorrhage,
  Infection,
  Ebola
};

enum class SEHeartRhythm {
  Invalid = -1,
  Asystole = 0,
  NormalSinus
};
enum class SESleepState {
  Invalid = -1,
  Awake = 0,
  Sleeping
};

inline std::string ToString(const SEInflammationSource& e)
{
  switch (e) {
  case SEInflammationSource::Burn:
    return "Burn";
  case SEInflammationSource::Hemorrhage:
    return "Hemorrhage";
  case SEInflammationSource::Infection:
    return "Infection";
  case SEInflammationSource::Ebola:
    return "Ebola";
  default:
    return "Invalid";
  }
}
inline std::string ToString(const SEHeartRhythm& e)
{
  switch (e) {
  case SEHeartRhythm::Asystole:
    return "Asystole";
  case SEHeartRhythm::NormalSinus:
    return "NormalSinus";
  default:
    return "Invalid";
  }
}
inline std::string ToString(const SESleepState& e)
{
  switch (e) {
  case SESleepState::Awake:
    return "Awake";
  case SESleepState::Sleeping:
    return "Sleep";
  default:
    return "Invalid";
  }
}


inline std::ostream& operator<<(std::ostream& os, const SEInflammationSource& e)
{
  os << ToString(e);
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SEHeartRhythm& e)
{
  os << ToString(e);
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SESleepState& e)
{
  os << ToString(e);
  return os;
}

}