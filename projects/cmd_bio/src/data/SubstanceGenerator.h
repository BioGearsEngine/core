#ifndef BIOGEARS_SUSTANCE_GENERATOR_H
#define BIOGEARS_SUSTANCE_GENERATOR_H

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

#include <string>
#include <vector>

#include "CSVToXMLConvertor.h"
#include <biogears/schema/cdm/Substance.hxx>
namespace biogears {

class SubstanceGenerator : public CSVToXMLConvertor {
public:
  using XSD_Type = mil::tatrc::physiology::datamodel::SubstanceData;
  using PDMOD_Type = mil::tatrc::physiology::datamodel::PharmacodynamicModifierData;

  SubstanceGenerator(std::string path = "");
  ~SubstanceGenerator() override;

  bool parse() override;
  bool save() const override;
  void print() const override;

protected:
  bool process(const std::string& name, const std::string& value, mil::tatrc::physiology::datamodel::SubstanceData& substance);
  bool process_aerosol(CSV_RowItr);
  bool process_clearance(CSV_RowItr);
  bool process_pharmacokinetics(CSV_RowItr);
  bool process_pharmacodynamics(CSV_RowItr);
  bool process_tissues(CSV_RowItr);
  PDMOD_Type set_PDModifier(PDMOD_Type modifier, std::string value);

private:
  std::vector<XSD_Type> _substances;
};
} //namespace biogears

#endif //BIOGEARS_SUSTANCE_GENERATOR_H