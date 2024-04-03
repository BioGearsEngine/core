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
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEOxygenWallPortPressureLoss.h>

#include "io/cdm/Anesthesia.h"
#include "io/cdm/AnesthesiaActions.h"

#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SEOxygenWallPortPressureLoss::SEOxygenWallPortPressureLoss()
  : SEAnesthesiaMachineAction()
{
  m_State = SEOnOff::Off;
}
//-------------------------------------------------------------------------------
SEOxygenWallPortPressureLoss::~SEOxygenWallPortPressureLoss()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEOxygenWallPortPressureLoss::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  m_State = SEOnOff::Invalid;
}
//-------------------------------------------------------------------------------
bool SEOxygenWallPortPressureLoss::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid();
}
//-------------------------------------------------------------------------------
bool SEOxygenWallPortPressureLoss::IsActive() const
{
  return m_State == SEOnOff::On;
}
//-------------------------------------------------------------------------------
void SEOxygenWallPortPressureLoss::SetActive(bool b)
{
  m_State = b ? SEOnOff::On : SEOnOff::Off;
}
//-------------------------------------------------------------------------------
bool SEOxygenWallPortPressureLoss::Load(const CDM::OxygenWallPortPressureLossData& in, std::default_random_engine *rd)
{
  io::AnesthesiaActions::UnMarshall(in, *this, rd);
  return true;
}
//-------------------------------------------------------------------------------
CDM::OxygenWallPortPressureLossData* SEOxygenWallPortPressureLoss::Unload() const
{
  CDM::OxygenWallPortPressureLossData* data = new CDM::OxygenWallPortPressureLossData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SEOxygenWallPortPressureLoss::Unload(CDM::OxygenWallPortPressureLossData& data) const
{
  io::AnesthesiaActions::Marshall(*this, data);
}
//-------------------------------------------------------------------------------
void SEOxygenWallPortPressureLoss::ToString(std::ostream& str) const
{
  str << "Anesthesia Machine Action : Oxygen Wall Port Pressure Loss";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tState: " << IsActive();
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SEOxygenWallPortPressureLoss::operator==(SEOxygenWallPortPressureLoss const& rhs) const
{
  if (this == &rhs)
    return true;

  return m_State == rhs.m_State
    && m_Comment == rhs.m_Comment;
}
//-------------------------------------------------------------------------------
bool SEOxygenWallPortPressureLoss::operator!=(SEOxygenWallPortPressureLoss const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}
