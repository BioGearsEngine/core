%{
#include <biogears/cdm/patient/actions/SESleep.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace biogears::SESleep;

%ignore biogears::SESleep::Unload;
%ignore biogears::SESleep::Load;

%include <biogears/cdm/patient/actions/SESleep.h>
