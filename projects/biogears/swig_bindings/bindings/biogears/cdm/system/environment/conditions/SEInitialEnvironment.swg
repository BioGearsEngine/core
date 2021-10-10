%{
#include <biogears/cdm/system/environment/conditions/SEInitialEnvironment.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/system/environment/conditions/SEEnvironmentCondition.swg>
%import <biogears/cdm/system/environment/SEEnvironmentalConditions.swg>

%nspace biogears::SEInitialEnvironment;

%ignore biogears::SEInitialEnvironment::Unload;
%ignore biogears::SEInitialEnvironment::Load;
%ignore biogears::SEInitialEnvironment::SetConditionsFile(const std::string& fileName);

%rename (GetConditions_const) biogears::SEInitialEnvironment::GetConditions() const;
%ignore SEInitialEnvironment::ToString(std::ostream& str) const;

%include <biogears/cdm/system/environment/conditions/SEInitialEnvironment.h>