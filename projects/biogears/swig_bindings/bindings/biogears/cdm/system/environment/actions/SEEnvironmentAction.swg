%{
#include <biogears/cdm/system/environment/actions/SEEnvironmentAction.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/scenario/SEAction.swg>
%import <biogears/cdm/substance/SESubstanceManager.swg>

%nspace biogears::SEEnvironmentAction;

%ignore biogears::SEEnvironmentAction::Unload;
%ignore biogears::SEEnvironmentAction::Load;

%ignore biogears::SEEnvironmentAction::newFromBind(const CDM::ActionData& action, SESubstanceManager& substances);
%ignore biogears::SEEnvironmentAction::SetComment(const std::string& name);

%ignore operator<<(std::ostream& out, const SEAction& a);

%ignore SEEnvironmentAction::ToString(std::ostream& str) const;

%include <biogears/cdm/system/environment/actions/SEEnvironmentAction.h>

