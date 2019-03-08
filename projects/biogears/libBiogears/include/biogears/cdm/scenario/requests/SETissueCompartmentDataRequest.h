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
#include <biogears/cdm/scenario/requests/SECompartmentDataRequest.h>

namespace biogears {
class SESubstance;
class SESubstanceManager;
class SEDataRequestManager;

class BIOGEARS_API SETissueCompartmentDataRequest : public SECompartmentDataRequest {
  friend class SEDataRequestManager;

protected:
  SETissueCompartmentDataRequest(const SEDecimalFormat* dfault = nullptr);

public:
  virtual ~SETissueCompartmentDataRequest();

  virtual void Clear(); //clear memory

  virtual bool Load(const CDM::TissueCompartmentDataRequestData& in);
  virtual CDM::TissueCompartmentDataRequestData* Unload() const;

protected:
  virtual void Unload(CDM::TissueCompartmentDataRequestData& data) const;

public:
  virtual size_t HashCode();
};
}