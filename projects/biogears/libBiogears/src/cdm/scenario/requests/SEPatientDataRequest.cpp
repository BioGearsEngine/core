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
#include <biogears/cdm/scenario/requests/SEPatientDataRequest.h>

namespace biogears {
SEPatientDataRequest::SEPatientDataRequest(const SEDecimalFormat* dfault)
  : SEDataRequest(dfault)
{
}

SEPatientDataRequest::~SEPatientDataRequest()
{
  Clear();
}

void SEPatientDataRequest::Clear()
{
  SEDataRequest::Clear();
}

bool SEPatientDataRequest::Load(const CDM::PatientDataRequestData& in)
{
  SEDataRequest::Load(in);
  return true;
}

CDM::PatientDataRequestData* SEPatientDataRequest::Unload() const
{
  CDM::PatientDataRequestData* data = new CDM::PatientDataRequestData();
  Unload(*data);
  return data;
}

void SEPatientDataRequest::Unload(CDM::PatientDataRequestData& data) const
{
  SEDataRequest::Unload(data);
}

size_t SEPatientDataRequest::HashCode() const 
{
  if (m_Hash == 0) {
    m_Hash = std::hash<std::string>()(m_Name);
  }
  return m_Hash;
}
}