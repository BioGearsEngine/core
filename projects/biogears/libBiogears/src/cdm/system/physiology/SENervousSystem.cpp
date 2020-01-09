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
constexpr char idHeartRateScale[] = "HeartRateScale";
constexpr char idHeartElastanceScale[] = "HeartElastanceScale";
constexpr char idResistanceScaleExtrasplanchnic[] = "ResistanceScaleExtrasplanchnic";
constexpr char idResistanceScaleMuscle[] = "ResistanceScaleMuscle";
constexpr char idResistanceScaleMyocardium[] = "ResistanceScaleMyocardium";
constexpr char idResistanceScaleSplanchnic[] = "ResistanceScaleSplanchnic";
constexpr char idComplianceScale[] = "ComplianceScale";
constexpr char idPainVisualAnalogueScale[] = "PainVisualAnalogueScale";
constexpr char idLeftEyePupillaryResponse[] = "LeftEyePupillaryResponse";
constexpr char idRightEyePupillaryResponse[] = "RightEyePupillaryResponse";

SENervousSystem::SENervousSystem(Logger* logger)
  : SESystem(logger)
{
  m_HeartRateScale = nullptr;
  m_HeartElastanceScale = nullptr;
  m_ResistanceScaleExtrasplanchnic = nullptr;
  m_ResistanceScaleMuscle = nullptr;
  m_ResistanceScaleMyocardium = nullptr;
  m_ResistanceScaleSplanchnic = nullptr;
  m_ComplianceScale = nullptr;

  m_LeftEyePupillaryResponse = nullptr;
  m_RightEyePupillaryResponse = nullptr;
  m_PainVisualAnalogueScale = nullptr;
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
  SAFE_DELETE(m_HeartRateScale);
  SAFE_DELETE(m_HeartElastanceScale);
  SAFE_DELETE(m_ResistanceScaleExtrasplanchnic);
  SAFE_DELETE(m_ResistanceScaleMuscle);
  SAFE_DELETE(m_ResistanceScaleMyocardium);
  SAFE_DELETE(m_ResistanceScaleSplanchnic);
  SAFE_DELETE(m_ComplianceScale);
  SAFE_DELETE(m_LeftEyePupillaryResponse);
  SAFE_DELETE(m_RightEyePupillaryResponse);
  SAFE_DELETE(m_PainVisualAnalogueScale);
}
//-------------------------------------------------------------------------------
const SEScalar* SENervousSystem::GetScalar(const char* name)
{
  return GetScalar(std::string{ name });
}
//-------------------------------------------------------------------------------
const SEScalar* SENervousSystem::GetScalar(const std::string& name)
{
  if (name == idHeartRateScale)
    return &GetHeartRateScale();
  if (name == idHeartElastanceScale)
    return &GetHeartElastanceScale();
  if (name == idResistanceScaleExtrasplanchnic)
    return &GetResistanceScaleExtrasplanchnic();
  if (name == idResistanceScaleMuscle)
    return &GetResistanceScaleMuscle();
  if (name == idResistanceScaleMyocardium)
    return &GetResistanceScaleMyocardium();
  if (name == idResistanceScaleSplanchnic)
    return &GetResistanceScaleSplanchnic();
  if (name == idComplianceScale)
    return &GetComplianceScale();
  if (name == idPainVisualAnalogueScale)
    return &GetPainVisualAnalogueScale();

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
  if (in.HeartRateScale().present())
    GetHeartRateScale().Load(in.HeartRateScale().get());
  if (in.HeartElastanceScale().present())
    GetHeartElastanceScale().Load(in.HeartElastanceScale().get());
  if (in.ResistanceScaleExtrasplanchnic().present())
    GetResistanceScaleExtrasplanchnic().Load(in.ResistanceScaleExtrasplanchnic().get());
  if (in.ResistanceScaleMuscle().present())
    GetResistanceScaleMuscle().Load(in.ResistanceScaleMuscle().get());
  if (in.ResistanceScaleMyocardium().present())
    GetResistanceScaleMyocardium().Load(in.ResistanceScaleMyocardium().get());
  if (in.ResistanceScaleSplanchnic().present())
    GetResistanceScaleSplanchnic().Load(in.ResistanceScaleSplanchnic().get());
  if (in.ComplianceScale().present())
    GetComplianceScale().Load(in.ComplianceScale().get());
  if (in.PainVisualAnalogueScale().present())
    GetPainVisualAnalogueScale().Load(in.PainVisualAnalogueScale().get());
  if (in.LeftEyePupillaryResponse().present())
    GetLeftEyePupillaryResponse().Load(in.LeftEyePupillaryResponse().get());
  if (in.RightEyePupillaryResponse().present())
    GetRightEyePupillaryResponse().Load(in.RightEyePupillaryResponse().get());
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
  if (m_HeartRateScale != nullptr)
    data.HeartRateScale(std::unique_ptr<CDM::ScalarData>(m_HeartRateScale->Unload()));
  if (m_HeartElastanceScale != nullptr)
    data.HeartElastanceScale(std::unique_ptr<CDM::ScalarData>(m_HeartElastanceScale->Unload()));
  if (m_ResistanceScaleExtrasplanchnic != nullptr)
    data.ResistanceScaleExtrasplanchnic(std::unique_ptr<CDM::ScalarData>(m_ResistanceScaleExtrasplanchnic->Unload()));
  if (m_ResistanceScaleMuscle != nullptr)
    data.ResistanceScaleMuscle(std::unique_ptr<CDM::ScalarData>(m_ResistanceScaleMuscle->Unload()));
  if (m_ResistanceScaleMyocardium != nullptr)
    data.ResistanceScaleMyocardium(std::unique_ptr<CDM::ScalarData>(m_ResistanceScaleMyocardium->Unload()));
  if (m_ResistanceScaleSplanchnic != nullptr)
    data.ResistanceScaleSplanchnic(std::unique_ptr<CDM::ScalarData>(m_ResistanceScaleSplanchnic->Unload()));
  if (m_ComplianceScale != nullptr)
    data.ComplianceScale(std::unique_ptr<CDM::ScalarData>(m_ComplianceScale->Unload()));
  if (m_PainVisualAnalogueScale != nullptr)
    data.PainVisualAnalogueScale(std::unique_ptr<CDM::ScalarData>(m_PainVisualAnalogueScale->Unload()));
  if (m_LeftEyePupillaryResponse != nullptr)
    data.LeftEyePupillaryResponse(std::unique_ptr<CDM::PupillaryResponseData>(m_LeftEyePupillaryResponse->Unload()));
  if (m_RightEyePupillaryResponse != nullptr)
    data.RightEyePupillaryResponse(std::unique_ptr<CDM::PupillaryResponseData>(m_RightEyePupillaryResponse->Unload()));
}

//-------------------------------------------------------------------------------
bool SENervousSystem::HasHeartRateScale() const
{
  return m_HeartRateScale == nullptr ? false : m_HeartRateScale->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetHeartRateScale()
{
  if (m_HeartRateScale == nullptr)
    m_HeartRateScale = new SEScalar();
  return *m_HeartRateScale;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetHeartRateScale() const
{
  if (m_HeartRateScale == nullptr)
    return SEScalar::dNaN();
  return m_HeartRateScale->GetValue();
}
//-------------------------------------------------------------------------------

bool SENervousSystem::HasHeartElastanceScale() const
{
  return m_HeartElastanceScale == nullptr ? false : m_HeartElastanceScale->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetHeartElastanceScale()
{
  if (m_HeartElastanceScale == nullptr)
    m_HeartElastanceScale = new SEScalar();
  return *m_HeartElastanceScale;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetHeartElastanceScale() const
{
  if (m_HeartElastanceScale == nullptr)
    return SEScalar::dNaN();
  return m_HeartElastanceScale->GetValue();
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
bool SENervousSystem::HasResistanceScaleMyocardium() const
{
  return m_ResistanceScaleMyocardium == nullptr ? false : m_ResistanceScaleMyocardium->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetResistanceScaleMyocardium()
{
  if (m_ResistanceScaleMyocardium == nullptr)
    m_ResistanceScaleMyocardium = new SEScalar();
  return *m_ResistanceScaleMyocardium;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetResistanceScaleMyocardium() const
{
  if (m_ResistanceScaleMyocardium == nullptr)
    return SEScalar::dNaN();
  return m_ResistanceScaleMyocardium->GetValue();
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

bool SENervousSystem::HasComplianceScale() const
{
  return m_ComplianceScale == nullptr ? false : m_ComplianceScale->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetComplianceScale()
{
  if (m_ComplianceScale == nullptr)
    m_ComplianceScale = new SEScalar();
  return *m_ComplianceScale;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetComplianceScale() const
{
  if (m_ComplianceScale == nullptr)
    return SEScalar::dNaN();
  return m_ComplianceScale->GetValue();
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

Tree<const char*> SENervousSystem::GetPhysiologyRequestGraph() const
{
  return Tree<const char*>{ classname() }
    .emplace_back(idHeartRateScale)
    .emplace_back(idHeartElastanceScale)
    .emplace_back(idResistanceScaleExtrasplanchnic)
    .emplace_back(idResistanceScaleMuscle)
    .emplace_back(idResistanceScaleMyocardium)
    .emplace_back(idResistanceScaleSplanchnic)
    .emplace_back(idComplianceScale)
    .emplace_back(idPainVisualAnalogueScale)
    .emplace_back(idLeftEyePupillaryResponse)
    .emplace_back(idRightEyePupillaryResponse);
}
}