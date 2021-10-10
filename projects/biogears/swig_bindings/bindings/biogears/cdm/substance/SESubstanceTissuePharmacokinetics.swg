%{
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/properties/SEScalar.swg>

%nspace biogears::SESubstanceTissuePharmacokinetics;

%ignore biogears::SESubstanceTissuePharmacokinetics::GetScalar(const std::string& name);
%ignore biogears::SESubstanceTissuePharmacokinetics::Unload;
%ignore biogears::SESubstanceTissuePharmacokinetics::Load;

%rename (GetPartitionCoefficient_const) biogears::SESubstanceTissuePharmacokinetics::GetPartitionCoefficient() const;

%include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>