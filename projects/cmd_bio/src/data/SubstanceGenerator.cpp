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

#include "SubstanceGenerator.h"

#include <fstream>
#include <iostream>

namespace biogears {
//-----------------------------------------------------------------------------
SubstanceGenerator::SubstanceGenerator(std::string path)
  : CSVToXMLConvertor(path, "Substances.csv")
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
  namespace CDM = mil::tatrc::physiology::datamodel;
  bool rValue = true;
  read_csv();
  for (auto lineItr = begin(); lineItr != end(); ++lineItr) {
    if ("Name" == lineItr->first) {
      for (auto name : lineItr->second) {
        CDM::SubstanceData data;
        data.Name(name);
        _substances.push_back(std::move(data));
      }
    } else if ("Aerosolization (all or none)" == lineItr->first) {

    } else if ("Clearance (all or none)" == lineItr->first) {

    } else if ("Renal Dynamics Choices" == lineItr->first) {

    } else if ("Pharmacokinetics (all or none)" == lineItr->first) {

    } else if ("Aerosolization (all or none)" == lineItr->first) {

    } else if ("Aerosolization (all or none)" == lineItr->first) {

    } else if ("Aerosolization (all or none)" == lineItr->first) {

    } else if ("Aerosolization (all or none)" == lineItr->first) {

    } else if ("BoneTissue Pharmacokinetics" == lineItr->first) {

    } else {
      for (auto property : lineItr->second) {
        for (auto& substance : _substances) {
          process(lineItr->first, property, substance);
        }
      }
    }
  }
  return rValue;
}
//-----------------------------------------------------------------------------
bool SubstanceGenerator::save() const
{
  for (auto& substance : _substances) {
    xml_schema::namespace_infomap info;
    info[""].name = "uri:/mil/tatrc/physiology/datamodel";
    info[""].schema = "BioGears.xsd";

    try {
      std::ofstream file;
      file.open(substance.Name() + ".xml");
      Substance(file, substance, info);
      file.close();

    } catch (std::exception e) {
      std::cout << e.what() << std::endl;
    }
  }
  return false;
}
//-----------------------------------------------------------------------------
void SubstanceGenerator::print() const
{
  for (auto& substance : _substances) {
    std::cout << substance;
  }
}
//-----------------------------------------------------------------------------
bool SubstanceGenerator::process(const std::string& name, const std::string& value, mil::tatrc::physiology::datamodel::SubstanceData& substance)
{
  using namespace mil::tatrc::physiology::datamodel;
  bool rValue = true;
  if (value.empty()) {
    //Process nothing as the value is empty.
  } else if ("State" == name) {
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
}