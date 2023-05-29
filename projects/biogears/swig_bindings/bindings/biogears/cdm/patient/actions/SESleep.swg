%{
#include <biogears/cdm/patient/actions/SESleep.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SESleep;

%ignore BIOGEARS_NAMESPACE SESleep::Unload;
%ignore BIOGEARS_NAMESPACE SESleep::Load;

%include <biogears/cdm/patient/actions/SESleep.h>
