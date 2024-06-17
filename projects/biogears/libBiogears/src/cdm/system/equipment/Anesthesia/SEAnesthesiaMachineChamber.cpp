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
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineChamber.h>

#include "io/cdm/Anesthesia.h"
#include "io/cdm/AnesthesiaActions.h"

#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceManager.h>

namespace biogears {
SEAnesthesiaMachineChamber::SEAnesthesiaMachineChamber(SESubstanceManager& substances)
  : Loggable(substances.GetLogger())
  , m_Substances(substances)
{
  m_State = SEOnOff::Invalid;
  m_SubstanceFraction = nullptr;
  m_Substance = nullptr;
}
//-------------------------------------------------------------------------------

SEAnesthesiaMachineChamber::~SEAnesthesiaMachineChamber()
{
  Clear();
}
//-------------------------------------------------------------------------------

void SEAnesthesiaMachineChamber::Clear()
{
  m_State = SEOnOff::Invalid;
  SAFE_DELETE(m_SubstanceFraction);
  m_Substance = nullptr;
}
//-------------------------------------------------------------------------------
 
bool SEAnesthesiaMachineChamber::Load(const CDM::AnesthesiaMachineChamberData& in)
{
  io::Anesthesia::UnMarshall(in, *this);
  return true;
}
//-------------------------------------------------------------------------------

CDM::AnesthesiaMachineChamberData* SEAnesthesiaMachineChamber::Unload() const
{
  CDM::AnesthesiaMachineChamberData* data = new CDM::AnesthesiaMachineChamberData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------

void SEAnesthesiaMachineChamber::Unload(CDM::AnesthesiaMachineChamberData& data) const
{
  io::Anesthesia::Marshall(*this, data);
}
//-------------------------------------------------------------------------------

void SEAnesthesiaMachineChamber::Merge(const SEAnesthesiaMachineChamber& from)
{
  if (from.HasState())
    SetState(from.m_State);
  if (from.HasSubstanceFraction())
    GetSubstanceFraction().Set(*from.m_SubstanceFraction);
  if (from.m_Substance != nullptr) {
    if (&m_Substances != &from.m_Substances) {
      m_Substance = m_Substances.GetSubstance(from.m_Substance->GetName());
      if (m_Substance == nullptr) {
        std::stringstream ss;
        ss << "Do not have substance : " << from.m_Substance->GetName();
        Error(ss);
      }
    } else
      m_Substance = from.m_Substance;
  }
}
//-------------------------------------------------------------------------------
const SEScalar* SEAnesthesiaMachineChamber::GetScalar(const char* name)
{
  return GetScalar(std::string { name });
}
//-------------------------------------------------------------------------------
const SEScalar* SEAnesthesiaMachineChamber::GetScalar(const std::string& name)
{
  if (name == "SubstanceFraction")
    return &GetSubstanceFraction();
  return nullptr;
}
//-------------------------------------------------------------------------------

SEOnOff SEAnesthesiaMachineChamber::GetState() const
{
  return m_State;
}
//-------------------------------------------------------------------------------
void SEAnesthesiaMachineChamber::SetState(SEOnOff state)
{
  m_State = state;
}
//-------------------------------------------------------------------------------
bool SEAnesthesiaMachineChamber::HasState() const
{
  return m_State == (SEOnOff::Invalid) ? false : true;
}
//-------------------------------------------------------------------------------
void SEAnesthesiaMachineChamber::InvalidateState()
{
  m_State = SEOnOff::Invalid;
}
//-------------------------------------------------------------------------------

bool SEAnesthesiaMachineChamber::HasSubstanceFraction() const
{
  return m_SubstanceFraction == nullptr ? false : m_SubstanceFraction->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFraction& SEAnesthesiaMachineChamber::GetSubstanceFraction()
{
  if (m_SubstanceFraction == nullptr)
    m_SubstanceFraction = new SEScalarFraction();
  return *m_SubstanceFraction;
}
//-------------------------------------------------------------------------------
double SEAnesthesiaMachineChamber::GetSubstanceFraction() const
{
  if (m_SubstanceFraction == nullptr)
    return SEScalar::dNaN();
  return m_SubstanceFraction->GetValue();
}
//-------------------------------------------------------------------------------

bool SEAnesthesiaMachineChamber::HasSubstance() const
{
  return m_Substance == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SESubstance* SEAnesthesiaMachineChamber::GetSubstance() const
{
  return (SESubstance*)m_Substance;
}
//-------------------------------------------------------------------------------
void SEAnesthesiaMachineChamber::SetSubstance(const SESubstance& substance)
{
  m_Substance = &substance;
}
//-------------------------------------------------------------------------------
void SEAnesthesiaMachineChamber::RemoveSubstance()
{
  m_Substance = nullptr;
  ;
}
//-------------------------------------------------------------------------------

void SEAnesthesiaMachineChamber::ToString(std::ostream& str) const
{
  if (m_SubstanceFraction != nullptr)
    str << " Anesthesia Machine Chamber, Substance Fraction" << m_SubstanceFraction
        << ", Substance :" << GetSubstance()->GetName() << std::flush;
  else
    str << "Action not specified properly" << std::flush;
}
//-------------------------------------------------------------------------------
bool SEAnesthesiaMachineChamber::operator==(SEAnesthesiaMachineChamber const& rhs) const
{
  return ((m_Substance && rhs.m_Substance) ? m_Substance->operator==(*rhs.m_Substance)
                                           : m_Substance == rhs.m_Substance)
    && m_State == rhs.m_State
    && ((m_SubstanceFraction && rhs.m_SubstanceFraction)
          ? m_SubstanceFraction->operator==(*rhs.m_SubstanceFraction)
          : m_SubstanceFraction == rhs.m_SubstanceFraction);
}
//-------------------------------------------------------------------------------
bool SEAnesthesiaMachineChamber::operator!=(SEAnesthesiaMachineChamber const& rhs) const
{
  return !(*this == rhs);
}
}
