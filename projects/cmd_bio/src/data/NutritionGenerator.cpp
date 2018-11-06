
#include "NutritionGenerator.h"
#include "../utils/string-helpers.h"

#include <biogears/schema/cdm/Patient.hxx>
#include <fstream>
#include <iostream>

namespace biogears {

NutritionGenerator::NutritionGenerator(std::string path)
  : CSVToXMLConvertor(path, "Nutrition.csv")
{
}
//-----------------------------------------------------------------------------
NutritionGenerator::~NutritionGenerator()
{
}
//-----------------------------------------------------------------------------
bool NutritionGenerator::save() const
{
  for (auto& nutrition_conf : _nutrients) {
    xml_schema::namespace_infomap info;
    info[""].name = "uri:/mil/tatrc/physiology/datamodel";
    info[""].schema = "BioGears.xsd";

    try {
      std::ofstream file;
      file.open("nutrition/" + nutrition_conf.Name() + ".xml");
      mil::tatrc::physiology::datamodel::Nutrition(file, nutrition_conf, info);
      file.close();

    } catch (std::exception e) {
      std::cout << e.what() << std::endl;
    }
  }
  return false;
}
//-----------------------------------------------------------------------------
bool NutritionGenerator::parse()
{
  namespace CDM = mil::tatrc::physiology::datamodel;
  bool rValue = true;
  read_csv();
  for (auto lineItr = begin(); lineItr != end(); ++lineItr) {
    if ("Name" == lineItr->first) {
      for (auto name : lineItr->second) {
        CDM::NutritionData data;
        data.Name(name);
        _nutrients.push_back(std::move(data));
      }
    } else {
      for (size_t index = 0; index < _nutrients.size() && index < lineItr->second.size(); ++index) {
        process(lineItr->first, lineItr->second[index], _nutrients[index]);
      }
    }
  }
  return rValue;
}
//-----------------------------------------------------------------------------
void NutritionGenerator::print() const
{
  for (auto& env : _nutrients) {
    std::cout << env;
  }
}

//-----------------------------------------------------------------------------
bool NutritionGenerator::process(const std::string& name, const std::string& value, mil::tatrc::physiology::datamodel::NutritionData& nutrient)
{
  using namespace mil::tatrc::physiology::datamodel;
  bool rValue = true;
  if (value.empty()) {

  } else if ("Carbohydrate" == name) {
    size_t pos;
    NutritionData::Carbohydrate_type c_data;
    try {
      c_data.value(std::stod(value, &pos));
      c_data.unit(trim(value.substr(pos)));
      nutrient.Carbohydrate(c_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("CarbohydrateDigestionRate" == name) {
    size_t pos;
    NutritionData::CarbohydrateDigestionRate_type type_data;
    try {
      type_data.value(std::stod(value, &pos));
      type_data.unit(trim(value.substr(pos)));
      nutrient.CarbohydrateDigestionRate(type_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("Fat" == name) {
    size_t pos;
    NutritionData::Fat_type type_data;
    try {
      type_data.value(std::stod(value, &pos));
      type_data.unit(trim(value.substr(pos)));
      nutrient.Fat(type_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("FatDigestionRate" == name) {
    size_t pos;
    NutritionData::FatDigestionRate_type type_data;
    try {
      type_data.value(std::stod(value, &pos));
      type_data.unit(trim(value.substr(pos)));
      nutrient.FatDigestionRate(type_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("Protein" == name) {
    size_t pos;
    NutritionData::Protein_type type_data;
    try {
      type_data.value(std::stod(value, &pos));
      type_data.unit(trim(value.substr(pos)));
      nutrient.Protein(type_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("ProteinDigestionRate" == name) {
    size_t pos;
    NutritionData::ProteinDigestionRate_type type_data;
    try {
      type_data.value(std::stod(value, &pos));
      type_data.unit(trim(value.substr(pos)));
      nutrient.ProteinDigestionRate(type_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("Calcium" == name) {
    size_t pos;
    NutritionData::Calcium_type type_data;
    try {
      type_data.value(std::stod(value, &pos));
      type_data.unit(trim(value.substr(pos)));
      nutrient.Calcium(type_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("Sodium" == name) {
    size_t pos;
    NutritionData::Sodium_type type_data;
    try {
      type_data.value(std::stod(value, &pos));
      type_data.unit(trim(value.substr(pos)));
      nutrient.Sodium(type_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("Water" == name) {
    size_t pos;
    NutritionData::Water_type type_data;
    try {
      type_data.value(std::stod(value, &pos));
      type_data.unit(trim(value.substr(pos)));
      nutrient.Water(type_data);
    } catch (std::exception e) {
      rValue = false;
    }
  }
  return rValue;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

} //end namespace biogears

