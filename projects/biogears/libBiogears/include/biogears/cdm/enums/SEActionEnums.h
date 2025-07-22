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

enum class SESerializationType {
  Invalid = -1,
  Load = 0,
  Save,
};


inline std::string ToString(const SESerializationType& bit)
{
  switch (bit) {
  case SESerializationType::Load:
    return "Load";
  case SESerializationType::Save:
    return "Save";
  default:
    return "Invalid";
  }
}


inline std::ostream& operator<<(std::ostream& os, const SESerializationType& pt)
{
  os << ToString(pt);
  return os;
}

}
