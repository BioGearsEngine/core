#ifndef BIOGEARS_STABILIZATION_GENERATOR_H
#define BIOGEARS_STABILIZATION_GENERATOR_H

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

#include <biogears/schema/cdm/EngineConfiguration.hxx>
#include "CSVToXMLConvertor.h"

namespace biogears
{

  struct Stabilization
  {

  };

  class StabilizationGenerator : public CSVToXMLConvertor
  {
  public:
    StabilizationGenerator(std::string path = "");
    ~StabilizationGenerator() override;

    bool parse() override;
    bool save() const override;
    void print() const override;
  protected:
    bool process_RestingStabilizationCriteria(CSV_RowItr);
    bool process_FeedbackStabilizationCriteria(CSV_RowItr);
    bool process_ConditionStabilization(CSV_RowItr);
  private:
    mil::tatrc::physiology::datamodel::PhysiologyEngineDynamicStabilizationData _dynamic;
    mil::tatrc::physiology::datamodel::PhysiologyEngineTimedStabilizationData _timed;
  };
} //namespace biogears

#endif //BIOGEARS_STABILIZATION_GENERATOR_H
