/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/
#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP
#include <fstream>
#include <vector>
#include <string>
#include "utils/string-helpers.h"

namespace biogears {
std::vector<std::string> ParseConfigFile(const std::string& verificationConfigFilePath)
{
  std::ifstream file(verificationConfigFilePath);
  std::vector<std::string> files;
  std::string line;
  line = ltrim(line);
  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') {
      continue;
    }
    files.push_back(line.substr(0,line.find_first_of('=')));
  }
  return files;
}

} // end namespace biogears

#endif //CONFIG_PARSER_HPP