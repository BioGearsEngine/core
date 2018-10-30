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
//!
//! \return success : true if parsed correctly else false
//!
bool SubstanceGenerator::parse()
{
  bool rValue = true;

  std::ifstream file("Substances.csv");
  std::string row;
  std::vector<std::vector<std::string>> columns;
  int column = 0;
    int start = 0;
  int i = 0;
  std::getline(file, row);
  for (; i < row.size(); ++i) {
      if (row.at(i) == ',') {
      std::vector<std::string> temp;
        temp.push_back(row.substr(start, i - start));
      columns.push_back(temp);
      ++i;
      start = i;
    }
  }
  std::cout << "001" << std::endl;
  std::vector<std::string> finalColumn;
  finalColumn.push_back(row.substr(start, row.size() - start));
  while (std::getline(file, row)) {
    column = 0;
    i = 0;
    start = 0;
    while (i < row.size()) {
      if (row.at(i) == ',') {
        columns.at(column).push_back(row.substr(start, i - start));
        ++column;
        ++i;
        start = i;
      } else if (row.at(i) == '"') {
        ++i;
        start = i;
        while (row.at(i) != '"') {
          if (i == (row.size() - 1)) {
            std::string row2;
            std::getline(file, row2);
            row.append(row2);
          }
          ++i;
        }
        columns.at(column).push_back(row.substr(start, i - start));
        ++column;
        ++i;
        start = i;
        while (i < row.size() && row.at(i) != ',') {
          ++i;
        }
        ++i;
      } else {
        ++i;
      }
    }
    finalColumn.push_back(row.substr(start, row.size() - start));
  }
  columns.push_back(finalColumn);
  std::cout << "002"
  i = 0;
  for (i = 0; i < columns.size(); ++i) {
    for (int k = 0; k < columns.at(i).size(); ++k) {
      std::cout << columns.at(i).at(k) << ",";
	}
    std::cout << std::endl
              << std::endl;
  }
  std::cout << "Complete" << std::endl;
  for (i = 0; i < columns.size(); ++i) {
    std::cout << i << " " << columns.at(i).size() << std::endl;
  }
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
bool process(const std::string& name, const std::string& value, mil::tatrc::physiology::datamodel::SubstanceData& substance)
{
  using namespace mil::tatrc::physiology::datamodel;
  bool rValue = true;
  if ("State" == name) {
    if ("Solid" == value) {
      substance.State(SubstanceData::State_type::Solid);
    } else if ("Liquid" == value) {
      substance.State(SubstanceData::State_type::Liquid);
    } else if ("Gas" == value) {
      substance.State(SubstanceData::State_type::Gas);
    } else if ("Molecular" == value) {
      substance.State(SubstanceData::State_type::Molecular);
    } else {
      rValue = false;
    }
  } else if ("Classification" == name) {
    if ("Anesthetic" == value) {
      substance.Classification(SubstanceData::Classification_type::Anesthetic);
    } else if ("Antibiotic" == value) {
      substance.Classification(SubstanceData::Classification_type::Antibiotic);
    } else if ("Ion" == value) {
      substance.Classification(SubstanceData::Classification_type::Ion);
    } else if ("Opioid" == value) {
      substance.Classification(SubstanceData::Classification_type::Opioid);
    } else if ("ReversalAgent" == value) {
      substance.Classification(SubstanceData::Classification_type::ReversalAgent);
    } else if ("Sedative" == value) {
      substance.Classification(SubstanceData::Classification_type::Sedative);
    } else if ("" == value) {
      //Classification is Optional and need not be set
    } else {
      rValue = false;
    }
  } else if ("Density" == name) {
    SubstanceData::Density_type density;
    size_t pos;
    try {
      double v = std::stod(value, &pos);
      std::string u = value.substr(pos);
      density.value(v);
      density.unit(u);
      substance.Density(density);
    } catch (std::exception e) {
      rValue = false;
    }

  } else if ("MaximumDiffusionFlux" == name) {
    SubstanceData::MaximumDiffusionFlux_type diffusion;
    size_t pos;
    try {
      double v = std::stod(value, &pos);
      std::string u = value.substr(pos);
      diffusion.value(v);
      diffusion.unit(u);
      substance.MaximumDiffusionFlux(diffusion);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("MichaelisCoefficient" == name) {
    SubstanceData::MichaelisCoefficient_type type;
    size_t pos;
    try {
      double v = std::stod(value, &pos);
      std::string u = value.substr(pos);
      type.value(v);
      type.unit(u);
      substance.MichaelisCoefficient(type);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("MembraneResistance" == name) {
    SubstanceData::MembraneResistance_type type;
    size_t pos;
    try {
      double v = std::stod(value, &pos);
      std::string u = value.substr(pos);
      type.value(v);
      type.unit(u);
      substance.MembraneResistance(type);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("MolarMass" == name) {
    SubstanceData::MolarMass_type type;
    size_t pos;
    try {
      double v = std::stod(value, &pos);
      std::string u = value.substr(pos);
      type.value(v);
      type.unit(u);
      substance.MolarMass(type);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("RelativeDiffusionCoefficient" == name) {
    SubstanceData::RelativeDiffusionCoefficient_type type;
    size_t pos;
    try {
      double v = std::stod(value, &pos);
      std::string u = value.substr(pos);
      type.value(v);
      type.unit(u);
      substance.RelativeDiffusionCoefficient(type);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("SolubilityCoefficient" == name) {
    SubstanceData::SolubilityCoefficient_type type;
    size_t pos;
    try {
      double v = std::stod(value, &pos);
      std::string u = value.substr(pos);
      type.value(v);
      type.unit(u);
      substance.SolubilityCoefficient(type);
    } catch (std::exception e) {
      rValue = false;
    }
  }
  return rValue;
}
//-----------------------------------------------------------------------------
bool SubstanceGenerator::process(const std::vector<std::vector<std::string>>& data)
{
  using namespace mil::tatrc::physiology::datamodel;
  bool rValue = true;

  if (data.size() > 2) {
    auto& headers = data[0];
    auto headerItr = headers.begin() + 1;
    auto substanceItr = data.begin() + 1;

    for (auto itr = headers.begin() + 1; itr != headers.end(); ++itr) {
      _substances.emplace_back();
      auto& substance = _substances.back();
      substance.Name(*itr);
    }

    while (substanceItr != data.end()) {
      auto& sub = *substanceItr;
      auto itr = _substances.begin();
      size_t index = 1;
      for (auto& substance : _substances) {
        if (index < sub.size()) {
          rValue &= biogears::process(sub[0], sub[index++], substance);
        }
      }
      ++substanceItr;
    }
  }
  else {
    rValue = false;
  }
  for (auto& substance : _substances) {
    std::cout << substance;

    xml_schema::namespace_infomap info;
    info[""].name = "uri:/mil/tatrc/physiology/datamodel";
    info[""].schema = "BioGears.xsd";

    try {
      std::ofstream file;
      file.open(substance.Name() + ".xml");
      Substance(file, substance, info);
      file.close();
    }
    catch (std::exception e) {
      std::cout << e.what() << std::endl;
    }
  }
  return rValue;
}
}
