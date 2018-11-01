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

#include "../utils/string-helpers.h"
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
      process_aerosol(++lineItr);
      lineItr += 2;
    } else if ("Clearance (all or none)" == lineItr->first) {
      process_clearance(++lineItr);
      lineItr += 11;
    } else if ("Renal Dynamics Choices" == lineItr->first) {
      //TODO Process 6 Rows
    } else if ("Pharmacokinetics (all or none)" == lineItr->first) {
      //TODO Process 6 Rows
    } else if ("Aerosolization (all or none)" == lineItr->first) {
      //TODO Process 15 Rows
    } else if ("BoneTissue Pharmacokinetics" == lineItr->first) {
      //TODO PRocess Remaining ROWS in sets of two;
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
bool SubstanceGenerator::process_aerosol(CSV_RowItr itr)
{
  namespace CDM = mil::tatrc::physiology::datamodel;

  //TODO:Bounds Checking
  //TODO:Exceptiom Handling
  bool rValue = true;
  size_t index = 0;

  for (auto& substance : _substances) {

    CDM::SubstanceAerosolizationData data;
    //Step: 1 bronchiole_modifier
    auto& value = (itr)->second[index];
    if (!value.empty()) {
      CDM::SubstanceAerosolizationData::BronchioleModifier_type bron_data;
      try {

        bron_data.value(std::stoi(value));
        data.BronchioleModifier(bron_data);
      } catch (std::exception e) {
        rValue = false;
      }
    }
    //Step: 2 inflamation_coefficient
    value = (itr + 1)->second[index];
    if (!value.empty()) {
      CDM::SubstanceAerosolizationData::InflammationCoefficient_type infl_data;
      try {

        infl_data.value(std::stoi(value));
        data.InflammationCoefficient(infl_data);
      } catch (std::exception e) {
        rValue = false;
      }
    }
    //Step: 3 particulate_size_distribution
    value = (itr + 2)->second[index];
    if (!value.empty()) {
      CDM::SubstanceAerosolizationData::ParticulateSizeDistribution_type part_data;
      CDM::SubstanceAerosolizationData::ParticulateSizeDistribution_type::Independent_type indep_type;
      CDM::SubstanceAerosolizationData::ParticulateSizeDistribution_type::Dependent_type dep_type;

      value.erase(std::remove(value.begin(), value.end(), '"'), value.end());
      std::string delimiters = "][";
      auto token_start = value.begin();
      auto token_end = std::find_first_of(token_start + 1, value.end(), delimiters.begin(), delimiters.end());
      auto dependent = split({ token_start + 1, token_end }, ',');
      try {
        auto dVec = vstod(dependent);
        dep_type.DoubleList(CDM::SubstanceAerosolizationData::ParticulateSizeDistribution_type::Dependent_type::DoubleList_type(dVec.begin(), dVec.end()));
        part_data.Dependent(dep_type);
      } catch (std::exception e) {
        rValue = false;
      }

      token_start = std::find_first_of(token_end + 1, value.end(), delimiters.begin(), delimiters.end());
      token_end = std::find_first_of(token_start + 1, value.end(), delimiters.begin(), delimiters.end());
      auto indepdent = split({ token_start + 1, token_end }, ',');

      try {
        auto dVec = vstod(indepdent);
        indep_type.DoubleList(CDM::SubstanceAerosolizationData::ParticulateSizeDistribution_type::Dependent_type::DoubleList_type(dVec.begin(), dVec.end()));
        part_data.Independent(indep_type);
      } catch (std::exception e) {
        rValue = false;
      }

      token_start = std::find_first_of(token_end + 1, value.end(), delimiters.begin(), delimiters.end());
      token_end = std::find_first_of(token_start + 1, value.end(), delimiters.begin(), delimiters.end());
      auto units = split({ token_start + 1, token_end }, ',');
      part_data.DependentUnit(units[0]);
      part_data.IndependentUnit(units[1]);

      data.ParticulateSizeDistribution(part_data);
    }
    if (!value.empty()) {
      substance.Aerosolization(data);
    }
    ++index;
  }
  return rValue;
}
//-----------------------------------------------------------------------------
bool SubstanceGenerator::process_clearance(CSV_RowItr itr)
{
  namespace CDM = mil::tatrc::physiology::datamodel;

  //TODO:Bounds Checking
  //TODO:Exceptiom Handling
  bool rValue = true;
  bool apply_results = false;
  size_t index = 0;
  for (auto& substance : _substances) {
    CDM::SubstanceClearanceData data;

    auto& value = (itr)->second[index];
    if (!value.empty()) {
      CDM::SubstanceClearanceData::Systemic_type systemic_data;
      apply_results = true;

      CDM::SubstanceClearanceData::Systemic_type::FractionExcretedInFeces_type feif_data;
      CDM::SubstanceClearanceData::Systemic_type::FractionUnboundInPlasma_type fuip_data;
      CDM::SubstanceClearanceData::Systemic_type::IntrinsicClearance_type ic_data;
      CDM::SubstanceClearanceData::Systemic_type::RenalClearance_type rc_data;
      CDM::SubstanceClearanceData::Systemic_type::SystemicClearance_type sc_data;
      try {
        feif_data.value(std::stoi(value));
        systemic_data.FractionExcretedInFeces(feif_data);

        value = (itr + 1)->second[index];
        fuip_data.value(std::stoi(value));
        systemic_data.FractionUnboundInPlasma(fuip_data);

        value = (itr + 2)->second[index];
        ic_data.value(std::stoi(value));
        systemic_data.IntrinsicClearance(ic_data);

        value = (itr + 3)->second[index];
        rc_data.value(std::stoi(value));
        systemic_data.RenalClearance(rc_data);

        value = (itr + 4)->second[index];
        sc_data.value(std::stoi(value));
        systemic_data.SystemicClearance(sc_data);

        data.Systemic(systemic_data);
      } catch (std::exception e) {
        rValue = false;
      }
    }

    value = (itr + 6)->second[index];
    if (!value.empty()) {
      apply_results = true;
      CDM::SubstanceClearanceData::RenalDynamics_type renal_data;
      
    } else {
      value = (itr + 8)->second[index];
      if (!value.empty()) {
        apply_results = true;
        CDM::SubstanceClearanceData::RenalDynamics_type::Regulation_type;
      }
    }

    if (apply_results) {
      substance.Clearance(data);
    }
    ++index;
  }
  return rValue;
}
}
