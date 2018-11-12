//-------------------------------------------------------------------------------------------
//- Copyright 2017 Applied Research Associates, Inc.
//- Licensed under the Apache License, Version 2.0 (the "License"); you may not use
//- this file except in compliance with the License. You may obtain a copy of the License
//- at:
//- http://www.apache.org/licenses/LICENSE-2.0
//- Unless required by applicable law or agreed to in writing, software distributed under
//- the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//- CONDITIONS OF ANY KIND, either express or implied. See the License for the
//-  specific language governing permissions and limitations under the License.
//-------------------------------------------------------------------------------------------

//!
//! \author Alex Pronschinske
//! \date 2017 Mar 9
//!

#include <util/string_manipulation.h>

#include <algorithm>

using std::vector;
using std::string;

namespace ara {

  //----------------------------------------------------------------------------
  vector<string> string_split(const string& src, const string& delimiter) {
    vector<string> pieces;
    size_t i = 0;
    while (i < src.size()) {
      const auto n = std::min(src.size(), src.find(delimiter, i) - i);
      pieces.emplace_back(src, i, n);
      i += n + 1;
    }
    return pieces;
  }

  //----------------------------------------------------------------------------
  double parse_double_or(double alt, const std::string& txt) {
    double x = alt;
    try {
      x = std::stod(txt);
    } catch (...) {}
    return x;
  }

  //----------------------------------------------------------------------------
  float parse_float_or(float alt, const std::string& txt) {
    float x = alt;
    try {
      x = std::stof(txt);
    } catch (...) {}
    return x;
  }

  //----------------------------------------------------------------------------
  int parse_int_or(int alt, const std::string& txt) {
    int x = alt;
    try {
      x = std::stoi(txt);
    } catch (...) {}
    return x;
  }

} // end namespace ara
