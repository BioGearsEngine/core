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
#include <biogears/cdm/system/equipment/Inhaler/SEInhaler.h>

#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/equipment/Inhaler/actions/SEInhalerConfiguration.h>
#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/container/Tree.tci.h>

namespace biogears {
SEInhaler::SEInhaler(SESubstanceManager& substances)
  : SESystem(substances.GetLogger())
  , m_Substances(substances)
{
  m_State = CDM::enumOnOff::value(-1);
  m_MeteredDose = nullptr;
  m_NozzleLoss = nullptr;
  m_SpacerVolume = nullptr;
  m_Substance = nullptr;
}
//-------------------------------------------------------------------------------

SEInhaler::~SEInhaler()
{
  Clear();
}
//-------------------------------------------------------------------------------

void SEInhaler::Clear()
{
  SESystem::Clear();

  m_State = CDM::enumOnOff::value(-1);
  SAFE_DELETE(m_MeteredDose);
  SAFE_DELETE(m_NozzleLoss);
  SAFE_DELETE(m_SpacerVolume);
  m_Substance = nullptr;
}
//-------------------------------------------------------------------------------

bool SEInhaler::Load(const CDM::InhalerData& in)
{
  Clear();
  if (in.State().present())
    SetState(in.State().get());
  if (in.MeteredDose().present())
    GetMeteredDose().Load(in.MeteredDose().get());
  if (in.NozzleLoss().present())
    GetNozzleLoss().Load(in.NozzleLoss().get());
  if (in.SpacerVolume().present())
    GetSpacerVolume().Load(in.SpacerVolume().get());
  if (in.Substance().present())
    SetSubstance(m_Substances.GetSubstance(in.Substance().get()));
  StateChange();
  return true;
}
//-------------------------------------------------------------------------------

CDM::InhalerData* SEInhaler::Unload() const
{
  CDM::InhalerData* data = new CDM::InhalerData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------

void SEInhaler::Unload(CDM::InhalerData& data) const
{
  if (HasState())
    data.State(m_State);
  if (HasMeteredDose())
    data.MeteredDose(std::unique_ptr<CDM::ScalarMassData>(m_MeteredDose->Unload()));
  if (HasNozzleLoss())
    data.NozzleLoss(std::unique_ptr<CDM::ScalarFractionData>(m_NozzleLoss->Unload()));
  if (HasSpacerVolume())
    data.SpacerVolume(std::unique_ptr<CDM::ScalarVolumeData>(m_SpacerVolume->Unload()));
  if (HasSubstance())
    data.Substance(m_Substance->GetName());
}
//-------------------------------------------------------------------------------
const SEScalar* SEInhaler::GetScalar(const char* name)
{
  return GetScalar(std::string{ name });
}
//-------------------------------------------------------------------------------
const SEScalar* SEInhaler::GetScalar(const std::string& name)
{
  if (name.compare("MeteredDose") == 0)
    return &GetMeteredDose();
  if (name.compare("NozzleLoss") == 0)
    return &GetNozzleLoss();
  if (name.compare("SpacerVolume") == 0)
    return &GetSpacerVolume();
  return nullptr;
}
//-------------------------------------------------------------------------------

void SEInhaler::Merge(const SEInhaler& from)
{
  if (from.HasState())
    SetState(from.m_State);
  COPY_PROPERTY(MeteredDose);
  COPY_PROPERTY(NozzleLoss);
  COPY_PROPERTY(SpacerVolume);
  if (from.m_Substance != nullptr) {
    if (&m_Substances != &from.m_Substances) {
      m_Substance = m_Substances.GetSubstance(from.m_Substance->GetName());
      if (m_Substance == nullptr) {
        m_ss << "Do not have substance : " << from.m_Substance->GetName();
        Error(m_ss);
      }
    } else
      m_Substance = from.m_Substance;
  }
}
//-------------------------------------------------------------------------------

void SEInhaler::ProcessConfiguration(const SEInhalerConfiguration& config)
{
  if (config.HasConfiguration())
    Merge(*config.GetConfiguration());
  else if (config.HasConfigurationFile())
    if (!Load(config.GetConfigurationFile())) // Does NOT merge file in data, Should we?
      Error("Unable to load configuration file", "SEInhaler::ProcessConfiguration");
  StateChange();
}
//-------------------------------------------------------------------------------

bool SEInhaler::Load(const std::string& file)
{
  CDM::InhalerData* pData;
  std::unique_ptr<CDM::ObjectData> data;

  data = Serializer::ReadFile(file, GetLogger());
  pData = dynamic_cast<CDM::InhalerData*>(data.get());
  if (pData == nullptr) {
    std::stringstream ss;
    ss << "Inhaler file could not be read : " << file << std::endl;
    Error(ss);
    return false;
  }
  return Load(*pData);
}
//-------------------------------------------------------------------------------

CDM::enumOnOff::value SEInhaler::GetState() const
{
  return m_State;
}
//-------------------------------------------------------------------------------
void SEInhaler::SetState(CDM::enumOnOff::value state)
{
  m_State = state;
}
//-------------------------------------------------------------------------------
bool SEInhaler::HasState() const
{
  return m_State == ((CDM::enumOnOff::value)-1) ? false : true;
}
//-------------------------------------------------------------------------------
void SEInhaler::InvalidateState()
{
  m_State = (CDM::enumOnOff::value)-1;
}
//-------------------------------------------------------------------------------

bool SEInhaler::HasMeteredDose() const
{
  return m_MeteredDose == nullptr ? false : m_MeteredDose->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarMass& SEInhaler::GetMeteredDose()
{
  if (m_MeteredDose == nullptr)
    m_MeteredDose = new SEScalarMass();
  return *m_MeteredDose;
}
//-------------------------------------------------------------------------------
double SEInhaler::GetMeteredDose(const MassUnit& unit) const
{
  if (m_MeteredDose == nullptr)
    return SEScalar::dNaN();
  return m_MeteredDose->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEInhaler::HasNozzleLoss() const
{
  return m_NozzleLoss == nullptr ? false : m_NozzleLoss->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFraction& SEInhaler::GetNozzleLoss()
{
  if (m_NozzleLoss == nullptr)
    m_NozzleLoss = new SEScalarFraction();
  return *m_NozzleLoss;
}
//-------------------------------------------------------------------------------
double SEInhaler::GetNozzleLoss() const
{
  if (m_NozzleLoss == nullptr)
    return SEScalar::dNaN();
  return m_NozzleLoss->GetValue();
}
//-------------------------------------------------------------------------------

bool SEInhaler::HasSpacerVolume() const
{
  return m_SpacerVolume == nullptr ? false : m_SpacerVolume->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolume& SEInhaler::GetSpacerVolume()
{
  if (m_SpacerVolume == nullptr)
    m_SpacerVolume = new SEScalarVolume();
  return *m_SpacerVolume;
}
//-------------------------------------------------------------------------------
double SEInhaler::GetSpacerVolume(const VolumeUnit& unit) const
{
  if (m_SpacerVolume == nullptr)
    return SEScalar::dNaN();
  return m_SpacerVolume->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEInhaler::HasSubstance() const
{
  return m_Substance != nullptr;
}
//-------------------------------------------------------------------------------
void SEInhaler::SetSubstance(const SESubstance* sub)
{
  if (!sub->HasAerosolization())
    throw CommonDataModelException("Inhaler substance must have aerosolization data");
  m_Substance = sub;
}
//-------------------------------------------------------------------------------
SESubstance* SEInhaler::GetSubstance() const
{
  return (SESubstance*)m_Substance;
}
//-------------------------------------------------------------------------------
Tree<const char*> SEInhaler::GetPhysiologyRequestGraph() const
{
  return {""};
}
}