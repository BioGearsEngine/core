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
#include <biogears/cdm/scenario/requests/SETissueCompartmentDataRequest.h>

#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/utils/EnumHashSpecialization.h>

namespace biogears {
SETissueCompartmentDataRequest::SETissueCompartmentDataRequest(const SEDecimalFormat* dfault)
  : SECompartmentDataRequest(dfault)
{
}

SETissueCompartmentDataRequest::~SETissueCompartmentDataRequest()
{
  Clear();
}

void SETissueCompartmentDataRequest::Clear()
{
  SECompartmentDataRequest::Clear();
}

size_t SETissueCompartmentDataRequest::HashCode()
{
  size_t h = SECompartmentDataRequest::HashCode();
  return h;
}

bool SETissueCompartmentDataRequest::Load(const CDM::TissueCompartmentDataRequestData& in)
{
  SECompartmentDataRequest::Load(in);
  return true;
}

CDM::TissueCompartmentDataRequestData* SETissueCompartmentDataRequest::Unload() const
{
  CDM::TissueCompartmentDataRequestData* data = new CDM::TissueCompartmentDataRequestData();
  Unload(*data);
  return data;
}

void SETissueCompartmentDataRequest::Unload(CDM::TissueCompartmentDataRequestData& data) const
{
  SECompartmentDataRequest::Unload(data);
}
}