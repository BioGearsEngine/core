%{
#include <biogears/cdm/scenario/SEConditionManager.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/scenario/SECondition.swg>
%import <biogears/cdm/substance/SESubstanceManager.swg>

%import <biogears/cdm/patient/conditions/SEChronicAnemia.swg>
%import <biogears/cdm/patient/conditions/SEChronicHeartFailure.swg>
%import <biogears/cdm/patient/conditions/SEChronicObstructivePulmonaryDisease.swg>
%import <biogears/cdm/patient/conditions/SEChronicPericardialEffusion.swg>
%import <biogears/cdm/patient/conditions/SEChronicRenalStenosis.swg>
%import <biogears/cdm/patient/conditions/SEChronicVentricularSystolicDysfunction.swg>
%import <biogears/cdm/patient/conditions/SEDehydration.swg>
%import <biogears/cdm/patient/conditions/SEDiabetesType1.swg>
%import <biogears/cdm/patient/conditions/SEDiabetesType2.swg>
%import <biogears/cdm/patient/conditions/SEImpairedAlveolarExchange.swg>
%import <biogears/cdm/patient/conditions/SELobarPneumonia.swg>
%import <biogears/cdm/patient/conditions/SEPatientCondition.swg>
%import <biogears/cdm/patient/conditions/SEStarvation.swg>
%import <biogears/cdm/system/environment/conditions/SEInitialEnvironment.swg>

%nspace biogears::SEConditionManager;

%ignore biogears::SEConditionManager::Unload;
%ignore biogears::SEConditionManager::Load;
%ignore biogears::SEConditionManager::ProcessCondition(const CDM::ConditionData& condition);

%ignore biogears::SEConditionManager::GetCondition(const std::string& name) const;
%ignore biogears::SEConditionManager::ReadConditionFile(const std::string& name);
%ignore biogears::SEConditionManager::GetCompound(const std::string& name) const;

%include <biogears/cdm/scenario/SEConditionManager.h>


