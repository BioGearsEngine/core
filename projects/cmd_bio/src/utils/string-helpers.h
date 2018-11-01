#ifndef BIOGEARS_CMD_UTILS_STRING_HELPER_H
#define BIOGEARS_CMD_UTILS_STRING_HELPER_H

//**********************************************************************************
//Copyright 2015 Applied Research Associates, Inc.
//Licensed under the Apache License, Version 2.0 (the "License"); you may not use
//this file except in compliance with the License.You may obtain a copy of the License
//at :
//http:
//www.apache.org/licenses/LICENSE-2.0
//Unless required by applicable law or agreed to in writing, software distributed under
//the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//CONDITIONS OF ANY KIND, either express or implied.See the License for the
//specific language governing permissions and limitations under the License.
//**************************************************************************************:

#include <string>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

namespace biogears {
  inline std::string findAndReplace(std::string& S, const std::string& toReplace, const std::string& replaceWith)
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

  std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
  {
    str.erase(0, str.find_first_not_of(chars));
    return str;
  }

  std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
  {
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
  }

  std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
  {
    return ltrim(rtrim(str, chars), chars);
  }
}

#endif // BIOGEARS_CMD_UTILS_STRING_HELPER_H
