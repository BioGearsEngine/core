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

bool CompoundGenerator::save() const
{
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
      std::cout << e.what() << std::endl;
    }
  }
  return false;
}
//-----------------------------------------------------------------------------
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

void CompoundGenerator::print() const
{
  for (auto& compound : _compounds) {
    std::cout << compound;
  }
}
//-----------------------------------------------------------------------------
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
		con_data.value(std::stod(value2));
		con_data.unit(value2.substr(pos));
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