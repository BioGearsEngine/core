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
#include <biogears/cdm/patient/conditions/SECompartmentSyndrome.h>


namespace biogears {
SECompartmentSyndrome::SECompartmentSyndrome()
  : SEPatientCondition()
{
  m_CompartmentAffected = nullptr;
}
//-----------------------------------------------------------------------------
SECompartmentSyndrome::~SECompartmentSyndrome()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SECompartmentSyndrome::Clear()
{
  SEPatientCondition::Clear();
  m_CompartmentAffected="";
}
//-----------------------------------------------------------------------------
bool SECompartmentSyndrome::IsValid() const
{
  return SEPatientCondition::IsValid() && HasLocation();
}
//-----------------------------------------------------------------------------
bool SECompartmentSyndrome::Load(const CDM::CompartmentSyndromeData& in)
{
  SEPatientCondition::Load(in);
  m_CompartmentAffected = Load(in.Location());
  return true;
}
//-----------------------------------------------------------------------------
CDM::CompartmentSyndromeData* SECompartmentSyndrome::Unload() const
{
  CDM::CompartmentSyndromeData* data(new CDM::CompartmentSyndromeData());
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SECompartmentSyndrome::Unload(CDM::CompartmentSyndromeData& data) const
{
  SEPatientCondition::Unload(data);
  if (HasLocation())
    data.Location(m_CompartmentAffected);
}
//-----------------------------------------------------------------------------
bool SECompartmentSyndrome::HasLocation() const
{
  return !m_CompartmentAffected.empty();
}
//-----------------------------------------------------------------------------
std::string SECompartmentSyndrome::GetLocation()
{
  return m_CompartmentAffected;
}
//-----------------------------------------------------------------------------
void SECompartmentSyndrome::ToString(std::ostream& str) const
{
  str << "Patient Condition : Compartment Syndrome";
  str << "\n\tLocation: ";
  HasLocation() ? str << m_CompartmentAffected : str << "NaN";
  str << std::flush;
}
//-----------------------------------------------------------------------------
}