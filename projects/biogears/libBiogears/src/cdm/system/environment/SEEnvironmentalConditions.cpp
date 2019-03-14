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
#include <biogears/cdm/system/environment/SEEnvironmentalConditions.h>

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
#include <biogears/cdm/utils/FileUtils.h>

namespace biogears {
SEEnvironmentalConditions::SEEnvironmentalConditions(SESubstanceManager& substances)
  : Loggable(substances.GetLogger())
  , m_Substances(substances)
{
  m_SurroundingType = CDM::enumSurroundingType::value(-1);

  m_AirDensity = nullptr;
  m_AirVelocity = nullptr;
  m_AmbientTemperature = nullptr;
  m_AtmosphericPressure = nullptr;
  m_ClothingResistance = nullptr;
  m_Emissivity = nullptr;
  m_MeanRadiantTemperature = nullptr;
  m_RelativeHumidity = nullptr;
  m_RespirationAmbientTemperature = nullptr;
}
//-----------------------------------------------------------------------------
SEEnvironmentalConditions::~SEEnvironmentalConditions()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEEnvironmentalConditions::Clear()
{
  m_SurroundingType = CDM::enumSurroundingType::value(-1);
  SAFE_DELETE(m_AirDensity);
  SAFE_DELETE(m_AirVelocity);
  SAFE_DELETE(m_AmbientTemperature);
  SAFE_DELETE(m_AtmosphericPressure);
  SAFE_DELETE(m_ClothingResistance);
  SAFE_DELETE(m_Emissivity);
  SAFE_DELETE(m_MeanRadiantTemperature);
  SAFE_DELETE(m_RelativeHumidity);
  SAFE_DELETE(m_RespirationAmbientTemperature);

  DELETE_VECTOR(m_AmbientGases);
  m_cAmbientGases.clear();

  DELETE_VECTOR(m_AmbientAerosols);
  m_cAmbientAerosols.clear();
}
//-----------------------------------------------------------------------------
const SEScalar* SEEnvironmentalConditions::GetScalar(const char* name)
{
  return GetScalar(std::string{ name });
}
//-----------------------------------------------------------------------------
const SEScalar* SEEnvironmentalConditions::GetScalar(const std::string& name)
{
  if (name.compare("AirDensity") == 0)
    return &GetAirDensity();
  if (name.compare("AirVelocity") == 0)
    return &GetAirVelocity();
  if (name.compare("AmbientTemperature") == 0)
    return &GetAmbientTemperature();
  if (name.compare("AtmosphericPressure") == 0)
    return &GetAtmosphericPressure();
  if (name.compare("ClothingResistance") == 0)
    return &GetClothingResistance();
  if (name.compare("Emissivity") == 0)
    return &GetEmissivity();
  if (name.compare("MeanRadiantTemperature") == 0)
    return &GetMeanRadiantTemperature();
  if (name.compare("RelativeHumidity") == 0)
    return &GetRelativeHumidity();
  if (name.compare("RespirationAmbientTemperature") == 0)
    return &GetRespirationAmbientTemperature();
  // I did not support for getting a specific gas/aerosol scalars due to lack of coffee
  return nullptr;
}
//-----------------------------------------------------------------------------
bool SEEnvironmentalConditions::Load(const CDM::EnvironmentalConditionsData& in)
{
  Clear();
  if (in.Name().present())
    m_Name = in.Name().get();
  if (in.SurroundingType().present())
    m_SurroundingType = in.SurroundingType().get();
  if (in.AirDensity().present())
    GetAirDensity().Load(in.AirDensity().get());
  if (in.AirVelocity().present())
    GetAirVelocity().Load(in.AirVelocity().get());
  if (in.AmbientTemperature().present())
    GetAmbientTemperature().Load(in.AmbientTemperature().get());
  if (in.AtmosphericPressure().present())
    GetAtmosphericPressure().Load(in.AtmosphericPressure().get());
  if (in.ClothingResistance().present())
    GetClothingResistance().Load(in.ClothingResistance().get());
  if (in.Emissivity().present())
    GetEmissivity().Load(in.Emissivity().get());
  if (in.MeanRadiantTemperature().present())
    GetMeanRadiantTemperature().Load(in.MeanRadiantTemperature().get());
  if (in.RelativeHumidity().present())
    GetRelativeHumidity().Load(in.RelativeHumidity().get());
  if (in.RespirationAmbientTemperature().present())
    GetRespirationAmbientTemperature().Load(in.RespirationAmbientTemperature().get());

  SESubstance* sub;
  for (const CDM::SubstanceFractionData& sfData : in.AmbientGas()) {
    sub = m_Substances.GetSubstance(sfData.Name());
    if (sub == nullptr) {
      Error("Substance not found : " + sfData.Name());
      return false;
    }
    if (sub->GetState() != CDM::enumSubstanceState::Gas) {
      Error("Substance not gas : " + sfData.Name());
      return false;
    }
    SESubstanceFraction* sf = new SESubstanceFraction(*sub);
    sf->Load(sfData);
    m_AmbientGases.push_back(sf);
    m_cAmbientGases.push_back(sf);
  }

  for (const CDM::SubstanceConcentrationData& scData : in.AmbientAerosol()) {
    sub = m_Substances.GetSubstance(scData.Name());
    if (sub == nullptr) {
      Error("Substance not found : " + scData.Name());
      return false;
    }
    if (sub->GetState() != CDM::enumSubstanceState::Solid && sub->GetState() != CDM::enumSubstanceState::Liquid) {
      Error("Substance not a liquid or solid : " + scData.Name());
      return false;
    }
    SESubstanceConcentration* sc = new SESubstanceConcentration(*sub);
    sc->Load(scData);
    m_AmbientAerosols.push_back(sc);
    m_cAmbientAerosols.push_back(sc);
  }

  return true;
}
//-----------------------------------------------------------------------------
CDM::EnvironmentalConditionsData* SEEnvironmentalConditions::Unload() const
{
  CDM::EnvironmentalConditionsData* data = new CDM::EnvironmentalConditionsData();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEEnvironmentalConditions::Unload(CDM::EnvironmentalConditionsData& data) const
{
  if (HasName()) {
    data.Name(m_Name);
  } else {
    data.Name("Local Environment Conditions");
  }
  if (HasSurroundingType())
    data.SurroundingType(m_SurroundingType);
  if (m_AirDensity != nullptr)
    data.AirDensity(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_AirDensity->Unload()));
  if (m_AirVelocity != nullptr)
    data.AirVelocity(std::unique_ptr<CDM::ScalarLengthPerTimeData>(m_AirVelocity->Unload()));
  if (m_AmbientTemperature != nullptr)
    data.AmbientTemperature(std::unique_ptr<CDM::ScalarTemperatureData>(m_AmbientTemperature->Unload()));
  if (m_AtmosphericPressure != nullptr)
    data.AtmosphericPressure(std::unique_ptr<CDM::ScalarPressureData>(m_AtmosphericPressure->Unload()));
  if (m_ClothingResistance != nullptr)
    data.ClothingResistance(std::unique_ptr<CDM::ScalarHeatResistanceAreaData>(m_ClothingResistance->Unload()));
  if (m_Emissivity != nullptr)
    data.Emissivity(std::unique_ptr<CDM::ScalarFractionData>(m_Emissivity->Unload()));
  if (m_MeanRadiantTemperature != nullptr)
    data.MeanRadiantTemperature(std::unique_ptr<CDM::ScalarTemperatureData>(m_MeanRadiantTemperature->Unload()));
  if (m_RelativeHumidity != nullptr)
    data.RelativeHumidity(std::unique_ptr<CDM::ScalarFractionData>(m_RelativeHumidity->Unload()));
  if (m_RespirationAmbientTemperature != nullptr)
    data.RespirationAmbientTemperature(std::unique_ptr<CDM::ScalarTemperatureData>(m_RespirationAmbientTemperature->Unload()));

  for (SESubstanceFraction* sf : m_AmbientGases)
    data.AmbientGas().push_back(std::unique_ptr<CDM::SubstanceFractionData>(sf->Unload()));

  for (SESubstanceConcentration* sc : m_AmbientAerosols)
    data.AmbientAerosol().push_back(std::unique_ptr<CDM::SubstanceConcentrationData>(sc->Unload()));
}
//-----------------------------------------------------------------------------
void SEEnvironmentalConditions::Merge(const SEEnvironmentalConditions& from)
{
  if (from.HasSurroundingType())
    SetSurroundingType(from.m_SurroundingType);
  COPY_PROPERTY(AirDensity);
  COPY_PROPERTY(AirVelocity);
  COPY_PROPERTY(AmbientTemperature);
  COPY_PROPERTY(AtmosphericPressure);
  COPY_PROPERTY(ClothingResistance);
  COPY_PROPERTY(Emissivity);
  COPY_PROPERTY(MeanRadiantTemperature);
  COPY_PROPERTY(RelativeHumidity);
  COPY_PROPERTY(RespirationAmbientTemperature);

  // Always need to provide a full (fractions sum to 1) substance list that replaces current
  if (from.HasAmbientGas()) {
    double amt;
    double total = 0;
    RemoveAmbientGases();
    SESubstance* sub;
    SESubstanceFraction* sf;
    for (SESubstanceFraction* osf : from.m_AmbientGases) {
      if (&m_Substances != &from.m_Substances) {
        sub = m_Substances.GetSubstance(osf->GetSubstance().GetName());
        if (sub == nullptr) {
          std::stringstream ss;
          ss << "Do not have substance : " << osf->GetSubstance().GetName();
          Error(ss);
        }
      } else
        sub = &osf->GetSubstance();

      sf = new SESubstanceFraction(*sub);
      sf->GetFractionAmount().Set(osf->GetFractionAmount());
      amt = sf->GetFractionAmount().GetValue();
      total += amt;
      m_AmbientGases.push_back(sf);
      m_cAmbientGases.push_back(sf);
      m_Substances.AddActiveSubstance((SESubstance&)sf->m_Substance);
    }
    if (!SEScalar::IsValue(1, total))
      Error("Environment Ambient Substance fractions do not sum to 1");
  }

  if (from.HasAmbientAerosol()) {
    for (SESubstanceConcentration* sc : from.m_AmbientAerosols) {
      SESubstanceConcentration& mine = GetAmbientAerosol(sc->GetSubstance());
      mine.GetConcentration().Set(sc->GetConcentration());
    }
  }
}
//-----------------------------------------------------------------------------
bool SEEnvironmentalConditions::Load(const char* environmentFile)
{
  return Load(std::string{ environmentFile });
}
//-----------------------------------------------------------------------------
bool SEEnvironmentalConditions::Load(const std::string& given)
{
  CDM::EnvironmentalConditionsData* eData;
  std::unique_ptr<CDM::ObjectData> data;

  std::string filepath = given;
  if (!IsAbsolutePath(given) && !TestFirstDirName(given,"environments")) {
    filepath = "environments/";
    filepath += given;
  }
  data = Serializer::ReadFile(filepath, GetLogger());
  eData = dynamic_cast<CDM::EnvironmentalConditionsData*>(data.get());
  if (eData == nullptr) {
    std::stringstream ss;
    ss << "Environmental Conditions file could not be read : " << given << std::endl;
    Error(ss);
    return false;
  }
  if (!Load(*eData))
    return false;

  return true;
}
//-----------------------------------------------------------------------------
std::string SEEnvironmentalConditions::GetName() const
{
  return m_Name;
}
//-----------------------------------------------------------------------------
const char* SEEnvironmentalConditions::GetName_cStr() const
{
  return m_Name.c_str();
}
//-----------------------------------------------------------------------------
void SEEnvironmentalConditions::SetName(const char* name)
{
  m_Name = name;
}
//-----------------------------------------------------------------------------
void SEEnvironmentalConditions::SetName(const std::string& name)
{
  m_Name = name;
}
//-----------------------------------------------------------------------------
bool SEEnvironmentalConditions::HasName() const
{
  return m_Name.empty() ? false : true;
}
//-----------------------------------------------------------------------------
void SEEnvironmentalConditions::InvalidateName()
{
  m_Name = "";
}
//-----------------------------------------------------------------------------
CDM::enumSurroundingType::value SEEnvironmentalConditions::GetSurroundingType() const
{
  return m_SurroundingType;
}
//-----------------------------------------------------------------------------
void SEEnvironmentalConditions::SetSurroundingType(CDM::enumSurroundingType::value state)
{
  m_SurroundingType = state;
}
//-----------------------------------------------------------------------------
bool SEEnvironmentalConditions::HasSurroundingType() const
{
  return m_SurroundingType == ((CDM::enumSurroundingType::value)-1) ? false : true;
}
//-----------------------------------------------------------------------------
void SEEnvironmentalConditions::InvalidateSurroundingType()
{
  m_SurroundingType = (CDM::enumSurroundingType::value)-1;
}
//-----------------------------------------------------------------------------
bool SEEnvironmentalConditions::HasAirDensity() const
{
  return m_AirDensity == nullptr ? false : m_AirDensity->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerVolume& SEEnvironmentalConditions::GetAirDensity()
{
  if (m_AirDensity == nullptr)
    m_AirDensity = new SEScalarMassPerVolume();
  return *m_AirDensity;
}
//-----------------------------------------------------------------------------
double SEEnvironmentalConditions::GetAirDensity(const MassPerVolumeUnit& unit) const
{
  if (m_AirDensity == nullptr)
    return SEScalar::dNaN();
  return m_AirDensity->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEEnvironmentalConditions::HasAirVelocity() const
{
  return m_AirVelocity == nullptr ? false : m_AirVelocity->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarLengthPerTime& SEEnvironmentalConditions::GetAirVelocity()
{
  if (m_AirVelocity == nullptr)
    m_AirVelocity = new SEScalarLengthPerTime();
  return *m_AirVelocity;
}
//-----------------------------------------------------------------------------
double SEEnvironmentalConditions::GetAirVelocity(const LengthPerTimeUnit& unit) const
{
  if (m_AirVelocity == nullptr)
    return SEScalar::dNaN();
  return m_AirVelocity->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEEnvironmentalConditions::HasAmbientTemperature() const
{
  return m_AmbientTemperature == nullptr ? false : m_AmbientTemperature->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarTemperature& SEEnvironmentalConditions::GetAmbientTemperature()
{
  if (m_AmbientTemperature == nullptr)
    m_AmbientTemperature = new SEScalarTemperature();
  return *m_AmbientTemperature;
}
//-----------------------------------------------------------------------------
double SEEnvironmentalConditions::GetAmbientTemperature(const TemperatureUnit& unit) const
{
  if (m_AmbientTemperature == nullptr)
    return SEScalar::dNaN();
  return m_AmbientTemperature->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEEnvironmentalConditions::HasAtmosphericPressure() const
{
  return m_AtmosphericPressure == nullptr ? false : m_AtmosphericPressure->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarPressure& SEEnvironmentalConditions::GetAtmosphericPressure()
{
  if (m_AtmosphericPressure == nullptr)
    m_AtmosphericPressure = new SEScalarPressure();
  return *m_AtmosphericPressure;
}
//-----------------------------------------------------------------------------
double SEEnvironmentalConditions::GetAtmosphericPressure(const PressureUnit& unit) const
{
  if (m_AtmosphericPressure == nullptr)
    return SEScalar::dNaN();
  return m_AtmosphericPressure->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEEnvironmentalConditions::HasClothingResistance() const
{
  return m_ClothingResistance == nullptr ? false : m_ClothingResistance->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarHeatResistanceArea& SEEnvironmentalConditions::GetClothingResistance()
{
  if (m_ClothingResistance == nullptr)
    m_ClothingResistance = new SEScalarHeatResistanceArea();
  return *m_ClothingResistance;
}
//-----------------------------------------------------------------------------
double SEEnvironmentalConditions::GetClothingResistance(const HeatResistanceAreaUnit& unit) const
{
  if (m_ClothingResistance == nullptr)
    return SEScalar::dNaN();
  return m_ClothingResistance->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEEnvironmentalConditions::HasEmissivity() const
{
  return m_Emissivity == nullptr ? false : m_Emissivity->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFraction& SEEnvironmentalConditions::GetEmissivity()
{
  if (m_Emissivity == nullptr)
    m_Emissivity = new SEScalarFraction();
  return *m_Emissivity;
}
//-----------------------------------------------------------------------------
double SEEnvironmentalConditions::GetEmissivity() const
{
  if (m_Emissivity == nullptr)
    return SEScalar::dNaN();
  return m_Emissivity->GetValue();
}
//-----------------------------------------------------------------------------
bool SEEnvironmentalConditions::HasMeanRadiantTemperature() const
{
  return m_MeanRadiantTemperature == nullptr ? false : m_MeanRadiantTemperature->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarTemperature& SEEnvironmentalConditions::GetMeanRadiantTemperature()
{
  if (m_MeanRadiantTemperature == nullptr)
    m_MeanRadiantTemperature = new SEScalarTemperature();
  return *m_MeanRadiantTemperature;
}
//-----------------------------------------------------------------------------
double SEEnvironmentalConditions::GetMeanRadiantTemperature(const TemperatureUnit& unit) const
{
  if (m_MeanRadiantTemperature == nullptr)
    return SEScalar::dNaN();
  return m_MeanRadiantTemperature->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEEnvironmentalConditions::HasRelativeHumidity() const
{
  return m_RelativeHumidity == nullptr ? false : m_RelativeHumidity->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFraction& SEEnvironmentalConditions::GetRelativeHumidity()
{
  if (m_RelativeHumidity == nullptr)
    m_RelativeHumidity = new SEScalarFraction();
  return *m_RelativeHumidity;
}
//-----------------------------------------------------------------------------
double SEEnvironmentalConditions::GetRelativeHumidity() const
{
  if (m_RelativeHumidity == nullptr)
    return SEScalar::dNaN();
  return m_RelativeHumidity->GetValue();
}
//-----------------------------------------------------------------------------
bool SEEnvironmentalConditions::HasRespirationAmbientTemperature() const
{
  return m_RespirationAmbientTemperature == nullptr ? false : m_RespirationAmbientTemperature->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarTemperature& SEEnvironmentalConditions::GetRespirationAmbientTemperature()
{
  if (m_RespirationAmbientTemperature == nullptr)
    m_RespirationAmbientTemperature = new SEScalarTemperature();
  return *m_RespirationAmbientTemperature;
}
//-----------------------------------------------------------------------------
double SEEnvironmentalConditions::GetRespirationAmbientTemperature(const TemperatureUnit& unit) const
{
  if (m_RespirationAmbientTemperature == nullptr)
    return SEScalar::dNaN();
  return m_RespirationAmbientTemperature->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEEnvironmentalConditions::HasAmbientGas() const
{
  return m_AmbientGases.size() == 0 ? false : true;
}
//-----------------------------------------------------------------------------
bool SEEnvironmentalConditions::HasAmbientGas(const SESubstance& s) const
{
  for (const SESubstanceFraction* sf : m_AmbientGases) {
    if (&s == &sf->GetSubstance())
      return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
const std::vector<SESubstanceFraction*>& SEEnvironmentalConditions::GetAmbientGases()
{
  return m_AmbientGases;
}
//-----------------------------------------------------------------------------
const std::vector<const SESubstanceFraction*>& SEEnvironmentalConditions::GetAmbientGases() const
{
  return m_cAmbientGases;
}
//-----------------------------------------------------------------------------
SESubstanceFraction& SEEnvironmentalConditions::GetAmbientGas(SESubstance& s)
{
  for (SESubstanceFraction* sf : m_AmbientGases) {
    if (&s == &sf->GetSubstance())
      return *sf;
  }
  SESubstanceFraction* sf = new SESubstanceFraction(s);
  sf->GetFractionAmount().SetValue(0);
  m_AmbientGases.push_back(sf);
  m_cAmbientGases.push_back(sf);
  return *sf;
}
//-----------------------------------------------------------------------------
const SESubstanceFraction* SEEnvironmentalConditions::GetAmbientGas(const SESubstance& s) const
{
  const SESubstanceFraction* sf = nullptr;
  for (unsigned int i = 0; i < m_AmbientGases.size(); i++) {
    sf = m_AmbientGases[i];
    if (&s == &sf->GetSubstance())
      return sf;
  }
  return sf;
}
//-----------------------------------------------------------------------------
void SEEnvironmentalConditions::RemoveAmbientGas(const SESubstance& s)
{
  const SESubstanceFraction* sf;
  for (unsigned int i = 0; i < m_AmbientGases.size(); i++) {
    sf = m_AmbientGases[i];
    if (&s == &sf->GetSubstance()) {
      m_AmbientGases.erase(m_AmbientGases.begin() + i);
      m_cAmbientGases.erase(m_cAmbientGases.begin() + i);
      delete sf;
    }
  }
}
//-----------------------------------------------------------------------------
void SEEnvironmentalConditions::RemoveAmbientGases()
{
  DELETE_VECTOR(m_AmbientGases);
  m_cAmbientGases.clear();
}
//-----------------------------------------------------------------------------
bool SEEnvironmentalConditions::HasAmbientAerosol() const
{
  return m_AmbientAerosols.size() == 0 ? false : true;
}
//-----------------------------------------------------------------------------
bool SEEnvironmentalConditions::HasAmbientAerosol(const SESubstance& substance) const
{
  for (const SESubstanceConcentration* sc : m_AmbientAerosols) {
    if (&substance == &sc->GetSubstance())
      return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
const std::vector<SESubstanceConcentration*>& SEEnvironmentalConditions::GetAmbientAerosols()
{
  return m_AmbientAerosols;
}
//-----------------------------------------------------------------------------
const std::vector<const SESubstanceConcentration*>& SEEnvironmentalConditions::GetAmbientAerosols() const
{
  return m_cAmbientAerosols;
}
//-----------------------------------------------------------------------------
SESubstanceConcentration& SEEnvironmentalConditions::GetAmbientAerosol(SESubstance& substance)
{
  for (SESubstanceConcentration* sc : m_AmbientAerosols) {
    if (&substance == &sc->GetSubstance())
      return *sc;
  }
  SESubstanceConcentration* sc = new SESubstanceConcentration(substance);
  sc->GetConcentration().SetValue(0, MassPerVolumeUnit::ug_Per_L);
  m_AmbientAerosols.push_back(sc);
  m_cAmbientAerosols.push_back(sc);
  return *sc;
}
//-----------------------------------------------------------------------------
const SESubstanceConcentration* SEEnvironmentalConditions::GetAmbientAerosol(const SESubstance& substance) const
{
  const SESubstanceConcentration* sc = nullptr;
  for (unsigned int i = 0; i < m_AmbientAerosols.size(); i++) {
    sc = m_AmbientAerosols[i];
    if (&substance == &sc->GetSubstance())
      return sc;
  }
  return sc;
}
//-----------------------------------------------------------------------------
void SEEnvironmentalConditions::RemoveAmbientAerosol(const SESubstance& substance)
{
  const SESubstanceConcentration* sc;
  for (unsigned int i = 0; i < m_AmbientAerosols.size(); i++) {
    sc = m_AmbientAerosols[i];
    if (&substance == &sc->GetSubstance()) {
      m_AmbientAerosols.erase(m_AmbientAerosols.begin() + i);
      m_cAmbientAerosols.erase(m_cAmbientAerosols.begin() + i);
      delete sc;
    }
  }
}
//-----------------------------------------------------------------------------
void SEEnvironmentalConditions::RemoveAmbientAerosols()
{
  DELETE_VECTOR(m_AmbientAerosols);
  m_cAmbientAerosols.clear();
}
//-----------------------------------------------------------------------------
}
