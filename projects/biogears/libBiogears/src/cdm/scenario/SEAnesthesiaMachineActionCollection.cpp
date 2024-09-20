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
#include <biogears/cdm/scenario/SEAnesthesiaMachineActionCollection.h>

#include "io/cdm/Actions.h"
#include "io/cdm/AnesthesiaActions.h"
#include "io/cdm/Scenario.h"

#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>

#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineChamber.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineOxygenBottle.h>

namespace biogears {
SEAnesthesiaMachineActionCollection::SEAnesthesiaMachineActionCollection(SESubstanceManager& substances)
  : Loggable(substances.GetLogger())
  , m_Substances(substances)
{
  m_Configuration = nullptr;

  m_OxygenTankPressureLoss = nullptr;
  m_OxygenWallPortPressureLoss = nullptr;

  m_ExpiratoryValveLeak = nullptr;
  m_ExpiratoryValveObstruction = nullptr;
  m_InspiratoryValveLeak = nullptr;
  m_InspiratoryValveObstruction = nullptr;
  m_MaskLeak = nullptr;
  m_SodaLimeFailure = nullptr;
  m_TubeCuffLeak = nullptr;
  m_VaporizerFailure = nullptr;
  m_VentilatorPressureLoss = nullptr;
  m_YPieceDisconnect = nullptr;
}
//-------------------------------------------------------------------------------
SEAnesthesiaMachineActionCollection::~SEAnesthesiaMachineActionCollection()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEAnesthesiaMachineActionCollection::Clear()
{
  // State
  RemoveConfiguration();
  // Incidents
  RemoveOxygenTankPressureLoss();
  RemoveOxygenWallPortPressureLoss();
  // Failures
  RemoveExpiratoryValveLeak();
  RemoveExpiratoryValveObstruction();
  RemoveInspiratoryValveLeak();
  RemoveInspiratoryValveObstruction();
  RemoveMaskLeak();
  RemoveSodaLimeFailure();
  RemoveTubeCuffLeak();
  RemoveVaporizerFailure();
  RemoveVentilatorPressureLoss();
  RemoveYPieceDisconnect();
}
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
bool SEAnesthesiaMachineActionCollection::ProcessAction(const SEAnesthesiaMachineAction& action, const PhysiologyEngine& engine)
{
  auto actionData = io::AnesthesiaActions::factory(&action);
  if (auto config = dynamic_cast<const SEAnesthesiaMachineConfiguration*>(&action)) {
    if (m_Configuration == nullptr) {
      m_Configuration = new SEAnesthesiaMachineConfiguration(m_Substances);
    }
    auto configData = dynamic_cast<CDM::AnesthesiaMachineConfigurationData*>(actionData.get());
    io::AnesthesiaActions::UnMarshall(*configData, *m_Configuration);
    return IsValid(*m_Configuration);
  }

  if (auto O2Tank = dynamic_cast<const SEOxygenTankPressureLoss*>(&action)) {
    if (m_OxygenTankPressureLoss == nullptr) {
      m_OxygenTankPressureLoss = new SEOxygenTankPressureLoss();
    }
    auto O2TankData = dynamic_cast<CDM::OxygenTankPressureLossData*>(actionData.get());
    io::AnesthesiaActions::UnMarshall(*O2TankData, *m_OxygenTankPressureLoss);
    if (!m_OxygenTankPressureLoss->IsActive()) {
      RemoveOxygenTankPressureLoss();
      return true;
    }
    return IsValid(*m_OxygenTankPressureLoss);
  }

  if (auto O2Wall = dynamic_cast<const SEOxygenWallPortPressureLoss*>(&action)) {
    if (m_OxygenWallPortPressureLoss == nullptr) {
      m_OxygenWallPortPressureLoss = new SEOxygenWallPortPressureLoss();
    }
    auto O2WallData = dynamic_cast<CDM::OxygenWallPortPressureLossData*>(actionData.get());
    io::AnesthesiaActions::UnMarshall(*O2WallData, *m_OxygenWallPortPressureLoss);
    if (!m_OxygenWallPortPressureLoss->IsActive()) {
      RemoveOxygenWallPortPressureLoss();
      return true;
    }
    return IsValid(*m_OxygenWallPortPressureLoss);
  }

  if (auto eLeak = dynamic_cast<const SEExpiratoryValveLeak*>(&action)) {
    if (m_ExpiratoryValveLeak == nullptr) {
      m_ExpiratoryValveLeak = new SEExpiratoryValveLeak();
    }
    auto eLeakData = dynamic_cast<CDM::ExpiratoryValveLeakData*>(actionData.get());
    io::AnesthesiaActions::UnMarshall(*eLeakData, *m_ExpiratoryValveLeak);
    if (!m_ExpiratoryValveLeak->IsActive()) {
      RemoveExpiratoryValveLeak();
      return true;
    }
    return IsValid(*m_ExpiratoryValveLeak);
  }

  const SEExpiratoryValveObstruction* eOb = dynamic_cast<const SEExpiratoryValveObstruction*>(&action);
  if (eOb != nullptr) {
    if (m_ExpiratoryValveObstruction == nullptr) {
      m_ExpiratoryValveObstruction = new SEExpiratoryValveObstruction();
    }
    auto eObData = dynamic_cast<CDM::ExpiratoryValveObstructionData*>(actionData.get());
    io::AnesthesiaActions::UnMarshall(*eObData, *m_ExpiratoryValveObstruction);
    if (!m_ExpiratoryValveObstruction->IsActive()) {
      RemoveExpiratoryValveObstruction();
      return true;
    }
    return IsValid(*m_ExpiratoryValveObstruction);
  }

  if (auto iLeak = dynamic_cast<const SEInspiratoryValveLeak*>(&action)) {
    if (m_InspiratoryValveLeak == nullptr) {
      m_InspiratoryValveLeak = new SEInspiratoryValveLeak();
    }
    auto iLeakData = dynamic_cast<CDM::InspiratoryValveLeakData*>(actionData.get());
    io::AnesthesiaActions::UnMarshall(*iLeakData, *m_InspiratoryValveLeak);
    if (!m_InspiratoryValveLeak->IsActive()) {
      RemoveInspiratoryValveLeak();
      return true;
    }
    return IsValid(*m_InspiratoryValveLeak);
  }

  if (auto iOb = dynamic_cast<const SEInspiratoryValveObstruction*>(&action)) {
    if (m_InspiratoryValveObstruction == nullptr) {
      m_InspiratoryValveObstruction = new SEInspiratoryValveObstruction();
    }
    auto iObData = dynamic_cast<CDM::InspiratoryValveObstructionData*>(actionData.get());
    io::AnesthesiaActions::UnMarshall(*iObData, *m_InspiratoryValveObstruction);
    if (!m_InspiratoryValveObstruction->IsActive()) {
      RemoveInspiratoryValveObstruction();
      return true;
    }
    return IsValid(*m_InspiratoryValveObstruction);
  }

  if (auto mask = dynamic_cast<const SEMaskLeak*>(&action)) {
    if (m_MaskLeak == nullptr) {
      m_MaskLeak = new SEMaskLeak();
    }
    auto maskData = dynamic_cast<CDM::MaskLeakData*>(actionData.get());
    io::AnesthesiaActions::UnMarshall(*maskData, *m_MaskLeak);
    if (!m_MaskLeak->IsActive()) {
      RemoveMaskLeak();
      return true;
    }
    return IsValid(*m_MaskLeak);
  }

  if (auto soda = dynamic_cast<const SESodaLimeFailure*>(&action)) {
    if (m_SodaLimeFailure == nullptr) {
      m_SodaLimeFailure = new SESodaLimeFailure();
    }
    auto sodaData = dynamic_cast<CDM::SodaLimeFailureData*>(actionData.get());
    io::AnesthesiaActions::UnMarshall(*sodaData, *m_SodaLimeFailure);
    if (!m_SodaLimeFailure->IsActive()) {
      RemoveSodaLimeFailure();
      return true;
    }
    return IsValid(*m_SodaLimeFailure);
  }

  if (auto tube = dynamic_cast<const SETubeCuffLeak*>(&action)) {
    if (m_TubeCuffLeak == nullptr) {
      m_TubeCuffLeak = new SETubeCuffLeak();
    }
    auto tubeData = dynamic_cast<CDM::TubeCuffLeakData*>(actionData.get());
    io::AnesthesiaActions::UnMarshall(*tubeData, *m_TubeCuffLeak);
    if (!m_TubeCuffLeak->IsActive()) {
      RemoveTubeCuffLeak();
      return true;
    }
    return IsValid(*m_TubeCuffLeak);
  }

  if (auto vFail = dynamic_cast<const SEVaporizerFailure*>(&action)) {
    if (m_VaporizerFailure == nullptr) {
      m_VaporizerFailure = new SEVaporizerFailure();
    }
    auto vFailData = dynamic_cast<CDM::VaporizerFailureData*>(actionData.get());
    io::AnesthesiaActions::UnMarshall(*vFailData, *m_VaporizerFailure);
    if (!m_VaporizerFailure->IsActive()) {
      RemoveVaporizerFailure();
      return true;
    }
    return IsValid(*m_VaporizerFailure);
  }

  if (auto vLoss = dynamic_cast<const SEVentilatorPressureLoss*>(&action)) {
    if (m_VentilatorPressureLoss == nullptr) {
      m_VentilatorPressureLoss = new SEVentilatorPressureLoss();
    }
    auto vLossData = dynamic_cast<CDM::VentilatorPressureLossData*>(actionData.get());
    io::AnesthesiaActions::UnMarshall(*vLossData, *m_VentilatorPressureLoss);
    if (!m_VentilatorPressureLoss->IsActive()) {
      RemoveVentilatorPressureLoss();
      return true;
    }
    return IsValid(*m_VentilatorPressureLoss);
  }

  if (auto Y = dynamic_cast<const SEYPieceDisconnect*>(&action)) {
    if (m_YPieceDisconnect == nullptr) {
      m_YPieceDisconnect = new SEYPieceDisconnect();
    }
    auto YData = dynamic_cast<CDM::YPieceDisconnectData*>(actionData.get());
    io::AnesthesiaActions::UnMarshall(*YData, *m_YPieceDisconnect);
    if (!m_YPieceDisconnect->IsActive()) {
      RemoveYPieceDisconnect();
      return true;
    }
    return IsValid(*m_YPieceDisconnect);
  }
  /// \error Unsupported Action
  Error("Unsupported Action");
  return false;
}
//-------------------------------------------------------------------------------
bool SEAnesthesiaMachineActionCollection::IsValid(const SEAnesthesiaMachineAction& action)
{
  if (!action.IsValid()) {
    Error("Invalid Anesthesia Machine Action");
    return false;
  }
  return true;
}
//-------------------------------------------------------------------------------
bool SEAnesthesiaMachineActionCollection::HasConfiguration() const
{
  return m_Configuration == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEAnesthesiaMachineConfiguration* SEAnesthesiaMachineActionCollection::GetConfiguration() const
{
  return m_Configuration;
}
//-------------------------------------------------------------------------------
void SEAnesthesiaMachineActionCollection::RemoveConfiguration()
{
  SAFE_DELETE(m_Configuration);
}
//-------------------------------------------------------------------------------
bool SEAnesthesiaMachineActionCollection::HasOxygenTankPressureLoss() const
{
  return m_OxygenTankPressureLoss == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEOxygenTankPressureLoss* SEAnesthesiaMachineActionCollection::GetOxygenTankPressureLoss() const
{
  return m_OxygenTankPressureLoss;
}
//-------------------------------------------------------------------------------
void SEAnesthesiaMachineActionCollection::RemoveOxygenTankPressureLoss()
{
  SAFE_DELETE(m_OxygenTankPressureLoss);
}
//-------------------------------------------------------------------------------
bool SEAnesthesiaMachineActionCollection::HasOxygenWallPortPressureLoss() const
{
  return m_OxygenWallPortPressureLoss == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEOxygenWallPortPressureLoss* SEAnesthesiaMachineActionCollection::GetOxygenWallPortPressureLoss() const
{
  return m_OxygenWallPortPressureLoss;
}
//-------------------------------------------------------------------------------
void SEAnesthesiaMachineActionCollection::RemoveOxygenWallPortPressureLoss()
{
  SAFE_DELETE(m_OxygenWallPortPressureLoss);
}
//-------------------------------------------------------------------------------
bool SEAnesthesiaMachineActionCollection::HasExpiratoryValveLeak() const
{
  return m_ExpiratoryValveLeak == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEExpiratoryValveLeak* SEAnesthesiaMachineActionCollection::GetExpiratoryValveLeak() const
{
  return m_ExpiratoryValveLeak;
}
//-------------------------------------------------------------------------------
void SEAnesthesiaMachineActionCollection::RemoveExpiratoryValveLeak()
{
  SAFE_DELETE(m_ExpiratoryValveLeak);
}
//-------------------------------------------------------------------------------
bool SEAnesthesiaMachineActionCollection::HasExpiratoryValveObstruction() const
{
  return m_ExpiratoryValveObstruction == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEExpiratoryValveObstruction* SEAnesthesiaMachineActionCollection::GetExpiratoryValveObstruction() const
{
  return m_ExpiratoryValveObstruction;
}
//-------------------------------------------------------------------------------
void SEAnesthesiaMachineActionCollection::RemoveExpiratoryValveObstruction()
{
  SAFE_DELETE(m_ExpiratoryValveObstruction);
}
//-------------------------------------------------------------------------------
bool SEAnesthesiaMachineActionCollection::HasInspiratoryValveLeak() const
{
  return m_InspiratoryValveLeak == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEInspiratoryValveLeak* SEAnesthesiaMachineActionCollection::GetInspiratoryValveLeak() const
{
  return m_InspiratoryValveLeak;
}
//-------------------------------------------------------------------------------
void SEAnesthesiaMachineActionCollection::RemoveInspiratoryValveLeak()
{
  SAFE_DELETE(m_InspiratoryValveLeak);
}
//-------------------------------------------------------------------------------
bool SEAnesthesiaMachineActionCollection::HasInspiratoryValveObstruction() const
{
  return m_InspiratoryValveObstruction == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEInspiratoryValveObstruction* SEAnesthesiaMachineActionCollection::GetInspiratoryValveObstruction() const
{
  return m_InspiratoryValveObstruction;
}
//-------------------------------------------------------------------------------
void SEAnesthesiaMachineActionCollection::RemoveInspiratoryValveObstruction()
{
  SAFE_DELETE(m_InspiratoryValveObstruction);
}
//-------------------------------------------------------------------------------
bool SEAnesthesiaMachineActionCollection::HasMaskLeak() const
{
  return m_MaskLeak == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEMaskLeak* SEAnesthesiaMachineActionCollection::GetMaskLeak() const
{
  return m_MaskLeak;
}
//-------------------------------------------------------------------------------
void SEAnesthesiaMachineActionCollection::RemoveMaskLeak()
{
  SAFE_DELETE(m_MaskLeak);
}
//-------------------------------------------------------------------------------
bool SEAnesthesiaMachineActionCollection::HasSodaLimeFailure() const
{
  return m_SodaLimeFailure == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SESodaLimeFailure* SEAnesthesiaMachineActionCollection::GetSodaLimeFailure() const
{
  return m_SodaLimeFailure;
}
//-------------------------------------------------------------------------------
void SEAnesthesiaMachineActionCollection::RemoveSodaLimeFailure()
{
  SAFE_DELETE(m_SodaLimeFailure);
}
//-------------------------------------------------------------------------------
bool SEAnesthesiaMachineActionCollection::HasTubeCuffLeak() const
{
  return m_TubeCuffLeak == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SETubeCuffLeak* SEAnesthesiaMachineActionCollection::GetTubeCuffLeak() const
{
  return m_TubeCuffLeak;
}
//-------------------------------------------------------------------------------
void SEAnesthesiaMachineActionCollection::RemoveTubeCuffLeak()
{
  SAFE_DELETE(m_TubeCuffLeak);
}
//-------------------------------------------------------------------------------
bool SEAnesthesiaMachineActionCollection::HasVaporizerFailure() const
{
  return m_VaporizerFailure == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEVaporizerFailure* SEAnesthesiaMachineActionCollection::GetVaporizerFailure() const
{
  return m_VaporizerFailure;
}
//-------------------------------------------------------------------------------
void SEAnesthesiaMachineActionCollection::RemoveVaporizerFailure()
{
  SAFE_DELETE(m_VaporizerFailure);
}
//-------------------------------------------------------------------------------
bool SEAnesthesiaMachineActionCollection::HasVentilatorPressureLoss() const
{
  return m_VentilatorPressureLoss == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEVentilatorPressureLoss* SEAnesthesiaMachineActionCollection::GetVentilatorPressureLoss() const
{
  return m_VentilatorPressureLoss;
}
//-------------------------------------------------------------------------------
void SEAnesthesiaMachineActionCollection::RemoveVentilatorPressureLoss()
{
  SAFE_DELETE(m_VentilatorPressureLoss);
}
//-------------------------------------------------------------------------------
bool SEAnesthesiaMachineActionCollection::HasYPieceDisconnect() const
{
  return m_YPieceDisconnect == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SEYPieceDisconnect* SEAnesthesiaMachineActionCollection::GetYPieceDisconnect() const
{
  return m_YPieceDisconnect;
}
//-------------------------------------------------------------------------------
void SEAnesthesiaMachineActionCollection::RemoveYPieceDisconnect()
{
  SAFE_DELETE(m_YPieceDisconnect);
}
//-------------------------------------------------------------------------------
}