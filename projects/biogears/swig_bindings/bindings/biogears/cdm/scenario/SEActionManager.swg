%{
#include <biogears/cdm/scenario/SEActionManager.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/scenario/SEAction.swg>
// %import <biogears/cdm/scenario/SEAnesthesiaMachineActionCollection.swg>
%import <biogears/cdm/scenario/SEEnvironmentActionCollection.swg>
// %import <biogears/cdm/scenario/SEInhalerActionCollection.swg>
%import <biogears/cdm/scenario/SEPatientActionCollection.swg>

%nspace biogears::SEActionManager;

%ignore biogears::SEActionManager::GetAction(const std::string& name) const;
%ignore biogears::SEActionManager::ReadActionFile(const std::string& name);
%ignore biogears::SEActionManager::GetCompound(const std::string& name) const;

// %ignore biogears::SEActionManager::GetEnvironmentActions();
// %ignore biogears::SEActionManager::GetPatientActions();
%ignore biogears::SEActionManager::GetAnesthesiaMachineActions();
%ignore biogears::SEActionManager::GetInhalerActions();

%ignore biogears::SEActionManager::ProcessAction(const CDM::ActionData& in);
%ignore biogears::SEActionManager::Load;
%ignore biogears::SEActionManager::Unload;

%include <biogears/cdm/scenario/SEActionManager.h>


