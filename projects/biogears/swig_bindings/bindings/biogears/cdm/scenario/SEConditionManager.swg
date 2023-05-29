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

%nspace BIOGEARS_NAMESPACE SEConditionManager;

%ignore BIOGEARS_NAMESPACE SEConditionManager::Unload;
%ignore BIOGEARS_NAMESPACE SEConditionManager::Load;
%ignore BIOGEARS_NAMESPACE SEConditionManager::ProcessCondition(const CDM::ConditionData& condition);

%ignore BIOGEARS_NAMESPACE SEConditionManager::GetCondition(const std::string& name) const;
%ignore BIOGEARS_NAMESPACE SEConditionManager::ReadConditionFile(const std::string& name);
%ignore BIOGEARS_NAMESPACE SEConditionManager::GetCompound(const std::string& name) const;

%include <biogears/cdm/scenario/SEConditionManager.h>


