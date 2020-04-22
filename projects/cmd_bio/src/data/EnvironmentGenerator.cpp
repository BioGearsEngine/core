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

#include "../utils/string-helpers.h"
#include "EnvironmentGenerator.h"

#include <fstream>
#include <iostream>
#include <cstdlib>

#include <biogears/string/manipulation.h>

#ifdef ANDROID
  namespace std {
    static double std::stod(const std::string& value, std::size_t* pos = 0){
      char **end = value.c_str();
      double result= std::strold(value.c_str(), end); 
      *pos = *end - value.c_str();
      return result;
    }
  }
#endif
namespace biogears {
std::string ConvertTemperatureUnit(std::string unit);



//!
//! \brief converts string std::string unit into appropriate representation of the unit it represents
//! \param unit 
//! \return 
//! \details for the purpose of running scenarios, the units "C", "F", and "R" are not recognized. This
//!          converts them into units that are accepted by the scenario executor.
//! 
std::string ConvertTemperatureUnit(std::string unit)
{
  if (unit == "C") {
    return "degC";
  }
  if (unit == "F") {
    return "degF";
  }
  if (unit == "R") {
    return "degR";
  }
  return unit;
}

EnvironmentGenerator::EnvironmentGenerator(std::string path)
  : CSVToXMLConvertor(path, "Environment.csv")
{
}
//-----------------------------------------------------------------------------
EnvironmentGenerator::~EnvironmentGenerator()
{
}
//-----------------------------------------------------------------------------
//!
//! \brief saves xml file for  each environment
//! \return bool, true if no exceptions were thrown, false otherwise 
//! 
bool EnvironmentGenerator::save() const
{
  bool rValue = true;
  for (auto& env : _environments) {
    xml_schema::namespace_infomap info;
    info[""].name = "uri:/mil/tatrc/physiology/datamodel";
    info[""].schema = "BioGears.xsd";

    try {
      std::ofstream file;
      file.open("environments/" + env.Name().get() + ".xml");
      EnvironmentalConditions(file, env, info);
      file.close();
      std::cout << "Saved environments/" + env.Name().get() + ".xml" << "\n";

    } catch (std::exception e) {
      rValue = false;
      std::cout << e.what() << std::endl;
    }
  }
  return rValue;
}
//-----------------------------------------------------------------------------
//!
//! \brief populates _environments with EnvironmentalDataObjects to be read into xml's
//! \return bool
//! 
bool EnvironmentGenerator::parse()
{
  namespace CDM = mil::tatrc::physiology::datamodel;
  bool rValue = true;
  read_csv();
  for (auto lineItr = begin(); lineItr != end(); ++lineItr) {
    if ("Name" == lineItr->first) {
      for (auto name : lineItr->second) {
        CDM::EnvironmentalConditionsData data;
        data.Name(name);
        _environments.push_back(std::move(data));
      }
    } else if ("AmbientGasData" == lineItr->first) {
      process_ambientgasdata(lineItr);
      lineItr += 2;
    } else if ("AmbientAerosolData" == lineItr->first) {
	      process_ambientaerosoldata(lineItr);
      lineItr += 2;
    } else {
      for (size_t index = 0; index < _environments.size() && index < lineItr->second.size(); ++index) {
        process(lineItr->first, lineItr->second[index], _environments[index]);
      }
    }
  }
  return rValue;
}
//-----------------------------------------------------------------------------
void EnvironmentGenerator::print() const
{
  for (auto& env : _environments) {
    std::cout << env;
  }
}
//-----------------------------------------------------------------------------
//!
//! \brief Checks first cell of csv row and sets corresponding data of EnvironmentalConditionsObject 
//! \param name first cell of row
//! \param value another cell of the same row
//! \param environment EnvironmentalConditionsData object
//! \return 
//! 
bool EnvironmentGenerator::process(const std::string& name, const std::string& value, mil::tatrc::physiology::datamodel::EnvironmentalConditionsData& environment)
{
  using namespace mil::tatrc::physiology::datamodel;
  bool rValue = true;

  if (value.empty()) {
    // do nothing if value is empty
  } else if ("SurroundingType" == name) {
    if ("Air" == value) {
      environment.SurroundingType(EnvironmentalConditionsData::SurroundingType_type::Air);
    } else if ("Water" == value) {
      environment.SurroundingType(EnvironmentalConditionsData::SurroundingType_type::Water);
    }
  } else if ("AirVelocity" == name) {
    EnvironmentalConditionsData::AirVelocity_type av_data;
    size_t pos = 0;
    try {
      av_data.value(std::stod(value, &pos));
	    av_data.unit(ConvertTemperatureUnit(trim(value.substr(pos))));
      environment.AirVelocity(av_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("AmbientTemperature" == name) {
    EnvironmentalConditionsData::AmbientTemperature_type at_data;
    size_t pos;
    try {
      double v = std::stod(value, &pos);
      std::string u = value.substr(pos);
      at_data.value(v);
      at_data.unit(ConvertTemperatureUnit(trim(u)));
      environment.AmbientTemperature(at_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("AtmosphericPressure" == name) {
    EnvironmentalConditionsData::AtmosphericPressure_type ap_data;
    size_t pos;
    try {
      double v = std::stod(value, &pos);
      std::string u = value.substr(pos);
      ap_data.value(v);
      ap_data.unit(ConvertTemperatureUnit(trim(u)));
      environment.AtmosphericPressure(ap_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("ClothingResistance" == name) {
    EnvironmentalConditionsData::ClothingResistance_type cr_data;
    size_t pos;
    try {
      double v = std::stod(value, &pos);
      std::string u = value.substr(pos);
      cr_data.value(v);
      cr_data.unit(ConvertTemperatureUnit(trim(u)));
      environment.ClothingResistance(cr_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("Emissivity" == name) {
    EnvironmentalConditionsData::Emissivity_type e_data;
    try {
      double v = std::stod(value);
      e_data.value(v);
      environment.Emissivity(e_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("MeanRadiantTemperature" == name) {
    EnvironmentalConditionsData::MeanRadiantTemperature_type mrt_data;
    size_t pos;
    try {
      double v = std::stod(value, &pos);
      std::string u = value.substr(pos);
      mrt_data.value(v);
      mrt_data.unit(ConvertTemperatureUnit(trim(u)));
      environment.MeanRadiantTemperature(mrt_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("RelativeHumidity" == name) {
    EnvironmentalConditionsData::RelativeHumidity_type rh_data;
    size_t pos;
    try {
      double v = std::stod(value, &pos);
      std::string u = value.substr(pos);
      rh_data.value(v);
      rh_data.unit(ConvertTemperatureUnit(trim(u)));
      environment.RelativeHumidity(rh_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("RespirationAmbientTemperature" == name) {
    EnvironmentalConditionsData::RespirationAmbientTemperature_type rat_data;
    size_t pos;
    try {
      double v = std::stod(value, &pos);
      std::string u = value.substr(pos);
      rat_data.value(v);
      rat_data.unit(ConvertTemperatureUnit(trim(u)));
      environment.RespirationAmbientTemperature(rat_data);
    } catch (std::exception e) {
      rValue = false;
    }
  }
  return rValue;
}

//-----------------------------------------------------------------------------

//!
//! \brief Reads in data for the xml tags nested inside the aerosolization tag
//! \param itr, iterator for data structure representation of CSV rows
//! \return 
//! \details since nested tags are dependent on multiple rows of the csv document it is necessary to write a method such as this one
//!          for all tags which nest other xml tags.
bool EnvironmentGenerator::process_ambientgasdata(CSV_RowItr itr)
{
  namespace CDM = mil::tatrc::physiology::datamodel;
  bool rValue = true;
  bool apply_results = false;
  size_t index = 0;
  for (auto& environment : _environments) {
    auto& value1 = (itr + 1)->second[index];
    auto& value2 = (itr + 2)->second[index];
    if (!value1.empty()) {
      try {
        CDM::EnvironmentalConditionsData::AmbientGas_type ag_data;
        CDM::EnvironmentalConditionsData::AmbientGas_type::FractionAmount_type fa_data;
        ag_data.Name(value1);
        fa_data.value(std::stod(value2));
        ag_data.FractionAmount(fa_data);
        environment.AmbientGas().push_back(ag_data);
      } catch (std::exception e) {
        rValue = false;
      }
    }
    ++index;
  }
  return rValue;
}

//-----------------------------------------------------------------------------
//!
//! \brief Reads in data for the xml tags nested inside the ambientaerosoldata tag
//! \param itr, iterator for data structure representation of CSV rows
//! \return bool, true if no exceptions were thrown, false otherwise
//! 
bool EnvironmentGenerator::process_ambientaerosoldata(CSV_RowItr itr)
{
  namespace CDM = mil::tatrc::physiology::datamodel;
  bool rValue = true;
  bool apply_results = false;
  size_t index = 0;
  for (auto& environment : _environments) {
    auto& value1 = (itr + 1)->second[index];
    auto& value2 = (itr + 2)->second[index];
    if (!value1.empty()) {
      CDM::EnvironmentalConditionsData::AmbientAerosol_type ae_data;
      try {
        size_t pos = 0;
        ae_data.Name(value1);
        ae_data.Concentration(std::stod(value2, &pos));
        ae_data.Concentration().unit(ConvertTemperatureUnit(trim(value2.substr(pos))));
        environment.AmbientAerosol().push_back(ae_data);
      } catch (std::exception e) {
        rValue = false;
      }
    }
    ++index;
  }
  return rValue;
}



} //Namespace
