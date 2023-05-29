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

%nspace BIOGEARS_NAMESPACE SEActionManager;

%ignore BIOGEARS_NAMESPACE SEActionManager::GetAction(const std::string& name) const;
%ignore BIOGEARS_NAMESPACE SEActionManager::ReadActionFile(const std::string& name);
%ignore BIOGEARS_NAMESPACE SEActionManager::GetCompound(const std::string& name) const;

// %ignore BIOGEARS_NAMESPACE SEActionManager::GetEnvironmentActions();
// %ignore BIOGEARS_NAMESPACE SEActionManager::GetPatientActions();
%ignore BIOGEARS_NAMESPACE SEActionManager::GetAnesthesiaMachineActions();
%ignore BIOGEARS_NAMESPACE SEActionManager::GetInhalerActions();

%ignore BIOGEARS_NAMESPACE SEActionManager::ProcessAction(const CDM::ActionData& in);
%ignore BIOGEARS_NAMESPACE SEActionManager::Load;
%ignore BIOGEARS_NAMESPACE SEActionManager::Unload;

%include <biogears/cdm/scenario/SEActionManager.h>


