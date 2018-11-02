//**********************************************************************************
//Copyright 2015 Applied Research Associates, Inc.
//Licensed under the Apache License, Version 2.0 (the "License"); you may not use
//this file except in compliance with the License.You may obtain a copy of the License
//at :
//http://www.apache.org/licenses/LICENSE-2.0
//Unless required by applicable law or agreed to in writing, software distributed under
//the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//CONDITIONS OF ANY KIND, either express or implied.See the License for the
//specific language governing permissions and limitations under the License.
//**************************************************************************************

#include "CSVToXMLConvertor.h"
#include <biogears/exports.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../utils/string-helpers.h"
namespace biogears {

CSVToXMLConvertor::CSVToXMLConvertor(std::string path, std::string filename)
  : _path(path)
  , _filename(filename)
{
}
//-------------------------------------------------------------------------------
bool CSVToXMLConvertor::read_csv()
{
  using csv = std::vector<std::pair<std::string, std::vector<std::string>>>;
  using csv_row = std::pair<std::string, std::vector<std::string>>;

  bool rValue = true;

  auto filename = (Path().empty()) ? Filename() : Path() + "/" + Filename();
  std::ifstream input(filename);
  std::string line = "";
  std::string token = "";

  csv_row current_row;
  while (std::getline(input, line)) {
    current_row.first.clear();
    current_row.second.clear();

    rValue &= parse_line(line, current_row, input);
    if (!current_row.first.empty() && "UNSET" != current_row.first) {
      _data.push_back(current_row);
    }
  }

  return rValue;
}
//-------------------------------------------------------------------------------
bool CSVToXMLConvertor::parse_line(std::string& line, std::pair<std::string, std::vector<std::string>>& row, std::ifstream& input)
{
  bool rValue = true;

  auto token_start = line.begin();
  auto token_end = line.begin();
  std::string delimiters = R"(")";
  //Grab The Header
  while (token_end != line.end()) {
    switch (*token_end) {
    case ',':
      if (row.first.empty()) {
        row.first = std::string(token_start, token_end);
        token_start = token_end + 1;
        if (row.first.empty()) {
          row.first = "LOGIC_GUARD_FOR_UNAMED_ROWS";
        }
      } else {
        row.second.emplace_back(token_start, token_end);
        trim(row.second.back());
        token_start = token_end + 1;
      }
      break;
    case '"':
      token_end = std::find_first_of(token_end + 1, line.end(), delimiters.begin(), delimiters.end());
      //Handle Returnlines in " which is insane but in the files
      if (token_end == line.end()) {
        do {
          std::string next_line;
          std::getline(input, next_line);
          size_t start = std::distance(line.begin(), token_start);
          size_t end = std::distance(line.begin(), token_end);

          line += next_line;
          token_start = line.begin() + start;
          token_end = line.begin() + end;
          token_end = std::find_first_of(token_end + 1, line.end(), delimiters.begin(), delimiters.end());
        } while (token_end == line.end() && input.good());
      }
      break;
    default:
      break;
    }
    ++token_end;
  }
  if ("LOGIC_GUARD_FOR_UNAMED_ROWS" == row.first) {
    row.first.clear();
  }
  return rValue;
}
}
