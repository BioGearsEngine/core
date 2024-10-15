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
#include <biogears/cdm/enums/SEPatientActionsEnums.h>
#include <biogears/cdm/scenario/SEAction.h>

#include <random>

namespace biogears {
class BIOGEARS_API SEPatientAction : public SEAction {
public:
  SEPatientAction();
  virtual ~SEPatientAction();

  virtual void Clear(); // clear memory

  virtual bool IsValid() const;

  bool operator==(const SEPatientAction& rhs) const;
  bool operator!=(const SEPatientAction& rhs) const;

public:
  using SEAction::ToString;
  virtual void ToString(std::ostream& str) const = 0;
};
}