%{
#include <biogears/cdm/scenario/SEActionManager.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/scenario/SECondition.h>
#include <biogears/cdm/system/environment/actions/SEEnvironmentAction.h>
#include <biogears/cdm/system/environment/conditions/SEEnvironmentCondition.h>
%}

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) SEActionManager
#endif

%include <biogears/exports.swg>

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


