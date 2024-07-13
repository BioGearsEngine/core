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

#include "io/cdm/Physiology.h"
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarLength.h>
#include <biogears/cdm/properties/SEScalarPressurePerVolume.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/container/Tree.tci.h>

namespace biogears {
constexpr char idAttentionLapses[] = "AttentionLapses";
constexpr char idBiologicalDebt[] = "BiologicalDebt";
constexpr char idComplianceScale[] = "ComplianceScale";
constexpr char idHeartRateScale[] = "HeartRateScale";
constexpr char idHeartElastanceScale[] = "HeartElastanceScale";
constexpr char idMentalStatus[] = "MentalStatus";
constexpr char idPainVisualAnalogueScale[] = "PainVisualAnalogueScale";
constexpr char idGlasgowComaScalar[] = "GlasgowComaScalar";
constexpr char idLeftEyePupillaryResponse[] = "LeftEyePupillaryResponse";
constexpr char idRightEyePupillaryResponse[] = "RightEyePupillaryResponse";
constexpr char idReactionTime[] = "ReactionTime";
constexpr char idResistanceScaleExtrasplanchnic[] = "ResistanceScaleExtrasplanchnic";
constexpr char idResistanceScaleMuscle[] = "ResistanceScaleMuscle";
constexpr char idResistanceScaleMyocardium[] = "ResistanceScaleMyocardium";
constexpr char idResistanceScaleSplanchnic[] = "ResistanceScaleSplanchnic";
constexpr char idSleepTime[] = "SleepTime";
constexpr char idWakeTime[] = "WakeTime";
constexpr char idRichmondAgitationSedationScale[] = "RichmondAgitationSedationScale";

SENervousSystem::SENervousSystem(Logger* logger)
  : SESystem(logger)
{
  m_AttentionLapses = nullptr;
  m_BiologicalDebt = nullptr;
  m_ComplianceScale = nullptr;
  m_HeartRateScale = nullptr;
  m_HeartElastanceScale = nullptr;
  m_GlasgowComaScalar = nullptr;
  m_LeftEyePupillaryResponse = nullptr;
  m_RightEyePupillaryResponse = nullptr;
  m_MentalStatus = nullptr;
  m_PainVisualAnalogueScale = nullptr;
  m_ReactionTime = nullptr;
  m_ResistanceScaleExtrasplanchnic = nullptr;
  m_ResistanceScaleSplanchnic = nullptr;
  m_ResistanceScaleMuscle = nullptr;
  m_ResistanceScaleMyocardium = nullptr;
  m_SleepTime = nullptr;
  m_SleepState = (SESleepState)-1;
  m_WakeTime = nullptr;
  m_RichmondAgitationSedationScale = nullptr;
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
  SAFE_DELETE(m_AttentionLapses);
  SAFE_DELETE(m_BiologicalDebt);
  SAFE_DELETE(m_HeartRateScale);
  SAFE_DELETE(m_HeartElastanceScale);
  SAFE_DELETE(m_MentalStatus);
  SAFE_DELETE(m_ReactionTime);
  SAFE_DELETE(m_ResistanceScaleExtrasplanchnic);
  SAFE_DELETE(m_ResistanceScaleMuscle);
  SAFE_DELETE(m_ResistanceScaleMyocardium);
  SAFE_DELETE(m_ResistanceScaleSplanchnic);
  SAFE_DELETE(m_ComplianceScale);
  SAFE_DELETE(m_GlasgowComaScalar);
  SAFE_DELETE(m_LeftEyePupillaryResponse);
  SAFE_DELETE(m_RightEyePupillaryResponse);
  SAFE_DELETE(m_PainVisualAnalogueScale);
  SAFE_DELETE(m_SleepTime);
  m_SleepState = (SESleepState)-1;
  SAFE_DELETE(m_WakeTime);
  SAFE_DELETE(m_RichmondAgitationSedationScale);
}
//-------------------------------------------------------------------------------
const SEScalar* SENervousSystem::GetScalar(const char* name)
{
  return GetScalar(std::string { name });
}
//-------------------------------------------------------------------------------
const SEScalar* SENervousSystem::GetScalar(const std::string& name)
{
  if (name == idAttentionLapses)
    return &GetAttentionLapses();
  if (name == idBiologicalDebt)
    return &GetBiologicalDebt();
  if (name == idHeartRateScale)
    return &GetHeartRateScale();
  if (name == idHeartElastanceScale)
    return &GetHeartElastanceScale();
  if (name == idMentalStatus)
    return &GetMentalStatus();
  if (name == idReactionTime)
    return &GetReactionTime();
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
  if (name == idGlasgowComaScalar)
    return &GetGlasgowComaScalar();
  if (name == idPainVisualAnalogueScale)
    return &GetPainVisualAnalogueScale();
  if (name == idSleepTime)
    return &GetSleepTime();
  if (name == idWakeTime)
    return &GetWakeTime();
  if (name == idRichmondAgitationSedationScale)
    return &GetRichmondAgitationSedationScale();

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
  io::Physiology::UnMarshall(in,  *this);
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
  io::Physiology::Marshall(*this, data);
}

//-------------------------------------------------------------------------------
bool SENervousSystem::HasAttentionLapses() const
{
  return m_AttentionLapses == nullptr ? false : m_AttentionLapses->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetAttentionLapses()
{
  if (m_AttentionLapses == nullptr)
    m_AttentionLapses = new SEScalar();
  return *m_AttentionLapses;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetAttentionLapses() const
{
  if (m_AttentionLapses == nullptr)
    return SEScalar::dNaN();
  return m_AttentionLapses->GetValue();
}
//-------------------------------------------------------------------------------
bool SENervousSystem::HasBiologicalDebt() const
{
  return m_BiologicalDebt == nullptr ? false : m_BiologicalDebt->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetBiologicalDebt()
{
  if (m_BiologicalDebt == nullptr)
    m_BiologicalDebt = new SEScalar();
  return *m_BiologicalDebt;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetBiologicalDebt() const
{
  if (m_BiologicalDebt == nullptr)
    return SEScalar::dNaN();
  return m_BiologicalDebt->GetValue();
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

bool SENervousSystem::HasMentalStatus() const
{
  return m_MentalStatus == nullptr ? false : m_MentalStatus->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetMentalStatus()
{
  if (m_MentalStatus == nullptr)
    m_MentalStatus = new SEScalar();
  return *m_MentalStatus;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetMentalStatus() const
{
  if (m_MentalStatus == nullptr)
    return SEScalar::dNaN();
  return m_MentalStatus->GetValue();
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

bool SENervousSystem::HasRichmondAgitationSedationScale() const
{
  return m_RichmondAgitationSedationScale == nullptr ? false : m_RichmondAgitationSedationScale->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetRichmondAgitationSedationScale()
{
  if (m_RichmondAgitationSedationScale == nullptr)
    m_RichmondAgitationSedationScale = new SEScalar();
  return *m_RichmondAgitationSedationScale;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetRichmondAgitationSedationScale() const
{
  if (m_RichmondAgitationSedationScale == nullptr)
    return SEScalar::dNaN();
  return m_RichmondAgitationSedationScale->GetValue();
}
//-------------------------------------------------------------------------------
SEScalar& SENervousSystem::GetGlasgowComaScalar()
{
  if (m_GlasgowComaScalar == nullptr)
    m_GlasgowComaScalar = new SEScalar();
  return *m_GlasgowComaScalar;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetGlasgowComaScalar() const
{
  if (m_GlasgowComaScalar == nullptr)
    return SEScalar::dNaN();
  return m_GlasgowComaScalar->GetValue();
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
    m_LeftEyePupillaryResponse = new SEPupillaryResponse();
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
    m_RightEyePupillaryResponse = new SEPupillaryResponse();
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
bool SENervousSystem::HasReactionTime() const
{
  return m_ReactionTime == nullptr ? false : m_ReactionTime->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarTime& SENervousSystem::GetReactionTime()
{
  if (m_ReactionTime == nullptr)
    m_ReactionTime = new SEScalarTime();
  return *m_ReactionTime;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetReactionTime(const TimeUnit& unit) const
{
  if (m_ReactionTime == nullptr)
    return SEScalar::dNaN();
  return m_ReactionTime->GetValue();
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
bool SENervousSystem::IsAsleep() const
{
  return m_SleepState == ((SESleepState::Sleeping)) ? true : false;
}
//-------------------------------------------------------------------------------
SEScalarTime& SENervousSystem::GetSleepTime()
{
  if (m_SleepTime == nullptr)
    m_SleepTime = new SEScalarTime();
  return *m_SleepTime;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetSleepTime(const TimeUnit& unit) const
{
  if (m_SleepTime == nullptr)
    return SEScalar::dNaN();
  return m_SleepTime->GetValue();
}
//-------------------------------------------------------------------------------

SESleepState SENervousSystem::GetSleepState() const
{
  return m_SleepState;
}
//-------------------------------------------------------------------------------
void SENervousSystem::SetSleepState(SESleepState sleep)
{
  m_SleepState = sleep;
}
//-------------------------------------------------------------------------------
void SENervousSystem::InvalidateSleepState()
{
  m_SleepState = (SESleepState)-1;
}
//-------------------------------------------------------------------------------
bool SENervousSystem::HasSleepState() const
{
  return m_SleepState == ((SESleepState)-1) ? false : true;
}
//-------------------------------------------------------------------------------
bool SENervousSystem::IsAwake() const
{
  return m_SleepState == ((SESleepState::Awake)) ? true : false;
}
//-------------------------------------------------------------------------------
SEScalarTime& SENervousSystem::GetWakeTime()
{
  if (m_WakeTime == nullptr)
    m_WakeTime = new SEScalarTime();
  return *m_WakeTime;
}
//-------------------------------------------------------------------------------
double SENervousSystem::GetWakeTime(const TimeUnit& unit) const
{
  if (m_WakeTime == nullptr)
    return SEScalar::dNaN();
  return m_WakeTime->GetValue();
}
//-------------------------------------------------------------------------------
Tree<const char*> SENervousSystem::GetPhysiologyRequestGraph() const
{
  return Tree<const char*> { classname() }
    .emplace_back(idAttentionLapses)
    .emplace_back(idBiologicalDebt)
    .emplace_back(idHeartRateScale)
    .emplace_back(idHeartElastanceScale)
    .emplace_back(idMentalStatus)
    .emplace_back(idReactionTime)
    .emplace_back(idResistanceScaleExtrasplanchnic)
    .emplace_back(idResistanceScaleMuscle)
    .emplace_back(idResistanceScaleMyocardium)
    .emplace_back(idResistanceScaleSplanchnic)
    .emplace_back(idRichmondAgitationSedationScale)
    .emplace_back(idComplianceScale)
    .emplace_back(idPainVisualAnalogueScale)
    .emplace_back(idLeftEyePupillaryResponse)
    .emplace_back(idRightEyePupillaryResponse)
    .emplace_back(idSleepTime)
    .emplace_back(idWakeTime);
}
//-------------------------------------------------------------------------------
#pragma optimize("", off)
bool SENervousSystem::operator==(SENervousSystem const& rhs) const
{
  if (this == &rhs)
    return true;

  bool equivilant = m_SleepState == rhs.m_SleepState;
  equivilant &= ((m_GlasgowComaScalar && rhs.m_GlasgowComaScalar) ? m_GlasgowComaScalar->operator==(*rhs.m_GlasgowComaScalar) : m_GlasgowComaScalar == rhs.m_GlasgowComaScalar);
  equivilant &= ((m_LeftEyePupillaryResponse && rhs.m_LeftEyePupillaryResponse) ? m_LeftEyePupillaryResponse->operator==(*rhs.m_LeftEyePupillaryResponse) : m_LeftEyePupillaryResponse == rhs.m_LeftEyePupillaryResponse);
  equivilant &= ((m_RightEyePupillaryResponse && rhs.m_RightEyePupillaryResponse) ? m_RightEyePupillaryResponse->operator==(*rhs.m_RightEyePupillaryResponse) : m_RightEyePupillaryResponse == rhs.m_RightEyePupillaryResponse);
  equivilant &= ((m_AttentionLapses && rhs.m_AttentionLapses) ? m_AttentionLapses->operator==(*rhs.m_AttentionLapses) : m_AttentionLapses == rhs.m_AttentionLapses);
  equivilant &= ((m_BiologicalDebt && rhs.m_BiologicalDebt) ? m_BiologicalDebt->operator==(*rhs.m_BiologicalDebt) : m_BiologicalDebt == rhs.m_BiologicalDebt);
  equivilant &= ((m_ComplianceScale && rhs.m_ComplianceScale) ? m_ComplianceScale->operator==(*rhs.m_ComplianceScale) : m_ComplianceScale == rhs.m_ComplianceScale);
  equivilant &= ((m_HeartElastanceScale && rhs.m_HeartElastanceScale) ? m_HeartElastanceScale->operator==(*rhs.m_HeartElastanceScale) : m_HeartElastanceScale == rhs.m_HeartElastanceScale);
  equivilant &= ((m_HeartRateScale && rhs.m_HeartRateScale) ? m_HeartRateScale->operator==(*rhs.m_HeartRateScale) : m_HeartRateScale == rhs.m_HeartRateScale);
  equivilant &= ((m_MentalStatus && rhs.m_MentalStatus) ? m_MentalStatus->operator==(*rhs.m_MentalStatus) : m_MentalStatus == rhs.m_MentalStatus);
  equivilant &= ((m_PainVisualAnalogueScale && rhs.m_PainVisualAnalogueScale) ? m_PainVisualAnalogueScale->operator==(*rhs.m_PainVisualAnalogueScale) : m_PainVisualAnalogueScale == rhs.m_PainVisualAnalogueScale);
  equivilant &= ((m_ResistanceScaleExtrasplanchnic && rhs.m_ResistanceScaleExtrasplanchnic) ? m_ResistanceScaleExtrasplanchnic->operator==(*rhs.m_ResistanceScaleExtrasplanchnic) : m_ResistanceScaleExtrasplanchnic == rhs.m_ResistanceScaleExtrasplanchnic);
  equivilant &= ((m_ResistanceScaleMuscle && rhs.m_ResistanceScaleMuscle) ? m_ResistanceScaleMuscle->operator==(*rhs.m_ResistanceScaleMuscle) : m_ResistanceScaleMuscle == rhs.m_ResistanceScaleMuscle);
  equivilant &= ((m_ResistanceScaleMyocardium && rhs.m_ResistanceScaleMyocardium) ? m_ResistanceScaleMyocardium->operator==(*rhs.m_ResistanceScaleMyocardium) : m_ResistanceScaleMyocardium == rhs.m_ResistanceScaleMyocardium);
  equivilant &= ((m_ResistanceScaleSplanchnic && rhs.m_ResistanceScaleSplanchnic) ? m_ResistanceScaleSplanchnic->operator==(*rhs.m_ResistanceScaleSplanchnic) : m_ResistanceScaleSplanchnic == rhs.m_ResistanceScaleSplanchnic);
  equivilant &= ((m_RichmondAgitationSedationScale && rhs.m_RichmondAgitationSedationScale) ? m_RichmondAgitationSedationScale->operator==(*rhs.m_RichmondAgitationSedationScale) : m_RichmondAgitationSedationScale == rhs.m_RichmondAgitationSedationScale);
  equivilant &= ((m_ReactionTime && rhs.m_ReactionTime) ? m_ReactionTime->operator==(*rhs.m_ReactionTime) : m_ReactionTime == rhs.m_ReactionTime);
  equivilant &= ((m_SleepTime && rhs.m_SleepTime) ? m_SleepTime->operator==(*rhs.m_SleepTime) : m_SleepTime == rhs.m_SleepTime);
  equivilant &= ((m_WakeTime && rhs.m_WakeTime) ? m_WakeTime->operator==(*rhs.m_WakeTime) : m_WakeTime == rhs.m_WakeTime)

    ;
  return equivilant;
}
bool SENervousSystem::operator!=(SENervousSystem const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}
#pragma optimize("", on)