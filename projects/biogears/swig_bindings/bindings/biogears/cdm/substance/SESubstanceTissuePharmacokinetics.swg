%{
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/properties/SEScalar.swg>

%nspace biogears::SubstanceConcenration;

%ignore biogears::SESubstanceTissuePharmacokinetics::GetScalar(const std::string& name);
%ignore biogears::SESubstanceTissuePharmacokinetics::Unload;
%ignore biogears::SESubstanceTissuePharmacokinetics::Load;

%import <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>