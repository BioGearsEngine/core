%{
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/properties/SEScalar.swg>

%nspace BIOGEARS_NAMESPACE SESubstanceTissuePharmacokinetics;

%ignore BIOGEARS_NAMESPACE SESubstanceTissuePharmacokinetics::GetScalar(const std::string& name);
%ignore BIOGEARS_NAMESPACE SESubstanceTissuePharmacokinetics::Unload;
%ignore BIOGEARS_NAMESPACE SESubstanceTissuePharmacokinetics::Load;

%rename (GetPartitionCoefficient_const) BIOGEARS_NAMESPACE SESubstanceTissuePharmacokinetics::GetPartitionCoefficient() const;

%include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>