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
void SEAnesthesiaMachineActionCollection::Unload(std::vector<CDM::ActionData*>& to)
{
  if (HasConfiguration())
    to.push_back(io::AnesthesiaActions::factory(GetConfiguration()).release());
  if (HasOxygenTankPressureLoss())
    to.push_back(io::AnesthesiaActions::factory(GetOxygenTankPressureLoss()).release());
  if (HasOxygenWallPortPressureLoss())
    to.push_back(io::AnesthesiaActions::factory(GetOxygenWallPortPressureLoss()).release());
  if (HasExpiratoryValveLeak())
    to.push_back(io::AnesthesiaActions::factory(GetExpiratoryValveLeak()).release());
  if (HasExpiratoryValveObstruction())
    to.push_back(io::AnesthesiaActions::factory(GetExpiratoryValveObstruction()).release());
  if (HasInspiratoryValveLeak())
    to.push_back(io::AnesthesiaActions::factory(GetInspiratoryValveLeak()).release());
  if (HasInspiratoryValveObstruction())
    to.push_back(io::AnesthesiaActions::factory(GetInspiratoryValveObstruction()).release());
  if (HasMaskLeak())
    to.push_back(io::AnesthesiaActions::factory(GetMaskLeak()).release());
  if (HasSodaLimeFailure())
    to.push_back(io::AnesthesiaActions::factory(GetSodaLimeFailure()).release());
  if (HasTubeCuffLeak())
    to.push_back(io::AnesthesiaActions::factory(GetTubeCuffLeak()).release());
  if (HasVaporizerFailure())
    to.push_back(io::AnesthesiaActions::factory(GetVaporizerFailure()).release());
  if (HasVentilatorPressureLoss())
    to.push_back(io::AnesthesiaActions::factory(GetVentilatorPressureLoss()).release());
  if (HasYPieceDisconnect())
    to.push_back(io::AnesthesiaActions::factory(GetYPieceDisconnect()).release());
}
//-------------------------------------------------------------------------------
bool SEAnesthesiaMachineActionCollection::ProcessAction(const SEAnesthesiaMachineAction& action, const PhysiologyEngine& engine)
{
  if (!IsValid(action))
    return false;
  auto bind = io::Actions::factory(&action);
  bool b = ProcessAction((const SEAnesthesiaMachineAction&)*bind, engine);
  return b;
}
//-------------------------------------------------------------------------------
bool SEAnesthesiaMachineActionCollection::ProcessAction(const CDM::AnesthesiaMachineActionData& action, const PhysiologyEngine& engine)
{

  if (const CDM::AnesthesiaMachineConfigurationData* config = dynamic_cast<const CDM::AnesthesiaMachineConfigurationData*>(&action)) {
    if (m_Configuration == nullptr) {
      m_Configuration = new SEAnesthesiaMachineConfiguration(m_Substances);
    }
    io::AnesthesiaActions::UnMarshall(*config, *m_Configuration);
    return IsValid(*m_Configuration);
  }

  if (const CDM::OxygenTankPressureLossData* O2Tank = dynamic_cast<const CDM::OxygenTankPressureLossData*>(&action)) {
    if (m_OxygenTankPressureLoss == nullptr) {
      m_OxygenTankPressureLoss = new SEOxygenTankPressureLoss();
    }
    io::AnesthesiaActions::UnMarshall(*O2Tank, *m_OxygenTankPressureLoss);
    if (!m_OxygenTankPressureLoss->IsActive()) {
      RemoveOxygenTankPressureLoss();
      return true;
    }
    return IsValid(*m_OxygenTankPressureLoss);
  }

  if (const CDM::OxygenWallPortPressureLossData* O2Wall = dynamic_cast<const CDM::OxygenWallPortPressureLossData*>(&action)) {
    if (m_OxygenWallPortPressureLoss == nullptr)
      m_OxygenWallPortPressureLoss = new SEOxygenWallPortPressureLoss();
    io::AnesthesiaActions::UnMarshall(*O2Wall, *m_OxygenWallPortPressureLoss);
    if (!m_OxygenWallPortPressureLoss->IsActive()) {
      RemoveOxygenWallPortPressureLoss();
      return true;
    }
    return IsValid(*m_OxygenWallPortPressureLoss);
  }

  if (const CDM::ExpiratoryValveLeakData* eLeak = dynamic_cast<const CDM::ExpiratoryValveLeakData*>(&action)) {
    if (m_ExpiratoryValveLeak == nullptr)
      m_ExpiratoryValveLeak = new SEExpiratoryValveLeak();
    io::AnesthesiaActions::UnMarshall(*eLeak, *m_ExpiratoryValveLeak);
    if (!m_ExpiratoryValveLeak->IsActive()) {
      RemoveExpiratoryValveLeak();
      return true;
    }
    return IsValid(*m_ExpiratoryValveLeak);
  }

  const CDM::ExpiratoryValveObstructionData* eOb = dynamic_cast<const CDM::ExpiratoryValveObstructionData*>(&action);
  if (eOb != nullptr) {
    if (m_ExpiratoryValveObstruction == nullptr)
      m_ExpiratoryValveObstruction = new SEExpiratoryValveObstruction();
    io::AnesthesiaActions::UnMarshall(*eOb, *m_ExpiratoryValveObstruction);
    if (!m_ExpiratoryValveObstruction->IsActive()) {
      RemoveExpiratoryValveObstruction();
      return true;
    }
    return IsValid(*m_ExpiratoryValveObstruction);
  }

  if (const CDM::InspiratoryValveLeakData* iLeak = dynamic_cast<const CDM::InspiratoryValveLeakData*>(&action)) {
    if (m_InspiratoryValveLeak == nullptr)
      m_InspiratoryValveLeak = new SEInspiratoryValveLeak();
    io::AnesthesiaActions::UnMarshall(*iLeak, *m_InspiratoryValveLeak);
    if (!m_InspiratoryValveLeak->IsActive()) {
      RemoveInspiratoryValveLeak();
      return true;
    }
    return IsValid(*m_InspiratoryValveLeak);
  }

  if (const CDM::InspiratoryValveObstructionData* iOb = dynamic_cast<const CDM::InspiratoryValveObstructionData*>(&action)) {
    if (m_InspiratoryValveObstruction == nullptr)
      m_InspiratoryValveObstruction = new SEInspiratoryValveObstruction();
    io::AnesthesiaActions::UnMarshall(*iOb, *m_InspiratoryValveObstruction);
    if (!m_InspiratoryValveObstruction->IsActive()) {
      RemoveInspiratoryValveObstruction();
      return true;
    }
    return IsValid(*m_InspiratoryValveObstruction);
  }

  if (const CDM::MaskLeakData* mask = dynamic_cast<const CDM::MaskLeakData*>(&action)) {
    if (m_MaskLeak == nullptr)
      m_MaskLeak = new SEMaskLeak();
    io::AnesthesiaActions::UnMarshall(*mask, *m_MaskLeak);
    if (!m_MaskLeak->IsActive()) {
      RemoveMaskLeak();
      return true;
    }
    return IsValid(*m_MaskLeak);
  }

  if (const CDM::SodaLimeFailureData* soda = dynamic_cast<const CDM::SodaLimeFailureData*>(&action)) {
    if (m_SodaLimeFailure == nullptr)
      m_SodaLimeFailure = new SESodaLimeFailure();
    io::AnesthesiaActions::UnMarshall(*soda, *m_SodaLimeFailure);
    if (!m_SodaLimeFailure->IsActive()) {
      RemoveSodaLimeFailure();
      return true;
    }
    return IsValid(*m_SodaLimeFailure);
  }

  if (const CDM::TubeCuffLeakData* tube = dynamic_cast<const CDM::TubeCuffLeakData*>(&action)) {
    if (m_TubeCuffLeak == nullptr)
      m_TubeCuffLeak = new SETubeCuffLeak();
    io::AnesthesiaActions::UnMarshall(*tube, *m_TubeCuffLeak);
    if (!m_TubeCuffLeak->IsActive()) {
      RemoveTubeCuffLeak();
      return true;
    }
    return IsValid(*m_TubeCuffLeak);
  }

  if (const CDM::VaporizerFailureData* vFail = dynamic_cast<const CDM::VaporizerFailureData*>(&action)) {
    if (m_VaporizerFailure == nullptr)
      m_VaporizerFailure = new SEVaporizerFailure();
    io::AnesthesiaActions::UnMarshall(*vFail, *m_VaporizerFailure);
    if (!m_VaporizerFailure->IsActive()) {
      RemoveVaporizerFailure();
      return true;
    }
    return IsValid(*m_VaporizerFailure);
  }

  if (const CDM::VentilatorPressureLossData* vLoss = dynamic_cast<const CDM::VentilatorPressureLossData*>(&action)) {
    if (m_VentilatorPressureLoss == nullptr)
      m_VentilatorPressureLoss = new SEVentilatorPressureLoss();
    io::AnesthesiaActions::UnMarshall(*vLoss, *m_VentilatorPressureLoss);
    if (!m_VentilatorPressureLoss->IsActive()) {
      RemoveVentilatorPressureLoss();
      return true;
    }
    return IsValid(*m_VentilatorPressureLoss);
  }

  if (const CDM::YPieceDisconnectData* Y = dynamic_cast<const CDM::YPieceDisconnectData*>(&action)) {
    if (m_YPieceDisconnect == nullptr)
      m_YPieceDisconnect = new SEYPieceDisconnect();
    io::AnesthesiaActions::UnMarshall(*Y, *m_YPieceDisconnect);
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