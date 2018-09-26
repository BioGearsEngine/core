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
#include <biogears/cdm/scenario/requests/SEGasCompartmentDataRequest.h>

#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/utils/EnumHashSpecialization.h>

namespace biogears {
SEGasCompartmentDataRequest::SEGasCompartmentDataRequest(const SEDecimalFormat* dfault)
  : SECompartmentSubstanceDataRequest(dfault)
{
}

SEGasCompartmentDataRequest::~SEGasCompartmentDataRequest()
{
  Clear();
}

bool SEGasCompartmentDataRequest::Load(const CDM::GasCompartmentDataRequestData& in, const SESubstanceManager& substances)
{
  SECompartmentSubstanceDataRequest::Load(in, substances);
  return true;
}

CDM::GasCompartmentDataRequestData* SEGasCompartmentDataRequest::Unload() const
{
  CDM::GasCompartmentDataRequestData* data = new CDM::GasCompartmentDataRequestData();
  Unload(*data);
  return data;
}

void SEGasCompartmentDataRequest::Unload(CDM::GasCompartmentDataRequestData& data) const
{
  SECompartmentSubstanceDataRequest::Unload(data);
}
}