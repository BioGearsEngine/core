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

#include <biogears/schema/cdm/Substance.hxx>
#include "CVSToXMLConvertor.h"
namespace biogears
{

  class SubstanceGenerator : public CSVToXMLConvertor
  {
  public:
    SubstanceGenerator(std::string path);
    ~SubstanceGenerator() override;

    bool parse() override;
    bool save() const override;
    void print() const override;
  protected:
    bool process(const std::string& name, const std::string& value, mil::tatrc::physiology::datamodel::SubstanceData& substance);
  private:
    std::vector<mil::tatrc::physiology::datamodel::SubstanceData> _substances;
  };
} //namespace biogears

#endif //BIOGEARS_SUSTANCE_GENERATOR_H