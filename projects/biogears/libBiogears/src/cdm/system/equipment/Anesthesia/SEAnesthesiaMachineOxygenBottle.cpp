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
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineOxygenBottle.h>

#include "io/cdm/Anesthesia.h"
#include "io/cdm/AnesthesiaActions.h"

#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/container/Tree.tci.h>

namespace biogears {
namespace io {
  class Anesthesia;
}
SEAnesthesiaMachineOxygenBottle::SEAnesthesiaMachineOxygenBottle(Logger* logger)
  : Loggable(logger)
{
  m_Volume = nullptr;
}
//-------------------------------------------------------------------------------

SEAnesthesiaMachineOxygenBottle::~SEAnesthesiaMachineOxygenBottle()
{
  Clear();
}
//-------------------------------------------------------------------------------

void SEAnesthesiaMachineOxygenBottle::Clear()
{
  SAFE_DELETE(m_Volume);
}
//-------------------------------------------------------------------------------

bool SEAnesthesiaMachineOxygenBottle::Load(const CDM::AnesthesiaMachineOxygenBottleData& in)
{
  io::Anesthesia::UnMarshall(in, *this);
  return true;
}
//-------------------------------------------------------------------------------

CDM::AnesthesiaMachineOxygenBottleData* SEAnesthesiaMachineOxygenBottle::Unload() const
{
  CDM::AnesthesiaMachineOxygenBottleData* data = new CDM::AnesthesiaMachineOxygenBottleData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------

void SEAnesthesiaMachineOxygenBottle::Unload(CDM::AnesthesiaMachineOxygenBottleData& data) const
{
  io::Anesthesia::Marshall(*this, data);
}
//-------------------------------------------------------------------------------

void SEAnesthesiaMachineOxygenBottle::Merge(const SEAnesthesiaMachineOxygenBottle& from)
{
  COPY_PROPERTY(Volume);
}
//-------------------------------------------------------------------------------
const SEScalar* SEAnesthesiaMachineOxygenBottle::GetScalar(const char* name)
{
  return GetScalar(std::string { name });
}
//-------------------------------------------------------------------------------
const SEScalar* SEAnesthesiaMachineOxygenBottle::GetScalar(const std::string& name)
{
  if (name == "Volume")
    return &GetVolume();
  return nullptr;
}
//-------------------------------------------------------------------------------

bool SEAnesthesiaMachineOxygenBottle::HasVolume() const
{
  return m_Volume == nullptr ? false : m_Volume->IsValid();
}
//-------------------------------------------------------------------------------

SEScalarVolume& SEAnesthesiaMachineOxygenBottle::GetVolume()
{
  if (m_Volume == nullptr)
    m_Volume = new SEScalarVolume();
  return *m_Volume;
}
//-------------------------------------------------------------------------------
double SEAnesthesiaMachineOxygenBottle::GetVolume(const VolumeUnit& unit) const
{
  if (m_Volume == nullptr)
    return SEScalar::dNaN();
  return m_Volume->GetValue(unit);
}
//-------------------------------------------------------------------------------
void SEAnesthesiaMachineOxygenBottle::ToString(std::ostream& str)
{
  str << " Anesthesia Machine Oxygen Bottle, ";
  if (HasVolume())
    str << "\rVolume : " << GetVolume();
  else
    str << "\rVolume : NaN" << GetVolume();
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SEAnesthesiaMachineOxygenBottle::operator==(SEAnesthesiaMachineOxygenBottle const& rhs) const
{
  return (m_Volume && rhs.m_Volume) ? m_Volume->operator==(*rhs.m_Volume) : m_Volume == rhs.m_Volume;
}
//-------------------------------------------------------------------------------
bool SEAnesthesiaMachineOxygenBottle::operator!=(SEAnesthesiaMachineOxygenBottle const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}