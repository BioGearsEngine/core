%{
#include <biogears/cdm/compartment/substances/SESubstanceQuantity.h>
%}
%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/properties/SEScalar.swg>

%nspace biogears::SESubstanceQuantity;

%ignore biogears::SESubstanceQuantity::Unload;
%ignore biogears::SESubstanceQuantity::Load;

%ignore GetScalar(const std::string& name);

%include <biogears/cdm/compartment/substances/SESubstanceQuantity.h>
