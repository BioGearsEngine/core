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

#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachine.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEAnesthesiaMachineConfiguration.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEExpiratoryValveLeak.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEExpiratoryValveObstruction.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEInspiratoryValveLeak.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEInspiratoryValveObstruction.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEMaskLeak.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEOxygenTankPressureLoss.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEOxygenWallPortPressureLoss.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SESodaLimeFailure.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SETubeCuffLeak.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEVaporizerFailure.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEVentilatorPressureLoss.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEYPieceDisconnect.h>

namespace biogears {
class BIOGEARS_API SEAnesthesiaMachineActionCollection : public Loggable {
public:
  SEAnesthesiaMachineActionCollection(SESubstanceManager&);
  ~SEAnesthesiaMachineActionCollection();

  void Clear();

  void Unload(std::vector<CDM::ActionData*>& to);

  bool ProcessAction(const SEAnesthesiaMachineAction& action);
  bool ProcessAction(const CDM::AnesthesiaMachineActionData& action);

  // STATE ACTION
  bool HasConfiguration() const;
  SEAnesthesiaMachineConfiguration* GetConfiguration() const;
  void RemoveConfiguration();

  // INCIDENT ACTIONS

  bool HasOxygenTankPressureLoss() const;
  SEOxygenTankPressureLoss* GetOxygenTankPressureLoss() const;
  void RemoveOxygenTankPressureLoss();

  bool HasOxygenWallPortPressureLoss() const;
  SEOxygenWallPortPressureLoss* GetOxygenWallPortPressureLoss() const;
  void RemoveOxygenWallPortPressureLoss();

  // FAILURE ACTIONS

  bool HasExpiratoryValveLeak() const;
  SEExpiratoryValveLeak* GetExpiratoryValveLeak() const;
  void RemoveExpiratoryValveLeak();

  bool HasExpiratoryValveObstruction() const;
  SEExpiratoryValveObstruction* GetExpiratoryValveObstruction() const;
  void RemoveExpiratoryValveObstruction();

  bool HasInspiratoryValveLeak() const;
  SEInspiratoryValveLeak* GetInspiratoryValveLeak() const;
  void RemoveInspiratoryValveLeak();

  bool HasInspiratoryValveObstruction() const;
  SEInspiratoryValveObstruction* GetInspiratoryValveObstruction() const;
  void RemoveInspiratoryValveObstruction();

  bool HasMaskLeak() const;
  SEMaskLeak* GetMaskLeak() const;
  void RemoveMaskLeak();

  bool HasSodaLimeFailure() const;
  SESodaLimeFailure* GetSodaLimeFailure() const;
  void RemoveSodaLimeFailure();

  bool HasTubeCuffLeak() const;
  SETubeCuffLeak* GetTubeCuffLeak() const;
  void RemoveTubeCuffLeak();

  bool HasVaporizerFailure() const;
  SEVaporizerFailure* GetVaporizerFailure() const;
  void RemoveVaporizerFailure();

  bool HasVentilatorPressureLoss() const;
  SEVentilatorPressureLoss* GetVentilatorPressureLoss() const;
  void RemoveVentilatorPressureLoss();

  bool HasYPieceDisconnect() const;
  SEYPieceDisconnect* GetYPieceDisconnect() const;
  void RemoveYPieceDisconnect();

protected:
  bool IsValid(const SEAnesthesiaMachineAction& action);

  SEAnesthesiaMachineConfiguration* m_Configuration;
  //Anesthesia Machine Incidents
  SEOxygenTankPressureLoss* m_OxygenTankPressureLoss;
  SEOxygenWallPortPressureLoss* m_OxygenWallPortPressureLoss;
  //Anesthesia Machine Failures
  SEExpiratoryValveLeak* m_ExpiratoryValveLeak;
  SEExpiratoryValveObstruction* m_ExpiratoryValveObstruction;
  SEInspiratoryValveLeak* m_InspiratoryValveLeak;
  SEInspiratoryValveObstruction* m_InspiratoryValveObstruction;
  SEMaskLeak* m_MaskLeak;
  SESodaLimeFailure* m_SodaLimeFailure;
  SETubeCuffLeak* m_TubeCuffLeak;
  SEVaporizerFailure* m_VaporizerFailure;
  SEVentilatorPressureLoss* m_VentilatorPressureLoss;
  SEYPieceDisconnect* m_YPieceDisconnect;
  // General
  SESubstanceManager& m_Substances;
  std::stringstream m_ss;
};
}