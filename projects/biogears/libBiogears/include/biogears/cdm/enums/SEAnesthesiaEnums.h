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

enum class SEAnesthesiaMachineEvent {
  Invalid = -1,
  OxygenBottle1Exhausted = 0,
  OxygenBottle2Exhausted,
  ReliefValveActive,
};

enum class SEAnesthesiaMachineOxygenSource {
  Invalid = -1,
  Wall = 0,
  BottleOne,
  BottleTwo,
};

enum class SEAnesthesiaMachinePrimaryGas {
  Invalid = -1,
  Air = 0,
  Nitrogen,
};

enum class SEAnesthesiaMachineConnection {
  Invalid = -1,
  Off = 0,
  Mask,
  Tube,
};

inline std::string ToString(const SEAnesthesiaMachineEvent& bit)
{
  switch (bit) {
  case SEAnesthesiaMachineEvent::OxygenBottle1Exhausted:
    return "OxygenBottle1Exhausted";
  case SEAnesthesiaMachineEvent::OxygenBottle2Exhausted:
    return "OxygenBottle2Exhausted";
  case SEAnesthesiaMachineEvent::ReliefValveActive:
    return "ReliefValveActive";
  default:
    return "Invalid";
  }
}

inline std::string ToString(const SEAnesthesiaMachineOxygenSource& bit)
{
  switch (bit) {
  case SEAnesthesiaMachineOxygenSource::Wall:
    return "Wall";
  case SEAnesthesiaMachineOxygenSource::BottleOne:
    return "BottleOne";
  case SEAnesthesiaMachineOxygenSource::BottleTwo:
    return "BottleTwo";
  default:
    return "Invalid";
  }
}

inline std::string ToString(const SEAnesthesiaMachinePrimaryGas& bit)
{
  switch (bit) {
  case SEAnesthesiaMachinePrimaryGas::Air:
    return "Air";
  case SEAnesthesiaMachinePrimaryGas::Nitrogen:
    return "Nitrogen";
  default:
    return "Invalid";
  }
}

inline std::string ToString(const SEAnesthesiaMachineConnection& bit)
{
  switch (bit) {
  case SEAnesthesiaMachineConnection::Off:
    return "Off";
  case SEAnesthesiaMachineConnection::Mask:
    return "Mask";
  case SEAnesthesiaMachineConnection::Tube:
    return "Tube";
  default:
    return "Invalid";
  }
}

inline std::ostream& operator<<(std::ostream& os, const SEAnesthesiaMachineEvent& pt)
{
  os << ToString(pt);
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const SEAnesthesiaMachineOxygenSource& pt)
{
  os << ToString(pt);
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const SEAnesthesiaMachinePrimaryGas& pt)
{
  os << ToString(pt);
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const SEAnesthesiaMachineConnection& pt)
{
  os << ToString(pt);
  return os;
}

}
