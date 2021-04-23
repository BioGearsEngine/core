
// Add necessary symbols to generated header
%{
#include <biogears/cdm/patient/actions/SEPatientAction.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/scenario/SEAction.swg>

%nspace biogears::SEPatientAction;

%ignore biogears::SEPatientAction::Unload;
%ignore biogears::SEPatientAction::Load;
%include <biogears/cdm/patient/actions/SEPatientAction.h>


