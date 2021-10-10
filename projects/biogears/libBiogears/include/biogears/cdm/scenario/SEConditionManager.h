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

#pragma once
#include <biogears/exports.h>

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/patient/conditions/SEChronicAnemia.h>
#include <biogears/cdm/patient/conditions/SEChronicObstructivePulmonaryDisease.h>
#include <biogears/cdm/patient/conditions/SEChronicPericardialEffusion.h>
#include <biogears/cdm/patient/conditions/SEChronicRenalStenosis.h>
#include <biogears/cdm/patient/conditions/SEChronicVentricularSystolicDysfunction.h>
#include <biogears/cdm/patient/conditions/SEDehydration.h>
#include <biogears/cdm/patient/conditions/SEDiabetesType1.h>
#include <biogears/cdm/patient/conditions/SEDiabetesType2.h>
#include <biogears/cdm/patient/conditions/SEImpairedAlveolarExchange.h>
#include <biogears/cdm/patient/conditions/SELobarPneumonia.h>
#include <biogears/cdm/patient/conditions/SEPatientCondition.h>
#include <biogears/cdm/patient/conditions/SEStarvation.h>
#include <biogears/cdm/system/environment/conditions/SEInitialEnvironment.h>

#include <biogears/schema/cdm/Scenario.hxx>
CDM_BIND_DECL(ConditionData);

namespace biogears {

class BIOGEARS_API SEConditionManager : public Loggable {
public:
  SEConditionManager(SESubstanceManager&);
  ~SEConditionManager();

  void Clear();

  void Unload(std::vector<CDM::ConditionData*>& to);

  bool ProcessCondition(const SECondition& condition);
  bool ProcessCondition(const CDM::ConditionData& condition);

  // Not too many conditions, so just have one manager
  // If we start getting alot, I will make patient/environment/equipment condition managers, like the action managers

  // Patient Conditions

  bool HasChronicAnemia() const;
  SEChronicAnemia* GetChronicAnemia() const;

  bool HasChronicObstructivePulmonaryDisease() const;
  SEChronicObstructivePulmonaryDisease* GetChronicObstructivePulmonaryDisease() const;

  bool HasChronicHeartFailure() const;
  bool HasChronicVentricularSystolicDysfunction() const;
  SEChronicVentricularSystolicDysfunction* GetChronicVentricularSystolicDysfunction() const;

  bool HasChronicPericardialEffusion() const;
  SEChronicPericardialEffusion* GetChronicPericardialEffusion() const;

  bool HasChronicRenalStenosis() const;
  SEChronicRenalStenosis* GetChronicRenalStenosis() const;

  bool HasStarvation() const;
  SEStarvation* GetStarvation() const;

  bool HasDehydration() const;
  SEDehydration* GetDehydration() const;

  bool HasDiabetesType1() const;
  SEDiabetesType1* GetDiabetesType1() const;

  bool HasDiabetesType2() const;
  SEDiabetesType2* GetDiabetesType2() const;

  bool HasImpairedAlveolarExchange() const;
  SEImpairedAlveolarExchange* GetImpairedAlveolarExchange() const;

  bool HasLobarPneumonia() const;
  SELobarPneumonia* GetLobarPneumonia() const;

  // Environment Conditions

  bool HasInitialEnvironment() const;
  SEInitialEnvironment* GetInitialEnvironment() const;

protected:
  SESubstanceManager& m_Substances;
  SEChronicAnemia* m_Anemia;
  SEStarvation* m_Starvation;
  SEChronicObstructivePulmonaryDisease* m_COPD;
  SEDehydration* m_Dehydration;
  SEDiabetesType1* m_DiabetesType1;
  SEDiabetesType2* m_DiabetesType2;
  SEChronicHeartFailure* m_HeartFailure;
  SEImpairedAlveolarExchange* m_ImpairedAlveolarExchange;
  SEChronicPericardialEffusion* m_PericardialEffusion;
  SELobarPneumonia* m_LobarPneumonia;
  SEChronicRenalStenosis* m_RenalStenosis;

  SEInitialEnvironment* m_InitialEnvironment;

  std::vector<SECondition*> m_Conditions;
  std::stringstream m_ss;
};
}