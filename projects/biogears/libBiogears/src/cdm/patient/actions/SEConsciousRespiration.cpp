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

SEConsciousRespiration::~SEConsciousRespiration()
{
  Clear();
}

void SEConsciousRespiration::Clear()
{
  SEPatientAction::Clear();
  if (m_ClearCommands)
    DELETE_VECTOR(m_Commands);
}

bool SEConsciousRespiration::IsValid() const
{
  return SEPatientAction::IsValid() && !m_Commands.empty();
}

bool SEConsciousRespiration::IsActive() const
{
  return SEPatientAction::IsActive();
}

bool SEConsciousRespiration::Load(const CDM::ConsciousRespirationData& in, const SESubstanceManager& substances)
{
  // Set this before our super class tells us to Clear if the action wants us to keep our current data
  m_ClearCommands = !in.AppendToPrevious();
  SEPatientAction::Load(in);
  m_ClearCommands = true;
  CDM::ConsciousRespirationCommandData* command;
  for (unsigned int i = 0; i < in.Command().size(); i++) {
    command = (CDM::ConsciousRespirationCommandData*)&in.Command()[i];

    CDM::BreathHoldData* bh = dynamic_cast<CDM::BreathHoldData*>(command);
    if (bh != nullptr) {
      AddBreathHold().Load(*bh);
      continue;
    }

    CDM::ForcedInhaleData* fi = dynamic_cast<CDM::ForcedInhaleData*>(command);
    if (fi != nullptr) {
      AddForcedInhale().Load(*fi);
      continue;
    }

    CDM::ForcedExhaleData* fe = dynamic_cast<CDM::ForcedExhaleData*>(command);
    if (fe != nullptr) {
      AddForcedExhale().Load(*fe);
      continue;
    }

    CDM::UseInhalerData* si = dynamic_cast<CDM::UseInhalerData*>(command);
    if (si != nullptr) {
      AddUseInhaler().Load(*si);
      continue;
    }
  }
  return true;
}

CDM::ConsciousRespirationData* SEConsciousRespiration::Unload() const
{
  CDM::ConsciousRespirationData* data(new CDM::ConsciousRespirationData());
  Unload(*data);
  return data;
}

void SEConsciousRespiration::Unload(CDM::ConsciousRespirationData& data) const
{
  SEPatientAction::Unload(data);
  data.AppendToPrevious(false);
  for (SEConsciousRespirationCommand* cmd : m_Commands)
    data.Command().push_back(std::unique_ptr<CDM::ConsciousRespirationCommandData>(cmd->Unload()));
}

SEConsciousRespirationCommand* SEConsciousRespiration::GetActiveCommand()
{
  if (!m_Commands.empty())
    return m_Commands[0];
  return nullptr;
}
void SEConsciousRespiration::RemoveActiveCommand()
{
  SEConsciousRespirationCommand* c = GetActiveCommand();
  if (c != nullptr) {
    m_Commands.erase(m_Commands.begin());
    SAFE_DELETE(c);
  }
}

SEForcedExhale& SEConsciousRespiration::AddForcedExhale()
{
  SEForcedExhale* myEx = new SEForcedExhale();
  m_Commands.push_back(myEx);
  return *myEx;
}

SEForcedInhale& SEConsciousRespiration::AddForcedInhale()
{
  SEForcedInhale* myIn = new SEForcedInhale();
  m_Commands.push_back(myIn);
  return *myIn;
}

SEBreathHold& SEConsciousRespiration::AddBreathHold()
{
  SEBreathHold* myHold = new SEBreathHold();
  m_Commands.push_back(myHold);
  return *myHold;
}

SEUseInhaler& SEConsciousRespiration::AddUseInhaler()
{
  SEUseInhaler* myUse = new SEUseInhaler();
  m_Commands.push_back(myUse);
  return *myUse;
}

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
}