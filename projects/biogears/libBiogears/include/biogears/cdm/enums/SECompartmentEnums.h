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
enum class SECompartmentType {
  Invalid = -1,
  Electrical = 0,
  Gas,
  Liquid,
  Thermal,
  Tissue,
};

inline std::string ToString(const SECompartmentType& e)
{
  switch (e) {
  case SECompartmentType::Electrical:
    return "Electrical";
  case SECompartmentType::Gas:
    return "Gas";
  case SECompartmentType::Liquid:
    return "Liquid";
  case SECompartmentType::Thermal:
    return "Thermal";
  case SECompartmentType::Tissue:
    return "Tissue";

  default:
    return "Invalid";
  }
}

inline std::ostream& operator<<(std::ostream& os, const SECompartmentType& e)
{
  os << ToString(e);
  return os;
}

}