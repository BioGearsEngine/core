#include "../utils/string-helpers.h"
#include "EnvironmentGenerator.h"

#include <fstream>
#include <iostream>

namespace biogears {

EnvironmentGenerator::EnvironmentGenerator(std::string path)
  : CSVToXMLConvertor(path, "Environment.csv")
{
}
//-----------------------------------------------------------------------------
EnvironmentGenerator::~EnvironmentGenerator()
{
}
//-----------------------------------------------------------------------------
bool EnvironmentGenerator::save() const
{
  for (auto& env : _environments) {
    xml_schema::namespace_infomap info;
    info[""].name = "uri:/mil/tatrc/physiology/datamodel";
    info[""].schema = "BioGears.xsd";

    try {
      std::ofstream file;
      file.open("environments/" + env.Name() + ".xml");
      EnvironmentalConditions(file, env, info);
      file.close();

    } catch (std::exception e) {
      std::cout << e.what() << std::endl;
    }
  }
  return false;
}
//-----------------------------------------------------------------------------
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
      av_data.unit(trim(value.substr(pos)));
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
      at_data.unit(u);
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
      ap_data.unit(u);
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
      cr_data.unit(u);
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
      mrt_data.unit(u);
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
      rh_data.unit(u);
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
      rat_data.unit(u);
      environment.RespirationAmbientTemperature(rat_data);
    } catch (std::exception e) {
      rValue = false;
    }
  }
  return rValue;
}

//-----------------------------------------------------------------------------

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
        ae_data.Concentration().unit(trim(value2.substr(pos)));
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