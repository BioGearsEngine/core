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

#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/equipment/Inhaler/SEInhaler.h>
#include <biogears/cdm/system/equipment/Inhaler/actions/SEInhalerConfiguration.h>

namespace biogears {
class BIOGEARS_API SEInhalerActionCollection : public Loggable {
public:
  SEInhalerActionCollection(SESubstanceManager&);
  ~SEInhalerActionCollection();

  void Clear();

  void Unload(std::vector<CDM::ActionData*>& to);

  bool ProcessAction(const SEInhalerAction& action);
  bool ProcessAction(const CDM::InhalerActionData& action);

  // STATE ACTION
  bool HasConfiguration() const;
  SEInhalerConfiguration* GetConfiguration() const;
  void RemoveConfiguration();

protected:
  bool IsValid(const SEInhalerAction& action);

  SEInhalerConfiguration* m_Configuration;
  // General
  SESubstanceManager& m_Substances;
  std::stringstream m_ss;
};
}