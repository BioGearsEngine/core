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
enum class SESide {
  Invalid = -1,
  Left = 0,
  Right
};

enum class SEOpenClosed {
  Invalid = -1,
  Closed = 0,
  Open
};
enum class SEOnOff {
  Invalid = -1,
  Off = 0,
  On
};
enum class SEErrorType {
  Invalid = -1,
  Standard = 0
};
inline std::ostream& operator<<(std::ostream& os, const SESide& e)
{
  switch (e) {
  case SESide::Left:
    os << "Left";
    break;
  case SESide::Right:
    os << "Right";
    break;
  default:
    os << "Invalid";
  }
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SEOpenClosed& e)
{
  switch (e) {
  case SEOpenClosed::Closed:
    os << "Closed";
    break;
  case SEOpenClosed::Open:
    os << "Open";
    break;
  default:
    os << "Invalid";
  }
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SEOnOff& e)
{
  switch (e) {
  case SEOnOff::Off:
    os << "Off";
    break;
  case SEOnOff::On:
    os << "On";
    break;
  default:
    os << "Invalid";
  }
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SEErrorType& e)
{
  switch (e) {
  case SEErrorType::Standard:
    os << "Standard";
    break;
  default:
    os << "Invalid";
  }
  return os;
}
}