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
#include <biogears/cdm/system/physiology/SENervousSystem.h>

#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarLength.h>
#include <biogears/cdm/properties/SEScalarPressurePerVolume.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/container/Tree.tci.h>

namespace biogears {
  constexpr char idBaroreceptorHeartRateScale[] = "BaroreceptorHeartRateScale";
  constexpr char idBaroreceptorHeartElastanceScale[] = "BaroreceptorHeartElastanceScale";
  constexpr char idBaroreceptorResistanceScale[] = "BaroreceptorResistanceScale";
  constexpr char idBaroreceptorComplianceScale[] = "BaroreceptorComplianceScale";
  constexpr char idChemoreceptorHeartRateScale[] = "ChemoreceptorHeartRateScale";
  constexpr char idChemoreceptorHeartElastanceScale[] = "ChemoreceptorHeartElastanceScale";
  constexpr char idPainVisualAnalogueScale[] = "PainVisualAnalogueScale";
  constexpr char idLeftEyePupillaryResponse[] = "LeftEyePupillaryResponse";
  constexpr char idRightEyePupillaryResponse[] = "RightEyePupillaryResponse";
  constexpr char idResistanceScaleCerebral[] = "ResistanceScaleCerebral";
  constexpr char idResistanceScaleExtrasplanchnic[] = "ResistanceScaleExtrasplanchnic";
  constexpr char idResistanceScaleMuscle[] = "ResistanceScaleMuscle";
  constexpr char idResistanceScaleSplanchnic[] = "ResistanceScaleSplanchnic";
  constexpr char idResistanceScaleVentricle[] = "ResistanceScaleVentricle";

SENervousSystem::SENervousSystem(Logger* logger)
  : SESystem(logger)
{
  m_NumFrequencyComponents = 3;
  m_BaroreceptorFrequencyComponents.clear();
  m_BaroreceptorHeartRateScale = nullptr;
  m_BaroreceptorHeartElastanceScale = nullptr;
  m_BaroreceptorResistanceScale = nullptr;
  m_BaroreceptorComplianceScale = nullptr;
  m_ChemoreceptorHeartRateScale = nullptr;
  m_ChemoreceptorHeartElastanceScale = nullptr;
  m_LeftEyePupillaryResponse = nullptr;
  m_RightEyePupillaryResponse = nullptr;
  m_PainVisualAnalogueScale = nullptr;
  m_ResistanceScaleCerebral = nullptr;
  m_ResistanceScaleExtrasplanchnic = nullptr;
  m_ResistanceScaleMuscle = nullptr;
  m_ResistanceScaleSplanchnic = nullptr;
  m_ResistanceScaleVentricle = nullptr;
}
//-------------------------------------------------------------------------------

SENervousSystem::~SENervousSystem()
{
  Clear();
}
//-------------------------------------------------------------------------------

void SENervousSystem::Clear()
{
  SESystem::Clear();
  m_BaroreceptorFrequencyComponents.clear();
  SAFE_DELETE(m_BaroreceptorHeartRateScale);
  SAFE_DELETE(m_BaroreceptorHeartElastanceScale);
  SAFE_DELETE(m_BaroreceptorResistanceScale);
  SAFE_DELETE(m_BaroreceptorComplianceScale);
  SAFE_DELETE(m_ChemoreceptorHeartRateScale);
  SAFE_DELETE(m_ChemoreceptorHeartElastanceScale);
  SAFE_DELETE(m_LeftEyePupillaryResponse);
  SAFE_DELETE(m_RightEyePupillaryResponse);
  SAFE_DELETE(m_PainVisualAnalogueScale);
  SAFE_DELETE(m_ResistanceScaleCerebral);
  SAFE_DELETE(m_ResistanceScaleExtrasplanchnic);
  SAFE_DELETE(m_ResistanceScaleMuscle);
  SAFE_DELETE(m_ResistanceScaleSplanchnic);
  SAFE_DELETE(m_ResistanceScaleVentricle);
}
//-------------------------------------------------------------------------------
const SEScalar* SENervousSystem::GetScalar(const char* name)
{
  return GetScalar(std::string{ name });
}
//-------------------------------------------------------------------------------
const SEScalar* SENervousSystem::GetScalar(const std::string& name)
{
  if (name == idBaroreceptorHeartRateScale)
    return &GetBaroreceptorHeartRateScale();
  if (name == idBaroreceptorHeartElastanceScale)
    return &GetBaroreceptorHeartElastanceScale();
  if (name == idBaroreceptorResistanceScale)
    return &GetBaroreceptorResistanceScale();
  if (name == idBaroreceptorComplianceScale)
    return &GetBaroreceptorComplianceScale();
  if (name == idChemoreceptorHeartRateScale)
    return &GetChemoreceptorHeartRateScale();
  if (name == idChemoreceptorHeartElastanceScale)
    return &GetChemoreceptorHeartElastanceScale();
  if (name == idPainVisualAnalogueScale)
    return &GetPainVisualAnalogueScale();
  if (name == idResistanceScaleCerebral)
    return &GetResistanceScaleCerebral();
  if (name == idResistanceScaleExtrasplanchnic)
    return &GetResistanceScaleExtrasplanchnic();
  if (name == idResistanceScaleMuscle)
    return &GetResistanceScaleMuscle();
  if (name == idResistanceScaleSplanchnic)
    return &GetResistanceScaleSplanchnic();
  if (name == idResistanceScaleVentricle)
    return &GetResistanceScaleVentricle();


  size_t split = name.find('-');
  if (split != name.npos) {
    std::string child = name.substr(0, split);
    std::string prop = name.substr(split + 1, name.npos);
    if (child == idLeftEyePupillaryResponse)
      return GetLeftEyePupillaryResponse().GetScalar(prop);
    if (child == idRightEyePupillaryResponse)
      return GetRightEyePupillaryResponse().GetScalar(prop);
  }
  return nullptr;
}
//-------------------------------------------------------------------------------

bool SENervousSystem::Load(const CDM::NervousSystemData& in)
{
  SESystem::Load(in);
  m_BaroreceptorFrequencyComponents.clear();
  for (auto bfc : in.BaroreceptorFrequencyComponents()) {
    SEScalarFrequency* component = new SEScalarFrequency();
    component->Load(bfc);
    m_BaroreceptorFrequencyComponents.push_back(component);
  }
  if (in.BaroreceptorHeartRateScale().present())
    GetBaroreceptorHeartRateScale().Load(in.BaroreceptorHeartRateScale().get());
  if (in.BaroreceptorHeartElastanceScale().present())
    GetBaroreceptorHeartElastanceScale().Load(in.BaroreceptorHeartElastanceScale().get());
  if (in.BaroreceptorResistanceScale().present())
    GetBaroreceptorResistanceScale().Load(in.BaroreceptorResistanceScale().get());
  if (in.BaroreceptorComplianceScale().present())
    GetBaroreceptorComplianceScale().Load(in.BaroreceptorComplianceScale().get());
  if (in.ChemoreceptorHeartRateScale().present())
    GetChemoreceptorHeartRateScale().Load(in.ChemoreceptorHeartRateScale().get());
  if (in.ChemoreceptorHeartElastanceScale().present())
    GetChemoreceptorHeartElastanceScale().Load(in.ChemoreceptorHeartElastanceScale().get());
  if (in.PainVisualAnalogueScale().present())
    GetPainVisualAnalogueScale().Load(in.PainVisualAnalogueScale().get());
  if (in.LeftEyePupillaryResponse().present())
    GetLeftEyePupillaryResponse().Load(in.LeftEyePupillaryResponse().get());
  if (in.RightEyePupillaryResponse().present())
    GetRightEyePupillaryResponse().Load(in.RightEyePupillaryResponse().get());
  if (in.ResistanceScaleCerebral().present())
    GetResistanceScaleCerebral().Load(in.ResistanceScaleCerebral().get());
  if (in.ResistanceScaleExtrasplanchnic().present())
    GetResistanceScaleExtrasplanchnic().Load(in.ResistanceScaleExtrasplanchnic().get());
  if (in.ResistanceScaleMuscle().present())
    GetResistanceScaleMuscle().Load(in.ResistanceScaleMuscle().get());
  if (in.ResistanceScaleSplanchnic().present())
    GetResistanceScaleSplanchnic().Load(in.ResistanceScaleSplanchnic().get());
  if (in.ResistanceScaleVentricle().present())
    GetResistanceScaleVentricle().Load(in.ResistanceScaleVentricle().get());
  return true;
}
//-------------------------------------------------------------------------------

CDM::NervousSystemData* SENervousSystem::Unload() const
{
  CDM::NervousSystemData* data = new CDM::NervousSystemData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------

void SENervousSystem::Unload(CDM::NervousSystemData& data) const
{
  SESystem::Unload(data);
  for (auto component : m_BaroreceptorFrequencyComponents) {
    data.BaroreceptorFrequencyComponents().push_back(std::unique_ptr<CDM::ScalarFrequencyData>(component->Unload()));
  }
  if (m_BaroreceptorHeartRateScale != nullptr)
    data.BaroreceptorHeartRateScale(std::unique_ptr<CDM::ScalarData>(m_BaroreceptorHeartRateScale->Unload()));
  if (m_BaroreceptorHeartElastanceScale != nullptr)
    data.BaroreceptorHeartElastanceScale(std::unique_ptr<CDM::ScalarData>(m_BaroreceptorHeartElastanceScale->Unload()));
  if (m_BaroreceptorResistanceScale != nullptr)
    data.BaroreceptorResistanceScale(std::unique_ptr<CDM::ScalarData>(m_BaroreceptorResistanceScale->Unload()));
  if (m_BaroreceptorComplianceScale != nullptr)
    data.BaroreceptorComplianceScale(std::unique_ptr<CDM::ScalarData>(m_BaroreceptorComplianceScale->Unload()));
  if (m_ChemoreceptorHeartRateScale != nullptr)
    data.ChemoreceptorHeartRateScale(std::unique_ptr<CDM::ScalarData>(m_ChemoreceptorHeartRateScale->Unload()));
  if (m_ChemoreceptorHeartElastanceScale != nullptr)
    data.ChemoreceptorHeartElastanceScale(std::unique_ptr<CDM::ScalarData>(m_ChemoreceptorHeartElastanceScale->Unload()));
  if (m_PainVisualAnalogueScale != nullptr)
    data.PainVisualAnalogueScale(std::unique_ptr<CDM::ScalarData>(m_PainVisualAnalogueScale->Unload()));
  if (m_LeftEyePupillaryResponse != nullptr)
    data.LeftEyePupillaryResponse(std::unique_ptr<CDM::PupillaryResponseData>(m_LeftEyePupillaryResponse->Unload()));
  if (m_RightEyePupillaryResponse != nullptr)
    data.RightEyePupillaryResponse(std::unique_ptr<CDM::PupillaryResponseData>(m_RightEyePupillaryResponse->Unload()));
  if (m_ResistanceScaleCerebral != nullptr)
    data.ResistanceScaleCerebral(std::unique_ptr<CDM::ScalarData>(m_ResistanceScaleCerebral->Unload()));
  if (m_ResistanceScaleExtrasplanchnic != nullptr)
    data.ResistanceScaleExtrasplanchnic(std::unique_ptr<CDM::ScalarData>(m_ResistanceScaleExtrasplanchnic->Unload()));
  if (m_ResistanceScaleMuscle != nullptr)
    data.ResistanceScaleMuscle(std::unique_ptr<CDM::ScalarData>(m_ResistanceScaleMuscle->Unload()));
  if (m_ResistanceScaleSplanchnic != nullptr)
    data.ResistanceScaleSplanchnic(std::unique_ptr<CDM::ScalarData>(m_ResistanceScaleSplanchnic->Unload()));
  if (m_ResistanceScaleVentricle != nullptr)
    data.ResistanceScaleVentricle(std::unique_ptr<CDM::ScalarData>(m_ResistanceScaleVentricle->Unload()));
}
//-------------------------------------------------------------------------------
bool SENervousSystem::HasBaroreceptorFrequencyComponents() const
{
  return m_BaroreceptorFrequencyComponents.size() == m_NumFrequencyComponents; //There should be 3 components at all times
}
//-------------------------------------------------------------------------------
std::vector<SEScalarFrequency*> SENervousSystem::GetBaroreceptorFrequencyComponents()
{
  return m_BaroreceptorFrequencyComponents;
}
//-------------------------------------------------------------------------------
std::vector<double> SENervousSystem::GetBaroreceptorFrequencyComponents(const FrequencyUnit& unit) const
{
  std::vector<double> componentsAsDoubles;
  for (auto component : m_BaroreceptorFrequencyComponents) {
    componentsAsDoubles.push_back(component->GetValue(unit));
  }
  return componentsAsDoubles;
}
//-------------------------------------------------------------------------------
bool SENervousSystem::SetBaroreceptorFrequencyComponents(std::vector<double> newComponents, const FrequencyUnit& unit)
{
  if (newComponents.size() != m_NumFrequencyComponents) {
    return false;
  }
  if (m_BaroreceptorFrequencyComponents.size() == 0) {
	  //This means we are initializing this vecotr for the first time
    for (size_t pos = 0; pos < m_NumFrequencyComponents; ++pos) {
      SEScalarFrequency* component = new SEScalarFrequency();
      component->SetValue(newComponents[pos], unit);
      m_BaroreceptorFrequencyComponents.push_back(component);
    }
  }
  else {
    std::vector<double>::iterator dblIt;
    std::vector<SEScalarFrequency*>::iterator sfIt;
    for (dblIt = newComponents.begin(), sfIt = m_BaroreceptorFrequencyComponents.begin(); dblIt != newComponents.end() && sfIt != m_BaroreceptorFrequencyComponents.end(); ++dblIt, ++sfIt) {
      (*sfIt)->SetValue(*dblIt, unit);
    }
  }
  return true;
}
//-------------------------------------------------------------------------------
bool SENervousSystem::HasBaroreceptorHeartRateScale() const
{
  return m_BaroreceptorHeartRateScale == nullptr ? false : m_BaroreceptorHeartRateScale->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetBaroreceptorHeartRateScale()
{
  if (m_BaroreceptorHeartRateScale == nullptr)
    m_BaroreceptorHeartRateScale = new SEScalar();
  return *m_BaroreceptorHeartRateScale;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetBaroreceptorHeartRateScale() const
{
  if (m_BaroreceptorHeartRateScale == nullptr)
    return SEScalar::dNaN();
  return m_BaroreceptorHeartRateScale->GetValue();
}
//-------------------------------------------------------------------------------

bool SENervousSystem::HasBaroreceptorHeartElastanceScale() const
{
  return m_BaroreceptorHeartElastanceScale == nullptr ? false : m_BaroreceptorHeartElastanceScale->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetBaroreceptorHeartElastanceScale()
{
  if (m_BaroreceptorHeartElastanceScale == nullptr)
    m_BaroreceptorHeartElastanceScale = new SEScalar();
  return *m_BaroreceptorHeartElastanceScale;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetBaroreceptorHeartElastanceScale() const
{
  if (m_BaroreceptorHeartElastanceScale == nullptr)
    return SEScalar::dNaN();
  return m_BaroreceptorHeartElastanceScale->GetValue();
}
//-------------------------------------------------------------------------------

bool SENervousSystem::HasBaroreceptorResistanceScale() const
{
  return m_BaroreceptorResistanceScale == nullptr ? false : m_BaroreceptorResistanceScale->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetBaroreceptorResistanceScale()
{
  if (m_BaroreceptorResistanceScale == nullptr)
    m_BaroreceptorResistanceScale = new SEScalar();
  return *m_BaroreceptorResistanceScale;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetBaroreceptorResistanceScale() const
{
  if (m_BaroreceptorResistanceScale == nullptr)
    return SEScalar::dNaN();
  return m_BaroreceptorResistanceScale->GetValue();
}
//-------------------------------------------------------------------------------

bool SENervousSystem::HasBaroreceptorComplianceScale() const
{
  return m_BaroreceptorComplianceScale == nullptr ? false : m_BaroreceptorComplianceScale->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetBaroreceptorComplianceScale()
{
  if (m_BaroreceptorComplianceScale == nullptr)
    m_BaroreceptorComplianceScale = new SEScalar();
  return *m_BaroreceptorComplianceScale;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetBaroreceptorComplianceScale() const
{
  if (m_BaroreceptorComplianceScale == nullptr)
    return SEScalar::dNaN();
  return m_BaroreceptorComplianceScale->GetValue();
}
//-------------------------------------------------------------------------------

bool SENervousSystem::HasChemoreceptorHeartRateScale() const
{
  return m_ChemoreceptorHeartRateScale == nullptr ? false : m_ChemoreceptorHeartRateScale->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetChemoreceptorHeartRateScale()
{
  if (m_ChemoreceptorHeartRateScale == nullptr)
    m_ChemoreceptorHeartRateScale = new SEScalar();
  return *m_ChemoreceptorHeartRateScale;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetChemoreceptorHeartRateScale() const
{
  if (m_ChemoreceptorHeartRateScale == nullptr)
    return SEScalar::dNaN();
  return m_ChemoreceptorHeartRateScale->GetValue();
}
//-------------------------------------------------------------------------------

bool SENervousSystem::HasChemoreceptorHeartElastanceScale() const
{
  return m_ChemoreceptorHeartElastanceScale == nullptr ? false : m_ChemoreceptorHeartElastanceScale->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetChemoreceptorHeartElastanceScale()
{
  if (m_ChemoreceptorHeartElastanceScale == nullptr)
    m_ChemoreceptorHeartElastanceScale = new SEScalar();
  return *m_ChemoreceptorHeartElastanceScale;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetChemoreceptorHeartElastanceScale() const
{
  if (m_ChemoreceptorHeartElastanceScale == nullptr)
    return SEScalar::dNaN();
  return m_ChemoreceptorHeartElastanceScale->GetValue();
}
//-------------------------------------------------------------------------------

bool SENervousSystem::HasPainVisualAnalogueScale() const
{
  return m_PainVisualAnalogueScale == nullptr ? false : m_PainVisualAnalogueScale->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetPainVisualAnalogueScale()
{
  if (m_PainVisualAnalogueScale == nullptr)
    m_PainVisualAnalogueScale = new SEScalar();
  return *m_PainVisualAnalogueScale;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetPainVisualAnalogueScale() const
{
  if (m_PainVisualAnalogueScale == nullptr)
    return SEScalar::dNaN();
  return m_PainVisualAnalogueScale->GetValue();
}
//-------------------------------------------------------------------------------

bool SENervousSystem::HasLeftEyePupillaryResponse() const
{
  return (m_LeftEyePupillaryResponse != nullptr);
}
//-------------------------------------------------------------------------------
SEPupillaryResponse& SENervousSystem::GetLeftEyePupillaryResponse()
{
  if (m_LeftEyePupillaryResponse == nullptr)
    m_LeftEyePupillaryResponse = new SEPupillaryResponse(GetLogger());
  return *m_LeftEyePupillaryResponse;
}
//-------------------------------------------------------------------------------
const SEPupillaryResponse* SENervousSystem::GetLeftEyePupillaryResponse() const
{
  return m_LeftEyePupillaryResponse;
}
//-------------------------------------------------------------------------------
void SENervousSystem::RemoveLeftEyePupillaryResponse()
{
  SAFE_DELETE(m_LeftEyePupillaryResponse);
}
//-------------------------------------------------------------------------------

bool SENervousSystem::HasRightEyePupillaryResponse() const
{
  return (m_RightEyePupillaryResponse != nullptr);
}
//-------------------------------------------------------------------------------
SEPupillaryResponse& SENervousSystem::GetRightEyePupillaryResponse()
{
  if (m_RightEyePupillaryResponse == nullptr)
    m_RightEyePupillaryResponse = new SEPupillaryResponse(GetLogger());
  return *m_RightEyePupillaryResponse;
}
//-------------------------------------------------------------------------------
const SEPupillaryResponse* SENervousSystem::GetRightEyePupillaryResponse() const
{
  return m_RightEyePupillaryResponse;
}
//-------------------------------------------------------------------------------
void SENervousSystem::RemoveRightEyePupillaryResponse()
{
  SAFE_DELETE(m_RightEyePupillaryResponse);
}
//-------------------------------------------------------------------------------

bool SENervousSystem::HasResistanceScaleCerebral() const
{
  return m_ResistanceScaleCerebral == nullptr ? false : m_ResistanceScaleCerebral->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetResistanceScaleCerebral()
{
  if (m_ResistanceScaleCerebral == nullptr)
    m_ResistanceScaleCerebral = new SEScalar();
  return *m_ResistanceScaleCerebral;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetResistanceScaleCerebral() const
{
  if (m_ResistanceScaleCerebral == nullptr)
    return SEScalar::dNaN();
  return m_ResistanceScaleCerebral->GetValue();
}
//-------------------------------------------------------------------------------
bool SENervousSystem::HasResistanceScaleExtrasplanchnic() const
{
  return m_ResistanceScaleExtrasplanchnic == nullptr ? false : m_ResistanceScaleExtrasplanchnic->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetResistanceScaleExtrasplanchnic()
{
  if (m_ResistanceScaleExtrasplanchnic == nullptr)
    m_ResistanceScaleExtrasplanchnic = new SEScalar();
  return *m_ResistanceScaleExtrasplanchnic;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetResistanceScaleExtrasplanchnic() const
{
  if (m_ResistanceScaleExtrasplanchnic == nullptr)
    return SEScalar::dNaN();
  return m_ResistanceScaleExtrasplanchnic->GetValue();
}
//-------------------------------------------------------------------------------
bool SENervousSystem::HasResistanceScaleMuscle() const
{
  return m_ResistanceScaleMuscle == nullptr ? false : m_ResistanceScaleMuscle->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetResistanceScaleMuscle()
{
  if (m_ResistanceScaleMuscle == nullptr)
    m_ResistanceScaleMuscle = new SEScalar();
  return *m_ResistanceScaleMuscle;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetResistanceScaleMuscle() const
{
  if (m_ResistanceScaleMuscle == nullptr)
    return SEScalar::dNaN();
  return m_ResistanceScaleMuscle->GetValue();
}
//-------------------------------------------------------------------------------
bool SENervousSystem::HasResistanceScaleSplanchnic() const
{
  return m_ResistanceScaleSplanchnic == nullptr ? false : m_ResistanceScaleSplanchnic->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetResistanceScaleSplanchnic()
{
  if (m_ResistanceScaleSplanchnic == nullptr)
    m_ResistanceScaleSplanchnic = new SEScalar();
  return *m_ResistanceScaleSplanchnic;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetResistanceScaleSplanchnic() const
{
  if (m_ResistanceScaleSplanchnic == nullptr)
    return SEScalar::dNaN();
  return m_ResistanceScaleSplanchnic->GetValue();
}
//-------------------------------------------------------------------------------
bool SENervousSystem::HasResistanceScaleVentricle() const
{
  return m_ResistanceScaleVentricle == nullptr ? false : m_ResistanceScaleVentricle->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetResistanceScaleVentricle()
{
  if (m_ResistanceScaleVentricle == nullptr)
    m_ResistanceScaleVentricle = new SEScalar();
  return *m_ResistanceScaleVentricle;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetResistanceScaleVentricle() const
{
  if (m_ResistanceScaleVentricle == nullptr)
    return SEScalar::dNaN();
  return m_ResistanceScaleVentricle->GetValue();
}
//-------------------------------------------------------------------------------
bool SENervousSystem::HasResistanceScales() const
{
  return (HasResistanceScaleCerebral() && HasResistanceScaleExtrasplanchnic() && HasResistanceScaleMuscle() && HasResistanceScaleSplanchnic() && HasResistanceScaleVentricle());
}
//-------------------------------------------------------------------------------

Tree<const char*> SENervousSystem::GetPhysiologyRequestGraph() const
{
  return Tree<const char*>{classname()}
    .emplace_back(idBaroreceptorHeartRateScale)
    .emplace_back(idBaroreceptorHeartElastanceScale)
    .emplace_back(idBaroreceptorResistanceScale)
    .emplace_back(idBaroreceptorComplianceScale)
    .emplace_back(idChemoreceptorHeartRateScale)
    .emplace_back(idChemoreceptorHeartElastanceScale)
    .emplace_back(idPainVisualAnalogueScale)
    .emplace_back(idLeftEyePupillaryResponse)
    .emplace_back(idRightEyePupillaryResponse)
    .emplace_back(idResistanceScaleCerebral)
    .emplace_back(idResistanceScaleExtrasplanchnic)
    .emplace_back(idResistanceScaleMuscle)
    .emplace_back(idResistanceScaleSplanchnic)
    .emplace_back(idResistanceScaleVentricle)
  ;
}
}