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

%nspace BIOGEARS_NAMESPACE SEThermalApplication;

%ignore BIOGEARS_NAMESPACE SEThermalApplication::Unload;
%ignore BIOGEARS_NAMESPACE SEThermalApplication::Load;

%ignore BIOGEARS_NAMESPACE SEThermalApplication::ToString(std::ostream& str) const;

%include <biogears/cdm/system/environment/actions/SEThermalApplication.h>