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

#include "CompoundGenerator.h"

#include <biogears/schema/cdm/Substance.hxx>
#include <fstream>
#include <iostream>

#include <biogears/string/manipulation.h>

namespace biogears {

CompoundGenerator::CompoundGenerator(std::string path)
  : CSVToXMLConvertor(path, "Compounds.csv")
{
}
//-----------------------------------------------------------------------------

CompoundGenerator::~CompoundGenerator()
{
}
//-----------------------------------------------------------------------------

//!
//! \brief Iterates through _compounds and saves an xml of each SubstanceCompoundData object
//! \return bool rValue, true if there were no exceptions, false otherwise
//!
bool CompoundGenerator::save() const
{
  bool rValue = true;
  for (auto& compound : _compounds) {
    xml_schema::namespace_infomap info;
    info[""].name = "uri:/mil/tatrc/physiology/datamodel";
    info[""].schema = "BioGears.xsd";

    try {
      std::ofstream file;
      file.open("substances/" + compound.Name() + ".xml");
      mil::tatrc::physiology::datamodel::SubstanceCompound(file, compound, info);
      file.close();
      std::cout << "Saved substances/" + compound.Name() + ".xml"
                << "\n";

    } catch (std::exception e) {
      rValue = false;
      std::cout << e.what() << std::endl;
    }
  }
  return rValue;
}
//-----------------------------------------------------------------------------
//!
//! \brief reads Compounds.csv and populates _compounds with SubstanceCompoundData objects for each compound in the csv`
//! \return bool rValue, true if there were no issues reading from Compounds.csv, false otherwise
//!
bool CompoundGenerator::parse()
{
  namespace CDM = mil::tatrc::physiology::datamodel;
  bool rValue = true;
  read_csv();
  for (auto lineItr = begin(); lineItr != end(); ++lineItr) {
    if ("Compound Name" == lineItr->first) {
      for (auto name : lineItr->second) {
        CDM::SubstanceCompoundData data;
        data.Name(name);
        _compounds.push_back(std::move(data));
      }
    } else if ("Classification" == lineItr->first) {
      for (size_t index = 0; index < _compounds.size() && index < lineItr->second.size(); ++index) {
        if ("WholeBlood" == lineItr->second[index]) {
          _compounds[index].Classification(CDM::SubstanceCompoundData::Classification_type::WholeBlood);
        } else if ("" == lineItr->second[index]) {
			//Do nothing, classification is optional
        } else {
          rValue = false;
		}
      }
    } else if ("BloodRHFactor" == lineItr->first) {
      for (size_t index = 0; index < _compounds.size() && index < lineItr->second.size(); ++index) {
        if ("TRUE" == lineItr->second[index] || "true" == lineItr->second[index]) {
          _compounds[index].BloodRHFactor(true);
        } else if ("FALSE" == lineItr->second[index] || "false" == lineItr->second[index]) {
          _compounds[index].BloodRHFactor(false);
        } else if ("" == lineItr->second[index]) {
			//Do nothing, blood rh factor is optional
		} else {
          rValue = false;
        }
      }
    } 
	else if ("Component Name" == lineItr->first) {
      rValue &= process_substance(lineItr);
      lineItr += 1;
    }
  }
  return rValue;
}
//-----------------------------------------------------------------------------

//!
//! \brief prints out contents of SubstanceCompoundData objects in _compounds
//! \details Note that the '<<' operator is overloaded for SubstanceCompoundData objects in this case, and will display each member of the objects
void CompoundGenerator::print() const
{
  for (auto& compound : _compounds) {
    std::cout << compound;
  }
}
//-----------------------------------------------------------------------------
//!
//! \brief called by parse in order to write nested tags into the SubstanceCompoundData objects of _compounds
//! \param itr : iterator for cells of the Compounds.csv document
//! \return bool rValue, true if there were no exceptions, false otherwise
//! \details since nested tags are dependent on multiple rows of the csv document it is necessary to write a method such as this one
//!          for all tags which nest other xml tags.
bool CompoundGenerator::process_substance(CSV_RowItr itr)
{
  namespace CDM = mil::tatrc::physiology::datamodel;
  bool rValue = true;
  size_t index = 0;
  for (auto& compound : _compounds) {
    auto& value1 = (itr)->second[index];
    auto& value2 = (itr + 1)->second[index];
    if (!value1.empty()) {
      size_t pos = 0;
      CDM::SubstanceCompoundData::Component_type com_data;
      CDM::SubstanceCompoundData::Component_type::Concentration_type con_data;
      try {
        com_data.Name(value1);
        con_data.value(std::stod(value2, &pos));
        con_data.unit(trim(value2.substr(pos)));
        com_data.Concentration(con_data);
        compound.Component().push_back(com_data);
      } catch (std::exception e) {
        rValue = false;
      }
    }
    ++index;
  }
  return rValue;
}

} // end namespace biogears
