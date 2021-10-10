

%{
#include <biogears/cdm/patient/actions/SEBreathHold.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>
%import <biogears/cdm/patient/actions/SEConsciousRespirationCommand.swg>

%nspace biogears::SEBreathHold;

%ignore biogears::SEBreathHold::Unload;
%ignore biogears::SEBreathHold::Load;

%include <biogears/cdm/patient/actions/SEBreathHold.h>
