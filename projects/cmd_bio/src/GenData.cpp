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

#include "GenData.h"
#include <biogears/exports.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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

namespace biogears {
CSVToXMLConvertor::CSVToXMLConvertor(std::string path, std::string filename)
  : _path(path)
  , _filename(filename)
{
}
//-----------------------------------------------------------------------------
SubstanceGenerator::SubstanceGenerator(std::string path, std::string filename)
  : CSVToXMLConvertor(path, filename)
{
}
//-----------------------------------------------------------------------------
SubstanceGenerator::~SubstanceGenerator()
{
}
//-----------------------------------------------------------------------------
bool SubstanceGenerator::parse()
{
  std::ifstream file("Substances.csv");
  std::string row;
  std::vector<std::vector<std::string>> rows;
  
  while (std::getline(file, row)) {
    std::vector<std::string> temp;
    int i = 0;
    int start = 0;
    while (i < row.size()) {
      if (row.at(i) == ',') {
        temp.push_back(row.substr(start, i - start));
        ++i;
        start = i;
	  }
      else if (row.at(i) == '"') {
        ++i;
        int start = i;
        while (row.at(i) != '"') {
          if (i == row.size() - 1) {
            std::string row2;
            std::getline(file, row2);
			row.append(row2);
		  }
          ++i;
		}
        temp.push_back(row.substr(start, i - start));
        while (i < row.size() && row.at(i) != ',') {
          ++i;
		}
        ++i;
	  } 
	  else {
        ++i;
	  }
	}
    rows.push_back(temp);
  }
  for (auto& row : rows) {
    for (auto& string : row) {
      std::cout << string << " ";
	}
    std::cout << std::endl;
  }
  std::cout << "Complete" << std::endl;
  return false;
}
//-----------------------------------------------------------------------------
bool SubstanceGenerator::save() const
{
  return false;
}
//-----------------------------------------------------------------------------
void SubstanceGenerator::print() const
{
  std::cout << "\n"
            << Path() << Filename() << "\n";
}
//-----------------------------------------------------------------------------
}
