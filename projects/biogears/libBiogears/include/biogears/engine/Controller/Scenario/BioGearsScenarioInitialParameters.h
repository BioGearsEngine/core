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

#include <biogears/cdm/scenario/SEScenarioInitialParameters.h>
#include <biogears/engine/Controller/BioGearsConfiguration.h>

namespace biogears {
class BioGearsScenario;

/**
* @brief Overloaded scenario that creates a %BioGears configuration as its configuration object
*/
class BIOGEARS_API BioGearsScenarioInitialParameters : public SEScenarioInitialParameters {
protected:
  friend BioGearsScenario;

  BioGearsScenarioInitialParameters(SESubstanceManager& subMgr);
  virtual ~BioGearsScenarioInitialParameters();

  virtual BioGearsConfiguration& GetConfiguration();
  virtual const BioGearsConfiguration* GetConfiguration() const;
  virtual void SetConfiguration(const BioGearsConfiguration& config);

protected:
};
}