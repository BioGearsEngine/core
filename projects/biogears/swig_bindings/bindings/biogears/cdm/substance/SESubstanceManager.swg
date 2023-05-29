%{
#include <biogears/cdm/substance/SESubstanceManager.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/substance/SESubstanceCompound.swg>

%nspace BIOGEARS_NAMESPACE SESubstanceManager;

%ignore BIOGEARS_NAMESPACE SESubstanceManager::GetSubstance(const std::string& name) const;
%ignore BIOGEARS_NAMESPACE SESubstanceManager::ReadSubstanceFile(const std::string& name);
%ignore BIOGEARS_NAMESPACE SESubstanceManager::GetCompound(const std::string& name) const;
%include <biogears/cdm/substance/SESubstanceManager.h>


