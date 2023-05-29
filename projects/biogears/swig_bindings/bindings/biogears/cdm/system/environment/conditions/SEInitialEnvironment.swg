%{
#include <biogears/cdm/system/environment/conditions/SEInitialEnvironment.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/system/environment/conditions/SEEnvironmentCondition.swg>
%import <biogears/cdm/system/environment/SEEnvironmentalConditions.swg>

%nspace BIOGEARS_NAMESPACE SEInitialEnvironment;

%ignore BIOGEARS_NAMESPACE SEInitialEnvironment::Unload;
%ignore BIOGEARS_NAMESPACE SEInitialEnvironment::Load;
%ignore BIOGEARS_NAMESPACE SEInitialEnvironment::SetConditionsFile(const std::string& fileName);

%rename (GetConditions_const) BIOGEARS_NAMESPACE SEInitialEnvironment::GetConditions() const;
%ignore SEInitialEnvironment::ToString(std::ostream& str) const;

%include <biogears/cdm/system/environment/conditions/SEInitialEnvironment.h>