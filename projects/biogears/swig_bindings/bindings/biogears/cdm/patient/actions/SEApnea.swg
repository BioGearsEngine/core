

%{
#include <biogears/cdm/patient/actions/SEApnea.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace biogears::SEApnea;

%ignore biogears::SEApnea::Unload;
%ignore biogears::SEApnea::Load;

%include <biogears/cdm/patient/actions/SEApnea.h>
