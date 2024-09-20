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
#include <biogears/cdm/scenario/SECondition.h>

namespace biogears {
namespace io {
  class EnvironmentConditions;
}
class BIOGEARS_API SEEnvironmentCondition : public SECondition {
  friend io::EnvironmentConditions;

public:
  SEEnvironmentCondition();
  virtual ~SEEnvironmentCondition();

  using SECondition::Clear;

  using SECondition::IsValid;

  bool operator==(SECondition const&) const override = 0;
  bool operator!=(SECondition const&) const override = 0;

public:
  virtual void ToString(std::ostream& str) const  override = 0;
};
}
