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
#include <biogears/cdm/patient/actions/SEPericardialEffusion.h>

#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SEPericardialEffusion::SEPericardialEffusion()
  : SEPatientAction()
{
  m_EffusionRate = nullptr;
}

SEPericardialEffusion::~SEPericardialEffusion()
{
  Clear();
}

void SEPericardialEffusion::Clear()
{
  SEPatientAction::Clear();
  SAFE_DELETE(m_EffusionRate);
}

bool SEPericardialEffusion::IsValid() const
{
  return SEPatientAction::IsValid() && HasEffusionRate();
}

bool SEPericardialEffusion::IsActive() const
{
  return IsValid() ? !m_EffusionRate->IsZero() : false;
}

bool SEPericardialEffusion::Load(const CDM::PericardialEffusionData& in)
{
  SEPatientAction::Load(in);
  GetEffusionRate().Load(in.EffusionRate());
  return true;
}

CDM::PericardialEffusionData* SEPericardialEffusion::Unload() const
{
  CDM::PericardialEffusionData* data(new CDM::PericardialEffusionData());
  Unload(*data);
  return data;
}

void SEPericardialEffusion::Unload(CDM::PericardialEffusionData& data) const
{
  SEPatientAction::Unload(data);
  if (m_EffusionRate != nullptr)
    data.EffusionRate(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_EffusionRate->Unload()));
}

bool SEPericardialEffusion::HasEffusionRate() const
{
  return m_EffusionRate == nullptr ? false : m_EffusionRate->IsValid();
}

SEScalarVolumePerTime& SEPericardialEffusion::GetEffusionRate()
{
  if (m_EffusionRate == nullptr)
    m_EffusionRate = new SEScalarVolumePerTime();
  return *m_EffusionRate;
}

void SEPericardialEffusion::ToString(std::ostream& str) const
{
  str << "Patient Action : Pericardial Effusion";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tEffusion Rate: ";
  HasEffusionRate() ? str << *m_EffusionRate : str << "NaN";
  str << std::flush;
}
}