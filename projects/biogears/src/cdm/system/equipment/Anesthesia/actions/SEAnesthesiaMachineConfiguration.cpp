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
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEAnesthesiaMachineConfiguration.h>

#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachine.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineChamber.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineOxygenBottle.h>

namespace biogears {
SEAnesthesiaMachineConfiguration::SEAnesthesiaMachineConfiguration(SESubstanceManager& substances)
  : SEAnesthesiaMachineAction()
  , m_Substances(substances)
{
  m_Configuration = nullptr;
  InvalidateConfigurationFile();
}
//-----------------------------------------------------------------------------
SEAnesthesiaMachineConfiguration::~SEAnesthesiaMachineConfiguration()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEAnesthesiaMachineConfiguration::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  InvalidateConfigurationFile();
  SAFE_DELETE(m_Configuration);
}
//-----------------------------------------------------------------------------
bool SEAnesthesiaMachineConfiguration::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && (HasConfiguration() || HasConfigurationFile());
}
//-----------------------------------------------------------------------------
bool SEAnesthesiaMachineConfiguration::Load(const CDM::AnesthesiaMachineConfigurationData& in)
{
  SEAnesthesiaMachineAction::Load(in);
  if (in.ConfigurationFile().present())
    SetConfigurationFile(in.ConfigurationFile().get());
  if (in.Configuration().present())
    GetConfiguration().Load(in.Configuration().get());
  return true;
}
//-----------------------------------------------------------------------------
CDM::AnesthesiaMachineConfigurationData* SEAnesthesiaMachineConfiguration::Unload() const
{
  CDM::AnesthesiaMachineConfigurationData* data = new CDM::AnesthesiaMachineConfigurationData();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEAnesthesiaMachineConfiguration::Unload(CDM::AnesthesiaMachineConfigurationData& data) const
{
  SEAnesthesiaMachineAction::Unload(data);
  if (HasConfiguration())
    data.Configuration(std::unique_ptr<CDM::AnesthesiaMachineData>(m_Configuration->Unload()));
  else if (HasConfigurationFile())
    data.ConfigurationFile(m_ConfigurationFile);
}
//-----------------------------------------------------------------------------
bool SEAnesthesiaMachineConfiguration::HasConfiguration() const
{
  return m_Configuration != nullptr;
}
//-----------------------------------------------------------------------------
SEAnesthesiaMachine& SEAnesthesiaMachineConfiguration::GetConfiguration()
{
  m_ConfigurationFile = "";
  if (m_Configuration == nullptr)
    m_Configuration = new SEAnesthesiaMachine(m_Substances);
  return *m_Configuration;
}
//-----------------------------------------------------------------------------
const SEAnesthesiaMachine* SEAnesthesiaMachineConfiguration::GetConfiguration() const
{
  return m_Configuration;
}
//-----------------------------------------------------------------------------
const char* SEAnesthesiaMachineConfiguration::GetConfigurationFile() const
{
  return m_ConfigurationFile.c_str();
}
//-----------------------------------------------------------------------------
void SEAnesthesiaMachineConfiguration::SetConfigurationFile(const char* fileName)
{
  SetConfigurationFile(std::string{ fileName });
}
//-----------------------------------------------------------------------------
void SEAnesthesiaMachineConfiguration::SetConfigurationFile(const std::string& fileName)
{
  if (m_Configuration != nullptr)
    SAFE_DELETE(m_Configuration);
  m_ConfigurationFile = fileName;
}
//-----------------------------------------------------------------------------
bool SEAnesthesiaMachineConfiguration::HasConfigurationFile() const
{
  return m_ConfigurationFile.empty() ? false : true;
}
//-----------------------------------------------------------------------------
void SEAnesthesiaMachineConfiguration::InvalidateConfigurationFile()
{
  m_ConfigurationFile = "";
}
//-----------------------------------------------------------------------------
void SEAnesthesiaMachineConfiguration::ToString(std::ostream& str) const
{
  str << "Anesthesia Machine Configuration";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  if (HasConfigurationFile())
    str << "\n\tConfiguration File: ";
  str << m_ConfigurationFile;
  if (HasConfiguration()) {
    str << "\n\tConnection: ";
    m_Configuration->HasConnection() ? str << m_Configuration->GetConnection() : str << "Not Set";
    str << "\n\tInlet Flow Rate: ";
    m_Configuration->HasInletFlow() ? str << m_Configuration->GetInletFlow() : str << "NaN";
    str << "\n\tInspiratoryExpiratory Ratio: ";
    m_Configuration->HasInspiratoryExpiratoryRatio() ? str << m_Configuration->GetInspiratoryExpiratoryRatio() : str << "NaN";
    str << "\n\tOxygen Fraction: ";
    m_Configuration->HasOxygenFraction() ? str << m_Configuration->GetOxygenFraction() : str << "NaN";
    str << "\n\tOxygen Source: ";
    m_Configuration->HasOxygenSource() ? str << m_Configuration->GetOxygenSource() : str << "Not Set";
    str << "\n\tPositive End Expired Pressure: ";
    m_Configuration->HasPositiveEndExpiredPressure() ? str << m_Configuration->GetPositiveEndExpiredPressure() : str << "NaN";
    str << "\n\tPrimary Gas: ";
    m_Configuration->HasPrimaryGas() ? str << m_Configuration->GetPrimaryGas() : str << "Not Set";
    str << "\n\tRelief Valve Pressure: ";
    m_Configuration->HasReliefValvePressure() ? str << m_Configuration->GetReliefValvePressure() : str << "NaN";
    str << "\n\tRespiratory Rate: ";
    m_Configuration->HasRespiratoryRate() ? str << m_Configuration->GetRespiratoryRate() : str << "NaN";
    str << "\n\tVentilator Pressure: ";
    m_Configuration->HasVentilatorPressure() ? str << m_Configuration->GetVentilatorPressure() : str << "NaN";
    if (m_Configuration->HasLeftChamber()) {
      str << "\n\tLeftChamberState: ";
      m_Configuration->GetLeftChamber().HasState() ? str << m_Configuration->GetLeftChamber().GetState() : str << "Not Supplied";
      str << "\n\tLeftChamberSubstance: ";
      m_Configuration->GetLeftChamber().HasSubstance() ? str << m_Configuration->GetLeftChamber().GetSubstance()->GetName() : str << "Not Set";
      str << "\n\tLeftChamberSubstanceAmount: ";
      m_Configuration->GetLeftChamber().HasSubstanceFraction() ? str << m_Configuration->GetLeftChamber().GetSubstanceFraction() : str << "Not Set";
    }
    if (m_Configuration->HasRightChamber()) {
      str << "\n\tRightChamberState: ";
      m_Configuration->GetRightChamber().HasState() ? str << m_Configuration->GetRightChamber().GetState() : str << "Not Supplied";
      str << "\n\tRightChamberSubstance: ";
      m_Configuration->GetRightChamber().HasSubstance() ? str << m_Configuration->GetRightChamber().GetSubstance()->GetName() : str << "Not Set";
      str << "\n\tRightChamberSubstanceAmount: ";
      m_Configuration->GetRightChamber().HasSubstanceFraction() ? str << m_Configuration->GetRightChamber().GetSubstanceFraction() : str << "Not Set";
    }
    if (m_Configuration->HasOxygenBottleOne()) {
      str << "\n\tOxygenBottleOneVolume: ";
      m_Configuration->GetOxygenBottleOne().HasVolume() ? str << m_Configuration->GetOxygenBottleOne().GetVolume() : str << "Not Set";
    }
    if (m_Configuration->HasOxygenBottleTwo()) {
      str << "\n\tOxygenBottleTwoVolume: ";
      m_Configuration->GetOxygenBottleTwo().HasVolume() ? str << m_Configuration->GetOxygenBottleTwo().GetVolume() : str << "Not Set";
    }
  }
  str << std::flush;
}
  //-----------------------------------------------------------------------------
}