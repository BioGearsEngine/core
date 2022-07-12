

%{
#include <biogears/cdm/patient/actions/SEEbola.h>
%}

%import <biogears/exports.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace biogears::SEEbola;

%ignore biogears::SEEbola::Unload;
%ignore biogears::SEEbola::Load;

%include <biogears/cdm/patient/actions/SEEbola.h>
