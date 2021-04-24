%{
#include <biogears/cdm/patient/conditions/SEChronicAnemia.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/patient/conditions/SEPatientCondition.swg>

%import <biogears/cdm/properties/SEScalarArea.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>

%nspace biogears::SEChronicAnemia;

%ignore biogears::SEChronicAnemia::Unload;
%ignore biogears::SEChronicAnemia::Load;


%include <biogears/cdm/patient/conditions/SEChronicAnemia.h>
