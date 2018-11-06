#ifndef BIOGEARS_COMPOUND_GENERATOR_H
#define BIOGEARS_COMPOUND_GENERATOR_H

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

#include "CSVToXMLConvertor.h"
namespace biogears
{

  class CompoundGenerator : public CSVToXMLConvertor
  {
  public:
    CompoundGenerator(std::string path = "");
    ~CompoundGenerator() override;

    bool parse() override;
    bool save() const override;
    void print() const override;
  protected:
    bool process_substance(CSV_RowItr itr);
  private:
    std::vector<mil::tatrc::physiology::datamodel::SubstanceCompoundData> _compounds;
  };
} //namespace biogears

#endif //BIOGEARS_COMPOUND_GENERATOR_H