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

#include <biogears/cdm/system/equipment/Anesthesia/actions/SEAnesthesiaMachineAction.h>

#include "io/cdm/Anesthesia.h"
#include "io/cdm/AnesthesiaActions.h"

namespace biogears {
SEAnesthesiaMachineAction::SEAnesthesiaMachineAction()
  : SEAction()
{
}

SEAnesthesiaMachineAction::~SEAnesthesiaMachineAction()
{
  Clear();
}

void SEAnesthesiaMachineAction::Clear()
{
  SEAction::Clear();
}

bool SEAnesthesiaMachineAction::IsValid() const
{
  return SEAction::IsValid();
}

bool SEAnesthesiaMachineAction::Load(const CDM::AnesthesiaMachineActionData& in, std::default_random_engine* rd)
{
  io::AnesthesiaActions::UnMarshall(in, *this, rd);
  return true;
}

CDM::AnesthesiaMachineActionData* SEAnesthesiaMachineAction::Unload() const
{
  CDM::AnesthesiaMachineActionData* data = new CDM::AnesthesiaMachineActionData();
  Unload(*data);
  return data;
}

void SEAnesthesiaMachineAction::Unload(CDM::AnesthesiaMachineActionData& data) const
{
  io::AnesthesiaActions::Marshall(*this, data);
}
}