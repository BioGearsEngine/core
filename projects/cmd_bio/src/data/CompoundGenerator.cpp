#include "CompoundGenerator.h"
#include "../utils/string-helpers.h"

#include <biogears/schema/cdm/Substance.hxx>
#include <fstream>
#include <iostream>

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
//! \brief Iterates through _compounds and saves an xml of each compound
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

    } catch (std::exception e) {
      rValue = false;
      std::cout << e.what() << std::endl;
    }
  }
  return rValue;
}
//-----------------------------------------------------------------------------
//!
//! \brief Reads Compounds.csv and populates _compounds with compound objects for each compound in the csv
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
	} else if ("Component Name" == lineItr->first) {
	  rValue &= process_substance(lineItr);
	  lineItr += 1;
	}
	
  }
  return rValue;
}
//-----------------------------------------------------------------------------

//!
//! \brief prints out contents of _compounds
//! 
void CompoundGenerator::print() const
{
  for (auto& compound : _compounds) {
    std::cout << compound;
  }
}
//-----------------------------------------------------------------------------
//!
//! \brief Called by parse in order to write nested tags into the compound objects of _compounds
//! \param itr, iterator for cells of the Compounds.csv document
//! \return bool rValue, true if there were no exceptions, false otherwise
//! 
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
		con_data.value(std::stod(value2,&pos));
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