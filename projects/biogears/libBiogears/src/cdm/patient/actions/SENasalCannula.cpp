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
#include <biogears/cdm/patient/actions/SENasalCannula.h>

#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SENasalCannula::SENasalCannula()
  : SEPatientAction()
  , m_FlowRate(new SEScalarVolumePerTime())
{
  m_FlowRate = nullptr;
}
//-------------------------------------------------------------------------------
SENasalCannula::~SENasalCannula()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SENasalCannula::Clear()
{

  SEPatientAction::Clear();
  SAFE_DELETE(m_FlowRate);
}
//-------------------------------------------------------------------------------
bool SENasalCannula::IsValid() const
{
  return SEPatientAction::IsValid() && HasFlowRate();
}
//-------------------------------------------------------------------------------
bool SENasalCannula::IsActive() const
{
  return IsValid() ? ! (m_FlowRate->GetValue(VolumePerTimeUnit::mL_Per_min) <= ZERO_APPROX) : false;
}
//-------------------------------------------------------------------------------
bool SENasalCannula::Load(const CDM::NasalCannulaData& in)
{
  SEPatientAction::Load(in);
  GetFlowRate().Load(in.FlowRate());
  return true;
}
//-------------------------------------------------------------------------------
CDM::NasalCannulaData* SENasalCannula::Unload() const
{
  CDM::NasalCannulaData* data(new CDM::NasalCannulaData());
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SENasalCannula::Unload(CDM::NasalCannulaData& data) const
{
  SEPatientAction::Unload(data);
  if (m_FlowRate != nullptr)
    data.FlowRate(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_FlowRate->Unload()));
}
//-------------------------------------------------------------------------------
bool SENasalCannula::HasFlowRate() const
{
  return m_FlowRate == nullptr ? false : m_FlowRate->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTime& SENasalCannula::GetFlowRate()
{
  if (m_FlowRate == nullptr)
    m_FlowRate = new SEScalarVolumePerTime();
  return *m_FlowRate;
}
//-------------------------------------------------------------------------------
void SENasalCannula::ToString(std::ostream& str) const
{
  str << "Patient Action : Apnea";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity: ";
  HasFlowRate() ? str << *m_FlowRate : str << "Not Set";
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SENasalCannula::operator==(const SENasalCannula& rhs) const
{
  bool equivilant = m_Comment == rhs.m_Comment;
  equivilant &= (m_FlowRate && rhs.m_FlowRate) ? m_FlowRate->operator==(*rhs.m_FlowRate) : m_FlowRate == rhs.m_FlowRate;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SENasalCannula::operator!=( const SENasalCannula& rhs) const
{
  return !(*this == rhs);
}
}