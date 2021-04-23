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

inline std::string ToString(const SESide& e)
{
  switch (e) {
  case SESide::Left:
    return "Left";
  case SESide::Right:
    return "Right";
  default:
    return "Invalid";
  }
}
inline std::string ToString(const SEOpenClosed& e)
{
  switch (e) {
  case SEOpenClosed::Closed:
    return "Closed";
  case SEOpenClosed::Open:
    return "Open";
  default:
    return "Invalid";
  }
}
inline std::string ToString(const SEOnOff& e)
{
  switch (e) {
  case SEOnOff::Off:
    return "Off";
  case SEOnOff::On:
    return "On";
  default:
    return "Invalid";
  }
}
inline std::string ToString(const SEErrorType& e)
{
  switch (e) {
  case SEErrorType::Standard:
    return "Standard";

  default:
    return "Invalid";
  }
}

inline std::ostream& operator<<(std::ostream& os, const SESide& e)
{
  os << ToString(e);
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SEOpenClosed& e)
{
  os << ToString(e);
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SEOnOff& e)
{
  os << ToString(e);
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const SEErrorType& e)
{
  os << ToString(e);
  return os;
}
}