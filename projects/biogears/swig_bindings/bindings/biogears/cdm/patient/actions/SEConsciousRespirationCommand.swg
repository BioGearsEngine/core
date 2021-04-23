

%{
#include <biogears/cdm/patient/actions/SEConsciousRespirationCommand.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace biogears::SEConsciousRespirationCommand;

%ignore biogears::SEConsciousRespirationCommand::Unload;
%ignore biogears::SEConsciousRespirationCommand::Load;

%include <biogears/cdm/patient/actions/SEConsciousRespirationCommand.h>
