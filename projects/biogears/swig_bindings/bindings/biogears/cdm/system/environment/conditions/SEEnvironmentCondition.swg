%{
#include <biogears/cdm/system/environment/conditions/SEEnvironmentCondition.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/scenario/SECondition.swg>
%import <biogears/cdm/substance/SESubstanceManager.swg>

%nspace biogears::SEEnvironmentCondition;

%ignore biogears::SEEnvironmentCondition::Unload;
%ignore biogears::SEEnvironmentCondition::Load;

%ignore SEEnvironmentCondition::ToString(std::ostream& str) const;

%include <biogears/cdm/system/environment/conditions/SEEnvironmentCondition.h>