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
#include <biogears/cdm/patient/actions/SEConsciousRespiration.h>

#include "io/cdm/PatientActions.h"
#include <biogears/cdm/patient/actions/SEBreathHold.h>
#include <biogears/cdm/patient/actions/SEConsciousRespirationCommand.h>
#include <biogears/cdm/patient/actions/SEForcedExhale.h>
#include <biogears/cdm/patient/actions/SEForcedInhale.h>
#include <biogears/cdm/patient/actions/SEUseInhaler.h>
#include <biogears/cdm/substance/SESubstanceManager.h>

namespace biogears {
SEConsciousRespiration::SEConsciousRespiration()
  : SEPatientAction()
{
  m_ClearCommands = true;
}
//-------------------------------------------------------------------------------

SEConsciousRespiration::~SEConsciousRespiration()
{
  Clear();
}
//-------------------------------------------------------------------------------

void SEConsciousRespiration::Clear()
{
  SEPatientAction::Clear();
  if (m_ClearCommands)
    DELETE_VECTOR(m_Commands);
}
//-------------------------------------------------------------------------------

bool SEConsciousRespiration::IsValid() const
{
  return SEPatientAction::IsValid() && !m_Commands.empty();
}
//-------------------------------------------------------------------------------

bool SEConsciousRespiration::IsActive() const
{
  return SEPatientAction::IsActive();
}
//-------------------------------------------------------------------------------

bool SEConsciousRespiration::Load(const CDM::ConsciousRespirationData& in, const SESubstanceManager& substances, std::default_random_engine *rd)
{
  io::PatientActions::UnMarshall(in, *this, rd);
  return true;
}
//-------------------------------------------------------------------------------

CDM::ConsciousRespirationData* SEConsciousRespiration::Unload() const
{
  CDM::ConsciousRespirationData* data(new CDM::ConsciousRespirationData());
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------

void SEConsciousRespiration::Unload(CDM::ConsciousRespirationData& data) const
{
  io::PatientActions::Marshall(*this, data);
}
//-------------------------------------------------------------------------------

SEConsciousRespirationCommand* SEConsciousRespiration::GetActiveCommand()
{
  if (!m_Commands.empty())
    return m_Commands[0];
  return nullptr;
}
//-------------------------------------------------------------------------------

void SEConsciousRespiration::RemoveActiveCommand()
{
  SEConsciousRespirationCommand* c = GetActiveCommand();
  if (c != nullptr) {
    m_Commands.erase(m_Commands.begin());
    SAFE_DELETE(c);
  }
}
//-------------------------------------------------------------------------------

SEForcedExhale& SEConsciousRespiration::AddForcedExhale()
{
  SEForcedExhale* myEx = new SEForcedExhale();
  m_Commands.push_back(myEx);
  return *myEx;
}
//-------------------------------------------------------------------------------

SEForcedInhale& SEConsciousRespiration::AddForcedInhale()
{
  SEForcedInhale* myIn = new SEForcedInhale();
  m_Commands.push_back(myIn);
  return *myIn;
}
//-------------------------------------------------------------------------------

SEBreathHold& SEConsciousRespiration::AddBreathHold()
{
  SEBreathHold* myHold = new SEBreathHold();
  m_Commands.push_back(myHold);
  return *myHold;
}
//-------------------------------------------------------------------------------

SEUseInhaler& SEConsciousRespiration::AddUseInhaler()
{
  SEUseInhaler* myUse = new SEUseInhaler();
  m_Commands.push_back(myUse);
  return *myUse;
}
//-------------------------------------------------------------------------------

void SEConsciousRespiration::ToString(std::ostream& str) const
{
  str << "Patient Action : Conscious Respiration";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  for (SEConsciousRespirationCommand* c : m_Commands) {
    str << "\n\t";
    c->ToString(str);
  }
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SEConsciousRespiration::operator==( const SEConsciousRespiration& rhs) const
{
  bool equivilant = m_Comment == rhs.m_Comment;
  equivilant &= m_ClearCommands == rhs.m_ClearCommands;
  equivilant &= m_Commands.size() == rhs.m_Commands.size();
  
  if(equivilant){
    for ( auto i = 0; i < m_Commands.size(); i++) {
      //NOTE: Other definitions of equivilance might be resonable
       if(m_Commands[i] && rhs.m_Commands[i]){
         equivilant &= m_Commands[i]->operator==(*rhs.m_Commands[i]);
       }
    }
  }

  return equivilant;
}
//-------------------------------------------------------------------------------
bool SEConsciousRespiration::operator!=( const SEConsciousRespiration& rhs) const
{
  return !(*this == rhs);
}
}