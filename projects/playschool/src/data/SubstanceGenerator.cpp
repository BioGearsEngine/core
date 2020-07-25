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

#include <biogears/string/manipulation.h>
#include <fstream>
#include <iostream>

namespace biogears {
//-----------------------------------------------------------------------------
//!
//! \brief
//! \param path -- std:string -- The Path to the CSV document folder will be appened with a delimiter.
//!
SubstanceGenerator::SubstanceGenerator(std::string path)
  : CSVToXMLConvertor(path, "Substances.csv")
{
}
//-----------------------------------------------------------------------------
//!
//! \brief
//!
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
      rValue &= process_aerosol(++lineItr);
      lineItr += 2;
    } else if ("Clearance (all or none)" == lineItr->first) {
      rValue &= process_clearance(++lineItr);
      lineItr += 14;
    } else if ("Pharmacokinetics (all or none)" == lineItr->first) {
      rValue &= process_pharmacokinetics(++lineItr);
      lineItr += 7;
    } else if ("Pharmacodynamics (all or none)" == lineItr->first) {
      rValue &= process_pharmacodynamics(++lineItr);
      lineItr += 17;
    } else if (lineItr->first.find("Tissue Pharmacokinetics") != std::string::npos) {
      rValue &= process_tissues(lineItr);
      lineItr += 1;
    } else {
      for (size_t index = 0; index < _substances.size() && index < lineItr->second.size(); ++index) {
        process(lineItr->first, lineItr->second[index], _substances[index]);
      }
    }
  }
  return rValue;
}
//-----------------------------------------------------------------------------
//!
//! \brief Saves xml for each SubstanceData object in _substances
//! \return
//!
bool SubstanceGenerator::save() const
{
  for (auto& substance : _substances) {
    xml_schema::namespace_infomap info;
    info[""].name = "uri:/mil/tatrc/physiology/datamodel";
    info[""].schema = "BioGears.xsd";

    try {
      std::ofstream file;
      file.open("substances/" + substance.Name() + ".xml");
      Substance(file, substance, info);
      file.close();
      std::cout << "Saved substances/" + substance.Name() + ".xml"
                << "\n";

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
//!
//! \brief Checks first cell of csv row and sets corresponding data of SubstanceData object
//! \param name first cell of row
//! \param value another cell of same row
//! \param substance SubstanceData object
//! \return
//!
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
    } else if ("WholeBlood" == value) {
      substance.Classification(SubstanceData::Classification_type::WholeBlood);
    } else if ("" == value) {
      //Classification is Optional and need not be set
    } else {
      rValue = false;
    }
  } else if ("Density" == name) {
    SubstanceData::Density_type density;
    size_t pos = 0;
    try {
      density.value(std::stod(value, &pos));
      density.unit(trim(value.substr(pos)));
      substance.Density(density);
    } catch (std::exception e) {
      rValue = false;
    }

  } else if ("MaximumDiffusionFlux" == name) {
    SubstanceData::MaximumDiffusionFlux_type diffusion;
    size_t pos = 0;
    try {
      diffusion.value(std::stod(value, &pos));
      diffusion.unit(trim(value.substr(pos)));
      substance.MaximumDiffusionFlux(diffusion);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("MichaelisCoefficient" == name) {
    SubstanceData::MichaelisCoefficient_type type;
    size_t pos = 0;
    try {
      type.value(std::stod(value, &pos));
      //type.unit(trim(value.substr(pos)));
      substance.MichaelisCoefficient(type);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("MembraneResistance" == name) {
    SubstanceData::MembraneResistance_type type;
    size_t pos = 0;
    try {
      type.value(std::stod(value, &pos));
      type.unit(trim(value.substr(pos)));
      substance.MembraneResistance(type);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("MolarMass" == name) {
    SubstanceData::MolarMass_type type;
    size_t pos = 0;
    try {
      type.value(std::stod(value, &pos));
      type.unit(trim(value.substr(pos)));
      substance.MolarMass(type);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("RelativeDiffusionCoefficient" == name) {
    SubstanceData::RelativeDiffusionCoefficient_type type;
    size_t pos = 0;
    try {
      type.value(std::stod(value, &pos));
      //type.unit(trim(value.substr(pos))); //No Unit
      substance.RelativeDiffusionCoefficient(type);
    } catch (std::exception e) {
      rValue = false;
    }
  } else if ("SolubilityCoefficient" == name) {
    SubstanceData::SolubilityCoefficient_type type;
    size_t pos = 0;
    try {

      type.value(std::stod(value, &pos));
      type.unit(trim(value.substr(pos)));
      substance.SolubilityCoefficient(type);
    } catch (std::exception e) {
      rValue = false;
    }
  }
  return rValue;
}
//-----------------------------------------------------------------------------
//!
//! \brief Reads in data for the xml tags nested inside the aerosol tag
//! \param itr, iterator for the data structure representation of csv rows
//! \return
//! \details since nested tags are dependent on multiple rows of the csv document it is necessary to write a method such as this one
//!          for all tags which nest other xml tags.
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

        bron_data.value(std::stod(value));
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

        infl_data.value(std::stod(value));
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
//!
//! \brief Reads in data for xml tags nested in the clearance tag
//! \param itr, iterator for the data structure representation of csv rows
//! \return
//!
bool SubstanceGenerator::process_clearance(CSV_RowItr itr)
{
  namespace CDM = mil::tatrc::physiology::datamodel;

  //TODO:Bounds Checking
  //TODO:Exceptiom Handling
  bool rValue = true;
  bool default_clearance = true;
  size_t index = 0;
  size_t pos = 0;
  for (auto& substance : _substances) {
    CDM::SubstanceClearanceData data;
    default_clearance = true;

    auto& value = (itr)->second[index];
    if (!value.empty()) {
      CDM::SubstanceClearanceData::Systemic_type systemic_data;

      CDM::SubstanceClearanceData::Systemic_type::FractionExcretedInFeces_type feif_data;
      CDM::SubstanceClearanceData::Systemic_type::FractionUnboundInPlasma_type fuip_data;
      CDM::SubstanceClearanceData::Systemic_type::IntrinsicClearance_type ic_data;
      CDM::SubstanceClearanceData::Systemic_type::RenalClearance_type rc_data;
      CDM::SubstanceClearanceData::Systemic_type::SystemicClearance_type sc_data;
      try {
        feif_data.value(std::stod(value));
        systemic_data.FractionExcretedInFeces(feif_data);

        value = (itr + 1)->second[index];
        fuip_data.value(std::stod(value));
        systemic_data.FractionUnboundInPlasma(fuip_data);

        value = (itr + 2)->second[index];
        ic_data.value(std::stod(value, &pos));
        ic_data.unit(trim(value.substr(pos)));
        systemic_data.IntrinsicClearance(ic_data);

        value = (itr + 3)->second[index];
        rc_data.value(std::stod(value, &pos));
        rc_data.unit(trim(value.substr(pos)));
        systemic_data.RenalClearance(rc_data);

        value = (itr + 4)->second[index];
        sc_data.value(std::stod(value, &pos));
        sc_data.unit(trim(value.substr(pos)));
        systemic_data.SystemicClearance(sc_data);

        data.Systemic(systemic_data);
      } catch (std::exception e) {
        rValue = false;
      }
    }

    value = (itr + 6)->second[index];
    CDM::SubstanceClearanceData::RenalDynamics_type renal_data;
    if (!value.empty()) {
      default_clearance = false;
      try {
        renal_data.Clearance(std::stod(value, &pos));
        renal_data.Clearance()->unit(trim(value.substr(pos)));
        data.RenalDynamics(renal_data);
      } catch (std::exception e) {
        rValue = false;
      }
    } else {
      value = (itr + 8)->second[index];
      if (!value.empty()) {
        default_clearance = false;
        CDM::SubstanceClearanceData::RenalDynamics_type::Regulation_type regulation_data;
        regulation_data.ChargeInBlood(value);
        try {
          value = (itr + 9)->second[index];
          regulation_data.FractionUnboundInPlasma(std::stod(value));
          value = (itr + 10)->second[index];
          regulation_data.ReabsorptionRatio(std::stod(value));
          value = (itr + 11)->second[index];

          CDM::SubstanceClearanceData::RenalDynamics_type::Regulation_type::TransportMaximum_type transport_data;
          regulation_data.TransportMaximum(transport_data);
          regulation_data.TransportMaximum().value(std::stod(value, &pos));
          regulation_data.TransportMaximum().unit(trim(value.substr(pos)));

          renal_data.Regulation(regulation_data);
          data.RenalDynamics(renal_data);
        } catch (std::exception e) {
          rValue = false;
        }
      }
    }

    value = (itr + 13)->second[index];
    CDM::SubstanceClearanceData::CellRegulation_type cellregulation_data;
    if (!value.empty()) {
      default_clearance = false;
      CDM::SubstanceClearanceData::CellRegulation_type::CellDeathRate_type crDeathRate_data;
      CDM::SubstanceClearanceData::CellRegulation_type::CellBirthRate_type crBirthRate_data;
      try {
        crDeathRate_data.value(std::stod(value, &pos));
        crDeathRate_data.unit(trim(value.substr(pos)));
        cellregulation_data.CellDeathRate(crDeathRate_data);

        value = (itr + 14)->second[index];
        crBirthRate_data.value(std::stod(value, &pos));
        crBirthRate_data.unit(trim(value.substr(pos)));
        cellregulation_data.CellBirthRate(crBirthRate_data);

        data.CellRegulation(cellregulation_data);
      } catch (std::exception e) {
        rValue = false;
      }
    }

    if (default_clearance) {
      CDM::SubstanceClearanceData::RenalDynamics_type renal_data;
      renal_data.Clearance(0.0);
      renal_data.Clearance()->unit("mL/min kg");
      data.RenalDynamics(renal_data);
    }
    substance.Clearance(data);
    ++index;
  }
  return rValue;
}
//-----------------------------------------------------------------------------
//!
//! \brief Reads in data for the xml tags nested inside the pharmacokinetics tag
//! \param itr, iterator for the data structure representation of csv rows
//! \return
//!
bool SubstanceGenerator::process_pharmacokinetics(CSV_RowItr itr)
{
  namespace CDM = mil::tatrc::physiology::datamodel;

  //TODO:Bounds Checking
  //TODO:Exceptiom Handling
  bool rValue = true;
  bool apply_results = false;
  size_t index = 0;
  for (auto& substance : _substances) {
    CDM::SubstanceData::Pharmacokinetics_type data;
    CDM::SubstanceData::Pharmacokinetics_type::Physicochemicals_type phys_type;

    auto& value = (itr)->second[index];
    if (!value.empty()) {
      try {
        size_t secondPKA = value.find(';'); //If two pKa's are present, they will be separated by a ;
        if (secondPKA == value.npos) {
          phys_type.AcidDissociationConstant().push_back(std::stod(value));
        } else {
          std::string pKA1 = trim(value.substr(0, secondPKA));
          std::string pKA2 = trim(value.substr(secondPKA + 1, value.npos));
          phys_type.AcidDissociationConstant().push_back(std::stod(pKA1));
          phys_type.AcidDissociationConstant().push_back(std::stod(pKA2));
        }
        value = (itr + 1)->second[index];
        phys_type.BindingProtein(value);
        value = (itr + 2)->second[index];
        phys_type.BloodPlasmaRatio(std::stod(value));
        value = (itr + 3)->second[index];
        phys_type.FractionUnboundInPlasma(std::stod(value));
        value = (itr + 4)->second[index];
        phys_type.IonicState(value);
        value = (itr + 5)->second[index];
        phys_type.LogP(std::stod(value));
        value = (itr + 6)->second[index];
        phys_type.HydrogenBondCount(std::stod(value));
        value = (itr + 7)->second[index];
        phys_type.PolarSurfaceArea(std::stod(value));

        data.Physicochemicals(phys_type);
        substance.Pharmacokinetics(data);
      } catch (std::exception e) {
        rValue = false;
      }
    }
    ++index;
  }
  return rValue;
}
//-------------------------------------------------------------------------------
//!
//! \brief Reads in data for the xml tags nested inside the pharmacodynamics tag
//! \param itr, iterator for the data structure representation of csv rows
//! \return
//!
bool SubstanceGenerator::process_pharmacodynamics(CSV_RowItr itr)
{
  namespace CDM = mil::tatrc::physiology::datamodel;

  //TODO:Bounds Checking
  //TODO:Exceptiom Handling
  bool rValue = true;
  bool apply_results = false;
  size_t index = 0;
  for (auto& substance : _substances) {
    CDM::SubstanceData::Pharmacodynamics_type data;

    auto& value = (itr)->second[index];
    if (!value.empty()) {
      try {
        size_t pos = 0;
        CDM::PharmacodynamicModifierData pdMod;

        CDM::SubstanceData::Pharmacodynamics_type::EffectSiteRateConstant_type esrc_type;
        esrc_type.value(std::stod(value, &pos));
        esrc_type.unit(trim(value.substr(pos)));
        data.EffectSiteRateConstant(esrc_type);
        value = (itr + 1)->second[index];
        data.EMaxShapeParameter(std::stod(value));
        value = (itr + 2)->second[index];
        CDM::SubstanceData::Pharmacodynamics_type::AntibacterialEffect_type anti;
        anti.value(std::stod(value, &pos));
        anti.unit(trim(value.substr(pos)));
        data.AntibacterialEffect(anti);
        value = (itr + 3)->second[index];
        data.Bronchodilation(set_PDModifier(pdMod, value));
        value = (itr + 4)->second[index];
        data.CentralNervousModifier(set_PDModifier(pdMod, value));
        value = (itr + 5)->second[index];
        data.DiastolicPressureModifier(set_PDModifier(pdMod, value));
        value = (itr + 6)->second[index];
        data.FeverModifier(set_PDModifier(pdMod, value));
        value = (itr + 7)->second[index];
        data.HeartRateModifier(set_PDModifier(pdMod, value));
        value = (itr + 8)->second[index];
        data.HemorrhageModifier(set_PDModifier(pdMod, value));
        value = (itr + 9)->second[index];
        data.NeuromuscularBlock(set_PDModifier(pdMod, value));
        value = (itr + 10)->second[index];
        data.PainModifier(set_PDModifier(pdMod, value));
        value = (itr + 11)->second[index];
        data.PupilReactivityModifier(set_PDModifier(pdMod, value));
        value = (itr + 12)->second[index];
        data.PupilSizeModifier(set_PDModifier(pdMod, value));
        value = (itr + 13)->second[index];
        data.RespirationRateModifier(set_PDModifier(pdMod, value));
        value = (itr + 14)->second[index];
        data.Sedation(set_PDModifier(pdMod, value));
        value = (itr + 15)->second[index];
        data.SystolicPressureModifier(set_PDModifier(pdMod, value));
        value = (itr + 16)->second[index];
        data.TidalVolumeModifier(set_PDModifier(pdMod, value));
        value = (itr + 17)->second[index];
        data.TubularPermeabilityModifier(set_PDModifier(pdMod, value));
        substance.Pharmacodynamics(data);
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
//! \brief Reads in data for the xml tags nested inside the tissues tag
//! \param itr, iterator for the data structure representation of csv rows
//! \return
//!
bool SubstanceGenerator::process_tissues(CSV_RowItr itr)
{
  namespace CDM = mil::tatrc::physiology::datamodel;

  //TODO:Bounds Checking
  //TODO:Exceptiom Handling
  bool rValue = true;
  bool apply_results = false;
  size_t index = 0;

  for (auto& substance : _substances) {
    auto& value = (itr + 1)->second[index];
    if (!value.empty()) {
      std::unique_ptr<CDM::SubstanceData::Pharmacokinetics_type> data;
      std::unique_ptr<CDM::SubstanceData::Pharmacokinetics_type::TissueKinetics_sequence> tissues;
      if (substance.Pharmacokinetics().present()) {
        data = substance.Pharmacokinetics().detach();
      } else {
        data = std::make_unique<CDM::SubstanceData::Pharmacokinetics_type>();
        tissues = std::make_unique<CDM::SubstanceData::Pharmacokinetics_type::TissueKinetics_sequence>();
      }

      CDM::SubstanceData::Pharmacokinetics_type::TissueKinetics_type tissue_data;
      try {
        auto name = split(itr->first, ' ');
        tissue_data.Name(name[0]);
        value = (itr + 1)->second[index];
        tissue_data.PartitionCoefficient(std::stod(value));
        data->TissueKinetics().push_back(tissue_data);
        substance.Pharmacokinetics(std::move(data));
      } catch (std::exception e) {
        rValue = false;
      }
    }
    ++index;
  }
  return rValue;
}

using PDMod_Type = mil::tatrc::physiology::datamodel::PharmacodynamicModifierData;
PDMod_Type SubstanceGenerator::set_PDModifier(PDMod_Type modifier, std::string value)
{
  namespace CDM = mil::tatrc::physiology::datamodel;
  //PD Modifier value has format "[EMax;EC50]"    EC50 has a value and a unit (ScalarMassPerVolume)
  //Get rid of leading and trailing braces
  value.erase(value.begin());
  value.erase(value.end() - 1);
  //Split string at token (;)
  auto modifier_data = split(value, ';');
  //Set EMax to first element
  modifier.EMax(std::stod(modifier_data[0]));
  //Set EC50 value and unit
  CDM::ScalarMassPerVolumeData ec50;
  size_t pos = 0;
  ec50.value(std::stod(trim(modifier_data[1]), &pos));
  ec50.unit(trim(modifier_data[1].substr(pos)));
  modifier.EC50(ec50);

  return modifier;
}

}
