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

#include <biogears/string/manipulation.h>

#include <algorithm>
#include <regex>
#include <sstream>

#ifdef ANDROID
#include <cstdlib>
namespace std {
static double stod(const std::string& value, std::size_t* pos = 0)
{
  std::stringstream ss;
  ss << value;
  double result;
  ss >> result;
  if (pos)
    *pos = ss.tellp();
  return result;
}
}
#endif

namespace biogears {

//----------------------------------------------------------------------------
std::vector<std::string> string_split(const std::string& src, const std::string& delimiter)
{
  std::vector<std::string> pieces;
  size_t i = 0;
  while (i < src.size()) {
    const auto n = std::min(src.size(), src.find(delimiter, i) - i);
    pieces.emplace_back(src, i, n);
    i += n + 1;
  }
  return pieces;
}

//----------------------------------------------------------------------------
double parse_double_or(double alt, const std::string& txt)
{
  double x = alt;
#ifndef ANDROID
  try {
    x = std::stod(txt);
  } catch (...) {
  }
#else
  std::std::stringstream ss;
  ss << txt;
  ss >> x;
#endif

  return x;
}

//----------------------------------------------------------------------------
float parse_float_or(float alt, const std::string& txt)
{
  float x = alt;
#ifndef ANDROID
  try {
    x = std::stof(txt);
  } catch (...) {
  }
#else
  std::std::stringstream ss;
  ss << txt;
  ss >> x;
#endif
  return x;
}

//----------------------------------------------------------------------------
int parse_int_or(int alt, const std::string& txt)
{
  int x = alt;
#ifndef ANDROID
  try {
    x = std::stoi(txt);
  } catch (...) {
  }
#else
  std::std::stringstream ss;
  ss << txt;
  ss >> x;
#endif
  return x;
}

//!
//! \brief Replaces all instances of the string toReplace in the string S with the string replaceWith
//! \param S : string to be split
//! \param toReplace : replacement string
//! \param replaceWith : string to be replaced
//! \return
//!
std::string findAndReplace(std::string& S, const std::string& toReplace, const std::string& replaceWith)
{
  size_t start = 0;
  while (true) {
    size_t pos = S.find(toReplace, start);
    if (pos == std::string::npos) {
      break;
    }
    S.replace(pos, toReplace.length(), replaceWith);
    start = pos + replaceWith.length();
  }
  return S;
}
//-------------------------------------------------------------------------------
//!
//! \brief Trims all leading whitespaces of string str
//! \param str : string to be trimmed
//! \param chars : whitespace characters
//! \return resulting string
//!
std::string& ltrim(std::string& str, const std::string& chars)
{
  str.erase(0, str.find_first_not_of(chars));
  return str;
}
//-------------------------------------------------------------------------------
//!
//! \brief Trims all leading whitespaces of string str
//! \param str : string to be trimmed
//! \param chars : whitespace characters
//! \return resulting string
//!
std::string ltrim(const std::string& input, const std::string& chars)
{
  std::string str(input);
  str.erase(0, str.find_first_not_of(chars));
  return str;
}
//-------------------------------------------------------------------------------
//!
//! \brief Trims all trailing whitespace of string str
//! \param str : string to be trimmed
//! \param chars : whitespace characters
//! \return resulting string
//!
std::string& rtrim(std::string& str, const std::string& chars)
{
  str.erase(str.find_last_not_of(chars) + 1);
  return str;
}
//-------------------------------------------------------------------------------
//!
//! \brief Trims all trailing whitespace of string str
//! \param str : string to be trimmed
//! \param chars : whitespace characters
//! \return resulting string
//!
std::string rtrim(const std::string& input, const std::string& chars)
{
  std::string str(input);
  str.erase(str.find_last_not_of(chars) + 1);
  return str;
}
//-------------------------------------------------------------------------------
//!
//! \brief Trims all leading and trailing whitespace of string str
//! \param str : string to be trimmed
//! \param chars : whitespace characters
//! \return resulting string
//! \details this version of trim is differentiated from the other version as the strings being passed in are rvalue references.
std::string& trim(std::string&& str, const std::string&& chars)
{
  return ltrim(rtrim(str, chars), chars);
}
//-------------------------------------------------------------------------------
//!
//! \brief Trims all leading and trailing whitespace of string str
//! \param str : string to be trimmed
//! \param chars :whitespace characters
//! \return resulting string
//!
std::string& trim(std::string& str, const std::string& chars)
{
  return ltrim(rtrim(str, chars), chars);
}
//-------------------------------------------------------------------------------
//!
//! \brief Trims all leading and trailing whitespace of string str
//! \param str : string to be trimmed
//! \param chars :whitespace characters
//! \return resulting string
//!
std::string trim(const std::string& input, const std::string& chars)
{
  std::string str(input);
  return ltrim(rtrim(str, chars), chars);
}
//-------------------------------------------------------------------------------
//!
//! \brief Performs a regex split of a string
//! \param input : string to be split
//! \param regex : string to split around
//! \return resulting string
//!
std::vector<std::string> re_split(const std::string& input, const std::string& regex)
{
  // passing -1 as the submatch index parameter performs splitting
  std::regex re(regex);
  std::sregex_token_iterator
    first { input.begin(), input.end(), re, -1 },
    last;
  return { first, last };
}
//-------------------------------------------------------------------------------
//!
//! \brief Splits string input around char delimiter
//! \param input : string to be split
//! \param delimiter : char to split around
//! \return std::vector containing both left and right split of string input
//!
std::vector<std::string> split(const std::string& input, const char delimiter)
{
  // passing -1 as the submatch index parameter performs splitting
  auto start = input.begin();
  auto end = input.begin();
  std::string d { delimiter };

  std::vector<std::string> rValue;
  while (end != input.end()) {
    end = std::find_first_of(start, input.end(), d.begin(), d.end());
    rValue.emplace_back(start, end);
    if (end != input.end()) {
      start = ++end;
    }
  }
  return rValue;
}
//-------------------------------------------------------------------------------
//!
//! \brief Converts all strings in vector of strings into doubles
//! \param input : string vector
//! \return double vector
//!
std::vector<double> vstod(const std::vector<std::string>& input)
{
  std::vector<double> result(input.size());
  std::transform(input.begin(), input.end(), result.begin(), [](const std::string& val) {
    return std::stod(val);
  });
  return result;
}
//--------------------------------------------------------------------------------
} // end namespace ara
