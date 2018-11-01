#include "EnvironmentGenerator.h"

#include <fstream>
#include <iostream>

namespace biogears {

bool EnvironmentGenerator::process(const std::string& name, const std::string& value, mil::tatrc::physiology::datamodel::EnvironmentalConditionsData& environment)
{
  using namespace mil::tatrc::physiology::datamodel;
  bool rValue = true;
  if (value.empty()) {

  } else if ("SurroundingType" == name) {
	if ("Air" == value) {
      environment.SurroundingType(EnvironmentalConditionsData::SurroundingType_type::Air);
	} else if ("Water" == value) {
      environment.SurroundingType(EnvironmentalConditionsData::SurroundingType_type::Water);
	}
  } else if ("AirVelocity" == name) {
    EnvironmentalConditionsData::AirVelocity_type av_data;
    size_t pos;
    try {
      double v = std::stod(value, &pos);
      std::string u = value.substr(pos);
      av_data.value(v);
      av_data.unit(u);
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
}