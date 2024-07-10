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

enum class SECircuitType {
  Invalid = -1,
  Electrical= 0,
  Fluid,
  Thermal,
};

enum class SEResistancePathType {
  Invalid = -1,
  Cerebral = 0,
  Extrasplanchnic,
  Muscle,
  Splanchnic,
  Myocardium
};


inline std::string ToString(const SECircuitType& bit)
{
  switch (bit) {
  case SECircuitType::Electrical:
    return "Electrical";
  case SECircuitType::Fluid:
    return "Fluid";
  case SECircuitType::Thermal:
    return "Thermal";
  default:
    return "Invalid";
  }
}

inline std::string ToString(const SEResistancePathType& bit)
{
  switch (bit) {
  case SEResistancePathType::Cerebral:
    return "Cerebral";
  case SEResistancePathType::Extrasplanchnic:
    return "Extrasplanchnic";
  case SEResistancePathType::Muscle:
    return "Muscle";
  case SEResistancePathType::Splanchnic:
    return "Splanchnic";
  case SEResistancePathType::Myocardium:
    return "Myocardium";
  default:
    return "Invalid";
  }
}

inline std::ostream& operator<<(std::ostream& os, const SECircuitType& pt)
{
  os << ToString(pt);
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const SEResistancePathType& pt)
{
  os << ToString(pt);
  return os;
}

}
