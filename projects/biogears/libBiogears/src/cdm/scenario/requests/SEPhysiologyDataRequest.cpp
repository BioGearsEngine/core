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

#include <biogears/cdm/scenario/requests/SEPhysiologyDataRequest.h>

// Private Include
#include <io/cdm/Scenario.h>

namespace biogears {
SEPhysiologyDataRequest::SEPhysiologyDataRequest(const SEDecimalFormat* dfault)
  : SEDataRequest(dfault)
{
}

SEPhysiologyDataRequest::~SEPhysiologyDataRequest()
{
  Clear();
}

void SEPhysiologyDataRequest::Clear()
{
  SEDataRequest::Clear();
}

bool SEPhysiologyDataRequest::Load(const CDM::PhysiologyDataRequestData& in)
{
  io::Scenario::UnMarshall(in, *this);
  return true;
}

CDM::PhysiologyDataRequestData* SEPhysiologyDataRequest::Unload() const
{
  CDM::PhysiologyDataRequestData* data = new CDM::PhysiologyDataRequestData();
  Unload(*data);
  return data;
}

void SEPhysiologyDataRequest::Unload(CDM::PhysiologyDataRequestData& data) const
{
  io::Scenario::Marshall(*this, data);
}

size_t SEPhysiologyDataRequest::HashCode() const
{
  if (m_Hash == 0) {
    m_Hash = std::hash<std::string>()(m_Name);
  }
  return m_Hash;
}
}