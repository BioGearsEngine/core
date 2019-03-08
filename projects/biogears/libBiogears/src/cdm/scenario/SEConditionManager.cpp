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
#include <biogears/cdm/scenario/SEConditionManager.h>

#include <biogears/cdm/scenario/SECondition.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/patient/conditions/SEChronicAnemia.h>
#include <biogears/cdm/patient/conditions/SEChronicHeartFailure.h>
#include <biogears/cdm/patient/conditions/SEChronicObstructivePulmonaryDisease.h>
#include <biogears/cdm/patient/conditions/SEChronicPericardialEffusion.h>
#include <biogears/cdm/patient/conditions/SEChronicRenalStenosis.h>
#include <biogears/cdm/patient/conditions/SEChronicVentricularSystolicDysfunction.h>
#include <biogears/cdm/patient/conditions/SEDehydration.h>
#include <biogears/cdm/patient/conditions/SEDiabetesType1.h>
#include <biogears/cdm/patient/conditions/SEDiabetesType2.h>
#include <biogears/cdm/patient/conditions/SELobarPneumonia.h>
#include <biogears/cdm/patient/conditions/SEStarvation.h>
#include <biogears/cdm/patient/conditions/SEImpairedAlveolarExchange.h>
#include <biogears/cdm/system/environment/conditions/SEInitialEnvironment.h>

namespace biogears {
SEConditionManager::SEConditionManager(SESubstanceManager& substances)
  : Loggable(substances.GetLogger())
  , m_Substances(substances)
{
  m_Anemia = nullptr;
  m_COPD = nullptr;
  m_HeartFailure = nullptr;
  m_RenalStenosis = nullptr;
  m_Dehydration = nullptr;
  m_DiabetesType1 = nullptr;
  m_DiabetesType2 = nullptr;
  m_Starvation = nullptr;
  m_LobarPneumonia = nullptr;
  m_PericardialEffusion = nullptr;
  m_ImpairedAlveolarExchange = nullptr;
  m_InitialEnvironment = nullptr;
}

SEConditionManager::~SEConditionManager()
{
  Clear();
}

void SEConditionManager::Clear()
{
  m_Anemia = nullptr;
  m_COPD = nullptr;
  m_HeartFailure = nullptr;
  m_RenalStenosis = nullptr;
  m_Dehydration = nullptr;
  m_DiabetesType1 = nullptr;
  m_DiabetesType2 = nullptr;
  m_Starvation = nullptr;
  m_LobarPneumonia = nullptr;
  m_PericardialEffusion = nullptr;
  m_ImpairedAlveolarExchange = nullptr;
  m_InitialEnvironment = nullptr;
  DELETE_VECTOR(m_Conditions);
}

void SEConditionManager::Unload(std::vector<CDM::ConditionData*>& to)
{
  for (SECondition* c : m_Conditions)
    to.push_back(c->Unload());
}

bool SEConditionManager::ProcessCondition(const SECondition& condition)
{
  //if (!IsValid(action))
  //  return false;
  CDM::ConditionData* bind = condition.Unload();
  bool b = ProcessCondition(*bind);
  delete bind;
  return b;
}

bool SEConditionManager::ProcessCondition(const CDM::ConditionData& condition)
{
  const CDM::ChronicAnemiaData* anemiaData = dynamic_cast<const CDM::ChronicAnemiaData*>(&condition);
  if (anemiaData != nullptr) {
    if (HasChronicAnemia()) {
      Error("Cannot have multiple Anemia conditions");
      return false;
    }
    m_Anemia = new SEChronicAnemia();
    m_Anemia->Load(*anemiaData);
    m_Conditions.push_back(m_Anemia);
    return true;
  }

  const CDM::ChronicObstructivePulmonaryDiseaseData* copdData = dynamic_cast<const CDM::ChronicObstructivePulmonaryDiseaseData*>(&condition);
  if (copdData != nullptr) {
    if (HasChronicObstructivePulmonaryDisease()) {
      Error("Cannot have multiple COPD conditions");
      return false;
    }
    m_COPD = new SEChronicObstructivePulmonaryDisease();
    m_COPD->Load(*copdData);
    m_Conditions.push_back(m_COPD);
    return true;
  }

  const CDM::ChronicHeartFailureData* heartFailureData = dynamic_cast<const CDM::ChronicHeartFailureData*>(&condition);
  if (heartFailureData != nullptr) {
    if (HasChronicHeartFailure()) {
      Error("Cannot have multiple Heart Failure conditions");
      return false;
    }
    const CDM::ChronicVentricularSystolicDysfunctionData* vsdData = dynamic_cast<const CDM::ChronicVentricularSystolicDysfunctionData*>(&condition);
    if (vsdData != nullptr) {
      m_HeartFailure = new SEChronicVentricularSystolicDysfunction();
      m_HeartFailure->Load(*vsdData);
      m_Conditions.push_back(m_HeartFailure);
      return true;
    }
    Error("Unknown Heart Failure condition");
    return false;
  }

  const CDM::ChronicPericardialEffusionData* peData = dynamic_cast<const CDM::ChronicPericardialEffusionData*>(&condition);
  if (peData != nullptr) {
    if (HasChronicPericardialEffusion()) {
      Error("Cannot have multiple Pericardial Effusion conditions");
      return false;
    }
    m_PericardialEffusion = new SEChronicPericardialEffusion();
    m_PericardialEffusion->Load(*peData);
    m_Conditions.push_back(m_PericardialEffusion);
    return true;
  }

  const CDM::ChronicRenalStenosisData* renalStenosisData = dynamic_cast<const CDM::ChronicRenalStenosisData*>(&condition);
  if (renalStenosisData != nullptr) {
    if (HasChronicRenalStenosis()) {
      Error("Cannot have multiple Renal Stenosis conditions");
      return false;
    }
    m_RenalStenosis = new SEChronicRenalStenosis();
    m_RenalStenosis->Load(*renalStenosisData);
    m_Conditions.push_back(m_RenalStenosis);
    return true;
  }

  const CDM::DehydrationData* dehydrationData = dynamic_cast<const CDM::DehydrationData*>(&condition);
  if (dehydrationData != nullptr) {
    if (HasDehydration()) {
      Error("Cannot have multiple Dehydration conditions");
      return false;
    }
    m_Dehydration = new SEDehydration();
    m_Dehydration->Load(*dehydrationData);
    m_Conditions.push_back(m_Dehydration);
    return true;
  }

  const CDM::DiabetesType1Data* diabetes1Data = dynamic_cast<const CDM::DiabetesType1Data*>(&condition);
  if (diabetes1Data != nullptr) {
    if (HasDiabetesType1()) {
      Error("Cannot have multiple DiabetesType1 conditions");
      return false;
    }
    m_DiabetesType1 = new SEDiabetesType1();
    m_DiabetesType1->Load(*diabetes1Data);
    m_Conditions.push_back(m_DiabetesType1);
    return true;
  }

  const CDM::DiabetesType2Data* diabetes2Data = dynamic_cast<const CDM::DiabetesType2Data*>(&condition);
  if (diabetes2Data != nullptr) {
    if (HasDiabetesType2()) {
      Error("Cannot have multiple DiabetesType2 conditions");
      return false;
    }
    m_DiabetesType2 = new SEDiabetesType2();
    m_DiabetesType2->Load(*diabetes2Data);
    m_Conditions.push_back(m_DiabetesType2);
    return true;
  }

  const CDM::StarvationData* starvationData = dynamic_cast<const CDM::StarvationData*>(&condition);
  if (starvationData != nullptr) {
    if (HasStarvation()) {
      Error("Cannot have multiple Starvation conditions");
      return false;
    }
    m_Starvation = new SEStarvation();
    m_Starvation->Load(*starvationData);
    m_Conditions.push_back(m_Starvation);
    return true;
  }

  const CDM::ImpairedAlveolarExchangeData* impairedAlvioliData = dynamic_cast<const CDM::ImpairedAlveolarExchangeData*>(&condition);
  if (impairedAlvioliData != nullptr) {
    if (HasImpairedAlveolarExchange()) {
      Error("Cannot have multiple Impaired Alveolar Exchange conditions");
      return false;
    }
    m_ImpairedAlveolarExchange = new SEImpairedAlveolarExchange();
    m_ImpairedAlveolarExchange->Load(*impairedAlvioliData);
    m_Conditions.push_back(m_ImpairedAlveolarExchange);
    return true;
  }

  const CDM::LobarPneumoniaData* pneumoniaData = dynamic_cast<const CDM::LobarPneumoniaData*>(&condition);
  if (pneumoniaData != nullptr) {
    if (HasLobarPneumonia()) {
      Error("Cannot have multiple Lobar Pneumonia conditions");
      return false;
    }
    m_LobarPneumonia = new SELobarPneumonia();
    m_LobarPneumonia->Load(*pneumoniaData);
    m_Conditions.push_back(m_LobarPneumonia);
    return true;
  }

  const CDM::InitialEnvironmentData* environmentData = dynamic_cast<const CDM::InitialEnvironmentData*>(&condition);
  if (environmentData != nullptr) {
    if (HasInitialEnvironment()) {
      Error("Cannot have multiple Initial Environment conditions");
      return false;
    }
    m_InitialEnvironment = new SEInitialEnvironment(m_Substances);
    m_InitialEnvironment->Load(*environmentData);
    m_Conditions.push_back(m_InitialEnvironment);
    return true;
  }

  /// \error Unsupported Condition
  Error("Unsupported Condition");
  return false;
}

bool SEConditionManager::HasChronicAnemia() const
{
  return m_Anemia != nullptr;
}
SEChronicAnemia* SEConditionManager::GetChronicAnemia() const
{
  return m_Anemia;
}

bool SEConditionManager::HasChronicObstructivePulmonaryDisease() const
{
  return m_COPD != nullptr;
}
SEChronicObstructivePulmonaryDisease* SEConditionManager::GetChronicObstructivePulmonaryDisease() const
{
  return m_COPD;
}

bool SEConditionManager::HasChronicHeartFailure() const
{
  return m_HeartFailure != nullptr;
}
bool SEConditionManager::HasChronicVentricularSystolicDysfunction() const
{
  return m_HeartFailure == nullptr ? false : dynamic_cast<SEChronicVentricularSystolicDysfunction*>(m_HeartFailure) != nullptr;
}
SEChronicVentricularSystolicDysfunction* SEConditionManager::GetChronicVentricularSystolicDysfunction() const
{
  return dynamic_cast<SEChronicVentricularSystolicDysfunction*>(m_HeartFailure);
}

bool SEConditionManager::HasChronicPericardialEffusion() const
{
  return m_PericardialEffusion != nullptr;
}
SEChronicPericardialEffusion* SEConditionManager::GetChronicPericardialEffusion() const
{
  return m_PericardialEffusion;
}

bool SEConditionManager::HasChronicRenalStenosis() const
{
  return m_RenalStenosis != nullptr;
}
SEChronicRenalStenosis* SEConditionManager::GetChronicRenalStenosis() const
{
  return m_RenalStenosis;
}

bool SEConditionManager::HasDehydration() const
{
  return m_Dehydration != nullptr;
}
SEDehydration* SEConditionManager::GetDehydration() const
{
  return m_Dehydration;
}

bool SEConditionManager::HasDiabetesType1() const
{
  return m_DiabetesType1 != nullptr;
}
SEDiabetesType1* SEConditionManager::GetDiabetesType1() const
{
  return m_DiabetesType1;
}

bool SEConditionManager::HasDiabetesType2() const
{
  return m_DiabetesType2 != nullptr;
}
SEDiabetesType2* SEConditionManager::GetDiabetesType2() const
{
  return m_DiabetesType2;
}

bool SEConditionManager::HasStarvation() const
{
  return m_Starvation != nullptr;
}
SEStarvation* SEConditionManager::GetStarvation() const
{
  return m_Starvation;
}

bool SEConditionManager::HasImpairedAlveolarExchange() const
{
  return m_ImpairedAlveolarExchange == nullptr ? false : true;
}
SEImpairedAlveolarExchange* SEConditionManager::GetImpairedAlveolarExchange() const
{
  return m_ImpairedAlveolarExchange;
}

bool SEConditionManager::HasLobarPneumonia() const
{
  return m_LobarPneumonia != nullptr;
}
SELobarPneumonia* SEConditionManager::GetLobarPneumonia() const
{
  return m_LobarPneumonia;
}

bool SEConditionManager::HasInitialEnvironment() const
{
  return m_InitialEnvironment != nullptr;
}
SEInitialEnvironment* SEConditionManager::GetInitialEnvironment() const
{
  return m_InitialEnvironment;
}
}