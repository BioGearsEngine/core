

%{
#include <biogears/cdm/patient/actions/SECardiacArrest.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace biogears::SECardiacArrest;

%ignore biogears::SECardiacArrest::Unload;
%ignore biogears::SECardiacArrest::Load;

%include <biogears/cdm/patient/actions/SECardiacArrest.h>
