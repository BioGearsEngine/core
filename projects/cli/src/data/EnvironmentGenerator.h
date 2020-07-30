#ifndef BIOGEARS_ENVIRONMENT_GENERATOR_H
#define BIOGEARS_ENVIRONMENT_GENERATOR_H

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

#include <biogears/schema/biogears/BioGearsConfiguration.hxx>
#include "CSVToXMLConvertor.h"
namespace biogears
{

  struct Environment
  {

  };

  class EnvironmentGenerator : public CSVToXMLConvertor
  {
  public:
    EnvironmentGenerator(std::string path = "");
    ~EnvironmentGenerator() override;

    bool parse() override;
    bool save() const override;
    void print() const override;
  protected:
    bool process_ambientgasdata(CSV_RowItr itr);
    bool process_ambientaerosoldata(CSV_RowItr itr);
    bool process(const std::string& name, const std::string& value, mil::tatrc::physiology::datamodel::EnvironmentalConditionsData& environment);
	
  private:
    std::vector<Environment> _input;
    std::vector<mil::tatrc::physiology::datamodel::EnvironmentalConditionsData> _environments;
  };
} //namespace biogears

#endif //BIOGEARS_ENVIRONMENT_GENERATOR_H