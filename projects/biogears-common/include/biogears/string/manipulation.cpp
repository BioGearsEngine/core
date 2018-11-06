//--NGSS PREAMBLE--//

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
