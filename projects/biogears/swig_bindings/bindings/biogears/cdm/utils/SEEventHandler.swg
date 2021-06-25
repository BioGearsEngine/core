%feature("director") SEEventHandler; 

%{
#include <biogears/cdm/utils/SEEventHandler.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/properties/SEScalarTime.swg>

%nspace biogears::SEEventHandler;

%ignore biogears::SEBreathHold::Unload;
%ignore biogears::SEBreathHold::Load;

%include <biogears/cdm/utils/SEEventHandler.h>
