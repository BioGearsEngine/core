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

#pragma once
#include <biogears/exports.h>

#include <biogears/cdm/enums/SEPatientEnums.h>
#include <biogears/cdm/enums/SEAnesthesiaEnums.h>
#include <biogears/cdm/utils/Logger.h>
#include <biogears/schema/cdm/Anesthesia.hxx>
#include <biogears/schema/cdm/Patient.hxx>

namespace CDM = mil::tatrc::physiology::datamodel;

namespace biogears {
class BIOGEARS_API SEEventHandler {
public:
  SEEventHandler() = default;
  virtual ~SEEventHandler() = default;

  virtual void HandlePatientEvent(SEPatientEventType type, bool active, const SEScalarTime* time = nullptr) = 0;
  virtual void HandleAnesthesiaMachineEvent(SEAnesthesiaMachineEvent type, bool active, const SEScalarTime* time = nullptr) = 0;
};
}