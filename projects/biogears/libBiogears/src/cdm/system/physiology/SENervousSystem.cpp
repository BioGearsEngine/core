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
constexpr char idAutoregulatedHeartResistanceScale[] = "AutoregulatedHeartResistanceScale";
constexpr char idAutoregulatedMuscleResistanceScale[] = "AutoregulatedMuscleResistanceScale";
constexpr char idEfferentHeartRateScale[] = "EfferentHeartRateScale";
constexpr char idEfferentHeartElastanceScale[] = "EfferentHeartElastanceScale";
constexpr char idEfferentResistanceScale[] = "EfferentResistanceScale";
constexpr char idEfferentComplianceScale[] = "EfferentComplianceScale";
constexpr char idPainVisualAnalogueScale[] = "PainVisualAnalogueScale";
constexpr char idLeftEyePupillaryResponse[] = "LeftEyePupillaryResponse";
constexpr char idRightEyePupillaryResponse[] = "RightEyePupillaryResponse";

SENervousSystem::SENervousSystem(Logger* logger)
  : SESystem(logger)
{
  m_AutoregulatedHeartResistanceScale = nullptr;
  m_AutoregulatedMuscleResistanceScale = nullptr;
  m_EfferentHeartRateScale = nullptr;
  m_EfferentHeartElastanceScale = nullptr;
  m_EfferentResistanceScale = nullptr;
  m_EfferentComplianceScale = nullptr;

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
  SAFE_DELETE(m_AutoregulatedHeartResistanceScale);
  SAFE_DELETE(m_AutoregulatedMuscleResistanceScale);
  SAFE_DELETE(m_EfferentHeartRateScale);
  SAFE_DELETE(m_EfferentHeartElastanceScale);
  SAFE_DELETE(m_EfferentResistanceScale);
  SAFE_DELETE(m_EfferentComplianceScale);
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
  if (name == idAutoregulatedHeartResistanceScale)
    return &GetAutoregulatedHeartResistanceScale();
  if (name == idAutoregulatedMuscleResistanceScale)
    return &GetAutoregulatedMuscleResistanceScale();
  if (name == idEfferentHeartRateScale)
    return &GetEfferentHeartRateScale();
  if (name == idEfferentHeartElastanceScale)
    return &GetEfferentHeartElastanceScale();
  if (name == idEfferentResistanceScale)
    return &GetEfferentResistanceScale();
  if (name == idEfferentComplianceScale)
    return &GetEfferentComplianceScale();
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
  if (in.AutoregulatedHeartResistanceScale().present())
    GetAutoregulatedHeartResistanceScale().Load(in.AutoregulatedHeartResistanceScale().get());
  if (in.AutoregulatedMuscleResistanceScale().present())
    GetAutoregulatedMuscleResistanceScale().Load(in.AutoregulatedMuscleResistanceScale().get());
  if (in.EfferentHeartRateScale().present())
    GetEfferentHeartRateScale().Load(in.EfferentHeartRateScale().get());
  if (in.EfferentHeartElastanceScale().present())
    GetEfferentHeartElastanceScale().Load(in.EfferentHeartElastanceScale().get());
  if (in.EfferentResistanceScale().present())
    GetEfferentResistanceScale().Load(in.EfferentResistanceScale().get());
  if (in.EfferentComplianceScale().present())
    GetEfferentComplianceScale().Load(in.EfferentComplianceScale().get());
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
  if (m_AutoregulatedHeartResistanceScale != nullptr)
    data.AutoregulatedHeartResistanceScale(std::unique_ptr<CDM::ScalarData>(m_AutoregulatedHeartResistanceScale->Unload()));
  if (m_AutoregulatedMuscleResistanceScale != nullptr)
    data.AutoregulatedMuscleResistanceScale(std::unique_ptr<CDM::ScalarData>(m_AutoregulatedMuscleResistanceScale->Unload()));
  if (m_EfferentHeartRateScale != nullptr)
    data.EfferentHeartRateScale(std::unique_ptr<CDM::ScalarData>(m_EfferentHeartRateScale->Unload()));
  if (m_EfferentHeartElastanceScale != nullptr)
    data.EfferentHeartElastanceScale(std::unique_ptr<CDM::ScalarData>(m_EfferentHeartElastanceScale->Unload()));
  if (m_EfferentResistanceScale != nullptr)
    data.EfferentResistanceScale(std::unique_ptr<CDM::ScalarData>(m_EfferentResistanceScale->Unload()));
  if (m_EfferentComplianceScale != nullptr)
    data.EfferentComplianceScale(std::unique_ptr<CDM::ScalarData>(m_EfferentComplianceScale->Unload()));
  if (m_PainVisualAnalogueScale != nullptr)
    data.PainVisualAnalogueScale(std::unique_ptr<CDM::ScalarData>(m_PainVisualAnalogueScale->Unload()));
  if (m_LeftEyePupillaryResponse != nullptr)
    data.LeftEyePupillaryResponse(std::unique_ptr<CDM::PupillaryResponseData>(m_LeftEyePupillaryResponse->Unload()));
  if (m_RightEyePupillaryResponse != nullptr)
    data.RightEyePupillaryResponse(std::unique_ptr<CDM::PupillaryResponseData>(m_RightEyePupillaryResponse->Unload()));
}
//-------------------------------------------------------------------------------
bool SENervousSystem::HasAutoregulatedHeartResistanceScale() const
{
  return m_AutoregulatedHeartResistanceScale == nullptr ? false : m_AutoregulatedHeartResistanceScale->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetAutoregulatedHeartResistanceScale()
{
  if (m_AutoregulatedHeartResistanceScale == nullptr)
    m_AutoregulatedHeartResistanceScale = new SEScalar();
  return *m_AutoregulatedHeartResistanceScale;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetAutoregulatedHeartResistanceScale() const
{
  if (m_AutoregulatedHeartResistanceScale == nullptr)
    return SEScalar::dNaN();
  return m_AutoregulatedHeartResistanceScale->GetValue();
}
//-------------------------------------------------------------------------------
bool SENervousSystem::HasAutoregulatedMuscleResistanceScale() const
{
  return m_AutoregulatedMuscleResistanceScale == nullptr ? false : m_AutoregulatedMuscleResistanceScale->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetAutoregulatedMuscleResistanceScale()
{
  if (m_AutoregulatedMuscleResistanceScale == nullptr)
    m_AutoregulatedMuscleResistanceScale = new SEScalar();
  return *m_AutoregulatedMuscleResistanceScale;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetAutoregulatedMuscleResistanceScale() const
{
  if (m_AutoregulatedMuscleResistanceScale == nullptr)
    return SEScalar::dNaN();
  return m_AutoregulatedMuscleResistanceScale->GetValue();
}
//-------------------------------------------------------------------------------
bool SENervousSystem::HasEfferentHeartRateScale() const
{
  return m_EfferentHeartRateScale == nullptr ? false : m_EfferentHeartRateScale->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetEfferentHeartRateScale()
{
  if (m_EfferentHeartRateScale == nullptr)
    m_EfferentHeartRateScale = new SEScalar();
  return *m_EfferentHeartRateScale;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetEfferentHeartRateScale() const
{
  if (m_EfferentHeartRateScale == nullptr)
    return SEScalar::dNaN();
  return m_EfferentHeartRateScale->GetValue();
}
//-------------------------------------------------------------------------------

bool SENervousSystem::HasEfferentHeartElastanceScale() const
{
  return m_EfferentHeartElastanceScale == nullptr ? false : m_EfferentHeartElastanceScale->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetEfferentHeartElastanceScale()
{
  if (m_EfferentHeartElastanceScale == nullptr)
    m_EfferentHeartElastanceScale = new SEScalar();
  return *m_EfferentHeartElastanceScale;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetEfferentHeartElastanceScale() const
{
  if (m_EfferentHeartElastanceScale == nullptr)
    return SEScalar::dNaN();
  return m_EfferentHeartElastanceScale->GetValue();
}
//-------------------------------------------------------------------------------

bool SENervousSystem::HasEfferentResistanceScale() const
{
  return m_EfferentResistanceScale == nullptr ? false : m_EfferentResistanceScale->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetEfferentResistanceScale()
{
  if (m_EfferentResistanceScale == nullptr)
    m_EfferentResistanceScale = new SEScalar();
  return *m_EfferentResistanceScale;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetEfferentResistanceScale() const
{
  if (m_EfferentResistanceScale == nullptr)
    return SEScalar::dNaN();
  return m_EfferentResistanceScale->GetValue();
}
//-------------------------------------------------------------------------------

bool SENervousSystem::HasEfferentComplianceScale() const
{
  return m_EfferentComplianceScale == nullptr ? false : m_EfferentComplianceScale->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetEfferentComplianceScale()
{
  if (m_EfferentComplianceScale == nullptr)
    m_EfferentComplianceScale = new SEScalar();
  return *m_EfferentComplianceScale;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetEfferentComplianceScale() const
{
  if (m_EfferentComplianceScale == nullptr)
    return SEScalar::dNaN();
  return m_EfferentComplianceScale->GetValue();
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
    .emplace_back(idAutoregulatedHeartResistanceScale)
    .emplace_back(idAutoregulatedMuscleResistanceScale)
    .emplace_back(idEfferentHeartRateScale)
    .emplace_back(idEfferentHeartElastanceScale)
    .emplace_back(idEfferentResistanceScale)
    .emplace_back(idEfferentComplianceScale)
    .emplace_back(idPainVisualAnalogueScale)
    .emplace_back(idLeftEyePupillaryResponse)
    .emplace_back(idRightEyePupillaryResponse);
}
}