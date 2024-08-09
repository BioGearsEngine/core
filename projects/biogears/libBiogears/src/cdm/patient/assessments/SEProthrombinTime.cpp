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
#include <biogears/cdm/patient/assessments/SEProthrombinTime.h>
#include <biogears/cdm/properties/SEScalar.h>

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/schema/cdm/Properties.hxx>
#include "io/cdm/PatientAssessments.h"

namespace biogears {
SEProthrombinTime::SEProthrombinTime()
{
  m_InternationalNormalizedRatio = nullptr;
}
//-------------------------------------------------------------------------------
SEProthrombinTime::~SEProthrombinTime()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEProthrombinTime::Clear()
{
  SEPatientAssessment::Clear();
  SAFE_DELETE(m_InternationalNormalizedRatio);
}
//-------------------------------------------------------------------------------
void SEProthrombinTime::Reset()
{
  SEPatientAssessment::Reset();
  INVALIDATE_PROPERTY(m_InternationalNormalizedRatio);
}
//-------------------------------------------------------------------------------
bool SEProthrombinTime::HasInternationalNormalizedRatio()
{
  return m_InternationalNormalizedRatio == nullptr ? false : m_InternationalNormalizedRatio->IsValid();
}
SEScalar& SEProthrombinTime::GetInternationalNormalizedRatio()
{
  if (m_InternationalNormalizedRatio == nullptr)
    m_InternationalNormalizedRatio = new SEScalar();
  return *m_InternationalNormalizedRatio;
}
//-------------------------------------------------------------------------------
bool SEProthrombinTime::operator==(SEProthrombinTime const& rhs) const
{
  if (this == &rhs)
    return true;

  return ((m_InternationalNormalizedRatio && rhs.m_InternationalNormalizedRatio)
            ? m_InternationalNormalizedRatio->operator==(*rhs.m_InternationalNormalizedRatio)
            : m_InternationalNormalizedRatio == rhs.m_InternationalNormalizedRatio);
}
bool SEProthrombinTime::operator!=(SEProthrombinTime const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}