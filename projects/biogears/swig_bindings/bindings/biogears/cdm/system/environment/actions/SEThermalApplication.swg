%{
#include <biogears/cdm/system/environment/actions/SEThermalApplication.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/system/environment/actions/SEEnvironmentAction.swg>
%import <biogears/cdm/system/environment/conditions/SEEnvironmentCondition.swg>
%import <biogears/cdm/system/environment/SEActiveCooling.swg>
%import <biogears/cdm/system/environment/SEActiveHeating.swg>
%import <biogears/cdm/system/environment/SEAppliedTemperature.swg>

%nspace biogears::SEThermalApplication;

%ignore biogears::SEThermalApplication::Unload;
%ignore biogears::SEThermalApplication::Load;

%ignore biogears::SEThermalApplication::ToString(std::ostream& str) const;

%include <biogears/cdm/system/environment/actions/SEThermalApplication.h>