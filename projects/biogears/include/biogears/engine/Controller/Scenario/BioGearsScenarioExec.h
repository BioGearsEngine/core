/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#pragma once
#include <biogears/cdm/scenario/SEScenarioExec.h>

namespace biogears {
/**
* @brief Overloaded scenario exector to run a %BioGears specific scenario
*/
class BIOGEARS_API BioGearsScenarioExec : public SEScenarioExec {
public:
  BioGearsScenarioExec(PhysiologyEngine& engine);
  virtual ~BioGearsScenarioExec();

  virtual bool Execute(const std::string& scenarioFile, const std::string& resultsFile, SEScenarioCustomExec* cExec = nullptr);
  virtual bool Execute(const SEScenario& scenario, const std::string& resultsFile, SEScenarioCustomExec* cExec = nullptr);

protected:
};
}