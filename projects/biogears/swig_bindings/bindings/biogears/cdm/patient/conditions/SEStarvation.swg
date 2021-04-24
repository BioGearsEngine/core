%{
#include <biogears/cdm/patient/conditions/SEStarvation.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/patient/conditions/SEPatientCondition.swg>

%import <biogears/cdm/properties/SEScalarTime.swg>

%nspace biogears::SEStarvation;

%ignore biogears::SEStarvation::Unload;
%ignore biogears::SEStarvation::Load;


%include <biogears/cdm/patient/conditions/SEStarvation.h>
