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
#include <biogears/cdm/utils/ConfigParser.h>

#include <fstream>
#include <tuple>

namespace biogears {
void ConfigSet::SetKeyValues(const std::map<std::string, std::string>& keyValues)
{
  m_keyValues = keyValues;
}

void ConfigSet::AddKeyValue(const std::string& key, const std::string& value)
{
  m_keyValues[key] = value;
}

bool ConfigSet::HasKey(const std::string& key) const
{
  return m_keyValues.find(key) != end(m_keyValues);
}

std::string ConfigSet::GetValue(const std::string& key) const
{
  if (!HasKey(key)) {
    return "";
  } else {
    return m_keyValues.find(key)->second;
  }
}

const char* ConfigSet::GetValue(const char* key) const
{
  if (!HasKey(key)) {
    return "";
  } else {
    return m_keyValues.find(key)->second.c_str();
  }
}

ConfigParser::ConfigParser(const std::string& configFilePath)
{
  ParseConfigFile(configFilePath);
}

void ConfigParser::ParseConfigFile(const std::string& verificationConfigFilePath)
{
  std::ifstream file(verificationConfigFilePath);

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') {
      continue;
    }

    if (line[0] == '@') {
      if (line.find("group") == 1) {
        m_parsingGlobalKeyValues = false;
        m_configSets.push_back(ConfigSet());
        m_configSets.back().SetKeyValues(m_globalKeyValues);
      }
    } else if (line.find("=") != std::string::npos) {
      std::string key;
      std::string value;

      std::tie(key, value) = ParseKeyValue(line);

      if (m_parsingGlobalKeyValues) {
        m_globalKeyValues[key] = value;
      } else {
        m_configSets.back().AddKeyValue(key, value);
      }
    }
  }
}

std::tuple<std::string, std::string> ConfigParser::ParseKeyValue(const std::string& line)
{
  return std::make_tuple(line.substr(0, line.find("=")), line.substr(line.find("=") + 1));
}
}