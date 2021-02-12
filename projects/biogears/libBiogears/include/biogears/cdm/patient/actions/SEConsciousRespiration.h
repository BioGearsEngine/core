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
#include <biogears/cdm/patient/actions/SEPatientAction.h>
#include <biogears/schema/cdm/PatientActions.hxx>

namespace biogears {
class SEConsciousRespirationCommand;
class SEForcedExhale;
class SEForcedInhale;
class SEBreathHold;
class SEUseInhaler;
namespace io {
  class PatientActions;
}
class BIOGEARS_API SEConsciousRespiration : public SEPatientAction {
  friend io::PatientActions;
public:
  SEConsciousRespiration();
  virtual ~SEConsciousRespiration() override;

  static constexpr const char* TypeTag() { return "SEConsciousRespiration"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override; //clear memory

  virtual bool IsValid() const override;
  virtual bool IsActive() const override;

  virtual bool Load(const CDM::ConsciousRespirationData& in, const SESubstanceManager& substances);
  virtual CDM::ConsciousRespirationData* Unload() const override;

protected:
  virtual void Unload(CDM::ConsciousRespirationData& data) const;

public:
  // Get the active command
  virtual SEConsciousRespirationCommand* GetActiveCommand();
  /// When the active command has been processed, remove it
  virtual void RemoveActiveCommand();

  virtual SEForcedExhale& AddForcedExhale();
  virtual SEForcedInhale& AddForcedInhale();
  virtual SEBreathHold& AddBreathHold();
  virtual SEUseInhaler& AddUseInhaler();

  virtual void ToString(std::ostream& str) const override;

protected:
  bool m_ClearCommands;
  std::vector<SEConsciousRespirationCommand*> m_Commands;
};
}