%{
#include <biogears/cdm/system/environment/actions/SEEnvironmentChange.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/system/environment/actions/SEEnvironmentAction.swg>
%import <biogears/cdm/system/environment/conditions/SEEnvironmentCondition.swg>
%import <biogears/cdm/system/environment/SEEnvironmentalConditions.swg>

%nspace biogears::SEEnvironmentChange;

%ignore biogears::SEEnvironmentChange::Unload;
%ignore biogears::SEEnvironmentChange::Load;

%rename (GetConditions_config) GetConditions() const;

%ignore SetConditionsFile(const std::string& name);
%ignore biogears::SEEnvironmentChange::ToString(std::ostream& str) const;


%include <biogears/cdm/system/environment/actions/SEEnvironmentChange.h>