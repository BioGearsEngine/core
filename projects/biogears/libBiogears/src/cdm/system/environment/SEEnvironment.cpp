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
#include <biogears/cdm/system/environment/SEEnvironment.h>

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarHeatConductancePerArea.h>
#include <biogears/cdm/properties/SEScalarHeatResistanceArea.h>
#include <biogears/cdm/properties/SEScalarLengthPerTime.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPower.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/environment/SEActiveCooling.h>
#include <biogears/cdm/system/environment/SEActiveHeating.h>
#include <biogears/cdm/system/environment/SEAppliedTemperature.h>
#include <biogears/cdm/system/environment/actions/SEEnvironmentChange.h>
#include <biogears/cdm/system/environment/conditions/SEInitialEnvironment.h>
#include <biogears/container/Tree.tci.h>

namespace biogears {
SEEnvironment::SEEnvironment(SESubstanceManager& substances)
  : SESystem(substances.GetLogger())
  , m_Substances(substances)
{
  m_Name = "Local Environment";
  m_Conditions = nullptr;
  m_ConvectiveHeatLoss = nullptr;
  m_ConvectiveHeatTranferCoefficient = nullptr;
  m_EvaporativeHeatLoss = nullptr;
  m_EvaporativeHeatTranferCoefficient = nullptr;
  m_RadiativeHeatLoss = nullptr;
  m_RadiativeHeatTranferCoefficient = nullptr;
  m_RespirationHeatLoss = nullptr;
  m_SkinHeatLoss = nullptr;
}
//-------------------------------------------------------------------------------

SEEnvironment::~SEEnvironment()
{
  Clear();
}
//-------------------------------------------------------------------------------

void SEEnvironment::Clear()
{
  SESystem::Clear();
  m_Name = "";
  SAFE_DELETE(m_Conditions);
  SAFE_DELETE(m_ConvectiveHeatLoss);
  SAFE_DELETE(m_ConvectiveHeatTranferCoefficient);
  SAFE_DELETE(m_EvaporativeHeatLoss);
  SAFE_DELETE(m_EvaporativeHeatTranferCoefficient);
  SAFE_DELETE(m_RadiativeHeatLoss);
  SAFE_DELETE(m_RadiativeHeatTranferCoefficient);
  SAFE_DELETE(m_RespirationHeatLoss);
  SAFE_DELETE(m_SkinHeatLoss);
}
//-------------------------------------------------------------------------------
const SEScalar* SEEnvironment::GetScalar(const char* name)
{
  return GetScalar(std::string{ name });
}
//-------------------------------------------------------------------------------
const SEScalar* SEEnvironment::GetScalar(const std::string& name)
{
  if (name.compare("ConvectiveHeatLoss") == 0) {
    return &GetConvectiveHeatLoss();
  }
  if (name.compare("ConvectiveHeatTranferCoefficient") == 0) {
    return &GetConvectiveHeatTranferCoefficient();
  }
  if (name.compare("EvaporativeHeatLoss") == 0) {
    return &GetEvaporativeHeatLoss();
  }
  if (name.compare("EvaporativeHeatTranferCoefficient") == 0) {
    return &GetEvaporativeHeatTranferCoefficient();
  }
  if (name.compare("RadiativeHeatLoss") == 0) {
    return &GetRadiativeHeatLoss();
  }
  if (name.compare("RadiativeHeatTranferCoefficient") == 0) {
    return &GetRadiativeHeatTranferCoefficient();
  }
  if (name.compare("RespirationHeatLoss") == 0) {
    return &GetRespirationHeatLoss();
  }
  if (name.compare("SkinHeatLoss") == 0) {
    return &GetSkinHeatLoss();
  }

  size_t split = name.find('-');
  if (split != name.npos) {
    std::string child = name.substr(0, split);
    std::string prop = name.substr(split + 1, name.npos);
    if (child == "Conditions") {
      return GetConditions().GetScalar(prop);
    }
  }
  return nullptr;
}
//-------------------------------------------------------------------------------

bool SEEnvironment::Load(const CDM::EnvironmentData& in)
{
  SESystem::Load(in);
  if (in.Name().present()) {
    m_Name = in.Name().get();
  } else {
    m_Name = "Local Environment";
  }
  if (in.Conditions().present()) {
    GetConditions().Load(in.Conditions().get());
  }

  if (in.ConvectiveHeatLoss().present()) {
    GetConvectiveHeatLoss().Load(in.ConvectiveHeatLoss().get());
  }
  if (in.ConvectiveHeatTranferCoefficient().present()) {
    GetConvectiveHeatTranferCoefficient().Load(in.ConvectiveHeatTranferCoefficient().get());
  }
  if (in.EvaporativeHeatLoss().present()) {
    GetEvaporativeHeatLoss().Load(in.EvaporativeHeatLoss().get());
  }
  if (in.EvaporativeHeatTranferCoefficient().present()) {
    GetEvaporativeHeatTranferCoefficient().Load(in.EvaporativeHeatTranferCoefficient().get());
  }
  if (in.RadiativeHeatLoss().present()) {
    GetRadiativeHeatLoss().Load(in.RadiativeHeatLoss().get());
  }
  if (in.RadiativeHeatTranferCoefficient().present()) {
    GetRadiativeHeatTranferCoefficient().Load(in.RadiativeHeatTranferCoefficient().get());
  }
  if (in.RespirationHeatLoss().present()) {
    GetRespirationHeatLoss().Load(in.RespirationHeatLoss().get());
  }
  if (in.SkinHeatLoss().present()) {
    GetSkinHeatLoss().Load(in.SkinHeatLoss().get());
  }

  StateChange();
  return true;
}
//-------------------------------------------------------------------------------
bool SEEnvironment::Load(const char* patientFile)
{
  return Load(std::string(patientFile));
}
//-------------------------------------------------------------------------------
bool SEEnvironment::Load(const std::string& patientFile)
{
  CDM::EnvironmentData* pData;
  std::unique_ptr<CDM::ObjectData> data;

  data = Serializer::ReadFile(patientFile, GetLogger());
  pData = dynamic_cast<CDM::EnvironmentData*>(data.get());
  if (pData == nullptr) {
    std::stringstream ss;
    ss << "Environment file could not be read : " << patientFile << std::endl;
    Error(ss);
    return false;
  }
  return Load(*pData);
}
//-------------------------------------------------------------------------------

CDM::EnvironmentData* SEEnvironment::Unload() const
{
  CDM::EnvironmentData* data = new CDM::EnvironmentData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------

void SEEnvironment::Unload(CDM::EnvironmentData& data) const
{
  SESystem::Unload(data);
  if (HasName()) {
    data.Name(m_Name);
  } else {
    data.Name("Unknown Environment");
  }


  if (HasConditions()) {
    data.Conditions(std::unique_ptr<CDM::EnvironmentalConditionsData>(m_Conditions->Unload()));
  }

  if (m_ConvectiveHeatLoss != nullptr) {
    data.ConvectiveHeatLoss(std::unique_ptr<CDM::ScalarPowerData>(m_ConvectiveHeatLoss->Unload()));
  }
  if (m_ConvectiveHeatTranferCoefficient != nullptr) {
    data.ConvectiveHeatTranferCoefficient(std::unique_ptr<CDM::ScalarHeatConductancePerAreaData>(m_ConvectiveHeatTranferCoefficient->Unload()));
  }
  if (m_EvaporativeHeatLoss != nullptr) {
    data.EvaporativeHeatLoss(std::unique_ptr<CDM::ScalarPowerData>(m_EvaporativeHeatLoss->Unload()));
  }
  if (m_EvaporativeHeatTranferCoefficient != nullptr) {
    data.EvaporativeHeatTranferCoefficient(std::unique_ptr<CDM::ScalarHeatConductancePerAreaData>(m_EvaporativeHeatTranferCoefficient->Unload()));
  }
  if (m_RadiativeHeatLoss != nullptr) {
    data.RadiativeHeatLoss(std::unique_ptr<CDM::ScalarPowerData>(m_RadiativeHeatLoss->Unload()));
  }
  if (m_RadiativeHeatTranferCoefficient != nullptr) {
    data.RadiativeHeatTranferCoefficient(std::unique_ptr<CDM::ScalarHeatConductancePerAreaData>(m_RadiativeHeatTranferCoefficient->Unload()));
  }
  if (m_RespirationHeatLoss != nullptr) {
    data.RespirationHeatLoss(std::unique_ptr<CDM::ScalarPowerData>(m_RespirationHeatLoss->Unload()));
  }
  if (m_SkinHeatLoss != nullptr) {
    data.SkinHeatLoss(std::unique_ptr<CDM::ScalarPowerData>(m_SkinHeatLoss->Unload()));
  }
};
//-------------------------------------------------------------------------------

bool SEEnvironment::ProcessChange(const SEInitialEnvironment& change)
{
  // If we have data then we merge it, if a file was provided
  // we reset and set the environment to the file, so we only have the file data
  if (change.HasConditions()) {
    GetConditions().Merge(*change.GetConditions());
  } else if (change.HasConditionsFile()) {
    if (!GetConditions().Load(change.GetConditionsFile())) // Does NOT merge file in data, Should we?
    {
      /// \error Unable to read Configuration Action file
      Error("Could not read provided SEInitialEnvironment file", "SEEnvironment::ProcessChange");
      return false;
    }
  }
  StateChange();
  return true;
}
//-------------------------------------------------------------------------------

bool SEEnvironment::ProcessChange(const SEEnvironmentChange& change)
{
  // If we have data then we merge it, if a file was provided
  // we reset and set the environment to the file, so we only have the file data
  if (change.HasConditions()) {
    GetConditions().Merge(*change.GetConditions());
  } else if (change.HasConditionsFile()) {
    if (!GetConditions().Load(change.GetConditionsFile())) // Does NOT merge file in data, Should we?
    {
      /// \error Unable to read Configuration Action file
      Error("Could not read provided SEEnvironmentChange file", "SEEnvironment::ProcessChange");
      return false;
    }
  }
  StateChange();
  return true;
}
//-------------------------------------------------------------------------------

std::string SEEnvironment::GetName() const
{
  return m_Name;
}
//-------------------------------------------------------------------------------

const char* SEEnvironment::GetName_cStr() const
{
  return m_Name.c_str();
}
//-------------------------------------------------------------------------------
void SEEnvironment::SetName(const char* name)
{
  m_Name = name;
}
//-------------------------------------------------------------------------------
void SEEnvironment::SetName(const std::string& name)
{
  m_Name = name;
}
//-------------------------------------------------------------------------------
bool SEEnvironment::HasName() const
{
  return m_Name.empty() ? false : true;
}
//-------------------------------------------------------------------------------
void SEEnvironment::InvalidateName()
{
  m_Name = "";
}
//-------------------------------------------------------------------------------
bool SEEnvironment::HasConditions() const
{
  return m_Conditions != nullptr;
}
//-------------------------------------------------------------------------------
SEEnvironmentalConditions& SEEnvironment::GetConditions()
{
  if (m_Conditions == nullptr) {
    m_Conditions = new SEEnvironmentalConditions(m_Substances);
  }
  return *m_Conditions;
}
//-------------------------------------------------------------------------------
const SEEnvironmentalConditions* SEEnvironment::GetConditions() const
{
  return m_Conditions;
}
//-------------------------------------------------------------------------------
void SEEnvironment::RemoveConditions()
{
  SAFE_DELETE(m_Conditions);
}
//-------------------------------------------------------------------------------

bool SEEnvironment::HasConvectiveHeatLoss() const
{
  return m_ConvectiveHeatLoss == nullptr ? false : m_ConvectiveHeatLoss->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPower& SEEnvironment::GetConvectiveHeatLoss()
{
  if (m_ConvectiveHeatLoss == nullptr) {
    m_ConvectiveHeatLoss = new SEScalarPower();
  }
  return *m_ConvectiveHeatLoss;
}
//-------------------------------------------------------------------------------
double SEEnvironment::GetConvectiveHeatLoss(const PowerUnit& unit) const
{
  if (m_ConvectiveHeatLoss == nullptr) {
    return SEScalar::dNaN();
  }
  return m_ConvectiveHeatLoss->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEEnvironment::HasConvectiveHeatTranferCoefficient() const
{
  return m_ConvectiveHeatTranferCoefficient == nullptr ? false : m_ConvectiveHeatTranferCoefficient->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarHeatConductancePerArea& SEEnvironment::GetConvectiveHeatTranferCoefficient()
{
  if (m_ConvectiveHeatTranferCoefficient == nullptr) {
    m_ConvectiveHeatTranferCoefficient = new SEScalarHeatConductancePerArea();
  }
  return *m_ConvectiveHeatTranferCoefficient;
}
//-------------------------------------------------------------------------------
double SEEnvironment::GetConvectiveHeatTranferCoefficient(const HeatConductancePerAreaUnit& unit) const
{
  if (m_ConvectiveHeatTranferCoefficient == nullptr) {
    return SEScalar::dNaN();
  }
  return m_ConvectiveHeatTranferCoefficient->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEEnvironment::HasEvaporativeHeatLoss() const
{
  return m_EvaporativeHeatLoss == nullptr ? false : m_EvaporativeHeatLoss->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPower& SEEnvironment::GetEvaporativeHeatLoss()
{
  if (m_EvaporativeHeatLoss == nullptr) {
    m_EvaporativeHeatLoss = new SEScalarPower();
  }
  return *m_EvaporativeHeatLoss;
}
//-------------------------------------------------------------------------------
double SEEnvironment::GetEvaporativeHeatLoss(const PowerUnit& unit) const
{
  if (m_EvaporativeHeatLoss == nullptr) {
    return SEScalar::dNaN();
  }
  return m_EvaporativeHeatLoss->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEEnvironment::HasEvaporativeHeatTranferCoefficient() const
{
  return m_EvaporativeHeatTranferCoefficient == nullptr ? false : m_EvaporativeHeatTranferCoefficient->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarHeatConductancePerArea& SEEnvironment::GetEvaporativeHeatTranferCoefficient()
{
  if (m_EvaporativeHeatTranferCoefficient == nullptr) {
    m_EvaporativeHeatTranferCoefficient = new SEScalarHeatConductancePerArea();
  }
  return *m_EvaporativeHeatTranferCoefficient;
}
//-------------------------------------------------------------------------------
double SEEnvironment::GetEvaporativeHeatTranferCoefficient(const HeatConductancePerAreaUnit& unit) const
{
  if (m_EvaporativeHeatTranferCoefficient == nullptr) {
    return SEScalar::dNaN();
  }
  return m_EvaporativeHeatTranferCoefficient->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEEnvironment::HasRadiativeHeatLoss() const
{
  return m_RadiativeHeatLoss == nullptr ? false : m_RadiativeHeatLoss->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPower& SEEnvironment::GetRadiativeHeatLoss()
{
  if (m_RadiativeHeatLoss == nullptr) {
    m_RadiativeHeatLoss = new SEScalarPower();
  }
  return *m_RadiativeHeatLoss;
}
//-------------------------------------------------------------------------------
double SEEnvironment::GetRadiativeHeatLoss(const PowerUnit& unit) const
{
  if (m_RadiativeHeatLoss == nullptr) {
    return SEScalar::dNaN();
  }
  return m_RadiativeHeatLoss->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEEnvironment::HasRadiativeHeatTranferCoefficient() const
{
  return m_RadiativeHeatTranferCoefficient == nullptr ? false : m_RadiativeHeatTranferCoefficient->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarHeatConductancePerArea& SEEnvironment::GetRadiativeHeatTranferCoefficient()
{
  if (m_RadiativeHeatTranferCoefficient == nullptr) {
    m_RadiativeHeatTranferCoefficient = new SEScalarHeatConductancePerArea();
  }
  return *m_RadiativeHeatTranferCoefficient;
}
//-------------------------------------------------------------------------------
double SEEnvironment::GetRadiativeHeatTranferCoefficient(const HeatConductancePerAreaUnit& unit) const
{
  if (m_RadiativeHeatTranferCoefficient == nullptr) {
    return SEScalar::dNaN();
  }
  return m_RadiativeHeatTranferCoefficient->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEEnvironment::HasRespirationHeatLoss() const
{
  return m_RespirationHeatLoss == nullptr ? false : m_RespirationHeatLoss->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPower& SEEnvironment::GetRespirationHeatLoss()
{
  if (m_RespirationHeatLoss == nullptr) {
    m_RespirationHeatLoss = new SEScalarPower();
  }
  return *m_RespirationHeatLoss;
}
//-------------------------------------------------------------------------------
double SEEnvironment::GetRespirationHeatLoss(const PowerUnit& unit) const
{
  if (m_RespirationHeatLoss == nullptr) {
    return SEScalar::dNaN();
  }
  return m_RespirationHeatLoss->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEEnvironment::HasSkinHeatLoss() const
{
  return m_SkinHeatLoss == nullptr ? false : m_SkinHeatLoss->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPower& SEEnvironment::GetSkinHeatLoss()
{
  if (m_SkinHeatLoss == nullptr) {
    m_SkinHeatLoss = new SEScalarPower();
  }
  return *m_SkinHeatLoss;
}
//-------------------------------------------------------------------------------
double SEEnvironment::GetSkinHeatLoss(const PowerUnit& unit) const
{
  if (m_SkinHeatLoss == nullptr) {
    return SEScalar::dNaN();
  }
  return m_SkinHeatLoss->GetValue(unit);
}
//-------------------------------------------------------------------------------
Tree<const char*> SEEnvironment::GetPhysiologyRequestGraph() const
{
  return { "" };
}
}