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
enum class SEPresenceIndicator {
  Invalid = -1,
  Positive = 0,
  Negative
};

enum class SEClarityIndicator {
  Invalid = -1,
  Clear = 0,
  SlightlyCloudy,
  Cloudy,
  Turbid
};

enum class SEUrineColor {
  Invalid = -1,
  PaleYellow = 0,
  Yellow,
  DarkYellow,
  Pink
};

enum class SEMicroscopicObservationType {
  Invalid = -1,
  LowPowerField = 0,
  HighPowerField
};

enum class SEMicroscopicObservationAmount {
  Invalid = -1,
  Few = 0,
  Moderate,
  Many
};

inline std::string ToString(const SEPresenceIndicator& e)
{
  switch (e) {
  case SEPresenceIndicator::Positive:
    return "Positive";
  case SEPresenceIndicator::Negative:
    return "Negative";
  default:
    return "Invalid";
  }
}
inline std::string ToString(const SEClarityIndicator& e)
{
  switch (e) {
  case SEClarityIndicator::Clear:
    return "Clear";
  case SEClarityIndicator::SlightlyCloudy:
    return "SlightlyCloudy";
  case SEClarityIndicator::Cloudy:
    return "Cloudy";
  case SEClarityIndicator::Turbid:
    return "Turbid";
  default:
    return "Invalid";
  }
}
inline std::string ToString(const SEUrineColor& e)
{
  switch (e) {
  case SEUrineColor::PaleYellow:
    return "PaleYellow";
  case SEUrineColor::Yellow:
    return "Yellow";
  case SEUrineColor::DarkYellow:
    return "DarkYellow";
  case SEUrineColor::Pink:
    return "Pink";
  default:
    return "Invalid";
  }
}
inline std::string ToString(const SEMicroscopicObservationType& e)
{
  switch (e) {
  case SEMicroscopicObservationType::LowPowerField:
    return "LowPowerField";
  case SEMicroscopicObservationType::HighPowerField:
    return "HighPowerField";
  default:
    return "Invalid";
  }
}
inline std::string ToString(const SEMicroscopicObservationAmount& e)
{
  switch (e) {
  case SEMicroscopicObservationAmount::Few:
    return "Few";
  case SEMicroscopicObservationAmount::Moderate:
    return "Moderate";
  case SEMicroscopicObservationAmount::Many:
    return "Many";
  default:
    return "Invalid";
  }
}

inline std::ostream& operator<<(std::ostream& os, const SEPresenceIndicator& e)
{
  os << ToString(e);
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SEClarityIndicator& e)
{
  os << ToString(e);
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SEUrineColor& e)
{
  os << ToString(e);
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SEMicroscopicObservationType& e)
{
  os << ToString(e);
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SEMicroscopicObservationAmount& e)
{
  os << ToString(e);
  return os;
}
}