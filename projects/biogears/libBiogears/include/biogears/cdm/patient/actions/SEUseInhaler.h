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

#include <biogears/cdm/patient/actions/SEConsciousRespirationCommand.h>
#include <biogears/cdm/enums/SEPatientActionsEnums.h>

#include <random>

namespace biogears {
class SESubstance;
class SEConsciousRespiration;
class BIOGEARS_API SEUseInhaler : public SEConsciousRespirationCommand {
  friend class SEConsciousRespiration;

public:
  SEUseInhaler();
  virtual ~SEUseInhaler();

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual void ToString(std::ostream& str) const;

  bool operator==( const SEUseInhaler& rhs) const;
  bool operator!=( const SEUseInhaler& rhs) const;

protected:
};
}