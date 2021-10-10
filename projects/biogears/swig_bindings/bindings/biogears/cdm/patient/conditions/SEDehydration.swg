%{
#include <biogears/cdm/patient/conditions/SEDehydration.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/patient/conditions/SEPatientCondition.swg>

%import <biogears/cdm/properties/SEScalar0To1.swg>

%nspace biogears::SEDehydration;

%ignore biogears::SEDehydration::Unload;
%ignore biogears::SEDehydration::Load;


%include <biogears/cdm/patient/conditions/SEDehydration.h>
