%{
#include <biogears/cdm/patient/actions/SEPulmonaryShunt.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>

%nspace biogears::SEPulmonaryShunt;

%ignore biogears::SEPulmonaryShunt::Unload;
%ignore biogears::SEPulmonaryShunt::Load;

%include <biogears/cdm/patient/actions/SEPulmonaryShunt.h>
