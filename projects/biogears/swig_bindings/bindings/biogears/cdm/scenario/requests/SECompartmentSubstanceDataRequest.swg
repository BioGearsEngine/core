
%{
#include <biogears/cdm/scenario/requests/SECompartmentSubstanceDataRequest.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <std_map.i>

%import <biogears//cdm/substance/SESubstance.swg>
%import <biogears/cdm/scenario/requests/SECompartmentDataRequest.swg>

%nspace BIOGEARS_NAMESPACE SECompartmentSubstanceDataRequest;

%ignore Set(const std::string& cmpt, SESubstance& substance, const std::string& name, const std::string& unit = "");
%ignore Set(const std::string& cmpt, SESubstance& substance, const std::string& name, const CCompoundUnit& unit);

%include <biogears/cdm/scenario/requests/SECompartmentSubstanceDataRequest.h>
