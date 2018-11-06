#ifndef BIOGEARS_STRING_MAINPULATION_H
#define BIOGEARS_STRING_MAINPULATION_H

//--NGSS PREAMBLE--//

//!
//! \author Alex Pronschinske
//! \date 2017 Mar 9
//!

#include <vector>
#include <string>
#include <sstream>

namespace biogears {

  //----------------------------------------------------------------------------
  //!
  //! "A"llocated sprintf. Creates an exactly sized formatted string
  //!
  template <typename... Args>
  std::string asprintf(const char* format_str, Args&&... args)  {
    auto n = snprintf(nullptr, 0, format_str, std::forward<Args>(args)...);
    if (n <= 0) {
      return std::string();
    }
    else {
      auto out_str = std::string(n, '\0');
      snprintf(&out_str[0], out_str.size() + 1, format_str, std::forward<Args>(args)...);
      return out_str;
    }
  }

  //----------------------------------------------------------------------------
  //!
  //! "A"llocated sprintf. Creates an exactly sized formatted string
  //!
  inline std::string asprintf(const char* str) { return std::string(str); }

  //----------------------------------------------------------------------------
  //!
  //! Creates a vector of strings by splitting `src` at every occurrence of
  //! `delimiter`
  //!
  std::vector<std::string> string_split(const std::string& src,
                                        const std::string& delimiter);

  //----------------------------------------------------------------------------
  double parse_double_or(double alt, const std::string& txt);
  float parse_float_or(float alt, const std::string& txt);
  int parse_int_or(int alt, const std::string& txt);

  //----------------------------------------------------------------------------
  template<typename T>
  inline std::string to_string_or(const std::string& alt, T value) {
    std::string out = alt;
    try {
		  std::stringstream ss;
			ss << value;
      out = ss.str();
    } catch (...) {}
    return out;
  }

} // end namespace biogears

#endif // BIOGEARS_STRING_MAINPULATION_H
