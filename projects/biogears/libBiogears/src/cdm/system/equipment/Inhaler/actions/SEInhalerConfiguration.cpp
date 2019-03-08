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
#include <biogears/cdm/system/equipment/Inhaler/actions/SEInhalerConfiguration.h>

#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/equipment/Inhaler/SEInhaler.h>

namespace biogears {
SEInhalerConfiguration::SEInhalerConfiguration(SESubstanceManager& substances)
  : SEInhalerAction()
  , m_Substances(substances)
{
  m_Configuration = nullptr;
  InvalidateConfigurationFile();
}
//-----------------------------------------------------------------------------
SEInhalerConfiguration::~SEInhalerConfiguration()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEInhalerConfiguration::Clear()
{
  SEInhalerAction::Clear();
  InvalidateConfigurationFile();
  SAFE_DELETE(m_Configuration);
}
//-----------------------------------------------------------------------------
bool SEInhalerConfiguration::IsValid() const
{
  return SEInhalerAction::IsValid() && (HasConfiguration() || HasConfigurationFile());
}
//-----------------------------------------------------------------------------
bool SEInhalerConfiguration::Load(const CDM::InhalerConfigurationData& in)
{
  SEInhalerAction::Load(in);
  if (in.ConfigurationFile().present())
    SetConfigurationFile(in.ConfigurationFile().get());
  if (in.Configuration().present())
    GetConfiguration().Load(in.Configuration().get());
  return true;
}
//-----------------------------------------------------------------------------
CDM::InhalerConfigurationData* SEInhalerConfiguration::Unload() const
{
  CDM::InhalerConfigurationData* data = new CDM::InhalerConfigurationData();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEInhalerConfiguration::Unload(CDM::InhalerConfigurationData& data) const
{
  SEInhalerAction::Unload(data);
  if (HasConfiguration())
    data.Configuration(std::unique_ptr<CDM::InhalerData>(m_Configuration->Unload()));
  else if (HasConfigurationFile())
    data.ConfigurationFile(m_ConfigurationFile);
}
//-----------------------------------------------------------------------------
bool SEInhalerConfiguration::HasConfiguration() const
{
  return m_Configuration != nullptr;
}
//-----------------------------------------------------------------------------
SEInhaler& SEInhalerConfiguration::GetConfiguration()
{
  m_ConfigurationFile = "";
  if (m_Configuration == nullptr)
    m_Configuration = new SEInhaler(m_Substances);
  return *m_Configuration;
}
//-----------------------------------------------------------------------------
const SEInhaler* SEInhalerConfiguration::GetConfiguration() const
{
  return m_Configuration;
}
//-----------------------------------------------------------------------------
const char* SEInhalerConfiguration::GetConfigurationFile_cStr() const
{
  return m_ConfigurationFile.c_str();
}
//-----------------------------------------------------------------------------
std::string SEInhalerConfiguration::GetConfigurationFile() const
{
  return m_ConfigurationFile;
}
//-----------------------------------------------------------------------------
void SEInhalerConfiguration::SetConfigurationFile(const char* fileName)
{
  SetConfigurationFile(std::string{ fileName });
}
//-----------------------------------------------------------------------------
void SEInhalerConfiguration::SetConfigurationFile(const std::string& fileName)
{
  if (m_Configuration != nullptr)
    SAFE_DELETE(m_Configuration);
  m_ConfigurationFile = fileName;
}
//-----------------------------------------------------------------------------
bool SEInhalerConfiguration::HasConfigurationFile() const
{
  return m_ConfigurationFile.empty() ? false : true;
}
//-----------------------------------------------------------------------------
void SEInhalerConfiguration::InvalidateConfigurationFile()
{
  m_ConfigurationFile = "";
}
//-----------------------------------------------------------------------------
void SEInhalerConfiguration::ToString(std::ostream& str) const
{
  str << "Inhaler Configuration";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  if (HasConfigurationFile())
    str << "\n\tConfiguration File: ";
  str << m_ConfigurationFile;
  if (HasConfiguration()) {
    str << "\n\tState: ";
    m_Configuration->HasState() ? str << m_Configuration->GetState() : str << "Not Set";
    str << "\n\tMetered Dose: ";
    m_Configuration->HasMeteredDose() ? str << m_Configuration->GetMeteredDose() : str << "NaN";
    str << "\n\tNozzle Loss: ";
    m_Configuration->HasNozzleLoss() ? str << m_Configuration->GetNozzleLoss() : str << "NaN";
    str << "\n\tSpacerVolume: ";
    m_Configuration->HasSpacerVolume() ? str << m_Configuration->GetSpacerVolume() : str << "NaN";
    str << "\n\tSubstance: ";
    m_Configuration->HasSubstance() ? str << m_Configuration->GetSubstance()->GetName() : str << "Not Set";
  }
  str << std::flush;
}
//-----------------------------------------------------------------------------
}