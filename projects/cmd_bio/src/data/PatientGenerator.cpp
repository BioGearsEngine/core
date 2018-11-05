#include "PatientGenerator.h"
#include "../utils/string-helpers.h"

#include <fstream>
#include <iostream>

namespace biogears {

PatientGenerator::PatientGenerator(std::string path)
  : CSVToXMLConvertor(path, "Patients.csv")
{
}
//-----------------------------------------------------------------------------
PatientGenerator::~PatientGenerator()
{
}
//-----------------------------------------------------------------------------
bool PatientGenerator::save() const
{
  for (auto& patient : _patients) {
    xml_schema::namespace_infomap info;
    info[""].name = "uri:/mil/tatrc/physiology/datamodel";
    info[""].schema = "BioGears.xsd";

    try {
      std::ofstream file;
      file.open("patients" + patient.Name() + ".xml");
      mil::tatrc::physiology::datamodel::Patient(file, patient, info);
      file.close();

    } catch (std::exception e) {
      std::cout << e.what() << std::endl;
    }
  }
  return false;
}
//-----------------------------------------------------------------------------
bool PatientGenerator::parse()
{
  namespace CDM = mil::tatrc::physiology::datamodel;
  bool rValue = true;
  read_csv();
  for (auto lineItr = begin(); lineItr != end(); ++lineItr) {
    if ("Name" == lineItr->first) {
      for (auto name : lineItr->second) {
        CDM::PatientData data;
        data.Name(name);
        _patients.push_back(std::move(data));
      }
    } else if ("AmbientGasData" == lineItr->first) {
      //process_ambientgasdata(lineItr);
      //lineItr += 2;
    } else if ("AmbientAerosolData" == lineItr->first) {
      //process_ambientaerosoldata(lineItr);
      //lineItr += 2;
    } else {
      for (size_t index = 0; index < _patients.size() && index < lineItr->second.size(); ++index) {
        process(lineItr->first, lineItr->second[index], _patients[index]);
      }
    }
  }
  return rValue;
}
//-----------------------------------------------------------------------------
void PatientGenerator::print() const
{
  for (auto& env : _patients) {
    std::cout << env;
  }
}

//-----------------------------------------------------------------------------
bool PatientGenerator::process(const std::string& name, const std::string& value, mil::tatrc::physiology::datamodel::PatientData& patient)
{
  using namespace mil::tatrc::physiology::datamodel;
  bool rValue = true;
  if (value.empty()) {

  } else if ("Name" == name) {
    patient.Name(value);
  } else if ("Sex" == name) {
    patient.Sex(value);
  } else if ("Age" == name) {
    size_t pos;
    PatientData::Age_type a_data;
    try {
      a_data.value(std::stod(value, &pos));
      a_data.unit(value.substr(pos));
      patient.Age(a_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("Weight" == name) {
    size_t pos;
    PatientData::Weight_type w_data;
    try {
      w_data.value(std::stod(value, &pos));
      w_data.unit(value.substr(pos));
      patient.Weight(w_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("Height" == name) {
    size_t pos;
    PatientData::Height_type h_data;
    try {
      h_data.value(std::stod(value, &pos));
      h_data.unit(value.substr(pos));
      patient.Height(h_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("BodyFatFraction" == name) {
    PatientData::BodyFatFraction_type bff_data;
    try {
      bff_data.value(std::stod(value));
      patient.BodyFatFraction(bff_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("RightLungFraction" == name) {
    PatientData::RightLungRatio_type rlr_data;
    try {
      rlr_data.value(std::stod(value));
      patient.RightLungRatio(rlr_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("SkinSurfaceArea" == name) {
    size_t pos;
    PatientData::SkinSurfaceArea_type ssa_data;
    try {
      ssa_data.value(std::stod(value, &pos));
      ssa_data.unit(value.substr(pos));
      patient.SkinSurfaceArea(ssa_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("BasalMetabolicRate" == name) {
    size_t pos;
    PatientData::BasalMetabolicRate_type bmr_data;
    try {
      bmr_data.value(std::stod(value, &pos));
      bmr_data.unit(value.substr(pos));
      patient.BasalMetabolicRate(bmr_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("BloodVolumeBaseline" == name) {
    size_t pos;
    PatientData::BloodVolumeBaseline_type bvb_data;
    try {
      bvb_data.value(std::stod(value, &pos));
      bvb_data.unit(value.substr(pos));
      patient.BloodVolumeBaseline(bvb_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("DiastolicArterialPressureBaseline" == name) {
    size_t pos;
    PatientData::DiastolicArterialPressureBaseline_type dapb_data;
    try {
      dapb_data.value(std::stod(value, &pos));
      dapb_data.unit(value.substr(pos));
      patient.DiastolicArterialPressureBaseline(dapb_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("HeartRateBaseline" == name) {
    size_t pos;
    PatientData::HeartRateBaseline_type hrb_data;
    try {
      hrb_data.value(std::stod(value, &pos));
      hrb_data.unit(value.substr(pos));
      patient.HeartRateBaseline(hrb_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("RespirationRateBaseline" == name) {
    size_t pos;
    PatientData::RespirationRateBaseline_type rrb_data;
    try {
      rrb_data.value(std::stod(value, &pos));
      rrb_data.unit(value.substr(pos));
      patient.RespirationRateBaseline(rrb_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("SystolicArterialPressureBaseline" == name) {
    size_t pos;
    PatientData::SystolicArterialPressureBaseline_type sapb_data;
    try {
      sapb_data.value(std::stod(value, &pos));
      sapb_data.unit(value.substr(pos));
      patient.SystolicArterialPressureBaseline(sapb_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("HeartRateMaximum" == name) {
    size_t pos;
    PatientData::HeartRateMaximum_type hrm_data;
    try {
      hrm_data.value(std::stod(value, &pos));
      hrm_data.unit(value.substr(pos));
      patient.HeartRateMaximum(hrm_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("HeartRateMinimum" == name) {
    size_t pos;
    PatientData::HeartRateMinimum_type hrm_data;
    try {
      hrm_data.value(std::stod(value, &pos));
      hrm_data.unit(value.substr(pos));
      patient.HeartRateMinimum(hrm_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("FunctionalResidualCapacity" == name) {
    size_t pos;
    PatientData::FunctionalResidualCapacity_type frc_data;
    try {
      frc_data.value(std::stod(value, &pos));
      frc_data.unit(value.substr(pos));
      patient.FunctionalResidualCapacity(frc_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("TotalLungCapacity" == name) {
    size_t pos;
    PatientData::TotalLungCapacity_type tlc_data;
    try {
      tlc_data.value(std::stod(value, &pos));
      tlc_data.unit(value.substr(pos));
      patient.TotalLungCapacity(tlc_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("AlveoliSurfaceArea" == name) {
    size_t pos;
    PatientData::AlveoliSurfaceArea_type asa_data;
    try {
      asa_data.value(std::stod(value, &pos));
      asa_data.unit(value.substr(pos));
      patient.AlveoliSurfaceArea(asa_data);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("ResidualVolume" == name) {
    size_t pos;
    PatientData::ResidualVolume_type rv_data;
    try {
      rv_data.value(std::stod(value, &pos));
      rv_data.unit(value.substr(pos));
      patient.ResidualVolume(rv_data);
    } catch (std::exception e) {
      rValue = false;
    }
  }
  return rValue;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

} //end namespace biogears