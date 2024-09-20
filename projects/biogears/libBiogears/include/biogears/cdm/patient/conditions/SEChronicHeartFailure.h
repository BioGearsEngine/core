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
#include <biogears/exports.h>
#include <biogears/cdm/patient/conditions/SEPatientCondition.h>

namespace biogears {
class BIOGEARS_API SEChronicHeartFailure : public SEPatientCondition {
public:
  SEChronicHeartFailure();
  virtual ~SEChronicHeartFailure();

  virtual void Clear() override; //clear memory

  virtual bool IsValid() const override;


  bool operator==(SECondition const& rhs) const override = 0 ;
  bool operator!=(SECondition const& rhs) const override = 0 ;

public:
  virtual void ToString(std::ostream& str) const override = 0 ;

protected:
};
}
