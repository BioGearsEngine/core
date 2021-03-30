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
#include <biogears/cdm/patient/actions/SEPulmonaryShunt.h>

#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SEPulmonaryShunt::SEPulmonaryShunt()
  : SEPatientAction()
{
  m_FlowRateScale = nullptr;
}

SEPulmonaryShunt::~SEPulmonaryShunt()
{
  Clear();
}

void SEPulmonaryShunt::Clear()
{

  SEPatientAction::Clear();
  SAFE_DELETE(m_FlowRateScale);
}

bool SEPulmonaryShunt::IsValid() const
{
  return SEPatientAction::IsValid() && HasFlowRateScale();
}

bool SEPulmonaryShunt::IsActive() const
{
  return IsValid() ? !m_FlowRateScale->IsZero() : false;
}

bool SEPulmonaryShunt::Load(const CDM::PulmonaryShuntData& in)
{
  SEPatientAction::Load(in);
  GetFlowRateScale().Load(in.FlowRateScaling());
  return true;
}

CDM::PulmonaryShuntData* SEPulmonaryShunt::Unload() const
{
  CDM::PulmonaryShuntData* data(new CDM::PulmonaryShuntData());
  Unload(*data);
  return data;
}

void SEPulmonaryShunt::Unload(CDM::PulmonaryShuntData& data) const
{
  SEPatientAction::Unload(data);
  if (m_FlowRateScale != nullptr)
    data.FlowRateScaling(std::unique_ptr<CDM::Scalar0To1Data>(m_FlowRateScale->Unload()));
}

bool SEPulmonaryShunt::HasFlowRateScale() const
{
  return m_FlowRateScale == nullptr ? false : m_FlowRateScale->IsValid();
}

SEScalar0To1& SEPulmonaryShunt::GetFlowRateScale()
{
  if (m_FlowRateScale == nullptr)
    m_FlowRateScale = new SEScalar0To1();
  return *m_FlowRateScale;
}

void SEPulmonaryShunt::ToString(std::ostream& str) const
{
  str << "Patient Action : Pulmonary Shunt";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tFlow Rate Scale: ";
  HasFlowRateScale() ? str << *m_FlowRateScale : str << "Not Set";
  str << std::flush;
}
}