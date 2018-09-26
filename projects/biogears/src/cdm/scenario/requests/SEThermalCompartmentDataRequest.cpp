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
#include <biogears/cdm/scenario/requests/SEThermalCompartmentDataRequest.h>

#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/utils/EnumHashSpecialization.h>

namespace biogears {
SEThermalCompartmentDataRequest::SEThermalCompartmentDataRequest(const SEDecimalFormat* dfault)
  : SECompartmentDataRequest(dfault)
{
}

SEThermalCompartmentDataRequest::~SEThermalCompartmentDataRequest()
{
  Clear();
}

bool SEThermalCompartmentDataRequest::Load(const CDM::ThermalCompartmentDataRequestData& in)
{
  SECompartmentDataRequest::Load(in);
  return true;
}

CDM::ThermalCompartmentDataRequestData* SEThermalCompartmentDataRequest::Unload() const
{
  CDM::ThermalCompartmentDataRequestData* data = new CDM::ThermalCompartmentDataRequestData();
  Unload(*data);
  return data;
}

void SEThermalCompartmentDataRequest::Unload(CDM::ThermalCompartmentDataRequestData& data) const
{
  SECompartmentDataRequest::Unload(data);
}
}