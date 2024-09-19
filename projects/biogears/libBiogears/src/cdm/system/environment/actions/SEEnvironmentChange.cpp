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
#include <biogears/cdm/system/environment/actions/SEEnvironmentChange.h>

#include "io/cdm/Environment.h"
#include "io/cdm/EnvironmentActions.h"

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarHeatConductancePerArea.h>
#include <biogears/cdm/properties/SEScalarHeatResistanceArea.h>
#include <biogears/cdm/properties/SEScalarLengthPerTime.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/io/io-manager.h>

namespace biogears {
SEEnvironmentChange::SEEnvironmentChange(SESubstanceManager& substances)
  : SEEnvironmentAction(substances.GetLogger())
  , m_Substances(substances)
{
  m_Conditions = nullptr;
  InvalidateConditionsFile();
}
//-----------------------------------------------------------------------------
SEEnvironmentChange::~SEEnvironmentChange()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEEnvironmentChange::Clear()
{
  SEEnvironmentAction::Clear();
  InvalidateConditionsFile();
  SAFE_DELETE(m_Conditions);
}
//-----------------------------------------------------------------------------
bool SEEnvironmentChange::IsValid() const
{
  return SEEnvironmentAction::IsValid() && (HasConditions() || HasConditionsFile());
}

//-----------------------------------------------------------------------------
bool SEEnvironmentChange::HasConditions() const
{
  return m_Conditions != nullptr && m_Conditions->IsValid();
}
//-----------------------------------------------------------------------------
SEEnvironmentalConditions& SEEnvironmentChange::GetConditions()
{

  if (m_Conditions == nullptr) {
    m_Conditions = new SEEnvironmentalConditions(m_Substances);
    if (!m_ConditionsFile.empty()) {
      CDM::EnvironmentalConditionsData* ecData;
      std::unique_ptr<CDM::ObjectData> data;

      auto io = m_Substances.GetLogger()->GetIoManager().lock();
      auto possible_path = io->find_resource_file(m_ConditionsFile.c_str());
      if (possible_path.empty()) {
#ifdef BIOGEARS_IO_PRESENT
        size_t content_size;
        auto content = io->get_embedded_resource_file(m_ConditionsFile.c_str(), content_size);
        data = Serializer::ReadBuffer((XMLByte*)content, content_size, m_Substances.GetLogger());
#endif
      } else {
        data = Serializer::ReadFile(possible_path.c_str(), m_Substances.GetLogger());
      }

      ecData = dynamic_cast<CDM::EnvironmentalConditionsData*>(data.get());
      if (ecData == nullptr) {
        std::stringstream ss;
        ss << "EnvironmentalConditions file could not be read : " << m_ConditionsFile << std::endl;
        Error(ss);
      } else {
        m_ConditionsFile = "";
        io::Environment::UnMarshall(*ecData, *m_Conditions);
      }
    }
  }
  return *m_Conditions;
}
//-----------------------------------------------------------------------------
const SEEnvironmentalConditions* SEEnvironmentChange::GetConditions() const
{
  return m_Conditions;
}
//-----------------------------------------------------------------------------
const char* SEEnvironmentChange::GetConditionsFile() const
{
  return m_ConditionsFile.c_str();
}
//-----------------------------------------------------------------------------
void SEEnvironmentChange::SetConditionsFile(const char* fileName)
{
  SetConditionsFile(std::string { fileName });
}
//-----------------------------------------------------------------------------
void SEEnvironmentChange::SetConditionsFile(const std::string& fileName)
{
  if (m_Conditions != nullptr)
    SAFE_DELETE(m_Conditions);
  m_ConditionsFile = fileName;
}
//-----------------------------------------------------------------------------
bool SEEnvironmentChange::HasConditionsFile() const
{
  return m_ConditionsFile.empty() ? false : true;
}
//-----------------------------------------------------------------------------
void SEEnvironmentChange::InvalidateConditionsFile()
{
  m_ConditionsFile = "";
}
//-----------------------------------------------------------------------------
void SEEnvironmentChange::ToString(std::ostream& str) const
{
  str << "Environment Action : Environment Change";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  if (HasConditionsFile())
    str << "\n\tConditions File: ";
  str << m_ConditionsFile;
  if (HasConditions()) {
    str << "\n\tSurroundingType: ";
    m_Conditions->HasSurroundingType() ? str << m_Conditions->GetSurroundingType() : str << "Not Set";
    str << "\n\tAir Velocity: ";
    m_Conditions->HasAirVelocity() ? str << m_Conditions->GetAirVelocity() : str << "Not Set";
    str << "\n\tAmbient Temperature: ";
    m_Conditions->HasAmbientTemperature() ? str << m_Conditions->GetAmbientTemperature() : str << "Not Set";
    str << "\n\tAtmospheric Pressure: ";
    m_Conditions->HasAtmosphericPressure() ? str << m_Conditions->GetAtmosphericPressure() : str << "Not Set";
    str << "\n\tClothing Resistance: ";
    m_Conditions->HasClothingResistance() ? str << m_Conditions->GetClothingResistance() : str << "Not Set";
    str << "\n\tEmissivity: ";
    m_Conditions->HasEmissivity() ? str << m_Conditions->GetEmissivity() : str << "Not Set";
    str << "\n\tMean Radiant Temperature: ";
    m_Conditions->HasMeanRadiantTemperature() ? str << m_Conditions->GetMeanRadiantTemperature() : str << "Not Set";
    str << "\n\tRelative Humidity: ";
    m_Conditions->HasRelativeHumidity() ? str << m_Conditions->GetRelativeHumidity() : str << "Not Set";
    str << "\n\tRespiration Ambient Temperature: ";
    m_Conditions->HasRespirationAmbientTemperature() ? str << m_Conditions->GetRespirationAmbientTemperature() : str << "Not Set";
    if (m_Conditions->HasAmbientGas()) {
      for (SESubstanceFraction* sf : m_Conditions->GetAmbientGases()) {
        str << "\n\tSubstance : " << sf->GetSubstance().GetName() << " Fraction Amount " << sf->GetFractionAmount();
      }
    }
    if (m_Conditions->HasAmbientAerosol()) {
      for (SESubstanceConcentration* sc : m_Conditions->GetAmbientAerosols()) {
        str << "\n\tSubstance : " << sc->GetSubstance().GetName() << " Concentration " << sc->GetConcentration();
      }
    }
  }
  str << std::flush;
}
//-----------------------------------------------------------------------------
}