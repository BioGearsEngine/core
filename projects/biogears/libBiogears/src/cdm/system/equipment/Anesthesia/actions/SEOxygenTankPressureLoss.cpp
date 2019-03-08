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
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEOxygenTankPressureLoss.h>

#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SEOxygenTankPressureLoss::SEOxygenTankPressureLoss()
  : SEAnesthesiaMachineAction()
{
  m_State = CDM::enumOnOff::Off;
}

SEOxygenTankPressureLoss::~SEOxygenTankPressureLoss()
{
  Clear();
}

void SEOxygenTankPressureLoss::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  m_State = (CDM::enumOnOff::value)-1;
}

bool SEOxygenTankPressureLoss::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid();
}
bool SEOxygenTankPressureLoss::IsActive() const
{
  return m_State == CDM::enumOnOff::On;
}
void SEOxygenTankPressureLoss::SetActive(bool b)
{
  m_State = b ? CDM::enumOnOff::On : CDM::enumOnOff::Off;
}

bool SEOxygenTankPressureLoss::Load(const CDM::OxygenTankPressureLossData& in)
{
  SEAnesthesiaMachineAction::Load(in);
  SetActive(in.State() == CDM::enumOnOff::On ? true : false);
  return true;
}

CDM::OxygenTankPressureLossData* SEOxygenTankPressureLoss::Unload() const
{
  CDM::OxygenTankPressureLossData* data = new CDM::OxygenTankPressureLossData();
  Unload(*data);
  return data;
}

void SEOxygenTankPressureLoss::Unload(CDM::OxygenTankPressureLossData& data) const
{
  SEAnesthesiaMachineAction::Unload(data);
  data.State(IsActive() ? CDM::enumOnOff::On : CDM::enumOnOff::Off);
}

void SEOxygenTankPressureLoss::ToString(std::ostream& str) const
{
  str << "Anesthesia Machine Action : Oxygen Tank Pressure Loss";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tState: " << IsActive();
  str << std::flush;
}
}