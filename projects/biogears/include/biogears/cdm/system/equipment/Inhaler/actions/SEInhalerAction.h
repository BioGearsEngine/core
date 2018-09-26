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
#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/schema/cdm/InhalerActions.hxx>

namespace biogears {
class BIOGEARS_API SEInhalerAction : public SEAction {
public:
  SEInhalerAction();
  virtual ~SEInhalerAction();

  virtual void Clear(); // Deletes all members

  virtual bool IsValid() const;

  virtual bool Load(const CDM::InhalerActionData& in);
  virtual CDM::InhalerActionData* Unload() const;

protected:
  virtual void Unload(CDM::InhalerActionData& data) const;

public:
  virtual void ToString(std::ostream& str) const = 0;
};
}